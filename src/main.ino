#include "../include/config.h"
#include "../include/distance_hcsr04.h"
#include "../include/pid_controller.h"
#include "../include/motor_driver.h"
#include "../include/safety.h"
#include "../include/metrics.h"

// ===================== Objetos globales =====================
DistanceHcsr04 distanceSensor(HCSR04_TRIG_PIN, HCSR04_ECHO_PIN, HCSR04_TIMEOUT_US);
PidController pidDist(
  PID_DIST_KP, PID_DIST_KI, PID_DIST_KD,
  -(float)SERVO_MAX_DELTA_DEG, (float)SERVO_MAX_DELTA_DEG
);

MotorDriver motors(
  SERVO_Y_FRONT_PIN, SERVO_Y_REAR_PIN,
  SERVO_Z_LEFT_PIN, SERVO_Z_RIGHT_PIN
);

// Reutilizado como corte por E-STOP y chequeo de límites.
// Para ángulos, enviamos "error equivalente" en ambos ejes para usar el mismo guard.
SafetyManager safety(ESTOP_PIN, MAX_SAFE_ERROR_CM);
MetricsCollector metrics;

// ===================== Tiempo =====================
uint32_t lastControlMs = 0;
uint32_t startMs = 0;

// ===================== Utilidad =====================
float getTimeSec() {
  return (millis() - startMs) / 1000.0f;
}

void printHeader() {
  Serial.println("INFO,Robot Balanceador de Cargas - Prototipo 1 (Arduino Uno)");
  Serial.println("INFO,Formato CSV: DATA,time_s,dist_cm,error_cm,servoYF,servoYR,servoZL,servoZR");
}

void setup() {
  Serial.begin(115200);
  delay(700);

  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, LOW);

  printHeader();

  distanceSensor.begin();
  motors.begin();
  safety.begin();
  metrics.reset();

  startMs = millis();
  lastControlMs = millis();

  Serial.println("EVENT,BOOT,ok");
}

static bool getDistanceSample(DistanceData &dd, uint32_t nowMs) {
  if (!SIMULATION_MODE) {
    bool updated = distanceSensor.update();
    dd = distanceSensor.getData();
    return updated && dd.valid;
  }

  // Modo simulación por compilación (sin Node/hardware adicional).
  // Mantiene formato de telemetría igual al modo real.
  float t = (nowMs - startMs) / 1000.0f;
  float dist = SIM_BASE_DISTANCE_CM;

  if (SIM_SCENARIO == 0) { // nominal
    dist = TARGET_DISTANCE_CM + 1.2f * exp(-t / 2.5f) * sin(2.2f * t);
  } else if (SIM_SCENARIO == 1) { // perturbación
    float burst = (t > 3.0f && t < 4.5f) ? 3.5f * sin(8.0f * t) : 0.0f;
    dist = TARGET_DISTANCE_CM + 1.8f * exp(-t / 4.0f) * sin(2.0f * t) + burst;
  } else if (SIM_SCENARIO == 2) { // saturación
    dist = TARGET_DISTANCE_CM + 8.0f * sin(1.4f * t);
  } else if (SIM_SCENARIO == 3) { // fallo intermitente
    if (t > 2.0f && t < 2.8f) {
      dd.valid = false;
      dd.distanceCm = 0.0f;
      return false;
    }
    dist = TARGET_DISTANCE_CM + 1.6f * sin(2.0f * t);
  }

  // Ruido determinístico simple
  float noise = SIM_NOISE_AMPLITUDE_CM * sin(17.0f * t);
  dd.distanceCm = dist + noise;
  dd.valid = true;
  return true;
}

void loop() {
  uint32_t nowMs = millis();
  if (nowMs - lastControlMs < CONTROL_PERIOD_MS) {
    return;
  }
  lastControlMs = nowMs;

  DistanceData dd;
  bool sensorsHealthy = getDistanceSample(dd, nowMs);
  float errorCm = TARGET_DISTANCE_CM - dd.distanceCm;

  float absErr = errorCm >= 0.0f ? errorCm : -errorCm;
  bool safe = safety.update(sensorsHealthy, absErr, absErr);

  float controlY = 0.0f;
  float controlZ = 0.0f;

  if (safe) {
    float pidOut = pidDist.update(TARGET_DISTANCE_CM, dd.distanceCm, CONTROL_PERIOD_MS / 1000.0f);
    controlY = pidOut * AXIS_MIX_Y_GAIN;
    controlZ = pidOut * AXIS_MIX_Z_GAIN;

    motors.setOutput(controlY, controlZ, SERVO_MAX_DELTA_DEG);
    digitalWrite(STATUS_LED_PIN, HIGH);
  } else {
    motors.stopAll();
    pidDist.reset();

    digitalWrite(STATUS_LED_PIN, LOW);
    Serial.print("FAULT,");
    Serial.println(safety.getLastFault());
  }

  float tSec = getTimeSec();
  metrics.update(
    TARGET_DISTANCE_CM,
    dd.distanceCm,
    motors.getYFrontDeg(), motors.getYRearDeg(),
    motors.getZLeftDeg(), motors.getZRightDeg(),
    SERVO_CENTER_DEG, SERVO_MAX_DELTA_DEG,
    STABLE_ERROR_BAND_CM, tSec
  );

  metrics.printCsvLine(
    tSec,
    dd.distanceCm,
    errorCm,
    motors.getYFrontDeg(), motors.getYRearDeg(),
    motors.getZLeftDeg(), motors.getZRightDeg()
  );

  // Resumen cada ~1s
  static uint32_t lastReportMs = 0;
  if (nowMs - lastReportMs >= 1000) {
    lastReportMs = nowMs;
    ControlMetrics m = metrics.getMetrics();

    Serial.print("METRIC,");
    Serial.print("rms_cm="); Serial.print(m.rmsErrorCm, 3); Serial.print(",");
    Serial.print("max_cm="); Serial.print(m.maxAbsErrorCm, 3); Serial.print(",");
    Serial.print("settle_s="); Serial.print(m.settlingTimeSec, 3); Serial.print(",");
    Serial.print("sat_pct="); Serial.print(m.actuatorSaturationPct, 2); Serial.print(",");
    Serial.print("samples="); Serial.println(m.totalSamples);
  }
}

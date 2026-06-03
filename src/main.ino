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

MotorDriver motors(SERVO_MAIN_PIN);

// Reutilizado como corte por E-STOP y chequeo de límites.
SafetyManager safety(ESTOP_PIN, MAX_SAFE_ERROR_CM);
MetricsCollector metrics;

// ===================== Tiempo =====================
uint32_t lastControlMs = 0;
uint32_t startMs = 0;

// ===================== Estado operación =====================
bool automaticMode = DEFAULT_AUTOMATIC_MODE;
int manualServoDeg = SERVO_CENTER_DEG;

// ===================== Utilidad =====================
float getTimeSec() {
  return (millis() - startMs) / 1000.0f;
}

float getSimWeightGrams(float tSec) {
  float w = DEFAULT_SIM_WEIGHT_GRAMS + 0.8f * sin(0.7f * tSec);
  if (w < MIN_LOAD_GRAMS) return MIN_LOAD_GRAMS;
  return w;
}

void printHeader() {
  Serial.println("INFO,Robot Balanceador de Cargas - Prototipo 1 (Arduino Uno)");
  Serial.println("INFO,Formato CSV: DATA,time_s,dist_cm,error_cm,servoMain,latency_ms,weight_g,mode");
  Serial.println("INFO,Comandos: MODE,AUTO | MODE,MANUAL | MANUAL,<deg 0-180> | STATUS");
}

void printStatus(float weightGrams) {
  Serial.print("EVENT,STATUS,mode=");
  Serial.print(automaticMode ? "AUTO" : "MANUAL");
  Serial.print(",servo=");
  Serial.print(motors.getMainDeg());
  Serial.print(",minLoad_g=");
  Serial.print(MIN_LOAD_GRAMS, 2);
  Serial.print(",weight_g=");
  Serial.println(weightGrams, 2);
}

void processSerialCommands(float weightGrams) {
  if (!Serial.available()) return;

  String line = Serial.readStringUntil('\n');
  line.trim();

  if (line.length() == 0) return;

  if (line == "MODE,AUTO") {
    automaticMode = true;
    pidDist.reset();
    Serial.println("EVENT,MODE,auto");
    return;
  }

  if (line == "MODE,MANUAL") {
    automaticMode = false;
    motors.setManualAngle(manualServoDeg);
    Serial.println("EVENT,MODE,manual");
    return;
  }

  if (line == "STATUS") {
    printStatus(weightGrams);
    return;
  }

  if (line.startsWith("MANUAL,")) {
    int comma = line.indexOf(',');
    if (comma > 0 && comma < (int)line.length() - 1) {
      String v = line.substring(comma + 1);
      int deg = v.toInt();
      if (deg < SERVO_MIN_DEG) deg = SERVO_MIN_DEG;
      if (deg > SERVO_MAX_DEG) deg = SERVO_MAX_DEG;
      manualServoDeg = deg;

      if (!automaticMode) {
        motors.setManualAngle(manualServoDeg);
      }

      Serial.print("EVENT,MANUAL,deg=");
      Serial.println(manualServoDeg);
      return;
    }
  }

  Serial.print("EVENT,CMD,unknown:");
  Serial.println(line);
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

  float noise = SIM_NOISE_AMPLITUDE_CM * sin(17.0f * t);
  dd.distanceCm = dist + noise;
  dd.valid = true;
  return true;
}

void loop() {
  uint32_t nowMs = millis();
  float tSec = getTimeSec();
  float weightGrams = getSimWeightGrams(tSec);

  processSerialCommands(weightGrams);

  if (nowMs - lastControlMs < CONTROL_PERIOD_MS) {
    return;
  }
  uint32_t dtMs = nowMs - lastControlMs;
  lastControlMs = nowMs;

  DistanceData dd;
  bool sensorsHealthy = getDistanceSample(dd, nowMs);
  float errorCm = TARGET_DISTANCE_CM - dd.distanceCm;

  float absErr = errorCm >= 0.0f ? errorCm : -errorCm;
  bool safe = safety.update(sensorsHealthy, absErr, absErr);

  float controlMain = 0.0f;

  if (safe) {
    if (automaticMode) {
      float pidOut = pidDist.update(TARGET_DISTANCE_CM, dd.distanceCm, CONTROL_PERIOD_MS / 1000.0f);
      controlMain = pidOut * AXIS_MIX_MAIN_GAIN;
      motors.setOutput(controlMain, SERVO_MAX_DELTA_DEG);
    } else {
      motors.setManualAngle(manualServoDeg);
    }
    digitalWrite(STATUS_LED_PIN, HIGH);
  } else {
    motors.stopAll();
    pidDist.reset();

    digitalWrite(STATUS_LED_PIN, LOW);
    Serial.print("FAULT,");
    Serial.println(safety.getLastFault());
  }

  float latencyMs = (float)dtMs;
  metrics.update(
    TARGET_DISTANCE_CM,
    dd.distanceCm,
    motors.getMainDeg(),
    SERVO_CENTER_DEG, SERVO_MAX_DELTA_DEG,
    STABLE_ERROR_BAND_CM, tSec
  );

  metrics.printCsvLine(
    tSec,
    dd.distanceCm,
    errorCm,
    motors.getMainDeg(),
    latencyMs,
    weightGrams,
    automaticMode
  );

  static uint32_t lastReportMs = 0;
  if (nowMs - lastReportMs >= 1000) {
    lastReportMs = nowMs;
    ControlMetrics m = metrics.getMetrics();

    Serial.print("METRIC,");
    Serial.print("rms_cm="); Serial.print(m.rmsErrorCm, 3); Serial.print(",");
    Serial.print("max_cm="); Serial.print(m.maxAbsErrorCm, 3); Serial.print(",");
    Serial.print("settle_s="); Serial.print(m.settlingTimeSec, 3); Serial.print(",");
    Serial.print("sat_pct="); Serial.print(m.actuatorSaturationPct, 2); Serial.print(",");
    Serial.print("samples="); Serial.print(m.totalSamples); Serial.print(",");
    Serial.print("mode="); Serial.print(automaticMode ? "AUTO" : "MANUAL"); Serial.print(",");
    Serial.print("servo="); Serial.print(motors.getMainDeg()); Serial.print(",");
    Serial.print("weight_g="); Serial.println(weightGrams, 2);
  }
}

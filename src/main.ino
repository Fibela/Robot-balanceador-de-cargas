#include "../include/config.h"
#include "../include/imu_mpu6050.h"
#include "../include/loadcell_hx711.h"
#include "../include/pid_controller.h"
#include "../include/motor_driver.h"
#include "../include/safety.h"
#include "../include/metrics.h"

// ===================== Objetos globales =====================
ImuMpu6050 imu;
LoadCellHx711 loadcells(
  HX711_LEFT_DT_PIN, HX711_LEFT_SCK_PIN,
  HX711_RIGHT_DT_PIN, HX711_RIGHT_SCK_PIN
);

PidController pidX(PID_X_KP, PID_X_KI, PID_X_KD, -MAX_PWM_OUTPUT, MAX_PWM_OUTPUT);
PidController pidY(PID_Y_KP, PID_Y_KI, PID_Y_KD, -MAX_PWM_OUTPUT, MAX_PWM_OUTPUT);

MotorDriver motors(
  MOTOR_LEFT_PWM_PIN, MOTOR_LEFT_DIR_PIN,
  MOTOR_RIGHT_PWM_PIN, MOTOR_RIGHT_DIR_PIN
);

SafetyManager safety(ESTOP_PIN, MAX_SAFE_ANGLE_DEG);
MetricsCollector metrics;

// ===================== Tiempo =====================
uint32_t lastControlMs = 0;
uint32_t startMs = 0;

// ===================== Utilidad =====================
float getTimeSec() {
  return (millis() - startMs) / 1000.0f;
}

void printHeader() {
  Serial.println("INFO,Robot Balanceador de Cargas - Inicio");
  Serial.println("INFO,Formato CSV: DATA,time,angleX,angleY,loadL,loadR,ctrlX,ctrlY");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  printHeader();

  bool imuOk = imu.begin();
  bool lcOk = loadcells.begin(
    HX711_LEFT_SCALE, HX711_RIGHT_SCALE,
    HX711_LEFT_OFFSET, HX711_RIGHT_OFFSET
  );

  motors.begin();
  safety.begin();
  metrics.reset();

  startMs = millis();
  lastControlMs = millis();

  if (!imuOk) Serial.println("WARN,IMU no detectada");
  if (!lcOk) Serial.println("WARN,HX711 no inicializado");
}

void loop() {
  uint32_t nowMs = millis();
  if (nowMs - lastControlMs < CONTROL_PERIOD_MS) {
    return;
  }

  float dtSec = (nowMs - lastControlMs) / 1000.0f;
  lastControlMs = nowMs;

  bool imuUpdated = imu.update(dtSec);
  bool lcUpdated = loadcells.update();

  ImuData id = imu.getData();
  LoadCellData ld = loadcells.getData();

  bool sensorsHealthy = imuUpdated && lcUpdated && id.valid && ld.valid;
  bool safe = safety.update(sensorsHealthy, id.angleXDeg, id.angleYDeg);

  float ctrlX = 0.0f;
  float ctrlY = 0.0f;

  if (safe) {
    ctrlX = pidX.update(TARGET_ANGLE_X_DEG, id.angleXDeg, dtSec);
    ctrlY = pidY.update(TARGET_ANGLE_Y_DEG, id.angleYDeg, dtSec);

    motors.setOutput(ctrlX, ctrlY, MAX_PWM_OUTPUT);
  } else {
    motors.stopAll();
    pidX.reset();
    pidY.reset();

    Serial.print("FAULT,");
    Serial.println(safety.getLastFault());
  }

  float tSec = getTimeSec();
  metrics.update(
    TARGET_ANGLE_X_DEG, TARGET_ANGLE_Y_DEG,
    id.angleXDeg, id.angleYDeg,
    motors.getLeftPwm(), motors.getRightPwm(),
    MAX_PWM_OUTPUT, tSec
  );

  metrics.printCsvLine(
    tSec,
    id.angleXDeg, id.angleYDeg,
    ld.leftKg, ld.rightKg,
    ctrlX, ctrlY
  );

  // Resumen cada ~1s
  static uint32_t lastReportMs = 0;
  if (nowMs - lastReportMs >= 1000) {
    lastReportMs = nowMs;
    ControlMetrics m = metrics.getMetrics();

    Serial.print("METRICS,");
    Serial.print("rmsX="); Serial.print(m.rmsErrorX, 3); Serial.print(",");
    Serial.print("rmsY="); Serial.print(m.rmsErrorY, 3); Serial.print(",");
    Serial.print("maxX="); Serial.print(m.maxAbsErrorX, 3); Serial.print(",");
    Serial.print("maxY="); Serial.print(m.maxAbsErrorY, 3); Serial.print(",");
    Serial.print("settleX="); Serial.print(m.settlingTimeXSec, 3); Serial.print(",");
    Serial.print("settleY="); Serial.print(m.settlingTimeYSec, 3); Serial.print(",");
    Serial.print("satPct="); Serial.print(m.actuatorSaturationPct, 2); Serial.print(",");
    Serial.print("samples="); Serial.println(m.totalSamples);
  }
}

#include "../include/motor_driver.h"
#include "../include/config.h"

MotorDriver::MotorDriver(uint8_t mainPin)
  : mainPin(mainPin), lastMainDeg(SERVO_CENTER_DEG) {}

int MotorDriver::clampDeg(int v) const {
  if (v < SERVO_MIN_DEG) return SERVO_MIN_DEG;
  if (v > SERVO_MAX_DEG) return SERVO_MAX_DEG;
  return v;
}

void MotorDriver::begin() {
  servoMain.attach(mainPin);
  stopAll();
}

void MotorDriver::setOutput(float controlMain, int maxDeltaDeg) {
  if (controlMain > maxDeltaDeg) controlMain = (float)maxDeltaDeg;
  if (controlMain < -maxDeltaDeg) controlMain = (float)-maxDeltaDeg;

  int mainDeg = clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_MAIN + (int)controlMain);
  servoMain.write(mainDeg);
  lastMainDeg = mainDeg;
}

void MotorDriver::setManualAngle(int targetDeg) {
  int mainDeg = clampDeg(targetDeg);
  servoMain.write(mainDeg);
  lastMainDeg = mainDeg;
}

void MotorDriver::stopAll() {
  int safeDeg = clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_MAIN);
  servoMain.write(safeDeg);
  lastMainDeg = safeDeg;
}

int MotorDriver::getMainDeg() const { return lastMainDeg; }

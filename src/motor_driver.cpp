#include "../include/motor_driver.h"
#include "../include/config.h"

MotorDriver::MotorDriver(
  uint8_t yFrontPin, uint8_t yRearPin,
  uint8_t zLeftPin, uint8_t zRightPin
)
  : yFrontPin(yFrontPin), yRearPin(yRearPin), zLeftPin(zLeftPin), zRightPin(zRightPin),
    lastYFrontDeg(SERVO_CENTER_DEG), lastYRearDeg(SERVO_CENTER_DEG),
    lastZLeftDeg(SERVO_CENTER_DEG), lastZRightDeg(SERVO_CENTER_DEG) {}

int MotorDriver::clampDeg(int v) const {
  if (v < SERVO_MIN_DEG) return SERVO_MIN_DEG;
  if (v > SERVO_MAX_DEG) return SERVO_MAX_DEG;
  return v;
}

void MotorDriver::begin() {
  servoYFront.attach(yFrontPin);
  servoYRear.attach(yRearPin);
  servoZLeft.attach(zLeftPin);
  servoZRight.attach(zRightPin);
  stopAll();
}

void MotorDriver::setOutput(float controlY, float controlZ, int maxDeltaDeg) {
  if (controlY > maxDeltaDeg) controlY = (float)maxDeltaDeg;
  if (controlY < -maxDeltaDeg) controlY = (float)-maxDeltaDeg;
  if (controlZ > maxDeltaDeg) controlZ = (float)maxDeltaDeg;
  if (controlZ < -maxDeltaDeg) controlZ = (float)-maxDeltaDeg;

  // Mezcla por eje (parejas opuestas)
  int yFront = clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Y_FRONT + (int)controlY);
  int yRear  = clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Y_REAR  - (int)controlY);
  int zLeft  = clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Z_LEFT  + (int)controlZ);
  int zRight = clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Z_RIGHT - (int)controlZ);

  servoYFront.write(yFront);
  servoYRear.write(yRear);
  servoZLeft.write(zLeft);
  servoZRight.write(zRight);

  lastYFrontDeg = yFront;
  lastYRearDeg = yRear;
  lastZLeftDeg = zLeft;
  lastZRightDeg = zRight;
}

void MotorDriver::stopAll() {
  servoYFront.write(clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Y_FRONT));
  servoYRear.write(clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Y_REAR));
  servoZLeft.write(clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Z_LEFT));
  servoZRight.write(clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Z_RIGHT));

  lastYFrontDeg = clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Y_FRONT);
  lastYRearDeg = clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Y_REAR);
  lastZLeftDeg = clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Z_LEFT);
  lastZRightDeg = clampDeg(SERVO_CENTER_DEG + SERVO_TRIM_Z_RIGHT);
}

int MotorDriver::getYFrontDeg() const { return lastYFrontDeg; }
int MotorDriver::getYRearDeg() const { return lastYRearDeg; }
int MotorDriver::getZLeftDeg() const { return lastZLeftDeg; }
int MotorDriver::getZRightDeg() const { return lastZRightDeg; }

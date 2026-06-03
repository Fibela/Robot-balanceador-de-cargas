#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>
#include <Servo.h>

class MotorDriver {
public:
  explicit MotorDriver(uint8_t mainPin);

  void begin();
  void setOutput(float controlMain, int maxDeltaDeg);
  void setManualAngle(int targetDeg);
  void stopAll();

  int getMainDeg() const;

private:
  uint8_t mainPin;
  Servo servoMain;
  int lastMainDeg;

  int clampDeg(int v) const;
};

#endif

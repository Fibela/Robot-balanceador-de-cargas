#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>
#include <Servo.h>

class MotorDriver {
public:
  MotorDriver(
    uint8_t yFrontPin, uint8_t yRearPin,
    uint8_t zLeftPin, uint8_t zRightPin
  );

  void begin();
  void setOutput(float controlY, float controlZ, int maxDeltaDeg);
  void stopAll();

  int getYFrontDeg() const;
  int getYRearDeg() const;
  int getZLeftDeg() const;
  int getZRightDeg() const;

private:
  uint8_t yFrontPin;
  uint8_t yRearPin;
  uint8_t zLeftPin;
  uint8_t zRightPin;

  Servo servoYFront;
  Servo servoYRear;
  Servo servoZLeft;
  Servo servoZRight;

  int lastYFrontDeg;
  int lastYRearDeg;
  int lastZLeftDeg;
  int lastZRightDeg;

  int clampDeg(int v) const;
};

#endif

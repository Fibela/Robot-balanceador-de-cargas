#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

class MotorDriver {
public:
  MotorDriver(uint8_t leftPwmPin, uint8_t leftDirPin, uint8_t rightPwmPin, uint8_t rightDirPin);

  void begin();
  void setOutput(float controlX, float controlY, int maxPwm);
  void stopAll();

  int getLeftPwm() const;
  int getRightPwm() const;
  bool getLeftDir() const;
  bool getRightDir() const;

private:
  uint8_t leftPwmPin;
  uint8_t leftDirPin;
  uint8_t rightPwmPin;
  uint8_t rightDirPin;

  int lastLeftPwm;
  int lastRightPwm;
  bool lastLeftDir;
  bool lastRightDir;

  void setMotor(uint8_t pwmPin, uint8_t dirPin, float command, int &amp;outPwm, bool &amp;outDir, int maxPwm);
};

#endif

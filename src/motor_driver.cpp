#include "../include/motor_driver.h"

MotorDriver::MotorDriver(uint8_t leftPwmPin, uint8_t leftDirPin, uint8_t rightPwmPin, uint8_t rightDirPin)
  : leftPwmPin(leftPwmPin), leftDirPin(leftDirPin),
    rightPwmPin(rightPwmPin), rightDirPin(rightDirPin),
    lastLeftPwm(0), lastRightPwm(0), lastLeftDir(true), lastRightDir(true) {}

void MotorDriver::begin() {
  pinMode(leftPwmPin, OUTPUT);
  pinMode(leftDirPin, OUTPUT);
  pinMode(rightPwmPin, OUTPUT);
  pinMode(rightDirPin, OUTPUT);
  stopAll();
}

void MotorDriver::setMotor(uint8_t pwmPin, uint8_t dirPin, float command, int &outPwm, bool &outDir, int maxPwm) {
  outDir = command >= 0.0f;
  float absCmd = command >= 0.0f ? command : -command;
  if (absCmd > maxPwm) absCmd = (float)maxPwm;

  outPwm = (int)absCmd;
  digitalWrite(dirPin, outDir ? HIGH : LOW);
  analogWrite(pwmPin, outPwm);
}

void MotorDriver::setOutput(float controlX, float controlY, int maxPwm) {
  // Mezcla simple para plataforma de dos actuadores:
  // Izq = X + Y, Der = X - Y
  float leftCmd = controlX + controlY;
  float rightCmd = controlX - controlY;

  setMotor(leftPwmPin, leftDirPin, leftCmd, lastLeftPwm, lastLeftDir, maxPwm);
  setMotor(rightPwmPin, rightDirPin, rightCmd, lastRightPwm, lastRightDir, maxPwm);
}

void MotorDriver::stopAll() {
  lastLeftPwm = 0;
  lastRightPwm = 0;
  analogWrite(leftPwmPin, 0);
  analogWrite(rightPwmPin, 0);
}

int MotorDriver::getLeftPwm() const {
  return lastLeftPwm;
}

int MotorDriver::getRightPwm() const {
  return lastRightPwm;
}

bool MotorDriver::getLeftDir() const {
  return lastLeftDir;
}

bool MotorDriver::getRightDir() const {
  return lastRightDir;
}

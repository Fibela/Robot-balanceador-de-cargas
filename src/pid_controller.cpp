#include "../include/pid_controller.h"

PidController::PidController(float kp, float ki, float kd, float outMin, float outMax)
  : kp(kp), ki(ki), kd(kd), outMin(outMin), outMax(outMax),
    integral(0.0f), lastError(0.0f), lastOutput(0.0f), firstRun(true) {}

void PidController::reset() {
  integral = 0.0f;
  lastError = 0.0f;
  lastOutput = 0.0f;
  firstRun = true;
}

float PidController::update(float setpoint, float measurement, float dtSec) {
  if (dtSec <= 0.0f) {
    return lastOutput;
  }

  float error = setpoint - measurement;
  integral += error * dtSec;

  float derivative = 0.0f;
  if (!firstRun) {
    derivative = (error - lastError) / dtSec;
  } else {
    firstRun = false;
  }

  float output = (kp * error) + (ki * integral) + (kd * derivative);

  if (output > outMax) output = outMax;
  if (output < outMin) output = outMin;

  lastError = error;
  lastOutput = output;
  return output;
}

float PidController::getLastError() const {
  return lastError;
}

float PidController::getIntegral() const {
  return integral;
}

float PidController::getLastOutput() const {
  return lastOutput;
}

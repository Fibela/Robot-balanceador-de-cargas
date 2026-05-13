#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

class PidController {
public:
  PidController(float kp, float ki, float kd, float outMin, float outMax);

  void reset();
  float update(float setpoint, float measurement, float dtSec);

  float getLastError() const;
  float getIntegral() const;
  float getLastOutput() const;

private:
  float kp;
  float ki;
  float kd;
  float outMin;
  float outMax;

  float integral;
  float lastError;
  float lastOutput;
  bool firstRun;
};

#endif

#ifndef SAFETY_H
#define SAFETY_H

#include <Arduino.h>

class SafetyManager {
public:
  SafetyManager(uint8_t estopPin, float maxSafeAngleDeg);

  void begin();
  bool update(bool sensorsHealthy, float angleXDeg, float angleYDeg);
  bool isEmergencyStopActive() const;
  const char* getLastFault() const;

private:
  uint8_t estopPin;
  float maxSafeAngleDeg;
  bool emergencyStopActive;
  const char* lastFault;
};

#endif

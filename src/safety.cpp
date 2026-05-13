#include "../include/safety.h"

SafetyManager::SafetyManager(uint8_t estopPin, float maxSafeAngleDeg)
  : estopPin(estopPin),
    maxSafeAngleDeg(maxSafeAngleDeg),
    emergencyStopActive(false),
    lastFault("NONE") {}

void SafetyManager::begin() {
  pinMode(estopPin, INPUT_PULLUP);
}

bool SafetyManager::update(bool sensorsHealthy, float angleXDeg, float angleYDeg) {
  if (digitalRead(estopPin) == LOW) {
    emergencyStopActive = true;
    lastFault = "ESTOP_BUTTON";
    return false;
  }

  if (!sensorsHealthy) {
    emergencyStopActive = true;
    lastFault = "SENSOR_HEALTH";
    return false;
  }

  float absX = angleXDeg >= 0.0f ? angleXDeg : -angleXDeg;
  float absY = angleYDeg >= 0.0f ? angleYDeg : -angleYDeg;
  if (absX > maxSafeAngleDeg || absY > maxSafeAngleDeg) {
    emergencyStopActive = true;
    lastFault = "ANGLE_LIMIT";
    return false;
  }

  emergencyStopActive = false;
  lastFault = "NONE";
  return true;
}

bool SafetyManager::isEmergencyStopActive() const {
  return emergencyStopActive;
}

const char* SafetyManager::getLastFault() const {
  return lastFault;
}

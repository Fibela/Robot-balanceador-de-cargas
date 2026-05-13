#include "../include/metrics.h"

MetricsCollector::MetricsCollector() {
  reset();
}

void MetricsCollector::reset() {
  sumSqErrX = 0.0f;
  sumSqErrY = 0.0f;
  maxAbsErrX = 0.0f;
  maxAbsErrY = 0.0f;

  settledX = false;
  settledY = false;
  settlingTimeXSec = -1.0f;
  settlingTimeYSec = -1.0f;

  saturationCount = 0;
  sampleCount = 0;
}

void MetricsCollector::update(
  float targetX, float targetY,
  float measuredX, float measuredY,
  int pwmLeft, int pwmRight,
  int maxPwm, float timeSec
) {
  float errX = targetX - measuredX;
  float errY = targetY - measuredY;

  float absErrX = errX >= 0.0f ? errX : -errX;
  float absErrY = errY >= 0.0f ? errY : -errY;

  sumSqErrX += errX * errX;
  sumSqErrY += errY * errY;

  if (absErrX > maxAbsErrX) maxAbsErrX = absErrX;
  if (absErrY > maxAbsErrY) maxAbsErrY = absErrY;

  const float settleBandDeg = 2.0f;
  if (!settledX && absErrX <= settleBandDeg) {
    settledX = true;
    settlingTimeXSec = timeSec;
  }
  if (!settledY && absErrY <= settleBandDeg) {
    settledY = true;
    settlingTimeYSec = timeSec;
  }

  int absPwmL = pwmLeft >= 0 ? pwmLeft : -pwmLeft;
  int absPwmR = pwmRight >= 0 ? pwmRight : -pwmRight;
  if (absPwmL >= maxPwm || absPwmR >= maxPwm) {
    saturationCount++;
  }

  sampleCount++;
}

ControlMetrics MetricsCollector::getMetrics() const {
  ControlMetrics m{};
  m.totalSamples = sampleCount;

  if (sampleCount > 0) {
    m.rmsErrorX = sqrt(sumSqErrX / (float)sampleCount);
    m.rmsErrorY = sqrt(sumSqErrY / (float)sampleCount);
    m.maxAbsErrorX = maxAbsErrX;
    m.maxAbsErrorY = maxAbsErrY;
    m.actuatorSaturationPct = 100.0f * ((float)saturationCount / (float)sampleCount);
  } else {
    m.rmsErrorX = 0.0f;
    m.rmsErrorY = 0.0f;
    m.maxAbsErrorX = 0.0f;
    m.maxAbsErrorY = 0.0f;
    m.actuatorSaturationPct = 0.0f;
  }

  m.settlingTimeXSec = settlingTimeXSec;
  m.settlingTimeYSec = settlingTimeYSec;
  return m;
}

void MetricsCollector::printCsvLine(
  float timeSec,
  float angleX, float angleY,
  float loadLeft, float loadRight,
  float controlX, float controlY
) const {
  Serial.print("DATA,");
  Serial.print(timeSec, 3); Serial.print(",");
  Serial.print(angleX, 3); Serial.print(",");
  Serial.print(angleY, 3); Serial.print(",");
  Serial.print(loadLeft, 3); Serial.print(",");
  Serial.print(loadRight, 3); Serial.print(",");
  Serial.print(controlX, 3); Serial.print(",");
  Serial.println(controlY, 3);
}

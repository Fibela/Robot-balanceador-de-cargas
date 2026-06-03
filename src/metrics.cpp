#include "../include/metrics.h"

MetricsCollector::MetricsCollector() {
  reset();
}

void MetricsCollector::reset() {
  sumSqErrCm = 0.0f;
  maxAbsErrCm = 0.0f;

  settled = false;
  settlingTimeSec = -1.0f;

  saturationCount = 0;
  sampleCount = 0;
}

void MetricsCollector::update(
  float targetDistanceCm,
  float measuredDistanceCm,
  int servoMainDeg,
  int centerDeg, int maxDeltaDeg,
  float stableBandCm, float timeSec
) {
  float err = targetDistanceCm - measuredDistanceCm;
  float absErr = err >= 0.0f ? err : -err;

  sumSqErrCm += err * err;
  if (absErr > maxAbsErrCm) maxAbsErrCm = absErr;

  if (!settled && absErr <= stableBandCm) {
    settled = true;
    settlingTimeSec = timeSec;
  }

  int d = servoMainDeg - centerDeg;
  if (d < 0) d = -d;
  if (d >= maxDeltaDeg) {
    saturationCount++;
  }

  sampleCount++;
}

ControlMetrics MetricsCollector::getMetrics() const {
  ControlMetrics m{};
  m.totalSamples = sampleCount;

  if (sampleCount > 0) {
    m.rmsErrorCm = sqrt(sumSqErrCm / (float)sampleCount);
    m.maxAbsErrorCm = maxAbsErrCm;
    m.actuatorSaturationPct = 100.0f * ((float)saturationCount / (float)sampleCount);
  } else {
    m.rmsErrorCm = 0.0f;
    m.maxAbsErrorCm = 0.0f;
    m.actuatorSaturationPct = 0.0f;
  }

  m.settlingTimeSec = settlingTimeSec;
  return m;
}

void MetricsCollector::printCsvLine(
  float timeSec,
  float distanceCm,
  float errorCm,
  int servoMainDeg,
  float latencyMs,
  float weightGrams,
  bool automaticMode
) const {
  Serial.print("DATA,");
  Serial.print(timeSec, 3); Serial.print(",");
  Serial.print(distanceCm, 3); Serial.print(",");
  Serial.print(errorCm, 3); Serial.print(",");
  Serial.print(servoMainDeg); Serial.print(",");
  Serial.print(latencyMs, 2); Serial.print(",");
  Serial.print(weightGrams, 2); Serial.print(",");
  Serial.println(automaticMode ? "AUTO" : "MANUAL");
}

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
  int servoYFrontDeg, int servoYRearDeg,
  int servoZLeftDeg, int servoZRightDeg,
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

  int d1 = servoYFrontDeg - centerDeg; if (d1 < 0) d1 = -d1;
  int d2 = servoYRearDeg - centerDeg;  if (d2 < 0) d2 = -d2;
  int d3 = servoZLeftDeg - centerDeg;  if (d3 < 0) d3 = -d3;
  int d4 = servoZRightDeg - centerDeg; if (d4 < 0) d4 = -d4;

  if (d1 >= maxDeltaDeg || d2 >= maxDeltaDeg || d3 >= maxDeltaDeg || d4 >= maxDeltaDeg) {
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
  int servoYFrontDeg, int servoYRearDeg,
  int servoZLeftDeg, int servoZRightDeg
) const {
  Serial.print("DATA,");
  Serial.print(timeSec, 3); Serial.print(",");
  Serial.print(distanceCm, 3); Serial.print(",");
  Serial.print(errorCm, 3); Serial.print(",");
  Serial.print(servoYFrontDeg); Serial.print(",");
  Serial.print(servoYRearDeg); Serial.print(",");
  Serial.print(servoZLeftDeg); Serial.print(",");
  Serial.println(servoZRightDeg);
}

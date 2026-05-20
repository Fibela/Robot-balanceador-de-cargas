#ifndef METRICS_H
#define METRICS_H

#include <Arduino.h>

struct ControlMetrics {
  float rmsErrorCm;
  float maxAbsErrorCm;
  float settlingTimeSec;
  float actuatorSaturationPct;
  uint32_t totalSamples;
};

class MetricsCollector {
public:
  MetricsCollector();

  void reset();
  void update(
    float targetDistanceCm,
    float measuredDistanceCm,
    int servoYFrontDeg, int servoYRearDeg,
    int servoZLeftDeg, int servoZRightDeg,
    int centerDeg, int maxDeltaDeg,
    float stableBandCm, float timeSec
  );

  ControlMetrics getMetrics() const;
  void printCsvLine(
    float timeSec,
    float distanceCm,
    float errorCm,
    int servoYFrontDeg, int servoYRearDeg,
    int servoZLeftDeg, int servoZRightDeg
  ) const;

private:
  float sumSqErrCm;
  float maxAbsErrCm;

  bool settled;
  float settlingTimeSec;

  uint32_t saturationCount;
  uint32_t sampleCount;
};

#endif

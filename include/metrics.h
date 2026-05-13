#ifndef METRICS_H
#define METRICS_H

#include <Arduino.h>

struct ControlMetrics {
  float rmsErrorX;
  float rmsErrorY;
  float maxAbsErrorX;
  float maxAbsErrorY;
  float settlingTimeXSec;
  float settlingTimeYSec;
  float actuatorSaturationPct;
  uint32_t totalSamples;
};

class MetricsCollector {
public:
  MetricsCollector();

  void reset();
  void update(
    float targetX, float targetY,
    float measuredX, float measuredY,
    int pwmLeft, int pwmRight,
    int maxPwm, float timeSec
  );

  ControlMetrics getMetrics() const;
  void printCsvLine(
    float timeSec,
    float angleX, float angleY,
    float loadLeft, float loadRight,
    float controlX, float controlY
  ) const;

private:
  float sumSqErrX;
  float sumSqErrY;
  float maxAbsErrX;
  float maxAbsErrY;

  bool settledX;
  bool settledY;
  float settlingTimeXSec;
  float settlingTimeYSec;

  uint32_t saturationCount;
  uint32_t sampleCount;
};

#endif

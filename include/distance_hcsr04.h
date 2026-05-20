#ifndef DISTANCE_HCSR04_H
#define DISTANCE_HCSR04_H

#include <Arduino.h>

struct DistanceData {
  float distanceCm;
  bool valid;
};

class DistanceHcsr04 {
public:
  DistanceHcsr04(uint8_t trigPin, uint8_t echoPin, uint16_t timeoutUs);

  void begin();
  bool update();
  DistanceData getData() const;
  bool isHealthy() const;

private:
  uint8_t trigPin;
  uint8_t echoPin;
  uint16_t timeoutUs;
  bool healthy;
  DistanceData data;
};

#endif

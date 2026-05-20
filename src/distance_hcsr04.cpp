#include "../include/distance_hcsr04.h"

DistanceHcsr04::DistanceHcsr04(uint8_t trigPin, uint8_t echoPin, uint16_t timeoutUs)
  : trigPin(trigPin), echoPin(echoPin), timeoutUs(timeoutUs), healthy(false) {
  data = {0.0f, false};
}

void DistanceHcsr04::begin() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  healthy = true;
  data.valid = false;
}

bool DistanceHcsr04::update() {
  if (!healthy) {
    data.valid = false;
    return false;
  }

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long durationUs = pulseIn(echoPin, HIGH, timeoutUs);
  if (durationUs == 0UL) {
    data.valid = false;
    return false;
  }

  // HC-SR04: distancia (cm) ~= durationUs * 0.0343 / 2
  data.distanceCm = (float)durationUs * 0.01715f;
  data.valid = true;
  return true;
}

DistanceData DistanceHcsr04::getData() const {
  return data;
}

bool DistanceHcsr04::isHealthy() const {
  return healthy;
}

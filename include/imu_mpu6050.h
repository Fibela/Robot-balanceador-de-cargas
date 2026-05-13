#ifndef IMU_MPU6050_H
#define IMU_MPU6050_H

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>

struct ImuData {
  float angleXDeg;
  float angleYDeg;
  float gyroXDegS;
  float gyroYDegS;
  float accelX;
  float accelY;
  float accelZ;
  bool valid;
};

class ImuMpu6050 {
public:
  ImuMpu6050();

  bool begin();
  bool update(float dtSec);
  ImuData getData() const;
  bool isHealthy() const;

private:
  MPU6050 mpu;
  ImuData data;
  bool healthy;
  float angleX;
  float angleY;
};

#endif

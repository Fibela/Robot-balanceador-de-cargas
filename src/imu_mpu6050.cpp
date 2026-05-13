#include "../include/imu_mpu6050.h"
#include "../include/config.h"

ImuMpu6050::ImuMpu6050() : healthy(false), angleX(0.0f), angleY(0.0f) {
  data = {0, 0, 0, 0, 0, 0, 0, false};
}

bool ImuMpu6050::begin() {
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    healthy = false;
    data.valid = false;
    return false;
  }

  healthy = true;
  data.valid = true;
  angleX = 0.0f;
  angleY = 0.0f;
  return true;
}

bool ImuMpu6050::update(float dtSec) {
  if (!healthy || dtSec <= 0.0f) {
    data.valid = false;
    return false;
  }

  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Conversión aproximada:
  // Acelerómetro ±2g -> 16384 LSB/g
  // Giroscopio ±250°/s -> 131 LSB/(°/s)
  float accelX = (float)ax / 16384.0f;
  float accelY = (float)ay / 16384.0f;
  float accelZ = (float)az / 16384.0f;

  float gyroX = (float)gx / 131.0f;
  float gyroY = (float)gy / 131.0f;

  // Ángulos por acelerómetro
  float accelAngleX = atan2(accelY, accelZ) * 180.0f / PI;
  float accelAngleY = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180.0f / PI;

  // Filtro complementario
  angleX = COMPLEMENTARY_ALPHA * (angleX + gyroX * dtSec) + (1.0f - COMPLEMENTARY_ALPHA) * accelAngleX;
  angleY = COMPLEMENTARY_ALPHA * (angleY + gyroY * dtSec) + (1.0f - COMPLEMENTARY_ALPHA) * accelAngleY;

  data.angleXDeg = angleX;
  data.angleYDeg = angleY;
  data.gyroXDegS = gyroX;
  data.gyroYDegS = gyroY;
  data.accelX = accelX;
  data.accelY = accelY;
  data.accelZ = accelZ;
  data.valid = true;
  return true;
}

ImuData ImuMpu6050::getData() const {
  return data;
}

bool ImuMpu6050::isHealthy() const {
  return healthy;
}

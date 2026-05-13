#include "../include/loadcell_hx711.h"

LoadCellHx711::LoadCellHx711(
  uint8_t leftDt, uint8_t leftSck,
  uint8_t rightDt, uint8_t rightSck
) : healthy(false), leftScaleFactor(1.0f), rightScaleFactor(1.0f) {
  hxLeft.begin(leftDt, leftSck);
  hxRight.begin(rightDt, rightSck);
  data = {0, 0, 0, 0, false};
}

bool LoadCellHx711::begin(float leftScale, float rightScale, long leftOffset, long rightOffset) {
  leftScaleFactor = leftScale;
  rightScaleFactor = rightScale;

  hxLeft.set_scale(leftScaleFactor);
  hxRight.set_scale(rightScaleFactor);

  hxLeft.set_offset(leftOffset);
  hxRight.set_offset(rightOffset);

  healthy = true;
  data.valid = true;
  return true;
}

bool LoadCellHx711::update() {
  if (!healthy) {
    data.valid = false;
    return false;
  }

  // read_average(3) para reducir ruido
  float left = hxLeft.get_units(3);
  float right = hxRight.get_units(3);

  // Normalizar a kg si escala fue calibrada para ello
  data.leftKg = left;
  data.rightKg = right;
  data.totalKg = left + right;
  data.diffKg = left - right;
  data.valid = true;

  return true;
}

LoadCellData LoadCellHx711::getData() const {
  return data;
}

bool LoadCellHx711::isHealthy() const {
  return healthy;
}

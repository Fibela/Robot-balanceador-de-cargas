#ifndef LOADCELL_HX711_H
#define LOADCELL_HX711_H

#include <Arduino.h>
#include <HX711.h>

struct LoadCellData {
  float leftKg;
  float rightKg;
  float totalKg;
  float diffKg;
  bool valid;
};

class LoadCellHx711 {
public:
  LoadCellHx711(
    uint8_t leftDt, uint8_t leftSck,
    uint8_t rightDt, uint8_t rightSck
  );

  bool begin(float leftScale, float rightScale, long leftOffset, long rightOffset);
  bool update();
  LoadCellData getData() const;
  bool isHealthy() const;

private:
  HX711 hxLeft;
  HX711 hxRight;
  LoadCellData data;
  bool healthy;

  float leftScaleFactor;
  float rightScaleFactor;
};

#endif

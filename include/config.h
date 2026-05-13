#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ===================== Pines =====================
// I2C en Arduino MEGA: SDA=20, SCL=21

// HX711
static const uint8_t HX711_LEFT_DT_PIN  = 22;
static const uint8_t HX711_LEFT_SCK_PIN = 23;
static const uint8_t HX711_RIGHT_DT_PIN  = 24;
static const uint8_t HX711_RIGHT_SCK_PIN = 25;

// Driver motores (PWM + DIR)
static const uint8_t MOTOR_LEFT_PWM_PIN = 5;
static const uint8_t MOTOR_LEFT_DIR_PIN = 26;
static const uint8_t MOTOR_RIGHT_PWM_PIN = 6;
static const uint8_t MOTOR_RIGHT_DIR_PIN = 27;

// Seguridad
static const uint8_t ESTOP_PIN = 28; // INPUT_PULLUP

// ===================== Lazo de control =====================
static const float CONTROL_DT_SEC = 0.01f;   // 100 Hz
static const uint32_t CONTROL_PERIOD_MS = 10;

// ===================== Límites =====================
static const float MAX_SAFE_ANGLE_DEG = 15.0f;
static const int MAX_PWM_OUTPUT = 180;       // limitar para pruebas
static const int MIN_PWM_OUTPUT = 0;

// ===================== PID X =====================
static const float PID_X_KP = 10.0f;
static const float PID_X_KI = 0.8f;
static const float PID_X_KD = 1.2f;

// ===================== PID Y =====================
static const float PID_Y_KP = 10.0f;
static const float PID_Y_KI = 0.8f;
static const float PID_Y_KD = 1.2f;

// ===================== Filtro complementario =====================
static const float COMPLEMENTARY_ALPHA = 0.98f;

// ===================== Celdas de carga =====================
// Ajustar experimentalmente:
static const float HX711_LEFT_SCALE = 420.0f;
static const float HX711_RIGHT_SCALE = 420.0f;
static const long HX711_LEFT_OFFSET = 0;
static const long HX711_RIGHT_OFFSET = 0;

// ===================== Consignas =====================
static const float TARGET_ANGLE_X_DEG = 0.0f;
static const float TARGET_ANGLE_Y_DEG = 0.0f;

#endif

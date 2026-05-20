#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ===================== Pines Arduino Uno R3 =====================
// HC-SR04
static const uint8_t HCSR04_TRIG_PIN = 8;
static const uint8_t HCSR04_ECHO_PIN = 7;

// Servos SG90 (plataforma 2 ejes con 4 actuadores)
static const uint8_t SERVO_Y_FRONT_PIN = 3;
static const uint8_t SERVO_Y_REAR_PIN = 5;
static const uint8_t SERVO_Z_LEFT_PIN = 6;
static const uint8_t SERVO_Z_RIGHT_PIN = 9;

// Estado y seguridad
static const uint8_t STATUS_LED_PIN = 13;
static const uint8_t ESTOP_PIN = 2; // INPUT_PULLUP (opcional)

// ===================== Lazo de control =====================
static const uint32_t CONTROL_PERIOD_MS = 20; // 50 Hz

// ===================== Distancia objetivo =====================
static const float TARGET_DISTANCE_CM = 20.0f;
static const float STABLE_ERROR_BAND_CM = 1.0f;

// ===================== Límites de servo =====================
static const int SERVO_MIN_DEG = 0;
static const int SERVO_MAX_DEG = 180;
static const int SERVO_CENTER_DEG = 90;
static const int SERVO_TRIM_Y_FRONT = 0;
static const int SERVO_TRIM_Y_REAR = 0;
static const int SERVO_TRIM_Z_LEFT = 0;
static const int SERVO_TRIM_Z_RIGHT = 0;
static const int SERVO_MAX_DELTA_DEG = 35; // desviación máxima desde centro

// ===================== Control PID (error de distancia) =====================
static const float PID_DIST_KP = 2.2f;
static const float PID_DIST_KI = 0.15f;
static const float PID_DIST_KD = 0.4f;

// Conversión de salida PID -> comando por eje
static const float AXIS_MIX_Y_GAIN = 1.0f;
static const float AXIS_MIX_Z_GAIN = 1.0f;

// ===================== Seguridad =====================
static const float MAX_SAFE_ERROR_CM = 12.0f;
static const uint16_t HCSR04_TIMEOUT_US = 30000; // ~5m

// ===================== Modo simulación (sin hardware completo) =====================
// true: usa distancia simulada en main.ino
// false: usa HC-SR04 real
static const bool SIMULATION_MODE = false;

// Escenarios de simulación
// 0 = nominal, 1 = perturbación, 2 = saturación, 3 = fallo intermitente de sensor
static const uint8_t SIM_SCENARIO = 0;

// Parámetros de simulación
static const float SIM_BASE_DISTANCE_CM = TARGET_DISTANCE_CM;
static const float SIM_NOISE_AMPLITUDE_CM = 0.25f;

#endif

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ===================== Pines Arduino Uno R3 =====================
// HC-SR04
static const uint8_t HCSR04_TRIG_PIN = 8;
static const uint8_t HCSR04_ECHO_PIN = 7;

// Servo SG90 único (plataforma simplificada 1 actuador)
static const uint8_t SERVO_MAIN_PIN = 3;

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
static const int SERVO_TRIM_MAIN = 0;
static const int SERVO_MAX_DELTA_DEG = 35; // desviación máxima desde centro

// ===================== Control PID (error de distancia) =====================
static const float PID_DIST_KP = 2.2f;
static const float PID_DIST_KI = 0.15f;
static const float PID_DIST_KD = 0.4f;

// Conversión de salida PID -> comando del único servo
static const float AXIS_MIX_MAIN_GAIN = 1.0f;

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

// ===================== Carga (HX711 opcional) =====================
static const float MIN_LOAD_GRAMS = 5.0f; // requisito mínimo solicitado
static const float DEFAULT_SIM_WEIGHT_GRAMS = 5.0f;

// ===================== Modo de operación =====================
// true = control automático por PID
// false = control manual por comando serial
static const bool DEFAULT_AUTOMATIC_MODE = true;

#endif

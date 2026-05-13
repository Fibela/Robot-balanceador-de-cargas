# Robot Balanceador de Cargas (Plataforma) - Arduino MEGA

Proyecto semestral universitario: plataforma móvil/estática que mantiene el equilibrio de una carga mediante control de inclinación y distribución de peso, usando componentes accesibles y código en C++ compatible con Arduino.

## 1) Objetivo del sistema

Diseñar e implementar un robot/plataforma balanceadora de cargas que:

- Detecte inclinación en eje X/Y con IMU (MPU6050).
- Mida distribución de carga con celdas de carga + HX711.
- Aplique control PID para corregir la inclinación.
- Accione motores (o actuadores) para restablecer equilibrio.
- Reporte métricas de desempeño por puerto serial.

## 2) Alcance académico

Incluye:

- Código fuente completo en C++ para Arduino MEGA.
- Arquitectura modular mantenible.
- Parámetros de control ajustables.
- Métricas de calidad de sistema.
- Documentación de conexión, calibración y pruebas.

No incluye CAD mecánico ni PCB custom (puede añadirse luego).

---

## 3) Arquitectura del repositorio

```text
.
├── README.md
├── TODO.md
├── docs
│   ├── quality-metrics.md
│   └── test-plan.md
├── include
│   ├── config.h
│   ├── imu_mpu6050.h
│   ├── loadcell_hx711.h
│   ├── pid_controller.h
│   ├── motor_driver.h
│   ├── safety.h
│   └── metrics.h
└── src
    ├── main.ino
    ├── imu_mpu6050.cpp
    ├── loadcell_hx711.cpp
    ├── pid_controller.cpp
    ├── motor_driver.cpp
    ├── safety.cpp
    └── metrics.cpp
```

---

## 4) Componentes de hardware (accesibles)

### Control y sensado
- 1x Arduino MEGA 2560
- 1x MPU6050 (IMU 6 ejes)
- 2x o 4x celdas de carga (según diseño mecánico)
- 1x o 2x módulos HX711 (depende de cantidad de celdas)
- Cables dupont / protoboard / borneras

### Actuación
- Opción A (simple): 2x motores DC + driver BTS7960 o L298N
- Opción B (fina): 2x motores con encoder + driver robusto
- Fuente externa para motores (ej. 12V), GND común con Arduino

### Estructura
- Plataforma base rígida (madera/acrílico/aluminio)
- Soportes para sensores y carga

---

## 5) Variables de desempeño (métricas)

1. Error angular RMS (°)
2. Tiempo de asentamiento (s)
3. Sobreimpulso máximo (% o °)
4. Porcentaje de saturación del actuador (%)
5. Desbalance de carga (% entre lados)
6. Disponibilidad en lazo de control (% ciclos válidos)

Detalles en `docs/quality-metrics.md`.

---

## 6) Conexión recomendada (Arduino MEGA)

> Ajustable en `include/config.h`.

- MPU6050 (I2C):
  - SDA -> 20
  - SCL -> 21
- HX711 izquierda:
  - DT -> 22
  - SCK -> 23
- HX711 derecha:
  - DT -> 24
  - SCK -> 25
- Motor izquierdo (PWM/DIR):
  - PWM -> 5
  - DIR -> 26
- Motor derecho (PWM/DIR):
  - PWM -> 6
  - DIR -> 27
- E-STOP / botón seguridad:
  - Pin digital -> 28 (INPUT_PULLUP)

---

## 7) Compilación y carga

1. Abrir `src/main.ino` en Arduino IDE.
2. Instalar librerías:
   - `Wire` (incluida normalmente)
   - `MPU6050` (Jeff Rowberg o compatible)
   - `HX711` (Bogde o compatible)
3. Seleccionar placa: **Arduino Mega or Mega 2560**
4. Seleccionar puerto COM.
5. Subir firmware.

---

## 8) Calibración mínima

1. Encender sin carga para fijar offset IMU.
2. Calibrar factor HX711 por lado (izq/der).
3. Ajustar PID:
   - iniciar con Kp bajo
   - aumentar Kp hasta respuesta aceptable
   - añadir Kd para amortiguar
   - añadir Ki pequeño para error estacionario
4. Verificar límites de seguridad (ángulo y PWM máximos).

---

## 9) Uso por Serial

Baud rate: `115200`.

Se imprimen variables:

- `angle_x`, `angle_y`
- `load_left`, `load_right`, `load_diff`
- `pid_x`, `pid_y`
- métricas acumuladas (`rms_error`, `settling_time`, `saturation_pct`)

---

## 10) Pregunta clave: ¿Cuánto peso soporta?

Depende de:

- Diseño mecánico de la plataforma
- Torque del motor/actuador
- Tipo de transmisión (directa, engranada, husillo, etc.)
- Capacidad de celdas de carga
- Margen de seguridad (recomendado 30%)

Para laboratorio universitario accesible:
- objetivo inicial sugerido: **0.5 kg a 2.0 kg** de carga útil.
- validar experimentalmente con incrementos de 0.25 kg.

---

## 11) Seguridad y buenas prácticas

- Usar fuente separada para motores.
- Unir GND de potencia y control.
- Activar `E_STOP` físico.
- Limitar PWM y ángulo máximo.
- Implementar paro por sensor inválido.

---

## 12) Entregables académicos recomendados

- Video de prueba con escalones de carga.
- Tabla de métricas (antes/después de sintonizar PID).
- Discusión de límites del sistema.
- Propuesta de mejoras futuras.

---

## 13) Licencia

Uso académico. Puedes cambiar a MIT si lo subes públicamente en GitHub.

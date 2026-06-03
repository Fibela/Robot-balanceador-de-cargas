# Robot Balanceador de Cargas (Plataforma 10x10 cm) - Arduino Uno R3

Proyecto técnico-académico (Calidad de Sistemas): plataforma balanceadora con control por software, telemetría serial y dashboard visual para análisis de estabilidad, saturación, latencia, peso y auditoría operativa.

## 1) Objetivo del sistema

Diseñar, implementar y validar un sistema de control para plataforma balanceadora que:

- opere con **1 servomotor SG90** (iteración actual),
- utilice **HC-SR04** para medición de distancia,
- permita operación **automática (PID)** y **manual (panel de control)**,
- registre métricas de **hardware, software e integración**,
- entregue evidencia auditable para pruebas robustas y unitarias.

Masa mínima objetivo de operación: **5 g**.  
Masa máxima: **determinada por pruebas experimentales**.

---

## 2) Alcance actual

Este repositorio cubre:

- Firmware Arduino (control + seguridad + telemetría).
- Interfaz visual técnica local (`ui/`) para monitoreo y control manual.
- Documentación de pruebas, auditoría y costos.
- Trazabilidad de calidad alineada a ISO/IEC 25010 e IEEE 29119.

---

## 3) Arquitectura del repositorio

```text
.
├── README.md
├── TODO.md
├── docs
│   ├── quality-metrics.md
│   ├── test-plan.md
│   ├── testing-terminal.md
│   ├── testing-runner.md
│   ├── test-evidence-template.md
│   └── costos-hardware-software.md
├── include
│   ├── config.h
│   ├── distance_hcsr04.h
│   ├── pid_controller.h
│   ├── motor_driver.h
│   ├── safety.h
│   └── metrics.h
├── src
│   ├── main.ino
│   ├── distance_hcsr04.cpp
│   ├── pid_controller.cpp
│   ├── motor_driver.cpp
│   ├── safety.cpp
│   └── metrics.cpp
├── tools
│   └── test_runner.cpp
└── ui
    ├── index.html
    ├── style.css
    └── app.js
```

---

## 4) Funcionalidades clave (iteración actual)

### Firmware (Arduino IDE)
- Control PID de distancia en modo automático.
- Control manual por comandos seriales:
  - `MODE,AUTO`
  - `MODE,MANUAL`
  - `MANUAL,<0-180>`
  - `STATUS`
- Seguridad con E-STOP y gestión de fallos.
- Telemetría CSV:
  - `DATA,time_s,dist_cm,error_cm,servoMain,latency_ms,weight_g,mode`
  - `METRIC,rms_cm,max_cm,settle_s,sat_pct,samples,...`
  - `EVENT,...`
  - `FAULT,...`

### Interfaz visual técnica (`ui/`)
- Conexión por Web Serial.
- Panel visual de métricas en vivo.
- Panel manual para posicionamiento de servo.
- Módulo de auditoría de eventos y sugerencias técnicas.

---

## 5) Hardware (configuración actual)

- 1x Arduino Uno R3
- 1x Servo SG90 (principal)
- 1x HC-SR04
- Protoboard + cables dupont
- Fuente recomendada 5V/2A
- E-STOP opcional (D2 INPUT_PULLUP)

Ver costos en: **`docs/costos-hardware-software.md`**

---

## 6) Preparación y ejecución

## Firmware
1. Abrir `src/main.ino` en Arduino IDE.
2. Seleccionar Board: **Arduino Uno**.
3. Seleccionar puerto COM.
4. Compilar y cargar.
5. Abrir monitor serial a **115200**.

## Interfaz
1. Abrir `ui/index.html` en Chrome/Edge.
2. Clic en **Conectar Serial**.
3. Seleccionar puerto del Arduino.
4. Visualizar métricas y usar panel manual si aplica.

---

## 7) Calidad, pruebas y auditoría

- Pruebas de ruta crítica y exhaustivas: `docs/testing-terminal.md`
- Runner de validación de logs: `docs/testing-runner.md`
- Plantilla de evidencia formal: `docs/test-evidence-template.md`
- Requisito explícito del proceso:
  - pruebas robustas,
  - pruebas unitarias de módulos críticos,
  - registro de resultados y acciones correctivas.

---

## 8) Propiedad intelectual y licencias

- Firmware y documentación técnica: **Propiedad de Corporación Premos**.
- Arduino IDE y librerías base: según sus licencias oficiales.
- Uso del repositorio: técnico/académico bajo lineamientos del equipo.

---

## 9) Documentación técnica extendida

- `Documentación/system-engineering.md`
- `Documentación/standards-compliance.md`
- `Documentación/metrics-and-calculations.md`
- `Documentación/efficiency-scalability.md`
- `docs/testing-terminal.md`
- `docs/costos-hardware-software.md`
- `docs/deuda-tecnica.md`

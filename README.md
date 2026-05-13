# Robot Balanceador de Cargas (Plataforma 10x10 cm) - Arduino Uno R3

Proyecto semestral universitario (Calidad de Sistemas): plataforma balanceadora de cargas con control por software, orientada a validar lógica de control, seguridad y métricas de calidad mediante ejecución y pruebas desde terminal (monitor serial) en VSCode/Arduino IDE.

## 1) Objetivo del sistema

Diseñar, implementar y validar un sistema de control para plataforma balanceadora que:

- opere sobre los ejes **Y/Z** (según alcance del equipo),
- utilice **4 servomotores SG90** como actuadores,
- utilice **HC-SR04** para detección de objeto y seguimiento de variación de distancia/movimiento,
- procese señales en lazo de control y genere comando por servo,
- registre resultados y métricas por serial para análisis de calidad.

Masa objetivo de validación del caso base: **36 g**.

---

## 2) Alcance académico (calidad de sistemas)

Este repositorio cubre:

- Documentación técnica de inicio a fin del proceso.
- Diseño de pruebas y criterios de aceptación formales.
- Marco de métricas alineado a **ISO/IEC 25010**.
- Estructura de pruebas alineada a **IEEE 29119**.
- Estrategia de validación del software desde terminal (serial logging).

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

> Nota de ingeniería documental: el repositorio incluye módulos heredados (IMU/HX711) de una iteración previa.  
> Para el alcance actual del equipo, la documentación y validación se concentran en **Uno R3 + SG90 + HC-SR04** y pruebas de software por serial.

---

## 4) Hardware del equipo (versión actual)

### Control y sensado
- 1x Arduino Uno R3
- 1x Sensor ultrasónico HC-SR04
- 1x Protoboard
- Cables dupont
- LEDs de testeo

### Actuación
- 4x servomotores Power Pro Micro Servo 9g SG90

### Alimentación
- Fuente dedicada recomendada para servos (**5V, al menos 2A**)
- Batería de 9V: solo referencia inicial (no recomendada como única fuente de servos bajo carga)

### Estructura
- Plataforma conceptual de 10x10 cm (modelo de referencia del proyecto)

---

## 5) Flujo funcional del software (alto nivel)

1. Leer distancia del HC-SR04.
2. Estimar variación respecto al setpoint/escenario esperado.
3. Aplicar lógica de control (proporcional/PID simplificado según implementación activa).
4. Calcular comandos para servos por eje.
5. Limitar salida por seguridad (rangos y saturación).
6. Publicar telemetría serial (`DATA`, `EVENT`, `METRIC`).
7. Evaluar métricas agregadas y criterios de aceptación.

---

## 6) Mapeo de pines sugerido (Arduino Uno R3)

> Configurable en `include/config.h`.

- HC-SR04
  - TRIG -> D8
  - ECHO -> D7
- SG90 #1 (Eje Y frontal) -> D3 (PWM)
- SG90 #2 (Eje Y trasero) -> D5 (PWM)
- SG90 #3 (Eje Z izquierdo) -> D6 (PWM)
- SG90 #4 (Eje Z derecho) -> D9 (PWM)
- LED estado -> D13
- E-STOP (opcional) -> D2 (INPUT_PULLUP)

---

## 7) Preparación de entorno y compilación

1. Abrir `src/main.ino` en Arduino IDE.
2. Seleccionar placa: **Arduino Uno**.
3. Seleccionar procesador/puerto COM correspondiente.
4. Instalar librerías mínimas (si aplica en implementación actual):
   - `Servo` (incluida en entorno Arduino)
5. Compilar y subir.
6. Abrir monitor serial a `115200` baudios.

---

## 8) Estrategia de validación por terminal (VSCode/Serial Monitor)

Como el enfoque inmediato es probar software, ejecutar campañas de prueba por serial:

- Registrar líneas tipo:
  - `DATA,time_ms,dist_cm,error_y,error_z,servo1,servo2,servo3,servo4`
  - `EVENT,time_ms,code,detail`
  - `METRIC,window_s,rms_error,max_error,settling_ms,sat_pct,loop_ok_pct`
- Exportar logs a CSV.
- Analizar en hoja de cálculo o script Python.
- Consolidar evidencia por caso de prueba (ver `docs/test-plan.md`).

---

## 9) Parámetros iniciales recomendados

- Frecuencia de lazo: 20 a 50 Hz
- Ventana de métrica: 10 s
- Límite angular/comando equivalente por servo: 0° a 180° (con límites operativos más conservadores)
- Banda de error estable: ±5% del valor objetivo de distancia
- Saturación aceptable (operación nominal): < 25%

---

## 10) Criterios de aceptación global (resumen)

- Estabilidad del control en caso base de **36 g**.
- Errores dentro de umbral definido en plan de métricas.
- Saturación no excesiva en operación nominal.
- Trazas seriales completas y consistentes.
- Cumplimiento documental de ISO/IEC 25010 + IEEE 29119.

---

## 11) Gestión de calidad y trazabilidad

La calidad se evidencia con:

- Matriz requisito → caso de prueba → métrica → evidencia.
- Registro de resultados por ejecución.
- Umbrales de aprobación cuantitativos.
- Historial de ajustes de parámetros y su impacto.

Ver detalle en:

- `docs/quality-metrics.md`
- `docs/test-plan.md`
- `TODO.md`

---

## 12) Entregables académicos recomendados

- Documento técnico final (arquitectura, decisiones y resultados).
- Plan de pruebas ejecutado y evidencias.
- Tabla comparativa de iteraciones de control.
- Bitácora de defectos/incidencias y correcciones.
- Conclusiones de calidad del sistema.

---

## 13) Licencia

Uso académico.

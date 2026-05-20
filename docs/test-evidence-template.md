# Plantilla de evidencia de pruebas (Arduino Balanceador)

Fecha: `YYYY-MM-DD`  
Responsable: `nombre`  
Firmware commit/tag: `hash o versión`  
Placa: `Arduino Uno`  
Puerto: `COMx`  
Baudrate: `115200`

## Configuración ejecutada

- `SIMULATION_MODE = true|false`
- `SIM_SCENARIO = 0|1|2|3`
- Hardware conectado:
  - HC-SR04: `sí/no`
  - MPU6050: `sí/no`
  - HX711: `sí/no`
  - Motores/servos: `sí/no`
  - E-STOP (D2): `sí/no`

---

## Caso: CP-01 Arranque limpio

### Pasos
1. Compilar y subir firmware.
2. Abrir monitor serial a 115200.
3. Capturar 20–30 líneas iniciales.

### Evidencia
- Archivo log: `docs/evidence/cp01.log`
- Extracto relevante:
  - `EVENT,BOOT,ok`
  - líneas `DATA,...`
  - líneas `METRIC,...`

### Resultado
- Estado: `PASS/FAIL`
- Observaciones: `...`

---

## Caso: CP-02 Seguimiento nominal

### Pasos
1. Mantener `SIM_SCENARIO=0`.
2. Ejecutar ventana de observación (ej. 30–60 s).
3. Verificar estabilidad de error y saturación.

### Evidencia
- Archivo log: `docs/evidence/cp02.log`
- Indicadores observados:
  - `rms_cm`: `valor`
  - `sat_pct`: `valor`
  - comportamiento de error: `...`

### Resultado
- Estado: `PASS/FAIL`
- Observaciones: `...`

---

## Caso: CP-03 Integridad de telemetría

### Pasos
1. Revisar formato de líneas `DATA`.
2. Confirmar presencia de `METRIC` con campos esperados.

### Evidencia
- Archivo log: `docs/evidence/cp03.log`
- Campos presentes:
  - `rms_cm`: `sí/no`
  - `max_cm`: `sí/no`
  - `settle_s`: `sí/no`
  - `sat_pct`: `sí/no`
  - `samples`: `sí/no`

### Resultado
- Estado: `PASS/FAIL`
- Observaciones: `...`

---

## Caso: EX-01 Perturbación (`SIM_SCENARIO=1`)

### Evidencia
- Archivo log: `docs/evidence/ex01.log`
- Comportamiento transitorio y recuperación: `...`

### Resultado
- Estado: `PASS/FAIL`
- Observaciones: `...`

---

## Caso: EX-02 Saturación (`SIM_SCENARIO=2`)

### Evidencia
- Archivo log: `docs/evidence/ex02.log`
- `sat_pct` elevado y continuidad de telemetría: `...`

### Resultado
- Estado: `PASS/FAIL`
- Observaciones: `...`

---

## Caso: EX-03 Fallo de sensor (`SIM_SCENARIO=3`)

### Evidencia
- Archivo log: `docs/evidence/ex03.log`
- Presencia de `FAULT,...` y recuperación: `...`

### Resultado
- Estado: `PASS/FAIL`
- Observaciones: `...`

---

## Caso: EX-04 E-STOP físico (si cableado)

### Evidencia
- Archivo log: `docs/evidence/ex04.log`
- Señal de `FAULT,...` y detención segura de actuadores: `...`

### Resultado
- Estado: `PASS/FAIL`
- Observaciones: `...`

---

## Resumen final de corrida

- CP-01: `PASS/FAIL`
- CP-02: `PASS/FAIL`
- CP-03: `PASS/FAIL`
- EX-01: `PASS/FAIL`
- EX-02: `PASS/FAIL`
- EX-03: `PASS/FAIL`
- EX-04: `PASS/FAIL/NA`

Conclusión de aceptación: `APROBADO/NO APROBADO`  
Acciones correctivas: `...`

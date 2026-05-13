# Plan de Pruebas - Robot Balanceador de Cargas

## 1) Objetivo

Validar funcionamiento, estabilidad, seguridad y capacidad de carga de la plataforma balanceadora.

## 2) Precondiciones

- Firmware cargado en Arduino MEGA.
- MPU6050 detectada correctamente.
- HX711 calibrados (izq/der).
- Motores responden a señal PWM.
- E-STOP operativo.
- Fuente de motor adecuada y GND común.

## 3) Casos de prueba

### CP-01: Arranque en reposo
**Entrada:** encender sin carga.  
**Esperado:** ángulos cercanos a 0°, sin fault, PWM bajo.

### CP-02: Carga nominal baja
**Entrada:** colocar 0.5 kg centrados.  
**Esperado:** plataforma estable, RMS bajo, sin saturación excesiva.

### CP-03: Perturbación externa
**Entrada:** empuje leve lateral durante 1s.  
**Esperado:** recuperación al equilibrio y tiempo de asentamiento medible.

### CP-04: Carga desbalanceada
**Entrada:** mover carga hacia lado izquierdo.  
**Esperado:** corrección de inclinación y lectura de `diffKg` coherente.

### CP-05: Escalera de carga
**Entrada:** incrementar 0.25 kg hasta límite seguro.  
**Esperado:** registrar punto donde se pierde desempeño aceptable.

### CP-06: Seguridad por ángulo
**Entrada:** inclinar manualmente más allá del límite configurado.  
**Esperado:** `FAULT,ANGLE_LIMIT`, paro de motores.

### CP-07: Seguridad por botón
**Entrada:** presionar E-STOP.  
**Esperado:** `FAULT,ESTOP_BUTTON`, motores en 0 inmediato.

## 4) Registro de datos

Capturar por serial líneas:

- `DATA,time,angleX,angleY,loadL,loadR,ctrlX,ctrlY`
- `METRICS,rmsX=...,rmsY=...,satPct=...`

Guardar en CSV para análisis en Excel o Python.

## 5) Criterio de aprobación general

- Sin fallos críticos en 5 minutos de operación.
- Recuperación estable ante perturbaciones leves.
- Métricas dentro de umbrales definidos en `quality-metrics.md`.
- Seguridad actuando correctamente en todos los escenarios.

## 6) Plantilla de resultados (reporte)

| Caso | Estado | RMS X | RMS Y | Settling X | Settling Y | Sat % | Observaciones |
|------|--------|-------|-------|------------|------------|-------|---------------|
| CP-01 | PASS/FAIL | | | | | | |
| CP-02 | PASS/FAIL | | | | | | |
| CP-03 | PASS/FAIL | | | | | | |
| CP-04 | PASS/FAIL | | | | | | |
| CP-05 | PASS/FAIL | | | | | | |
| CP-06 | PASS/FAIL | | | | | | |
| CP-07 | PASS/FAIL | | | | | | |

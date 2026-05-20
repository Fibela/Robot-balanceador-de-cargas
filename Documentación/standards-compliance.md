# Cumplimiento de Normas y Estándares

## 1. Objetivo
Establecer el mapeo formal entre el desarrollo/pruebas del Robot Balanceador de Cargas (Prototipo 1) y marcos de referencia de calidad y pruebas:

- **ISO/IEC 25010** (calidad de producto software)
- **IEEE 29119** (proceso y documentación de pruebas)

---

## 2. Alcance evaluado
Sistema embebido en Arduino Uno R3 con:
- HC-SR04 (sensado de distancia),
- 4 servos SG90 (actuación),
- control en lazo cerrado con PID,
- telemetría serial (`EVENT`, `DATA`, `FAULT`, `METRIC`),
- mecanismos de seguridad (falla de sensor, E-STOP).

---

## 3. Mapeo a ISO/IEC 25010

## 3.1 Functional suitability (Idoneidad funcional)
- **Functional completeness**: cubre adquisición, control, actuación, seguridad y métricas.
- **Functional correctness**: calcula error, aplica PID, limita salidas y reporta resultados.
- **Functional appropriateness**: diseño enfocado al caso académico de balanceo y trazabilidad.

Evidencia:
- `src/main.ino`
- `include/config.h`
- `include/metrics.h` / `src/metrics.cpp`

## 3.2 Performance efficiency (Eficiencia de desempeño)
- **Time behavior**: lazo periódico con `CONTROL_PERIOD_MS`.
- **Resource utilization**: implementación ligera para MCU AVR.
- **Capacity**: operación con 1 sensor + 4 actuadores en tiempo real básico.

Evidencia:
- periodicidad observada vía serial,
- métricas de saturación y estabilidad.

## 3.3 Reliability (Fiabilidad)
- gestión de falla de sensor (timeout/invalidez),
- transición a estado seguro (`stopAll`, `FAULT`),
- reinicialización control ante condición insegura.

Evidencia:
- `SafetyManager`,
- eventos `FAULT`,
- pruebas con `SIM_SCENARIO=3`.

## 3.4 Maintainability (Mantenibilidad)
- modularidad por componentes (`distance`, `motor`, `metrics`, `safety`, `pid`),
- parámetros centralizados (`config.h`),
- modo simulación compilable para validación rápida.

Evidencia:
- estructura `include/` y `src/`,
- documentación técnica en carpeta `Documentación`.

## 3.5 Portability (Portabilidad)
- objetivo principal: Arduino Uno R3.
- portabilidad intra-Arduino posible con ajuste de pines/temporización.

---

## 4. Mapeo a IEEE 29119

## 4.1 Test process (proceso de prueba)
- Planificación: definición de ruta crítica y pruebas exhaustivas.
- Diseño: escenarios nominal, perturbación, saturación, fallo sensor, E-STOP.
- Ejecución: monitor serial + evidencia textual.
- Evaluación: criterios por métricas (`rms`, `max`, `sat_pct`, `settle`).

## 4.2 Test documentation (documentación)
Artefactos:
- especificación de pruebas por terminal serial (`docs/testing-terminal.md`),
- criterios y fórmulas (`Documentación/metrics-and-calculations.md`),
- trazabilidad técnica (este documento y `system-engineering.md`).

## 4.3 Test design techniques
- pruebas basadas en escenarios (state/behavior oriented),
- criterio umbral (threshold-based acceptance),
- pruebas de seguridad tipo fault-injection (simulada).

---

## 5. Matriz de trazabilidad (resumen)

| Requisito | Caso de prueba | Métrica | Criterio |
|---|---|---|---|
| Control estable en nominal | CP-02 | RMS error, settling | Error dentro de banda y convergencia |
| Trazabilidad serial | CP-03 | Integridad de líneas DATA/METRIC | Formato consistente |
| Seguridad ante falla sensor | EX-03 | Eventos FAULT + paro actuadores | Debe activar estado seguro |
| Robustez ante perturbación | EX-01 | Max abs error + recuperación | Recuperación sin colapso |
| Comportamiento en saturación | EX-02 | sat_pct | Saturación detectada y reportada |

---

## 6. Limitaciones y consideraciones
- El cumplimiento es de **aplicación académica y evidencial**, no certificación formal.
- Se requiere ejecución de pruebas en hardware real para cerrar evidencia final.
- Cualquier cambio mecánico/eléctrico impacta umbrales y tuning.

---

## 7. Conclusión
La solución se alinea de manera consistente con principios de ISO/IEC 25010 y IEEE 29119, especialmente en:
- calidad funcional,
- desempeño básico en tiempo,
- seguridad operacional,
- trazabilidad y documentación de pruebas.

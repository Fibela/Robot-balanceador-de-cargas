# Deuda técnica registrada

## Contexto

Durante esta iteración se implementaron cambios de arquitectura y producto:

- Migración de control de 4 servos a **1 servo**.
- Incorporación de modo **automático/manual** por comandos seriales.
- Nueva interfaz técnica visual en `ui/` (dashboard + panel manual + auditoría).
- Actualización de documentación y modelo de costos.

Por limitación de ejecución física en el momento, se registra la siguiente deuda técnica.

---

## DT-001 — Validación exhaustiva en hardware pendiente

**Descripción:**  
No se completó aún la corrida de pruebas exhaustivas con hardware conectado (Arduino + sensor + servo), incluyendo evidencia serial completa y verificación de integración UI↔firmware.

**Impacto:**  
Riesgo medio en validación final de comportamiento real (latencia física, respuesta mecánica, robustez ante fallos reales, comandos manuales bajo carga).

**Prioridad:** Alta

**Estado:** Abierta

**Criterio de cierre:**
1. Compilar/subir firmware actualizado en Arduino IDE.
2. Ejecutar y evidenciar comandos:
   - `STATUS`
   - `MODE,MANUAL`
   - `MANUAL,<deg>`
   - `MODE,AUTO`
3. Capturar logs con `EVENT`, `DATA`, `METRIC`, `FAULT`.
4. Validar escenarios CP/EX:
   - CP-01, CP-02, CP-03
   - EX-01, EX-02, EX-03, EX-04 (si E-STOP cableado)
5. Ejecutar pruebas completas de UI:
   - conexión/desconexión serial,
   - panel manual,
   - actualización de métricas y sugerencias,
   - auditoría visual.
6. Consolidar evidencia en plantilla:
   - `docs/test-evidence-template.md`

**Responsable propuesto:** Equipo de integración/hardware

**Fecha objetivo sugerida:** Próxima sesión con prototipo conectado

---

## DT-002 — Pruebas unitarias formales pendientes

**Descripción:**  
Falta completar batería de pruebas unitarias automatizadas para módulos críticos del firmware y parser de UI.

**Impacto:**  
Riesgo medio-bajo en mantenibilidad y detección temprana de regresiones.

**Prioridad:** Media

**Estado:** Abierta

**Criterio de cierre:**
- Definir y ejecutar unit tests para:
  - `motor_driver` (clamp y comandos),
  - `metrics` (cálculo RMS/saturación/settling),
  - parser de líneas seriales en UI (`DATA/METRIC/EVENT/FAULT`).

---

## DT-003 — Límite máximo de carga no caracterizado

**Descripción:**  
Se fijó carga mínima de operación en 5 g; el límite máximo permanece abierto y depende de campaña experimental.

**Impacto:**  
Riesgo medio en criterios de operación segura y especificación final del sistema.

**Prioridad:** Alta

**Estado:** Abierta

**Criterio de cierre:**
- Ejecutar pruebas incrementales de carga y registrar punto de degradación:
  - estabilidad,
  - saturación,
  - seguridad (fault/stop).
- Actualizar especificación de rango de masa operativa validada.

---

## Trazabilidad

Esta deuda técnica debe revisarse junto a:
- `docs/testing-terminal.md`
- `docs/testing-runner.md`
- `docs/test-evidence-template.md`
- `README.md`

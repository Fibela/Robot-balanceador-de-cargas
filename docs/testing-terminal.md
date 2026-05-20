# Pruebas por terminal serial (VSCode + Arduino IDE) — Prototipo 1

Este flujo es **100% Arduino/C++**, sin Node.js.

## 1) Compatibilidad de lenguaje

Sí: Arduino usa C/C++.  
Tu proyecto (`.ino`, `.h`, `.cpp`) es totalmente compatible con Arduino Uno.

---

## 2) Objetivo de pruebas

Validar por monitor serial:

- estabilidad básica del control,
- respuesta a perturbaciones,
- manejo de fallos de sensor,
- actuación segura de servos,
- trazabilidad de telemetría para evidencia de calidad.

Formato esperado:
- `DATA,...`
- `FAULT,...`
- `METRIC,...`
- `EVENT,...`

---

## 3) Referencias técnicas relacionadas

Para fundamentación teórica y normativa del plan de pruebas, usar en conjunto:

- `Documentación/system-engineering.md`
- `Documentación/standards-compliance.md`
- `Documentación/metrics-and-calculations.md`
- `Documentación/efficiency-scalability.md`

---

## 4) Configuración rápida

Archivo:
- `include/config.h`

Parámetros clave:
- `SIMULATION_MODE`:
  - `false` = usa HC-SR04 real
  - `true` = usa distancia simulada
- `SIM_SCENARIO`:
  - `0` nominal
  - `1` perturbación
  - `2` saturación
  - `3` fallo intermitente de sensor

---

## 5) Ejecución en terminal/monitor serial

1. Abrir `src/main.ino` en Arduino IDE (o extensión Arduino en VSCode).
2. Seleccionar placa: **Arduino Uno**.
3. Seleccionar puerto COM.
4. Compilar y subir.
5. Abrir monitor serial a **115200 baud**.
6. Guardar salida serial para evidencia (copiar/pegar a archivo `.csv` o `.log`).

---

## 6) Pruebas de ruta crítica

## CP-01 Arranque limpio
- Config: `SIMULATION_MODE=true`, `SIM_SCENARIO=0`.
- Esperado:
  - `EVENT,BOOT,ok`
  - líneas `DATA` periódicas
  - líneas `METRIC` ~cada 1s
  - sin `FAULT` continuo.

## CP-02 Seguimiento nominal
- Config: igual CP-01.
- Esperado:
  - error convergente,
  - `rms_cm` moderado,
  - `sat_pct` bajo/moderado.

## CP-03 Integridad de telemetría
- Esperado:
  - columnas consistentes en cada línea `DATA`,
  - `METRIC` con campos `rms_cm`, `max_cm`, `settle_s`, `sat_pct`, `samples`.

---

## 7) Pruebas exhaustivas (alineadas a calidad ISO/IEEE)

## EX-01 Perturbación
- `SIM_SCENARIO=1`
- Esperado:
  - subida temporal de error,
  - recuperación posterior,
  - métricas reflejan transitorio.

## EX-02 Saturación
- `SIM_SCENARIO=2`
- Esperado:
  - aumento de `sat_pct`,
  - sistema no debe colgarse,
  - mantiene telemetría continua.

## EX-03 Fallo de sensor
- `SIM_SCENARIO=3`
- Esperado:
  - aparición de `FAULT,...`,
  - salida segura (`stopAll`) durante condición inválida,
  - recuperación al restablecer lectura.

## EX-04 E-STOP físico (si está cableado)
- `SIMULATION_MODE=false` o `true` (según validación).
- Accionar pin E-STOP (D2, INPUT_PULLUP).
- Esperado:
  - `FAULT,...`,
  - detención de actuadores.

---

## 8) Trazabilidad de calidad (resumen)

- **ISO/IEC 25010** (en este contexto):
  - rendimiento temporal (periodicidad y estabilidad),
  - fiabilidad (degradación segura ante fallos),
  - mantenibilidad (configuración centralizada en `config.h`).

- **IEEE 29119**:
  - casos de prueba definidos (CP/EX),
  - entradas, pasos, resultados esperados,
  - evidencia por logs seriales.

---

## 9) Criterio de aceptación sugerido

- Arranque consistente sin bloqueos.
- Telemetría completa (`DATA/FAULT/METRIC/EVENT`).
- Comportamiento seguro en fallo de sensor y E-STOP.
- En nominal: error en banda estable la mayor parte del tiempo.
- Documentación de pruebas con evidencia guardada.

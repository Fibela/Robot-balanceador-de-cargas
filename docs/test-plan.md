# Plan de Pruebas - Robot Balanceador de Cargas (IEEE 29119)

## 1) Propósito y alcance

Definir y ejecutar pruebas del software de control para plataforma balanceadora (Arduino Uno R3 + 4x SG90 + HC-SR04), con verificación de comportamiento por telemetría serial y criterios de aceptación cuantitativos.

Este plan adopta estructura alineada a **IEEE 29119**.

---

## 2) Estrategia de pruebas (IEEE 29119)

### 2.1 Niveles cubiertos
- **Pruebas funcionales de sistema** (comportamiento esperado del lazo de control).
- **Pruebas no funcionales** (desempeño, confiabilidad, seguridad).
- **Pruebas de regresión básica** tras cambio de parámetros.

### 2.2 Técnicas principales
- Pruebas basadas en escenarios.
- Pruebas por umbral (límites de aceptación de KPI).
- Pruebas de perturbación controlada.
- Pruebas de seguridad por activación de eventos.

### 2.3 Criterio de entrada
- Firmware compila y carga correctamente en Arduino Uno R3.
- Monitor serial operativo a 115200.
- HC-SR04 y 4 servos conectados según configuración.
- Fuente dedicada de servos activa y GND común.

### 2.4 Criterio de salida
- Todos los casos críticos ejecutados.
- Evidencia serial registrada.
- KPIs evaluados contra umbrales (`docs/quality-metrics.md`).
- Informe PASS/FAIL consolidado.

---

## 3) Datos base y configuración de prueba

- Caso de referencia de masa: **36 g**.
- Ejes de control: **Y/Z**.
- Ventana mínima por ejecución: 120 s.
- Formatos de log esperados:
  - `DATA,time_ms,dist_cm,error_y,error_z,servo1,servo2,servo3,servo4`
  - `EVENT,time_ms,code,detail`
  - `METRIC,window_s,rms_y,rms_z,maxerr_y,maxerr_z,settling_ms,sat_pct,loop_ok_pct`

---

## 4) Casos de prueba detallados

### TP-01 Arranque y estado inicial
- **Objetivo:** validar inicialización estable del sistema.
- **Precondición:** firmware recién cargado, hardware energizado.
- **Pasos:**
  1. Encender sistema.
  2. Esperar 10 s de estabilización.
  3. Observar logs `DATA`/`EVENT`.
- **Resultado esperado:**
  - Sin eventos de error en arranque.
  - Salidas de servo en rango válido.
  - Telemetría continua.
- **Criterio PASS:** no hay fallo crítico y telemetría íntegra.

### TP-02 Seguimiento nominal con objeto (36 g)
- **Objetivo:** validar desempeño nominal de control.
- **Precondición:** TP-01 en PASS.
- **Pasos:**
  1. Ejecutar escenario nominal 120 s.
  2. Registrar logs.
  3. Calcular KPIs.
- **Resultado esperado:**
  - Error y settling dentro de umbral.
  - Saturación moderada.
- **Criterio PASS:** KPI-01..KPI-05 dentro de límites nominales.

### TP-03 Perturbación breve y recuperación
- **Objetivo:** medir recuperación del sistema.
- **Precondición:** TP-02 en ejecución.
- **Pasos:**
  1. Aplicar perturbación breve (simulada/controlada) durante ~1 s.
  2. Continuar registro por 60 s adicionales.
- **Resultado esperado:**
  - Recuperación al régimen estable.
  - Tiempo de asentamiento medible y dentro de umbral.
- **Criterio PASS:** KPI-03 cumple; sin fault crítico.

### TP-04 Prueba de saturación de actuadores
- **Objetivo:** cuantificar operación en límites.
- **Precondición:** sistema operativo.
- **Pasos:**
  1. Ejecutar escenario exigente (cambio brusco de referencia).
  2. Registrar porcentaje de saturación.
- **Resultado esperado:**
  - Saturación detectable pero controlada.
  - Sin pérdida de lazo.
- **Criterio PASS:** KPI-04 dentro de umbral definido para escenario.

### TP-05 Integridad del lazo y telemetría
- **Objetivo:** validar continuidad y calidad de datos.
- **Precondición:** ejecución mínima de 5 min.
- **Pasos:**
  1. Correr sistema continuo.
  2. Contar líneas válidas y ciclos válidos.
- **Resultado esperado:**
  - `loop_ok_pct` alto.
  - Telemetría sin cortes relevantes.
- **Criterio PASS:** KPI-05 y KPI-07 ≥ 99%.

### TP-06 Seguridad funcional (evento forzado)
- **Objetivo:** comprobar respuesta ante condición anómala.
- **Precondición:** sistema en ejecución.
- **Pasos:**
  1. Forzar condición de seguridad (evento de prueba).
  2. Verificar emisión de `EVENT`.
  3. Verificar comando seguro a servos.
- **Resultado esperado:**
  - Evento registrado con código claro.
  - Actuación en modo seguro.
- **Criterio PASS:** detección + reacción correcta + log consistente.

### TP-07 Regresión de parámetros de control
- **Objetivo:** validar que ajustes de parámetros no degradan calidad.
- **Precondición:** baseline de KPIs disponible.
- **Pasos:**
  1. Ajustar parámetros de control.
  2. Repetir TP-02 y TP-03.
  3. Comparar KPIs vs baseline.
- **Resultado esperado:**
  - Mejora o mantenimiento de desempeño.
- **Criterio PASS:** no degradación crítica de KPIs principales.

---

## 5) Matriz resumida de trazabilidad prueba ↔ métrica

| Caso | Adecuación funcional | Eficiencia | Confiabilidad | Seguridad | KPIs clave |
|------|----------------------|-----------|---------------|-----------|------------|
| TP-01 | Sí | Parcial | Sí | Sí | KPI-05, KPI-07 |
| TP-02 | Sí | Sí | Sí | Parcial | KPI-01, KPI-02, KPI-03, KPI-04 |
| TP-03 | Sí | Sí | Sí | Parcial | KPI-03, KPI-04 |
| TP-04 | Parcial | Sí | Sí | Parcial | KPI-04, KPI-06 |
| TP-05 | Parcial | Sí | Sí | No | KPI-05, KPI-07 |
| TP-06 | Parcial | No | Sí | Sí | KPI-08 |
| TP-07 | Sí | Sí | Sí | Parcial | KPI-01..KPI-07 |

---

## 6) Gestión de incidencias

Formato sugerido por incidencia:

- ID
- Fecha/hora
- Caso de prueba asociado
- Severidad (Alta/Media/Baja)
- Descripción
- Evidencia (fragmento de log)
- Estado (Abierta/En análisis/Cerrada)
- Acción correctiva

---

## 7) Plantilla de resultados de ejecución

| Ejecución | Caso | Estado | RMS Y | RMS Z | MaxErr Y | MaxErr Z | Settling (s) | Sat % | Loop OK % | Telemetría % | Observaciones |
|----------|------|--------|-------|-------|----------|----------|--------------|-------|-----------|--------------|---------------|
| RUN-01 | TP-01 | PASS/FAIL | | | | | | | | | |
| RUN-01 | TP-02 | PASS/FAIL | | | | | | | | | |
| RUN-01 | TP-03 | PASS/FAIL | | | | | | | | | |
| RUN-01 | TP-04 | PASS/FAIL | | | | | | | | | |
| RUN-01 | TP-05 | PASS/FAIL | | | | | | | | | |
| RUN-01 | TP-06 | PASS/FAIL | | | | | | | | | |
| RUN-01 | TP-07 | PASS/FAIL | | | | | | | | | |

---

## 8) Entregables de pruebas

- Evidencia de logs seriales (archivos CSV/TXT).
- Tabla consolidada de resultados.
- Incidencias registradas y estado.
- Conclusión de cumplimiento contra ISO/IEC 25010 (desde métricas).

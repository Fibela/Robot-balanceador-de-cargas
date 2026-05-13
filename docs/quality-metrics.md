# Métricas de Calidad del Sistema (ISO/IEC 25010)

## 1) Objetivo

Definir el marco de calidad y las métricas cuantitativas para evaluar el software del robot balanceador de cargas (Arduino Uno R3 + 4x SG90 + HC-SR04), con evidencia obtenida por ejecución y registro serial.

---

## 2) Modelo de calidad adoptado

Se emplea **ISO/IEC 25010** como referencia principal de calidad del producto software, priorizando las siguientes características:

1. **Adecuación funcional**  
2. **Eficiencia de desempeño**  
3. **Confiabilidad**  
4. **Usabilidad operativa** (nivel técnico para laboratorio)  
5. **Mantenibilidad**  
6. **Portabilidad** (entorno Arduino IDE/Uno R3)

---

## 3) KPIs medibles del sistema (software + control)

### KPI-01: Error RMS de control por eje (Y/Z)
- **Definición:** raíz media cuadrática del error respecto al setpoint.
- **Unidad:** cm o unidad normalizada de error.
- **Fórmula:**  
  \[
  RMS = \sqrt{\frac{1}{N}\sum_{i=1}^{N}(e_i)^2}
  \]
- **Objetivo (caso base 36 g):** RMS ≤ 5% del setpoint.

### KPI-02: Error máximo absoluto
- **Definición:** máximo valor absoluto de error durante la ventana.
- **Unidad:** cm o error normalizado.
- **Objetivo:** Error pico ≤ 12% del setpoint en operación nominal.

### KPI-03: Tiempo de asentamiento
- **Definición:** tiempo hasta que el error entra y permanece en banda de estabilidad.
- **Banda sugerida:** ±5% del setpoint.
- **Unidad:** ms o s.
- **Objetivo:** ≤ 3.0 s.

### KPI-04: Porcentaje de saturación de servos
- **Definición:** porcentaje de muestras en que la salida está en límite operativo.
- **Unidad:** %.
- **Objetivo:** < 25% en operación nominal.

### KPI-05: Disponibilidad del lazo de control
- **Definición:** ciclos válidos / ciclos totales.
- **Unidad:** %.
- **Objetivo:** ≥ 99%.

### KPI-06: Latencia de ciclo
- **Definición:** tiempo entre iteraciones completas del lazo.
- **Unidad:** ms.
- **Objetivo:** media ≤ 50 ms, jitter controlado.

### KPI-07: Integridad de telemetría
- **Definición:** líneas seriales válidas recibidas / líneas esperadas.
- **Unidad:** %.
- **Objetivo:** ≥ 99%.

### KPI-08: Tasa de eventos de seguridad
- **Definición:** cantidad de eventos de seguridad por ventana de prueba.
- **Unidad:** eventos/10 min.
- **Objetivo:** 0 en operación nominal; activación correcta en pruebas forzadas.

---

## 4) Relación KPI ↔ ISO/IEC 25010

- **Adecuación funcional:** KPI-01, KPI-02, KPI-03  
- **Eficiencia de desempeño:** KPI-04, KPI-06  
- **Confiabilidad:** KPI-05, KPI-07, KPI-08  
- **Usabilidad operativa:** consistencia de logs y claridad de estados (`EVENT`)  
- **Mantenibilidad:** facilidad de ajuste de parámetros en `include/config.h`  
- **Portabilidad:** compilación reproducible en Arduino IDE para Uno R3

---

## 5) Formato de evidencia serial recomendado

Se recomienda que el firmware emita líneas estructuradas:

- `DATA,time_ms,dist_cm,error_y,error_z,servo1,servo2,servo3,servo4`
- `EVENT,time_ms,code,detail`
- `METRIC,window_s,rms_y,rms_z,maxerr_y,maxerr_z,settling_ms,sat_pct,loop_ok_pct`

Estas líneas permiten cálculo automático de KPIs y trazabilidad de resultados.

---

## 6) Criterios de aceptación (calidad semestral)

Para aprobar una ejecución de referencia:

1. KPI-01 y KPI-02 dentro de umbrales.
2. KPI-03 ≤ 3.0 s en pruebas nominales.
3. KPI-04 < 25% en condiciones normales.
4. KPI-05 y KPI-07 ≥ 99%.
5. KPI-08 = 0 en nominal y activación correcta en pruebas de seguridad.
6. Evidencia serial completa y reproducible.

---

## 7) Procedimiento de medición

1. Configurar ejecución con caso base (36 g) y setpoint definido.
2. Ejecutar ventana de prueba nominal (mínimo 2 minutos).
3. Aplicar perturbación controlada y registrar recuperación.
4. Exportar logs seriales a CSV.
5. Calcular KPIs en hoja de cálculo/script.
6. Comparar contra umbrales de aceptación.
7. Registrar resultado PASS/FAIL por escenario.

---

## 8) Riesgos de medición y control de calidad de datos

- Ruido en lectura HC-SR04 por reflexión/superficie.
- Variación de voltaje en línea de servos.
- Jitter por bloqueos en loop o delays excesivos.
- Pérdida de líneas seriales por buffer.

Control recomendado:

- Promediado/filtrado simple de distancia.
- Muestreo con timestamp en ms.
- Validación de rango de sensor antes de control.
- Verificación de secuencia y completitud de logs.

---

## 9) Trazabilidad calidad ↔ código

- `src/main.ino`: ciclo principal, lectura de sensor, telemetría.
- `src/motor_driver.cpp`: comando y límites de servos/actuación.
- `src/pid_controller.cpp`: cálculo de control.
- `src/safety.cpp`: reglas de seguridad y eventos.
- `src/metrics.cpp`: cálculo acumulado de métricas.
- `include/config.h`: parámetros ajustables y umbrales.

---

## 10) Plantilla rápida de reporte de métricas

| Ejecución | RMS Y | RMS Z | MaxErr Y | MaxErr Z | Settling (s) | Sat % | Loop OK % | Telemetría OK % | Estado |
|----------|-------|-------|----------|----------|--------------|-------|-----------|------------------|--------|
| RUN-01   |       |       |          |          |              |       |           |                  | PASS/FAIL |
| RUN-02   |       |       |          |          |              |       |           |                  | PASS/FAIL |

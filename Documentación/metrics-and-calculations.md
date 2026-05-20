# Métricas, Cálculos y Base Teórica de Evaluación

## 1. Objetivo
Definir las métricas cuantitativas para evaluar el desempeño del lazo de control del Robot Balanceador y su interpretación técnica.

## 2. Variables fundamentales
- \(r_k\): referencia (distancia objetivo en cm) en muestra \(k\)
- \(y_k\): distancia medida en cm
- \(e_k = r_k - y_k\): error instantáneo
- \(u_k\): salida de control PID (antes de límites)
- \(T_s\): periodo de muestreo (s), con `CONTROL_PERIOD_MS`

---

## 3. Métricas principales

## 3.1 Error instantáneo
\[
e_k = r_k - y_k
\]
Uso:
- señal primaria para control,
- base de cálculo para métricas agregadas.

## 3.2 Error RMS
\[
\mathrm{RMS}(e)=\sqrt{\frac{1}{N}\sum_{k=1}^{N} e_k^2}
\]
Interpretación:
- penaliza errores grandes,
- indicador robusto de calidad de seguimiento.

## 3.3 Error máximo absoluto
\[
e_{\max}=\max_k |e_k|
\]
Interpretación:
- captura peor desviación observada,
- útil para seguridad y límites de operación.

## 3.4 Tiempo de establecimiento (settling time)
Definición operativa:
- tiempo hasta que \( |e_k| \leq \Delta \) y permanece dentro de banda \(\Delta\) (ej. `STABLE_ERROR_BAND_CM`) durante una ventana mínima.

Interpretación:
- rapidez de convergencia del sistema.

## 3.5 Porcentaje de saturación de actuadores
Para cada muestra, se detecta saturación cuando algún servo alcanza límite de delta configurado respecto al centro.

\[
\mathrm{sat\_pct} = \frac{N_{\text{sat}}}{N} \times 100
\]

Interpretación:
- alto valor sugiere control agresivo, límites mecánicos o perturbaciones severas.

## 3.6 Frecuencia efectiva de lazo
\[
f = \frac{1}{T_s}
\]
Con `CONTROL_PERIOD_MS = 20 ms`, nominalmente:
\[
f \approx 50\ \text{Hz}
\]

---

## 4. Modelo PID discreto (visión simplificada)
\[
u_k = K_p e_k + K_i \sum_{i=1}^{k} e_i T_s + K_d\frac{e_k-e_{k-1}}{T_s}
\]
Aplicación:
- `Kp`: reacción al error actual.
- `Ki`: elimina sesgo permanente.
- `Kd`: amortigua cambios bruscos.

---

## 5. Criterios sugeridos de aceptación (académicos)
Valores orientativos (ajustables por montaje físico):

- Escenario nominal:
  - `RMS error` bajo (ej. \(\le 1.0\sim1.5\) cm)
  - `max abs error` controlado
  - `sat_pct` moderado/bajo
- Perturbación:
  - incremento transitorio aceptable
  - recuperación sin inestabilidad sostenida
- Saturación:
  - saturación detectable y reportada
  - operación segura sin bloqueo
- Falla sensor:
  - emisión de `FAULT`
  - transición a estado seguro

---

## 6. Ejemplo de cálculo rápido
Si en 5 muestras: \(e=[0.5, -0.2, 0.8, -0.4, 0.1]\) cm

\[
\mathrm{RMS}=\sqrt{\frac{0.25+0.04+0.64+0.16+0.01}{5}}
=\sqrt{0.22}\approx 0.469\ \text{cm}
\]

\[
e_{\max}=0.8\ \text{cm}
\]

---

## 7. Relación con telemetría serial
Campos relevantes:
- `DATA`: tiempo, distancia, error, servos
- `METRIC`: `rms_cm`, `max_cm`, `settle_s`, `sat_pct`, `samples`
- `FAULT`: estado de falla y seguridad

Estos datos permiten análisis posterior en hoja de cálculo o scripts externos.

---

## 8. Buenas prácticas de medición
- Mantener condiciones de prueba repetibles.
- Registrar timestamp y escenario.
- No cambiar múltiples parámetros a la vez durante tuning.
- Conservar evidencia bruta de serial y resultados resumidos.

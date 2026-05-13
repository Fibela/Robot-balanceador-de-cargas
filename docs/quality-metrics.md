# Métricas de Calidad del Sistema

## 1. Objetivo

Definir cómo evaluar el desempeño del robot balanceador de cargas de forma objetiva y repetible.

## 2. Métricas implementadas en firmware

1. **Error RMS angular (X, Y)**  
   - Unidad: grados (°)  
   - Definición: raíz media cuadrática del error respecto a la consigna.

2. **Error absoluto máximo (X, Y)**  
   - Unidad: grados (°)  
   - Definición: peor desvío observado durante la prueba.

3. **Tiempo de asentamiento (X, Y)**  
   - Unidad: segundos (s)  
   - Definición: instante en que el error entra por primera vez en banda ±2°.

4. **Saturación del actuador**  
   - Unidad: %  
   - Definición: porcentaje de muestras donde PWM alcanza el límite.

5. **Muestras válidas totales**  
   - Unidad: cantidad  
   - Definición: número de iteraciones procesadas para métricas.

## 3. Métricas recomendadas para informe semestral

Aunque no todas están en firmware base, se recomienda incluir:

- **Disponibilidad del lazo** = ciclos válidos / ciclos totales.
- **Tiempo de recuperación ante perturbación** (golpe o carga súbita).
- **Consumo aproximado** (corriente promedio de actuadores).
- **Capacidad de carga útil máxima estable** (kg).

## 4. Criterios de aceptación sugeridos (universitario)

- RMS angular < 3.0° en pruebas nominales.
- Tiempo de asentamiento < 3.0 s.
- Saturación < 25% durante operación normal.
- Sin fallos de seguridad en operación continua de 5 min.
- Error de reparto de carga aceptable según diseño (<15% sugerido).

## 5. Procedimiento de medición

1. Inicializar sistema sin carga y esperar estabilización.
2. Aplicar carga incremental (0.25 kg por paso).
3. Introducir perturbación leve manual y observar recuperación.
4. Registrar salida serial CSV (`DATA,...` y `METRICS,...`).
5. Analizar datos en Excel/Python y construir gráficas:
   - ángulo vs tiempo
   - control vs tiempo
   - carga izq/der vs tiempo
   - evolución de métricas

## 6. Riesgos y validez experimental

- Vibración mecánica puede contaminar IMU.
- Calibración HX711 impacta medición de carga.
- Fricción de actuadores altera tiempo de respuesta.
- Fuente de alimentación insuficiente produce resultados no repetibles.

## 7. Trazabilidad calidad <-> código

- `src/metrics.cpp`: cálculo de RMS, máximos, asentamiento, saturación.
- `src/safety.cpp`: lógica de seguridad y fault handling.
- `src/main.ino`: lazo principal, impresión de datos y métricas.

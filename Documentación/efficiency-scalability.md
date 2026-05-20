# Eficiencia, Escalabilidad y Límites Técnicos

## 1. Objetivo
Analizar el comportamiento del software de control en términos de eficiencia de ejecución y capacidad de crecimiento (escalabilidad), considerando restricciones de Arduino Uno.

---

## 2. Contexto de plataforma
Arduino Uno R3 (ATmega328P):
- 16 MHz
- 2 KB SRAM
- 32 KB Flash (aprox.)
- recursos de temporización y E/S limitados

Implicación:
- Se requiere código compacto, predecible y con asignación de memoria controlada.

---

## 3. Eficiencia temporal (tiempo real básico)

## 3.1 Periodicidad de lazo
El sistema opera con periodo fijo:
- `CONTROL_PERIOD_MS = 20 ms` (~50 Hz)

Ventaja:
- comportamiento determinista para control.

Riesgo:
- si el tiempo de ciclo excede 20 ms, se degrada la respuesta y la estabilidad.

## 3.2 Costos dominantes por ciclo
- lectura HC-SR04 (`pulseIn` con timeout),
- cálculo PID,
- actualización de 4 servos,
- impresión serial.

Observación:
- el mayor jitter suele venir de sensado y serial.

## 3.3 Recomendaciones
- limitar longitud/frecuencia de prints seriales si hay sobrecarga,
- evitar cálculos costosos innecesarios por ciclo,
- mantener timeout de sensor coherente con rango físico real.

---

## 4. Eficiencia de memoria

## 4.1 Buenas prácticas aplicadas
- estructura modular sin buffers masivos dinámicos,
- uso de tipos simples (`float`, `int`, `uint8_t`),
- estado compartido mínimo entre módulos.

## 4.2 Riesgos comunes
- crecimiento de strings en `Serial.print` extensivo,
- acumulación de estructuras de logging no acotadas,
- incorporar librerías pesadas sin necesidad.

## 4.3 Recomendaciones
- mantener telemetría esencial,
- usar constantes en compilación para features opcionales,
- revisar consumo SRAM si se agregan sensores/módulos.

---

## 5. Escalabilidad funcional

## 5.1 Escalar en frecuencia de control
Pasar de 50 Hz a mayor frecuencia requiere:
- reducir latencia de sensado,
- minimizar serial por ciclo,
- validar estabilidad PID bajo nuevo \(T_s\).

## 5.2 Escalar en número de sensores/actuadores
Agregar más entradas/salidas aumenta:
- carga de CPU,
- uso de memoria,
- complejidad de sincronización.

En Uno, la escalabilidad es limitada; para ampliaciones grandes conviene migrar MCU (p. ej. MEGA/ESP32).

## 5.3 Escalar en complejidad algorítmica
Incorporar filtros avanzados, observadores o control multivariable:
- puede exceder holgura temporal del Uno,
- exige profiling y posible refactor por etapas.

---

## 6. Indicadores prácticos de eficiencia
- estabilidad de periodo efectivo del lazo,
- porcentaje de saturación (`sat_pct`),
- tasa de fallos sensor/timeout,
- continuidad de telemetría sin bloqueos,
- temperatura/consumo eléctrico de actuadores en pruebas prolongadas.

---

## 7. Estrategia de optimización incremental
1. Establecer baseline con escenario nominal.
2. Medir métricas (`rms`, `max`, `settle`, `sat`).
3. Ajustar un parámetro por vez (PID, límites, mezcla).
4. Repetir pruebas en perturbación/saturación/falla.
5. Congelar configuración estable y documentar cambios.

---

## 8. Conclusión
El diseño actual es eficiente para alcance académico en Uno (1 sensor + 4 servos), con margen suficiente para validación y tuning.  
La escalabilidad existe pero está condicionada por límites de CPU/SRAM y latencia de E/S; cualquier crecimiento relevante debe planificarse con métricas de desempeño y posible migración de hardware.

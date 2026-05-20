# Ingeniería del Sistema — Robot Balanceador de Cargas (Prototipo 1)

## 1. Propósito del documento
Definir la base técnica del sistema de control para plataforma balanceadora sobre Arduino Uno, estableciendo arquitectura, conceptos fundamentales y teoría de control aplicada.

## 2. Alcance técnico
El sistema se orienta a:
- Control de plataforma en ejes Y/Z (mezcla de mando en 4 servos SG90).
- Sensado de distancia con HC-SR04.
- Ejecución de lazo de control periódico.
- Seguridad por pérdida de sensor / E-STOP.
- Telemetría por serial para evaluación cuantitativa.

No incluye en el flujo activo: IMU MPU6050 y celda HX711 (módulos heredados).

## 3. Arquitectura funcional

### 3.1 Bloques principales
1. **Sensado**: `DistanceHcsr04`
2. **Control**: `PidController`
3. **Actuación**: `MotorDriver` (4 SG90)
4. **Seguridad**: `SafetyManager`
5. **Métricas**: `MetricsCollector`
6. **Orquestación**: `main.ino`

### 3.2 Flujo por ciclo de control
1. Adquirir distancia (real o simulada).
2. Calcular error: `e(t) = referencia - medición`.
3. Validar estado seguro (sensor + límites + E-STOP).
4. Si seguro:
   - PID de distancia
   - Mezcla a ejes Y/Z
   - Aplicar comando a servos
5. Si no seguro:
   - detener actuadores
   - reset controlador
   - emitir `FAULT`
6. Actualizar métricas y emitir telemetría.

## 4. Definiciones y glosario
- **Lazo cerrado**: realimenta la salida medida para corregir el error.
- **Setpoint**: valor objetivo (distancia objetivo).
- **Error instantáneo**: diferencia entre objetivo y medición.
- **Saturación**: limitación del actuador por topes físicos/lógicos.
- **Settling time**: tiempo para entrar y permanecer en banda de error.
- **Fail-safe**: transición a estado seguro ante falla detectada.

## 5. Teoría de control aplicada

## 5.1 Modelo de error
\[
e(t) = r(t) - y(t)
\]
donde:
- \(r(t)\): distancia objetivo,
- \(y(t)\): distancia medida por HC-SR04.

## 5.2 Ley PID
\[
u(t) = K_p e(t) + K_i \int e(t)\,dt + K_d \frac{de(t)}{dt}
\]
Implementación discreta en periodo `CONTROL_PERIOD_MS`.

## 5.3 Mezcla de ejes y actuadores
La salida PID se proyecta a ejes Y y Z mediante ganancias de mezcla:
- `controlY = pidOut * AXIS_MIX_Y_GAIN`
- `controlZ = pidOut * AXIS_MIX_Z_GAIN`

Luego se transforma a 4 servos (frontal/trasero/izq/der) respetando:
- límites [0°, 180°],
- centro de operación (90°),
- delta máximo permitido.

## 5.4 Estabilidad práctica
El sistema busca estabilidad empírica mediante:
- frecuencia de lazo fija (20 ms),
- ganancias PID calibradas,
- límites de seguridad,
- observación de métricas seriales (`rms_cm`, `max_cm`, `sat_pct`, `settle_s`).

## 6. Operación y uso
- Modo real: `SIMULATION_MODE=false`.
- Modo simulación: `SIMULATION_MODE=true` + escenario.
- Salida por serial a 115200 para inspección y evidencia.

## 7. Criterios técnicos mínimos de aceptación
- Arranque consistente (`EVENT,BOOT,ok`).
- Flujo continuo de `DATA`.
- Reporte periódico de `METRIC`.
- Activación segura de `FAULT` ante falla.

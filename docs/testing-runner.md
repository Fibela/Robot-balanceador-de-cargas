# Runner C++ para validar logs de pruebas (CP/EX)

Este documento define cómo usar `tools/test_runner.cpp` para validar evidencias seriales
generadas durante pruebas en Arduino IDE.

## 1) Objetivo

Automatizar la verificación de logs (`DATA`, `METRIC`, `FAULT`, `EVENT`) y emitir
resultado `PASS/FAIL` por escenario.

> Nota: este runner valida **evidencia serial ya capturada**.  
> La ejecución sobre hardware (subir firmware, mover actuadores, E-STOP) sigue siendo manual.

---

## 2) Compilación del runner (Windows, g++)

Si tienes MinGW/MSYS2 con `g++`:

```bash
g++ -std=c++17 -O2 -o tools/test_runner.exe tools/test_runner.cpp
```

Si no tienes `g++`, puedes:
- instalar MSYS2/MinGW-w64, o
- compilar desde Visual Studio Developer Command Prompt con `cl` (adaptando comando).

---

## 3) Uso básico

```bash
tools/test_runner.exe --scenario CP-01 --log docs/evidence/cp01.log
tools/test_runner.exe --scenario CP-03 --log docs/evidence/cp03.log
tools/test_runner.exe --scenario EX-03 --log docs/evidence/ex03.log
```

Salida esperada:

```text
SCENARIO=CP-01
RESULT=PASS
DETAILS=BOOT=1, DATA=120, METRIC=12
```

Códigos de salida:
- `0`: PASS
- `10`: FAIL del caso
- `1/2/3`: error de uso/archivo/escenario no soportado

---

## 4) Casos soportados actualmente

- `CP-01` (arranque limpio): requiere `EVENT,BOOT,ok`, `DATA` y `METRIC`.
- `CP-03` (integridad telemetría): requiere `DATA`, `METRIC`, y campos `rms_cm` + `sat_pct`.
- `EX-03` (fallo sensor): requiere presencia de `FAULT`.

---

## 5) Flujo recomendado completo

1. Ejecutar prueba en Arduino IDE (según `docs/testing-terminal.md`).
2. Guardar salida serial en `docs/evidence/<caso>.log`.
3. Ejecutar runner para el caso.
4. Registrar resultado en `docs/test-evidence-template.md` (o reporte final).

---

## 6) Próxima ampliación sugerida

Extender `tools/test_runner.cpp` para:
- `CP-02`: validar convergencia básica (ventanas de error).
- `EX-01`: detectar recuperación tras perturbación.
- `EX-02`: validar incremento de `sat_pct` sin pérdida de telemetría.
- `EX-04`: validar secuencia de E-STOP con eventos/FAULT esperados.

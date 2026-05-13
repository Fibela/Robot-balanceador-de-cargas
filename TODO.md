# TODO - Proyecto Robot Balanceador de Cargas (Plataforma)

## Estado previo (histórico)
- [x] Crear estructura de repositorio GitHub completa.
- [x] Implementar firmware principal en C++ (Arduino).
- [x] Implementar módulos de control/sensado/seguridad/métricas.
- [x] Agregar configuración de hardware y parámetros ajustables.
- [x] Documentar criterios de calidad de sistema y métricas.
- [x] Escribir README completo (instalación, uso, calibración, pruebas).
- [x] Incluir BOM de componentes accesibles para proyecto universitario.

## Replanificación aprobada (datos reales del equipo)
- [x] Alinear documentación al escenario actual:
  - Arduino Uno R3
  - 4x SG90
  - HC-SR04
  - batería/fuente dedicada para servos
  - pruebas de software por terminal (simulación operativa)
  - masa objetivo de validación: 36 g
  - ejes de trabajo: Y/Z
  - estándares: ISO/IEC 25010 + IEEE 29119

## Tareas de actualización documental (completadas)
- [x] Actualizar `README.md` con arquitectura, flujo de software, cableado para Uno R3, ejecución y plan de evidencias.
- [x] Actualizar `docs/quality-metrics.md` con modelo de calidad ISO/IEC 25010 y KPIs medibles en simulación por serial.
- [x] Actualizar `docs/test-plan.md` con diseño de pruebas IEEE 29119 (casos, pasos, criterios PASS/FAIL, registro de resultados).
- [x] Verificar consistencia transversal de terminología y métricas entre todos los `.md`.
- [x] Dejar checklist final para entrega semestral (calidad de sistemas).

## Evidencias requeridas para cierre académico (pendientes de ejecución en laboratorio)
- [ ] Logs seriales de ejecución (`DATA`, `EVENT`, `METRIC`).
- [ ] Matriz requisito → prueba → métrica → evidencia.
- [ ] Tabla de resultados por caso de prueba.
- [ ] Conclusiones técnicas y siguientes iteraciones de software.

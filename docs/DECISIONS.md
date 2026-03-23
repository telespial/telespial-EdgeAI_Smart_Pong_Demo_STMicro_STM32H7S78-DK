# Technical Decisions Log

## 2026-03-22
- Decision: Keep the copied NXP project as functional baseline reference.
- Rationale: The game/platform split reduced port risk and sped up bring-up.
- Impact: STM32 implementation could focus on platform replacement with minimal gameplay rewrites.

- Decision: Treat failsafe/ as the authoritative golden and failsafe restore store.
- Rationale: Board regressions require one-command rollback without rebuilding.
- Impact: Golden and failsafe_active binaries are versioned with checksums and scripted restore.

- Decision: Use CPU predictor backend in npu_hal_stub.c on STM32H7S78.
- Rationale: STM32H7S78 has no on-chip NPU; this keeps AI path active with measurable telemetry.
- Impact: L/M telemetry reports live timing while preserving a future hook for true ML runtime integration.

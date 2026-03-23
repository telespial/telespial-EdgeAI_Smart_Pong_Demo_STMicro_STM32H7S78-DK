# Risk Register - STM32 Smart Pong Port

## R1 - Touch Reliability Under Real Use
- Risk: Touch behavior may still be inconsistent in some board states.
- Mitigation: Run dedicated touch smoke pass and log reproducible failure conditions.

## R2 - Restore Point Drift
- Risk: Golden/failsafe files can become stale after code changes.
- Mitigation: Refresh failsafe artifacts after each validated milestone and verify SHA256 checksums.

## R3 - Persistence Sector Safety
- Risk: Flash sector choice for persistence can overlap code/data if memory map changes.
- Mitigation: Keep explicit linker/memory map documentation and validate before enabling persistent writes.

## R4 - AI Runtime Capability Gap
- Risk: No hardware NPU on STM32H7S78 may limit future ML model complexity.
- Mitigation: Keep CPU predictor path stable now and plan optional CMSIS-NN or X-CUBE-AI backend.

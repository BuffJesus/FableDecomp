# Game-component timing recovery, 2026-09-25

Continues the lifecycle work on land/boot-debake. The GFMain Play boundary is still
an integration call counter; these checks do not establish playable world entry.

## Recovered functions

- CStopWatch constructor (0062f740), Start (0062f7c0), StartZero (0062f800),
  Stop (0062f850), Reset (0062f8b0), GetElapsedSeconds (0062f900): genuine C++
  RELOCATION_MATCH. Shared layout is 32 bytes, including alignment padding.
- GetTicks (0062f790): shared inline implementation with an address-taking anchor
  to emit the out-of-line COMDAT. Corrected QueryPerformanceCounter to C linkage;
  its previous local declaration named a C++ function despite masked byte parity.
- CInterpolationInfo constructor (00415a60): MATCH across the actual 43-byte body,
  six real interpolation pairs plus WFServerCurrentTime. The larger manifest span
  includes neighboring functions and is not used as its oracle.
- CRoughFrameCounter constructor (009f8b10): MATCH, now modeled as the component's
  real member type rather than a generic two-word initializer. Its prior propagated
  CBaseIntelligentPointer label is replaced for this call site.
- CRoughFrameCounter::NewFrame (009f8ba0): 62-byte RELOCATION_MATCH. Retail imports
  WINMM timeGetTime and converts the unsigned delta to float before dividing 1000.0.

## Verification

All ten functions pass retail parity and behavioral fixtures. Clock queries are
controlled in the test executable; fixtures cover failed queries, running/stopped
state, 64-bit carry/subtraction, preserved padding, zero frame duration, unsigned
millisecond wraparound, and deltas above INT_MAX. Constructor tests use poisoned
storage and guards. The old rough-counter fixture only printed PASS.

Four negative controls are rejected: wrong StartZero tick reset, inverted Stop
condition, wrong interpolation initial time, and halved FPS numerator. GetTicks'
fixture references the source object's member-pointer anchor so it cannot pass
using only the header implementation.

All 118 boot leaves and all three component ABI checks pass. The full unmodified
Release bootstrap exits zero, including WinMain, GFMain phases, Stage 3 and visual
checkpoint. This full run preceded the final NewFrame declaration/leaf addition;
that addition has its own passing parity and fixture checks and changes no layout.
Final shared-header dependent checks found no regressions across 28 entries.

VC7.1/x87 lesson: a float-returning function can leave excess precision in ST0.
The elapsed-seconds fixture explicitly stores its result as volatile float before
comparing it with a rounded float expectation; no production arithmetic was changed.

Next: recover game-time/frame-smoothing dependencies and the remaining actual
Init/Run paths before connecting CGame::Play to the reconstructed runtime.

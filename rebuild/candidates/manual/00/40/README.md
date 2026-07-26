# GFInitialise recovery workspace

This address shard records the manual recovery work that promoted retail
`GFInitialise @ 0x004022B0` into the verified tree.

Retail disassembly corrected two generated assumptions:

- `GFInitialise` receives no caller-supplied parameters.
- It loads the fixed `GFInitialiseState` at `0x013B83D0` into `ECX` before
  calling the verified `GFInitialise_SetupProgressDisplay @ 0x00413120`.

VC7.1 Release status:

- behavior fixture: `PASS`
- retail length: 311 bytes
- candidate length: 311 bytes
- parity: `RELOCATION_MATCH`
- promoted source:
  `rebuild/src/compiled/00/40/global_GFInitialise_004022b0.cpp`
- promoted fixture:
  `rebuild/tests/00/40/global_GFInitialise_004022b0_test.cpp`

The matching source keeps one mutable `displayDimension` local across the
width and height clamps. Besides describing the shared operation clearly,
that source shape gives VC7.1 the same register allocation and instruction
scheduling as retail. Scratch build products belong under
`rebuild/build/gfinitialise-Release/`.

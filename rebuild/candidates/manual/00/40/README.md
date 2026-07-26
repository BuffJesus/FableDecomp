# GFInitialise recovery workspace

This address shard contains the reviewed manual candidate for retail
`GFInitialise @ 0x004022B0` and its focused behavior fixture.

Retail disassembly corrected two generated assumptions:

- `GFInitialise` receives no caller-supplied parameters.
- It loads the fixed `GFInitialiseState` at `0x013B83D0` into `ECX` before
  calling the verified `GFInitialise_SetupProgressDisplay @ 0x00413120`.

VC7.1 Release status:

- behavior fixture: `PASS`
- retail length: 311 bytes
- candidate length: 312 bytes
- relocation-masked bytes 0-219: identical
- remaining blocker: register allocation/instruction scheduling in the two
  display-dimension clamps

The candidate stays here until the complete object passes the retail parity
gate. It must not be copied into `rebuild/src/compiled/` or counted in the
verified percentage before then. Scratch build products belong under
`rebuild/build/gfinitialise-Release/`.

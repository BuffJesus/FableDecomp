# FableDecomp technical brief

This is the concise architectural orientation for the Fable: The Lost
Chapters reconstruction. It summarizes current repository evidence; generated
or third-party briefs do not override `README.md`, `rebuild/RUNNABLE.md`, or
the latest section of `docs/HANDOFF.md`.

## Mission and evidence model

The primary goal is a complete, readable, buildable reconstruction of the
native Win32 x86 `Fable.exe`. The work currently has three connected lanes:

1. recover and type the retail program;
2. reproduce individual functions with VC7.1 and prove their behavior/parity;
3. connect stable contracts into runnable checkpoints and, when sufficiently
   bounded, cohesive C++23 subsystems.

Native modding and editor tooling are major products of that work. A broad
x64 renderer/engine port remains future work, not a description of the current
boot executable. The only native x64 C++23 reconstruction module today is the
bounded multiplayer `CGameEvent` codec under `rebuild/modern/`.

Evidence grades must remain distinct:

- `MATCH`: complete retail bytes match.
- `RELOCATION_MATCH`: all non-relocation bytes and the relocation schedule
  match the retail oracle.
- behavior-gated: focused fixtures prove representative normal, boundary, and
  failure behavior.
- `INTEGRATION_COMPLETE`: an authored coordinator connects proven units and
  is tested end-to-end; it is not a retail byte-parity claim for that parent.

## Current measured status

The canonical generated metrics currently report:

- 49,568 catalogued retail functions;
- 5,588 curated VC7.1-compiled and behavior-gated sources;
- 5,440 verified functional or matching reconstructions (10.97%);
- 3,049 byte-identical reconstructions (6.15%);
- 257/257 observed `GFMain` direct-call sites proven;
- 10/10 authored `GFMain` phases callable.
- 0/10 `GFMain` phases promoted into a modern C++23 startup target; that pass
  has deliberately not started yet.

The 3,952-byte retail `GFMain` parent remains an authored ten-phase integration
seam. Its call graph and early-return behavior are closed, while remaining
work replaces subordinate runtime boundaries and eventually attempts parent
parity. These boot-path percentages must not be presented as whole-executable
coverage.

## Source architecture

The project deliberately separates:

| Lane | Location | Purpose |
|---|---|---|
| Agent intake | `lift/reports/...` | Generated evidence; never counted merely for structural PASS |
| Retail parity | `rebuild/src/compiled/...` and `rebuild/tests/...` | One VC7.1 translation unit and focused fixture per function |
| Integration | `rebuild/integration/` | Authored ownership and execution seams connecting proven retail units |
| Modern | `rebuild/modern/` | Human-facing C++23 modules derived from stable evidence |

Boundary functions are temporary, observable contracts around unrecovered
ownership, OS, or subsystem graphs. They are valuable differential-test and
modernization surfaces, but they are not automatically permanent public APIs
or mod extension points. Promote or remove them only after their retail owner
is reconstructed.

Phase-state instrumentation records calls, arguments, branch results, and
lifetime effects. It is part of the integration test oracle, not retail global
state.

## GFMain waterfall

`FableRunGFMainComplete` executes ten authored clusters in retail order and
stops on the same phase-level failure edges:

| Phase | Recovered responsibility |
|---|---|
| 1 | startup profile, shared `CSystemManagerInit` lifetime, project path/current directory, console registrar boundary |
| 2 | file-installer ownership, basic install, startup latch, async failure policy |
| 3 | language/settings path, persistence, IME settings, text alignment |
| 4 | retail-bank aliases and navigator policy |
| 5 | retail BIGB and development INI bank setup |
| 6 | definition table, save/checkpoint paths, permissions, fonts and text banks |
| 7 | Win32 inputs and display/region/resource configuration in the shared `CSystemManagerInit` |
| 8 | EULA and hardware configuration detection/release |
| 9 | system manager, engine options, IME, profiling, optional LUG-to-MET metadata |
| 10 | `GFInitialise`, startup text, game launch boundary, shutdown, error and registry closure |

Complete Stage 3 and visual builds now execute exact subordinate coordinators
for Phase 6 definition-table pathname/load ownership; Phase 7 window-title
ownership; Phase 8 EULA/configuration/release; Phase 9 system-manager
initialization, CIME initialization, and optional LUG-to-MET generation; and
Phase 10 `GFInitialise`/`GFUninitialise`.

The Phase 6 live route owns one recovered definition-table object and executes
the exact 8-byte pathname setter plus the exact 164-byte loader. Its isolated
file and stream shims prove pathname propagation, open flags, 16 KiB stream
construction, read, loaded/read-only flag publication, and balanced cleanup
without requiring a retail `names.bin` on the host. The same runtime owner
executes the exact 372-byte My Documents permission probe through its
successful create/open/delete sequence using memory-only handles and streams;
the focused fixture retains redirected-folder and create/open/delete failures.

The live `CSystemManager::Initialise` route consumes the same recovered
0xC8-byte init owner constructed in Phase 1 and populated in Phase 7. With the
recovered default `0x05` flags, the 998-byte exact coordinator performs
process/scratch setup and six allocations for display, input, drive, text,
mesh, and colours, including the display/mesh component handoff. Its focused
fixture additionally proves the early skip, window failure, drive failure,
and full eight-allocation success routes.

## Runnable and visual architecture

- `rebuild/build_bootstrap.ps1` is the canonical VC7.1 parity, behavior,
  integration, and visual build gate.
- `bootstrap_main.cpp` proves Stage 0 with the 75-byte byte-identical
  `MemCmp_Unsigned16 @ 0x00403C60`; it does not contain the 4,158-byte console
  registrar.
- `stage2_engine_boundary.cpp` supplies the broad authored dependency closure.
  Subsystem-specific ownership shims are being split into files such as
  `system_manager_runtime_boundary.cpp`, `phase8_runtime_boundary.cpp`, and
  `phase7_window_title_runtime_boundary.cpp`.
- `visual_boot_checkpoint.cpp` is an authored standalone Win32 interaction
  harness, not the recovered retail frontend parent.
- `visual_boot_d3d9.cpp` presents retail-derived local assets through D3D9.
- Frontend wheel/list/click input and manager condition/process ordering are
  recovered at an explicit callback boundary; the delete-confirmation frame is
  composed from retail banks/definitions, while native profile persistence is
  still deliberately unlinked.
- `retail_video_bridge.cpp` uses DirectShow and can select an explicitly
  generated local upscaled-video cache.
- `frontend_startup_sequence.cpp` is an authored seam for a recovered
  post-movie prefix; its actions remain boundaries.

The Render2D planner and draw-list adapter preserve recovered ordering and
state transitions behind an interface. They are useful graphics migration
membranes, but the current implementation and surrounding visual checkpoint
remain D3D9-oriented; no D3D12 or Vulkan backend is presently claimed.

## Near-term order

1. Replace remaining ownership-heavy `GFMain` boundaries, especially Phases
   3, 5, and 6 and the Phase 10 game/registry seams.
2. Continue progress-display and renderer ownership closure.
3. Pursue the exact 3,952-byte `GFMain` parent only when its stack owners and
   exception/cleanup schedule are sufficiently recovered.
4. Promote stable subsystems into `rebuild/modern/` with C++23 differential
   tests. Keep the VC7.1 parity lane permanently available as the oracle.
5. Treat x64 engine and modern graphics backends as later subsystem projects,
   with explicit pointer-width, handle, asset-format, and presentation gates.

## Canonical references

- `README.md`: public status and generated metrics.
- `rebuild/RUNNABLE.md`: generated boot dashboard and runnable claims.
- `docs/HANDOFF.md`: latest detailed checkpoint and resume state.
- `docs/SOURCE_ARCHITECTURE.md`: promotion and C++23 policy.
- `docs/FULL_DECOMP.md`: full-decomp strategy.
- `docs/COMPREHENSIVE_MODDING_ENVIRONMENT.md`: editor/tooling direction.

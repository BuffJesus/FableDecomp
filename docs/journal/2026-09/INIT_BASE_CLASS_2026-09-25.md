# CInitBaseClass, frame-smoother lifecycle and the CGame singleton, 2026-09-25

Continues land/boot-debake after `bd0d088` (frame smoothing). Still not Play wiring.

## What landed

Shared headers now model the real hierarchy from FableWin.pdb and retail vtables:

- `fable_cbase.h`: `CBase` (vtable 0x01231710, one slot). `fable_string_parser.h` now
  includes it instead of carrying its own copy. `fable_persistence.h` still models CBase
  as a bare vtable slot; don't include it alongside.
- `fable_init_base_class.h`: `CInitBaseClass : CBase` (8 bytes: vtable, `Valid`;
  vtable 0x0129A7CC = scalar deleting destructor 0x0099A490, `Validate` 0x0099A340).
  Two unnamed counters: the constructor bumps 0x013BCA18, `Uninitialise` drops
  0x013BCA14. Retail `Initialise` touches neither (fixture checks this).
- `CFrameRateSmoother : CInitBaseClass` (vtable 0x0122F168), `CGame : CInitBaseClass`,
  `CSystemManager` loses its 8-byte stand-in pad for the base.
- STLport vector gains `begin/end`, `erase` via `__copy_trivial` (its empty-range test
  survives in retail's `clear()`), `clear`, and an out-of-line `resize` (0x00442770 is the
  folded float/unsigned-int instantiation).

Genuine C++ now matching retail (all behaviour fixtures pass; 4 mutations rejected):

| addr | function | parity |
|---|---|---|
| 0099a310 | CInitBaseClass::CInitBaseClass (inlines CBase ctor) | RELOCATION_MATCH |
| 0099a340 | CInitBaseClass::Validate (retyped shim) | MATCH |
| 0099a350 | CInitBaseClass::Initialise | MATCH |
| 0099a360 | CInitBaseClass::Uninitialise (retyped shim) | RELOCATION_MATCH |
| 0099a410 | CInitBaseClass::~CInitBaseClass (retyped shim) | RELOCATION_MATCH |
| 0099a490 | CInitBaseClass scalar deleting destructor | RELOCATION_MATCH |
| 00412f60 | CGame::Uninitialise (retyped; was "CInitBaseClass::Uninitialise") | RELOCATION_MATCH |
| 00419747 | CFrameRateSmoother ctor (+ implicit dtor 0x419764, sdd 0x41977d) | RELOCATION_MATCH |
| 0049ba70 | CFrameRateSmoother::Init | RELOCATION_MATCH |
| 0049ba20 | CFrameRateSmoother::Uninitialise | RELOCATION_MATCH |

Details worth keeping:

- The CBase/CInitBaseClass bodies are one retail TU (0x0099A2D0..0x0099A4D0): the
  ctor inlines `CBase::CBase` (its vtable store is dead), the dtor inlines
  `Uninitialise` and `CBase::~CBase`. Each landed file defines the siblings it inlines.
- CFrameRateSmoother's destructor is the *implicit* one: a user-declared destructor adds a
  vtable store retail doesn't have. The 0x41xxxx TU is size-optimised (`pop ecx` cleanup).
- `Init` stores `0.1f` widened to double (0x01238B48 = 0.10000000149), not `0.1`.

## CGame singleton

0x013B83D0 is the global `CGame`: GFUninitialise calls 0x00412F60 on it, and
0x00412F60 is `CGame::Uninitialise` (a tail jump to the base). GFUninitialise now calls
`g_Game_013B83D0.Uninitialise()`; the runtime defines the object in
`integration/game_singleton_runtime_boundary.cpp` and links the real CGame/CInitBaseClass
leaves (four new bootstrap-verified leaves: 00412f60, 0099a310, 0099a410, 0099a340).
stage2_engine_boundary's fake `CInitBaseClass g_FableInitBase_013B83D0` is gone.

### Follow-up commit: CGame::Initialise

0x00413120, landed as "GFInitialise_SetupProgressDisplay", is `CGame::Initialise`
(`?Initialise@CGame@@QAE_NXZ`; docs/engine/EGOMP_DONOR_AUDIT.md already said so). It calls
CInitBaseClass::Initialise on `this`, clears `Quit` (+0x20C) and `CurrentGameComponent`
(+8), then installs a new progress display. The byte struct `GFInitialiseState` in
fable_boot.h was CGame seen as raw bytes, and `g_GFInitialiseState_013B83D0` was the same
singleton. Both are gone:

- `fable_cgame.h` now holds CGame (fable_game.h includes it). GFInitialise can't include
  fable_game.h: its local byte-model `CDisplayManager` clashes with
  fable_display_viewport.h's, and Ego_r's CDisplayManager (748 bytes) doesn't match
  retail's 0x2C8, so a shared model is separate work.
- GFInitialise calls `g_Game_013B83D0.Initialise() == true` (retail `cmp al, 1`);
  CGame::Initialise itself tests the base result plainly (`test al, al`); writing
  `== true` there emits `cmp al, 1` and misses.
- The leaf is renamed `CGame_Initialise_00413120.cpp`; its fixture checks the receiver,
  the two cleared fields, untouched neighbours, the rejected-base branch and the
  allocation-failure path.
- The progress-phase harness holds a `CGame*`; its stand-in for the base Initialise
  (toggle + call count) lives in its test, since retail's base can't fail.
- In the Stage 3/visual runtimes the base Initialise is still the system-manager
  boundary's counting stub; linking the real 0x0099A350 there means retiring
  `exactSystemPreInitialiseCalls` first.

Verified: 122/122 boot leaves, 0 regressions across 52 dependent catalog entries,
full bootstrap exit 0 (WinMain, progress setup, progress phase, Stage 3, visual),
OpenRetailBank link smoke 0 unresolved.

## Verification

- 122/122 boot leaves (4 new), component ABI gate, frame-smoothing gate.
- gate_header_dependents: 0 regressions across 44 dependent catalog entries.
- Full Release bootstrap exit 0 (WinMain, GFMain phases, Stage 3, visual checkpoint).
- OpenRetailBank link smoke: 32/32 objects, 0 unresolved.

Gotcha: when a source emits a vtable, the generated `??_G` can be longer than the target
and "longest symbol containing the leaf" picks it. Name ctor/dtor oracle rows with the
mangled prefix (`CInitBaseClass::??0CInitBaseClass`, `::??1...`).

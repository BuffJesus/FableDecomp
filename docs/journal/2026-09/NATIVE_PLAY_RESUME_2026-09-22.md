# Native engine restart — 2026-09-22

Worktree: `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
The shared `D:\Documents\FableTLC` tree is back on `feat/novi-script-recovery`;
concurrent script changes and the pre-existing compile reports were preserved.
The native worktree uses sparse checkout to avoid materializing all 45,697 tracked
files. Expand its sparse paths before working on another subsystem.

## Direction and completed increment

The user wants the native readable-C++ reconstruction resumed, with background
work only while using the screen. They explicitly accepted functionally equivalent
code with reasonable, explained byte differences instead of delaying integration
for exact parity. Retail remains the reference; exact/relocation matching and
functional integration must remain separate grades. No game, checkpoint, debugger,
or visible window was launched during this session.

`CGame::Play` at `0x00412F90` now uses ordinary C++ construction, virtual dispatch,
string lifetimes, component transitions, and quit handling. Its old assembly body
is retained in `rebuild/src/asm_bake/00/41/CGame_Play_00412f90.cpp` for comparison.
The shared `fable_game.h` now uses recovered PDB names for the four main-game init
fields and the game component, parameter buffer, and quit fields. Evidence:
`ghidra_out/struct_layouts_egor.tsv`, CMainGameComponentInit and CGame records,
cross-checked against Play's retail field accesses and constructor/destructor order.

VC7.1 RTM `/MT /GS /O2 /Oy` emits **394 bytes**, with the same **23 COFF relocation
offsets** as retail. The only non-relocation differences are:

| Offset | Retail | Readable C++ |
|---|---|---|
| +0xEB instruction (changed byte +0xEC) | `lea ecx,[esp+0xC]` | `lea edx,[esp+0xC]` |
| +0xEF | `push ecx` | `push edx` |

Both push the same initializer address. The next instructions push the game
pointer and put the allocated object in `ecx` before calling the same constructor.
This is an equivalent temporary-register choice, not an altered argument or call.
The oracle was independently re-read from installed retail executable SHA-256
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

**Grade: functional; strict byte status: DIFFER.** The function manifest's single
`retail_parity` cell is corrected from `RELOCATION_MATCH` to `DIFFER`. No coverage
numbers were edited or aggregate dashboard refresh run; those remain dated snapshots.
The bootstrap checker permits only this exact difference for this address, length,
instruction pattern, and relocation map. It prints `FUNCTIONAL_REGISTER_RESIDUE`,
never `MATCH` or `RELOCATION_MATCH`. All other differences remain failures.

## Verification and commands

Run from the native worktree:

```powershell
python tools/decomp_pipeline/check_cgame_play.py
python tools/decomp_pipeline/check_cgame_play.py --baseline
python tools/decomp_pipeline/check_cgame_play.py --require-byte-match
python -m unittest discover -s tools -p test_check_boot_object.py
```

The first two commands pass. The third deliberately returns 1 while the known
residue remains; this is the honest exact-parity check. Reports and object
disassemblies are generated under `work/cgame_play_check/{readable,baseline}`.
Strict runs write `strict-report.json`, leaving the normal acceptance report intact.
Only isolated console fixtures run, using hidden subprocesses. No retail process
is started and no save/profile/game-install files are written.

Both implementations pass seven scenarios: compile-definitions disposal, main
game startup and reverse string destruction, legacy frontend/component transition,
normal new frontend, already-quit entry, compile flag changed during Init, and
retired-component disposal on exit. Constructor doubles check the game pointer,
zero frontend initializer, and actual startup-path transfer. Six gate tests also
prove that the allowance rejects other addresses, lengths, relocation maps,
changed instruction patterns, and an extra changed byte at every body offset.

The full bootstrap/UI smoke has **not** run because it includes GUI/WinMain paths.
The actual bootstrap object checker accepts the readable object only with the
explicit allowance and rejects it in strict mode; the archived object still
relocation-matches. PowerShell parsing of the changed bootstrap script passes.
The bootstrap's Play leaf now uses the same explicit functional-residue allowance
and the default readable-C++ fixture. Exact assembly testing uses the dedicated
`FABLETLC_CGAME_PLAY_ASM_BASELINE` test define.

## Frontend startup connection — follow-up increment

Three more real translation units are linked and tested:

| Function | Retail / compiled bytes | Grade |
|---|---|---|
| CGameComponent constructor, 0051D8E0 | 49 / 49 | RELOCATION_MATCH |
| CNewFrontendGameComponent constructor, 0042EA8F | 295 / 324 | functional, DIFFER |
| CNewFrontendGameComponent::Init, 0042F75E | 47 / 48 | functional, DIFFER |

The constructor's extra bytes come from inlining the four main-game initializer
string constructors and different zero-store/register choices. Init loads its
font argument through a register and schedules argument pushes differently.
The observed dependency calls and values match retail; exact byte matching is
still open. The reviewed object fingerprints **and relocation target symbols**
are pinned in `rebuild/integration/frontend_startup_contract.json`, so accepting
functional code cannot silently accept new code or changed call destinations.

Recovered layout facts:

- Retail RTTI at primary vtable 01230CA0 / secondary 01230C94 proves inheritance
  through CGameComponent, CBaseClassNonCopyable and CBaseClass, with a
  CDeviceResetCallback secondary base at +4. The typed declarations now model it.
- CGameComponent is 0x10 bytes, with Quit/Running/Game at +8/+9/+C. The old
  `CAIStateGroup_GazeAtHome` label on 0051D8E0 was propagated incorrectly. Its
  manifest identity and durable `function_overrides.tsv` correction are updated.
- The donor frontend is 0x150 bytes; retail is 0x148. Each retail vector is 12
  bytes, versus 16 in the donor. FileNames is +0x6C, FileTimes +0x78, SaveGame
  +0x84; later fields shift by eight bytes, including XMVCodeLoaded +0xB1,
  m_fLastRealUpdateTime +0xC0 and WindowsMediaPlayerInstalled +0x144.
- The constructor deliberately does not initialize input, AVI rectangles, or
  the two float frame timestamps. Poison-filled storage tests preserve this
  behavior instead of concealing it with a whole-object memset.
- Init ends at 0042F78D. The 99-byte next-catalogued-function range contains
  unrelated aligned leaves; the real Init oracle is only 47 bytes.
- Console Init receives **ToggleChar 0x60, ToggleKey 0x29, font**, not dimensions.
  The accepted prototype, PDB ToggleChar/ToggleKey/PFont fields, and retail
  009ED190 byte/dword/dword stores independently confirm the argument meanings.

The small storage types in `fable_frontend_component_types.h` model empty counted
pointers and retail vectors; they do not yet implement release, growth, or the
frontend's shutdown. Strings retain external method dependencies; stopwatches
were subsequently reconstructed as recorded below.
Do not treat the fixture destructor or Run replacement as recovered engine code.

Run the complete offline check:

```powershell
python tools/decomp_pipeline/check_frontend_startup.py
python tools/decomp_pipeline/check_frontend_startup.py --require-byte-match
```

Normal mode passes; strict mode returns 1 because the two functional bodies differ.
The checker also runs the seven-scenario Play gate. Its own fixture covers direct
construction/Init with a supplied font, the same with a null font, and a linked
**real Play -> real base/frontend constructors -> real Init -> test Run** path.
It verifies primary and secondary virtual dispatch, 20 field offsets, default
values, deferred initialization, ordered time/timer/string dependencies, singleton
publication, XMV flag ordering, console arguments, owning game, and Play's quit
latch. The bootstrap script invokes this check; its PowerShell syntax passes.
Reports/disassemblies live under `work/frontend_startup_check/`. No full bootstrap,
visual smoke, game, debugger, or foreground process ran. All new function manifest
rows report their actual byte grade; aggregate dashboards remain historical.

## Resume next

Do not resume the old general smallest-function crawl or spend another session on
the ECX/EDX choice by default. Bounded experiments already tried 55 CPU/optimization
flag combinations, branch/scope/allocation/factory forms, and initializer/ABI
declaration variations; none closed the two-byte residue. Local experiments remain
in the shared tree's ignored `work/cgame_play_resume/`.

New frontend construction and Init are now covered. Next recover a useful slice
of Run `0x0042EC7C` (2,191 bytes, through its ret 4 at 0042F508): startup bank/movie
preparation, the frame-loop dependencies, and component-transition/shutdown
ownership. Its constructor's engine services and Run itself are still doubles in
the offline integration fixture. Avoid claiming their runtime closure from this test.
Main-game constructor `0x00418DCA` and legacy frontend constructor `0x00496070`
are the alternative branches. Static source/ABI/dependency work can proceed now;
runtime capture and frontend visual verification wait until the screen is available.
`FableGFMainPhase10PlayBoundary` is still a counter stub. Do not replace it with a
blind call until a real game object and component dependency chain exist.

## Stopwatch dependency and visual recheck

Implemented readable CStopWatch constructor, Start, StartZero, Stop, Reset and
GetElapsedSeconds, and moved the existing GetTicks implementation onto the shared
32-byte class contract. Ego_r fields at offsets 0/8/16/24 agree with retail.
The six new methods are functional, not byte-matching. GetTicks remains a
36-byte relocation match. Compiled/retail body sizes respectively:
constructor 74/70, Start 20/63, StartZero 28/77, Stop 57/87, Reset 33/76,
GetElapsedSeconds 72/93. Most differences arise because retail inlined GetTicks
while these translation units call its single readable definition. Constructor
float-constant emission and register scheduling also differ. The compiled
elapsed conversion still uses x87 FILD/FMUL without an intermediate double store.

`python tools/decomp_pipeline/check_stopwatch.py` passes. The strict
`--require-byte-match` invocation deliberately exits 1. Reviewed fingerprints,
retail-body hashes, and relocation symbols live in
`rebuild/integration/stopwatch_contract.json`. Fixture coverage includes stopped
operations without counter queries, pause/resume, repeated Start, StartZero,
running/stopped Reset, nonmutating elapsed queries, high-word ticks/frequency,
low-word borrow/carry, failed counter calls (including negative elapsed time),
and failed frequency fallback. Only OS clock imports are replaced; no stopwatch
method is mocked. The fixture's import data symbols are renamed with objcopy
before linking, because /alternatename allowed the CRT to resolve actual kernel32
clocks first. Production objects are never rewritten.

The startup gate runs the timer gate and links its real constructor. Its three
frequency queries replace the previous constructor double, preserving ordered
initialization checks and checking all three final periods. Full startup passes.
Run remains an explicit double. See `docs/engine/FRONTEND_RUN_RECOVERY.md` for the
retail control-flow map and next recovery boundaries.

The user then explicitly allowed screen use while away. Launched an isolated copy
of the existing 2026-08-19 VisualCheckpoint build, SHA256
`b37f6935e25d80a3c39b467535f787088935ececae0dbd1c1a0488de84ddfd3e`, with its loose
frontend atlases, `--skip-boot-videos --retail-frontend-reference-size`.
Visually inspected 1024x768 captures of title, main menu, Options, Audio Options,
Redefine Keys before/after scrolling, and Select Profile. Those screens rendered
and mouse navigation worked; no profile was created/deleted and no settings were
applied. Closed the process normally. Captures and the local navigation helper are
under `work/visual-resume/` in this worktree. This was a baseline scaffold smoke
check, not a newly built executable and not evidence that native Run is complete.
No retail game or debugger was launched.

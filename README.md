# Fable: The Lost Chapters — Decompilation & Native Modding

A reverse-engineering project on the **native PC** `Fable.exe` (Fable: The Lost Chapters, Steam).
**End goal: a full decompilation** — a complete, readable, *buildable* C/C++ source reconstruction of
the game (see `docs/FULL_DECOMP.md`) — with a deep native modding surface as the natural dividend.

> **This is not a port project.** TLC is already a native x86-32 Windows game — there is nothing to
> recompile (unlike the sibling **Fable2RE**, which statically recompiles an Xbox 360 PowerPC binary).
> This is pure static analysis + live instrumentation of a **clean, unpacked PE32** (ImageBase `0x400000`,
> MSVC 2003 / VC7.1, ships `msvcr71.dll`).

## Where the project stands

Two milestones run in parallel: a near-complete **analysis database** (names + types for navigation and
reasoning) and an early-stage **buildable reconstruction** (source that compiles under the original
compiler and matches retail bytes). The reconstruction is deliberately *not* counted until it compiles.

| Track | Metric | Status |
|---|---|---|
| Analysis DB | Functions catalogued | **49,552** |
| Analysis DB | Mechanically named (no `FUN_*`) | 100.000% |
| Analysis DB | Usable reconstruction/navigation names | 99.211% |
| Analysis DB | Calling convention known | 77.676% |
| Analysis DB | Complete non-`undefined` prototype | 69.053% |
| Reconstruction | Curated sources, VC7.1-compiled **and** behaviour-gated | **4,917** |
| Reconstruction | Retail `.text` match (exact + relocation-normalized) | **4,586** (9.25%) |
| Reconstruction | — of which byte-**identical** (no relocation masking) | 2,687 (5.42%) |
| Reconstruction | Compiled sources still honestly `DIFFER` | 203 |
| Reconstruction | Compiled rows lacking a Ghidra function-start oracle | 128 |
| Auto-RE intake | Generated candidates / structural checker PASS | 632 / 619 |
| Boot path | GFMain direct-call sites proven | **40 / 257** (15.56%) |
| Boot path | Callable authored GFMain phases | **2 / 10** (20.00%) |
| Boot path | Current Phase 3 direct calls proven | **21 / 34** (61.76%) |

Counts above are from the 2026-07-26 canonical refresh: the VC7.1 compile/behaviour catalog,
`rebuild/compile-gate/retail-parity.json`, and `rebuild/COVERAGE.md`. Generated agent code is tracked
separately and is never counted as reconstructed merely because a structural checker accepted it.
The successful refresh also synchronizes this table automatically; GitHub is updated at reviewed
checkpoints rather than publishing live, unreviewed queue output.
The first **5%** compiled-byte-match milestone (2,478 functions) is passed; current verified retail
parity is **9.25%** of the 49,552-function catalog. The lower match count than an earlier README is an
audit reconciliation, not deleted source: the unified gate now exposes every `DIFFER` and missing
function-start oracle instead of mixing older mass-land and curated-subset totals.
The 9.25% figure is intentionally the strict, whole-executable denominator. The boot-path rows are
a second lens over the 3,952-byte GFMain coordinator: they measure proven direct call sites and
callable integration phases, not percentage of total engineering time. Repeated calls count
separately because every occurrence must be linked in the correct lifetime and control-flow
context.
**`docs/HANDOFF.md` is the authoritative resume point** — read its top section first.

The executable-integration lane has advanced beyond isolated object files. The original VC7.1
toolchain now builds and runs a Win32 GUI shell through the recovered `WinMain @ 0x00403480`;
that wrapper is a 141-byte relocation-normalized retail match and its focused fixture proves the
single-instance mutex, MicroThread stack handoff, duplicate-instance skip, and fastcall `GFMain`
boundary. Stage 3 now continues through authored GFMain Phase 1 and Phase 2 integration units. It
establishes the executable/project path with the promoted retail-matching functions, constructs
the recovered system defaults, crosses an explicit console-variable boundary, applies the basic-install and
failure-policy state, and exits at the Phase 3 boundary.
`rebuild/RUNNABLE.md` tracks the remaining boot gates, with engine initialization, data loading,
and the game loop ahead of installer, settings-menu, x64, or broad C++23 work.

The full `GFInitialise @ 0x004022B0` coordinator is now connected in the runnable
reconstruction. Retail proves that it has no caller-supplied parameters: it obtains the fixed
engine root from `0x009A4EC0` and loads progress state `0x013B83D0` into `ECX`. The readable
311-byte implementation is a relocation-normalized retail match, and focused behavior covers root
discovery, texture-depth fallback, display clamping, player registration, progress setup, and both
return paths. The visual executable now invokes that verified coordinator through an explicit
engine boundary before opening the project boot window.

Its `GFInitialise_SetupProgressDisplay @ 0x00413120` leaf independently remains a 128-byte
relocation-normalized match with allocation, failure, and counted-lifetime tests. Its review
corrected three misleading generated types: the allocated 0x88-byte object is
`CProgressDisplay` (proven by its vtable), not `C3DMeshStats`, and the forwarded smart pointer is
correspondingly `CCountedPointer<CProgressDisplay>`. The object now uses the recovered
`CProgressDisplay::CProgressDisplay @ 0x00499CE0`, a 163-byte relocation-normalized match that
types the complete 0x88-byte layout and constructs its three embedded string objects. The
integration boundary performs balanced recovered string teardown. The remaining boundary objects
stand in for the unrecovered engine singleton graph and renderer. The global retained owner is now
recovered too: `SetProgressDisplay @ 0x009E9FD0` is a 133-byte
relocation-normalized match that releases the prior counted object, retains the incoming
reference across the visible window lifetime, and releases it on shutdown. The handoff now
acquires that owner through the recovered 28-byte `GetProgressDisplay @ 0x009EA060` and queries
the real `active79` state through the exact four-byte
`CProgressDisplay::IsActive @ 0x0049B460`. The startup now also traverses the recovered
47-byte `CProgressDisplay::SetToDisplayText @ 0x00499A70` state transition before presentation.
A successful smoke boot exposes the retained state in the top-level window title. The authored window is still
scaffolding, not a claim that retail rendering or the game loop is recovered.

The `GFMain` Phase-1 filesystem pair is promoted as well:
`CAFile::GetProjectPath @ 0x00997510` (146 bytes) and
`CAFile::SetCurrentPath @ 0x009974F0` (30 bytes) are relocation-normalized retail matches.
Focused fixtures prove executable-directory discovery and that the OS working directory changes
before the engine's cached path. The folded `CWideString` and `CCharString` lifetime pairs are also
recovered and now live in a shared string-domain header. The PDB lineage also resolves the one-byte
retail no-op as `NProfileTimer::StartProfile`, bringing Phase 1 to 7/9 direct callees at
relocation match. The nested default `CCharString` constructor is proven as well, and
`CSystemManagerInit` now has readable typed source plus a passing behavior fixture for its
layout, defaults, region GUID, and subobject order. Its 265-byte build differs only by one
three-byte `lea` being scheduled earlier, so that residue is tracked explicitly rather than
misreported as an exact match.

Those dependencies are now linked together rather than only tested in isolation:
`FableTLC-Reconstruction-Stage2.exe` runs from the retail-matched `WinMain` through GFMain Phase 1.
The integration fixture proves the phase returns successfully, invokes the console-variable
boundary exactly once, and leaves a non-empty cached project path. The phase unit is clearly
marked as authored integration code—not a claim that the complete 3,952-byte retail GFMain matches.

`FableTLC-Reconstruction-Stage3.exe` continues through the authored Phase 2 sequence. Its fixture
proves readable state propagation, conditional installer creation, startup-latch handling, async
failure policy, and balanced counted ownership. Stage 2 remains independently runnable so the
Phase 1 checkpoint does not disappear as later phases are added.

There is now a real visible checkpoint as well:
`FableTLC-Reconstruction-VisualCheckpoint.exe` follows the retail-matched `WinMain` and the same
reconstructed Phase 1/2 startup path, runs the retail-matched progress-display setup leaf through
the authored `GFInitialise` tail phase, then opens a responsive 1280x720 Win32 window containing
the real retail `FRONTEND_BACKDROP_01` image when a local `frontend.big` is available. The build
decodes its Lionhead-LZO/DXT1 payload, crops the allocated surface to the authored 640x480 frame,
and embeds it without committing retail artwork; the project boot image remains the fallback.
The visual handoff consumes a balanced retail counted-pointer snapshot and the retail active-state
query before creating that window. Image presentation is still authored GDI scaffolding—not a
claim that the retail renderer, runtime archive loader, or game loop has been recovered. Build it
with `rebuild/build_bootstrap.ps1`, then launch it from `rebuild/build/bootstrap-Release/`.

### How close is a retail visual boot?

| Visible milestone | Current state | What remains |
|---|---|---|
| First actual retail image | **Runnable now: `FRONTEND_BACKDROP_01` is decoded from `frontend.big` and shown by the reconstructed executable** | Presentation currently uses the authored GDI window rather than Lionhead's runtime archive/texture/renderer stack. |
| First recovered retail progress setup | **Coordinator, setup, 0x88-byte constructor, retained owner, counted getter, active-state query, and text-mode transition are connected; the primary/fallback text-bank selector is recovered** | Populate the retail bank owners, integrate the corrected 418-byte `StartProgress`, and recover texture initialization. |
| Retail-rendered frame/frontend | **The 489-byte `RenderProgress` boundary is mapped and 15/17 direct dependencies are retail matches** | Recover the 5,101-byte retail-display builder and 3,344-byte 2D draw-list submitter, then replace the GDI bridge with the runtime render/present loop. |
| Retail intro video | **Farther than the first renderer frame** | Add the movie/Bink ownership and decode path, timing, audio, and frame presentation after renderer/display initialization. |

The “actual image” milestone is therefore reached, but “the game is rendering” is not. Retail
inspection and a reviewed Ghidra boundary repair establish one 418-byte
`StartProgress @ 0x00499AA0` function through `0x00499C41`; the former
`0x00499AAD` catalog row was a false mid-function start and is now excluded
reproducibly. The nearest honest engine milestone remains the game's progress display and its texture consumers, not the
intro movie or interactive menu. That is not one or two functions away: only two of ten GFMain
integration phases are callable, even though 40 of its 257 direct call sites and 21 of Phase 3's
34 sites are already proven. These dependency counts are more informative than converting them
into a date estimate.

Phase 2 recovery has reached all seven direct calls in retail order. PDB and donor lineage resolves
the repeated one-byte
`$E2` leaf as the compiled-out `NProfileTimer::EndProfile`, and the retail executable confirms the
17-byte `CBankFileAsync::SetEnableFailureHandling` policy encoder. A second 17-byte TLC-specific
leaf conditionally clears startup latch `0x013964A8`; its old BSim-derived `Draw` label was incorrect
and has been replaced with a conservative address-bearing name. These leaves and the reused
string/profile calls now relocation-match and pass focused behavior fixtures. The 69-byte
`CFileInstallerSingleton::Get` also matches retail exactly, including its lazy 160-byte allocation,
constructor, and counted-pointer assignment paths. Typing the caller's temporary counted-pointer
cleanup completed the Phase 2 ownership closure; Phase 3 settings and persistence are now the next
runnable boundary.

Phase 3 now has 21 of 34 direct calls proven. Fifteen are honest reuse of the already-matched
string/profile lifetime targets; another pair are seven-byte primary and secondary text-alignment
setters at `0x009BC890` and `0x009BC8A0`. Their adjacent retail globals remain address-bearing
until the settings caller proves whether they are channels or fields of one owner. Two additional
seven-byte cleanup leaves restore the shared `CBase` vtable; their address-bearing function names
preserve the still-unknown derived persistence owners.

The newest dependency also closes a naming trap. Donor PDB/BSim evidence called
`0x00415530` a virtual `CActionDoCreatureAction::GetActionName`, but the donor ABI loads a hidden
return pointer from the stack and ends in `ret 4`; TLC receives the hidden `CCharString` result in
`ECX` and ends in plain `ret`. The GFMain caller and adjacent language paths show that TLC's leaf is
a no-argument default-language factory. It is now readable typed C++, still 19 bytes after
relocation normalization, and its fixture proves construction of `"English"` with length `-1`
and balanced result destruction. The correction is recorded in
`rebuild/integration/abi_corrections.tsv` so later name imports cannot silently restore the donor
mistake.

The following GFMain call had the same failure mode: `0x0099E480` was labeled
`GetDefaultVal<CAnimationSet>`, even though its retail ABI takes a `CCharString` in `ECX` and a
hidden `CWideString` result on the stack. Donor PDB lineage and the complete retail call chain
confirm `CCharString::ToWideString`. Its readable 45-byte implementation, the 13-byte
`CWideString::CreateFromCharString` factory, and the 66-byte counted-storage copy constructor all
relocation-match retail and pass focused construction, sharing, copy, and destruction fixtures.
Both corrections are durable manual overrides and documented in the ABI-corrections ledger.

The next language-path pair is now identified without promoting a false match:
`0x0099BFF0` is the fastcall
`operator+(const wchar_t*, const CWideString&)`, assembling
`L"Data\\lang\\" + language`, and `0x0099BF30` is
`operator+(const CWideString&, const wchar_t*)`, appending
`L"\\lang_settings.txt"`. The readable VC7.1 forms reproduce the hidden-result ABI but still
differ from retail's inlined copy/destruction scheduling, so Phase 3 remains honestly at 21/34
until the byte/behavior gate closes.

The project-owner-provided [boot-screen concept](rebuild/assets/boot/fabledecomp_boot_concept.png)
remains the dependency-safe fallback. When a retail bank is found, the build instead extracts
`FRONTEND_BACKDROP_01` into the ignored build tree and converts it to the VC7.1/GDI resource.
Neither the decoded retail image nor generated bitmap is committed.

The unattended Wave 3 lane has moved from the co-op event/package codecs into ForgeFSE Quest
wrappers. The current refresh validates 452/452 recommended Quest bindings against their exact
CGSI vtable slots; generated wrappers remain reviewable intake until their ABI, behavior, and
retail bytes are independently proven. See `docs/HANDOFF.md` for the live batch and promotion
caveats.

A second unattended lane now advances the strict verified counter without using Ghidra.
`tools/run_local_parity_queue.ps1` selects fresh short functions, authors only deterministic
byte-implied VC7.1 forms, and sends them through the same retail-byte and focused-behavior gate
before landing. Its first two restored batches landed 97 authoritative manifest-backed matches.
The same pass found and removed speculative post-`ret` tails that lacked a known function start;
both selector and lander now reject that inflation path. Wave 3 also has a 48-hour cross-run
cooldown after two unresolved failures, preventing a handful of hard wrappers from consuming every
scheduled batch. The throughput diagnosis and the distinction between structural `PASS` and
verified parity are documented in `docs/FULL_DECOMP.md`.

The first installed hourly run added another 204 manifest-backed exact matches from four bounded
batches. Its incremental VC7.1 gate rebuilt the 204 changed rows in 32 seconds, and the new local
object/oracle fingerprint cache reduced the unchanged 2,154-row parity pass to 0.18 seconds.

The latest convergence pass added **330** more verified functions. Seven came from the final
ordinary 32-byte batches; a new pending-pattern replay then recovered 323 earlier misses after the
deterministic vocabulary learned global-object tail calls, optional global-pointer calls, and
small method/field wrapper sequences. Replay rejects addresses outside the canonical manifest,
deduplicates staged rows, and uses the same byte-and-behavior landing gate. This means a pattern
improvement now harvests old batches automatically instead of helping only newly selected code.

The first replay extension after that pass modeled a fixed object array as a typed reverse
pointer walk. It recovered seven 32-byte `CreateCRC` wrappers with behavior tests and
relocation-normalized retail matches, bringing the canonical totals to 3,163 compiled functions
and 2,836 verified matches (5.72%).

The first modern reconstruction proof of concept now lives in
`rebuild/modern/multiplayer/`: an x64 C++23 `GameEvent` model and codec using `std::span`,
`std::expected`, owned storage, explicit little-endian serialization, and focused malformed-input
and round-trip tests. It preserves the recovered single-event wire bytes while enforcing the
retail object's actual 32-byte payload capacity. This is a separate human-facing layer, not a
replacement for or parity claim about the VC7.1 one-function translation units.

## The cut multiplayer system

The retail PC binary retains a substantial disabled co-op implementation. It is more than a few
unused strings, but it is **not a ready-to-enable multiplayer mode**.

What survives:

- `CPlayerManager` manages four player slots and exposes a data-driven
  `IsMultiplayerGameActive` check.
- `CTCCoopSpirit` is a combat/movement entity for the extra player, with player-one through
  player-four definition slots.
- `CNetworkClient` is embedded in `CMainGameComponent`; its per-frame update is gated by byte
  `CNetworkClient+0x2662`. `InitialiseAsLocal` sets that gate and the required component back-pointer.
- `CGameEvent`, `CGameEventPackage`, and `CGameEventPackageSet` retain a dense replication protocol.
  An event is `[u16 id+flag][u8 player/subfield][u8 payload length][payload]`, while package sets add
  sequence numbers and event counts.
- The receive/apply path still rejects stale sequence numbers, forwards packages into the world and
  display engine, dispatches individual events, and integrates package sets with save/load.

What was cut or gutted:

- `CheckSync` reads a remote three-word synchronization record and computes the local world checksum,
  then discards every value. There is no comparison, desync latch, report, or recovery path.
- The tag-1 sync-event producer and the meanings of the other two synchronization words are not yet
  proven.
- A complete retail lobby/transport startup path has not been recovered. Network host/client setup
  and the original desync reaction still need reconstruction.
- Raw-poking the enable byte is unsafe: update forwarders dereference a back-pointer installed by
  `InitialiseAsLocal`, so setting only `+0x2662` can crash.

The practical revival order is therefore: seat a valid second player, initialize local mode through
the real initializer, confirm event sequence movement, rebuild synchronization checking, and only
then attach a modern transport. The byte-level contract, corrected addresses, evidence limits, and
revival plan are in [`docs/COOP_REVIVAL.md`](docs/COOP_REVIVAL.md).

## Why this is tractable
- `Fable.exe` is a clean MSVC PE32 — **no packer, no DRM stub** — Ghidra loads it directly.
- **FableScriptExtender (FSE)** already reversed the game's C++ scripting API: its **931-function
  manifest** (`refs/fse_api_manifest.json`) plus its local source seed Ghidra with real names, types,
  and engine call sites. The `ForgeFSE` binding lane pins those call targets back into the database.
- **Rich donor symbols.** BSim ports names from a PDB-symboled debug build (`ego_r.exe`, 28k PDB names)
  and from `FableWin.exe` (the Lionhead level editor, 164k names) — this is what took naming from 3.9%
  to ~100%. Runbook: `docs/BSIM_PORT.md`.
- **~20 years of community RE** (fabletlcmod.com, Fable Explorer, EgoCore) already solved the *data
  formats* — this project does the complementary *engine internals* work.

## The reconstruction pipeline (`rebuild/`)

Each function is promoted through an evidence gate, not asserted:

1. **Auto-RE lift** — an agent lane drafts candidate C++ from Ghidra decompiler output
   (`lift/reports/`), tracked as low-confidence until it compiles.
2. **Curated port** — a faithful, VC7.1-compilable translation lands in the address-sharded
   `rebuild/src/compiled/<aa>/<bb>/`
   with real declarations from `rebuild/include/`.
3. **Compile + behaviour test** — `rebuild/build_candidates.ps1` compiles each changed unit with the
   original **VC7.1 `cl.exe`** and runs a per-function behaviour oracle
   (`rebuild/tests/<aa>/<bb>/`). Unchanged passing products are reused only when their source, test,
   and shared headers are older; `-Force` performs a clean full-catalog audit.
4. **Retail parity** — `tools/compare_candidate_objects.py` disassembles the object and compares its
   `.text` against authoritative retail bytes (`rebuild/oracles/`, exported from Ghidra by
   `ExportFunctionOracle.java`), masking expected COFF relocation fields. Result: `MATCH`,
   `RELOCATION_MATCH`, or `DIFFER` — recorded in `rebuild/compile-gate/`. Unchanged object/oracle
   fingerprints reuse an ignored local cache; `--force` performs a clean comparison.

Promotion queues and the backlog are generated under `rebuild/backlog/`.

### Tooling

- **`tools/decomp_pipeline/`** — the promotion loop as reusable scripts: oracle extraction, disasm
  bundling, and `verify_and_land.py` (VC7.1 fixups + byte/behaviour gate + auto pragma-sweep + catalog
  wiring). See its README for the cycle and the resume point.
- **`tools/run_local_parity_queue.ps1`** — the Ghidra-free unattended promotion lane. It advances
  through bounded short-function batches, invokes deterministic authoring and the real byte/behavior
  landing gate, then refreshes canonical parity reports and this README after verified wins.
- **`tools/decomp_pipeline/crack_residue.py` + the diff-feedback refine loop** — for byte-parity
  residues the plain gate leaves as `DIFFER`. `crack_residue.py` sweeps mechanical
  semantics-preserving remedies (comparison-operand flip, liveness-shaping inline, pragma sweep); when
  those exhaust, an agent **refine loop** feeds the annotated retail-vs-built disasm diff back to an
  authoring agent with register-allocation nudging guidance, which cracks structural/allocation
  residues the mechanical remedies can't (and honestly labels the genuinely irreducible ones —
  register-allocation artifacts no VC7.1 source spelling can reproduce). `verify_residue.py` gates and
  overwrites landed sources on a win.
- **`tools/permuter/`** — a [decomp-permuter](https://github.com/simonlindholm/decomp-permuter)-style
  matcher on our toolchain, for the register-allocation / instruction-scheduling tail: a relocation-masked
  VC7.1 scorer, an automatic flag/pragma sweep, and libclang AST mutations (temp-introduction, reassoc,
  statement-split) with greedy + random multi-mutation search. Cracked several DIFFERs that plain
  compilation missed (many retail TUs were size-optimized). Requires `libclang`.
- **`tools/organize_workspace.ps1`** — non-destructive local housekeeping for loose root scratch
  objects/sources, RE-agent transcripts, address-sharded candidate/curated source, tests, and
  per-function build products. It preserves collisions and live worker safety; preview with
  `-WhatIf`. `rebuild/ARTIFACT_INDEX.tsv` provides address/module navigation.
- **`docs/SOURCE_ARCHITECTURE.md`** — the boundary between raw agent intake, the one-function
  VC7.1 retail-parity layer, and cohesive human-facing C++23 subsystem code.

## Layout
| Path | What |
|---|---|
| `docs/HANDOFF.md` | ▶ **Authoritative resume point — read first.** |
| `docs/PLAN.md` | The plan: goals, ecosystem integration, phased roadmap. |
| `docs/FULL_DECOMP.md` | The full-decompilation strategy (the primary goal). |
| `docs/FINDINGS.md` | Cited technical truth (cross-checked ≥2 sources). |
| `docs/SYSTEMS_ANALYSIS.md` | Per-subsystem maps + moddability verdicts. |
| `docs/TOOLCHAIN.md` | Exact commands: Ghidra import, GhidraMCP, FSE import, VC7.1 setup. |
| `docs/WORKSPACE_LAYOUT.md` | Public/local artifact boundaries and safe housekeeping. |
| `docs/SOURCE_ARCHITECTURE.md` | Address sharding, readable module design, and C++23 policy. |
| `docs/COOP_REVIVAL.md` | Retail evidence, historical context, and the grounded Alter Ego revival plan. |
| `docs/FABLE_TLC_REPORTED_BUGS.md` | Reported defects and the evidence template used before promotion to confirmed issues. |
| `rebuild/` | The buildable reconstruction: curated source, tests, oracles, compile gate, coverage. |
| `rebuild/RUNNABLE.md` | Staged executable-integration status and the retail boot-chain blockers. |
| `lift/` | Auto-RE agent lane: candidate reports, config, durable run state. |
| `ghidra_out/` | Decompile dumps + `labels_*.tsv` (the reproducible name DB source). |
| `tools/` | Ghidra RE script suite, FSE import, parity/dashboard tooling, Lua + asset tooling. |
| `refs/` | FSE manifest + runtime log, format references. |
| `work/`, `snapshots/`, `FSE/` | Local experiments, archives, and deployment backups (ignored). |

## Repository hygiene

This public repository preserves reconstruction source, documentation, automation, curated tests, and
compact derived analysis catalogs. It intentionally **does not** contain original game executables,
private PDBs, Ghidra/BSim databases, staged game archives, crash dumps, or generated build products —
those are reproducible local prerequisites, not redistributable project source. See `docs/TOOLCHAIN.md`
for the expected local setup. `.gitignore` enforces this; regenerate large BSim/PDB exports locally.

## Related projects on this machine
- **FableForge** (`D:\Code\FableForge`) — the active C++ modding toolchain that consumes this project's
  RE outputs (WAD/TNG/WLD/LEV/STB readers, `forge validate`). The downstream target.
- **Fable2RE** (`D:\Documents\Fable2RE`) — sibling; Xbox 360 Fable 2 recomp+decomp. Source of tooling.
- **FQT** (`D:\Code\FQT`) — the user's WPF quest editor over FSE; consumes extracted name/def tables.
- **EgoCore** — the completed data-format RE + headless extraction engine (asset/level/def/audio answer key).

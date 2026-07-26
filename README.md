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
| Analysis DB | Functions catalogued | **49,553** |
| Analysis DB | Mechanically named (no `FUN_*`) | 100.000% |
| Analysis DB | Usable reconstruction/navigation names | 99.211% |
| Analysis DB | Calling convention known | 77.660% |
| Analysis DB | Complete non-`undefined` prototype | 69.027% |
| Reconstruction | Curated sources, VC7.1-compiled **and** behaviour-gated | **1,850** |
| Reconstruction | Retail `.text` match (exact + relocation-normalized) | **1,523** (3.07%) |
| Reconstruction | — of which byte-**identical** (no relocation masking) | 914 (1.84%) |
| Reconstruction | Compiled sources still honestly `DIFFER` | 199 |
| Reconstruction | Compiled rows lacking a Ghidra function-start oracle | 128 |
| Auto-RE intake | Generated candidates / structural checker PASS | 573 / 565 |

Counts above are from the 2026-07-25 canonical refresh: the VC7.1 compile/behaviour catalog,
`rebuild/compile-gate/retail-parity.json`, and `rebuild/COVERAGE.md`. Generated agent code is tracked
separately and is never counted as reconstructed merely because a structural checker accepted it.
The successful refresh also synchronizes this table automatically; GitHub is updated at reviewed
checkpoints rather than publishing live, unreviewed queue output.
The first **1%** compiled-byte-match milestone (496 functions) is passed; current verified retail
parity is ~3.07% of the 49,553-function catalog. The lower match count than an earlier README is an
audit reconciliation, not deleted source: the unified gate now exposes every `DIFFER` and missing
function-start oracle instead of mixing older mass-land and curated-subset totals.
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

The first GFInitialise leaf is ready behind that boundary:
`GFInitialise_SetupProgressDisplay @ 0x00413120` is a 128-byte relocation-normalized match with
focused allocation, failure, and counted-lifetime tests. Its review corrected three misleading
generated types: the allocated 0x88-byte object is `CProgressDisplay` (proven by its vtable), not
`C3DMeshStats`, and the forwarded smart pointer is correspondingly
`CCountedPointer<CProgressDisplay>`. WinMain plus this leaf are two post-refresh promotions; the
canonical metrics table above will absorb them on the next queue-safe refresh.

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

The unattended Wave 3 lane has moved from the co-op event/package codecs into ForgeFSE Quest
wrappers. The current refresh validates 452/452 recommended Quest bindings against their exact
CGSI vtable slots; generated wrappers remain reviewable intake until their ABI, behavior, and
retail bytes are independently proven. See `docs/HANDOFF.md` for the live batch and promotion
caveats.

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
3. **Compile + behaviour test** — `rebuild/build_candidates.ps1` compiles each unit with the original
   **VC7.1 `cl.exe`** and runs a per-function behaviour oracle
   (`rebuild/tests/<aa>/<bb>/`).
4. **Retail parity** — `tools/compare_candidate_objects.py` disassembles the object and compares its
   `.text` against authoritative retail bytes (`rebuild/oracles/`, exported from Ghidra by
   `ExportFunctionOracle.java`), masking expected COFF relocation fields. Result: `MATCH`,
   `RELOCATION_MATCH`, or `DIFFER` — recorded in `rebuild/compile-gate/`.

Promotion queues and the backlog are generated under `rebuild/backlog/`.

### Tooling

- **`tools/decomp_pipeline/`** — the promotion loop as reusable scripts: oracle extraction, disasm
  bundling, and `verify_and_land.py` (VC7.1 fixups + byte/behaviour gate + auto pragma-sweep + catalog
  wiring). See its README for the cycle and the resume point.
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

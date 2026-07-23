# Full decompilation strategy — `Fable.exe`

*User directive (2026-07-17): the end goal is a **fully decompiled** game — a complete, readable,
buildable C/C++ source reconstruction of Fable: The Lost Chapters, not merely a modding surface.*

This doc is the realistic engineering plan for that. It is deliberately honest about scale, and
leans on automation because the target is large.

## Active reconstruction task list

- [x] Feed exact donor-PDB symbols into unattended reversals without taking the Ghidra lock.
- [x] Prove the first nav transition/linking candidates under VC7.1 with focused behavior tests.
- [x] Generate `rebuild/backlog/PROMOTION_QUEUE.md`, a continuously refreshed ranking of every
  uncompiled auto-RE result by checker/signature quality and estimated porting cost.
- [x] Validate the queue by promoting its first-ranked result, `GFGetBuildNumber2 @ 0x00401F30`.
- [x] Establish the public recovery repository at `BuffJesus/FableDecomp`, excluding original game
  binaries, PDBs, Ghidra databases, build products, and raw agent transcripts.
- [x] Promote `CEngineLandscapePatch::RelocateData @ 0x00BF3980`; its six-path behavior oracle passes
  and its 143-byte body is an exact relocation-normalized retail match.
- [x] Promote the dependency-free `Getter_FieldE0_FC @ 0x00662020`; its corrected fastcall prototype
  and 13-byte VC7.1 object are raw byte-identical to retail.
- [x] Build a 462-address ForgeFSE binding-verification queue and place it ahead of generic naming
  and prototype backlog work in the existing lock-aware unattended runner.
- [x] Promote `CNavQuadTree::InitialiseLines @ 0x00A7A5E0`; its grow/shrink/zero behavior oracle
  passes and its complete 102-byte VC7.1 instruction stream is a relocation-normalized retail match.
- [x] Complete the first bounded ForgeFSE reversal batch: 16/16 bindings reached structural PASS
  and were ingested into the generated candidate/signature/promotion reports.
- [ ] Ingest and review both completed `ConnectVerticalMapEdge` reconstructions; promote them only
  after their allocation/refcount and connector-vector behavior can be isolated in a safe harness.
- [ ] Take the next queue-ranked small candidate, currently `entry @ 0x00401067`, through VC7.1
  compile, behavior, and retail comparison; skip to `CNavQuadTree::IsAreaBlockedByLines @ 0x00A76F30`
  if CRT startup dependencies make the entry-point harness disproportionately expensive.
- [x] Promote the dependency-free ForgeFSE wrapper pair at `0x0088EDB0` and `0x0088EDA0` as one
  shared declaration/test batch; both virtual/const ABI identities and raw retail bodies match.
- [ ] Promote the next compile-sized ForgeFSE batch, beginning with `CancelRadialBlurFade @
  0x00890180`, while the unattended lane continues reversing direct-signature bindings.
- [ ] Tune `LinkToNeighbouringMaps @ 0x00A7A650` toward the retail x87/local schedule; its current
  implementation is behavior-verified but nine bytes longer than retail.
- [ ] Continue closing corrected ABI identities from decorated retail/donor symbols, treating the
  decorated `ABV` const-reference qualifier as authoritative when `llvm-pdbutil pretty` omits it.
- [ ] Promote in small dependency-sharing batches until the curated lane reaches 496 functions
  (the first 1% milestone), then begin module-level link replacement instead of isolated objects.

## Current unattended reconstruction loop (2026-07-22)

The active implementation is `lift/scripts/run_re_agent_wave3_queue.ps1`, launched every 15 minutes by the
existing `FableTLC Auto RE Wave 2` scheduled-task entry. Each run is deliberately bounded to 16
functions. It resumes its JSON ledger, exhausts the curated seed cluster first, then refills from
prototype gaps in the generated reconstruction backlog while excluding every address already seen
by the primary, retry, Wave 2, or Wave 3 ledgers. A 35-minute target watchdog and two-attempt policy
keep provider or Ghidra stalls from stopping later batches.

Queue transcripts are separated from durable state: new attempts write to
`lift/logs/<wave>/YYYY-MM-DD/`, YAML lives in `lift/config/`, durable queue control files live in
`lift/state/`, and generated code/reviews live in `lift/reports/<wave>/`.
`lift/scripts/organize_lift.ps1` safely sweeps older root-level stdout/stderr
transcripts on every queue startup; it excludes recent and active-run files and never overwrites a
collision. This keeps unattended operation bounded in directory density without adding another
scheduled task or risking a live attempt.

The final Wave 3 report/state move was completed without interrupting its active batch. The scheduled
runner retains a guarded `lift/scripts/migrate_wave3_layout.ps1` preflight so an older checkout can
self-migrate before launching, without risking duplicate workers during the transition.

Generated source is not counted as reconstructed C++. Promotion still requires the VC7.1 object,
a focused behavior test, a clean signature audit, and retail `.text` comparison. The scheduled
rebuild refresh ingests Wave 3 reports, compiles the curated catalog, exports retail bytes for every
compiled row, rebuilds the manifest/backlog, and updates `rebuild/COVERAGE.md` after the RE queue
releases Ghidra.

The scheduled refresh and RE triggers intentionally share a 15-minute cadence. The queue runner now
waits up to five minutes when it sees a live refresh PID, polling every 15 seconds and resuming as
soon as Ghidra is released. Previously their roughly 30-second trigger offset made every new RE run
exit immediately behind the refresh task. A live scheduled smoke run now proceeds into its target
instead of silently losing the entire cadence.

The queue's cross-wave exclusion set also imports the curated VC7.1 compile gate. A candidate with
both compile and focused-behavior PASS is terminal for agent scheduling even when the LLM checker was
overly conservative; byte-parity and human review remain separate promotion evidence. This prevents
the unattended loop from repeatedly spending provider time on functions already proven by a
stronger executable oracle.

The unattended refill order now includes `rebuild/backlog/forgefse-binding-queue.tsv` immediately
after the completed subsystem seeds and before generic naming/prototype work. The queue contains all
462 owner-aligned FSE recommendations and classifies the wrapper-to-retail bridge: 167 direct
signatures, 269 expected wrapper adapters, and 26 signature/arity reviews. It prioritizes direct,
unique, strong-owner, complete-prototype rows. The agent may produce a reconstruction and structural
PASS, but it cannot set `hookApproved`; compile/behavior proof, target-build identity, ABI/callsite
review, and a ForgeFSE runtime probe remain separate gates.

ForgeFSE entry validation is also automated against `ghidra_out/cgsi_slots_aligned.tsv`. The first
audit compared 443 recommended Quest APIs with 918 existing `pVTable[N]` assignments: 442 already
resolved to the exact recommended retail address, including the four overloaded
`CameraUseCameraPoint` entries. The sole mismatch identified a decomp naming gap rather than a bad
ForgeFSE slot: CGSI slot 15 is `CGameScriptInterface::GetRegionName @ 0x0088E340`, while exact-leaf
matching had selected `CTCQuestCard::GetRegionName @ 0x007025A0`. A decorated-symbol/vtable-backed
override corrects the manifest. The forced post-batch refresh applied it and the scheduled audit
passed 443/443 entries with zero mismatches or missing assignment families.

The landscape-paint lane now includes four relocation-masked byte-identical `CWorldMap` dispatch
methods, four functional `CMap::Set/GetEngineTheme/BlendAt` implementations, and three functional
`CScriptedMapBrush` cell accessors. The latter expose the authored brush layout directly: five
32-bit theme slots per cell and byte-addressed blend values at cell offset `0x10 + layer`, with a
`0x14`-byte cell stride. The curated suite is 94/94 for VC7.1 compile and focused behavior; the
map/brush bodies remain honestly non-matching until their compiler code shape is tuned against
retail.

For milestone accounting, 1% of the 49,553-function catalog is approximately 496 verified
functions. The compile+behavior lane is currently 94 functions (0.190%); reaching 1% therefore
requires 402 additional promotions. The stricter functional-or-matching count is 71 (0.143%).
Clean pseudo-C, naming, and prototype coverage are
tracked separately and must not be presented as completed buildable source.

### Downstream native-evidence consumers (2026-07-22 19:25 MDT)

The FSE-to-engine overlay is now an actively validated interface rather than a passive report. Its
931 rows are mirrored to FableForge, ForgeFSE, and FQT. FableForge parses the evidence for native
backing inspection (6/6 CTests pass); FQT annotates its API Reference while keeping verified engine
implementation separate from binding approval (190/190 tests pass); and ForgeFSE validates every
manifest key, count, candidate address, and safety-policy flag before runtime binding work. Current
evidence provides 462 owner-aligned recommendations and 54 verified engine functions, but
`verifiedRecommendedBindings` and `hookApprovedBindings` both remain zero. This intentionally
prevents exact-name correlation from being mistaken for a safe callable address.

The recent curated exact-match closure now includes eight locally proven code-shape matches. Five are raw
byte matches: `$E2 @ 0x00406770`, the
`initterm @ 0x00401356` import thunk, `__dllonexit @ 0x004013DE`, and the 17-byte
`__SEH_epilog @ 0x004013C7`, and the 59-byte `__SEH_prolog @ 0x0040138C`. Two are
relocation-normalized exact matches:
`CNavQuadTree::VerifyTransitionPoints @ 0x00A77500` (53 bytes) and
`CNavQuadTree::JoinNavigationLayers @ 0x00A77480` (117 bytes), plus
`GFGetBuildNumber2 @ 0x00401F30` (162 bytes). The thunk declarations use their real forwarded MSVCR71 parameter
shapes even though their naked bodies do not directly reference the arguments.
`Vector_FreeElements @ 0x0040E130` adds a 43-byte relocation-masked match: all instruction bytes
and offsets match retail, with only the expected object-file relocation payload for `free` masked.
The adjacent helper batch adds `__RTC_Initialize`, `Std_Move_Backward`, `MemCmp_Unsigned16`,
`WideString_EqualsLen`, and two circular-list allocators. The wide-string helper and both node
allocators are relocation-masked matches. The allocators' retail `ret 4` proved one ignored stack
slot beyond the ECX/EDX fastcall pair, correcting two incomplete Ghidra-derived prototypes.
`CCharString_ComputeCRC32 @ 0x00404310` is also a 46-byte relocation-masked match. Its retail
tail calls preserve `CCRC::Calc`'s EAX value, correcting the decompiler's provisional `void`
return to an unsigned 32-bit CRC.
The unrelated `$E2 @ 0x00404A90` static-data initializer is a 147-byte relocation-masked match;
its behavior oracle verifies all 19 global stores and their exact widths.
`CUserProfileManager_SaveAndCleanup @ 0x0040BB90` is behavior-correct for ordinary cursor advance
and allocator-block rollover/free. Its clean 144-byte object deliberately omits 40 bytes of
provably unreachable copy scaffolding retained in the 191-byte retail body, so it is functional
but nonmatching.
`CEngineLandscapePatch::RelocateData @ 0x00BF3980` now adds a 143-byte relocation-normalized exact
match. Its focused oracle covers direct patch-data relocation, asynchronous-data relocation and
priority notification, head/interior linked-list repair, missing nodes, and the null early-return
path. All 57 fixed instruction bytes and control-flow offsets match retail; only the three expected
direct-call COFF relocations are masked.
`Getter_FieldE0_FC @ 0x00662020` adds a raw 13-byte match for the opaque
`*(*(self + 0xE0) + 0xFC)` accessor. The curated override records the observed ECX fastcall input and
unsigned 32-bit return, closing one calling-convention/prototype gap without inventing a class name.
`CNavQuadTree::InitialiseLines @ 0x00A7A5E0` adds a 102-byte relocation-normalized match. Its oracle
proves grid-vector growth, shrink, and zero-size behavior, integer cell bounds, input forwarding,
and the final `UpdateLines` dispatch; donor decoration also fixes the nested STL/reference ABI.
The ForgeFSE-facing `CameraCancelScreenEffect @ 0x0088EDB0` and `CameraUseScreenEffect @ 0x0088EDA0`
stubs add raw one-byte (`ret`) and three-byte (`ret 0x0C`) matches. Their exact decorated symbols
prove public virtual const methods and the latter's three-float stack ABI.

The signature audit supports both qualified member definitions and `_global` definitions with C
linkage/calling-convention qualifiers. This removed seven false-negative parse reviews and raised
the clean audited candidate set to 102/148; 46 snapshots remain in signature review, including two
genuinely noncanonical constructor/destructor definitions that still cannot be parsed.

The toolchain now also queries the private donor PDB directly through Visual Studio's installed
`llvm-pdbutil` and DIA runtime. Each future auto-RE target receives an exact qualified donor symbol
record when available (return type, parameter types/names, donor address, and body size), alongside
the existing PDB-derived class-layout TSV. This lookup takes less than a second, is cached per agent
process, and does not acquire the Ghidra project lock. Manual targeted queries are exposed through
`tools/query_pdb_oracle.ps1`; the broader web-researched tool assessment and pilot order live in
`docs/DECOMP_ACCELERATORS.md`.

The first direct-PDB application corrected the complete active navigation cluster: `Initialise`,
`InitialiseLines`, `IsAreaBlockedByLines`, `UpdateLines`, `CNavQuadTreeNode::Initialise`,
`JoinNavigationLayers`, `VerifyTransitionPoints`, both `ConnectVerticalMapEdge` bodies, and
`LinkToNeighbouringMaps`. Their manifest parameters now preserve the original reference qualifiers
(`CNavigationAreaInit&`, `CNavigatorManager const&`, and the nested vector/list inputs) instead of
decompiler-style pointers.

The first direct-PDB-assisted batch validated the new lane immediately. `VerifyTransitionPoints`
passed its checker in one round, while `JoinNavigationLayers` and `LinkToNeighbouringMaps` passed
in two; the two `ConnectVerticalMapEdge` bodies subsequently passed in three rounds and one round.
The first two are now curated VC7.1 translation units with focused behavior tests; their
compiled instruction streams match retail exactly after normalizing the expected direct-call COFF
relocations. This is also useful navmesh evidence: the transition table at `CNavQuadTree+0x28`
stores 16-byte position/layer pairs, verification resolves both layers, and joining adds reciprocal
links only when both resolved nodes exist and have their `+0x0F` disable byte clear.

`LinkToNeighbouringMaps` is also curated and behavior-verified. Its oracle covers all four map-edge
coordinate pairs, the live `Initialising` flag during dispatch, manager-reference propagation, and
the connector-heap dirty/order fields at `+0x78..+0x85`. Its current 273-byte object is nine bytes
longer than the 264-byte retail body because VC7.1 chose a different x87/local schedule, so it is
classified functional/nonmatching. Decorated retail and donor symbols contain `ABV` and therefore
prove `CNavigatorManager const&`; this corrects the direct `llvm-pdbutil pretty` rendering, which
lost the const qualifier.

The first generated promotion-queue target, `GFGetBuildNumber2`, is now curated as well. Missing-file
and present-file tests prove the path temporary cleanup, disk construction modes `(1,2)`, whole-file
read length `-1`, global build-string assignment, vtable reset, close, member destruction, and final
base cleanup order. VC7.1 emits the same 162-byte instruction layout as retail after normalizing its
string/global/direct-call relocations.

The first deeper nav seed, `CWorldMap::GetMapNavigationAreaInit @ 0x0050A650`, has an accepted agent
reconstruction. It is not yet a curated compile candidate due to its large dependency surface, but
it records the native map-bounds, placed-thing, physics, perception, and navigation-layer assembly
path needed to progress from navmesh parsing toward editable/regenerable navigation data.

Prototype closure also has a deterministic lane: `tools/recover_msvc_prototypes.py` runs VC7.1's
own undecorator over raw Ghidra API rows whose calling convention is still unknown. It emits a
generated override table only for conservatively parsed member signatures; reviewed manual
overrides win on overlap. The first pass recovered 4,627 complete signatures and moved complete
prototype coverage from 59.645% to 68.983% without consuming RE-agent calls.

---

## ★★★ GAME-CHANGER (2026-07-17): a debug build with FULL PDB symbols was found
Archive: `D:\Downloads\Fable_Anniversary-2013-02-25.7z` (3.36 GB) — a pre-release **Fable Anniversary**
build containing **private PDB symbol files** and debug executables, all built with **VC7.1 / MSVC 2003
— the exact compiler as the shipping TLC `Fable.exe`** (confirmed by the bundled `msvcr71.dll`/`msvcp71.dll`).

| Binary | Size | PDB | PDB size | Note |
|---|---|---|---|---|
| `ego_r.exe` | 15.96 MB | `Ego_r.pdb` | 133 MB | Release **Ego engine** (Lionhead's engine — what EgoCore is named after). **Near-exact size match to TLC `Fable.exe` (16.6 MB)** → primary lineage match. |
| `FableWin.exe` | 55.6 MB | `FableWin.pdb` | 254 MB | Full/debug build — richest symbols. |
| `Ego_d.exe` | (stub) | `Ego_d.pdb` | 269 MB | Debug variant. |

Also: **`Data\Defs\DevHeaders\`** — the developers' own C headers (`particles.h`, `meshdata.h` 338 KB,
`front_end_bank.h`, `pc/`+`xbox/` variants) = direct struct definitions.

**What a private PDB gives us:** every function name, class name, struct/member layout, local-variable
name, and **source file + line number** for that build. This is the single most valuable artifact
possible for RE — it collapses "blind decompile tens of thousands of `sub_*`" into "apply symbols,
then read source-equivalent code." It also recovers the **original source-tree layout** (from the
file/line records), telling us how the engine's modules are organised and how to structure our output.

**★ The "Anniversary" label is misleading — this IS the TLC codebase.** The PDB path embedded in
every exe's debug directory reads
`E:\FableTLC\Fable TLC Build Repository\BBBProjects\Fable1_5MainPC\Fable\Build\ego_r.pdb` — i.e. it
was built from the **"Fable TLC Build Repository", project "Fable1_5MainPC"** (Fable 1.5 = the TLC-era
engine). Confirmed further at load time: it imports **`d3d9`/`dinput8`/`d3dx9`** (the DX9 TLC-era
renderer, **not** Anniversary's DX11), and Ghidra reconstructs real TLC engine classes from the PDB
(`CGuiWindow`, `CEnvironment`, `CClass`, `CNavQuadTree`, `CParentDefClassBase`, …). So this is not a
divergent remaster; it is the **TLC engine source tree, symbolized**. Symbols transfer near-directly
for engine/gameplay/script; only late Anniversary-specific deltas (if any) differ. We still use it
strictly as a **symbol/type oracle**, never as shipped content (consistent with the "TLC only" directive).

### Revised primary strategy: two-binary symbol port
1. **Symbolized source DB** — load `ego_r.exe` **with `Ego_r.pdb`** (and `FableWin.exe`+`FableWin.pdb`)
   into Ghidra; the PDB Universal analyzer applies all names/types → a near-source-level DB *for free*.
   Parse the `DevHeaders` `.h` into it for exact struct layouts.
2. **Target DB** — the shipping TLC `Fable.exe` (analysis already running) is the ground truth we
   ultimately reconstruct.
3. **Port** symbols/types from the symbolized DB onto the TLC target by **function matching** — Ghidra
   **BSim** (decompiler-signature similarity) and/or **Version Tracking** / FunctionID. Same compiler +
   shared codebase makes match rates high for engine/gameplay code.
4. **Reconstruct** buildable source against TLC, now with real names, real types, and the real source-tree
   layout as the skeleton — turning bar #2 from a long-shot into a structured port.

The sections below (scale, pipeline) still hold, but the PDB **dramatically** raises the starting
coverage and lowers the per-function cost. The pivot: the debug build is now the primary *source of
truth*; the shipping `Fable.exe` is the *reconstruction target*.

---

## What "fully decompiled" means here (pick the bar)
Two industry-standard bars; we pursue them in order, per module:

1. **Full pseudo-C coverage** — every function in `.text` decompiles to clean, named, typed
   pseudo-C in Ghidra (nothing left as raw bytes / undefined / failed-decompile). This is a
   *complete RE database*. Achievable, and the prerequisite for #2.
2. **Buildable source reconstruction** — recovered `.c/.cpp` that a compiler turns back into a
   functionally-equivalent `Fable.exe`. Two grades:
   - **Functional** (recommended target): recompiles to a binary that *behaves* identically
     (same logic; compiler may reorder/allocate differently). Practical and verifiable by running.
   - **Matching** (gold standard, optional per-module): recompiles to *byte-identical* code with the
     era compiler. Far more effort; reserve for hot/critical modules where it's worth proving exact.

We target **#1 across the whole binary**, then **#2-functional module by module**, opportunistically
going to **#2-matching** where cheap.

## The honest scale
- `.text` is **14.8 MB** of x86-32 → on the order of **tens of thousands of functions**. This is a
  large-project decomp (comparable in scope to community game-decomp efforts that run for years).
- Therefore: **automation-first**, coverage tracked as a metric, and the work is parallelizable
  across subsystems. A "big-bang" hand decomp is not realistic; an incremental, verified,
  module-by-module lift is.

## Why it's *tractable* here (the multipliers)
This binary is unusually friendly for a full decomp:
- **Clean PE32, no packer, no obfuscation** — Ghidra gets near-complete auto-analysis for free.
- **`_PDATA` section** gives accurate function boundaries (the usual "where do functions start/end"
  problem is largely pre-solved).
- **Intact MSVC RTTI** (expected on this PC build) → class names, vtable layouts, inheritance
  recovered wholesale; C++ structure comes back, not just flat functions.
- **The era compiler is known and available**: game ships `msvcr71.dll`/`msvcp71.dll` → **MSVC 2003
  (VC7.1)**. Having the *exact* toolset is what makes a matching/functional rebuild feasible at all.
- **Two ready-made answer keys** for large swaths of the code:
  - **EgoCore** source = every asset/resource/level/def/audio format parser in readable C++
    (the whole resource-loading half of the engine has a reference implementation).
  - **FSE** manifest/source = the gameplay/script API surface (931 functions, real signatures).
- **Sibling priors** from Fable2RE for shared-lineage subsystems (Will/spells, Babel text,
  appearance, Havok/anim, the string-hash resource index).

## The pipeline
1. **Import & maximize auto-analysis** (Phase 0). Clean PE32 import; enable RTTI, decompiler
   parameter ID, `_PDATA`-driven function creation. Goal: minimize `sub_*`/undefined count on day one.
2. **Mass-name & type** (Phases 1–3) via the labelling loop (`docs/METHODOLOGY.md`): RTTI classes →
   FSE targets → string xrefs → Lua natives → EgoCore-guided resource loaders. Every named/typed
   function raises pseudo-C quality. Track: `named / total`, `typed / total`.
3. **Coverage-close pseudo-C** — sweep for functions Ghidra failed to decompile or mis-split; fix
   boundaries (often `_PDATA`), calling conventions (VC7.1 uses `__thiscall`/`__fastcall` heavily),
   and struct types until **100% decompile cleanly**. Metric: `clean-decompile / total`.
4. **Build harness** — a VC7.1 project that can link recovered TUs against a "rest of binary" stub
   so each lifted module is compiled and **diffed** against the original (functional or byte-match).
   This is what makes it a *decompilation* and not just annotated disassembly.
5. **Lift module-by-module** — Ghidra pseudo-C → compileable C, prioritized by the Phase-2 subsystem
   order (script/quest VM → entities → resource/BIG/tng loaders → combat/Will/items → save → text/UI).
   - **Bulk** via `auto-re-agent` (`D:\Documents\Fable2RE\auto-re-agent`): its AI+Ghidra parity loop
     produces candidate C and checks it against the original — exactly this job.
   - **Cross-check** each lifted module against its answer key (EgoCore for formats, FSE for API),
     and against **runtime behaviour** (x32dbg breakpoints, FSE Lua probes, EgoCore headless fixtures).
   - **Hand-finish** the functions automation can't match (heavy inlining, floating-point, SIMD).
6. **Integrate & verify** — as modules land, assemble them into a buildable tree; the ultimate test
   is a rebuilt `Fable.exe` that boots and plays. Keep a running **coverage dashboard**
   (`ghidra_out/coverage.tsv`: function → {named, typed, clean-decomp, lifted, matched}).

## Tooling for the lift (beyond Phase 0–3 toolchain)
- **`auto-re-agent`** — primary bulk lifter (parity-checked). Set up its config for the VC7.1 target.
- **GhidraMCP** — batch decompile/export + programmatic rename/retype; the LLM's interface to the DB.
- **VC7.1 (MSVC 2003)** — the build/oracle compiler. Obtain the matching toolset (the game's CRT
  DLLs confirm the version).
- **EgoCore headless** — data-side oracle: extract known assets and diff a reversed loader's output.
- **Diff/verify** — a per-function objdiff-style comparison (recovered vs original code bytes) for
  matching grade; behavioural tests for functional grade.

## Realistic sequencing & expectations
- Full **pseudo-C coverage** (bar #1): the near-term, clearly-achievable milestone — weeks of the
  labelling loop + coverage sweeps.
- **Functional source** (bar #2) is the multi-month/long-horizon body of work, done incrementally so
  every step is verified and useful on its own (each lifted subsystem immediately deepens modding).
- We will **not** claim "done" until a rebuilt binary is verified; interim status is always reported
  as a coverage percentage with evidence, never as assumption (project evidence rule).

## Relationship to modding (no wasted work)
The full decomp and the modding surface are the *same work at different resolutions*: every function
lifted for the decomp is also a function moddable natively. So Phases 1–3 serve both goals at once —
there is no fork in the road, only increasing depth.

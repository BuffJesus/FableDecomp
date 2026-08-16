<!-- Generated 2026-08-16 by an ultracode multi-agent workflow (10 subsystem
mappers + lead-architect synthesis). Grounded in the reconstruction as of this date.
Resume/authoritative plan for the "controllable hero in Oakvale" north star. -->

# FableTLC — Roadmap to a Controllable Hero in the Oakvale Prologue

*Decision-grade. Byte-purity and retail-parity are the hard constraints. No softening.*

## 1. Reality check

The honest gap is enormous and it is not a menu-plumbing problem. Today the reconstruction can render an **authored** D3D9 frontend (`FableRunVisualBootCheckpoint`) that draws bitmaps and Render2D quads — but it never crosses into retail's `CGame::Play` game loop, never constructs a live `CWorld`, never streams a level's geometry into a GPU-visible scene, and has **zero reconstructed D3D9 device layer** (no shader creation, no `SetRenderState`, no `DrawPrimitive`, no `Present`). "Menus render" exercises maybe 22% of functions as *functional* C++ and 8.9% as *byte-identical*, but that 8.9% is overwhelmingly leaf functions, definition-class accessors, and frontend 2D. The subsystems that stand between here and a visible hero — **3D scene render, per-frame tick orchestration, world/subsystem bring-up, entity/hero spawn, animation runtime, level-streaming crash fix** — are each either undecomposed, DEFER-class (regalloc/temp-scheduling non-byte-matchable), or landed-but-crashing. **The single biggest cost driver is the 3D scene renderer + D3D9 device layer** (the map calls it XL/EXTREME, ~14–20 weeks in its own estimate): ~20 of 49,568 functions landed in that pipeline, and it is a hard dependency for the north star because nothing the hero does is *visible* without it. Everything else on the critical path is measured in weeks; the renderer is measured in months. **Controllable-hero is a multi-month, byte-pure effort. Plan accordingly and pick a vertical slice that produces motivating proof-of-life long before the full renderer is pure.**

## 2. Critical path (ordered dependency chain)

Each row is a *hard* prerequisite for a hero to stand and be controlled. State: **landed** / **decoded** (semantics recovered, parity DEFER) / **inferred** (named only) / **none**. Effort is byte-pure reconstruction effort from *here*.

| # | Subsystem gate | Key entry point(s) | State | Effort |
|---|---|---|---|---|
| 1 | Engine init (pre-loop) | `GFInitialise` 0x004022b0; `CProgressDisplay` family | **landed** (byte-exact) | done |
| 2 | Boot→loop seam | `GFMain` phases; `FableGFMainPhase10PlayBoundary` stub (must call Play) | **decoded** (authored stub, no real call) | S |
| 3 | Game-loop entry | `CGame::Play` 0x00412f90 (203B) | **landed** byte-exact | done |
| 4 | Frontend→gameplay handoff | `CMainGameComponent::InitWorld` 0x41735a; `CFrontEndManager::Action` 0x59a238 | **landed** (RELOC_MATCH); `LoadGame` 0x594f36 DIFFER | S (verify) |
| 5 | World construction | `CWorld` ctor 0x4a67d0 (727B); `CWorld::PostInit` 0x4a6550 (625B) | **decoded** (ctor DEFER; PostInit no source) | M |
| 6 | World subsystem ctors | 0x6e7740, 0xcb5c70, 0x4b4590, 0x6be300 | **none** | M |
| 7 | Per-frame tick orchestration | `CMainGameComponent::Update` 0x418289; `CWorld::Update` 0x49dfb0; `CDisplayEngine::WorldUpdate` 0x434f60 | **inferred** (named only) | L |
| 8 | Level streaming (Oakvale) | `LoadLevel` 0x502620; `OpenStaticMap` 0xbdd0e0 (**CRASH** 0x7dd1d3) | **landed but DIFFER + crash** | M |
| 9 | Entity/Thing + hero spawn | `OnCreateHero` 0x6a9ef0; `CThingManager::LoadGameState` 0x5253f0 (2272B); `ConstructFromParams` 0x6a9dd0 | **inferred / decoded-defer** | L |
| 10 | Hero grounding + movement | `GetGroundSizeZAt` 0x63a300 (landed); hero-Z spawn init; velocity→sweep | **landed (collision) / none (integration)** | M |
| 11 | Animation runtime | `GetInterpolatedMeshAnim` 0x70e650; bone-matrix accumulation (unknown addr); D3D9 skinning | **decoded (accessors) / inferred (traversal)** | M–L |
| 12 | **3D scene render + D3D9 device** | `DoRender` 0x435530/0x435f70; `RenderPrimitiveList` 0xb91760/0xb917e0; `RenderCellClipped` 0xb92500…0xb92c20; **D3D9 device layer (none)** | **inferred (protos exist) / none (device)** | **XL** |

**Reading:** gates 1–4 are essentially done. The wall is **7, 11, and 12** — and 12 dwarfs all others.

## 3. Thinnest viable vertical slice

**Target: "Static Oakvale-child region loads from real WAD; hero mesh spawns and renders standing on real navmesh ground; camera looks at it; one idle animation clip plays" — under the real engine's `CGame::Play` loop, driven by reconstructed code, with the authored D3D9 present as the *only* declared scaffold.**

This is deliberately **not**: no combat, no Will, no quests, no dialogue, no NPC AI, no save/load round-trip, no multi-region streaming, no controllable *input* yet (that is the very next slice), no pure D3D9 device layer (scaffold-allowed initially).

**Why this is the right first target:**
- It is the **shortest chain that touches every architectural seam once**: seam (gate 2), loop (3), world (5–6), streaming (8), entity/hero (9), grounding (10), a single anim (11), and *a* path to pixels (12). If any seam is fundamentally wrong, this slice exposes it cheaply.
- It converts the scariest unknown (the renderer) into a **bounded scaffold decision** rather than a blocking 14-week prerequisite: use the already-working D3D9 window + a *minimal authored mesh-draw scaffold* to get the hero on screen, then replace `RenderCellClipped`/`RenderPrimitiveList`/`DoRender` with pure reconstructions incrementally *behind* a working picture.
- It produces a genuinely motivating artifact: a Fable hero standing in Oakvale, rendered by *your* loop — the "proof of life" that justifies the months of renderer purity work that follow.
- It forces the **`0x7dd1d3` OpenStaticMap crash** to be fixed early (gate 8), which is on the critical path anyway and currently blocks all region testing.

**Slice-complete definition:** launch the reconstruction, it enters `CGame::Play` (not the visual checkpoint), constructs a real `CWorld`, streams the Oakvale-child filler region from `FinalAlbion.wad` without the 0x7dd1d3 AV, spawns the hero Thing at a navmesh-grounded Z, and the per-frame tick renders one posed (idle) hero frame repeatedly. **Controllable input is the immediately-following slice, not this one** — separating "visible" from "controllable" keeps the first win reachable.

## 4. Purity / parity strategy

The governing pattern is the **OpenRetailBank runtime-module anchor** (memory: *openretailbank-runtime-module*): before reconstructing a subsystem purely, run it *inside real `Fable.exe`* as a live oracle so you know the exact behavior, arguments, and neighbor-call contract you must reproduce. Apply it per gate:

- **Live-oracle first, reconstruct second.** For every gate below, attach the Ghidra debugger (`mcp__ghidra__debugger_*`) or x32dbg to retail `Fable.exe`, breakpoint the entry, and capture register/stack/arg state and the exact callee sequence. This is how you de-risk *before* authoring a single pure line. It is the difference between guessing the MI-ctor ordering in `CWorld` and *watching it*.
- **Byte-purity holds unconditionally** for: `CGame::Play` (already landed), the seam call into it (a real `call 0x412f90`, not a stub), `CWorld::PostInit` subsystem ctors (5 leaf-ish functions), `OnCreateHero`, ground collision (`0x63a300` landed), the animation bone-matrix loop, and every `RenderCellClipped`/`RenderPrimitiveList`/`DoRender` body. These are the load-bearing engine logic — the whole project's north star (*byte-purity-policy*: said 3×) requires them genuine.
- **DEFER-class is acceptable as decoded-defer, not as bake.** `CWorld` ctor, `CThingManager::LoadGameState`, `CPlayerManager::LoadGameState`, `CWorld::LoadGameState`, `LoadGame` 0x594f36 are documented regalloc/temp-scheduling coin-flips. Land them as **semantics-verified, behavior-verified, non-byte-exact** with the DEFER tag — do *not* invent asm to force a match, and do *not* bake. Revisit only if a QFE-4035/permuter pass closes them.
- **Temporary authored scaffold is acceptable ONLY at two boundaries, both explicitly flagged:** (1) the **D3D9 device layer** for the first vertical slice — the authored `visual_boot` present/upload path may draw the hero mesh while the pure `RenderCell` family is reconstructed behind it; (2) the **input→control** step of the *next* slice. Everything the scaffold touches must be marked `authored-scaffold` in the manifest with a tracked replace-target address, so scaffold never masquerades as reconstruction. The frontend `FableRunVisualBootCheckpoint` is already this kind of flagged scaffold and is the model.
- **Parity verification per landing:** keep the existing `verify_and_land.py` / `verify_land_jumptable.py` harness contract (RELOCATION_MATCH / MATCH). For behavior parity of *integration* seams that can't be byte-matched, use the **live retail run as differential oracle**: same input region, compare hero spawn coordinates, subsystem-ctor order, and frame output against retail captured under the debugger. NATIVE_TERRAIN_STATUS.md's WER-fault-offset + `offset+0x400000` triage stays the fast crash-diff loop.

## 4b. Biggest risks / unknowns (ranked)

1. **3D scene renderer + D3D9 device volume (XL, dominant).** ~20/49,568 functions landed; the `RenderCellClipped` family (0xb92500–0xb92c20), `RenderPrimitiveList` (0xb91760/0xb917e0) and `DoRender` (0x435530/0x435f70) have *prototypes accepted and decompile=yes* (good — the shapes are known) but no bodies, and the **device layer has zero bytes**. Risk is not feasibility, it is *sheer volume + D3D9 state-machine correctness* (render-state ordering, shader creation, present). Mitigation: scaffold present for the slice; reconstruct innermost (`RenderCellClipped` 0xb92500) first to expose device-call signatures, exactly as the map advises. **This is the schedule.**
2. **`OpenStaticMap` 0x7dd1d3 crash (M, blocks all region testing).** Retail `CEngineLandscapeMap::OpenStaticMap` `malloc(field_04)` with no null-check → AV. Root cause still open: garbage `field_04` (chunk defect) vs stream misposition. Mitigation: **debugger BP `0xBDD1B2`** during a *retail* Oakvale load to read `field_04` — this is a single focused session and it is on the critical path regardless. Until resolved, no level streams.
3. **Per-frame tick orchestration is inferred-only (L).** `CMainGameComponent::Update` 0x418289, `CWorld::Update` 0x49dfb0, `CDisplayEngine::WorldUpdate` 0x434f60 — subsystem pump *order* (physics/AI/anim/render/script) is unknown and getting it wrong means a frozen or garbage world. Mitigation: debugger trace the call graph out of `CMainGameComponent::Update` in retail before authoring.
4. **Animation runtime traversal unread (M–L).** Format is 100% done, but the bone-matrix accumulation order (pre/post-multiply, root-motion timing) and the D3D9 skinning application are inferred. Wrong order = T-pose/stick-figure. Mitigation: decompile `GetInterpolatedMeshAnim` 0x70e650 inward to the bone loop; verify against a retail single-frame pose capture.
5. **DEFER-class entity deserializers (M, purity-only risk).** `CThingManager::LoadGameState` 2272B and the `LoadGameState` family may never byte-match without a better compiler. Low *functional* risk (behavior recoverable), real *audit-completeness* risk. Accept as decoded-defer.
6. **MI-ctor / temp-scheduling on `CWorld` ctor (M, purity-only).** Same class; land decoded-defer.

## 5. Phase plan

### Phase A — Cross the seam into the real loop (goal: engine runs `CGame::Play`, not the checkpoint)
- Replace the `FableGFMainPhase10PlayBoundary` stub (`stage2_engine_boundary.cpp:1034-1038`) with a real `CGame::Play` 0x412f90 invocation (already landed byte-exact).
- Verify `CMainGameComponent::InitWorld` 0x41735a (RELOC_MATCH) actually fires and constructs a `CWorld`.
- **Exit:** under the debugger, execution reaches `CGame::Play`'s component-Run loop with a constructed `CWorld*`, no crash. (This is essentially FIRST INCREMENT — see §6.)

### Phase B — Kill the streaming crash + load the real region (goal: Oakvale-child filler region streams from WAD)
- Debugger BP `0xBDD1B2` on retail; resolve `field_04` semantics; fix `CEngineLandscapeMap::OpenStaticMap` 0xbdd0e0 (and `CLocalDetailCacheMap::OpenStaticMap` 0xbdf010).
- Confirm `LoadLevel` 0x502620 opens LEV+TNG from `FinalAlbion.wad` for the prologue region under the live loop.
- **Exit:** region streams to completion, no 0x7dd1d3 AV; `forge validate` still clean; region entities enumerable in memory.

### Phase C — World subsystem bring-up (goal: `CWorld::PostInit` runs with real subsystems)
- Reconstruct the 5 `PostInit` subsystem ctors byte-pure: `CGameScriptInterface` 0x6e7740, `CScriptInfoManager` 0xcb5c70 (+Init 0xcb5d80), `CQuestManager` 0x4b4590, engine/render 0x6be300, combat-anim-set 0x6b1960.
- Author `CWorld::PostInit` 0x4a6550 as decoded-defer (CCharString temp-scheduling DEFER accepted); `CWorld` ctor 0x4a67d0 decoded-defer.
- **Exit:** `PostInit` behavior-verified against retail (subsystem pointers at +0x24/+0x38/+0x58/+0x6c populated; render-layer passes set at 0x13b8394).

### Phase D — Tick orchestration (goal: the world updates every frame)
- Debugger-trace then reconstruct `CMainGameComponent::Update` 0x418289 → `CWorld::Update` 0x49dfb0 → `CDisplayEngine::WorldUpdate` 0x434f60. Document order in `docs/GAME_LOOP_TICK.md`.
- Wire `CThingManager::WorldUpdateThings` 0x51fcc0 and the landed `WorldFrameUpdate` leaves (0xb28c80, 0xb3b180).
- **Exit:** the loop pumps subsystems in retail order for N frames without divergence; world clock advances.

### Phase E — Spawn + ground the hero (goal: hero Thing exists, standing)
- Reconstruct `OnCreateHero` 0x6a9ef0 (S; ~100–200B) and `ConstructFromParams` 0x6a9dd0 linkage; land `CThingCreatureDef::Transfer` 0x671600.
- Integrate ground collision: hero-Z spawn via `GetGroundSizeZAt` 0x63a300 (landed) snapping to navmesh layer-0 ground.
- **Exit:** hero `PlayerCreature` present in the Thing manager at a grounded Z; verified against retail spawn coordinates for the prologue.

### Phase F — Make the hero visible (goal: VERTICAL SLICE — hero renders standing, one idle clip)
- Reconstruct innermost render first: `RenderCellClipped` 0xb92500 → `RenderPrimitiveList` 0xb91760 → `CDisplayEngine::DoRender` 0x435530, exposing D3D9 device signatures as you go.
- Animation: decompile `GetInterpolatedMeshAnim` 0x70e650 bone loop; apply one idle clip; skinning via the CTCGraphicAppearance/SubPrimitive path.
- **Scaffold allowed:** authored D3D9 present may draw the hero mesh until the `RenderCell` family + device layer are pure. Flag every scaffold node with its replace-target.
- **Exit:** the reconstruction renders a grounded, idle-animated hero in the Oakvale-child region under `CGame::Play`. **This is the motivating milestone.** Controllable input follows as Phase G (input dispatch → `CModeStandBase`/movement → velocity → collision sweep), and full renderer *purity* (device layer, all render-cell bodies) is the long tail after the picture works.

## 6. FIRST INCREMENT — start now, one focused session

**Wire the boot seam to the real game loop and prove entry under the debugger.** This is the smallest byte-pure, verifiable step that changes the fundamental fact "we never enter retail's loop," and it depends only on already-landed code.

**Exact work:**
1. **File:** `D:\Documents\FableTLC\rebuild\integration\stage2_engine_boundary.cpp`, `FableGFMainPhase10PlayBoundary` (lines ~1034–1038). It currently increments a call-counter stub and does **not** call Play.
2. **Reconstruct/wire:** replace the stub body with the genuine call path Phase10 must take — invoke `CGame::Play` @ `0x00412f90` (already landed byte-exact at `rebuild/src/compiled/00/41/CGame_Play_00412f90.cpp`) on the real game object, matching retail's `GFMain`→Play edge. Do **not** author a proxy: call the landed function. Confirm the `this`/argument the retail `GFMain` passes to `CGame::Play` by reading it live first (below).
3. **Guard:** keep the `FABLETLC_ENABLE_VISUAL_BOOT` branch (line ~1285–1315) as a *flagged fallback*, but add the real-Play path so a build without the visual-checkpoint macro executes `CGame::Play`.

**Verify (build + live oracle + behavior):**
- **Live oracle first:** attach the Ghidra debugger to retail `Fable.exe`, `debugger_set_breakpoint` at `0x00412f90`, start a New Game, and `debugger_read_args` / `debugger_registers` / `debugger_stack_trace` to capture the exact `ecx`/stack Play receives and the caller (confirm it is the `GFMain` phase-10 edge). This is the OpenRetailBank-anchor pattern — you reproduce a *measured* contract, not a guessed one.
- **Build:** `rebuild/build_bootstrap.ps1` (with the retail frontend bank as in the visual-parity recipe).
- **Behavior check:** run the reconstruction under the debugger with a BP at `0x412f90`; assert execution reaches Play's component-Run dispatch loop with the same `this` shape captured from retail, and that `CMainGameComponent::InitWorld` 0x41735a is on the resulting call graph. Success = "we are now inside the real loop." Expect it to then fault in streaming (the 0x7dd1d3 crash) — that fault is the *proof* you crossed the seam and is exactly Phase B's starting point.

**Why this first:** it is achievable in one session, uses only landed byte-exact code (`CGame::Play`), is verifiable by a single debugger capture, and it flips the project from "authored menu demo" to "executing the real engine loop" — the precondition for every subsequent phase. The crash it surfaces is the next task, not a regression.

---

*Relevant files:* `D:\Documents\FableTLC\rebuild\integration\stage2_engine_boundary.cpp` (seam stub), `D:\Documents\FableTLC\rebuild\src\compiled\00\41\CGame_Play_00412f90.cpp` (landed loop entry), `D:\Documents\FableTLC\rebuild\integration\boot_oracles.tsv` (oracle), `D:\Documents\FableTLC\docs\NEW_GAME_PATH.md` + `CONTINUE_GAME_PATH.md` (seam semantics), `D:\Documents\FableTLC\rebuild\manifest\functions.tsv` (render family protos: 0xb91760/0xb92500/0x435530 accepted, decompile=yes, no source). *Load-bearing fact:* `FableGFMainPhase10PlayBoundary` at `stage2_engine_boundary.cpp:1034-1038` is the blocking stub — it must call `0x00412f90`, and today it does not.
---

## Implementation reality-check (2026-08-16, from reading the seam + landed Play)

Two corrections to §6 "FIRST INCREMENT" after inspecting the actual files (the
synthesis over-optimistically framed it as a one-session "just call Play"):

1. **`CGame::Play` @ 0x412f90 is a `__declspec(naked)` asm bake, not genuine C++.**
   `rebuild/src/compiled/00/41/CGame_Play_00412f90.cpp` is hand-written `__asm` with
   `_emit` bytes (e.g. the `8D 9B 00 00 00 00` lea at run_component). It is byte-exact
   but it is a **purity debt** — it belongs to the de-bake lane ([[debake-lane]],
   [[byte-purity-policy]]), not "landed byte-pure". Crossing the seam with it works for
   *execution* but does not advance byte-purity; the genuine-C++ reconstruction of Play
   is still owed.

2. **Play's adapter ring is NOT wired to the real engine — only to fixture stubs.**
   Play calls 8 extern adapters (`FableGameCompileFrontendDefinitions_00412f90`,
   `FableGameOperatorNew_00412f90`, `FableGameMainComponentCtor_00412f90`,
   `FableGameNewFrontendCtor_00412f90`, `FableGameLegacyFrontendCtor_00412f90`, the
   WideString/CharString ctor/dtor/assign adapters) and reads globals
   `g_FableCompileFrontendDefinitions_013B8648`, `g_FableStartMainGame_013B8605`,
   `g_FableUseLegacyFrontend_013B8642`, `g_FableRetiredGameComponent_013B7D58`,
   `g_FableMainGameStartupPath_013B7D5C`. A repo-wide search finds these defined **only**
   in `CGame_Play_00412f90.cpp` (extern decls) and `CGame_Play_00412f90_test.cpp`
   (stub bodies). There is **no real-engine wiring** and **no constructed `CGame this`**.

**Revised Phase-A first increment (evidence-first):** the true first step is the
**debugger live-oracle capture**, not a code edit. Attach the Ghidra debugger to retail
`Fable.exe`, BP `0x00412f90`, start a New Game, and capture: (a) the `CGame*` (ecx) that
retail's `GFMain` phase-10 passes to Play; (b) the concrete targets of each adapter call
(so the ring can be pointed at real engine functions, OpenRetailBank-anchor style); (c)
the values of the five gating globals during a real boot. Only then wire the ring + a real
`CGame this` and flip the `FableGFMainPhase10PlayBoundary` stub (behind a new
`FABLETLC_EXECUTE_EXACT_PLAY` macro, mirroring the existing
`FABLETLC_EXECUTE_EXACT_GFUNINITIALISE` guard at stage2_engine_boundary.cpp:1045 so the
default visual-checkpoint build is unaffected). Budget Phase A as **debugger-capture +
adapter-ring wiring**, not a single call edit.

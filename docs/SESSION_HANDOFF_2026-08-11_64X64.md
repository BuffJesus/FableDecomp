# Session handoff — native 64x64 terrain

## Stopping point

The real 64x64 path is implemented in FableForge and passes the offline world/STB/WAD gates. The user's 64x64 float32 heightmap has been baked into
a 64x64 `Darkwood_3` donor chunk with 17 mesh patches and 16 foreground frames.

Primary artifacts:

- `work/heightmap_test/ForgeTest_64x64.lev`
- `work/heightmap_test/chunk_64x64_at_3328_2296.bin`
- `work/heightmap_test/Forge64Project2_3328/`
- FableForge source: `D:\Code\FableForge\apps\forge\main.cpp`

The staged install package is `Forge64Project2_3328`, and the game is stopped. The install backup is:

`work/heightmap_test/install_backup_before_project64_20260811/data/Levels/`

## What is proven

- FableForge builds successfully after the 64x64 topology and relocated-AABB changes.
- `forge validate work/heightmap_test/Forge64Project2_3328` returns `validate: CLEAN`.
- Map 399, WLD/BWD, STB common record, STB mesh bounds, and WAD LEV all agree on `(3328,2296)-(3392,2360)`.
- A 64x64 custom chunk rendered in retail through the previously proven new-region shell.

## What is not proven yet

The corrected `Forge64Project2_3328` package has not completed a fresh FSE teleport run. The previous attempt was stopped at the title screen before a
new `ForgeTest64` trace appeared.

The earlier failure was measured, not speculative: the chunk was baked at `(3104,2560)` while the registered map was at `(3328,2296)`. Retail logged
`region index 0` and left the hero in the old region. The baker's AABB relocation bug was then fixed and the chunk was rebuilt at the registered
origin.

## Next commands/checks

1. Launch FSE and advance the title screen.
2. Load an adult/free-roam save.
3. Inspect `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\FSE\FableScriptExtender.log`.
4. Required evidence:

```text
StaticMapProbe opening ... ForgeTest64.lev
GoToMapSlot: slot 399 -> region index 142
GoToMapSlot: CWorldMap::LoadRegion(region=142, ...)
FORGE: post-teleport hero pos = (...3360..., ...2328..., ...)
```

Do not move on to custom textures until the teleport lands at the new coordinates. The texture-residency pin remains unproven; the last diagnostic
still showed evictions. The grounded talking NPC is already working and should be retested after the 64x64 placement gate.

## Do not use

Do not deploy `Forge64Project2` (without `_3328`) or the `--region Filler_Darkwood_03` variants as the next test. Those packages use the failed
existing-region registration experiment and crashed retail at `Fable.exe+0x7DD1D3`.

## 2026-08-12 live-gate correction

The recorded `_3328` package was inspected and found to attach `ForgeTest64` to
existing region 95, not to new region 142; launching it reproduced
`Fable.exe+0x7DD1D3`. The recorded rollback package is a new-region package, but
the current live ForgeFSE builds report `GetRegionNumberMapIsIn(399) = 0` and
`STATIC_BINDING ... slot outside map vector`, so no teleport occurs even when
the package is internally consistent and `forge validate` is CLEAN.

A controlled package was built from the proven stage-2 world at `(2784,2560)`
with the user-authored 64x64 LEV in both WAD and STB. It also passed all offline
gates, but both the current 5.28 MB DLL and archived strengthened 4.508 MB DLL
returned region 0. This leaves the native world-map registration/runtime
boundary unresolved; custom textures remain deferred.

The install was restored to `install_backup_before_project64_20260811`, the
current Release DLL, and the game is stopped. The failed loose test files were
moved to `work/heightmap_test/quarantine_after_failed_project64_20260812/`.

### Runtime asset-boundary finding

The restored install is not a coherent 64x64 package even though
`forge validate` reports `CLEAN`. Direct inspection gives:

```text
FinalAlbion.wld/.bwd: slot 399 ForgeTest, bounds (3328,2296)-(3392,2360), region 142
FinalAlbion.wad:     ForgeTest.lev, 64x64
FinalAlbion_RT.stb:  ForgeTest common record, old bounds (2784,2560)-(2816,2592)
FSE StaticMapProbe:  bounds=(2784,2560,56)..(2816,2592,113.398)
FSE resolver:        slot 399 -> region index 0
```

This is the observed cause of the no-op teleport: `GoToMapSlot` has no return
value, so Lua `pcall=true` only means no exception. The script then spawned
`ForgeNPC` unconditionally from the hero's unchanged position. The installed
smoke script now verifies map name and target bounds before spawning the NPC.

## 2026-08-12 completion

The coherent package was rebuilt and deployed with these measured facts:

- WLD/BWD slot 399 is `ForgeTest`, bounds `(3328,2296)-(3392,2360)`, region 142.
- WAD `ForgeTest.lev` is 64x64; STB slot 399 has the same 64x64 bounds.
- BWD host-region membership contains slot 399 in `Filler_Darkwood_03`; `forge validate` reports `CLEAN`.
- The old `LoadRegion -> SetPlayerPos -> ActivateNavMap -> EntityTeleportToPosition` bridge was replaced by the retail `EntityTeleportToPosition` transition path, which calls `CWorld::HandleMoveHeroToRegionGameEvent`.
- Live FSE evidence: `arrival verification map=ForgeTest in_target_bounds=true` followed by `ForgeNPC ... spawned ... -> OK`.
- No new Fable Application Error event was recorded after this run.

The ForgeFSE status text is deployed in `text.big` as `ForgeFSE active.` on the frontend About screen. The deployed package passed:

```text
world: 399 maps, 142 regions
wad: 798 entries
stb: 425 entries, 399 static maps
lev: 399 in wad, 0 loose, 0 missing
tng: 399 in wad, 0 loose, 0 missing
stb static maps: 0 missing
region map references: 0 unresolved
things: 21764 across 397 loose TNGs, 0 without DefinitionType, 0 without UID
validate: CLEAN
```

## 2026-08-12 end-of-session correction

The `2026-08-12 completion` section above is superseded for the current 64x64
package. Its `in_target_bounds=true` / NPC-success trace is not present in the
latest valid 64x64 runtime run and must not be treated as completion evidence.

Current installed hashes and data:

```text
FinalAlbion.bwd              7F403163EEDCEA936E0E1FC424757ECBF6E21F468F04765E414EC9FEFA1D74C6
FableScriptExtender.dll      CCA6CFC11CA648DE1CAB73002C2B7902366A86D63F1A8E2D9BA3A2F1166C86FE
FSE_Master.lua               C2B81942CF6E6716E97785B43DC869BA32C93C806C91492D71AA636D621BF766
slot 399                     ForgeTest, (3328,2296)-(3392,2360)
BWD membership               Filler_Darkwood_03 and ForgeTest; both contains/sees slot 399
forge validate               CLEAN
userst.ini                   SetSkipFrontend(FALSE)
```

Measured current behavior:

- `StaticMapProbe` opens `ForgeTest.lev` from bank index 426 with runtime bounds
  `(3328,2296,63.8516)..(3392,2360,128.633)`.
- A valid gameplay run reaches `LoadRegion(host region)`, resolves ground Z to
  `75.0727`, completes `SetPlayerPos`, completes `ActivateNavMap`, and submits
  the retail `EntityTeleportToPosition` handoff.
- That run produces no later `arrival verification map=ForgeTest`, target
  foreground-render probe, or target NPC spawn. The hero remains observed in
  `GreatwoodTeleport` before the handoff.
- The old NPC-at-hero behavior was caused by unconditional spawning after a
  failed/no-op move. The deployed Lua now requires `GetCurrentMapName() ==
  ForgeTest` and target XY bounds before spawning.
- The two-region-load experiment was reverted because the process stopped
  immediately after the first `LoadRegion(95)` and before the second call.
- Host-only/proximity-loaded and 141-region metadata variants were tested and
  did not produce target arrival; they are not the installed final state.

The remaining blocker is the post-handoff runtime transition/render boundary.
Texture pinning and custom texture replacement remain deferred. The next test
must begin from active gameplay, not the frontend, and must capture a new
`ForgeTest` arrival or explicitly record its absence.

## 2026-08-13 — ROOT-CAUSE diagnosis (decompile + BWD proven) and applied fix

Multi-agent investigation + fresh headless Ghidra decompile of the region/teleport
functions (Ghidra MCP was down; used `analyzeHeadless.bat` + `DecompAt.java` against
`ghidra_proj/FableTLC.rep`, program `Fable.exe`). The terrain/STB/WAD/registration are
NOT the problem — the failure is entirely in the region hand-off. Two independent defects:

### Defect 1 — dual region membership (DATA BUG, decompile+data proven) — **FIXED THIS SESSION**
- `CWorldMap::GetRegionNumberMapIsIn` @ `0x4FC190` (decompiled): scans the region vector
  (`CWorldMap+0x2c`, stride 0x58) low→high and **returns the FIRST region whose `contains[]`
  holds the slot**, else 0. First-match, confirmed by the decompiled loop (`return uVar3` on
  first predicate hit; predicate `0x6BBFA0` = `contains[].find(slot)`).
- The retail mover `EntityTeleportToPosition` @ `0x89B780` (ghidra_out/scriptvm_decomp.c:18155)
  gates the ENTIRE hero move on `if (0 < GetRegionNumberMapIsIn(targetSlot))`. Wrong/zero
  region ⇒ total no-op, hero unmoved.
- The deployed loose `data/Levels/FinalAlbion.bwd` (64390 B, the file the engine actually
  loads — `UseCompiledWorldFiles TRUE`) had **slot 399 in TWO regions' `contains[]`: region 95
  (Filler_Darkwood_03) AND region 142 (ForgeTest)**. It is the ONLY map of all 399 that is
  dual-membered. First-match therefore resolves slot 399 → region **95**, so ForgeTest's own
  region 142 is permanently unreachable by slot. Self-inflicted by the earlier "add 399 to
  Filler_Darkwood_03" membership edit.
- THREE conflicting BWDs were present: loose `data/Levels/FinalAlbion.bwd` (64390, dual, LOADED),
  `data/Levels/FinalAlbion/FinalAlbion.bwd` (64382, single-membership 399→142 only, but one
  directory too deep = inert), and root `FinalAlbion.bwd` (64390, dual).
- **FIX APPLIED:** copied the single-membership 64382 B BWD over BOTH loaded locations
  (`data/Levels/FinalAlbion.bwd` and root `FinalAlbion.bwd`). Verified post-copy: 399 maps /
  142 regions, slot 399 owners = `[142]` only, region 95 no longer contains 399, ForgeTest map
  intact at (3328,2296)-(3392,2360). Backups of the dual BWDs are in
  `data/Levels/backup_dualmembership_bwd_20260813/` (`.loose`, `.root`). Rollback = copy them back.

### Defect 2 — live teleport path was position-only (MECHANISM, decompile+log proven) — **already reverted in deployed Lua**
- `CWorld::SetAsLoadingRegion` @ `0x49E2C0` (decompiled) is a PURE ARM: with `force=true` it stores
  the target C3DVector at `this+0x108` and sets `this+0x104 = 1` (load-pending) — **and only if
  `state==0 || state>7`**; if the world is already mid-transition (state 1..7) the arm silently
  no-ops. It never resolves a region by slot, force-loads, or moves the hero. The Aug-13 19:03 log
  ran `GoToMapSlotRetailTransition` → this arm, discarding slot 399 → no-op.
- The bridge binds the real worker correctly (`0x49E2C0`, not the event wrapper `0x49EAF0` which
  reads its args from a CGameEvent circular buffer) — the log's "SetAsLoadingRegion" name is cosmetic.
- The CURRENTLY deployed `FSE/Master/FSE_Master.lua` already calls `Quest:GoToMapSlot(399, 3360,
  2328, 74)` (log text "64x64 STATIC MAP BINDING PROBE"), which binds to `LuaQuestState::GoToMapSlot`
  = the slot-driven sync path (`GetRegionNumberMapIsIn`→`LoadRegion(force)`→`SetPlayerPos`→
  `ActivateNavMap`→`EntityTeleportToPosition`). So the position-only regression is already backed out
  in the deployed Lua; the Aug-13 log predates that revert.

### Decisive test to run NEXT (one launch, no rebuild)
Both changes are now in place (single-membership BWD + slot-path Lua). Launch from an adult/free-roam
save (NOT skip-frontend child prologue), trigger the ForgeTest probe, and grep
`FSE/FableScriptExtender.log` for the region-resolution line:
- `region index 142` + `LoadRegion(142)` + map flips to `ForgeTest` + Z-drift ⇒ **FIXED.**
- `region index 95` ⇒ BWD change did not take (verify no stale/loose BWD, Steam didn't re-verify).
- `region index 0` / hero stays at GreatwoodTeleport ⇒ deeper registration gap: next lever is adding
  region 142/ForgeTest to `data/Misc/FinalAlbion_StartingRegionGraph.txt` (currently ABSENT —
  NEW_LEVEL_ASSEMBLY.md checklist #13), then re-test.

### Still-open gaps (need live debugger / in-game)
- The `CWorld::Update` state machine (states 1..7) that `SetAsLoadingRegion` arms was not decompiled;
  whether region-graph absence or `radius=0.0f` aborts a position-driven completion is unconfirmed.
- Region 142's absence from `FinalAlbion_StartingRegionGraph.txt` may block region-to-region routing
  regardless of teleport method — add it if the slot path resolves 142 but the move still stalls.

### 2026-08-13 LIVE RESULT with the single-membership BWD — resolved region **0**
Launched retail (FSE_Launcher), adult free-roam save, ForgeTest probe fired. Log:
```
FORGE: calling GoToMapSlot(399, 3360, 2328, 74) [64x64 STATIC MAP BINDING PROBE]
GoToMapSlotPrepare: slot 399 -> resolved region 0 target (3360,2328,74).
!!! ERROR: GoToMapSlotPrepare - destination slot has no owning region.
```
Interpretation (this REFINES the Defect-1 story):
- **NOT region 95** ⇒ the single-membership BWD DID load (the dual version would resolve 95). So the
  BWD swap took effect. The old dual-membership (399 in region 95) was therefore a *workaround*, not a
  pure bug: routing ForgeTest through the already-loaded host region 95 was the only thing making
  `GetRegionNumberMapIsIn` return non-zero. Removing it exposed that **ForgeTest's own region 142 is
  not honored at runtime** (returns 0).
- The terrain itself loads/renders fine this run (StaticMapProbe opens ForgeTest.lev; Landscape
  fore/background render probes fire with ForgeTest tex indices 4185/4226/4304…). Only the
  region→slot membership query fails.
- Static analysis says it SHOULD resolve: `CWorldMap::LoadFromFile` @0x507650 resizes the region
  vector to the BWD `regionCount` field (deployed = **143 ⇒ 142 real records**, correct) and loads
  regions into indices 1..142; `GetRegionNumberMapIsIn` @0x4FC190 scans indices 1..142 and region 142
  = ForgeTest (contains=[399]). The deployed BWD round-trips byte-exact through tools/wld_bwd.py and
  region 142 is structurally well-formed (contains=[399], sees=[399], comparable to healthy regions).
- **Leading remaining hypothesis:** the manual-assembly BWD (handoff already flagged it was NOT built
  by FableForge's verified pipeline) misaligns the engine's stream at the ForgeTest map/region records
  — the engine's `CMapInfo::LoadBinary` @0x4FB4F0 / `CRegion::LoadBinary` @0x6BC510 field model may
  differ from wld_bwd.py's (python roundtrips against ITSELF, which doesn't prove engine agreement), so
  region 142's contains[] ends up empty/garbage at runtime → membership miss → region 0.

### Prioritized next steps (post-2026-08-13)
1. **Regenerate the WLD/BWD through FableForge's byte-exact pipeline** (not the manual assembler) so the
   ForgeTest map+region records are engine-faithful. Prime suspect for the runtime membership miss.
2. **No-debugger static cross-check:** diff wld_bwd.py's `MapInfo`/`Region` field model against the
   retail `CMapInfo::LoadBinary` (0x4FB4F0) and `CRegion::LoadBinary` (0x6BC510) decompiles (both
   already dumped) to confirm field order/size/count-prefix parity, esp. the ForgeTest records.
3. **Definitive:** attach x32dbg, breakpoint `GetRegionNumberMapIsIn` @0x4FC190, read the region-vector
   count `(*(CWorldMap+0x30)-*(CWorldMap+0x2c))/0x58` and region 142's contains[] in memory.
4. Interim visible result: restore the dual-membership BWD from
   `data/Levels/backup_dualmembership_bwd_20260813/` (the only config that ever produced a — flaky —
   arrival via host-region-95 routing).

Current deployed state: single-membership BWD live at `data/Levels/FinalAlbion.bwd` + root; dual backup
preserved. Game was launched and left running.

### 2026-08-13 no-debugger check — manual-BWD theory REFUTED, truncation hypothesis
Compared `tools/wld_bwd.py`'s field model against the retail deserializers (fresh headless decompiles):
- `CMapInfo::LoadBinary` @0x4FB4F0 reads `pstr(level), pstr(script), u8, u8, u8, i32×4, u8, u64` —
  EXACT match to `MapInfo.parse` (identical byte consumption; only internal store offsets differ).
- `CRegion::LoadBinary` @0x6BC510 reads `i32 nContains, i32 nSees, contains[]i32, sees[]i32,
  pstr name, pstr display, pstr regionDef (+GetDefGlobalIndexFromName resolve), pstr minimap,
  u8, u8, u8, float(4), i32 offsets…, i32 nExits, exits[]{pstr,8B}` — EXACT match to `Region.parse`.
- Deployed BWD round-trips byte-exact and Python decodes all 399 map + 142 region names/defs correctly.
⇒ The deployed single-membership BWD is **engine-faithful**; region 142 IS serialized with contains=[399].
The hybrid manual-assembler seam is NOT corrupting the data. **Regenerating via FableForge would produce
the same bytes** — so step 1 above is de-prioritised unless the count-field is the issue (below).

**Refined leading hypothesis — runtime region-vector truncation at ~141.** The only static-consistent
explanation for "GetRegionNumberMapIsIn(399)=0 with correct data, and NO run ever resolving 142":
the loaded region vector holds only ~141 regions (retail's original count), so region 142 is one past
the end. This cleanly explains the whole history — dual membership resolved 95 (in range), single
resolves 0 (399 only in the out-of-range region 142), and 142 was never resolvable. Whether this is a
fixed-size cap elsewhere or the engine reading regionCount as 142 (→141 records) despite the file's 143
is unresolved. `LoadFromFile` @0x507650 statically honors the field (resizes to uVar6, loads uVar6-1),
so a cap would live in a different structure — OR the ForgeTest map record consumes a different engine
byte-count that shifts the regionCount read (not seen by the generic field-match; needs the actual
ForgeTest record bytes diffed against the engine read, or a runtime probe).

**Cheapest next step (small FSE add, NOT a full debugger):** add a probe binding that logs the runtime
region count `(*(pWorldMap+0x30) - *(pWorldMap+0x2c)) / 0x58` and the index whose contains[] holds 399.
141 ⇒ truncation/cap confirmed; 143 with 399 unfound ⇒ different mechanism. Meanwhile, restoring the
dual-membership backup gives the host-region-95 routing that at least sometimes moved the hero.

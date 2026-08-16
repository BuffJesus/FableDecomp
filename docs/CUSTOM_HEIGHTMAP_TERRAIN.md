# Custom Heightmap Terrain in Retail Fable — WORKING RECIPE (headless, no editor)

**Date proven live:** 2026-08-11. First time a **user-authored heightmap** rendered as custom,
textured, walkable terrain in retail `Fable.exe`, and was reachable + controllable in-game — with
**no `ego_r` editor bake** (fully headless via FableForge `forge`).

This supersedes the "editor bake is the only route" verdict in `NATIVE_TERRAIN_STATUS.md` /
`FORGETEST_STATE.md`: the current FableForge `forge stb bake-heightfield` (Aug-10 build, at
`D:\Code\FableForge\build\forge.exe`) retargets the **actual foreground mesh vertices** (not just the
composed patch), which is exactly what the old bake missed — so a native bake now renders.

---

## The one-shot pipeline (raw heightmap → in-game)

Inputs: a `WxH` heightmap and the ForgeTest scaffold (region already registered, slot 399, host
region 95). All artifacts are **byte-in-place** (STB chunk stays 132,632 B) except the WAD (grows if
nav rebuilt). `forge` = `D:\Code\FableForge\build\forge.exe`.

```
# 1. Heightmap (32x32 float32, normalized 0..1) -> custom LEV (tools/lev_rw.py)
python: parse work/forgetest_editor_bake/ForgeTest.lev  (32x32 map, 33x33 cells, sub-ver 9)
        for each cell (x,y): lev.set_height(x, y, BASE + hm[y,x]*AMP)   # e.g. BASE=40, AMP=2.5
        lev_rw.write_file(lev, ForgeTest_custom.lev)

# 2. Bake the STB terrain chunk from the LEV heights (retargets 15-B CLandscapeLayerMesh verts + normals + AABBs)
forge stb bake-heightfield <donor_chunk_132632> ForgeTest_custom.lev 2784 2560 ForgeTest_custom_chunk.bin
   donor_chunk = work/forgetest_heightfield/ForgeTest_chunk_heightfield_foreground.bin (any valid 132632-B ForgeTest chunk)

# 3. Splice the chunk into the STB (byte-in-place: entry 426 "Data\Levels\FinalAlbion\ForgeTest.lev")
forge stb replace <base_5b7e6c1f.stb> FinalAlbion_RT_custom.stb "Data\Levels\FinalAlbion\ForgeTest.lev" ForgeTest_custom_chunk.bin
   base = work/terrain_runtime_probe/stage/data/Levels/FinalAlbion_RT.stb (the matched authored-hill STB)

# 4. Put the SAME LEV in the WAD (render+collision MUST match or it whites out — see gotcha)
forge wad repack <base_authored_hill.wad> <replace_dir/Data/Levels/FinalAlbion/ForgeTest.lev> FinalAlbion_custom.wad
   base = work/terrain_runtime_probe/stage/data/Levels/FinalAlbion.wad

# 5. Deploy STB + WAD (Fable MUST be closed — it locks the files). Keep the .qst enable (below).
copy FinalAlbion_RT_custom.stb -> <install>/data/Levels/FinalAlbion_RT.stb
copy FinalAlbion_custom.wad     -> <install>/data/Levels/FinalAlbion.wad
```

Working example script chain: `work/heightmap_test/` (this session's `penis_heightmap_32x32.raw` run).

---

## Making it WALKABLE (else the hero is fenced into the donor's walkable shape)

A donor-clone LEV only has the donor's walkable cells (ForgeTest authored-hill = **175 / 1089**), so
the hero can't cross the non-walkable cells (they show as dark stripes and act as invisible walls).
Fix on the LEV, then re-put it in the WAD (STB unchanged — walkability/nav live in the LEV, not the STB):

```
forge lev paint-walkable ForgeTest_custom.lev ForgeTest_walk.lev  16 16 30 1     # cx cy radius value(=walkable)
forge lev rebuild-nav    ForgeTest_walk.lev   ForgeTest_walknav.lev --tng <ForgeTest.tng>
forge wad repack <base.wad> <replace_dir with ForgeTest_walknav.lev> FinalAlbion_walknav.wad
```
`paint-walkable` has a slope/threshold guard (~330 cells stayed unwalkable at 757/1089 here); for
FULL walkability, widen the pass / drop the threshold. `rebuild-nav` needs the TNG's `NAVIGATION_SEED`
to seed the walkable island (2 TNG anchors consumed). Rebuilt nav LEV is larger, so the WAD grows.

---

## Load-bearing gotchas (each cost hours this session)

1. **`.qst` ENABLE is mandatory and Steam-wiped.** The FSE teleport quest (`FSE_Master`) only runs
   `Init()/Main()` if `data/Levels/FinalAlbion.qst` contains `AddQuest("FSE_Master", TRUE);`. FSE
   registering it (ESAS_UNSTARTED) is NOT enough — the engine only calls a quest's allocator for
   *active* quests. Steam "verify integrity of game files" restores `.qst` to stock and deletes this
   line. Re-add after any verify. Do NOT force `ActivateQuest` from the DLL's `InjectCustomScripts`
   hook — it crashes at VA `0x4B0110` (quest manager not ready). See memory `fse-quest-qst-enable`,
   AeoN/SampleQuests README step 3. (Confirmed by AeoN: "add the quest name to FinalAlbion.qst — that's
   always the first step for all scripts".)

2. **Matched STB + WAD or it whites out.** The STB terrain chunk (render) and the WAD `ForgeTest.lev`
   (collision/topology) must be the SAME terrain. Deploying an authored STB over a flat-filler WAD
   renders WHITE. Always deploy the pair. The texture only appears when they match.

3. **The bake must retarget mesh VERTICES, not just the composed patch.** The old bake (pre Aug-10)
   edited only the 16-B composed patch, leaving `RenderForeground @0x00BF4570` submitting donor XY/Z
   → black/white silhouette. The current `bake-heightfield` retargets the 15-B `CLandscapeLayerMesh`
   vertices + height-gradient normals + quadtree AABBs → renders correctly. Confirm the build:
   `forge.exe` mtime must be ≥ `apps/forge/main.cpp` (the vertex-retarget code, ~main.cpp:5017).

4. **Frozen hero = skip-frontend prologue, NOT the terrain/nav.** `GoToMapSlot` restores player
   control only from a **free-roam save** (control returns ~+4s, per run11). A skip-frontend NEW game
   drops you in the `StartOakVale` child prologue where Gameflow owns the hero and control never
   returns after teleport — from ANY destination (two-hop through Greatwood did NOT help; both the
   `ce869710` 4.5MB and the known-good 5.2MB DLL froze identically). There is no FSE binding to force
   control. **Load a real adult/free-roam save**, then teleport — control works, terrain is walkable.

5. **Amplitude/scale.** Heightmaps are usually normalized 0..1; map to world Z with `BASE + val*AMP`.
   `AMP=20` (range 38..58) = near-vertical walls at sharp 0→1 cell transitions; `AMP=2.5` (40..42.5) =
   a gentle bump. Box-smooth the heightmap first to round off single-cell steps.

---

## Current deployed state (this session)
- The install is staged with the corrected 64x64 project package from
  `work/heightmap_test/Forge64Project2_3328/data/Levels/`:
  `FinalAlbion.bwd`, `.wld`, `.wad`, and `_RT.stb`, plus loose `ForgeTest64.lev/.tng`.
- The package is **not yet live-proven through the FSE teleport**. The game was launched to the title screen and then stopped before a fresh
  `ForgeTest64` teleport trace appeared. Do not call this a completed live placement yet.
- Install rollback for this package is
  `work/heightmap_test/install_backup_before_project64_20260811/data/Levels/`.
  The earlier pre-64 rollback is `work/heightmap_test/install_backup_before_64x64_20260811/`.
- The install `.qst` still needs `AddQuest("FSE_Master", TRUE);` and the FSE scripts/DLL remain the working custom setup. Steam validation can wipe
  the `.qst` enable line and the custom data files.

## 2026-08-11 native 64x64 checkpoint

**The tool extension is implemented and offline-proven.** `forge stb bake-heightfield` now accepts both 32x32 and 64x64 donor topology. The change is
in `D:\Code\FableForge\apps\forge\main.cpp`: 64x64 selects 16 foreground frames and 17 mesh patches; 32x32 retains the original 4/4 path.
FableForge was rebuilt successfully with:

```text
cmake --build D:\Code\FableForge\build --config Release --target forge --parallel 4
```

The user-provided `D:\Downloads\heightmap\penis_heightmap_64x64.raw` was baked against the retail `Darkwood_3` 64x64 donor. The final relocated
chunk is `work/heightmap_test/chunk_64x64_at_3328_2296.bin`, 811,136 bytes, with 17 mesh patches and 16 foreground frames. Its geometry bounds are
`(3328,2296)..(3392,2360)` and its authored height range is 72.0..74.5.

The baker also received a relocation fix in the quadtree AABB pass. AABB coordinates are donor-world coordinates, so the pass now normalizes them
against the donor chunk's minimum XY rather than subtracting the destination origin. This is what permits a 64x64 chunk to be baked at a new map
origin instead of only at the donor's original `(3104,2560)` origin.

The generated package is `work/heightmap_test/Forge64Project2_3328/`. Offline validation is clean:

```text
world: 399 maps, 141 regions
wad: 798 entries
stb: 425 entries, 399 static maps
lev: 399 in wad, 0 loose, 0 missing
tng: 399 in wad, 0 loose, 0 missing
stb static maps: 0 missing
region map references: 0 unresolved
validate: CLEAN
```

The WLD/BWD registration agrees with the chunk: map slot 399 is `ForgeTest64`, origin `(3328,2296)`, bounds `(3328,2296)-(3392,2360)`, and the owning
region is 142. The STB record is 1,582 bytes and contains the matching 64x64 dimensions/bounds.

**Important failed experiment:** the first 64x64 bake was made at `(3104,2560)` while the map was registered at `(3328,2296)`. Offline validation did
not catch that geometry-origin mismatch; retail resolved the map to region index 0 and the hero stayed in the old region. The corrected bake fixed that
specific mismatch. A separate `create-from-donor --region Filler_Darkwood_03` package crashed retail at `Fable.exe+0x7DD1D3` even with the donor payload,
empty TNG, proximity disabled, and both FSE and direct launches; keep that package as a failed existing-region-registration case, not as the next test.

**Known-good shell:** the earlier `work/heightmap_test/Forge64_region_shell/` package proved that the 64x64 custom payload itself can render in retail
when placed in the already-proven ForgeTest/new-region shell. The next live test should therefore use the corrected `Forge64Project2_3328` package and
inspect these exact markers:

```text
StaticMapProbe opening 'Data\\Levels\\FinalAlbion\\ForgeTest64.lev'
GoToMapSlot: slot 399 -> region index 142
GoToMapSlot: CWorldMap::LoadRegion(region=142, ...)
FORGE: post-teleport hero pos = (...3360..., ...2328..., ...)
```

If the resolver still returns region index 0, the next investigation is the compiled BWD/WLD map/region lookup, not the terrain bake. If it resolves 142
but the game crashes, capture the WER fault offset before changing the package.

### 2026-08-12 artifact correction

The live package was checked against the files actually loaded by ForgeFSE. Its
offline `forge validate` result is `CLEAN`, but the runtime package is not
coherent across containers:

```text
WLD/BWD slot 399: ForgeTest, (3328,2296)-(3392,2360), region 142
WAD ForgeTest.lev: 64x64
STB ForgeTest common record: (2784,2560)-(2816,2592), 32x32 record
runtime StaticMapProbe: bounds=(2784,2560,56)..(2816,2592,113.398)
runtime GetRegionNumberMapIsIn(399): 0
```

The failed teleport therefore has a direct asset mismatch: the world-map
registration is at `(3328,2296)` while the STB common map record still
describes the old `(2784,2560)` map. `forge validate` currently checks each
container and cross-file names/counts, but does not compare WLD/BWD map bounds
with STB common-record bounds. The next package must use one origin, dimensions,
level name, and map record across WLD, BWD, WAD, and STB before another live
teleport test.

The terrain geometry bake and STB/WAD payload operations used FableForge. The
WLD/BWD registration and the currently deployed mixed package were produced by
the older/manual assembly path, so the complete package was not produced by a
single FableForge-verified pipeline.

### 2026-08-12 final live result

That mixed-package finding was corrected. The deployed package was rebuilt from the coherent WLD/BWD/WAD/STB/TNG stage and passed `forge validate` with zero unresolved region references and zero missing static maps. The BWD region membership was also corrected so slot 399 belongs to `Filler_Darkwood_03` at runtime.

The teleport bridge was corrected from a hand-built sequence to the retail `EntityTeleportToPosition` transition path. The live log then recorded:

```text
GoToMapSlot: retail EntityTeleportToPosition handoff submitted.
FORGE: arrival verification map=ForgeTest in_target_bounds=true
FORGE: spawned ForgeNPC ... -> OK
```

This is the measured resolution of the prior unchanged-hero/NPC-at-hero symptom. The NPC now spawns only after the hero is verified inside the target map bounds.

## Next-session order

1. Launch the current staged package and advance the title screen.
2. Load the adult/free-roam save; do not use a skip-frontend child run for the control test.
3. Read the FSE log for the exact markers above and capture a screenshot only after the post-teleport position is correct.
4. Once 64x64 placement is live, return to the texture-residency fix. The current pin DLL was not proven to stop eviction; the last diagnostic showed
   terrain textures still becoming evictable. Custom PNG injection should wait until that is fixed.
5. Then add the user's 128x128 terrain textures, followed by baked foliage and the already-working grounded interactable NPC.

## 2026-08-11 session addendum — vanish, creature, texture model

**Custom creature NPC (milestone 1 DONE):** `CreateCreature(def, pos, scriptName)` + `AddEntityBinding`
+ an entity lua (`FSE/Master/Entities/ForgeNPC.lua`, talk loop with `IsTalkedToByHero`/`Speak`/
`GiveHeroYesNoQuestion`/`GiveHeroObject`). Registered as an entity of `FSE_Master` in quests.lua (id 60).
Spawns grounded via `GetGroundHeightAt(x,y)` (hero.z floats — the terrain bump differs from the hero's
cell). Ghost villager on ForgeTest, talks, gives a chocolate box. Custom creature DEFS = the
custom-npc-pipeline (needs pristine game.bin); spawning EXISTING defs is trivial.

**Camera-angle vanish — ROOT CAUSE (probe-proven, not fully fixed):** the terrain foreground textures
are **GLOBAL and EVICTABLE** (resolve through the 6292-entry global texture table, NOT inline-resident).
ForgeTest references **18 texture indices** (4173,4174,4175,4177,4184-4186,4192,4216,4218,4226,4231,
4232,4248,4304,4305,4307,4325). Its host region 95 is NOT preloaded (`IsRegionLoadedAndPreloaded
("Filler_Darkwood_03")==false`), so the streamer evicts those textures under view pressure (looking
across pulls far patches into view → demand spike). `LoadTextures @0x00BF4130` then returns
`result=0 missing=N`, `RenderForeground @0x00BF4570` clears draw-flag 0x04 and aborts → **vanish**.
Looking straight down keeps only the near patch in view → its few textures stay resident → visible.

**Vanish fixes tried:**
- `CameraSetCameraPreloadFlag(true)` (lua, bound vtable[403]) → ~76% resident (262 vs 83 evicted);
  partial, one-shot preload of the current view; looking-down fine, across sporadic. NOT sufficient.
- Theme consolidation (`lev paint-theme` to one theme) → **NO-OP**: `bake-heightfield` PRESERVES the
  donor chunk's textures and ignores the LEV theme (byte-identical STB). **Key: to change terrain
  textures (yours or fewer), edit the CHUNK's texture references, not the LEV theme.**
- `PreloadDiffuseTexture @0xBEC000` → **WRONG function**: PDB = `CEnginePrimitiveManagerMeshBase::
  PreloadDiffuseTexture(C3DMeshMaterial*, morphInfo*, ulong, bool&, GraphicDataBank*)` — a 3D-MESH
  preload, not a landscape-texture pin. Do not call it for terrain.
- **The real fix = landscape-texture residency pin** (keep the 18 global indices resident): needs the
  actual texture-manager resident/lock API (RE in progress) or a `LoadTextures` force-load hook. Must be
  texture-agnostic so it also holds future custom textures. This is ALSO the foundation for custom
  textures (global-evictable → they'd vanish identically unless pinned).

**Custom terrain textures — model + pipeline (scoped):** textures are 128×128 (user provided 10 PNGs),
DXT1, referenced by GLOBAL index. Injection = (a) DXT1-encode (`tools/texture_build.py`), (b) add to the
global texture bank OR the chunk's inline `CEngineTexturePalette` (inline-vs-global residency TBD by the
RE agent), (c) rewrite the chunk's foreground-layer texture triples (layer header +5, 3× u32) +
background-patch materials to the new index, (d) pin via the residency fix. Foreground layer texture-idx
layout confirmed in `forge` `stb foregroundinfo` parser (main.cpp ~4870, parseForegroundDiskFrame +5).

**64×64 heightmap:** user provided `penis_heightmap_64x64.raw` (double res, above the 32×32 tool limit).
Downsampled 2×2→32×32 + baked, staged at `work/heightmap_test/STB_64ds.stb` + `WAD_64ds.wad` (NOT
deployed). True 64×64 needs a 64×64 donor chunk + a `bake-heightfield` topology-assert bump.

## Open / next
- **Custom terrain textures** — inline DXT1 in the chunk (`CEngineTexturePalette`); needs a chunk
  texture-replace tool (see feasibility note in HANDOFF) or `lev paint-theme` to reassign existing
  global ground themes.
- **Full walkability** — drop the `paint-walkable` threshold to cover all 1089 cells.
- **Custom creature/NPC on the terrain** — FSE `Quest:CreateCreature(defName, pos, scriptName)` spawns
  + `AddEntityBinding` makes it interactable (see the community MAssassinQuest pattern); custom
  creature *def/mesh* via the custom-npc-pipeline (memory `custom-npc-pipeline`).

## 2026-08-12 end-of-session runtime correction

The earlier “final live result” paragraph is historical and is superseded for
the current 64x64 `ForgeTest` package. The latest valid gameplay trace reaches
the native handoff but does not record target-map arrival or target terrain
rendering. No current evidence supports claiming that the 64x64 teleport is
fixed.

The offline package remains FableForge-clean: 399 maps, 142 regions, 399 WAD
LEVs, 399 TNGs, 399 STB static maps, zero missing LEVs/TNGs, zero unresolved
region references, and `validate: CLEAN`. Runtime `StaticMapProbe` confirms the
target LEV and bounds `(3328,2296)-(3392,2360)`. The unresolved boundary is
after `LoadRegion -> SetPlayerPos -> ActivateNavMap -> EntityTeleportToPosition`.

The NPC safety fix is real and deployed: `ForgeNPC` is not created unless the
hero reports map `ForgeTest` and XY coordinates within the target bounds. This
prevents a failed teleport from placing the NPC beside the unchanged hero.

ForgeFSE branding is deployed in `text.big`: the About label is
`About (ForgeFSE active)` and the About/legal message contains `ForgeFSE active.`

## 2026-08-16 — CORRECTIONS (verified) supersede the earlier vanish/texture-residency theory

The "camera-angle vanish = global-evictable textures" note above is about RENDER, not the crash.
Verified this session against decomp + disassembly:

- **The teleport blocker is SOLVED at the resolution level.** Attaching slot 399 to an in-range
  region (`forge world attach-map <root> ForgeTest --region Darkwood3`, or any ≤141 region) makes
  `GetRegionNumberMapIsIn(399)` resolve non-zero and the full teleport chain completes. The old
  "region resolves to 0" was NOT a 141-region cap — the FSE probe measured runtime
  `region_vector_size=142` (stock=141 regions/398 maps; ForgeTest=142/399, all load). Do not chase
  the truncation theory.
- **The remaining wall is a CRASH, not a vanish:** `0x7dd1d3` = `CEngineLandscapeMap::OpenStaticMap`
  @0x00BDD0E0. Disassembled: `rep stosd` zero-filling `malloc(field_04)` with NO null check
  (field_04 = landscape block header alloc size). Garbage field_04 → NULL malloc → AV. It is a
  LANDSCAPE-HEADER fault, not textures. Next: debugger BP 0xBDD1B2, read field_04.
- **Foreground terrain textures are always GLOBAL** (GBANK_MAIN_PC ids in textures.big), never inline;
  `LoadForeground` @0x00bfe050 bounds-checks them gracefully (missing → null frame → vanish, no crash).
  So "inline the textures" is NOT a thing; custom textures = repurpose an UNASSIGNED_* GBANK_MAIN_PC
  slot. `lev paint-theme` remains a NO-OP (bake ignores LEV theme) — edit the CHUNK triples via
  `forge stb settex`.

### Custom terrain textures — WORKING pipeline (offline-proven; render gated on the crash)
1. `python tools/texture_build.py replace <textures.big> <out.big> <UNASSIGNED_slot> <custom.png> --format dxt1`
   (GBANK terrain slots are 512×512; 156 UNASSIGNED_* slots exist — GRASS/PATH/SAND/CLIFF/MULCH/WOOD/...).
2. `forge stb settex <chunk.bin> <out.bin> --map <oldId>:<slotId>`  (retexture the terrain layers).
3. Deploy the new textures.big + chunk (stb replace / world install-level).
Verified: T_TLC_Grass1 → UNASSIGNED_GRASS_PLAIN (id 4216) roundtrips 58.71 dB; settex 4185→4216 valid.
Full design: D:\Code\FableForge\docs\TERRAIN_TEXTURE_PAINT_PLAN.md.

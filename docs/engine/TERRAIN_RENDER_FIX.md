# ForgeTest Terrain Render Fix — Honest Status & Action Plan

**Date:** 2026-07-20
**Goal:** Make custom region `ForgeTest` (map slot 399, region 95, donor `Darkwood9_Leadout_01`) render its terrain instead of whiting out.
**Bottom line up front:** Nothing is *proven* to render yet. The **editor bake (user-run)** is the only route that can render the *authored hill*, and it is fully staged as a turnkey bundle. The FSE name-alias is built and worth running, but it is a **diagnostic probe** (best case renders the *donor's* terrain, not the hill), not a durable fix. No headless bake was possible.

---

## 1. Proven root cause — what the decompile actually shows

The white-out is created at **engine map-open** (`CEngineMap::OpenRetailStaticMap @0x00B41E50`), not in the baked mesh bytes. The ForgeTest STB chunk is structurally clean and re-verified this run (`forge stbvalidate`: 26/26 LZO frames decode byte-exact, quadtree 4/4 wired, S1 reserialize == raw). Rewriting chunk bytes does nothing.

Two candidate mechanisms were carried in as suspects. The fresh proven disasm (`work/terrain_path/mapopen_proven_decomp.log`, `retail_loadbake_decomp.log`) settles them as follows:

| Mechanism | What the disasm proves | Status |
|---|---|---|
| **(A) Bank-by-name** — `CreateNewBankMap @0x009d58d0` (called from `0x00b41ec3` inside OpenRetailStaticMap) | In the proven hot path, `CreateNewBankMap` receives an **offset/index** derived from the InfoBlock/manifest entry — **there is NO `FindIndexBySymbol(name)` call**. The map name at `this+0x1c` steers resolution only *indirectly*, via the manifest lookup `FUN_0042b467(this+0x1c)` that runs first. | **PARTLY CONTRADICTED.** The "by-name miss mutates shared bank state" story is *asserted from reasoning, not staged as a decomp log* (per `NATIVE_TERRAIN_STATUS.md §1`). Address disagreement noted: the doc cites `0x009e0360`, the proven call target is `0x009d58d0` — unreconciled. |
| **(B) Theme-by-index** — `BuildThemes @0x00b687d0` | Resizes/populates a **global** vector by `ENGINE_THEME` index count through the global `CDefinitionManager` (`DAT_01436e28`/`DAT_01436e98`). **Takes no map name.** | **NAME-AGNOSTIC.** If themes are the cause, no name-based fix can touch it. Roughly a live ~50/50 co-suspect. |

**Honest verdict on root cause:** The white-out is confirmed to originate at map-open and to involve **shared global state** (the tell: teleporting to ForgeTest also whites out Oakvale). But **which** of (A) or (B) — or both — is the cause is **NOT decompiled to proof**. The by-name-bank story is the *leading hypothesis* but the proven disasm shows the bank bind is by index, weakening it. The theme-by-index path is name-agnostic and remains un-modeled. This uncertainty is the reason neither fix below can be promised to render.

---

## 2. FSE name-alias (native route 2)

**Built?** Yes. Complete and compiles clean.
- Trampoline detour on `CEngineMap::OpenRetailStaticMap @0x00B41E50` (5-byte JMP + relocated position-independent prologue `83 EC 64 / 53 / 55`, same VirtualProtect technique as the existing `InstallHook`).
- When armed from Lua, rewrites the `CEngineMap` name at `this+0x1c` from source (`"ForgeTest.lev"`) to donor (`"Darkwood9_Leadout_01"`) for exactly one map-open, then restores it (refcount-safe in-place swap via the game's own `CCharString` ctor/assign/dtor).
- Lua bindings: `Quest:AliasMapResources(src, donor)` and `Quest:ClearMapResourceAlias()`.
- **DLL:** `D:/Code/ForgeFSE/Release/FableScriptExtender.dll` — **4,471,296 B** (~4.4 MB, +5,632 B over the 4,465,664 baseline; expected codegen size, **not** a bad link). Bindings + `[MapResourceAlias]` log tags confirmed embedded; donor name intentionally NOT hardcoded.

**Will it render?** **Likely-NO as a real fix; maybe (~even odds) as a donor-terrain diagnostic.** The plumbing is sound — the hook fires at the right seam and the `this+0x1c` rename lands *before* the name lookup `FUN_0042b467` runs. But it depends on **two unverified load-bearing assumptions**, either of which sinks it:
1. **The cause must be by-name bank resolution**, not the name-agnostic `BuildThemes` theme-by-index path. Unproven — and the proven disasm shows the bank bind is by index, not by a name miss. No `BuildThemes` clamp was added, so if themes contribute, the alias does nothing there.
2. **`Darkwood9_Leadout_01` must be a resident manifest key** in the region-95 blob loaded at teleport. Plausible (it's a real 132,632-B `.lev`, same size as ForgeTest's cloned chunk) but not proven resident at open time. If it's not a key, `FUN_0042b467` misses and the alias is a no-op or faults.

**Honest scope:** On success this opens ForgeTest **fully AS the donor** and renders the **donor's terrain at ForgeTest's slot** (and should stop the Oakvale white-out) — a useful bisection proving the bank/theme layer is the cause. It does **NOT** render the authored ForgeTest hill. **Not smoke-tested in-game** (per do-not-deploy).

**Exact user steps:**
1. Copy `D:/Code/ForgeFSE/Release/FableScriptExtender.dll` to your test game install's FSE DLL location. (Do this yourself — the deployed 5.2 MB game DLL was NOT touched.)
2. In `FSE_Master.lua`, immediately around the teleport:
   ```lua
   Quest:AliasMapResources("ForgeTest.lev", "Darkwood9_Leadout_01")
   Quest:GoToMapSlot(399, 2800, 2576, 0)
   Quest:NewScriptFrame()          -- let the map open under the alias
   Quest:ClearMapResourceAlias()
   ```
3. Watch `FSE/FableScriptExtender.log` for `[MapResourceAlias]` lines (hook installed; name matched; donor open complete; restored). The log prints the **actual** current name it saw — if no "name matched" appears, retry the source arg as `"ForgeTest"` (no extension) to match exactly what the engine stores at `+0x1c`.
4. If ForgeTest's slot shows the donor terrain instead of whiting out → the bank/theme layer is confirmed as the cause → proceed to the editor bake for the durable fix. If themes glitch but terrain appears, the follow-up is a `BuildThemes @0x00B687D0` index clamp.

**Confidence:** Mechanically well-built and correctly targeted; scientifically a **diagnostic probe**, not a proven fix. Run it to bisect bank-vs-theme, not to ship.

---

## 3. Editor bake (route 1 — user's preferred, durable)

**Turnkey bundle ready?** Yes — fully staged at `D:/Documents/FableTLC/work/forgetest_editor_bake/` (8 files: `ForgeTest.lev`, `RUN_SHEET.md`, `assemble_gameroot.ps1`, `banks.ini`, `edit_settings.ini`, `userst.bake.ini`, `userst.editmode.ini`, `README.md`). Verified live this run:
- Retail install present; both debug exes present (`ego_r.exe` 15.9 MB + `FableWin.exe` 55.5 MB, 2012 build); `deploy.ps1` present.
- `assemble_gameroot.ps1` is coherent: **omits the STB** (satisfies the mandatory crash-on-existing-STB gotcha → all 399 maps become non-existent → full-world rebake), copies `FinalAlbion.wad` + `FinalAlbion/` folder, injects `ForgeTest.lev` at `data\Levels\FinalAlbion\ForgeTest.lev` (retail has no loose copy — geometry lives in the WAD, so the inject is genuinely necessary), and renames `userst.bake.ini`→`userst.ini` (the corrected file — the old `dbugst.ini`→`userst.ini` fix is real).
- ini recipe verified: `AllowDataGeneration` / `BuildRetailStaticMaps` / `OnlyUpdateNonExistantStaticMaps` / `UseRetailBanks` / `UseCompiledDefs` all TRUE.
- Bundle `ForgeTest.lev` validates: `forge lev themegrid` → 33×33, `GROUND_SPOOKYTREES` resolves @strength 255, height 43 baseline; md5 matches origin.

**Did a headless bake work?** **NO — and it never can.** `ego_r.exe` is a GUI + D3D9 app (PE subsystem 2, `d3d9`/`d3dx9_43` imports, profile gate); it exits code 1 in ~0 s non-interactively. **A headless agent cannot drive it.** No baked chunk was produced this run. The bake is a **user step by nature** — it must run interactively on a GPU desktop.

**Exact run-sheet for the user** (read `work/forgetest_editor_bake/RUN_SHEET.md` for the full version):
1. `cd D:/Documents/FableTLC/work/forgetest_editor_bake`
2. Assemble the editor gameroot:
   ```
   powershell -ExecutionPolicy Bypass -File assemble_gameroot.ps1 `
     -RetailDir "C:/Programs/Steam/steamapps/common/Fable The Lost Chapters" `
     -OutRoot "D:/ForgeBake/gameroot" -CopyExes
   ```
   (~2.8 GB free needed for a scratch bake; junctions mitigate, but `data/Levels/FinalAlbion`, the WAD, and the ~570 MB retail STB backup are real copies.)
3. Right-click `D:/ForgeBake/gameroot/ego_r.exe` → Properties → Compatibility → **Windows XP (SP3)**.
4. Run `ego_r.exe` **from inside** `D:/ForgeBake/gameroot` (CWD must be the gameroot). Pick/create a profile.
5. (Recommended) Edit World Map → ForgeTest to confirm the hill + `GROUND_SPOOKY` theme; **SaveAll**. If heights don't show, **paint the hill in-editor** (tutorial-proven fallback — the `.lev` is the source either way, but auto-pickup of injected heights is unconfirmed).
6. Let it bake `FinalAlbion_RT.stb` into `data/Levels` (full-world rebake, **20–40+ min**; the editor is crash-prone — SaveAll often and babysit).
7. Copy the baked STB → `D:/Documents/FableTLC/work/smoke_test/stage/data/Levels/FinalAlbion_RT.stb`.
8. Close Fable, run `deploy.ps1`, launch `FSE_Launcher.exe`, teleport to ForgeTest → the hill should render.

**Why this is the durable route:** The rebake registers ForgeTest's **own** bank entry + theme indices under its **own** name in a fresh `FinalAlbion_RT.stb` — which is exactly the white-out fix surface for **both** (A) and (B) at once. It's the only route that can render the *authored hill*.

**Confidence:** Mechanism is sound; **unproven end-to-end** until the user runs it. `renders_terrain = unknown`. Two residual unknowns: (a) whether the editor auto-ingests the injected `.lev` heights or needs a manual repaint (fallback documented), and (b) whether bake-under-own-name fully cures the shared-state white-out (the core unproven hypothesis of the whole task — the bake is the *test* of it).

---

## 4. Native "own-it" registration (forge)

**By-name STB registration is ALREADY DONE and on disk.** Verified live via `forge stb list` on the staged STB: 425 entries, 399 static maps, **entry 426 = `Data\Levels\FinalAlbion\ForgeTest.lev` @ offset 597919744 (132632 B)**. The common-header `pos_lookup_map` key is the **full-path** form, identical to all 398 siblings (last open blocker — the key format — is now closed). This fixes the **mandatory-STB-entry crash**.

**What forge does NOT yet own:** a native `forge::stb` **write path** to *reproduce* that registration itself, rather than via the one-off Python assembler (`assemble_forgetest_stage2.py`). This is **feasible now, no new RE** — a mechanical ~300–500 LOC byte-exact writer over formats forge already parses. Deliberately **not half-built** this run; scoped as a follow-up in `docs/modding/README.md#native_stb_writer_scope`.

**Correction to the original goal:** The **"theme-by-index registration" half has NO target surface.** `CStaticMapInfoBlock` has no theme field; `BuildThemes` reads a global shared `ENGINE_THEME` table; the per-map LEV theme path is by-name and degrades gracefully. There is **nothing for forge to register** there.

**Honest bottom line:** Native name-registration = essentially shipped (crash fixed). Native theme-registration = no such surface exists. **Guaranteed render is NOT achievable by registration alone** — per `NATIVE_TERRAIN_STATUS.md` the terrain may still white out via the shared-bank-state mechanism that is explicitly *asserted-not-proven*. That residual needs the editor bake or a map-open shared-bank decomp spike to cure.

---

## 5. THE RECOMMENDED PATH

### Recommendation: **Run the editor bake (§3). It is the only route that can render the authored hill, it is fully staged, and it cures both suspected mechanisms at once.**

**Why this one:**
- It's the user's stated preference **and** the only durable route.
- It's the only route that produces the **authored ForgeTest hill** (the FSE alias at best shows the *donor's* terrain).
- The bundle is turnkey and every physical prereq, path, and ini value was verified on-disk this run — no step crashes or no-ops.
- It fixes the fix-surface directly: registers ForgeTest's own bank/theme under its own name, covering **both** (A) and (B) regardless of which is the true cause.
- The only cost is user time (~20–40 min interactive bake) — unavoidable, since `ego_r.exe` is GUI+D3D9 and cannot be driven headless.

**Optional first (10 min, high diagnostic value):** Run the **FSE name-alias (§2)** before the bake as a **bisection probe**. If ForgeTest opens as the donor terrain and Oakvale stops whiting out, you've *proven* the bank/theme layer is the cause and that the bake will cure it. If it still whites out, suspect the name-agnostic `BuildThemes` theme path and expect to need a theme clamp even after baking. This de-risks the 40-min bake for a 10-min test.

### Exact next action
1. **(Optional, 10 min)** Deploy the FSE DLL and run the alias Lua (§2 steps 1–4) to bisect bank-vs-theme.
2. **(Main, ~30 min)** Run the editor-bake run-sheet (§3 steps 1–8): assemble gameroot → XP-SP3 compat → run `ego_r.exe` from the gameroot → confirm/paint the hill → bake → copy STB → deploy → teleport.

### The brutally honest summary
**Nothing is proven to render.** The FSE alias is a diagnostic that at best shows the *donor's* terrain. Native registration fixes only the crash, not the white-out. **The editor bake, run by the user interactively, is the only real route to a rendering ForgeTest hill** — and even it is unproven end-to-end until run, because the shared-state white-out mechanism it's meant to cure is asserted-not-proven. The bundle to run it is ready; the render outcome is genuinely unknown until you run it.

---

## Key artifacts & files

| Artifact | Path |
|---|---|
| FSE alias DLL (built, not deployed) | `D:/Code/ForgeFSE/Release/FableScriptExtender.dll` (4,471,296 B) |
| FSE files changed | `FableAPI.h/.cpp`, `dllmain.cpp`, `LuaQuestState.h/.cpp`, `LuaManager.cpp` under `D:/Code/ForgeFSE/FableScriptExtender/` |
| Editor-bake turnkey bundle (8 files) | `D:/Documents/FableTLC/work/forgetest_editor_bake/` |
| Editor-bake run-sheet | `D:/Documents/FableTLC/work/forgetest_editor_bake/RUN_SHEET.md` |
| Bake doc (corrections applied) | `D:/Documents/FableTLC/docs/modding/README.md#bake_forgetest_terrain` |
| Native STB writer scope (follow-up) | `D:/Documents/FableTLC/docs/modding/README.md#native_stb_writer_scope` |
| Root-cause status (asserts residual) | `D:/Documents/FableTLC/docs/modding/README.md#native_terrain_status` |
| Proven map-open disasm | `D:/Documents/FableTLC/work/terrain_path/mapopen_proven_decomp.log` |
| Staged STB (entry 426 present) | `D:/Documents/FableTLC/work/smoke_test/stage/data/Levels/FinalAlbion_RT.stb` |

## Verified facts (from FINDINGS log)

- **2026-08-22 — Inline background patch textures store ONE mip, not a chain (2026-08-22).**
  `CLandscapeBackgroundPatch::Load @0x00BE7D70` allocates each patch texture with level count 1
  (`push 1` at 0x00BE81D0) and hard-coded `'DXT1'` (`push 0x31545844` at 0x00BE8197, when the
  header's `isDXT` byte is set). `CTexture::LoadFromDataStreamToPreallocatedSurface @0x009FB750`
  reads a 19-byte (`0x13`) header, loops mips bounded by the SURFACE level count
  (`surface->vtable[+0x34]()`, compared at 0x009FBA4D), never reads the header `levels`; compressed
  branch consumes `bpp * w * h / 8` RAW bytes per level (0x009FB865: `imul` w, `imul` h,
  `shr eax,3`, `rep movsd`) — no LZO framing, no per-mip length.
  - Retail Darkwood_9 frame 4 header `4000 4000 01 e3040000 ...` (64x64, levels=1), texture spans
    `19 + 2048 = 2067` bytes; every retail patch spans `19 + mip0`.
  - Defect: FableForge emitted six levels (`8000 8000 06 ...`, 128x128, `19 + 10920 = 10939`) while
    the engine consumed `19 + 8192 = 8211`; the 2728 extra bytes desynced the stream, so edge-strip
    counts + `hasWater` EBOOL were read from mip-1 data; garbage `hasWater` entered
    `CEngineWaterBackgroundSubPatch::Load`, whose `s32` count reached `CRangeCompressor::Decompress`
    -> 31,171,392-byte `rep movsd` -> stage20 AV at `Fable.exe+0xb39f25`. Only the water loaders' `s32`
    counts can reach 31 MB (mesh `u16` counts cap near 1 MB). Fixed by
    `forge::stbbake::singleLevelBackgroundTexture`.
- **2026-08-22 — Patch body tail grammar (2026-08-22).** Mesh and water-only paths of
  `CLandscapeBackgroundPatch::Load` converge at 0x00BE8803: 0x00BE8810 4x `call 0xBFC600`
  (`CPatchTesselationEdgeStrip::Load`, this += 0x2C); 0x00BE8872 `u8 hasWater` (!= 0 -> allocate 0x60,
  `CEngineWaterBackgroundSubPatch::Load @0xBFD7C0`).
  - `CPatchTesselationEdgeStrip::Load @0x00BFC600` reads `[u16 -> +4][u16 -> +6][u8 -> +0x29]`, then
    four `LoadVerts` (0xBFC880 / 0xBFCA20 / 0xBFCBC0 / 0xBFCD60, stride 0x14); each reads one `s32`
    into the resize at 0x00BFBE40; zero consumes exactly 4 bytes (`newCount == 0` branch at
    0x00BFBE56). Empty strip = 21 bytes: `[u16][u16][u8][4 x s32 zero]`.
  - Header bytes 8 and 9 are not interchangeable: byte 8 -> `[edi+0x11a]`, reloaded at 0x00BE7FC1 to
    skip the mesh (`isWaterOnly`); byte 9 -> `[edi+0x118]` (`detailMode`). Retail and authored bodies
    carry `b[8]=0x00, b[9]=0x01`.
- **2026-08-22 — Runtime region vector is hard-capped at 142 (2026-08-22).** ForgeFSE probe, three
  deploys: BWD 143 regions -> `region_vector_size` 142, slot-399 owner 142,
  `GetRegionNumberMapIsIn(399)` = 0; BWD 145 -> 142 / 142 / 0; BWD 145 with owner 141 -> 142 / 141 /
  141. All three installed BWDs (`FinalAlbion.bwd`, `data\Levels\FinalAlbion.bwd`,
  `data\Levels\FinalAlbion\FinalAlbion.bwd`) held 145 regions; no shadow BWD in the WAD.
  - Region index 142 never answers containment lookups. A custom map must be owned by a region at
    index <= 141 — repurpose an existing one; ForgeTest64 uses retail region 141
    `Filler_NorthernWastes_02` (inert: `contains=0 sees=0`). Retires the "hard-cap theory refuted"
    note; the stage9 `ForgeTerminalSentinel` region never had any effect.
- **2026-08-22 — Inline-texture mip fix confirmed in-game (2026-08-22).** `LandscapeBackgroundRenderProbe`
  on authored ForgeTest64: `map=(32,0) size=(16,16) z=(72,83.962) vertices=289 polys=512 lod=1
  texBytes=8192 ibBytes=2174 vbBytes=6936 sharedIB=1 water=0 result=1 safe=1`. `texBytes=8192` = one
  128x128 DXT1 mip (17 of 18 patches; the last is 64x64 at 2048); `water=0` on all 18; no AV where
  stage20 faulted.
- **2026-08-22 — Background patch contract — three defects found in-game (2026-08-22).** All in
  `forge::stbbake::buildBackgroundPatchRect`; retail evidence Darkwood_3.
  - (1) `pw`/`ph` are SUBDIVISION counts capped at 16: `CLandscapeBackgroundPatch::Load` at
    0x00BE860A indexes a shared index-buffer table as `table[pw*17 + ph]` when `detailMode == 1`;
    16x16 is the largest entry (16*17+16 = 288). `pw=ph=64` gave 64*17+64 = 1152 -> crash at
    0x00BE8626. Retail has only 16x16 (289 verts) and 16x8 (153 verts) patches; a 32x32 world node
    still stores a 17x17 grid — parent LOD nodes must be stride-decimated.
  - (2) Vertex order is part of the contract (shared IB): canonical serpentine first-touch strip
    order `(0,0) (1,0) (1,1) (0,1) (0,2) (1,2) (1,3) (0,3) (0,4) (1,4) ... (1,16) (2,0) (2,1) ...`;
    Darkwood_3 frame 35 and `buildLayerTopology` agree byte-for-byte across 44 vertices.
  - (3) `indexCount` is the STRIP length: retail 16x16 stores 1085 (strip size minus two priming
    indices, same as the foreground `polygonCount`); `16*16*2 = 512` rendered banded terrain.
  - Dump a patch VB in stored order: `forge stb patchverts <chunk> <frame>`.
- **2026-08-22 — Foreground layer texture ids must be global bank ids (2026-08-22).**
  `LandscapeTextureProbe` on retail patches reports ids 4185/4304/4192/4307; ours `1` and `3` because
  `forge stb create-terrain`'s name path substituted a placeholder `material.textures = {1,2,3}`.
  Foreground textures are always GLOBAL GBANK_MAIN_PC ids in `textures.big` — write custom art into an
  `UNASSIGNED_*` slot and reference by id. Retail composites multiple layers per patch (Darkwood_3
  frame 0 has four, `mapping` 0/2/4) blended via the per-vertex `blend` byte with per-vertex
  `cliffU`/`cliffV`; a single layer with `blend=255` flat-paints the map.

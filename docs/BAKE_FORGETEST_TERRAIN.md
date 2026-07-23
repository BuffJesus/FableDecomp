# Baking ForgeTest terrain via the Unified Fable editor — run sheet

Grounded in three sources now: the UnifiedFable ReadMe (AlbionSecrets), the
community "Fable:TLC Map Editing Creation Tutorial" (MakhnoBlazed), and our own RE.
Goal: get the debug editor to **compile a real baked STB landscape chunk for
ForgeTest** so it renders instead of voiding — and that chunk doubles as the
byte-exact oracle to finish `forge::stbbake` natively.

> **TURNKEY BUNDLE (2026-07-20):** everything staged for the interactive bake is at
> `work/forgetest_editor_bake/` (mirror of `work/forgetest_bake/turnkey_bundle/`) — authored
> `ForgeTest.lev`, the bake-mode `userst.bake.ini`, `banks.ini`, `edit_settings.ini`, an
> `assemble_gameroot.ps1` that builds a ready gameroot from the retail install (junctions data,
> omits the STB to force a bake, injects the `.lev`), a `README.md`, and a full `RUN_SHEET.md`.
> Start there. Verified on this box: retail install, both debug exes (`debug_build/{ego_r,FableWin}.exe`),
> and `deploy.ps1` are all present; `ForgeTest.lev` themes resolve via `forge lev themegrid`.
> The native by-name STB write path (own-it route) is scoped in `docs/NATIVE_STB_WRITER_SCOPE.md`.

> **VERIFIED CORRECTIONS (2026-07-20, from the binaries themselves):**
> 1. **The config file the editor reads is `userst.ini`, NOT `dbugst.ini`.** `ego_r.exe`'s wide
>    strings reference `userst.ini / default_userst.ini / edit_settings.ini / banks.ini /
>    banks_dvd.ini / turn_off|on_detail.ini` and contain **no `dbugst.ini` string at all**.
>    Put the bake flags (`AllowDataGeneration TRUE; BuildRetailStaticMaps TRUE; ...`) in
>    `userst.ini`. `dbugst.ini` is inert for this exe. (The bake console *verbs* are all present
>    in ego_r as ASCII: `AllowDataGeneration, BuildRetailStaticMaps, ForceStaticMapUpdateOnMap,
>    OnlyUpdateNonExistantStaticMaps, AutomatedMode`.)
> 2. **The bake CANNOT be driven headless / by an agent.** `ego_r.exe`, `FableWin.exe`,
>    `Fable.exe` are all PE **subsystem 2 (GUI)**; ego_r imports `d3d9.dll` + `d3dx9_43.dll`
>    (needs a real D3D9 device + interactive desktop) and hits an early startup guard
>    ("This directory has restricted permissions…") plus a profile-selection gate. Launched
>    non-interactively it **exits code 1 in ~0 s** — before graphics init, no window, no
>    `.Gfx.Log`, no STB. Tested empirically with a junctioned scratch gameroot (STB omitted,
>    bake flags + `AutomatedMode TRUE`). **You must run it interactively on a GPU desktop.**
> 3. **The retail install has no loose `ForgeTest.lev`** — only `ForgeTest.tng`. ForgeTest's
>    live geometry lives inside `FinalAlbion.wad`. So the authored hill `.lev` **must be injected
>    at `data\Levels\FinalAlbion\ForgeTest.lev`** for the editor to bake ForgeTest's terrain
>    (the assemble script does this). The `.lev` is validated: 33×33, sub-ver 9, 27 themes,
>    130,579 B, roundtrips byte-exact.
> 4. The editor resolves `data\` **relative to its CWD** (registry key
>    `Software\Microsoft\Microsoft Games\Fable TLC` exists but carries no install-path value).
>    Run ego_r.exe *from inside the gameroot folder*.

## What's already done for ForgeTest (we don't repeat the tutorial's from-scratch steps)
The tutorial builds a new map from nothing (drag a 2D box, paint, add region,
text.big TXT entry, Region.def, FinalAlbion.wld NewMap line, text.h +1, etc.).
**We already have all of that for ForgeTest:**
- ForgeTest is `NewMap 399` in `FinalAlbion.wld`/`.bwd`, in a region, in the WAD,
  with a `CStaticMapInfoBlock` — and it's been entered + walked live.
- `ForgeTest.lev` is authored with **real terrain**: a sculpted central hill
  (center raised +14 → ~54 wu, edges preserved at ~43, no cliff) over the donor's
  real `GROUND_SPOOKY*` themes. Staged at
  `work/forgetest_bake/ForgeTest.lev` (byte-exact writer, nav/obstacle preserved).

So for us the tutorial collapses to: **make the editor load ForgeTest, confirm the
terrain, and compile the STB.**

## The gotchas the tutorial/ReadMe insist on (don't skip)
1. **Move `FinalAlbion_RT.stb` out of `Data/Levels` before opening the editor** —
   the editor CRASHES on the existing STB ("Lock to edit" crash). Renaming/moving
   it is mandatory; `ego_r` regenerates a fresh one on compile.
2. **`userst.ini` has two modes** (NOT `dbugst.ini` — see correction above). Editing:
   `AllowDataGeneration FALSE`. Compiling the STB: `AllowDataGeneration TRUE` (+
   `BuildRetailStaticMaps TRUE`). Keep `UseRetailBanks TRUE; UseCompiledDefs TRUE;
   SetLevel("FinalAlbion.wld")`. Ready-made: `turnkey_bundle/userst.bake.ini` and
   `userst.editmode.ini`.
3. **Win11 compat:** run `ego_r.exe` in **Windows XP SP3** compatibility mode;
   `FableWin.exe` **without** it.
4. Editor GUI textures look corrupt under `UseRetailBanks TRUE` — cosmetic only.
5. `SaveAll` often (editor crashes randomly). Console (`~`):
   `EnableScreenEffectGlowRenderer FALSE`, `SetTimeOfDay(11` for daylight.
6. The compile is a **full-world STB rebake — it takes a long time** (all maps,
   since the STB was moved). That's expected.

## Procedure
**Prep (I can stage this for you):**
- A UnifiedFable data folder = our current retail data + `work/forgetest_bake/
  ForgeTest.lev` in place at `Data/Levels/FinalAlbion/ForgeTest.lev` (loose) and in
  `FinalAlbion.wad`; ForgeTest present in `FinalAlbion.wld`/`.bwd`.
- You supply `Fable.exe` + `ego_r.exe` + `FableWin.exe` in the UnifiedFable root.

**Run:**
1. Move `Data/Levels/FinalAlbion_RT.stb` out of the folder (backup).
2. (Optional but recommended) Open `FableWin.exe`/`ego_r.exe` editor → Options →
   3D Engine → Edit World Map → click ForgeTest → **"Locked for Editing"**. Confirm
   the hill + `GROUND_SPOOKY*` theme are present (our pre-authored `.lev`). Paint/
   smooth more if you want (Height Mode toolbox: Paint Height value, Smooth,
   DrawPaths for peaks). `SaveAll`.
   - If the editor doesn't pick up our authored heights, paint the hill here — the
     `.lev` is the terrain source either way; this is the tutorial-proven fallback.
3. Set `userst.ini`: `AllowDataGeneration TRUE; BuildRetailStaticMaps TRUE;
   OnlyUpdateNonExistantStaticMaps TRUE; UseRetailBanks TRUE; UseCompiledDefs TRUE;`
   (use `turnkey_bundle/userst.bake.ini`).
4. Run `ego_r.exe`, choose a profile. It **compiles `FinalAlbion_RT.stb`** into
   `Data/Levels` (long — full world). Wait for it to finish.
5. That new STB has ForgeTest's baked terrain. Deploy it (our `deploy.ps1` already
   copies `FinalAlbion_RT.stb`) → **ForgeTest renders the hill. Live win.**
6. To edit again later: swap in `userst.editmode.ini` (`AllowDataGeneration FALSE`) first.

## Then: use the baked chunk as the native-baker oracle
7. Extract ForgeTest's chunk from the new STB (`forge stb` / stb infoblock tool).
   `forge::stbbake::parseChunk` already round-trips it byte-exact and classifies
   FRAME/HDR/PAD; the 4 §3 foreground patches parse.
8. **Bake-and-diff** to reverse the tree-node/quadtree HDR grammar (our one OPEN
   item): compile ForgeTest twice with a single controlled `.lev` change (e.g. one
   cell height), diff the two chunks. The bytes that move pin how the quadtree
   directory references its mesh FRAMEs by offset — completing `forge::stbbake` so
   FableForge emits terrain natively, editor-free.

## Optimization to try (avoids the long full rebake)
Instead of moving the whole STB, keep it and use
`ForceStaticMapUpdateOnMap("ForgeTest")` (or `OnlyUpdateNonExistantStaticMaps TRUE`
if ForgeTest has no chunk) so only ForgeTest bakes. The tutorial's move-the-STB
path is the safe/proven one; this is a faster variant to test once the basic bake
works.

Cross-refs: `work/forgetest_bake/ForgeTest.lev` (authored hill), UnifiedFable
`dbugst.ini` (scratchpad `unified_cfg/`), `work/terrain_path/CHUNK_LAYOUT_SPEC.md`,
`libs/forgecore/src/stbbake.cpp`.

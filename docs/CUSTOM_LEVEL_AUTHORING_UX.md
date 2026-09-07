# Custom level authoring in FableForge — the complete user journey

Written 2026-08-23. Purpose: enumerate *everything* a user must touch to build a custom level or
region and have the player legitimately walk into it, then mark what FableForge has, what it lacks,
and the order to build the gaps. Evidence for every engine claim is cited inline.

## 0. The acceptance test

> A user with no RE knowledge picks an install, draws a region, sculpts a valley, drops their own
> PNG on the material palette and paints it, scatters grass and trees, places a signpost and a door,
> connects that door to an existing retail level, presses Test, and **walks** into their level from
> Greatwood. No command line, no hex, no bank ids, no `.qst` hand-edit.

"Walks" is the load-bearing word. A debug teleport proves streaming; it does not prove the level is
part of the world. Everything below is ordered around that.

## 1. What a custom level actually IS (the full artifact set)

| Artifact | Holds | Authored by |
|---|---|---|
| `FinalAlbion.wld` | map slot, `MapX`/`MapY`, `LevelName`, `LevelScriptName`, `MapUID`; region blocks with `RegionName`, `NewDisplayName`, `RegionDef`, minimap/worldmap graphics + offsets, `ContainsMap`, `SeesMap` | World Browser / Create Region |
| `FinalAlbion.bwd` | compiled binary world (bounds, counts, region membership) | `forge wld compile` at deploy |
| `<Level>.lev` | heightfield, 256 ground themes, per-cell 3-theme blend, walkability, preferred-path, navigation sections | Landscape (sculpt/paint) |
| `<Level>.tng` | every thing: `Object`, `Marker`, `Thing`, `AICreature`, `Building`, `TrackNode`, `Village` | Place tool |
| `FinalAlbion_RT.stb` | baked static map: foreground layers (texture triples + per-vertex blend/cliff), background, local-detail foliage, **plus the mandatory common-header chunk** | Bake |
| `textures.big` `GBANK_MAIN_PC` | terrain diffuse textures (6,293 entries; **only 8 truly free `UNASSIGNED_*` slots** -- see section 3) | PNG import |
| `graphics.big` `MBANK_ALLMESHES` | meshes for foliage/props (8,113 entries) | Asset import |
| `text.big` | `TXT_*` display strings (region names, sign text) | Text editor (missing) |
| `game.bin` | `OBJECT_*` / `REGION_*` / `CREATURE_*` defs | Records panel |
| `FSE_Master.lua` + `FinalAlbion.qst` | ForgeFSE scripts and quest activation | Script/node editor |

A level is therefore a **transaction across ~8 files in 3 formats**, which is exactly why the UI must
own it as one object ("the project") and never make the user reason about the pieces.

## 2. How the player gets in — the four entry mechanisms

Recovered from retail TNGs this session (`data/Levels/FinalAlbion/*.tng`).

### A. Region exit -> entrance pair (the walkable door). This is the primary mechanism.

Retail census: **372 `CTCDRegionEntrance`, 258 `CTCDRegionExit`.** Verbatim retail exit from
`Greatwood_1.tng`:

```
NewThing Thing;
Player 4;
UID 18446741874686296073;
DefinitionType "REGION_EXIT_POINT";
ScriptName NULL;
ScriptData "NULL";
ThingGamePersistent FALSE;
ThingLevelPersistent FALSE;
StartCTCPhysicsStandard;
PositionX 30.899902;  PositionY 156.005859;  PositionZ 30.657928;
RHSetForwardX 0.306239; RHSetForwardY 0.951949; RHSetForwardZ 0.0;
RHSetUpX -0.00087; RHSetUpY 0.00028; RHSetUpZ 0.999994;
EndCTCPhysicsStandard;
StartCTCEditor;
EndCTCEditor;
StartCTCDRegionExit;
Active TRUE;
Radius 3.0;
MessageRadius 11.0;
ReversedOnMiniMap FALSE;
HiddenOnMiniMap FALSE;
EntranceConnectedToUID 178605768327561327;
EndCTCDRegionExit;
EndThing;
```

The matching entrance is trivial — `DefinitionType "REGION_ENTRANCE_POINT"`, a physics block, then
`StartCTCDRegionEntrance; Active TRUE; EndCTCDRegionEntrance;` — and carries a human-readable
`ScriptData "FromLookoutPoint"` naming the source map.

**Two facts that decide the whole feature:**

1. `EntranceConnectedToUID` is the **runtime-remapped identity** of a
   `REGION_ENTRANCE_POINT` in a different level's TNG, not a literal copy of
   that file's serialized `UID`. Retail TNG thing IDs use the
   `0xFFFFFE00xxxxxxxx` map-local serialization sentinel and deliberately repeat
   between maps. A cross-map reference replaces the sentinel with the
   destination WLD MapUID: `(MapUID << 40) | localThingId32`. For example,
   `178605768327561327 = 0x027A89000000006F`; WLD MapUID `0x027A89` selects
   `LookoutPoint`, whose serialized local thing `0x...006F` is the entrance.
   `forge tng transition-audit` validates this over retail FinalAlbion: all 129
   region-exit links and all 53 interactive-teleporter links resolve uniquely,
   and every target is `REGION_ENTRANCE_POINT`. The editor must allocate a safe
   local ID and build cross-file references with the destination MapUID.
2. `Position*` in a TNG is **map-relative, not world-absolute**. The exit above sits at
   (30.9, 156.0) inside `Greatwood_1`, whose WLD origin is `MapX 3264 / MapY 3296` and whose bounds
   are 96 x 192. Placing at world coordinates would parse perfectly and land the object ~3 km away.
   `PositionZ` is height.

A two-way doorway is therefore **4 things**: exit A -> entrance B, exit B -> entrance A.

### B. Building door — `CTCDoor { Open FALSE; }`, 346 retail instances. Interior transitions.

### C. Teleporter — `CTCTeleporter`, 26 instances. Guild seal / world-map fast travel. Requires the
region to carry `AppearOnWorldMap`, `RegionDef`, `MiniMapGraphic`, and world/minimap offsets.

### D. Script teleport (ForgeFSE) — correct for quests and for the editor's own "Test here" button;
wrong as the shipping way a player reaches a level.

Retail also uses **leadout corridor maps** (`*_Leadout_01`) to stitch regions — a pattern the wizard
should offer, not force.

## 3. Constraints the UI must enforce up front (never discover at runtime)

- **Region "cap" — CORRECTED 2026-08-23. There is no fixed ceiling.** The earlier note that the
  runtime region vector is hard-capped at 142 is wrong, and `docs/HANDOFF.md` already refuted it: an
  FSE probe measured `region_vector_size = 142` at a moment when the data held **141** regions, i.e.
  the vector is sized from the world data (slots are 1-based, so size = count + 1). The installed
  world runs today at **145 regions / 399 maps** and teleports fine, which is direct empirical proof.
  The real failure was never truncation — map 399's *owning region membership* was not found at
  runtime and `GetRegionNumberMapIsIn` resolved to 0. Attaching the map to a region that actually
  contains it fixed it. So the UI rule is **"every map must be owned by exactly one region that
  contains it"**, enforced by validation, not a budget meter. A budget display is still worth showing
  as information, but it must not block.
- **Map slots** ceiling at 399/400.
- **STB common-header chunk is mandatory.** `OpenRetailStaticMap` (0xB41E50) does an *unchecked*
  `map<name,offset>` lookup per registered map; a miss garbage-seeks -> **CTD 0xA2428A**. Registering
  a map without its chunk is an instant crash, so Bake and Validate must treat it as fatal.
- **Only EIGHT free texture slots, not 156. CORRECTED 2026-08-23.** `forge texture free-slots`
  resolves every `ENGINE_THEME` def's six texture fields: 463 themes reference 247 distinct ids, and
  of the 156 `UNASSIGNED_*` entries **148 are already referenced by a theme**. Free: 4200
  CLIFF_SANDSTONE, 4241 PATH_MOSSY, 4263 SAND_SCRUB_02, 4277 TOWNGROUND, 4287 NW_SNOWCLIFF_01,
  4289 NW_SNOWCLIFF_BUMP_02, 4315 CLIFF_SANDSTONE_BUMP, 4330 PATH_MOSSY_BUMP -- two of which are
  bump maps, so ~6 usable diffuse slots. "UNASSIGNED" is a NAME, not a vacancy: overwriting a
  referenced one repaints real terrain elsewhere in Albion. A ledger is still needed, but the
  headline is that slot reuse is a demo-sized budget, not a project-sized one.
- **~6 foreground layers per patch** (retail Darkwood_3). The four-direction cliff fix already spends
  4 on one material. Paint must blend or warn, never silently drop a layer.
- **Levels stream from `FinalAlbion.wad`**, not loose `.lev` — the packager must repack.
- **Saves cache region entities.** Testing a changed level needs a fresh New Game or a save from
  before the region was first visited, or the player sees stale contents and blames the tool.
- **Steam "verify files" wipes** the `AddQuest(name, TRUE)` line in `FinalAlbion.qst` that activates
  custom quests. The installer must be able to re-assert it.
- **Grass currently needs the repeated-mesh manager enabled**; today that is a temporary probe force
  (stage55). Must become a real activation before any of this ships.

## 4. The journey, screen by screen

Status key: **[have]** works today, **[partial]** exists but wrong/incomplete, **[missing]**.

| # | Step | What the user touches | Status |
|---|---|---|---|
| 0 | Pick install + project | choose game root; New/Open Project (reversible copy) | **[partial]** path box only; no project concept in UI |
| 1 | Create region | name, display name, world-map placement, minimap art, budget meter | **[partial]** form exists; no budget/cap check, no minimap art, display name is a raw tag |
| 2 | Add level | name, size, world position, template | **[have]** via Create Region & First Level; **[missing]** adding *another* level to an existing region |
| 3 | Sculpt | 3D viewport, raise/lower/flatten/smooth, undo | **[have]** |
| 4 | Paint surface | material palette, drop a PNG to add a material, blend | **[partial]** theme painting is correct (a theme IS the material, see section 6); PNG import + material palette **[missing]** |
| 5 | Foliage | grass/plant palette, density brush, eraser | **[missing]** in UI (backend proven) |
| 6 | Props and NPCs | object palette, click-to-place, gizmo, snap-to-ground, inspector | **[missing]** — the Place tool |
| 7 | **Connect** | drop a doorway, pick target level, auto-create the exit/entrance pair both ways | **[missing]** — the centerpiece |
| 8 | Script | select thing -> node graph -> compile to ForgeFSE Lua + TNG ScriptName | **[missing]** |
| 9 | Navigation | walkable paint, preferred path, nav regen | **[partial]** LEV supports it; no UI |
| 10 | Validate | one list of errors, each click-to-fix | **[have]** `forge validate`; **[partial]** not wired to the new failure modes |
| 11 | Bake | terrain + foliage + textures -> STB | **[have]** CLI; **[missing]** in UI |
| 12 | Test | package, back up, install, launch, teleport to the level | **[partial]** scripts outside the GUI |
| 13 | Revert | one button back to pristine | **[have]** rollback script; **[missing]** in UI |

### The two screens that do not exist and matter most

**Connect (step 7).** In the 3D viewport, drop a *Doorway* on a map edge or a building. A panel asks
"leads to ->" with a searchable list of every level in the project *and* the retail world. On confirm
the tool writes all four things, allocates two UIDs in the retail range, sets
`ScriptData "From<Source>"`, and adds `SeesMap` entries both ways. A **Connections** view lists every
doorway with a red badge on any dangling `EntranceConnectedToUID` — the one error class that stays
invisible until a player walks into a wall.

**Place (step 6).** Palette of retail objects (signposts, doors, chests, lights, markers) filtered by
category, click to place on the terrain surface, gizmo to move/rotate, inspector for ScriptName /
text tag / persistence flags. Every other authoring step depends on it existing.

## 5. Build order

1. **Place tool + TNG append** (map-relative positions, UID allocation, retail-shaped blocks).
   Unblocks steps 6, 7, 8. First proof: a signpost in `Greatwood_1`, the level of the player's Save 1.
2. **Connect tool** — exit/entrance pairs, `SeesMap`, dangling-link validation.
3. **PNG material import** — encode to a free texture slot, point a theme's BaseTexture at it,
   surface it as a swatch. No new binding needed (section 6).
4. **Foliage and tree brush in the viewport** (backend proven this session).
5. **Bake / Test / Revert as GUI buttons** with a live log.
6. **Node editor** on a selected thing, compiling to ForgeFSE Lua.
7. **Region budget meter, minimap art, text-tag editor** — the polish that stops runtime surprises.

## 6. RESOLVED: a ground theme IS the material (2026-08-23)

The section-4 doubt ("Paint probably does not write appearance") is **answered: it does.** A ground
theme is an `ENGINE_THEME` def in `game.bin`, and its decoded fields are the whole surface contract.
`forge defs decode <root> def_schema.json GROUND_SS_GRASS` returns:

```
entry 1875: ENGINE_THEME / GROUND_SS_GRASS  (25 fields)
  BaseTexture              4267      CliffBaseTexture      4193
  BackgroundTexture        4267      CliffBackgroundTexture 4193
  BaseBumpMap              4299      CliffBumpMap          4308
  MaterialDef              2758      LocalDetailGeneratorDef 1847
  MinimapTheme             4         Friction              1.0
  WaterHeight / WaterType, CoverValue, Height, GroupDef ...
```

Those int32s are `textures.big` `GBANK_MAIN_PC` entry ids — the same id space as the 156
`UNASSIGNED_*` slots (verified: 159 name matches in the installed bank; `GBANK_MAIN_PC` currently
holds 6,293 entries, i.e. the install is **not** pristine).

**Consequences, all of which simplify the feature:**

1. Painting a theme already paints appearance, foliage eligibility (`LocalDetailGeneratorDef`),
   footstep material (`MaterialDef`), minimap colour, friction and water. A theme is the single
   authoring primitive a user should ever see — call it a **material**.
2. The PNG import path needs **no new binding**: encode PNG into a free `UNASSIGNED_*` slot, then
   point a theme's `BaseTexture`/`BackgroundTexture` (and optionally cliff/bump) at that slot id.
   `forge defs set-field` already exists for exactly this.
3. Prefer **in-place field edits** over appending a new def. CLAUDE.md's def-load contract warns that
   a game.bin APPEND only resolves if the names.bin CRC is crc0 *and* the cloned payload's self
   global-entry-index back-refs are retargeted; `setEntryData` field edits sidestep both. So v1 =
   claim an unused theme slot the same way we claim an unused texture slot. Appending is the
   scale-up, not the starting point.
4. The bake already consumes theme -> texture ids to build the foreground layer triples, so no new
   bake path is needed for custom textures — only correct ids.

Open sub-question: whether the LEV's 256-entry ground-theme table stores the def index or a name CRC
in its `u32 value`. That decides whether a per-map table entry must be added for a new material.

### 6.1 Sub-question answered, and a defect it exposed

The LEV 256-entry ground-theme table stores the **global `game.bin` def entry index**, not a name CRC.
Proven on retail data — `Greatwood_1.lev` (extracted from `FinalAlbion.wad`) stores:

```
GROUND_GRASS_NO_LOCAL_DETAIL 1917   <- forge defs show ... = entry 1917 ENGINE_THEME  (exact match)
GROUND_FOREST_LEAVES         1906   <- entry 1906 ENGINE_THEME                         (exact match)
```

**Our authored LEVs are wrong.** `work/no_donor_terrain_pack/ForgeTest64_final.lev` — and therefore
`AshfallHollow.lev`, which was cloned from it — carry the same names with indices **228 too low**:

```
GROUND_GRASS_NO_LOCAL_DETAIL 1689   -> entry 1689 = CREATURE / CREATURE_KN_VILLAGER_MALE
GROUND_PATH_SAND_DARK        1687   -> entry 1687 = CREATURE / CREATURE_BANDIT_FISTICUFFS_MEMBER
GROUND_SPOOKY_TEST03         1755   -> entry 1755 = CREATURE / CREATURE_GUILD_VILLAGER_SERVANT_MALE
```

The bias is *uniform*, so these were authored against a different `game.bin` (fewer preceding
entries) and never rebased onto this install.

**The runtime does consume the index.** `CMap`'s own member names settle it (FableWin symbol table):

```
?AddThemeDefIndexToPalette@CMap@@IAEEK@Z             u8  AddThemeDefIndexToPalette(u32 defIndex)
?GetThemePaletteIndexFromThemeDefIndex@CMap@@IAEEK@Z u8  GetThemePaletteIndexFromThemeDefIndex(u32)
?FindUnusedThemePaletteEntryIndex@CMap@@IAEEXZ       ?InitThemePalette@CMap@@IAEXXZ
?InitPassabilityFromThemesIfNecessary@CMap@@QAEXABVC2DBoxI@@@Z
?GetThemeForMiniMapAt@CMap@@QBEJABVC2DCoordI@@@Z     ?GetThemeSizeZAt / ?GetThemeMinCameraSizeZAt
```

So the 256-entry table is a **theme palette: palette slot (the u8 the cells store) -> theme def
index (the u32)**, and the def is read for passability seeding, minimap colour and camera Z clamps.
The 128-byte name is the editor's label, not the lookup key.

Why ForgeTest64 still looked fine: those indices land on `CREATURE` defs of 1,288 bytes, so every
read stays in bounds and returns plausible-looking garbage rather than faulting. The visible terrain
texture came from the STB bake's explicit texture ids, not from the theme, which is why the wrongness
never reached the screen. This is a live correctness bug with a quiet failure mode, not a crash.

It matters anyway, and it matters *first*, because the material feature is index-shaped:

- A custom material is a theme whose `BaseTexture` points at an imported slot. If the engine ever
  does use the index, a stale one silently paints the wrong material — the exact failure mode that
  looks like "the tool ignored my texture".
- Any authored LEV must have its theme table **rebased by name against the target install's
  `game.bin`** at save/bake time. That is a small, well-defined pass the writer does not do today.
- It also means adding defs to `game.bin` shifts global indices and can invalidate theme tables
  across every map — a second, independent argument for in-place field edits over appends.

**Fix**: a rebase-by-name pass over the theme palette at save/bake, plus a validator rule that flags
any palette entry whose def index is not an `ENGINE_THEME`. Both are cheap and both belong in the
tool before any material work lands on top of them.

## 7. Atmosphere: sky, lighting, music — all hang off the REGION def

A region's whole mood is one def, and it is reachable with the same in-place field edit the material
feature uses. `forge defs decode <root> def_schema.json REGION_GREATWOOD_TELEPORT` (entry 7146, 33
fields) opens with:

```
EnvironmentTheme                2346   -> ENVIRONMENT_THEME_DAY / ENVIRONMENT_THEME1
InternalEnvironmentTheme        2359   -> ENVIRONMENT_INTERNALS   (interiors)
InternalLightingChannelCount       1
Music                             16
ReverbEnvironmentType / Level      1 / 1.0     InBuildingOcclusionLevel  1.0
UseVillageTime false   AllowHenchmen true   HeroBreathParticle 0
MaxNumAttackersOnHero -1   CameraManagerSetIndex -1   RegionDescription 14244
WeaponOutCrime / UseCrimes / DisplayCrimes  false
```

So the chain is: **WLD region block -> `RegionDef` -> REGION def -> `EnvironmentTheme` ->
ENVIRONMENT_THEME_DAY def**. That theme def is the sky and lighting:

```
ENVIRONMENT_THEME_DAY / ENVIRONMENT_OAKVALE  (entry 2355)
  Time                vector<CEnvironmentThemeDef> count=7   <- seven times of day
  SunTilt 0.18   SunRotate 0.25   SunHeight 0.2
  MoonTilt -0.4  MoonRotate 0.35  MoonHeight 0.9
  ColourLookupColumn 36    EditorColour ff8080ff    FishWeightMult 1.0
```

39 `ENVIRONMENT_THEME_DAY` defs ship (OAKVALE, WITCHWOOD, HOOKCOAST, CAVE, HAUNTED, GRAVEYARD,
UNDERTREES, INTERNALS, plus effect themes like BULLET_TIME and JACK_OF_BLADES_GLOW).

### The sky itself

There is exactly **one** `SKY` def — `SKY_DEF`, entry 7283 (plus the NULLDEF):

```
SunRadius 100        SunTexture 384        SunFlareRadius 32000   SunFlareTexture 385
MoonRadius 300       MoonTexture 383       StarTexture 401        StarChartTextureSize 400
StarSize 7.0   StarChartFilter 16   TwinkleInterval 100   TwinkleSpeed 4.0   TwinkleMin/Max 0 / 4
FlareElements   vector<CLensFlareElementDef>   (10 elements)
```

The dome is **not** a textured skysphere: `CEngineSkyRenderer::BuildOuterSkyMesh` / `RenderOuterSky`
build it procedurally, and its colour comes from the environment theme's per-time-of-day entries and
`ColourLookupColumn`. Only the sun, moon, stars and lens flares are textures — and those ids
(383/384/385/401) live in the same `textures.big` space as everything else.

**What this gives the user, cheaply:**

| Want | Do | Scope |
|---|---|---|
| A different mood for my region | point the REGION def's `EnvironmentTheme` at another of the 39 | per region |
| Custom sun angle / day cycle / colours | clone-and-edit an `ENVIRONMENT_THEME_DAY` (7 time entries) | per region |
| Custom sun, moon, star art | swap `SKY_DEF`'s texture ids to imported slots — same PNG path as terrain | **global** |
| Different music, reverb, crime rules, henchmen | other fields on the same REGION def | per region |

The one trap: `SKY_DEF` is global, so editing it changes the sky everywhere. Per-region variation
must go through `EnvironmentTheme`, and the UI should say so rather than let someone "customise the
sky" and silently repaint Albion.

Note also that `EnvironmentTheme 2346` is another **global def index**, exactly like the ground-theme
palette in section 6.1 — so the rebase-by-name pass and the type-checking validator rule cover this
binding too. Build them once, use them everywhere.

### 7.1 Editing sky art and themes — most of it already exists (2026-08-23)

The sky textures are ordinary named bank entries, and `tools/texture_build.py` already round-trips
them. Verified by exporting all four straight out of the retail bank:

```
GRAPHIC_ATMOSPHERIC_MOON       id 383  DXT3 512x512   (185 KB on disk)
GRAPHIC_ATMOSPHERIC_SUN        id 384  DXT3 128x128
GRAPHIC_ATMOSPHERIC_SUN_FLARE  id 385  DXT1 256x256
GRAPHIC_ATMOSPHERIC_STAR_01    id 401  DXT1  16x16
```

`texture_build.py decode <big> <name> <out.png>` writes an editable PNG (the moon comes out as a
clean 512x512 with alpha). So the whole loop is: **decode -> edit in any image editor -> replace**.

The CLI already covers every step:

| Want | Command that exists today |
|---|---|
| See / edit the current art | `texture_build.py decode <big> GRAPHIC_ATMOSPHERIC_MOON moon.png` |
| Replace it in place | `texture_build.py replace <src.big> <out.big> GRAPHIC_ATMOSPHERIC_MOON moon.png` |
| Keep the original, add a new one | `texture_build.py add <src.big> <out.big> GBANK_MAIN_PC MY_MOON moon.png` — **prints the new id** |
| Point the sky at the new art | `forge defs set-field game.bin SKY_DEF MoonTexture <id>` |
| Verify nothing else moved | `texture_build.py roundtrip <src.big>` |

**Correction, twice over.** First: only **8** `UNASSIGNED_*` slots are genuinely unreferenced, not
156 (section 3). Second: append is therefore not an escape hatch, it is the main road for any
project with more than a handful of custom textures. Both `texture_build.py add` and
`forge texture import --add` perform the GBANK append and print the new id.

Append carries one **unverified engine assumption**: that the global `CGraphicInfoFrame` pool
grows with the bank. `LoadForeground` (0x00bfe050) bounds-checks and fails gracefully on an
out-of-range id, so the failure mode is a missing texture rather than a crash -- but it has not
been tested in game. Until it is, the honest ordering is: **use one of the 8 free slots first, and
treat `--add` as promising-but-unproven** behind an explicit in-game check.

So "let people edit the sky" is a **UI surface over existing, proven commands**, not new RE:

- A **Sky** panel on the region: a thumbnail each for sun, moon, flare, star, with Export / Replace
  buttons, plus the numeric knobs from `SKY_DEF` (radii, star size, twinkle interval/speed/min/max).
- A loud, permanent banner on that panel: **there is only one `SKY_DEF`** — changing it changes the
  sky for all of Albion. Per-region mood belongs on the environment theme.
- An **Atmosphere** panel on the region: pick from the 39 shipped `ENVIRONMENT_THEME_DAY` defs with a
  live preview of sun/moon angles, or **Duplicate** one to get an editable copy, exposing the seven
  time-of-day entries, `ColourLookupColumn`, and the sun/moon tilt-rotate-height triples.
- Both write through `forge defs set-field`, which is an in-place payload edit — the safe path that
  avoids the def-append contract entirely.

The same rebase-and-validate rule from section 6.1 applies here: `EnvironmentTheme`, `SkyDef` and
every other cross-def reference is a global index, so a project that ships an edited `game.bin` must
have its indices resolved by name at deploy, not copied from the authoring machine.

## 8. Custom foliage a modder supplies (grass, flowers, trees)

Foliage is not a special asset class. It is **a mesh + a texture + one def**, and the def is already
fully described. `GROUND_SS_GRASS` points its `LocalDetailGeneratorDef` at entry 1847 =
`LOCAL_DETAIL_GENERATOR / LOCAL_DETAIL_NW_GRASS`. 65 such generators ship. The shape is:

```
CEngineLocalDetailGeneratorDef -> Layers      vector<CEngineLocalDetailLayerDef>
CEngineLocalDetailLayerDef     -> SpacingFromLayer, Objects vector<CEngineLocalDetailObjectDef>
CEngineLocalDetailObjectDef (18 fields):
    <Mesh>  ShadowMesh  ZSpriteMesh                 <- three mesh-bank ids
    Probability  ThemeBlendThreshold                <- density, and how much theme weight it needs
    Scale  ScaleRandomElement                       <- size + per-instance variance
    FadeStart / FadeEnd                             <- draw distance
    ZSpriteFadeStart / ZSpriteFadeEnd               <- billboard LOD handover
    SlopeFadeStart / SlopeFadeEnd                   <- stop growing on cliffs
    AlphaRef  AlphaMipBias
    WindSkewConstantFactor / RandomFactor / SpeedFactor
```

That is the entire authoring surface a plant needs: what it looks like, how thick it grows, how big,
how far it draws, whether it climbs slopes, and how it moves in wind. Nothing here is unrecovered.

### The pipeline, end to end

| Step | Mechanism | Status |
|---|---|---|
| 1. Author the mesh | Blender via `tools/blender_addon/io_scene_fable` (`mesh_rw.compose_mesh`) | **exists** |
| 2. Add it to the mesh bank | `big_write.rebuild(adds=)` -> new `MBANK_ALLMESHES` id | **exists** (`docs/MESH_COMPOSE.md`) |
| 3. Add the diffuse | `texture_build.py add/replace` -> texture id | **exists** |
| 4. Describe the plant | clone a `LOCAL_DETAIL_GENERATOR`, set Mesh + Probability/Scale/Fade/Slope/Wind | needs UI; `defs set-field` exists |
| 5. Bind it to a surface | point a ground theme's `LocalDetailGeneratorDef` at it | needs UI; in-place field edit |
| 6. Scatter it | the STB local-detail writer (batches <=32, subsection tables, node spheres) | **proven this session** |

Retail grass is a **4-triangle** mesh, so a plant is a trivial asset -- the scary part was never the
geometry, it was the six bindings around it.

### What the user should actually see

Two ways in, and both should exist because they answer different questions:

- **"Paint a material that grows this."** The retail-faithful path: a plant belongs to a ground
  theme, so painting grass *is* planting grass. This is what makes a custom level feel authored
  rather than decorated, and it costs nothing extra once themes are editable.
- **"Paint these plants here."** Direct scatter into the map's local detail, for hand-placed
  clusters -- a flowerbed, a copse -- independent of the surface underneath.

A **Plant** asset in the UI is therefore one object bundling: a mesh file, a PNG, and the generator
fields. Import wizard: drop an `.obj`/`.blend` export + a PNG, name it, get a swatch. Everything
below that (mesh id, texture id, def index, palette slot, batching) stays invisible.

### Caveats to design around, not discover

- **Trees are not grass.** A tree is a normal `Object` thing in the TNG placed by the Place tool,
  with collision and a shadow; local detail is for dense, non-colliding, fade-out clutter. The UI
  should route "tree" to placement and "grass/flowers/bracken" to the foliage brush, and say why.
- **New defs vs. reuse.** Adding a `LOCAL_DETAIL_GENERATOR` means a `game.bin` append, which the
  def-load contract makes delicate (names.bin crc0 + self-index retargeting). 65 generators ship;
  repurposing an unused one via in-place `set-field` is the safe v1, exactly as with texture slots.
- **Index fragility again.** `LocalDetailGeneratorDef`, `Mesh`, `ShadowMesh`, `ZSpriteMesh` are all
  global indices. Same rebase-by-name discipline as section 6.1.
- **`ZSpriteMesh` is the billboard LOD.** Its on-disk batch grammar was recovered this session
  (`44 + count*84`), so distant foliage is authorable rather than a black box.

## 9. Acceptance tests

- **T1 sign**: a signpost appears in `Greatwood_1` at the placed spot, readable, retail install untouched.
- **T2 doorway**: walk from a retail level into the custom level and back, no teleport.
- **T3 texture**: a user PNG appears as a swatch, paints, survives bake, renders in game.
- **T4 revert**: one button returns the install to pristine, verified by hash.

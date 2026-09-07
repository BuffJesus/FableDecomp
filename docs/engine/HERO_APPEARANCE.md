# Hero appearance — equipment, skin/morph, creature modifiers, mesh deform

Merged reference (2026-09-07) for everything that puts a mesh or a texture on the hero (and on
creatures via appearance modifiers). Consolidates the former `APPEARANCE_SYSTEM.md` (2026-07-20),
`APPEARANCE_STRIP_FINDINGS.md` (2026-07-24), `HERO_MORPH.md` (2026-07-20) and
`HERO_MESH_DEFORM_FIX.md` (2026-07-21). Every fact, table, offset and address from those documents
is preserved below; dated findings are listed once as one-line bullets; claims that later evidence
overturned are collected under [Superseded](#superseded).

Evidence base: `tools/appearance_survey.py` (reproduces every clothing claim against the retail
install), decompiles in `ghidra_out/appearance_decomp.c`, HalsSword.fmp study, ChocolateBox
`data/def.xml` (community FableExplorer schema, corroborating), `tools/parse_mesh.py` descriptors,
`forge defs decode`, textures.big census, `hero_mesh_decomp.txt` (96 fns), `work/appearance_re/*`.

Contents

- [Dated findings](#dated-findings)
- [Equipment and clothing system](#equipment-and-clothing-system)
- [Creature appearance modifiers and the clothing strip](#creature-appearance-modifiers-and-the-clothing-strip)
- [Hero skin and morph texture chain](#hero-skin-and-morph-texture-chain)
- [Hero mesh deform bug (Banjo)](#hero-mesh-deform-bug-banjo)
- [Open questions](#open-questions)
- [Superseded](#superseded)

---

## Dated findings

- **2026-07-20** — Wearable item = game.bin `OBJECT` whose CDef listing points at an unnamed
  `CAppearanceModifierDef`; worn meshes are BIG type-5 skinned meshes on a hero-rig subset, drawn as
  `CEnginePrimitiveMorphedAnimatingMesh` sharing the hero's interpolators; 148/148 vanilla modifiers
  resolve to real `MBANK_ALLMESHES` entries.
- **2026-07-20** — Skinned type-5 compose gap CLOSED: `mesh_rw.compose_mesh(..., skeleton=
  mesh_rw.clone_skeleton(donor_pay, 5))`; Blender round trip of a re-skinned boot injected as id 8115.
- **2026-07-20** — Adult hero body is `MESH_HERO` (id 4299); `GRAPHIC_HERO_FACE` (45) is referenced by
  NO mesh — the stage-1 green-tint smoke test targeted the wrong texture. Morphs composite
  `COMP_HERO_*` overlays onto `HERO_SKIN_*` base sheets per `CHeroMorphDef.TextureMorphs`
  (game.bin 10535/11246, 156 records each, identical).
- **2026-07-20** — Stage-2 smoke test staged (23 sheets tinted green, NOT deployed);
  `tools/hero_overlay.py` custom-overlay authoring lane built (new textures.big entry id 6291 +
  TextureMorphs append in both CHeroMorphDef entries; worked example
  `COMP_HERO_SKIN_FACE_FORGE_CROSS`, records 156→158).
- **2026-07-21** — Banjo hero-mesh deform bug root-caused to authoring-space weighting in
  `build_banjo_animated.py` (head-only uniform-scale fit; WEAPON_FOCUS/WEAPON_SCABBARD deform
  disabled) — the byte format is NOT the bug.
- **2026-07-24** — Clothing-strip workflow (`appearance-clothing-re`, verdict CONFIRMED 7/0/3)
  corrected empirically: `InitialAppearanceModifiers` (tag `0x8F20181F`) lives on the `CCreatureDef`
  sub-def (330 entries), not the `CREATURE` wrapper; TRADER_01's list is `count = 0`, so the
  trader's clothing is NOT layered via it — the real lever is `CAppearanceDef`[10745] (18,103 B).

---

## Equipment and clothing system

*Originally `APPEARANCE_SYSTEM.md`, 2026-07-20. How equipping an item puts a mesh on the hero: the
complete slot -> mesh -> body-region model, decompile-confirmed, plus the recipe for a
custom-equipment ("new boots") mod.*

### TL;DR

- A wearable item is a **game.bin `OBJECT` entry** whose **CDef listing** (a table in the entry's
  payload *prefix*) points at an unnamed **`CAppearanceModifierDef`** entry (plus `CInventoryItemDef`,
  `CStockItemDef`, and template-inherited physics/targeting/sound defs).
- The `CAppearanceModifierDef` carries the whole worn-appearance contract: **`Graphics[]` ->
  graphics.big model IDs** (the worn meshes), **`SuitPart`** (equipment slot),
  **`CoversBodyAreaFlags`** (body-region mask), **`HideMaterials`** (hero body materials to hide),
  armour link, align stats.
- Worn meshes are **BIG type-5 skinned meshes** whose skeleton is a named subset of the hero Biped
  rig; the engine draws each as a `CEnginePrimitiveMorphedAnimatingMesh` sharing the hero's
  animation interpolators (decompile-confirmed) — they deform with the hero.
- Validation: **148/148** vanilla OBJECTs with an appearance modifier resolve every `Graphics` model
  ID to a real `MBANK_ALLMESHES` entry. 0 failures.

### game.bin entry anatomy: the CDef listing

Every "top-type" entry (OBJECT / THING / CREATURE / ...) starts with a **CDef listing** instead of
the bare 5-byte prefix:

```
01 00 01                          base prefix
u16  cdefCount
cdefCount * 12 bytes:
    u32  classKey     = crc0(C*Def class name)      e.g. 83AD7C03 = "CInventoryItemDef"
    i32  dataEntry    = game.bin entry index of the component-def data
    i32  ownerEntry   = entry index of the OBJECT that owns/defines that data
```

`crc0` = CRC-32 poly `0xEDB88320`, **seed 0, no final xor** — the same hash as the field tags
(`crc0("HideMaterials") = 0x7A59BE69`; def.xml prints these byte-swapped). Records are sorted by
classKey.

`OBJECT_HERO_BOOTS` (entry 3404) decodes to 9 cdefs:

| classKey | class | dataEntry | owner |
|---|---|---|---|
| 3cb1b8f9 | CPhysicsDef | 9192 | 326 OBJECT_HOLDABLE_CLOTHING_TEMPLATE |
| 80321056 | CTargetingDef | 9189 | 325 OBJECT_HOLDABLE_TEMPLATE |
| 83ad7c03 | CInventoryItemDef | **11710** | 3404 (self) |
| 8659b28c | CHitLocationsDef | 9134 | 285 OBJECT_BASE |
| 9a961b88 | CActionUseDef | 9132 | 285 OBJECT_BASE |
| ba19ef3e | CCarryableDef | 9190 | 325 |
| dfacdc94 | CStockItemDef | **11711** | 3404 (self) |
| e1c89f9c | CEntitySoundDef | 9135 | 285 |
| 50545995 | **CAppearanceModifierDef** | **11712** | 3404 (self) |

So component data is **shared by inheritance**: template-owned defs are reused across items;
item-specific defs (inventory, stock, appearance) are unnamed entries owned by the item. The engine
resolves by `dataEntry` (`CDefinitionManager::GetDef(entryIdx)` + `GetSubDef<T>` in every decompiled
consumer); HalsSword ships stale `ownerEntry` values and still works.

The OBJECT's own tagged fields also matter for equipment:
- `Components` (`CThingComponentSet`): `u32 count` + per component `{u32 names.bin-offset of the
  CTC* component class name, u32 param, u8}` + a fixed 21-slot `{u32 crc, u8 flag}` table
  (undecoded, identical CRC set on every object). Clothing items list `CTCInventoryItem`,
  `CTCGraphicAppearance(StaticMesh)`, `CTCStockItem`, etc.
- `Graphic` (`CEngineGraphic`, 17 bytes `{u32 kind=5, u32 modelId, u32 0, f32 scale=1.0, u8}`): the
  **world/ground prop** mesh — for OBJECT_HERO_BOOTS modelId 129 = `MESH_HERO_FOLDED_BOOTS_STANDARD`
  (BIG type-1 static). For weapons this is also the held mesh (katana -> 7942
  `MESH_KATANA_LEGENDARY_01`).

### CAppearanceModifierDef — the slot/mesh/body-region record

Schema (100 % field-decoded, `docs/re_reference/def_schema.json`; retail object offsets from
decompile: `CoversBodyAreaFlags` @ +0x40, `AppearanceType` @ +0x50):

| Field | Type | Meaning (evidence: 149 vanilla entries surveyed) |
|---|---|---|
| `HideMaterials` | vector\<string\> (`u32 n` + n NUL-terminated names) | Hero **body** materials hidden while worn: boots `[L_foot,R_foot]`, trousers `[L_calf,R_calf,L_thigh,R_thigh,hips]`, shirts `[v_neck,torso_front,torso_arms,torso_back,...]`, gloves `[L_hand,R_hand]`, assassin mask adds `[neck,chin,mouth]` |
| `Graphics` | `u32 n` + n × 24-byte `CAppearanceModifierGraphics::CEntry` | **The worn meshes.** `{i32 statA, i32 statB, i32 modelId, f32 scale, f32 appear, f32 disappear}` — `modelId` = graphics.big `MBANK_ALLMESHES` TOC **id**; `appear`/`disappear` = normalized hero-stat range in which this mesh shows (hair: 3 entries switching to BALD_A/BALD_B with age; horns: 3 entries on evil). Clothing always uses `a=4 b=0 s=1 ap=0 dis=0` (one or two entries; L+R for boots/gloves) |
| `CoversBodyAreaFlags` | i32 bitmask (`EClothingCoversArea`) | Body regions occupied — see table below |
| `IsRemoveable` | bool | false for hair/beard/job-mask |
| `HeroSuit` | i32 entry index | The `HERO_SUIT_*` THING this item belongs to (outfit grouping / suit bonuses) |
| `SuitPart` | `EClothingSuitPart` | **Equipment slot**: 1=Hat/Helmet, 2=Shirt/Chest, 3=Gloves, 4=Trousers, 5=Boots (0 for hair/beard/moustache/horns) |
| `AppearanceType` | `EHeroAttachableAppearanceModifierType` | Modifier list: 0=hair/beard/moustache, 1=horns, 2=clothing |
| `Scariness`/`Attractiveness`/`Goodstrength` | float | Opinion/alignment contributions |
| `SoundRadiusMultiplier`, `VisibilityMultiplier` | float | Stealth (e.g. boots 1.1 sound) |
| `ArmourDefIndex` | i32 entry index | The `ARMOUR` entry (damage/threshold maps; e.g. 404 = ARMOUR_HERO_GENERAL_CLOTHES) |
| `OveralProtectionWeighting` | float | Share of the suit's protection |

The `Graphics` list's crc0 field tag is **`0x5C9D0167`** (from the 2026-07-24 strip workflow).

#### EClothingCoversArea bits (empirical, all 149 entries consistent)

| Bit | Region | Evidence |
|---|---|---|
| 0x001 | feet | all boots |
| 0x002 | calves | trousers = 0x6 |
| 0x004 | thighs/hips | trousers = 0x6 |
| 0x008 | torso | all shirts |
| 0x010 | scalp/hair | hair objects, wigs, helmets |
| 0x020 | arms | long-sleeve shirts = 0x28 |
| 0x040 | hands | all gloves |
| 0x080 | chin/beard | beards; NO_BEARD_OR_TASH = 0x180 |
| 0x100 | mouth/moustache | moustaches; assassin mask shirt = 0x1A8 |
| 0x200 | head (hat volume) | hats = 0x210, full helmets = 0x310, hood-up shirt = 0x238, horns = 0x200 |

Equipping removes whatever overlaps: decompiled
`CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifiersCoveringBodyArea(mask, type)` @
`0x007066A0` drops every worn modifier of that type with `covers & mask != 0` (with a moustache-bit
0x100 exemption for clothing), then re-adds survivors.

### The suit layer

`HERO_SUIT_*` are **THING** entries (3346–3379 region) with components `CTCDHeroSuit` +
`CTCInventoryItem` and a CDef listing to an unnamed **`CHeroSuitDef`** whose single field
`SuitParts` (tag `0x4F51DB41`) is `u32 n` + n × `{i32 EClothingSuitPart, i32 OBJECT entry}`:

`HERO_SUIT_OAK_VALE` -> `{1:3518 hat, 2:3451 shirt, 3:3477 gloves, 4:3426 trousers, 5:3404 boots}`.

Decompiled `CTCInventoryClothing::SetThingAsWearingClothingSuit` @ `0x005B3D7A`: fetch the thing's
`CTCHeroAttachableAppearanceModifiers` component (interface key 0x5E),
`RemoveAllAppearanceModifiers(type=2)`, then for each SuitParts object present in the inventory,
`AddAppearanceModifierWithReplace(objEntry)`.

### Engine application and draw (decompile-confirmed)

Component: **`CTCHeroAttachableAppearanceModifiers`** (string @ `0x0123BCD0`). Worn state = 3 lists
(indexed by `AppearanceType`) of `CModifierGraphic {defEntryIdx, meshId, scale}` (12-byte stride).

| Function | Addr | Role |
|---|---|---|
| `AddAppearanceModifier(long entryIdx)` | 0x00706370 / 0x00706880 | GetDef(entryIdx) -> puts modifier in list `[AppearanceType]`, dirty-flags rebuild |
| `AddAppearanceModifierWithReplace` | 0x00706950 | remove-covering + add |
| `RemoveAppearanceModifiersCoveringBodyArea` | 0x007066A0 | covers-mask eviction (see bits table) |
| `RemoveAllAppearanceModifiers(type)` | 0x00706640 | strip a whole list |
| `IsWearingAppearanceModifier` | 0x00706000 | query |
| `GetMeshesUsed` | 0x00706C90 | reports `{meshId, scale}` per worn graphic (streaming/preload) |
| `OnAppearanceDraw` | 0x00707440 | **the mesh->hero bind**: for each worn `CModifierGraphic`, constructs a `CEnginePrimitiveMorphedAnimatingMesh(meshId, ...)` passing the hero's `C3DAnimationInterpolator` / position / orientation interpolators and adds it to the hero's `CTCGraphicAppearance` via `SetPrimitiveGraphics` |
| `SetThingAsWearingClothingSuit` | 0x005B3D7A | suit equip |
| `AddClothingToThing` | 0x005B43A2 | single-item equip path |
| script API `IsWearingClothingItem` / `RemoveHeroClothing` / `MsgOnHeroHairTypeChanged(EClothingCoversArea,...)` | 0x0089B2B0 / 0x0089B390 / 0x00893C00 | quest-script hooks |

**There is no attach-bone or socket data anywhere.** The worn mesh IS a skinned mesh:
`MESH_HERO_BOWERSTONE_BOOT_L_01` (BIG type 5) embeds a 10-bone skeleton `[Scene Root,
Movement_dummy, Sub_movement_dummy, Bip01, Bip01 Pelvis, Bip01 Spine, Bip01 L Thigh, Bip01 L Calf,
Bip01 L Foot, Bip01 L Toe0]` — a **named subset of the hero Biped rig** — plus per-vertex weights
(`BLENDER_ADDON.md` §1.2). The engine animates it with the hero's own interpolators, so it deforms in
lockstep; body regions underneath are hidden per `HideMaterials` (hero body material names) rather
than clipped.

### Validation (deliverable 2)

`python tools/appearance_survey.py` against the retail install:

- **148 named OBJECTs** carry a `CAppearanceModifierDef` in their CDef listing (the 149th modifier
  entry is `NULLDEF_CAppearanceModifierDef`).
- **148/148**: every `Graphics.modelId` resolves to a real `MBANK_ALLMESHES` entry, all BIG
  **type 5** (skinned). 0 dangling IDs.
- Sample chains (item -> inv def -> appearance def -> mesh):
  - OBJECT_HERO_BOOTS (3404) -> inv 11710, amd 11712 -> 4111 `MESH_HERO_BOWERSTONE_BOOT_L_01` +
    4112 `..._R_01`
  - OBJECT_HERO_SHIRT_CHAINMAIL (3470) -> 11941/11942 -> 4145 `MESH_HERO_CHAINMAIL_SHIRT_01`
  - OBJECT_HERO_HELMET_PLATINUM (3512) -> 12086/12087 -> 4225 `MESH_HERO_PLATINUM_HELMET_01`
    (covers 0x310)
  - OBJECT_HERO_HAIR_MOHAWK_01 (3383) -> 11658/11659 -> 4263/4264/4265 (age-staged MOHAWK ->
    BALD_A -> BALD_B)

### HalsSword.fmp study (how a working equipment mod wires it)

`ExampleFMPFilesForDev/HalsSword.fmp` (weapon, a clone of OBJECT_LEGENDARY_KATANA):
- Appends 5 game.bin entries at the next free indices (14761 OBJECT + 14762–14765
  CInventoryItemDef / CObjectAugmentationsDef / CWeaponDef / CStockItemDef); the OBJECT's CDef
  listing `dataEntry` fields already point at the new indices (`ownerEntry` left stale at the donor
  5516 — engine doesn't care).
- Adds `MESH_F2_LEGENDARY_LONGSWORD_02` to graphics.big as **id 8113** = vanilla max (8112) + 1, and
  patches `OBJECT.Graphic.modelId` to 8113.
- Adds 3 `maintextures` (diffuse/normal/spec, ids 6291–6293) and 2 `text.big` entries
  (name/description; `CInventoryItemDef.ItemDescription/ItemDetails` are text ids).
- The .fmp `*LinkMetaData` banks are .NET-BinaryFormatter `NameValueCollection`s (payload-offset ->
  referenced-entry-name) used by ChocolateBox at install time to re-target entry indices; the engine
  never sees them.

### Recipe: a "custom boots" mod

Data to produce (all write paths exist in FableForge / this repo today):

1. **Worn meshes** (1 per side, or a single mesh): skinned to the hero rig subset that the region
   touches (boots: `Bip01 L/R Thigh -> Calf -> Foot -> Toe0` chains + root/dummy chain, exact bone
   names from any retail `MESH_HERO_*_BOOT_*` import). Compose as a **type-5** graphics.big entry
   with bone blocks + animated blocks + per-vertex weights.
   *Gap CLOSED (2026-07-20): `mesh_rw.compose_mesh(..., skeleton=mesh_rw.clone_skeleton(donor_pay,
   5))` composes skinned type-5 payloads — skin input is per-vertex `[(bone_name_or_slot, weight)]`;
   validated by recompose oracles (goblin/seagull/boots) + a Blender end-to-end round trip of a NEW
   re-skinned boot injected as id 8115 (docs/formats/MESH_COMPOSE.md §1b/§3/§5).* Add with
   `big_write.rebuild(adds=...)` as id = maxId+1 (8113+ on vanilla).
2. **Ground-prop mesh** (optional but standard): a small static type-1 mesh ("folded boots"); or
   reuse retail id 129.
3. **game.bin entries** (append; `forge::bin addEntry` / `.fmp` route):
   - unnamed `CAppearanceModifierDef`: `HideMaterials=[L_foot,R_foot]`,
     `Graphics=[{4,0,newMeshIdL,1.0,0,0},{4,0,newMeshIdR,1.0,0,0}]`, `CoversBodyAreaFlags=0x1`,
     `IsRemoveable=1`, `HeroSuit=<a HERO_SUIT or 0>`, `SuitPart=5`, `AppearanceType=2`,
     `ArmourDefIndex=404` (or custom ARMOUR entry), `OveralProtectionWeighting≈0.05`, multipliers 1.0.
   - unnamed `CInventoryItemDef`: clone OBJECT_HERO_BOOTS' (entry 11710), point
     `ItemDescription`/`ItemDetails` at new text.big ids, `InventoryType=18`,
     `UIInventoryCategory=4`, `ItemToSelectUponRemoval=3425` (OBJECT_HERO_NO_BOOTS).
   - unnamed `CStockItemDef`: clone 11711 (pricing/shops).
   - named `OBJECT_MY_BOOTS`: clone OBJECT_HERO_BOOTS' payload; rewrite the CDef-listing `dataEntry`
     of the 3 self-owned records to the new entries (set `ownerEntry` = own index; keep the
     template-inherited records verbatim), set `Graphic.modelId` to the ground-prop mesh.
   *Names go through names.bin (`forge::bin` handles appends + CRCs).*
4. **Textures** in graphics.big `GBANK_MAIN_PC` (`tools/texture_build.py`), referenced by the mesh's
   material `matTexId`s.
5. **Text**: 2 text.big entries for name/description (CRC-seed-0 keyed).
6. **Getting it in-game**: add to a shop via `CStockItemDef`/stock lists, a chest
   (`CChestDef`/reward family), TNG placement, or FSE-Lua `GiveHeroItem`-style call. Equipping goes
   through the normal clothing inventory (SuitPart 5 slot) with zero code changes — the whole chain
   is data-driven.

Minimal-risk variant (proven-by-construction): **retarget an existing item** — field-merge only
`Graphics.modelId` in its CAppearanceModifierDef to a new mesh id, everything else untouched.

### Hero SKIN (face/hands) is not an appearance modifier

The hero's own skin is NOT an appearance modifier: the adult body is **`MESH_HERO`** (graphics.big id
4299, type 5) whose materials bind `HERO_SKIN_FACE` (1250), `HERO_SKIN_ARMS` (1248 — includes the
HANDS), `HERO_SKIN_TORSO/BACK/LEGS/MOUTH` + `HERO_BUMP_SKIN_*`. Age/alignment/will/muscle looks are
**composited** onto those base sheets from `COMP_HERO_*` overlays per `CHeroMorphDef.TextureMorphs`
(game.bin 10535/11246) — the mesh's texture ids never change. `GRAPHIC_HERO_FACE` (45) is referenced
by no mesh (stage-1 smoke-test tinted it: no in-game effect). Full record layout, stat table and the
corrected stage-2 green-tint staging: [Hero skin and morph texture chain](#hero-skin-and-morph-texture-chain).

---

## Creature appearance modifiers and the clothing strip

*Originally `APPEARANCE_STRIP_FINDINGS.md`, 2026-07-24. RE'd the appearance/clothing path via the
Claude Workflow loop (`appearance-clothing-re`: 5 decode agents → synthesis → adversarial verify,
verdict **CONFIRMED 7/0/3**) to find how to show a custom creature's bare custom body instead of the
trader's clothing. The workflow produced a correct-but-misapplied answer; an empirical scan of retail
game.bin corrected the target. Both are recorded because the correction is the load-bearing fact.*

### What the workflow got RIGHT (verified, keep)

- `CCreatureDef` has an `InitialAppearanceModifiers` field: crc0 tag **`0x8F20181F`** (LE bytes
  `1F 18 20 8F`), on-disk `[u32 tag][u32 count][count × i32 entryId]`, each id → a
  `CAppearanceModifierDef`. Serialized by `CCreatureDef::Transfer` @ `0x00675E20` at member `+0x54`
  (the vector<int> immediately after `InitialActions` @ `+0x48`; helper pair `0xFFDF0C55`/`0xFFDF0C87`).
  Triple-corroborated (disasm structure + `ghidra_out/def_schema.json:18688` type `Vector_J` +
  community `chocolatebox_def.xml:5107` Control id `1F18208F`).
- Field tags are stored **inline** as LE u32 immediately followed by the value (same premise the
  shipping `03_inplace_modelid.cpp` uses to find `Graphic` = `0x2E6B63C8`).
- A `CAppearanceModifierDef` carries a `Graphics` list: crc0 tag `0x5C9D0167`, `u32 n` + `n × 24-byte
  CEntry {i32 statA, i32 statB, i32 modelId, f32 scale, f32 appear, f32 disappear}`.

### The EMPIRICAL CORRECTION (retail game.bin scan — supersedes the strip recipe's target)

The workflow assumed the trader's clothing comes from `InitialAppearanceModifiers`. **It does not.**
Scanning all 14,761 retail entries (`work/appearance_re/*.cpp`):
- **`InitialAppearanceModifiers` lives on the `CCreatureDef` *sub-def*, not the `CREATURE` wrapper.**
  A `CREATURE` entry (e.g. `CREATURE_TRADER_01`, def type `CREATURE`) is a wrapper that references a
  component `CCreatureDef` (TRADER_01 → index **10744** at CREATURE-payload offset **297**; also
  `CAppearanceDef` **10745** @21, `CEnemyDef` **10746** @189). The tag `0x8F20181F` appears in **330
  `CCreatureDef` entries and ZERO `CREATURE` entries** — so the earlier attempt to scan the CREATURE
  payload for it correctly found nothing.
- **TRADER_01's `CCreatureDef`[10744] has `InitialAppearanceModifiers` present @ offset 461 but
  `count = 0`.** The trader ships with an EMPTY modifier list. Therefore clothing is NOT layered via
  this field for traders — emptying it is a no-op.
- ⇒ **`count = 0` is a proven engine-valid, retail-shipped state** — this resolves the workflow's one
  residual risk (whether the load helper tolerates an empty vector): retail itself ships it.
- **The trader's real appearance is `CAppearanceDef`[10745]** — an **18,103-byte** structure
  containing its own `Graphic` tag and ~20 graphics.big mesh-id-range u32s (the composited
  body/clothing meshes). This is the actual lever for a bare-body result, and it needs its own
  dedicated RE pass (structure not yet mapped) before any safe edit.

### Consequence for the custom-NPC work

- The `Graphic.modelId` repoint (Track A/B) sets the **base body** mesh. Whether the custom body
  shows bare or with layered meshes depends on `CAppearanceDef`[10745], NOT
  `InitialAppearanceModifiers` (which is already empty). **Do not implement an
  InitialAppearanceModifiers strip for traders — it would do nothing.**
- **Next step is observational, not speculative:** deploy Track B, spawn `CREATURE_MESHY_HUNTER`, and
  SEE what renders. If the custom body is obscured by `CAppearanceDef`[10745] meshes, RE that 18 KB
  `CAppearanceDef` structure next (a focused workflow on `CAppearanceDef::Transfer` `0x0046a174`
  region + the on-disk 10745 layout) to learn how to null/repoint its mesh list. If it already shows
  the bare custom body, no strip is needed.
- For a creature that DOES use `InitialAppearanceModifiers` (330 CCreatureDefs do, count>0), the
  workflow's strip recipe is valid — edit the **`CCreatureDef` sub-def** (not the CREATURE wrapper):
  scan its payload for `1F 18 20 8F`, then either set count=0 + drop entry bytes (via
  `setEntryData`, safe — retail ships count=0) or size-preserving repoint each id to
  `NULLDEF_CAppearanceModifierDef`.

*Artifacts: `work/appearance_re/` (bundles, `mechanism_md.txt`, `verify.txt`, and the scan tools
`scan_iam.cpp`/`where_iam.cpp`/`check_component.cpp`/`dump_appdef.cpp`). Full workflow synthesis in
the task output journal.*

---

## Hero skin and morph texture chain

*Originally `HERO_MORPH.md`, 2026-07-20 (stage-2 smoke-test agent). Empirical, NO Ghidra (lock was
taken). Evidence: MESH_HERO material list read straight from graphics.big (`tools/parse_mesh.py`
descriptor), CHeroMorphDef decoded from game.bin (`forge defs decode`), full texture-name census of
textures.big. Motivated by the stage-1 smoke-test miss: tinting `GRAPHIC_HERO_FACE` green produced NO
in-game change on an adult hero — wrong target.*

### What the live hero actually samples

- The playable adult hero's body mesh is **`MESH_HERO`** (graphics.big `MBANK_ALLMESHES` id **4299**,
  type 5 skinned). Its 12-material list IS the set of sheets the hero samples:

  | tex id | name | role |
  |---|---|---|
  | 1250 | `HERO_SKIN_FACE` | **the face** (512², DXT1) |
  | 1248 | `HERO_SKIN_ARMS` | **arms + HANDS** (512², DXT1) |
  | 1252 | `HERO_SKIN_TORSO` | chest/front |
  | 1249 | `HERO_SKIN_BACK` | back |
  | 1251 | `HERO_SKIN_LEGS` | legs/feet |
  | 1253 | `HERO_SKIN_MOUTH` | mouth interior (256²) |
  | 1233–1237 | `HERO_BUMP_SKIN_{FACE,TORSO,LEGS,BACK,ARMS}` | bump layers |
  | 1045 | `HERO_EYESHADOW` (dev-path name) | eye shadow card |

  (Eyes are a separate static mesh `MESH_HERO_EYE` id 120 → `HERO_EYE_NORMAL` 1046.)
- **`GRAPHIC_HERO_FACE` (id 45) is referenced by NO mesh in graphics.big** (exhaustive descriptor scan
  of all 3295 compiled meshes; ditto `GRAPHIC_HERO_MOUSTACHE` 46). Whatever it is (GUI/dev leftover),
  tinting it cannot touch the rendered hero — this was the stage-1 failure.
- Other hero-aged meshes: `MESH_YOUNGHERO_02` (4300) → `YOUNGHERO_SKIN_FACE` 792 /
  `YOUNGHERO_SKIN_TORSO` 794; `MESH_TEEN_HERO` (4301) → same `HERO_SKIN_*` sheets but
  `HERO_TEEN_FACE` 1254 instead of 1250. A mid-game **adult** hero uses `MESH_HERO` + `HERO_SKIN_FACE`.

### The morph side: CHeroMorphDef.TextureMorphs

game.bin has 2 populated `CHeroMorphDef` entries — **10535** (4217 B) and **11246** (3857 B) — plus
null/stub entries (0, 10543). Both populated entries carry **identical** `TextureMorphs` content (156
records). Fields (schema `docs/re_reference/def_schema.json`): `TextureMorphs`, `SkeletalMorphs`,
`ParticleMorphs`, `IdleParticleMorphs`.

`CTextureMorphs` blob layout (decoded here, self-consistent for all 156×2 records; trailing dword is
a constant 2, terminator `bb6cab01` = crc0 tag):

```
u32 count
count × 24 bytes:
    u32 statA        # hero stat driving the morph (statA == statB always)
    u32 statB
    u32 layer        # compositing layer/slot (0,2,3,4,5,7,8 observed)
    u32 texBase      # textures.big GBANK_MAIN_PC id of the RETAIL base sheet
    u32 texOverlay   # COMP_HERO_* overlay composited onto it
    u32 flag = 2
```

So the hero-morph system does NOT swap the mesh's texture id — it **composites `COMP_HERO_*` overlay
sheets onto the base `HERO_SKIN_*` sheets** keyed by hero stats. The base sheet named in the mesh
material is always the compositing input. Observed stat → overlay families (both defs identical):

| stat | overlays (face/arms rows shown) | reading |
|---|---|---|
| 3 | FACE→`COMP_HERO_SKIN_FACE_STUBBLE` | stubble (time since shave?) |
| 4 | every hair/beard/tash sheet → `*_OLD` | hair greying with age |
| 5 | `HERO_SKIN_*`→`*_OLD`, bumps→`BUMP_SKIN_OLD_*`, eye→`EYE_OLD`, also layer-8 `*_STUBBLE_OLD`/`HAIR_OLD_*` | age (skin) |
| 6 | layer 0: →`*_ANTITAN` + `BUMP_SKIN_LIGHT_*`; layer 4: →`*_EVIL` + `BUMP_EVIL_*` + `EYE_EVIL` | alignment (def.xml: Evil=6) |
| 7 | layer 0: →`*_TAN`; layer 4: hair/beard→`*_BLONDE`, skin→`HAIR_BLONDE_*`/`STUBBLE_BLONDE` | def.xml: Good=7 (sun-tan/fair) |
| 8 | →`COMP_HERO_MUSCLE_*` + `BUMP_MUSCLE_*` | strength/physique |
| 9 | TORSO→`*_MUSCLE_STOMACH` | fatness/stomach |
| 10 | →`COMP_HERO_WILL_*` + `BUMP_WILL_*` + `EYE_WILL` | will glow lines (def.xml: Will=10) |
| 11 | →`COMP_HERO_HAIR_*_1A/1B` + `STUBBLE` | body hair |

(def.xml comments "Age=4, Evil=6, Good=7, Will=10" corroborate; 4 vs 5 split = hair-age vs skin-age,
empirical.) Scars/bruises/tattoos (`COMP_HERO_SKIN_*_SCAR/BRUISE/TATTOO*`) are NOT in TextureMorphs —
they come in via other systems (tattoo cards are OBJECTs; bruises likely runtime).

`SkeletalMorphs` (6 records) references graphics.big ids 0x57f00+ region (morph-target meshes);
`ParticleMorphs` references effects by id (evil flies etc. at stat thresholds with float ranges). Not
needed for the texture answer; layouts only partially examined.

### Consequence for texture modding the hero

To recolor the adult hero's face/hands, edit **`HERO_SKIN_FACE` / `HERO_SKIN_ARMS`** (GBANK_MAIN_PC
1250/1248) — and, because a saved hero may have any stat mix composited on top, also the
`COMP_HERO_*` FACE/ARMS/MOUTH overlays (old, evil, tan, antitan, will, stubble, muscle, body-hair).
Tinting only the base is probably visible through most composites (overlays blend), but tinting all
23 face/hand-related sheets makes the smoke test save-independent.

### Stage-2 smoke test (staged, NOT deployed)

`work/smoke_test/stage2/build_stage2.py` (re-runnable, self-validating):
- Base: `work/smoke_test/backups/.../textures.big` — SHA256 checked against the deploy ledger
  (pristine retail).
- Tints 23 sheets bright green (65 % blend, same strength as stage 1): the 3 base sheets
  (FACE/ARMS/MOUTH) + all 20 face/arms/mouth morph overlays listed above. Each entry: decode mip-0 →
  tint → re-encode with the entry's own format (DXT1 `0x1f` / DXT3 `0x20`), mip count, real dims,
  TransparencyType; Info patched in place; ONE `big_write.rebuild` for all 23.
- Output: `work/smoke_test/stage2/data/graphics/pc/textures.big` (533,502,683 B). Validation ALL
  PASS: entry census identical to retail, all 23 decode back green-dominant (mean G exceeds R and B
  by >40; PSNR vs intended tint 33.9–47.1 dB), and `GRAPHIC_HERO_FACE` is byte-identical to RETAIL
  again (stage-1 tint corrected away).
- Expected in-game: green face, hands and (if visible) arms/legs-adjacent skin on ANY
  save/age/alignment; mouth interior green when talking. Deploy by copying over
  `data\graphics\pc\textures.big` (game closed, back up first — stage-1 `deploy.ps1`/`revert.ps1`
  pattern applies; stage2 ships no deploy script by design).

### Custom overlay AUTHORING (tools/hero_overlay.py, 2026-07-20)

*Empirical follow-up agent, same day; Ghidra lock still taken. Adds the write path for the system
decoded above: custom tattoos / war-paint / skin variants as NEW COMP-style overlays + TextureMorphs
records.*

#### What a valid overlay looks like (retail census, 301 COMP_HERO_*/HERO_SKIN_* entries)

All 235 *blending* overlays (skin/hair/beard/muscle/will/stubble/tattoo/scar/bruise) are **DXT3 with
`TransparencyType=1`** — the 4-bit explicit alpha IS the compositing mask (RGB = overlay colour;
background is alpha-0, RGB don't-care). The remaining 66 are DXT1 `TransparencyType=0`: the
TAN/ANTITAN full-sheet recolours (layer 0) and the eye/mouth variants — i.e. opaque whole-sheet
replacements-or-tints, not masked glyphs. Overlay dims always EQUAL the base sheet's dims and UV
layout (the mesh UVs sample both):

| region | base sheet | id | overlay format |
|---|---|---|---|
| face  | `HERO_SKIN_FACE`  | 1250 | 512×512 DXT3 tt=1, 8 mips |
| arms (incl. hands) | `HERO_SKIN_ARMS` | 1248 | 512×512 DXT3 tt=1, 8 mips |
| torso | `HERO_SKIN_TORSO` | 1252 | 512×512 DXT3 tt=1, 8 mips |
| back  | `HERO_SKIN_BACK`  | 1249 | 512×512 DXT3 tt=1, 8 mips |
| legs  | `HERO_SKIN_LEGS`  | 1251 | 512×512 DXT3 tt=1, 8 mips |
| mouth | `HERO_SKIN_MOUTH` | 1253 | 256×256 (retail: DXT1 tt=0, 7 mips) |

Every GBANK_MAIN_PC TOC record carries the same `crc` **0x7C8D23C4** (bank constant, mimicked for new
entries); deps are dev-path TGA strings (loader doesn't need them; new entries ship none).

#### The authoring tool

`python tools/hero_overlay.py stage glyph.png --region face --stat always` (also `spec` to print the
tables). Given a transparent PNG it stages, without touching the install:

1. **NEW textures.big entry** (default `COMP_HERO_SKIN_<REGION>_CUSTOM`, id = bank max + 1 → **6291**
   on retail): PNG → DXT3/tt=1 with the region's retail mip count via `texture_build.build_entry`,
   appended with `big_write.rebuild(adds=…)`.
2. **Modified `data\CompiledDefs\game.bin`**: appends one 24-byte record `{stat, stat, layer, baseId,
   newId, 2}` per trigger stat to `TextureMorphs` (count bumped, tail preserved) in **BOTH** populated
   CHeroMorphDef entries 10535 and 11246 — the record content is identical in retail and their owner
   is still unresolved (open question 2), so editing both is the safe default (`--defs` to restrict).
   The container rewrite is a Python mirror of FableForge `forge::bin::File::save` (main table +
   names refs preserved VERBATIM → names.bin unchanged; payload chunks re-cut ≤32 KiB, zlib): retail
   round-trips semantically identical (14761/14761 payloads).
3. Validation gates (all must pass before the stage is written/declared): new entry decodes back
   (format/tt/mips + PSNR), all other texture entries byte-identical, all other def entries
   byte-identical, edited defs re-parse with the appended records and preserved tail, and `forge defs
   decode` reports **leftover 0** on both entries.

#### "Always-on" (what a tattoo mod wants) — the honest answer

TextureMorphs records carry **no threshold/range floats** (contrast ParticleMorphs, whose records end
in rank-to-appear/disappear floats, e.g. evil flies at 0.6..1.1) — so overlay opacity is driven
**continuously by the stat's normalized rank** at composite time (matches observed gameplay:
stubble/tan/will-lines fade in gradually). Consequences:

- No single record can be proven always-on-at-full-strength from data alone; retail uses stats 3–11
  only, and none is guaranteed maxed on every save.
- `--stat always` therefore emits the SAME overlay under **both stat 6 (evil) and stat 7 (good)** —
  the two halves of the alignment axis. If the engine ranks the sides complementarily (rank₆+rank₇ ≈
  1) the pair composites at ~full strength on any save; if ranks are `max(0, ±alignment)` a perfectly
  neutral hero shows it faintly. Which of the two is true needs the compositor decompile (open
  questions 3/4) or a live A/B.
- For a GUARANTEED always-on mark, `--bake-base` additionally composites the glyph into the base
  `HERO_SKIN_*` sheet itself (stage-2-proven visible on any save; overlays then blend on top of it).

#### Worked example (staged 2026-07-20, NOT deployed)

`work/hero_overlay/build_example.py` (re-runnable): bold black cross on the face-sheet forehead (face
front centres on x=256; brow line ≈ y 140) → `work/hero_overlay/stage/` with `textures.big` (new
entry `COMP_HERO_SKIN_FACE_FORGE_CROSS`, id 6291) + `game.bin` (10535/11246 records 156→158:
`{6,6,4,1250,6291,2}`, `{7,7,4,1250,6291,2}` — layer 4 = the retail skin blend layer of the EVIL
overlays) + an unmodified `names.bin` copy so the stage is a self-contained defs root. ALL validation
gates green (overlay PSNR 63.06 dB; `forge defs decode` leftover 0 on both; manifest with SHA256s:
`stage/hero_overlay_manifest.json`; decode-back artifacts in `work/hero_overlay/validate/`). Base
container: the ledgered pristine textures.big backup — note the stage does NOT include the stage-2
green tint. Deploy = manual copy of the two staged files (game closed, back up first).

---

## Hero mesh deform bug (Banjo)

*Originally `HERO_MESH_DEFORM_FIX.md`. From engine decomp analysis 2026-07-21
(`hero_mesh_decomp.txt`, 96 fns). The byte format is NOT the bug.*

### Ruled out (byte-identical to EgoCore's proven meshes — do NOT touch)

- **Packed-normal bit layout** — engine never CPU-decodes normals; it builds a D3D9 vertex
  declaration (`CVertexShaderInput{Static,Animated}::Initialise` 0x00a90560/0x00a90630) and the GPU
  decodes the packed dword. Engine picks packed normal (typeCode 0xc) exactly when InitFlags&4
  (Banjo's 0x14 sets it). `mesh_rw.pack_normal` == EgoCore PackNormal byte-for-byte. Match.
- **Weight/bone-index bytes** — 4 joint bytes + 4 weight bytes after position, joint =
  paletteIndex*3, weights /255 renormalized. Matches `mesh_rw._skin_bytes`. Match.
- **Palette local-vs-global** — undecidable from decomp BUT can't be the cause: the build **clones
  the donor hero skeleton byte-exact** (`build_banjo_animated.py:292/298`, indices re-emitted
  verbatim), so the local->global map is identical to retail regardless.
- Engine only validates that the swapped mesh is a **type-5 entry** (`ReplaceMeshByIndex`
  @0x007a69f0 + CreatePPhysicsMeshInfo on graphic[0xd]==5). Nothing else is checked.

### ROOT CAUSE (authoring-space weighting error in `build_banjo_animated.py`)

1. **(high) Head-only + single-uniform-scale fit ignores bind orientation.** The Banjo geo is fit
   using only bone HEAD positions + one global scale (`:79-151`), then ARMATURE_AUTO bone-heat
   weights it in that rest pose (`:202`). Each bone's bind *orientation* and per-bone scale are never
   consulted, so vertices weight to bones whose bind-frame axes don't match Banjo's local geometry
   -> any animated rotation twists/collapses the mesh even though the file round-trips perfectly.
2. **(high) WEAPON_FOCUS/WEAPON_SCABBARD deform disabled before bone-heat** (`:184-193`) -> no Banjo
   vertices weight to the weapon bone -> **"sword doesn't track the hand"** (the attached weapon
   expects deform on a zero-weight bone).

### Fix (edit ONLY the build script, never mesh_rw.py)

1. Remove `WEAPON_FOCUS`, `WEAPON_SCABBARD` from the NONDEFORM tuple (`:184-193`); re-run; confirm via
   the bone_use histogram (`:242-252`) that WEAPON_FOCUS now carries weight.
2. Replace the head-only/uniform-scale fit (`:79-151`) with a **bind-aware fit**: place/orient
   geometry using each dominant bone's FULL rest matrix (head AND tail direction), not just
   `b.head_local`; drive ARMATURE_AUTO from a mesh whose limbs align to bone axes.
3. Harden the ANATOMY spot-check (`:254-263`) into a hard assert (upper-outer verts -> ARM bones,
   lower verts -> LEG bones above a threshold) so an orientation-scrambled fit fails the build
   instead of shipping.

### Validation (no in-game run)

One-bone skin test: build a trivial mesh 100% weighted to one non-root deform bone (e.g. `Bip01 R
Hand`), compose with the cloned hero skel, apply the base pose in a Python harness
(`CalcLocalBoneTransformationRelative` semantics: base TRS from C3DMesh2+0xa0 stride 0x30), and
confirm the mesh follows THAT bone. Validates the slot mapping end-to-end before a full run.

### Still open

Is the in-game symptom a whole-body scramble (-> fit/orientation, fix #2) or specifically the weapon
(-> WEAPON_FOCUS, fix #1)? The exact symptom picks the primary fix. Draw-time palette convention
(CalcMeshTransformations / CVertexBufferWin32 skin path) not in this closure; only matters if you
ever author a NON-cloned skeleton.

---

## Open questions

### Equipment / clothing

- `CAppearanceModifierGraphics::CEntry` ints A/B: A=4 everywhere; B=0 (clothing), 16 (hair), 6
  (horns). def.xml comments claim the values are the controlling morph stat ("Age=4, Evil=6, Good=7,
  Will=10") for the appear/disappear range (matches hair=age, horns=evil); which of A/B is the
  selector — and what 16 means — needs the `CAppearanceModifierGraphics` Transfer/refresh decompile
  (`FUN_00708600`-area rebuild path).
- The fixed 21-slot `{u32 crc, u8}` tail of `CThingComponentSet` (same CRC set on every THING/OBJECT;
  flags vary). Not crc0 of the CTC*/C*Def class names — unidentified hash input.
- Precise 0x2 vs 0x4 split (calves vs thighs+hips) inferred from HideMaterials pairing only.
- `SpecificCoversBodyAreaFlags` (def.xml control `A0E2EA95`) — present in the schema of other def
  types, unused by the 149 vanilla modifiers.
- Villager/NPC clothing (CREATURE `CAppearanceDef` + `CTCDVillagerLook`) not covered by the
  equipment section; hero-equipment scope only. (The trader case is in
  [Creature appearance modifiers](#creature-appearance-modifiers-and-the-clothing-strip).)
- Live (in-game) test of a composed skinned mesh: composition itself is now proven offline
  (grammar-exact vs retail, Blender round trip — docs/formats/MESH_COMPOSE.md §1b), but a Fable.exe
  load of a composed type-5 entry has not been observed yet.

### Skin / morph (Ghidra follow-ups; lock was taken during the 2026-07-20 runs)

1. What consumes `GRAPHIC_HERO_FACE`/`GRAPHIC_HERO_MOUSTACHE` (MAIN-bank ids 45/46, no mesh
   references)? Ghidra: xref the name/id in the texture-manager or hero-morph compositor.
2. Which CREATURE/def binds CHeroMorphDef 10535 vs 11246 (identical TextureMorphs, different
   SkeletalMorphs sizes — hero vs. rival-hero/apprentice?). Needs the CDef-listing owner walk or Ghidra.
3. `layer` field semantics (compositing order/slot?) and the stat-index → enum table
   (`CHeroMorph::Transfer`, rebuild path around the `FUN_00708600`-area noted above).
4. Whether overlays REPLACE or alpha-BLEND per layer (moot for the smoke test — every candidate is
   green). Evidence leans BLEND for the DXT3/tt=1 population (soft-alpha masks, no-threshold records,
   gradual in-game fade-ins); the DXT1/tt=0 TAN/ANTITAN layer-0 sheets are the likely REPLACE/tint
   class.
5. Alignment rank model: does `--stat always` (evil+good pair) composite at full strength on a
   neutral hero (rank₆+rank₇ ≈ 1) or fade to nothing (`max(0, ±alignment)`)? `CHeroMorph` rank
   derivation in the compositor, or a live A/B with a neutral save, decides. Also: engine acceptance
   of a re-chunked game.bin (forge::bin writer output) has not been live-tested.
6. Layer-field ordering for authored overlays: the worked example reuses retail layer 4; whether a
   tattoo should sit on a higher layer to render over tan/evil states is unknown until q3 lands.

### Creature appearance

- `CAppearanceDef` (e.g. entry 10745, 18,103 B) structure is not yet mapped; `CAppearanceDef::Transfer`
  is in the `0x0046a174` region.

---

## Superseded

- **Stage-1 smoke test target (`GRAPHIC_HERO_FACE`, id 45)** — superseded 2026-07-20: no mesh in
  graphics.big references it; the adult hero samples `HERO_SKIN_FACE` 1250 / `HERO_SKIN_ARMS` 1248 via
  `MESH_HERO` 4299. Stage 2 reverted the id-45 tint to retail bytes.
- **Clothing-strip recipe targeting the trader's `InitialAppearanceModifiers`** — superseded
  2026-07-24: TRADER_01's `CCreatureDef`[10744] ships `count = 0`; emptying it is a no-op. The lever is
  `CAppearanceDef`[10745]. The recipe remains valid only for the 330 `CCreatureDef`s with count>0, and
  must edit the sub-def, not the `CREATURE` wrapper.
- **Skinned type-5 composition as an open gap (custom-boots recipe step 1)** — closed 2026-07-20 by
  `mesh_rw.compose_mesh(..., skeleton=clone_skeleton(...))`.
- **Byte-format hypotheses for the Banjo deform bug** (packed normals, weight bytes, palette
  local-vs-global) — ruled out 2026-07-21; root cause is authoring-space weighting.

# Fable TLC appearance / clothing / equipment system

*2026-07-20. How equipping an item puts a mesh on the hero: the complete
slot -> mesh -> body-region model, decompile-confirmed, plus the recipe for a
custom-equipment ("new boots") mod. Evidence: `tools/appearance_survey.py`
(reproduces every claim against the retail install), decompiles in
`ghidra_out/appearance_decomp.c`, HalsSword.fmp study, ChocolateBox
`data/def.xml` (community FableExplorer schema, corroborating).*

---

## 0. TL;DR

- A wearable item is a **game.bin `OBJECT` entry** whose **CDef listing**
  (a table in the entry's payload *prefix*) points at an unnamed
  **`CAppearanceModifierDef`** entry (plus `CInventoryItemDef`, `CStockItemDef`,
  and template-inherited physics/targeting/sound defs).
- The `CAppearanceModifierDef` carries the whole worn-appearance contract:
  **`Graphics[]` -> graphics.big model IDs** (the worn meshes),
  **`SuitPart`** (equipment slot), **`CoversBodyAreaFlags`** (body-region mask),
  **`HideMaterials`** (hero body materials to hide), armour link, align stats.
- Worn meshes are **BIG type-5 skinned meshes** whose skeleton is a
  named subset of the hero Biped rig; the engine draws each as a
  `CEnginePrimitiveMorphedAnimatingMesh` sharing the hero's animation
  interpolators (decompile-confirmed) — they deform with the hero.
- Validation: **148/148** vanilla OBJECTs with an appearance modifier resolve
  every `Graphics` model ID to a real `MBANK_ALLMESHES` entry. 0 failures.

---

## 1. game.bin entry anatomy: the CDef listing (new format finding)

Every "top-type" entry (OBJECT / THING / CREATURE / ...) starts with a
**CDef listing** instead of the bare 5-byte prefix:

```
01 00 01                          base prefix
u16  cdefCount
cdefCount * 12 bytes:
    u32  classKey     = crc0(C*Def class name)      e.g. 83AD7C03 = "CInventoryItemDef"
    i32  dataEntry    = game.bin entry index of the component-def data
    i32  ownerEntry   = entry index of the OBJECT that owns/defines that data
```

`crc0` = CRC-32 poly `0xEDB88320`, **seed 0, no final xor** — the same hash as
the field tags (`crc0("HideMaterials") = 0x7A59BE69`; def.xml prints these
byte-swapped). Records are sorted by classKey.

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

So component data is **shared by inheritance**: template-owned defs are reused
across items; item-specific defs (inventory, stock, appearance) are unnamed
entries owned by the item. The engine resolves by `dataEntry`
(`CDefinitionManager::GetDef(entryIdx)` + `GetSubDef<T>` in every decompiled
consumer); HalsSword ships stale `ownerEntry` values and still works.

The OBJECT's own tagged fields also matter for equipment:
- `Components` (`CThingComponentSet`): `u32 count` + per component
  `{u32 names.bin-offset of the CTC* component class name, u32 param, u8}` +
  a fixed 21-slot `{u32 crc, u8 flag}` table (undecoded, identical CRC set on
  every object). Clothing items list `CTCInventoryItem`,
  `CTCGraphicAppearance(StaticMesh)`, `CTCStockItem`, etc.
- `Graphic` (`CEngineGraphic`, 17 bytes `{u32 kind=5, u32 modelId, u32 0,
  f32 scale=1.0, u8}`): the **world/ground prop** mesh — for
  OBJECT_HERO_BOOTS modelId 129 = `MESH_HERO_FOLDED_BOOTS_STANDARD`
  (BIG type-1 static). For weapons this is also the held mesh
  (katana -> 7942 `MESH_KATANA_LEGENDARY_01`).

## 2. CAppearanceModifierDef — the slot/mesh/body-region record

Schema (100 % field-decoded, `docs/re_reference/def_schema.json`; retail object
offsets from decompile: `CoversBodyAreaFlags` @ +0x40, `AppearanceType` @ +0x50):

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

### EClothingCoversArea bits (empirical, all 149 entries consistent)

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
`CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifiersCoveringBodyArea
(mask, type)` @ `0x007066A0` drops every worn modifier of that type with
`covers & mask != 0` (with a moustache-bit 0x100 exemption for clothing), then
re-adds survivors.

## 3. The suit layer

`HERO_SUIT_*` are **THING** entries (3346–3379 region) with components
`CTCDHeroSuit` + `CTCInventoryItem` and a CDef listing to an unnamed
**`CHeroSuitDef`** whose single field `SuitParts` (tag `0x4F51DB41`) is
`u32 n` + n × `{i32 EClothingSuitPart, i32 OBJECT entry}`:

`HERO_SUIT_OAK_VALE` -> `{1:3518 hat, 2:3451 shirt, 3:3477 gloves,
4:3426 trousers, 5:3404 boots}`.

Decompiled `CTCInventoryClothing::SetThingAsWearingClothingSuit` @ `0x005B3D7A`:
fetch the thing's `CTCHeroAttachableAppearanceModifiers` component (interface
key 0x5E), `RemoveAllAppearanceModifiers(type=2)`, then for each SuitParts
object present in the inventory, `AddAppearanceModifierWithReplace(objEntry)`.

## 4. Engine application & draw (decompile-confirmed)

Component: **`CTCHeroAttachableAppearanceModifiers`** (string @ `0x0123BCD0`).
Worn state = 3 lists (indexed by `AppearanceType`) of
`CModifierGraphic {defEntryIdx, meshId, scale}` (12-byte stride).

| Function | Addr | Role |
|---|---|---|
| `AddAppearanceModifier(long entryIdx)` | 0x00706370 / 0x00706880 | GetDef(entryIdx) -> puts modifier in list `[AppearanceType]`, dirty-flags rebuild |
| `AddAppearanceModifierWithReplace` | 0x00706950 | remove-covering + add |
| `RemoveAppearanceModifiersCoveringBodyArea` | 0x007066A0 | covers-mask eviction (§2) |
| `RemoveAllAppearanceModifiers(type)` | 0x00706640 | strip a whole list |
| `IsWearingAppearanceModifier` | 0x00706000 | query |
| `GetMeshesUsed` | 0x00706C90 | reports `{meshId, scale}` per worn graphic (streaming/preload) |
| `OnAppearanceDraw` | 0x00707440 | **the mesh->hero bind**: for each worn `CModifierGraphic`, constructs a `CEnginePrimitiveMorphedAnimatingMesh(meshId, ...)` passing the hero's `C3DAnimationInterpolator` / position / orientation interpolators and adds it to the hero's `CTCGraphicAppearance` via `SetPrimitiveGraphics` |
| `SetThingAsWearingClothingSuit` | 0x005B3D7A | suit equip (§3) |
| `AddClothingToThing` | 0x005B43A2 | single-item equip path |
| script API `IsWearingClothingItem` / `RemoveHeroClothing` / `MsgOnHeroHairTypeChanged(EClothingCoversArea,...)` | 0x0089B2B0 / 0x0089B390 / 0x00893C00 | quest-script hooks |

**There is no attach-bone or socket data anywhere.** The worn mesh IS a skinned
mesh: `MESH_HERO_BOWERSTONE_BOOT_L_01` (BIG type 5) embeds a 10-bone skeleton
`[Scene Root, Movement_dummy, Sub_movement_dummy, Bip01, Bip01 Pelvis,
Bip01 Spine, Bip01 L Thigh, Bip01 L Calf, Bip01 L Foot, Bip01 L Toe0]` — a
**named subset of the hero Biped rig** — plus per-vertex weights
(`BLENDER_ADDON.md` §1.2). The engine animates it with the hero's own
interpolators, so it deforms in lockstep; body regions underneath are hidden
per `HideMaterials` (hero body material names) rather than clipped.

## 5. Validation (deliverable 2)

`python tools/appearance_survey.py` against the retail install:

- **148 named OBJECTs** carry a `CAppearanceModifierDef` in their CDef listing
  (the 149th modifier entry is `NULLDEF_CAppearanceModifierDef`).
- **148/148**: every `Graphics.modelId` resolves to a real `MBANK_ALLMESHES`
  entry, all BIG **type 5** (skinned). 0 dangling IDs.
- Sample chains (item -> inv def -> appearance def -> mesh):
  - OBJECT_HERO_BOOTS (3404) -> inv 11710, amd 11712 -> 4111
    `MESH_HERO_BOWERSTONE_BOOT_L_01` + 4112 `..._R_01`
  - OBJECT_HERO_SHIRT_CHAINMAIL (3470) -> 11941/11942 -> 4145
    `MESH_HERO_CHAINMAIL_SHIRT_01`
  - OBJECT_HERO_HELMET_PLATINUM (3512) -> 12086/12087 -> 4225
    `MESH_HERO_PLATINUM_HELMET_01` (covers 0x310)
  - OBJECT_HERO_HAIR_MOHAWK_01 (3383) -> 11658/11659 -> 4263/4264/4265
    (age-staged MOHAWK -> BALD_A -> BALD_B)

## 6. HalsSword.fmp study (how a working equipment mod wires it)

`ExampleFMPFilesForDev/HalsSword.fmp` (weapon, a clone of
OBJECT_LEGENDARY_KATANA):
- Appends 5 game.bin entries at the next free indices (14761 OBJECT +
  14762–14765 CInventoryItemDef / CObjectAugmentationsDef / CWeaponDef /
  CStockItemDef); the OBJECT's CDef listing `dataEntry` fields already point at
  the new indices (`ownerEntry` left stale at the donor 5516 — engine doesn't
  care).
- Adds `MESH_F2_LEGENDARY_LONGSWORD_02` to graphics.big as **id 8113** =
  vanilla max (8112) + 1, and patches `OBJECT.Graphic.modelId` to 8113.
- Adds 3 `maintextures` (diffuse/normal/spec, ids 6291–6293) and 2 `text.big`
  entries (name/description; `CInventoryItemDef.ItemDescription/ItemDetails`
  are text ids).
- The .fmp `*LinkMetaData` banks are .NET-BinaryFormatter
  `NameValueCollection`s (payload-offset -> referenced-entry-name) used by
  ChocolateBox at install time to re-target entry indices; the engine never
  sees them.

## 7. Recipe: a "custom boots" mod

Data to produce (all write paths exist in FableForge / this repo today):

1. **Worn meshes** (1 per side, or a single mesh): skinned to the hero rig
   subset that the region touches (boots: `Bip01 L/R Thigh -> Calf -> Foot ->
   Toe0` chains + root/dummy chain, exact bone names from any retail
   `MESH_HERO_*_BOOT_*` import). Compose as a **type-5** graphics.big entry
   with bone blocks + animated blocks + per-vertex weights.
   *Gap CLOSED (2026-07-20): `mesh_rw.compose_mesh(..., skeleton=
   mesh_rw.clone_skeleton(donor_pay, 5))` composes skinned type-5 payloads —
   skin input is per-vertex `[(bone_name_or_slot, weight)]`; validated by
   recompose oracles (goblin/seagull/boots) + a Blender end-to-end round trip
   of a NEW re-skinned boot injected as id 8115 (docs/MESH_COMPOSE.md
   §1b/§3/§5).* Add with `big_write.rebuild(adds=...)` as id = maxId+1
   (8113+ on vanilla).
2. **Ground-prop mesh** (optional but standard): a small static type-1 mesh
   ("folded boots"); or reuse retail id 129.
3. **game.bin entries** (append; `forge::bin addEntry` / `.fmp` route):
   - unnamed `CAppearanceModifierDef`: `HideMaterials=[L_foot,R_foot]`,
     `Graphics=[{4,0,newMeshIdL,1.0,0,0},{4,0,newMeshIdR,1.0,0,0}]`,
     `CoversBodyAreaFlags=0x1`, `IsRemoveable=1`, `HeroSuit=<a HERO_SUIT or
     0>`, `SuitPart=5`, `AppearanceType=2`, `ArmourDefIndex=404` (or custom
     ARMOUR entry), `OveralProtectionWeighting≈0.05`, multipliers 1.0.
   - unnamed `CInventoryItemDef`: clone OBJECT_HERO_BOOTS' (entry 11710),
     point `ItemDescription`/`ItemDetails` at new text.big ids,
     `InventoryType=18`, `UIInventoryCategory=4`,
     `ItemToSelectUponRemoval=3425` (OBJECT_HERO_NO_BOOTS).
   - unnamed `CStockItemDef`: clone 11711 (pricing/shops).
   - named `OBJECT_MY_BOOTS`: clone OBJECT_HERO_BOOTS' payload; rewrite the
     CDef-listing `dataEntry` of the 3 self-owned records to the new entries
     (set `ownerEntry` = own index; keep the template-inherited records
     verbatim), set `Graphic.modelId` to the ground-prop mesh.
   *Names go through names.bin (`forge::bin` handles appends + CRCs).*
4. **Textures** in graphics.big `GBANK_MAIN_PC` (`tools/texture_build.py`),
   referenced by the mesh's material `matTexId`s.
5. **Text**: 2 text.big entries for name/description (CRC-seed-0 keyed).
6. **Getting it in-game**: add to a shop via `CStockItemDef`/stock lists, a
   chest (`CChestDef`/reward family), TNG placement, or FSE-Lua
   `GiveHeroItem`-style call. Equipping goes through the normal clothing
   inventory (SuitPart 5 slot) with zero code changes — the whole chain is
   data-driven.

Minimal-risk variant (proven-by-construction): **retarget an existing item** —
field-merge only `Graphics.modelId` in its CAppearanceModifierDef to a new
mesh id, everything else untouched.

## 7b. Hero SKIN (face/hands) — see docs/HERO_MORPH.md (2026-07-20)

The hero's own skin is NOT an appearance modifier: the adult body is
**`MESH_HERO`** (graphics.big id 4299, type 5) whose materials bind
`HERO_SKIN_FACE` (1250), `HERO_SKIN_ARMS` (1248 — includes the HANDS),
`HERO_SKIN_TORSO/BACK/LEGS/MOUTH` + `HERO_BUMP_SKIN_*`. Age/alignment/will/
muscle looks are **composited** onto those base sheets from `COMP_HERO_*`
overlays per `CHeroMorphDef.TextureMorphs` (game.bin 10535/11246) — the mesh's
texture ids never change. `GRAPHIC_HERO_FACE` (45) is referenced by no mesh
(stage-1 smoke-test tinted it: no in-game effect). Full record layout, stat
table and the corrected stage-2 green-tint staging: `docs/HERO_MORPH.md`.

## 8. Open questions

- `CAppearanceModifierGraphics::CEntry` ints A/B: A=4 everywhere; B=0
  (clothing), 16 (hair), 6 (horns). def.xml comments claim the values are the
  controlling morph stat ("Age=4, Evil=6, Good=7, Will=10") for the
  appear/disappear range (matches hair=age, horns=evil); which of A/B is the
  selector — and what 16 means — needs the `CAppearanceModifierGraphics`
  Transfer/refresh decompile (`FUN_00708600`-area rebuild path).
- The fixed 21-slot `{u32 crc, u8}` tail of `CThingComponentSet` (same CRC set
  on every THING/OBJECT; flags vary). Not crc0 of the CTC*/C*Def class names —
  unidentified hash input.
- Precise 0x2 vs 0x4 split (calves vs thighs+hips) inferred from HideMaterials
  pairing only.
- `SpecificCoversBodyAreaFlags` (def.xml control `A0E2EA95`) — present in the
  schema of other def types, unused by the 149 vanilla modifiers.
- Villager/NPC clothing (CREATURE `CAppearanceDef` + `CTCDVillagerLook`) not
  covered here; hero-equipment scope only.
- Live (in-game) test of a composed skinned mesh: composition itself is now
  proven offline (grammar-exact vs retail, Blender round trip —
  docs/MESH_COMPOSE.md §1b), but a Fable.exe load of a composed type-5 entry
  has not been observed yet.

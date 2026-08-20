# Quest card BODY art — the real card illustration (mesh + texture)

> **➤ Read `docs/QUEST_CARD_DISPLAY_MAP.md` first** — the authoritative per-screen map (4-context
> RE). Two corrections to this doc: (1) the parchment card is NOT the dominant art — the LEFT
> **region graphic** (`MINIMAP_<region>`) dominates the view/start screens; the card is the small
> right layer. (2) `OBJECT.Graphic.modelId` is confirmed per-card + editable (`forge ui
> set-cardmodel`) and definitely drives the **world/guild-table 3D card**, but whether the **UI
> card display** honors it is inferred (the `UI_QUEST_CARD_MODEL` render isn't in the decomp) —
> confirm in-game before relying on per-card UI mesh swaps.

*RE'd 2026-08-16. This is the card's actual picture (the parchment card you see in the Guild
logbook / start screen), distinct from the small type-seal orb (docs/QUEST_CARD_TEXTURE_BINDING.md)
and the text (docs/QUEST_CARD_BYTE_LAYOUT.md).*

## ⚠ The Quest Start screen is a COMPOSITE — the card texture is the SMALL piece
Ground-truth from a real in-game capture (`work/runtime_smoke_quest_terrain_20260722/
captures_native_run4/user_wasp_card_1337.png`, Wasp Menace). The screen layers, by prominence:

| Element | What it is | Source |
|---|---|---|
| **LEFT panel** (big region/scene illustration — the dominant art) | the quest **region graphic** | `CTCInventoryBase::GetMiniMapGraphic(region)` keyed by the card's `RegionName`, via `UI_QUEST_MENU_MINI_MAP_TEMPLATE` (`questcard_render_decomp.c:2250-2302`); textures = `MINIMAP_<region>_FRONT_END` in **frontend.big** GBANK_FRONT_END_PC |
| **RIGHT panel** (small parchment card in a gold frame) | **the card** (this doc) | `OBJECT.Graphic.modelId` → `MESH_QUEST_CARD_*` → diffuse tex (e.g. 1354). Correct, but the least prominent layer |
| Ornate circular **frame** | card frame | a UI sprite/component |
| **Type-seal orb** (by the title) | quest-type marker | `UI_QUEST_SPRITE_*` (5892/5894/5896) — docs/QUEST_CARD_TEXTURE_BINDING.md |
| Title / gold / renown / objective | text | def + Lua setters |

So to change what a player *reads* as "the quest card art", the highest-impact target is the
**region graphic** (MINIMAP_<region> in frontend.big), NOT the card texture below. The card
mesh/texture (below) is real but small.

## The card body is a 3D mesh chosen by the card OBJECT's Graphic.modelId
`UI_QUEST_CARD_MODEL` (game.bin UI/CUIDef entry 7546, 200×200, MeshType=5, GraphicIndex=0) is a
UI container that renders the **model the active card OBJECT specifies** — the
`OBJECT_QUEST_CARD.Graphic` (CEngineGraphic) `modelId` (the 2nd u32 of the 17-byte CEngineGraphic;
the 1st u32 = graphic-type 5). **modelId is per-OBJECT data**, so the card body is selectable per
card (in retail it's shared across quest groups by type + alignment, not unique per quest).

## The 6 card meshes (graphics.big / MBANK_ALLMESHES) + their textures
Each is a ~980 B card-quad with **2 materials → 2 textures** (diffuse + bump):

| modelId | mesh | diffuse tex id | bump tex id |
|--------:|------|---------------:|------------:|
| 4395 | MESH_QUEST_CARD_CORE_BAD_01 | 1352 `CORE_QUEST_CARD_BAD_01` | 1355 `..._BUMP` |
| 4396 | MESH_QUEST_CARD_CORE_GOOD_01 | 1353 `CORE_QUEST_CARD_GOOD_01` | 1356 |
| 4397 | MESH_QUEST_CARD_CORE_NEUTRAL_01 | **1354 `CORE_QUEST_CARD_NEUTRAL_01`** | 1357 |
| 4398 | MESH_QUEST_CARD_VIN_BAD_01 | (V_QUEST_CARD_BAD) | |
| 4399 | MESH_QUEST_CARD_VIN_GOOD_01 | (V_QUEST_CARD_GOOD) | |
| 4400 | MESH_QUEST_CARD_VIN_NEUTRAL_01 | 1348 `V_QUEST_CARD_NEUTRAL_01` | 1351 |
| 7591 | MESH_QUEST_SCROLL | (scroll diffuse) | |

`CORE` = core quests, `VIN` = vignette; × alignment `GOOD/NEUTRAL/BAD`. Diffuse textures are
**DXT3 256×256** in `textures.big` GBANK_MAIN_PC (the actual card illustration). Retail quest→card
mapping: a quest's OBJECT_QUEST_CARD.Graphic.modelId picks the mesh; e.g. WASP_MENACE + ARENA →
4397 (NEUTRAL core), OAKVALE_INTRO + TEMPLATE → 7591 (scroll).

## Customizing the card body
- **RESKIN (data-only, done + verified):** replace the diffuse texture (e.g. id 1354) in
  textures.big → every card of that type/alignment shows the new face. Proven: AI-reskinned
  CORE_QUEST_CARD_NEUTRAL_01 (256×256 DXT3) swapped in clean. Tools: `tools/ai_texture.py`
  (generate) → `tools/texture_build.py replace` (by name) or a small id-based replace.
- **NEW card model (data — answers "can we add a new one?"):** YES. Add a new mesh to
  graphics.big MBANK_ALLMESHES (`big_write.rebuild(adds=)` / mesh_rw) + add its texture
  (`texture_build.py add`), then set a card OBJECT's `Graphic.modelId` to the new mesh id
  (in-place def edit, or on a from-scratch card). Because Graphic.modelId is per-OBJECT, this is
  **per-card and needs NO native code** — unlike the type-seal orb, whose 3 names are hardcoded in
  ConstructQuestList and would need a detour. So a fully custom card = from-scratch card def +
  custom Graphic.modelId → custom mesh → custom diffuse texture.
- Bump map (2nd tex) can be left as-is or regenerated for lighting.

## Anchors
- `graphics.big` MBANK_ALLMESHES (8112 meshes); `tools/parse_mesh.py --entry <mesh>` → matTexId[].
- `UI_QUEST_CARD_MODEL` game.bin 7546; `OBJECT_QUEST_CARD.Graphic` CEngineGraphic modelId (per-card).
- Card diffuse ids 1352-1354 (CORE) / 1348 (VIN) / bump 1355-1357 / 1351, textures.big GBANK_MAIN_PC.

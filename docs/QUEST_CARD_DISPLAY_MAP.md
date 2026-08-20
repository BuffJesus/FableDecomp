# Quest Card Display Map — where every quest-card element is drawn + its art source

*RE'd 2026-08-16 (`quest-card-display-contexts` workflow: 4 parallel context RE agents +
synthesis, screenshot-grounded). Supersedes the "card art = X" claims in QUEST_CARD_BODY_ART.md
and QUEST_CARD_TEXTURE_BINDING.md, which oversold single elements.*

## Core truth
**"Quest card art" is not one asset — the dominant element changes per screen, and the
parchment card texture users picture is rarely the biggest thing shown.** On the prominent
screens (view/accept + start), the eye reads the **region illustration** (~70% of the width),
not the small parchment card.

## Per-context dominance
| Context | Renders | Dominant art | Source |
|---|---|---|---|
| **Logbook LIST** (`INVENTORY_TYPE_QUESTS_SCREEN`, `ConstructQuestList @0x0061B610`, `param_3==0`) | type-seal orb + title text; **no** card mesh, **no** region graphic | **type-seal orb** | `HUD_ORB_QUEST_{CORE,OPTIONAL,VIGNETTE}` ids **5892/5894/5896** (textures.big GBANK_MAIN_PC) via CUIDef `UI_QUEST_SPRITE_*` (game.bin 8487-8489) |
| **Single-card VIEW / accept** (`INVENTORY_TYPE_QUEST_CARD_SCREEN` — the "guild quest table" view) | composite | **LEFT region graphic** | `MINIMAP_<RegionName>_FRONT_END` (frontend.big GBANK_FRONT_END_PC) via `GetMiniMapGraphic(region)` + `UI_QUEST_MENU_MINI_MAP_TEMPLATE` (game.bin 7501) |
| **Quest-START composite** (on activation; screenshot user_wasp_card_1337.png) | LEFT region + RIGHT small card + orb + text | **LEFT region graphic** dominates; card is small | region as above; card = `MESH_QUEST_CARD_*` via `UI_QUEST_CARD_MODEL` (game.bin 7546, MeshType=5) |
| **Guild table / world pickup** (`CS_PICK_UP_CARD`, physical 3D) | 3D card in world | the card **mesh** | `MESH_QUEST_CARD_*` (graphics.big MBANK_ALLMESHES) |

## Confirmed asset ids
- **Region illustration:** `MINIMAP_<region>_FRONT_END`, frontend.big GBANK_FRONT_END_PC.
  Per-region (shared by all quests in a region). Keyed by `CQuestCardDef.RegionName`.
- **Card meshes (graphics.big MBANK_ALLMESHES):** 4395 CORE_BAD, 4396 CORE_GOOD,
  **4397 CORE_NEUTRAL**, 4398 VIN_BAD, 4399 VIN_GOOD, 4400 VIN_NEUTRAL, 7591 SCROLL.
- **Card diffuse/bump (textures.big GBANK_MAIN_PC, DXT3 256×256):** 1354/1357 CORE_NEUTRAL,
  1353/1356 CORE_GOOD, 1352/1355 CORE_BAD, 1348/1351 V_NEUTRAL.
- **Orbs:** 5892 CORE / 5894 OPTIONAL / 5896 VIGNETTE, 32×32 (DXT format NOT confirmed — TODO).

## Selection logic
- **Orb:** hardcoded 3-way branch in `ConstructQuestList @0x0061B610` (+PC twin `0x0061E6D0`):
  `IsCore` → CORE; else byte `*(card+0xc)+0x59`==0 → OPTIONAL else VIGNETTE. Class-only.
- **Card mesh:** the card OBJECT's `Graphic` (CEngineGraphic) `modelId` (2nd u32) — per-OBJECT,
  byte-verified to vary (WASP/ARENA 4397, TEMPLATE/OAKVALE 7591, PROTECT 4396), editable via
  `forge ui set-cardmodel`. **✅ CONFIRMED IN-GAME 2026-08-16**: setting the "Melee Combat Test"
  card modelId → a custom mesh (MESH_HEADSPIDER) rendered the custom mesh **on the on-screen
  quest-start card popup** (right ornate-ring panel). So the UI card display DOES honor
  `OBJECT.Graphic.modelId` — not just the world/pickup card. (Orientation: the FBX imported
  upside-down — Blender Z-up vs Fable's up-axis; rotate in the extract step.) `CQuestCardDef`
  (sub-def) correctly has NO Graphic field — modelId lives on the OBJECT.

## ⚠ Corrections from the first in-game run (2026-08-16)
A full-reskin deploy on "Melee Combat Test" (TRAINING_KILL_BEETLES) confirmed some mappings and
refuted others:
- ✅ **Card model** (right ring panel) = OBJECT.Graphic.modelId (spider rendered).
- ✅ **Title** = static def QuestName TextID (changed via text.big).
- ✅ **Type-seal orb** (by title) = HUD_ORB_QUEST_* 5892 (emerald orb showed).
- ✅ **LEFT hero panel** = **textures.big id 4722** (`PC_UI_QUEST_START_FRAME_1_SPRITE`, PC build) /
  **id 4783** (`UI_BACKDROP_QUEST_START_SPRITE_1`, console-variant). It is a **FIXED PAINTED sprite
  baked into the quest-start frame atlas**, NOT 5515, NOT a minimap, NOT a live 3D render. See the
  RESOLVED section below for the full mapping + reskin recipe. (This is why the 5515 reskin had no
  effect — 5515 is the wrong id.)
- ❌ **Objective/summary line** on the start screen was NOT changed by editing
  `TEXT_QUEST_TRAINING_KILL_BEETLES_SUMMARY` — that line is set at RUNTIME (SetQuestCardObjective)
  to a different symbol; only the TITLE came from the static def. So the start-screen body text is
  runtime, not the SUMMARY field.
- ✅ **Backdrop + ornate ring frame** = the same baked quest-start atlas (`PC_UI_QUEST_START_FRAME`
  4722-4730 / `UI_BACKDROP_QUEST_START` 4783-4788): wood-grain backdrop, beveled frame and the
  ornate filigree ring are all tiles of that composite. See the RESOLVED section below.
- ℹ️ **region map** (MINIMAP_GUILDWOODS) does NOT drive the quest-**start** composite — the start
  screen's left panel is the baked hero sprite, not a minimap. The minimap drives the single-card
  VIEW/accept screen only (see the per-context table above).
- **Region:** `CQuestCardDef.RegionName` (crc0 `0x6FE0488D`) → `GetMiniMapGraphic`.
- **Class family:** `CQuestCardDef.IsCoreQuest` (`0x3995EBC1`) / `IsVignette` (`0x3C9E385B`)
  pick core-vs-vignette (orb + mesh family).

## Customization tiers (what to actually do)
1. **Distinctive quest — DATA-ONLY, highest impact:** the **region graphic**
   (`MINIMAP_<region>_FRONT_END`) — set `RegionName` to the region whose illustration you want,
   OR replace/add the MINIMAP texture in frontend.big. Plus **Title/Objective/Reward** text
   (QuestName/QuestSummary TextIDs; objective/gold/renown via Lua `Quest:SetQuestCard*`). These
   are the only elements that read as "this quest's own art" without engine work.
   *(RegionName drives the `GiveHeroQuestCardDirectly` path; confirm your activation path honors
   it — the plain `AddQuestCard` path ignores RegionName per QUEST_CARD_BYTE_LAYOUT.md.)*
2. **Class reskin — DATA-ONLY, affects every quest of that class+alignment:** replace card
   diffuse (`texture_build.py replace` id 1354 etc.) and/or orb textures (5892/5894/5896), or
   edit CUIDef 8487-8489 `States[0].GraphicIndex` (`forge ui set-graphic`). A total-conversion
   aesthetic, not a one-quest standout.
3. **Custom card MESH — DATA (tooling built), UI-effect to confirm in-game:** add a mesh
   (graphics.big MBANK, `big_write.rebuild(adds=)`/mesh_rw) + its texture (`texture_build.py
   add`) + `forge ui set-cardmodel <OBJECT> <meshId>`. Definitely changes the world/pickup card;
   confirm the UI card display honors it (caveat above).
4. **Per-quest UNIQUE orb — NATIVE:** ForgeFSE detour on `0x0061B610`/`0x0061E6D0` reading
   `ScriptQuestName` (card+0x28) → registry → `CreateComponent(customCUIDef)`. Not data.

## Tooling (all built + verified headless this session)
`forge ui set-graphic` (orb/CUIDef GraphicIndex), `forge ui add-sprite` (custom CUIDef),
`forge ui set-cardmodel` (OBJECT Graphic.modelId), `tools/texture_build.py add|replace|decode`,
`tools/ai_texture.py` (generative), `tools/parse_mesh.py --entry <MESH>` (mesh→matTexId).

## Hero panel + backdrop RESOLVED (2026-08-16)

The quest-**START** composite window (screenshot `user_wasp_card_1337.png`) is **almost entirely a
single pre-painted, shared static atlas** baked into textures.big — NOT per-region, NOT per-quest,
NOT a live 3D hero render, NOT a minimap. Only the small parchment card inside the ring is dynamic
(the `OBJECT.Graphic.modelId` mesh, already confirmed above). This resolves the two `❌/❔` notes:
the 5515 reskin had no effect because 5515 is simply the wrong id.

### The two atlas families (PC vs console variant — same art)
| Family | textures.big ids | Entry names | game.bin CUIDef container | Build |
|---|---|---|---|---|
| **PC** (live in the PC screenshot) | **4722–4730** | `PC_UI_QUEST_START_FRAME_1..9_SPRITE` | `PC_UI_QUEST_START_FRAME` idx **6822** (crc `0x026E56D9`), children `PC_UI_QUEST_START_FRAME_1..9` idx **6856–6864** | PC (DXT3, 3×3 grid) |
| **Console/base** (parallel, identical art) | **4783–4788** | `UI_BACKDROP_QUEST_START_SPRITE_1..6` | `UI_BACKDROP_QUEST_START` idx **8345** (crc `0x06ED93B9`), children `_1.._6` idx **8346–8351** | base (DXT1, 3×2 grid) |

Both are Type-5 container CUIDefs (own `GraphicIndex=0`); each child tile carries a crc0-tagged
`States[0].GraphicIndex` → its `_SPRITE` texture id and a stepped grid `posX/posY`. The card mesh
renders on top via `UI_QUEST_CARD_MODEL` (game.bin 7546, MeshType=5).

### Element → exact source (CONFIRMED, ≥2 agreeing angles + visual pixel-match)
| On-screen element | bank | id | entry name | binding |
|---|---|---|---|---|
| **LEFT hero illustration** (bald kneeling swordsman + guild archway + cloudy sky) | textures.big | **4722** (PC) / **4783** (console) | `PC_UI_QUEST_START_FRAME_1_SPRITE` / `UI_BACKDROP_QUEST_START_SPRITE_1` | CUIDef child idx 6856 / 8346, `States[0].GraphicIndex`, top-left tile (posX=0,posY=0) |
| **Ornate metal RING frame** (filigree scrollwork around the card) | textures.big | **4723**(+4724/4726) (PC) / **4784** (console) | `PC_UI_QUEST_START_FRAME_2..5_SPRITE` / `UI_BACKDROP_QUEST_START_SPRITE_2` | CUIDef children idx 6857–6860 / 8347, right-column tiles |
| **Wood-grain BACKDROP + beveled frame** | textures.big | **4726/4785–4788** | `PC_UI_QUEST_START_FRAME_*_SPRITE` / `UI_BACKDROP_QUEST_START_SPRITE_*` | same atlas, lower/edge tiles |
| **3D parchment card** (inside the ring) | graphics.big MBANK | per-card | `MESH_QUEST_CARD_*` | `OBJECT.Graphic.modelId` via `UI_QUEST_CARD_MODEL` (game.bin 7546) — dynamic, see above |

**Shared vs per-region/quest:** the left hero, ring frame and backdrop are a **SHARED default** shown
on **every** quest-start screen regardless of quest or region. The companion `AddQuestCard` path used
for the screenshot ignores `RegionName` anyway (per QUEST_CARD_BYTE_LAYOUT.md), but even on the
`GiveHeroQuestCardDirectly` path this atlas is fixed — the minimap only appears on the separate
single-card VIEW screen. Confidence: **HIGH** (visual pixel-match of decoded PNGs + contiguous
texture-catalog entry names + matching game.bin CUIDef def-tree child counts, three independent
angles).

### How to reskin each layer
- **LEFT hero:** replace the hero tile in-place:
  `python tools/texture_build.py replace <textures.big> <out.big> 4722 hero.png`
  (PC build). Mirror to **4783** for the console/base variant if targeting both. The hero occupies
  the top-left tile; to repaint the *whole* left panel you may also need tiles 4725 (PC) / 4786 (lower
  body). 256×256 DXT3 (PC) / DXT1 (console).
- **Ring frame:** replace **4723/4724/4726** (PC) or **4784** (console) — but note the ring is spread
  across several tiles, so a clean reskin means editing the full 3×3 / 3×2 composite as one image and
  re-slicing.
- **Wood-grain backdrop:** replace the backdrop tiles **4726/4785–4788**.
- **The whole window at once:** treat 4722–4730 (PC) as a 768×768 3×3 composite, paint it, re-slice
  into the 9 `_SPRITE` ids, and `texture_build.py replace` each — the cleanest way to retheme the
  entire quest-start window.

### Winning evidence PNGs (viewable)
- LEFT hero (console 4783, includes red-scroll top border, exact panel match):
  `…/heroprobe/verify_left_hero_4783.png`
- LEFT hero (PC 4722, head/torso tile): `…/heroprobe/verify_left_hero_4722.png`
- Ring frame (PC 4723, filigree scrollwork ring): `…/heroprobe/verify_ring_frame_4723.png`
- Backdrop wood-grain (4763 family / 4786-class tiles): `…/heroprobe/verify_backdrop_4763.png`

*(Aside: `UI_VIEW_RING_*` 4864-4867 and `UI_VIEW_RING_SMALL_SPRITE` 4868 and `UI_RING_BACKDROP_*`
4763-4768 are the **single-card VIEW screen's** ring/backdrop assets — a different screen. They
visually resemble the quest-start ring but are NOT what the start composite binds; the start screen
uses the baked `*_QUEST_START_FRAME`/`*_BACKDROP_QUEST_START` atlas above. Left PLAUSIBLE, not the
answer for this screenshot.)*

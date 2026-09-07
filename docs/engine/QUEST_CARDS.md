# Quest Cards — script API, per-screen display, body art, orb art, texture binding, fixes

*Merged 2026-09-07 from `QUEST_CARD_SYSTEM.md`, `QUEST_CARD_DISPLAY_MAP.md`, `QUEST_CARD_BODY_ART.md`,
`QUEST_CARD_TEXTURE_BINDING.md`, `QUEST_CARD_EMPTY_FIX.md`. The wire format of `CQuestCardDef` +
`OBJECT_QUEST_CARD` (18-field Transfer order, crc0 tags, OBJECT back-refs at offsets 81/85) stays in
[../formats/QUEST_CARD_BYTE_LAYOUT.md](../formats/QUEST_CARD_BYTE_LAYOUT.md). Retail `Fable.exe`
addresses unless stated.*

## Contents
- [The two-entry model](#the-two-entry-model)
- [Script API: the two give paths](#script-api-the-two-give-paths)
- [Per-screen display map](#per-screen-display-map)
- [Body art: 3D card mesh](#body-art-3d-card-mesh)
- [Orb art: type-seal sprite](#orb-art-type-seal-sprite)
- [Texture binding: data-only vs detour](#texture-binding-data-only-vs-detour)
- [Empty-card fix](#empty-card-fix)
- [Card-viewport garble fix](#card-viewport-garble-fix)
- [Customization tiers and tooling](#customization-tiers-and-tooling)
- [Verified facts with addresses](#verified-facts-with-addresses)
- [Dated findings](#dated-findings)
- [Superseded claims](#superseded-claims)
- [Open questions](#open-questions)
- [Evidence](#evidence)

---

## The two-entry model

A usable custom card is a PAIR of game.bin entries:
1. an unnamed **`CQuestCardDef`** sub-def (~136 B) — the card CONTENT (QuestName/QuestSummary/
   RegionName TextIDs, rewards, IsCoreQuest/IsVignette, InventoryCategory …), and
2. a named **`OBJECT` "OBJECT_QUEST_CARD_<X>"** (a `CObjectDef`, ~458 B) — the inventory object the
   runtime resolves by NAME (`Quest:AddQuestCard("OBJECT_QUEST_CARD_<X>", …)`).

The OBJECT links to its card def by global entry index (`u32` @ offset 81) and the embedded
`CQuestCardDef` component record carries a self-back-ref to the OBJECT's own global index (`u32` @
offset 85); both must be retargeted on append (see `../formats/DEF_LOAD_CONTRACT.md` BUG #2). Field
order, crc0 tags, measured offsets and from-scratch defaults:
[QUEST_CARD_BYTE_LAYOUT.md](../formats/QUEST_CARD_BYTE_LAYOUT.md).

A card lives in **two places** at runtime: the **Guild "available cards" list** (what the Logbook
renders) and, transiently, as a **card *thing*** (`CTCQuestCard`) coupled to quest activation. The two
give-functions target different halves — which is the whole story behind "card gives ok=true but
never shows."

---

## Script API: the two give paths

The quest-card API is a contiguous `CGameScriptInterface` vtable group (slots 292–299).

### `AddQuestCard(cardObjName, questName, replayable, skipGMmsg)` — vtable[292] @0x008913F0
The **register-by-name** path. Calls `FUN_004b1670`:
- reads the `CQuestManager` guild **available-quest-cards `std::list`** (`this+0x58`),
- finds `cardObjName`; **if absent, appends `{cardObjName, questName, replayable}` and sets the
  dirty byte (+0x8F)**.
- **No card thing created, no quest activation, no kill.** Pure list insert.

The Logbook renders from this list, resolving `cardObjName → OBJECT def → its inline
`<CQuestCardDef>` component` (QuestName/QuestSummary/RegionName TextIDs) at draw time. **This is why
it reliably displays.**

### `GiveHeroQuestCardDirectly(cardObj, questName, flag)` — vtable[295] @0x008968C0
The **quest-activation-coupled** path — NOT a Logbook add. It:
1. `CThingObject::Create(cardObjectDefIndex, …)` — makes a **transient** card thing.
2. sets the card's script-quest name to `questName` (`(tc+0x28) = param_3`).
3. `CQuestManager::ActivateQuestCard(card, true)` @0x004b4aa0.
4. **If ActivateQuestCard returns false → `CThing::Kill(card)` — the card is destroyed.**

`ActivateQuestCard` succeeds only when `FUN_004b4a10(card+0x28, …)` →
`CQuestManager::ActivateMultipleQuests` can activate a quest **whose name equals the card's
RegionName field**, and `IsActiveQuestBetweenQuestStartAndFinishScreensInRegion` gates it. So this is
the **"start the quest via its card"** path (a real quest-start card). If the named quest isn't an
activatable card-region, it **silently kills the card** → invisible. The Lua `ok=true` only means the
pcall didn't throw; the engine fn returns void and self-destructs the card on failure.

**So:** calling `GiveHeroQuestCardDirectly` with `"FSE_Master"` (not an activatable card-region) →
card created, activation failed, card Killed → nothing shown. **Wrong function for "show a card"; it's
for "activate this quest through its card."**

The first argument is an `OBJECT_QUEST_CARD_*` definition name. ForgeFSE formerly called it
`textDBEntry` in its C++ header, but retail resolves that string to an object-definition index before
calling `CThingObject::Create`; passing `TEXT_*` there is therefore incorrect. ForgeFSE now names the
argument `questCardObjectName` in its header and generated manifest.

### The rest of the group
| Slot | Fn | Role |
|---|---|---|
| 292 `0x490` | `AddQuestCard` | register available card → Guild/Logbook |
| 293 `0x494` | `RemoveQuestCardFromGuild` | remove from guild list |
| 294 `0x498` | `RemoveQuestCardFromHero` | remove from hero's held |
| 295 `0x49C` | `GiveHeroQuestCardDirectly` | create card thing + activate quest (or kill) |
| 296 `0x4A0` | `SetQuestCardObjective(quest, objText, r1, r2)` | objective line on the card |
| 297 `0x4A4` | `SetQuestCardGoldReward(quest, amt)` | displayed gold |
| 298 `0x4A8` | `SetQuestCardRenownReward(quest, amt)` | displayed renown |
| 299 `0x4AC` | `RemoveAllAvailableQuestCardsFromGuild` | clear guild board |
| 544 `0x880` | `SetHeroGuideToShowQuestCardsWhenSpokenTo` | guide-shows-cards toggle |

### The working recipe (validated, deployed)
```lua
Quest:AddQuestCard("OBJECT_DUMMY_QUEST_CARD_DEFEAT_SNOW_TROLL", "FSE_Master", false, false)
```
- arg1 = card OBJECT def name (must exist in game.bin — our overwrite provides it; def **needs no
  field change** to appear — it's a real Create-able card).
- arg2 = host quest name (**stored, not validated at add-time**; the MASTER quest FSE_Master is fine —
  no `S_ALWAYS_ACTIVE` helper quest required, that shape is only needed if the host's `Main()` doesn't
  reliably run).
- arg3 = replayable flag (cosmetic, stored in the list node).
- arg4 = `false` → also flashes the "new quest" GM HUD popup (`TEXT_QST_078_GM_MSG_NEW_QUEST`);
  `true` → silent.
No ForgeFSE DLL change needed — `AddQuestCard` is already bound (vtable[292]).

For a quest with runtime objective/reward text, registration and population are two separate phases.
The setters resolve the **active** `CTCQuestCard` by script name, so calling them immediately after
`AddQuestCard` while the card is merely in the Guild list is a no-op:

```lua
-- Registration phase: makes the compiled card asset available.
Quest:AddQuestCard("OBJECT_QUEST_CARD_MY_CUSTOM_QUEST", "MyCustomQuest", false, true)

-- Activation phase: creates/resolves the runtime card thing.
Quest:ActivateQuest("MyCustomQuest")
Quest:Pause(0.1)
if not Quest:NewScriptFrame() then return end

-- Runtime-population phase: these now have an active card to update.
Quest:SetQuestCardObjective("MyCustomQuest", "TEXT_MY_CUSTOM_QUEST_OBJECTIVE",
                            "Oakvale", "")
Quest:SetQuestGoldReward("MyCustomQuest", 500)
Quest:SetQuestRenownReward("MyCustomQuest", 100)
Quest:KickOffQuestStartScreen("MyCustomQuest", false, false)
```

Use `Quest:` methods in Lua; these functions are methods on the quest-state object, not bare global
functions.

### Facts worth remembering
- **No `DUMMY_`-prefix hide filter exists** — the prefix is naming convention only.
- `AddQuestCard` associates card↔quest **purely by name string**; the OBJECT needs no back-link to the
  quest.
- `FUN_004b1670` **de-dups by card name** — if a prior run already added it to a persisted save's guild
  list, re-adding is a no-op (still displays, no second popup).
- If a card still doesn't appear after `AddQuestCard`, the next suspect is the card's `<CQuestCardDef>`
  `RegionName` / `IsCoreQuest` / `InventoryCategory` (which Logbook tab it files under: Current vs
  Available vs Completed) — **not** the give path.
- A full custom quest presents as: `AddQuestCard` (register) → `ActivateQuest` (create/resolve runtime
  card) → `SetQuestCardObjective` + reward setters (fill) → `AddQuestInfoBar/Counter/Timer` (live HUD
  tracking) → `TellHeroQuestObjectiveCompleted` (close).
- `RegionName` gates the `GiveHeroQuestCardDirectly` path only; the plain `AddQuestCard` path ignores
  it (per QUEST_CARD_BYTE_LAYOUT.md).

---

## Per-screen display map

### Core truth
**"Quest card art" is not one asset — the dominant element changes per screen, and the parchment card
texture users picture is rarely the biggest thing shown.** On the single-card VIEW/accept screen the
eye reads the **region illustration** (~70% of the width), not the small parchment card; on the
quest-START composite the left panel is a **baked hero sprite** from a shared atlas (see below).

### Per-context dominance
| Context | Renders | Dominant art | Source |
|---|---|---|---|
| **Logbook LIST** (`INVENTORY_TYPE_QUESTS_SCREEN`, `ConstructQuestList @0x0061B610`, `param_3==0`) | type-seal orb + title text; **no** card mesh, **no** region graphic | **type-seal orb** | `HUD_ORB_QUEST_{CORE,OPTIONAL,VIGNETTE}` ids **5892/5894/5896** (textures.big GBANK_MAIN_PC) via CUIDef `UI_QUEST_SPRITE_*` (game.bin 8487-8489) |
| **Single-card VIEW / accept** (`INVENTORY_TYPE_QUEST_CARD_SCREEN` — the "guild quest table" view) | composite | **LEFT region graphic** | `MINIMAP_<RegionName>_FRONT_END` (frontend.big GBANK_FRONT_END_PC) via `GetMiniMapGraphic(region)` + `UI_QUEST_MENU_MINI_MAP_TEMPLATE` (game.bin 7501) |
| **Quest-START composite** (on activation; screenshot `user_wasp_card_1337.png`) | LEFT baked hero panel + RIGHT small card in ornate ring + orb + text | **LEFT hero panel** (shared baked atlas); card is small | atlas `PC_UI_QUEST_START_FRAME` 4722–4730 / `UI_BACKDROP_QUEST_START` 4783–4788; card = `MESH_QUEST_CARD_*` via `UI_QUEST_CARD_MODEL` (game.bin 7546, MeshType=5) |
| **Guild table / world pickup** (`CS_PICK_UP_CARD`, physical 3D) | 3D card in world | the card **mesh** | `MESH_QUEST_CARD_*` (graphics.big MBANK_ALLMESHES) |

Single-card VIEW screen layers (from the Wasp Menace capture
`work/runtime_smoke_quest_terrain_20260722/captures_native_run4/user_wasp_card_1337.png`):

| Element | What it is | Source |
|---|---|---|
| **LEFT panel** (big region/scene illustration) | the quest **region graphic** | `CTCInventoryBase::GetMiniMapGraphic(region)` keyed by the card's `RegionName`, via `UI_QUEST_MENU_MINI_MAP_TEMPLATE` (`questcard_render_decomp.c:2250-2302`); textures = `MINIMAP_<region>_FRONT_END` in **frontend.big** GBANK_FRONT_END_PC |
| **RIGHT panel** (small parchment card in a gold frame) | **the card** | `OBJECT.Graphic.modelId` → `MESH_QUEST_CARD_*` → diffuse tex (e.g. 1354). Correct, but the least prominent layer |
| Ornate circular **frame** | card frame | a UI sprite/component (`UI_VIEW_RING_*` 4864-4867, `UI_VIEW_RING_SMALL_SPRITE` 4868, `UI_RING_BACKDROP_*` 4763-4768 — VIEW-screen assets) |
| **Type-seal orb** (by the title) | quest-type marker | `UI_QUEST_SPRITE_*` (5892/5894/5896) |
| Title / gold / renown / objective | text | def + Lua setters |

### Confirmed asset ids
- **Region illustration:** `MINIMAP_<region>_FRONT_END`, frontend.big GBANK_FRONT_END_PC. Per-region
  (shared by all quests in a region). Keyed by `CQuestCardDef.RegionName` (crc0 `0x6FE0488D`) →
  `GetMiniMapGraphic`.
- **Card meshes (graphics.big MBANK_ALLMESHES):** 4395 CORE_BAD, 4396 CORE_GOOD, **4397 CORE_NEUTRAL**,
  4398 VIN_BAD, 4399 VIN_GOOD, 4400 VIN_NEUTRAL, 7591 SCROLL.
- **Card diffuse/bump (textures.big GBANK_MAIN_PC, DXT3 256×256):** 1354/1357 CORE_NEUTRAL, 1353/1356
  CORE_GOOD, 1352/1355 CORE_BAD, 1348/1351 V_NEUTRAL.
- **Orbs:** 5892 CORE / 5894 OPTIONAL / 5896 VIGNETTE, 32×32 (DXT format NOT confirmed — TODO).
- **Class family:** `CQuestCardDef.IsCoreQuest` (`0x3995EBC1`) / `IsVignette` (`0x3C9E385B`) pick
  core-vs-vignette (orb + mesh family).

### Selection logic
- **Orb:** hardcoded 3-way branch in `ConstructQuestList @0x0061B610` (+PC twin `0x0061E6D0`):
  `IsCore` → CORE; else byte `*(card+0xc)+0x59`==0 → OPTIONAL else VIGNETTE. Class-only.
- **Card mesh:** the card OBJECT's `Graphic` (CEngineGraphic) `modelId` (2nd u32) — per-OBJECT,
  byte-verified to vary (WASP/ARENA 4397, TEMPLATE/OAKVALE 7591, PROTECT 4396), editable via
  `forge ui set-cardmodel`. **CONFIRMED IN-GAME**: setting the "Melee Combat Test" card modelId → a
  custom mesh (MESH_HEADSPIDER) rendered the custom mesh **on the on-screen quest-start card popup**
  (right ornate-ring panel). So the UI card display DOES honor `OBJECT.Graphic.modelId` — not just the
  world/pickup card. (Orientation: the FBX imported upside-down — Blender Z-up vs Fable's up-axis;
  rotate in the extract step.) `CQuestCardDef` (sub-def) correctly has NO Graphic field — modelId
  lives on the OBJECT.

### What the first in-game reskin run confirmed / refuted
A full-reskin deploy on "Melee Combat Test" (TRAINING_KILL_BEETLES):
- ✅ **Card model** (right ring panel) = OBJECT.Graphic.modelId (spider rendered).
- ✅ **Title** = static def QuestName TextID (changed via text.big).
- ✅ **Type-seal orb** (by title) = HUD_ORB_QUEST_* 5892 (emerald orb showed).
- ✅ **LEFT hero panel** = **textures.big id 4722** (`PC_UI_QUEST_START_FRAME_1_SPRITE`, PC build) /
  **id 4783** (`UI_BACKDROP_QUEST_START_SPRITE_1`, console-variant). It is a **FIXED PAINTED sprite
  baked into the quest-start frame atlas**, NOT 5515, NOT a minimap, NOT a live 3D render. (This is why
  the 5515 reskin had no effect — 5515 is the wrong id.)
- ❌ **Objective/summary line** on the start screen was NOT changed by editing
  `TEXT_QUEST_TRAINING_KILL_BEETLES_SUMMARY` — that line is set at RUNTIME (SetQuestCardObjective) to a
  different symbol; only the TITLE came from the static def. So the start-screen body text is runtime,
  not the SUMMARY field.
- ✅ **Backdrop + ornate ring frame** = the same baked quest-start atlas (`PC_UI_QUEST_START_FRAME`
  4722-4730 / `UI_BACKDROP_QUEST_START` 4783-4788): wood-grain backdrop, beveled frame and the ornate
  filigree ring are all tiles of that composite.
- ℹ️ **region map** (MINIMAP_GUILDWOODS) does NOT drive the quest-**start** composite — the start
  screen's left panel is the baked hero sprite, not a minimap. The minimap drives the single-card
  VIEW/accept screen only.

### Quest-start composite: hero panel + backdrop atlas
The quest-**START** composite window is **almost entirely a single pre-painted, shared static atlas**
baked into textures.big — NOT per-region, NOT per-quest, NOT a live 3D hero render, NOT a minimap. Only
the small parchment card inside the ring is dynamic (the `OBJECT.Graphic.modelId` mesh).

#### The two atlas families (PC vs console variant — same art)
| Family | textures.big ids | Entry names | game.bin CUIDef container | Build |
|---|---|---|---|---|
| **PC** (live in the PC screenshot) | **4722–4730** | `PC_UI_QUEST_START_FRAME_1..9_SPRITE` | `PC_UI_QUEST_START_FRAME` idx **6822** (crc `0x026E56D9`), children `PC_UI_QUEST_START_FRAME_1..9` idx **6856–6864** | PC (DXT3, 3×3 grid) |
| **Console/base** (parallel, identical art) | **4783–4788** | `UI_BACKDROP_QUEST_START_SPRITE_1..6` | `UI_BACKDROP_QUEST_START` idx **8345** (crc `0x06ED93B9`), children `_1.._6` idx **8346–8351** | base (DXT1, 3×2 grid) |

Both are Type-5 container CUIDefs (own `GraphicIndex=0`); each child tile carries a crc0-tagged
`States[0].GraphicIndex` → its `_SPRITE` texture id and a stepped grid `posX/posY`. The card mesh
renders on top via `UI_QUEST_CARD_MODEL` (game.bin 7546, MeshType=5).

#### Element → exact source (CONFIRMED, ≥2 agreeing angles + visual pixel-match)
| On-screen element | bank | id | entry name | binding |
|---|---|---|---|---|
| **LEFT hero illustration** (bald kneeling swordsman + guild archway + cloudy sky) | textures.big | **4722** (PC) / **4783** (console) | `PC_UI_QUEST_START_FRAME_1_SPRITE` / `UI_BACKDROP_QUEST_START_SPRITE_1` | CUIDef child idx 6856 / 8346, `States[0].GraphicIndex`, top-left tile (posX=0,posY=0) |
| **Ornate metal RING frame** (filigree scrollwork around the card) | textures.big | **4723**(+4724/4726) (PC) / **4784** (console) | `PC_UI_QUEST_START_FRAME_2..5_SPRITE` / `UI_BACKDROP_QUEST_START_SPRITE_2` | CUIDef children idx 6857–6860 / 8347, right-column tiles |
| **Wood-grain BACKDROP + beveled frame** | textures.big | **4726/4785–4788** | `PC_UI_QUEST_START_FRAME_*_SPRITE` / `UI_BACKDROP_QUEST_START_SPRITE_*` | same atlas, lower/edge tiles |
| **3D parchment card** (inside the ring) | graphics.big MBANK | per-card | `MESH_QUEST_CARD_*` | `OBJECT.Graphic.modelId` via `UI_QUEST_CARD_MODEL` (game.bin 7546) — dynamic |

**Shared vs per-region/quest:** the left hero, ring frame and backdrop are a **SHARED default** shown on
**every** quest-start screen regardless of quest or region. The companion `AddQuestCard` path used for
the screenshot ignores `RegionName` anyway, but even on the `GiveHeroQuestCardDirectly` path this atlas
is fixed — the minimap only appears on the separate single-card VIEW screen. Confidence: **HIGH**
(visual pixel-match of decoded PNGs + contiguous texture-catalog entry names + matching game.bin CUIDef
def-tree child counts, three independent angles).

#### How to reskin each layer
- **LEFT hero:** replace the hero tile in-place:
  `python tools/texture_build.py replace <textures.big> <out.big> 4722 hero.png` (PC build). Mirror to
  **4783** for the console/base variant if targeting both. The hero occupies the top-left tile; to
  repaint the *whole* left panel you may also need tiles 4725 (PC) / 4786 (lower body). 256×256 DXT3
  (PC) / DXT1 (console).
- **Ring frame:** replace **4723/4724/4726** (PC) or **4784** (console) — but note the ring is spread
  across several tiles, so a clean reskin means editing the full 3×3 / 3×2 composite as one image and
  re-slicing.
- **Wood-grain backdrop:** replace the backdrop tiles **4726/4785–4788**.
- **The whole window at once:** treat 4722–4730 (PC) as a 768×768 3×3 composite, paint it, re-slice into
  the 9 `_SPRITE` ids, and `texture_build.py replace` each — the cleanest way to retheme the entire
  quest-start window.

#### Winning evidence PNGs (viewable)
- LEFT hero (console 4783, includes red-scroll top border, exact panel match):
  `…/heroprobe/verify_left_hero_4783.png`
- LEFT hero (PC 4722, head/torso tile): `…/heroprobe/verify_left_hero_4722.png`
- Ring frame (PC 4723, filigree scrollwork ring): `…/heroprobe/verify_ring_frame_4723.png`
- Backdrop wood-grain (4763 family / 4786-class tiles): `…/heroprobe/verify_backdrop_4763.png`

*(Aside: `UI_VIEW_RING_*` 4864-4867 and `UI_VIEW_RING_SMALL_SPRITE` 4868 and `UI_RING_BACKDROP_*`
4763-4768 are the **single-card VIEW screen's** ring/backdrop assets — a different screen. They visually
resemble the quest-start ring but are NOT what the start composite binds; the start screen uses the baked
`*_QUEST_START_FRAME`/`*_BACKDROP_QUEST_START` atlas above. Left PLAUSIBLE, not the answer for the start
screenshot.)*

---

## Body art: 3D card mesh

The card body (the parchment card you see in the Guild logbook / start screen) is distinct from the
small type-seal orb and from the text.

### The card body is a 3D mesh chosen by the card OBJECT's Graphic.modelId
`UI_QUEST_CARD_MODEL` (game.bin UI/CUIDef entry 7546, 200×200, MeshType=5, GraphicIndex=0) is a UI
container that renders the **model the active card OBJECT specifies** — the `OBJECT_QUEST_CARD.Graphic`
(CEngineGraphic) `modelId` (the 2nd u32 of the 17-byte CEngineGraphic; the 1st u32 = graphic-type 5).
**modelId is per-OBJECT data**, so the card body is selectable per card (in retail it's shared across
quest groups by type + alignment, not unique per quest).

### The card meshes (graphics.big / MBANK_ALLMESHES) + their textures
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

`CORE` = core quests, `VIN` = vignette; × alignment `GOOD/NEUTRAL/BAD`. Diffuse textures are **DXT3
256×256** in `textures.big` GBANK_MAIN_PC (the actual card illustration). Retail quest→card mapping: a
quest's OBJECT_QUEST_CARD.Graphic.modelId picks the mesh; e.g. WASP_MENACE + ARENA → 4397 (NEUTRAL
core), OAKVALE_INTRO + TEMPLATE → 7591 (scroll), PROTECT → 4396.

### Customizing the card body
- **RESKIN (data-only, done + verified):** replace the diffuse texture (e.g. id 1354) in textures.big →
  every card of that type/alignment shows the new face. Proven: AI-reskinned
  CORE_QUEST_CARD_NEUTRAL_01 (256×256 DXT3) swapped in clean. Tools: `tools/ai_texture.py` (generate) →
  `tools/texture_build.py replace` (by name) or a small id-based replace.
- **NEW card model (data — "can we add a new one?"):** YES. Add a new mesh to graphics.big
  MBANK_ALLMESHES (`big_write.rebuild(adds=)` / mesh_rw) + add its texture (`texture_build.py add`),
  then set a card OBJECT's `Graphic.modelId` to the new mesh id (in-place def edit, or on a
  from-scratch card; `forge ui set-cardmodel <OBJECT> <meshId>`). Because Graphic.modelId is
  per-OBJECT, this is **per-card and needs NO native code** — unlike the type-seal orb, whose 3 names
  are hardcoded in ConstructQuestList and would need a detour. So a fully custom card = from-scratch
  card def + custom Graphic.modelId → custom mesh → custom diffuse texture. In-game confirmed
  (MESH_HEADSPIDER on the quest-start popup, see above).
- Bump map (2nd tex) can be left as-is or regenerated for lighting.

### Anchors
- `graphics.big` MBANK_ALLMESHES (8112 meshes); `tools/parse_mesh.py --entry <mesh>` → matTexId[].
- `UI_QUEST_CARD_MODEL` game.bin 7546; `OBJECT_QUEST_CARD.Graphic` CEngineGraphic modelId (per-card).
- Card diffuse ids 1352-1354 (CORE) / 1348 (VIN) / bump 1355-1357 / 1351, textures.big GBANK_MAIN_PC.

---

## Orb art: type-seal sprite

The `UI_QUEST_SPRITE_CORE/OPTIONAL/VIGNETTE` sprites are the small **quest-TYPE SEAL / ORB MARKER**
(32×32, Height/Width=0, an overlay used in *two* render paths incl. the HUD/list), **NOT** the quest
card's main picture. `CQuestCardDef` has **no** texture field; the Logbook picks one of exactly **three
shared orb sprites** by a hard 3-way branch on the card's `IsCoreQuest`/`IsVignette` flags. So a
brand-new card automatically inherits a class-appropriate orb; giving it a **unique** orb is not a
data-only operation. In retail every quest of a class shares the same card model family + one of 3
type seals; only the TEXT varies per quest.

### The chain (proven end-to-end)
1. **Render-time selection** — `CTCInventoryQuests::ConstructQuestList @0x0061B610` (PC twin
   `ConstructQuestListPC @0x0061E6D0`) picks a sprite by a 3-way branch on `CTCQuestCard::IsCore()` + a
   vignette byte (`ghidra_out/questcard_render_decomp.c:2368-2481`):
   - `IsCore` → `UI_QUEST_SPRITE_CORE`
   - else `!vignette` → `UI_QUEST_SPRITE_OPTIONAL`
   - else → `UI_QUEST_SPRITE_VIGNETTE`
2. Each name → `NUISystem::CManager::CreateComponent(name)` → instantiates the **CUIDef** widget of
   that name (game.bin entries 8487 / 8488 / 8489).
3. The CUIDef supplies the texture via `States[UI_STATE_INACTIVE].GraphicIndex` (`ui.def:14551-14581`):
   CORE → `HUD_ORB_QUEST_CORE`; OPTIONAL/VIGNETTE specialise.
4. Those resolve to **textures.big / GBANK_MAIN_PC** ids **5892 / 5894 / 5896**
   (`big_entries.csv:14399-14404`, `RetailHeaders/pc/textures.h:3289-3294`). (`HUD_ORB_QUEST_FEAT` 5893
   and the `*_SMALL` variants 5895/5897 also exist but the card-list path does not use them.)

> Correction folded in: the `HUD_ORB_QUEST_*` string seen near the render fn feeding
> `CQuestManager::OnQuestStartScreenDisplayed` is **not** the texture path — that call is a pure
> `StringMap_FindNode` bool ("start screen already shown?"), not rendering. The real texture bind is
> `CUIDef.GraphicIndex`.

The exact selection branch:

```c
ppVar51 = ppStack_428;                              // <- the CTCQuestCard* being rendered
if (CTCQuestCard::IsCore(ppStack_428)) {
    CreateComponent(mgr, "UI_QUEST_SPRITE_CORE");   // -> CUIDef 8487 -> tex 5892
} else {
    piVar27 = *(int**)(ppVar51 + 0xc);              // card inner object
    cVar44  = *(char*)((int)piVar27 + 0x59);        // vignette discriminator byte
    if (cVar44 == 0) CreateComponent(mgr, "UI_QUEST_SPRITE_OPTIONAL"); // 8488 -> 5894
    else             CreateComponent(mgr, "UI_QUEST_SPRITE_VIGNETTE"); // 8489 -> 5896
}
```

The sprite is 100% determined by the **CUIDef name string** passed to
`NUISystem::CManager::CreateComponent`. Per-card art = make that string card-specific.

**The card's identity at the branch:** the `CTCQuestCard*` (`ppStack_428`). Its owning quest name is
`ScriptQuestName` at card `+0x28` (see `GetActiveQuestCardFromScriptName`). That name is the join key
to a ForgeFSE registry.

- Vignette discriminator = raw byte at `*(card+0xc)+0x59` (CONFIRMED from the decomp, not a named
  IsVignette() call) — the mapping is exactly `{IsCore, thatByte}`.

### The CUIDef location — game.bin, not frontend.bin
The quest-sprite CUIDefs are **normal top-level `game.bin` entries** (def class `UI` = `CUIDef`), NOT in
frontend.bin and NOT a nested UI-tree — the earlier "frontend.bin UI-tree" worry was a wrong premise
(retail `frontend.bin` holds only 811 dialog/table/generic-sprite UI defs; the quest UI lives in
game.bin):

| Entry | Name | indexInDefinition | size |
|------:|------|------:|-----:|
| 8487 | `UI_QUEST_SPRITE_CORE` | 2205 | 1355 B |
| 8488 | `UI_QUEST_SPRITE_OPTIONAL` | 2206 | 1355 B |
| 8489 | `UI_QUEST_SPRITE_VIGNETTE` | 2207 | 1355 B |

Each decodes **clean** as a `CUIDef` (109 fields, leftover 0). The three are **byte-identical except one
dword**: the texture id in `States[0].GraphicIndex`, which equals **5892 / 5894 / 5896** (the
textures.big GBANK_MAIN_PC orb ids) respectively — byte-verified.

**Wire location (tag-anchored, robust):** inside the `States` field value (`crc0("States")=0x87ACD3D8`)
→ `[u32 count=5]` → each `CUIStateDef` element is itself crc0-tag-anchored, so the first element is
`crc0("GraphicIndex")=0x38E36902` then the `u32` texture id. In these retail entries the value sits at
payload offset **103**, but the correct/robust way to find it is: locate the `States` tag, skip the
count, then find `crc0("GraphicIndex")` and overwrite the following `u32`.
(`crc0("PositionX")=0x1EDB8A31` follows, confirming the element layout.)

---

## Texture binding: data-only vs detour

### Can a new card get new orb art from scratch?
| Path | Result | How |
|------|--------|-----|
| **Unique per-card orb** | ✗ via data | No per-card `GraphicIndex`, no def texture field, no `SetQuestCardGraphic` API. Flag-selected only. |
| **Shared re-skin (all cards of a class)** | ✓ via data | Replace texture id 5892/5894/5896 in textures.big (`tools/texture_build.py`, `../formats/TEXTURE_WRITER.md`). Up to 3 distinct looks by repointing the 3 CUIDef `GraphicIndex` values (game.bin def edit — needs a States-aware setter, `forge ui set-graphic`). |
| **True per-card orb** | ✓ via engine patch | Detour `ConstructQuestList`/`…PC` to read a card-specific CUIDef name instead of the fixed constant. ForgeFSE hook on `0x0061B610`/`0x0061E6D0`. Reconstructable now that the branch is decompiled. |

(The card **body** mesh, unlike the orb, IS per-card via `OBJECT.Graphic.modelId` — see
[Body art](#body-art-3d-card-mesh).)

### Works TODAY without the detour (data-only)
- **Shared re-skin** of any card class: overwrite texture id 5892 (core) / 5894 (optional) / 5896
  (vignette) in `textures.big` GBANK_MAIN_PC via `tools/texture_build.py` (numeric-id target; match the
  existing DXT format/dims — TODO confirm). Every card of that class then shows the new art. This is the
  cleanest path because it swaps the texture the `GraphicIndex` already points at — it needs NO CUIDef
  edit.
- **3 distinct looks** by class: author cards with the desired `IsCoreQuest`/`IsVignette` flags
  (from-scratch CLI supports both) → each renders its class orb → re-skin the 3 orb textures
  independently → 3 authorable looks, zero native code. Or set each orb CUIDef's `States[0].GraphicIndex`
  to a custom texture id (inject the texture via `texture_build.py`); `defs set-field` only reaches
  top-level tags, hence the nested States-aware setter (`forge ui set-graphic`).
- The from-scratch card ships with the correct **class orb** (core/optional/vignette) chosen by its
  `IsCoreQuest`/`IsVignette` flags — a real, non-placeholder look — without any art work. Bespoke
  per-card orb art is a **separate ForgeFSE detour** feature (roadmap item), not a blocker for the
  content-first card.

### Per-card orb — implementation design (detour)
The 3 required pieces (all native / in-game-iterative):
1. **Custom texture** into textures.big — READY today (`tools/texture_build.py`,
   `../formats/TEXTURE_WRITER.md`; match the DXT format/dims of 5892 — TODO: confirm Info tail/mips/WxH).
2. **Custom CUIDef** whose `States[UI_STATE_INACTIVE].GraphicIndex` = the new texture id (e.g.
   `UI_QUEST_SPRITE_CUSTOM_<X>`). This is just a **new `UI`/CUIDef entry appended to game.bin** (clone
   entry 8487, set GraphicIndex to the custom texture, name it `UI_QUEST_SPRITE_<X>`) via the
   DEF_LOAD_CONTRACT append (crc0 + index) forge already supports. (The earlier "frontend.bin CUIDef
   writer" gap was a wrong premise.)
3. **ForgeFSE detour** at `0x0061B610` (+ `0x0061E6D0`): before the 3-way branch, read the card's
   `ScriptQuestName` (card+0x28), look it up in a ForgeFSE registry (`std::map<name, customCUIDefName>`
   populated from Lua, e.g. `Quest:SetQuestCardArt("<questName>", "UI_QUEST_SPRITE_CUSTOM_<X>")`); if
   found, `CreateComponent(mgr, customName)` and skip the stock branch; else fall through to retail.

**Recommended hook shape (safe, opt-in, matches ForgeFSE MapResourceAlias pattern):** install a
trampoline at the branch head but keep it **inert until a Lua opt-in** (like
`InstallMapResourceAliasHook` — installed always, disabled until `AliasMapResources`). The detour
trampoline: save regs → get card ptr (from the stack slot / EBX-relative it lives in at that PC) →
`name = *(CCharString*)(card+0x28)` → registry lookup → if hit, build the custom component and jump
past the stock selection; else execute the displaced instructions and continue. Exact displaced-byte
capture requires disassembling `0x0061B610` in the retail exe.

**Status:** mechanism + selection branch + card identity CONFIRMED. NOT YET IMPLEMENTED as a live
patch: the trampoline is a **mid-function** hook whose only verification is in-game (crash-on-error),
so it must be built against the disassembled displaced bytes and validated with the user's in-game
loop — not shipped blind.

---

## Empty-card fix

Custom quest card shows EMPTY objective/gold/renown values. **ROOT CAUSE — runtime-population gap, NOT a
def/schema bug** (engine decomp analysis, `quest_card_decomp.txt`, 137 fns; HIGH confidence).

The engine sources OBJECTIVE, GOLD and RENOWN from the **runtime per-quest card THING** (CTCQuestCard:
objective at +0x1c, gold at +0x2c, renown at +0x30) + the manager's runtime quest list
(DAT_013b89fc+0x6c) — written ONLY by the script setters **SetQuestCardObjective /
SetQuestCardGoldReward / SetQuestCardRenownReward**. Those first call
**GetActiveQuestCardFromScriptName** (resolves a card only if an ACTIVE card THING exists for that quest
script name, matched via card+0x28). `forge quest card` emits only the static `CQuestCardDef` + OBJECT
clone and **no companion quest/script**, so at display time no active card is populated and
`ConstructQuestList` (param_3==1) renders blank objective/reward.

**TITLE and SUMMARY DO come from the static def** (GetQuestName reads def+0x30, GetQuestSummary
def+0x2c) — so forge's QuestName/QuestSummary fields ARE load-bearing and render — UNLESS a
**zero/out-of-range TextID** was passed: `forge` writes `--quest-name/--quest-summary` verbatim with no
zero-guard and no symbol resolver, so `--quest-name 0` silently bakes a blank title.

### Fix
1. Do NOT change the def-write logic — the engine cannot read objective/gold/renown from the static
   def. The def+OBJECT `author()` emits is correct (title/summary/IsCore render from it).
2. Add a RUNTIME-REQUIRED note to `questcard.hpp` (l.28-36): baked objective/reward do NOT drive the
   Current-quests screen; they show only after a quest script calls the setters against an ACTIVE card.
3. Emit (or docs mandate) a **companion quest script**: AddQuestRegion -> AddQuestCard(
   objectName='OBJECT_QUEST_CARD_<new>', questScriptName,...) -> activate (ActivateQuestCard, or a quest
   between start/finish screens in its region so GetActiveQuestCardFromScriptName resolves non-null) ->
   SetQuestCardObjective/GoldReward/RenownReward(questScriptName, ...).
4. Guard the numeric TextIDs: in `apps/forge/main.cpp` asI32 for --quest-name/--quest-summary/
   --success-summary, reject 0/invalid ("TextID 0 renders blank; omit to inherit donor text or pass a
   valid text.big id"). Better: add a TEXT_* symbol resolver (CRC-hash -> text.big TOC).
5. Document that these flags take a NUMERIC text.big TextID (forge has no resolver yet); the safe default
   is to OMIT them and inherit the donor's text.

### Tooling status
- FQT's normal start-screen generator registers with `AddQuestCard`, activates the quest, then reapplies
  objective/gold/renown to the active runtime card.
- The generator no longer automatically follows registration with `GiveQuestCardDirectly`; that path
  creates an activation-coupled transient card and can kill it on failure.
- ForgeFSE names the direct-delivery argument `questCardObjectName` in its header and generated
  manifest (the former `textDBEntry` name was misleading; retail resolves an object definition there).

### Validation (byte-level, no in-game run)
`forge quest card <root> <schema> TESTCARD --donor OBJECT_QUEST_CARD_WASP_MENACE --gold 500 --renown 200
--core 1` then `forge defs decode <out> def_schema.json <idx>` and assert: clean decode (leftover 0),
GoldReward==500, RenownReward==200, IsCoreQuest==true, QuestName/Summary = the donor's valid ids
(non-zero). Confirms the def is written right; the emptiness is the runtime gap, fixed by the companion
script.

---

## Card-viewport garble fix

The custom quest-start card (Trial of the Emerald Spider / Melee) shows a rectangular **rainbow-static
block** behind the custom (hero-head spider) card mesh. Screenshot-confirmed: that block is an
**uninitialized render-target texture** — the card's 3D-model viewport RT. The mesh renders INTO the RT
but doesn't fill it, so stale VRAM shows. The backdrop tiles around it render fine.

**Fix = clear that RT before the mesh draws** (faithful: the engine SHOULD clear; the 3D mesh stays).
Needs the exact fn that binds the card viewport RT.

- The card is rendered ONCE at card-open then cached as a 2D texture → breakpoints set AFTER the card
  is up never fire. Attach BEFORE opening the card, arm BPs, THEN open/re-open it to catch the render.
  Re-view path (logbook vs re-trigger) not yet confirmed to re-render the same viewport.
- Choke-point addresses to BP (manifest-verified): `BlitRenderTargetSurfaceToTexture` 0x009BF350 (RT
  surface -> display texture = card thumbnail; strongest anchor), `SetRenderTarget` 0x009BF160,
  `ClearRenderTarget` 0x009BE420 (CDisplayManager) / 0x009D8CF0 (CRenderManager2D), `CMesh::Draw`
  0x005CA490 (card-only on the card screen since world isn't drawn), `InitialiseAsRenderTarget`
  0x009F3310.
- Ruled out (0 fires as hooks): UpdateDiskCacheTexture 0xB2D330, IsNearlyIdentical 0xB2B050,
  StartComposition 0xF39730, RenderToTexture — the composite/disk-cache path is NOT the card viewport
  path.
- Next: live-trace via the pybag debugger — attach at menu, BP BlitRenderTargetSurfaceToTexture +
  CMesh::Draw, open the Melee card, capture the backtrace, then hook the caller to clear the RT
  (`ClearRenderTarget` needs flags bit2=4=D3DCLEAR_TARGET; flags=0 is a no-op).
- `tools/card_studio.py` `expand_backdrop` honors `backdrop.align=[dx,dy]` (positive dx shifts the whole
  backdrop LEFT, oversizes source so no empty edge); horizontal-only ~80px left = `"align":[80,0]`.

---

## Customization tiers and tooling

1. **Distinctive quest — DATA-ONLY, highest impact:** the **region graphic**
   (`MINIMAP_<region>_FRONT_END`) on the VIEW/accept screen — set `RegionName` to the region whose
   illustration you want, OR replace/add the MINIMAP texture in frontend.big. Plus
   **Title/Objective/Reward** text (QuestName/QuestSummary TextIDs; objective/gold/renown via Lua
   `Quest:SetQuestCard*`). *(RegionName drives the `GiveHeroQuestCardDirectly` path; confirm your
   activation path honors it — the plain `AddQuestCard` path ignores RegionName.)*
2. **Class reskin — DATA-ONLY, affects every quest of that class+alignment:** replace card diffuse
   (`texture_build.py replace` id 1354 etc.) and/or orb textures (5892/5894/5896), or edit CUIDef
   8487-8489 `States[0].GraphicIndex` (`forge ui set-graphic`). A total-conversion aesthetic, not a
   one-quest standout.
3. **Custom card MESH — DATA, confirmed in-game:** add a mesh (graphics.big MBANK,
   `big_write.rebuild(adds=)`/mesh_rw) + its texture (`texture_build.py add`) +
   `forge ui set-cardmodel <OBJECT> <meshId>`. Changes both the world/pickup card and the UI card
   display (quest-start popup right panel).
4. **Quest-start window retheme — DATA-ONLY:** repaint the shared atlas 4722–4730 (PC) / 4783–4788
   (console) as described above.
5. **Per-quest UNIQUE orb — NATIVE:** ForgeFSE detour on `0x0061B610`/`0x0061E6D0` reading
   `ScriptQuestName` (card+0x28) → registry → `CreateComponent(customCUIDef)`. Not data.

**Tooling (built + verified headless):** `forge ui set-graphic` (orb/CUIDef GraphicIndex),
`forge ui add-sprite` (custom CUIDef), `forge ui set-cardmodel` (OBJECT Graphic.modelId),
`tools/texture_build.py add|replace|decode`, `tools/ai_texture.py` (generative),
`tools/parse_mesh.py --entry <MESH>` (mesh→matTexId), `tools/card_studio.py` (backdrop composition).
Consumers: FableForge `libs/forgecore/src/questcard.cpp` — `author()` (clone) / `overwriteDonor()` /
`authorFromScratch()`.

---

## Verified facts with addresses

| Item | Value |
|---|---|
| `CGameScriptInterface::AddQuestCard` | vtable[292] `0x490`, `0x008913F0` → `FUN_004b1670` (guild list `this+0x58`, dirty byte `+0x8F`) |
| `CGameScriptInterface::GiveHeroQuestCardDirectly` | vtable[295] `0x49C`, `0x008968C0` |
| `CQuestManager::ActivateQuestCard` | `0x004b4aa0`; helper `FUN_004b4a10(card+0x28, …)` → `ActivateMultipleQuests` |
| `CTCInventoryQuests::ConstructQuestList` / PC twin | `0x0061B610` / `0x0061E6D0` (orb 3-way branch; `param_3==0` list, `==1` current) |
| `CTCQuestCard` fields | `+0x1c` objective, `+0x28` ScriptQuestName, `+0x2c` gold, `+0x30` renown; vignette byte `*(card+0xc)+0x59` |
| `CQuestCardDef` fields read at draw | `def+0x30` QuestName (GetQuestName), `def+0x2c` QuestSummary (GetQuestSummary) |
| Runtime quest list | `DAT_013b89fc+0x6c` |
| Orb CUIDefs (game.bin) | 8487/8488/8489 `UI_QUEST_SPRITE_CORE/OPTIONAL/VIGNETTE` (indexInDefinition 2205-2207, 1355 B) → tex 5892/5894/5896; GraphicIndex at payload offset 103 |
| Card model container | `UI_QUEST_CARD_MODEL` game.bin 7546 (200×200, MeshType=5, GraphicIndex=0) |
| Minimap template | `UI_QUEST_MENU_MINI_MAP_TEMPLATE` game.bin 7501 |
| Quest-start atlas containers | `PC_UI_QUEST_START_FRAME` 6822 (crc `0x026E56D9`, children 6856–6864 → tex 4722–4730); `UI_BACKDROP_QUEST_START` 8345 (crc `0x06ED93B9`, children 8346–8351 → tex 4783–4788) |
| Card meshes | 4395–4400, 7591 (graphics.big MBANK_ALLMESHES) |
| Card textures | diffuse 1352/1353/1354/1348, bump 1355/1356/1357/1351 (textures.big GBANK_MAIN_PC, DXT3 256×256) |
| crc0 tags | `States` `0x87ACD3D8`, `GraphicIndex` `0x38E36902`, `PositionX` `0x1EDB8A31`, `RegionName` `0x6FE0488D`, `IsCoreQuest` `0x3995EBC1`, `IsVignette` `0x3C9E385B` |
| Render-target choke points | `BlitRenderTargetSurfaceToTexture` `0x009BF350`, `SetRenderTarget` `0x009BF160`, `ClearRenderTarget` `0x009BE420` / `0x009D8CF0`, `CMesh::Draw` `0x005CA490`, `InitialiseAsRenderTarget` `0x009F3310` |
| Ruled-out (not card viewport) | `UpdateDiskCacheTexture` `0xB2D330`, `IsNearlyIdentical` `0xB2B050`, `StartComposition` `0xF39730` |

---

## Dated findings
- 2026-07-21 — Empty-card root cause pinned from `quest_card_decomp.txt` (137 fns): runtime-population
  gap, not def/schema.
- 2026-07-22 — FQT start-screen generator switched to AddQuestCard → activate → setters; ForgeFSE arg
  renamed `questCardObjectName`. Wasp Menace rendered via ForgeFSE with 1337 gold / card art.
- 2026-08-16 — `quest-card-re-prereqs` workflow: orb chain proven end-to-end; CUIDef location solved
  (game.bin 8487-8489, not frontend.bin); byte layout measured.
- 2026-08-16 — `quest-card-display-contexts` workflow (4 parallel context RE agents + synthesis):
  per-screen display map; body art = OBJECT.Graphic.modelId; first in-game reskin run confirmed card
  model / title / orb, refuted 5515 + SUMMARY-line claims; quest-start hero/backdrop atlas RESOLVED
  (4722–4730 / 4783–4788).
- 2026-08-16 — Rainbow-static garble behind the custom card mesh identified as an uninitialized
  card-viewport render target (fix = clear RT; render fn still to be pinned via live trace).

## Superseded claims
Kept so old notes can be reconciled; each is now WRONG:
- "`OBJECT_QUEST_CARD` has no `Graphic`/`modelId` field" — it does; `Graphic.modelId` is per-OBJECT and
  drives the card mesh in both world and UI.
- "The card body is a shared model; only text varies per quest" — model varies per OBJECT (4397 / 4396 /
  7591 across retail cards).
- "The LEFT region graphic (`MINIMAP_<region>`) dominates the quest-START screen" — the start screen's
  left panel is the baked hero sprite 4722/4783; the minimap drives the single-card VIEW screen only.
- "LEFT panel = textures.big 5515" — wrong id, reskin had no effect.
- "Editing `TEXT_QUEST_*_SUMMARY` changes the start-screen objective line" — that line is runtime
  (`SetQuestCardObjective`).
- "Custom orb CUIDefs need a frontend.bin CUIDef writer" — they are game.bin entries.
- "Whether the UI card display honors `Graphic.modelId` is only inferred" — confirmed in-game.

## Open questions
- DXT format/dimensions of orb textures 5892/5894/5896 for byte-exact custom replacements (Info tail /
  mips / WxH).
- The exact fn that binds the card-viewport render target (for the garble clear); whether re-viewing a
  card from the logbook re-renders the same viewport.
- Per-card orb detour: displaced-byte capture at `0x0061B610`/`0x0061E6D0` and in-game validation.
- Which runtime symbol the start-screen objective line is set from for the retail training quests.
- `InventoryCategory` id for the "Current Quests" tab (from-scratch default still inherits a literal).
- Remaining 458 B OBJECT wrapper bytes (0–76, 89–457) not fully reversed (see byte-layout doc).

## Evidence
- Ghidra headless of retail Fable.exe funcs 0x008913F0 / 0x004b1670 / 0x008968C0 / 0x004b4aa0 /
  0x004b4a10; `ghidra_out/questcard_render_decomp.c:2250-2302, 2368-2481`;
  `ghidra_out/quest_card_decomp.txt:8744-8754`.
- ForgeFSE bindings GameInterface.cpp:1223-1226, LuaQuestState.cpp:269,278; official FSE
  `MyFirstQuest.lua` (`AddQuestCard` usage); def schema chocolatebox_def.xml:2936.
- `RetailHeaders/pc/textures.h:3289-3294`, `big_entries.csv:14399-14404`,
  `compiled_game_entries.csv:10194-10196`, `ui.def:14551-14581`.
- Screenshots: `work/runtime_smoke_quest_terrain_20260722/captures_native_run4/user_wasp_card_1337.png`;
  `…/heroprobe/verify_*.png`.
- [QUEST_CARD_BYTE_LAYOUT.md](../formats/QUEST_CARD_BYTE_LAYOUT.md), `../formats/TEXTURE_WRITER.md`,
  `../formats/FRONTEND_FORMAT.md`, `../formats/DEF_LOAD_CONTRACT.md`, FableForge `questcard.hpp:28-102`.

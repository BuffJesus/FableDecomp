# FRONTEND / GUI data — formats, asset chain, editability

*Empirical pass 2026-07-20 (Ghidra lock held by another agent — no new decompilation used;
engine-behaviour claims below are data-side evidence only). Install READ-ONLY; all
experiments staged under `work/frontend_re/`.*

## 1. Where the frontend lives

| File | Format | Contents | Parses today with |
|---|---|---|---|
| `data\CompiledDefs\frontend.bin` | CompiledDefs (same container as game.bin/script.bin) | **810 entries** = the whole menu system: UI widget tree (785 `UI`/CUIDef), 10 `CONTROL_SCHEME`, 5 `ENGINE_VIDEO_OPTIONS` (LOW/NORMAL/HIGH/HIGHEST + null), and 2 each of `FRONT_END`, `UI_MISC_THINGS_DEF`, `UI_ICONS_DEF`, `ENGINE`, `CONFIG_OPTIONS_DEFAULTS_DEF` | `forge defs list/show/decode` (100.00% clean), `tools/parse_frontend.py` |
| `data\CompiledDefs\game.bin` | CompiledDefs | HUD defs: `PLAYER_GUI_DEFAULT` (#7074), **`PLAYER_GUI_PC` (#7075, 18,148 B)** = CPlayerGuiDef (463 fields in schema — the "432" figure is stale), plus `CQuickAccessItemButtonGuiDef`, `CAbilityButtonGuiDef` | same |
| `data\graphics\pc\frontend.big` | BIGB v100, 1 sub-bank `GBANK_FRONT_END_PC` (v122, align 2048) | **394 texture entries** (type 0, Info=34 B — identical texture descriptor to textures.big): menu backdrops, title sprites, keyboard, credits BGs, button sprites | `parse_bigb.py` CLEAN, `parse_texture.py` (explicitly supports it) |
| `data\graphics\pc\textures.big` | BIGB | sub-bank 0 `GBANK_GUI_PC` (34 entries, editor icons); sub-bank 1 `GBANK_MAIN_PC` (6,290) holds the **in-game HUD art**: `HUD_HEALTH_BAR_*` (5553–5565), `UI_*` sprites, minimap icons | `parse_texture.py`; writer `texture_build.py` |
| `data\lang\English\fonts.big` | BIGB v100, 3 sub-banks: `FONT_ENGLISH_MAIN` (8), `STREAMING_FONT_ENGLISH_PC` (8 + 25.9 MB `GlyphData`), `STREAMING_FONT_ENGLISH_XBOX` | 8 fonts (`ENG_TAHOMA_10/12`, `ENG_ARIAL_12/16/18/24/28`, `ENG_TIMES_NEW_ROMAN_16`; enum in `RetailHeaders\fonts.h`). Payload = face name ASCIIZ ("Arial") + metrics + glyph tables — **payload format NOT cracked** | container: `parse_bigb.py` CLEAN; payload: nothing yet |
| `data\lang\English\text.big` | BIGB (TEXTBIG) | all GUI strings (`TEXT_GUI_*`) | already read/WRITE (proven earlier) |
| `data\Misc\LoadProgress\*.dds` | plain DDS | loading-screen guild seals | any DDS tool (loose files!) |
| `data\Defs\RetailHeaders\pc\front_end_bank.h`, `gui_bank.h`, `textures.h`, `particles.h`, `fonts.h` | C headers (BankCreator output) | **the ID→name answer key** for every graphic/effect index in the defs | text |

No other `*frontend*` container exists in the install; there is no separate "gui textures"
bank beyond the three above.

## 2. Container formats — nothing new needed

- `frontend.bin` is the **already-cracked CompiledDefs format** (FableForge
  `libs/forgecore/include/forge/bin.hpp`; header magic region carries the PC names magic
  `0xA8E36C34`): 9-byte header, u32 entryCount @9, entryCount×{defNameOff, nameOff,
  indexInDefinition} @13, chunk table with (entryCount,totalCompressed) sentinel + one
  trailing u32, zlib chunks ≤32 KiB inflated, each starting with u16 payload offsets.
  Names resolve through the shared `names.bin`.
- Payload encoding is the tagged-field scheme (tag = CRC32 poly 0xEDB88320 seed 0
  no-invert of the field name; serialization order = Transfer() call order in
  `def_schema.json`). **810/810 frontend.bin and 14,761/14,761 game.bin entries decode
  100% clean** under the schema (`forge defs decode --all`).
- `frontend.big` / `fonts.big` are ordinary BIGB banks — same family as
  textures/graphics/text; `parse_bigb.py` reports CLEAN on both.

New tool: **`tools/parse_frontend.py`** — pure-python CompiledDefs reader/writer +
UI-specific views:

```
python tools/parse_frontend.py [root] [--bin game.bin] list|tree|survey|entry <name>
python tools/parse_frontend.py [--bin game.bin] patch <outdir> <entry> <Field> <hex>
```

`tree` prints the widget hierarchy; `patch` does a field-level edit and rebuilds the bin
(write path mirrors forge `bin::File::save`; validated below).

## 3. The UI widget tree (frontend.bin)

`UI` entries are **CUIDef widgets forming a real scene graph**: `Children` =
Vector_int32 of **global frontend.bin entry indices** (222 roots; e.g. `UI_DIALOG_YESNO`
→ `UI_YESNO_TABLE`/`UI_YESNO_YESBUTTON`(→`UI_SPRITE_A_BIG`, `UI_YESNO_YESTEXT`)…).
Key CUIDef fields (109 total):

- `Type` (EType_NUISystem: 0 sprite, 2 table, 4 dialog, 5 button, 6 text, 11 editbox,
  25 yes/no dialog, 30/31 lists, 37 editbox template …)
- `TextValue` (CWideString) — either a **literal** (`'Yes'`, `'Are you sure?'`) or a
  **text.big symbol** (`'TEXT_GUI_MENU_CONTINUE_GAME'`) resolved at runtime. This is the
  join to the already-writable text.big.
- `States` = Vector_CUIStateDef — per-state `GraphicIndex`, `PositionX/Y`, `ZoomX/Y`,
  `ColourR/G/B/A`, `UpdateTime`, transition type. **Widget position/colour/scale lives
  here**, not on the widget itself.
- Geometry/behaviour: `Width/Height`, `Layer`, `Angle`, ~17 `ActionOn*` enums
  (EActionType_NUISystem — action *numbers*; handlers are compiled code), `Font`
  (CDefString name → fonts.h/fonts.big), `TextBankIndex`, `MovieFilename`,
  `ViewArea*` (640×480 design space), `PCStyle`, sprite maps + separations for tables.

Graphic-index namespace is **per bank context** (verified by cross-referencing headers):
- frontend widgets: `UI_FRONTEND_BG_01` state GraphicIndex 14 == `FRONTEND_BG_01_SPRITE`
  = 14 in `front_end_bank.h` → **frontend.big** entry id 14; dialog sprite 87 ==
  `UI_TABLE_TEST_TL_FE`, A-button 60 == `HUD_ABXY_BIG_A_FE`.
- HUD defs (game.bin): ids match `textures.h` → **textures.big GBANK_MAIN_PC** entry ids.
- effect ids match `particles.h` (e.g. 816 = `GUI_HEART_BEAT_NORMAL`).

`UI_MISC_THINGS_DEF_INSTANCE` (133 fields) = global GUI strings/separators + layout
scalars (`RingCenterX/Y`, `PCRingCenterX/Y`, `WorldMapOffsetX/Y/Width/Height`, spell
palette counts). `UI_ICONS_DEF_INSTANCE` (32 fields) = icon id table (textures ids).
`FRONT_END_DEFAULT` = attract-movie list + error-background/button graphic ids.

## 4. HUD: CPlayerGuiDef (game.bin `PLAYER_GUI_PC`)

463 schema fields, decodes clean. It is the entire HUD layout database — for the health
bar alone: `HealthBarPos` (C2DVector, retail **(-9, -9)**), left/middle/right/heart/
inner/end graphic ids (5553–5565 = `HUD_HEALTH_BAR_*` in textures.h → textures.big),
good/evil heart variants, effect ids (particles.h `GUI_HEART_*`), low-health threshold
(0.2), `HealthBarResurrectionColour` (CRGBColour dafd35ff), full stamina-bar mirror
group. Similar groups exist for mana, experience, mini-map, quick-access slots, etc.
`PLAYER_GUI_DEFAULT` is the Xbox/gamepad variant.

## 5. Worked examples (both staged + machine-verified, engine test pending)

**A. Move the health bar** (`work/frontend_re/healthbar_move/`):
```
python tools/parse_frontend.py --bin game.bin patch \
  work/frontend_re/healthbar_move/data/CompiledDefs \
  PLAYER_GUI_PC HealthBarPos 00009643000010c1     # (300.0, -9.0)
```
Validation: `forge defs decode <staged-root> ... PLAYER_GUI_PC game.bin` shows the new
vector; `--all` = 14,761/14,761 clean; corpus diff vs retail = **exactly one payload
differs**. Deploy = copy staged `game.bin` over the install copy (names.bin unchanged).

**B. Reword a dialog** (`work/frontend_re/yesno_text/`): patched `UI_YESNO_TEXT.TextValue`
→ `"FORGE: sure?"` (variable-length edit, 945→943 B payload); 810/810 clean, single-entry
diff. (Menu text is usually better edited in text.big since most widgets reference
`TEXT_GUI_*` symbols; direct TextValue patching covers the hardcoded literals.)

**C. Main-menu background** (no new tooling needed): `FRONTEND_BACKDROP_01` = frontend.big
entry id 1, DXT1 640×480 (alloc 1024×512), 8 mips — same texture pipeline as textures.big,
so `tools/texture_build.py` replace applies as-is.

## 6. Editability today vs needs work

| Thing | Today |
|---|---|
| HUD layout/colors/thresholds (CPlayerGuiDef) | **EDITABLE** — parse_frontend.py patch (proven) or `forge defs merge` mod-roots |
| Menu structure/behaviour (frontend.bin widgets) | **EDITABLE** at field level (positions, states, colours, text, graphic ids, children vectors). Adding NEW widgets = `bin::File::addEntry` exists in forgecore; engine acceptance of new entries **[unproven]** |
| Menu/HUD textures (frontend.big, textures.big) | **EDITABLE** — texture_build.py |
| GUI text | **EDITABLE** — text.big writer (proven live earlier) |
| Loading screens | **EDITABLE** — loose DDS |
| Fonts (fonts.big payloads) | container parses; glyph/metric payload format **not cracked**; `GlyphData` blob (25.9 MB) untouched |
| Action semantics (`ActionOn*` enum values), screen-flow state machine | compiled code — data shows numbers only |

None of today's edits have had an **in-game smoke test** (game not launched per scope);
both staged CompiledDefs edits satisfy every static invariant forge checks.

## 7. Ghidra follow-ups (lock currently taken)

1. `frontend.bin` loader + `EType_NUISystem` / `EActionType_NUISystem` enum decode —
   confirm widget-type numbers and map action ids to handler functions (search
   `NUISystem` strings; FSE manifest has UI-adjacent APIs).
2. GraphicIndex bank-resolution rule (which CGuiBank a widget context binds — evidence
   above is header-id matching only).
3. Fonts: `FONT_ENGLISH_MAIN` payload struct + streaming-glyph lookup (`fonts.h`,
   "BankCreator", face-name string are good anchors); needed for adding font sizes or
   full charset swaps.
4. Whether the engine tolerates *added* frontend.bin/game.bin entries (addEntry write
   path exists but is engine-[unproven]) and whether entry order constraints exist
   beyond "entry order = runtime def index".
5. `CUIDef::Sprites` Map payload (Map_JW4ETableSprites) inner layout — currently treated
   as opaque bytes (safe for round-trip, not for authoring new tables).

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
| `data\lang\English\fonts.big` | BIGB v100, 3 sub-banks: `FONT_ENGLISH_MAIN` (8), `STREAMING_FONT_ENGLISH_PC` (8 + 25.9 MB `GlyphData`), `STREAMING_FONT_ENGLISH_XBOX` | 8 fonts (`ENG_TAHOMA_10/12`, `ENG_ARIAL_12/16/18/24/28`, `ENG_TIMES_NEW_ROMAN_16`; enum in `RetailHeaders\fonts.h`). Static face/style metadata, glyph metrics/UVs, and embedded RGBA TGA atlases are decoded. The PC streaming path's packed Unicode banks, shared LZO metric table, and 32x32/64x64 alpha blocks are decoded too. | container: `parse_bigb.py` CLEAN; static render: `tools/render_fable_static_font.py`; streaming Unicode render: `tools/render_fable_streaming_font.py` |
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

`tools/parse_frontend.py entry` now decodes `Vector_CUIStateDef_NUISystem`
into named state dictionaries instead of opaque hex. This directly proves
the title composition used by the runnable checkpoint: `UI_TITLE` is at
`(70,30)`, `UI_TITLE_01` uses graphic 3 at `(0,0)`, and `UI_TITLE_02` uses
graphic 4 at `(256,0)`.

`CDefString` values are names.bin-relative u32 offsets, not inline strings.
Resolving them correctly proves that both `UI_PRESS_START_TEXT` and
`UI_LEGAL_TEXT` select `ENG_ARIAL_24`. The press-start widget is centered at
`(320,240)` and resolves through text.big to
`Press Left Mouse Button To Continue`. The runnable checkpoint now renders
that line from the retail static-font glyph atlas and its exact advance/UV
metrics; it is not a host Arial approximation. Retail-reference comparison at
1280x720 also proves that frontend text uses a two-thirds atlas-geometry scale
while the 640x480 UI view itself stretches independently across the active
width and height.

`UI_LEGAL_TEXT` resolves to the same named font and the localized
`TEXT_GUI_MENU_LEGAL` string. The runnable checkpoint uses the PC streaming
metadata and shared `GlyphData` payload to render all 144 characters, including
the copyright symbol at `U+00A9`, centered at `(320,340)` and wrapped to its
decoded 420-pixel text window. The packed Unicode lookup has 1,024 banks;
one-based 24-bit glyph indices select 25,176 shared metrics, whose packed
memory offsets address 32x32 or 64x64 alpha blocks in the 25.9 MB payload.
ASCII glyphs retain the narrower matching static-atlas metrics and the
streaming path supplies Unicode fallback; this produces the retail three-line
legal wrap. Both text layers receive the observed one-design-pixel dark edge,
and the PBGRA composition is converted back to straight alpha before D3D9
blending so antialiasing is not multiplied twice.

The press-start root's `UI_MOUSE_POINTER` child uses graphic id 362, which
cross-references `MOUSE_POINTER_SPRITE_FE` in `front_end_bank.h`. The decoded
32x32 ARGB sprite is compiled as the window's cursor with hotspot `(0,0)`;
this preserves the retail pointer appearance and motion instead of exposing
the host arrow.

The press-start background subtree is also mapped. `UI_BLENDING_BACKGROUNDS_FORREST`
owns random swaps between four full backgrounds (8-second alpha transitions,
except the fourth at 2 seconds) and three sunbeam overlays (2-second
transitions). Each frame is six 256-pixel tiles laid out at `(0,0)`,
`(256,0)`, `(512,0)`, `(0,256)`, `(256,256)`, `(512,256)`: graphic ids
206–229 for the four backgrounds and 230–247 for the sunbeams.
`tools/render_fable_frontend_animation.py` now decodes all 42 entries in one
bank pass, crops the right/bottom edge tiles to the 640x480 view, and builds
four-frame and three-frame texture sheets. The runnable D3D9 checkpoint
selects non-repeating randomized targets and alpha-blends them with the decoded
8/8/8/2-second and 2-second durations. Its focused seam test and changing
window-hash smoke pass.

The next screen is decoded and live too. `UI_FRONTEND_PRESS_START_MENU`
(#620) contains list #624 and invisible PC button #625; the button's `Action`
and `ActionOnLeftUnclicked` fields are both 229. The resulting
`UI_FRONTEND_MAIN_MENU` (#212) owns list #242, title #200, and coastal
background controller #685. List #242 begins at `(200,200)` and contains seven
buttons at y offsets `0,30,60,120,180,210,240`. Their text children add
`x=120`, centering every label at design-space x=320. Continue Game uses
`UI_BUTTON_BIG` (400 pixels, x offset -140); the remaining rows use
`UI_BUTTON` (280 pixels, x offset -80).

The runnable uses those values without a host-layout approximation.
`tools/render_fable_frontend_menu.py` decodes the title, `ENG_ARIAL_24`, and
`TS_BUTTON_L/M/R` into a seven-frame selection sheet.
The `-140/-80` values are table-local component states, not final flat-surface
left edges. The retail `CTable` path generates its ornament pieces before
`Draw` adds the owning component position. The flat compositor therefore
materializes the 400- and 280-pixel ornaments at x=120 and x=180,
respectively, so both share the compiled text-child center x=320. Options uses
the same 280-pixel x=180..460 result. Native hover/click rectangles use those
same final bounds. A focused geometry test locks both widths to the shared
center.

This is currently a visual/behavior parity boundary, not an object-code parity
claim: the serialized records, sprites, text metrics, and final geometry are
retail-derived, while the full 1,399-byte Fable `CTable::Draw @ 0x00550DC0`
and its construction chain have not yet been promoted into the compiled
reconstruction.

The retail body now confirms the key local/final-coordinate rule directly.
`CTable::Draw` adds the table's local `+0x34/+0x38` position to its parent,
stores the resulting ordinary and true render origins, then adds every
materialized child's local position (with the frontend UI scale) before
drawing it. `CTable::Initialise @ 0x00550C60` invokes virtual slot `0x238`;
the retail `0x00551EA0` body walks sprite keys 0 through 12 and calls the
horizontal (`0x005518E0`) and vertical (`0x00551BC0`) line constructors.
Those bodies are bounded decomp evidence for the current composition and the
next promotion targets; they are not yet compiled parity claims.

Layout evidence is split across four retail sources:

- `frontend.bin` supplies component hierarchy, parent/local positions, sizes,
  text alignment, list offsets, table definitions, actions, and graphic ids.
- `frontend.big` supplies the exact source pixels and dimensions for every
  decoded frontend sprite.
- `text.big` and `fonts.big` supply localized strings, glyph metrics, UVs,
  alpha pixels, wrapping inputs, and font selection.
- `Fable.exe` supplies the runtime interpretation of those records:
  `CTable` materialization, `CList` offset propagation, hover/click dispatch,
  screen transitions, and generated controls.

The first three sources are decoded for the live frontend path. Executable
recovery is incremental, so a layout is only described as exact-code-backed
after its relevant runtime functions are promoted and object/behavior gated.
The remaining high-value gap for these menus is the `CTable`/`CList`
materialization chain; the current final-coordinate compositor is
retail-data-backed and visually gated but authored.

The build must source those pixels from an untouched install bank.
`work/ui_proto/art/frontend.big` is deliberately not a retail oracle: its
quest-card authoring experiment replaces exactly
`FRONTEND_BUTTON_L_SPRITE` with a gold version. The checkpoint now prefers
the read-only installed `frontend.big` and keeps that staged bank as a
last-resort development fallback, preventing that modified slice from leaking
into selected-button tables.

`UI_TABLE_TITLE_WHOLE` is not a selected-button table. Its decoded sprite map
assigns TOP_LEFT, TOP_RIGHT, and HORIZONTAL_TOP to the same
`UI_TEXTBOX_MIDDLE` component (#122), whose installed
`UI_TEXTBOX_MIDDLE_FE_SPRITE` is a gold-edged, blue-filled 8-pixel segment.
Retail `CTable::ConstructSpritesToDraw @ 0x00551EA0` treats the declared width
as the zoomed inner span and places natural-size corners around it. The title
builder now follows that exact inner-span-plus-corners rule; the 640-pixel
viewport clips the outer tail. All eight columns of this installed segment are
identical, so the result remains one continuous gold-edged/blue-filled rule.
The earlier `FRONTEND_BUTTON_L/M/R_SPRITE` substitution produced ornate blue
ends from an unrelated component and was not definition-faithful.

The shared title rule exposed the same local/final distinction. Options,
Gameplay, Audio, Video, and Redefine serialize their title text child at
`(65,44)` with left alignment, but treating that child state as the final
surface coordinate left every title visibly stranded at the rule's left end.
The checkpoint now preserves y=44 and the decoded font/outline while resolving
all five final title anchors to the 640-pixel rule center x=320. The Options
submenu and every detail frame call the same `_draw_title` path, and a focused
test locks `(320,44)` as the final header-text coordinate. This remains part of
the authored flat-composition boundary until the generated children and text
components are emitted live rather than baked into a sheet; the table's corner
and edge-cursor geometry itself is now promoted.

The build also embeds an explicitly non-parity `buff-jesus` text variant.
`--buff-jesus` selects a second seven-frame sheet whose row offsets, ornament
sizes, x=320 centers, retail font, title, animation, hitboxes, and actions are
unchanged. Only the displayed main-menu strings differ. The no-argument path
continues to use the retail-localized sheet. A normal `--buff-jesus` launch
also follows the recovered Lionhead/Microsoft/intro boot-movie sequence before
revealing that sheet; `--skip-boot-videos` is the explicit direct-frontend
developer override.
`tools/render_fable_frontend_animation.py --theme coastal` stitches the four
opaque and three sunbeam states selected by #685. `WM_LBUTTONUP` activates
the screen; `WM_MOUSEMOVE` maps the stretched client back into the 640x480
view before testing the compiled row rectangles.

The first two screen-flow branches are now code-backed as well as data-backed.
Retail `CFrontEndManager::Action @ 0x0059A238` maps action 297 (`0x129`) to
used key `0x18` and action 314 (`0x13A`) to key `0x1A`; both continue through
`GotoNextScreen(..., false)`. `CFrontEndManager::Init2 @ 0x00598A1C` binds
those keys to `UI_FRONTEND_OPTIONS_SUB_MENU` (#211) and
`UI_FRONTEND_QUIT_PROMPT` (#631).

Options list #219 is rooted at `(200,150)`, has a 30-pixel vertical offset,
and contains actions 9/13/12/283 for Gameplay/Video/Audio/Redefine Keys. Its
title, horizontal rule, forest controller, row ornaments, and Back button all
come from the compiled tree. Quit root #631 deliberately has no background:
the recovered `GotoNextScreen @ 0x00596763` filter path leaves the prompt over
black. Its No and Yes controls use action 86 (`GotoPreviousScreen`) and action
296 (main-component quit) respectively.

The four Options-row destinations are now recovered from the same retail
dispatcher and `Init2` map: action 9 uses key `0x01`
(`UI_FRONTEND_OPTIONS_MENU`), action 12 uses `0x04`
(`UI_FRONTEND_AUDIO_OPTIONS_MENU`), action 13 uses `0x05`
(`UI_FRONTEND_SCREEN_VIDEO_OPTIONS_PC`), and action 283 (`0x11B`) uses
`0x16` (`UI_FRONTEND_SCREEN_REDEFINE_KEYS_PC`).

`tools/render_fable_frontend_subscreens.py` decodes
`TS_BUTTON_L/M/R`, `FRONTEND_BUTTON_L/M/R_SPRITE`, the `FE_BUTTON_*` states,
the slider bar/knob/arrow sprites, slot pieces, and `ENG_ARIAL_24` into an
eight-frame Options/detail sheet plus Back/No/Yes helper states. The Options
sheet is 1024x3840: full 640x480 frames occupy its left side and 124 compact
200x30 control states occupy a transparent atlas on the right. Nine generated
Redefine rows using `FE_SLOT_TEST_L/M/R_ON` occupy a sixth 640x480 helper-sheet
frame as complete 588x35 strips. Keeping each strip intact lets D3D9 replace a
row with one quad, avoiding filtering seams while retaining safe texture sizes.

The deeper roots are live. Gameplay root #337 has ten rows at list #343;
Audio root #334 has three volume rows at #336 and selects coastal controller
#685; Video root #239 has ten rows at #220; Redefine root #238 uses generated
list #217 with 31-action `ActionMap`/`ActionOrder`, 26-pixel row spacing, and
250-pixel viewport height. The first Redefine page's bindings are decoded
from the 123 records in `FABLE_PC_CONTROL_SCHEME_GDD_WASD`.
Action 60 is not one displayed row: `CRedefinerList::RefreshScriptThings
@ 0x00556A40` expands its movement container into four `CKeyRedefiner`
children. `GetMovementActionText @ 0x00558170` maps subtypes 10–13 to Move
Forward, Move Back, Move Left, and Move Right, while the WASD scheme supplies
W, S, A, and D respectively. The first complete nine-row viewport is therefore
those four movement rows followed by Attack, Block, Flourish, Run, and Toggle
First Person Targeting. Interact and the unsheathe actions are below that
viewport, not substitutes for the generated movement children.
The two reset helpers are also decoded rather than decorative:
`UI_RESET_WASD` is action 311 at local `(0,385)` and dispatches
`ResetAssignedInputsWASD @ 0x00408820`; `UI_RESET` is action 284 at
`(320,385)` and dispatches `ResetAssignedInputs @ 0x004085F0`. The checkpoint
keeps the resulting W/S/A/D or Up/Down/Left/Right values inside the same
Apply/Cancel transaction as manual key capture. Their complete 320x64 retail
ON tables and centered `ENG_ARIAL_24` labels are packed into the unused tail
of the Redefine hover atlas and replace the matching OFF button on pointer
entry.
The generated row geometry is now materialized directly from the component
records: list origin `(40,115)`, table offset `(-32,-2)`, right slot offset
`(368,-3)`, and action/key text offsets `(0,3)`/`(380,3)`. Both dynamic text
children use `ENG_ARIAL_12`, not the screen title's `ENG_ARIAL_24`. The
invisible hover child is `600x24` at `(-40,0)`, leaving the serialized
two-pixel gap between successive 26-pixel list rows.
`CText::Draw @ 0x0054EF00` reads and rounds the component x/y render origin;
its alignment branch changes only x. There is no separate runtime vertical
centering adjustment. Consequently the serialized `+3` y origin plus the
retail `ENG_ARIAL_12` glyph offsets is the key/action baseline oracle, rather
than a visually estimated center of the 35-pixel slot texture.
The two 22-byte `CKeyRedefiner::OnHovered` routines at `0x00557860` and
`0x00557880` both call `CClickable::OnHovered` and then dispatch virtual slot
`0xC0` with state 3 or 4. The checkpoint mirrors those entry/exit states: the
full 588-pixel compiled row region selects the retail ON slots under the
pointer and restores OFF when the pointer leaves.
`CONFIG_OPTIONS_DEFAULTS_DEF_INSTANCE` seeds Video at 1024x768, AA off, and
1/3 detail. Recovered profile methods seed Gameplay at sensitivity 0.5 and
opacity 1.0, and Audio at music 0.6, sound 0.8, and dialogue 0.9.

The generic manager, rather than `CFrontEndManager::Action`, dispatches the
control actions. Text/numeric setters apply the in-memory profile immediately;
action 87 saves and returns; actions 324/325/326 load Gameplay/Video/Audio
defaults and refresh active sliders. The checkpoint snapshots values on entry,
composes the selected atlas tile for every row, reverts on Cancel/Escape, and
commits on Apply.

The runnable consumes those sheets with compiled hit rectangles.
`smoke_visual_checkpoint.ps1 -VerifySubscreens` gates all four destination
actions, live mutation, Cancel rollback, Apply persistence, Defaults,
Escape/action-86 returns, Back/No, and graceful Yes exit.

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
| Fonts (fonts.big payloads) | static `FONT_ENGLISH_MAIN` and PC streaming Unicode metadata/`GlyphData` both parse and render with retail metrics and pixels; Xbox streaming rendering and write/rebuild paths remain |
| Action semantics (`ActionOn*` enum values), screen-flow state machine | actions 9, 12, 13, 86, 229, 283, 296, 297, and 314 are recovered and live. Apply/default/individual option mutation actions remain compiled-code work |

None of today's edits have had an **in-game smoke test** (game not launched per scope);
both staged CompiledDefs edits satisfy every static invariant forge checks.

## 7. Ghidra follow-ups (lock currently taken)

1. Continue mapping `EActionType_NUISystem` through
   `CFrontEndManager::Action @ 0x0059A238` and the generic
   NUISystem/component controllers as new controls become live.
   Options actions 9/12/13/283 and their `Init2` screen-key bindings are now
   recovered alongside actions 86, 229, 296, 297, and 314.
2. GraphicIndex bank-resolution rule (which CGuiBank a widget context binds — evidence
   above is header-id matching only).
3. Fonts: recover the runtime kerning/adjustment-table semantics and validate
   Xbox streaming differences. The PC Unicode lookup, LZO metric chunks, alpha
   blocks, and legal-string rendering are complete.
4. Whether the engine tolerates *added* frontend.bin/game.bin entries (addEntry write
   path exists but is engine-[unproven]) and whether entry order constraints exist
   beyond "entry order = runtime def index".
5. `CUIDef::Sprites` is now decoded as
   `u32 count + count * { u32 ETableSprites, s32 CDefIndex }`.
   `parse_frontend.py entry` reports the enum name, numeric value, and component
   index. The recovered values are:

   | Value | Sprite |
   |---:|---|
   | 0–3 | top-left, top-right, bottom-left, bottom-right |
   | 4–5 | horizontal-top, horizontal-bottom |
   | 6–7 | vertical-left, vertical-right |
   | 8–11 | tee-up, tee-down, tee-right, tee-left |
   | 12 | cross |

   Round-trip writing remains supported; authoring brand-new tables still
   needs an explicit high-level mutation API and live engine acceptance test.

## 8. Optional high-resolution presentation

Menu/UI textures can follow the same opt-in AI-enhancement policy as the boot
movies. Retail bytes and decoded retail PNGs remain the parity source; 2x
results belong in an ignored cache and are selected only by an explicit
runtime option with per-asset retail fallback.

Transparent sprites require alpha separation, RGB edge extrusion, deterministic
alpha scaling, and recombination rather than feeding premultiplied RGBA
directly to Real-ESRGAN. Six-tile forest frames should be stitched before
upscaling to prevent seams. Text should preferentially be rerendered at higher
resolution from decoded font metrics instead of letting AI reshape glyphs.

The implementation order, manifest schema, validation gates, current exact
asset ids, and next-session checklist are in
[`docs/UI_UPSCALE_PLAN.md`](UI_UPSCALE_PLAN.md).

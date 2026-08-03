# FRONTEND / GUI data — formats, asset chain, editability

> **Current status (2026-08-02):** The authored frontend checkpoint has
> recovered retail wheel/list/click dispatch semantics, the generic manager's
> condition/process ordering, and the source-pipeline delete-confirmation
> frame. These are verified data/behavior boundaries, not claims that the
> visual adapter owns native `CList`, `CManager`, or profile persistence
> objects. The strict bootstrap gate remains active.

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

Options now constructs that 280-pixel table live: owned definitions
129/130/131 bind to the pristine 64x64 left/right and 8x64 middle textures,
with the middle component zoomed across 19 tiles. The no-ornament component
sheet keeps row text above those draws and recomposes pixel-identically with
the baked oracle. Main-menu selection ornaments use the same bridge: ordinary
rows reuse the 19-tile vector and Continue Game owns a 34-tile
`UI_BUTTON_BIG` vector. Retail and BuffJesus no-ornament sheets both recompose
pixel-identically with their baked seven-frame oracles.
The live Render2D-to-D3D9 bridge must explicitly resolve the title segment and
all three `TS_BUTTON_L/M/R` texture handles. A missing mapping here does not
break component generation, but it attaches null at draw time and hides the
ornament. The main-menu smoke now samples the translucent middle away from the
row text before and after hover; retail and BuffJesus runs produce deltas of
354 and 355, and the complete subscreen interaction smoke passes the same
check.

The seven main-menu labels are independent now as well. The embedded component
atlas is 1280x3360: the left 640 pixels hold title-only frames, while the right
half holds one transparent 640x480 text-child canvas per row. Render2D submits
all seven row canvases after the live selected-button components. The retained
640x3360 baked sheet remains the oracle, and overlaying the selected ornament,
title frame, and seven row children reproduces every retail and BuffJesus frame
pixel-identically. This is an intermediate runtime texture representation; the
row children consume the decoded `CUIState` position and colour at draw time,
but they do not yet use the retail streaming-font object.

The main-menu and Options lists both compile with `Wrapping=true`,
`Scrolling=false`, `AlphaOffset=0`, and `PositionOffsetY=30`. Every main-menu
text child has five states with x positions `(120,120,0,120,120)`, alpha
`(1,1,0,1,1)`, update times `(-1,-1,0.2,-1,-1)`, and state-change flag 7.
Keyboard Up/Down now enters state 4 on the old child and state 3 on the new
child through the recovered frontend-list decision path. Since states 3 and 4
share the same visible x/alpha and have no timed update, the exact definition
contract is an instantaneous row-state change plus movement of the selected
ornament—not a list slide or fade. The 0.2-second hidden state 2 belongs to a
different transition and is not synthesized during ordinary navigation.
Enter activation shares the mouse-release route for press-start action 229,
main-menu actions 297/314, and all four Options actions 9/13/12/283. This is
live manager-routing integration. The checkpoint now polls the period WinMM
joystick surface at the frontend timer: POV/analogue Up/Down, button-1/Start
accept, and button-2/Back are edge-gated into those same routes. Axis, POV,
button, hold, release, and reconnect behavior is focused-test covered.
Gameplay/Video/Audio screens wrap controller row focus and apply Left/Right
through their live profile-value path; the complete subscreen smoke mutates two
rows and proves a round-trip to the activation values. Held directions use the
recovered `ChangeSelection @ 0x00494380` timing/state contract: a changed
direction fires immediately, the same direction first repeats at 500 ms, and
continued repeats fire every 100 ms. Unimplemented main-menu actions remain a
separate boundary.

The decoded seven-row main-menu action order is now locked in the runtime
adapter and focused test as `(66,16,297,10,67,321,314)`: Continue Game,
Change Profile, Options, Games for Windows - LIVE, Credits, About, and Quit.
Continue Game, Options, Quit, the initial Credits route, and the normal
Change Profile list route are live; the PC LIVE no-op remains an explicit
runtime boundary. The recovered action identifiers stay attached to their
authored rows instead of relying on row-index guesses.

### Change Profile graph (`UI_FRONTEND_PROFILES_MENU`)

The shipped profile-management graph is now structurally gated against
`frontend.bin`. The normal profile screen uses the coastal blend, the 640px
title rule, `UI_HELPERS_PROFILE_DELETE`, and the dynamic
`UI_FRONTEND_LIST_FOR_PROFILES` container at `(200,120)` with height 260.
Its authored `PositionOffsetY=28` is the runtime row cadence; the renderer
uses that decoded value for visible-row placement and hit-testing.
The delete list is a separate dynamic Type-43 container at `(200,180)` with
height 210; their authored up/down arrow bindings and Type-11 row template are
preserved. Both list records have zero serialized children because rows are
created from runtime profile data.

Authored list arrows are input affordances, not decorative sprites. The
retail `frontend.big` bank identifies graphic entries 379–382 as the four
32x32 `FE_SCROLL_*_SPRITE`/hovered variants bound by these Type-38 children.
The checkpoint maps one signed mouse-wheel event over each list viewport and
left-clicks on those decoded 32x32 arrow children into the same recovered
`CFrontEndList::ScrollUp/ScrollDown` planner used by keyboard/controller
navigation. The profile and Redefine records retain their serialized
`Scrolling=true, Wrapping=false` boundary behavior; the save list retains its
serialized wrapping behavior. Redefine's first nine labels remain the only
rendered page until the stripped EGameAction display-name table is recovered.

The list planner and retail mouse-wheel input classes are decompiled and
verified. The retail `ego_r` `CMouseDX::ConvertMouseEventToInputEvent`
(`0x00C55D20`) handles event type `10` with
`CInputEvent::SetAsMouseWheelMovement` (`0x00B6EB00`), and the game input
process reads that delta through the recovered `GetMouseWheelMovement`
forwarders. The retail `CNewFrontendGameComponent::Input` (`0x0042E3EE`)
then maps input type `0x0E` to action `0x24` for positive movement or
`0x25` for negative movement, using the strict `+/-0.0001` threshold.
`CList::ProcessEvent` (`0x0053673B`) consumes list events `0`/`1` as
up/down, and `CClickable::ProcessEvent` (`0x0055AD60`) consumes
`0x1A`/`0x1C` for left clicked/unclicked, retaining the component's pressed
state between the two events. The current Win32 message is only the platform
ingress; the checkpoint routes through those recovered action/event IDs,
dispatches the arrow callback on press, clears the pressed state on release,
and does not expand a larger delta into repeated list events. The list event
boundary also preserves the four decompiled `CList::ProcessEvent` condition
results, but the visual adapter supplies the normal visible-list results
because it does not yet own a live `NUISystem::CList` object; it does not
assign guessed meanings to the opaque guard fields.

The manager handoff is also recovered: this input path calls the generic
frontend singleton `CFrontEndManager::GetInstance @ 0x0041E5F2`, whose vtable
slot 0 is `CManager::ProcessEvent @ 0x0055CB10`. That dispatcher prefers its
current component; when there is no current component it snapshots the
registered component list, evaluates each component condition, and processes
every component whose condition passes. The checkpoint represents that exact
condition-then-process ordering with a callback target in
`frontend_input_dispatch.h`; it does not fabricate a native manager object.

The same gate covers the delete-screen variant, the no-profiles message at
`(320,200)`, and the Type-12 new-profile menu at `(40,150)`. The normal route
now refreshes profile directories from the user save root and renders their
names through the authored `ENG_ARIAL_16` atlas; names are not compiled into
the visual checkpoint. The adapter now preserves the recovered manager action
boundary for delete-list entry (`0xFA`), delete-row selection (`0xD7`), empty
and normal New Profile (`0x125`), profile load (`0x124`), keyboard confirm
(`0x126`), and keyboard cancel (`0x127`). The native
`CreateNewProfile`/`LoadProfile`/delete-manager bodies remain the only owners
of persistence and are not replaced by a checkpoint-side writer; until that
manager instance is linked, the visual harness stops at those exact action
boundaries.

The delete-confirmation graph is locked by the retail layout oracle and now
has a source-pipeline composition: `UI_FRONTEND_DELETE_PROFILE_MENU` children are title,
spooky background, title rule, YES, NO, and explanation. The recovered text
symbols are `TEXT_GUI_MENU_DELETE_PROFILE`,
`TEXT_GUI_MENU_DELETE_PROFILE_PROMPT`, and
`TEXT_GUI_MENU_DELETE_PROFILE_EXPLANATION`, at `(65,44)`, `(320,100)`, and
`(320,240)`, with YES/NO controls at `(362,405)` and `(20,405)`. The English
text bank resolves the exact prompt and explanation strings. The profiles
resource sheet now carries the authored normal frame plus the confirmation
frame from those retail definitions. Row action `0xD7` enters it; YES stops at
native action `0xD6`, whose manager callback still owns deletion, refresh, and
persistence. No checkpoint-side delete writer has been added.

New-profile text input is likewise source-backed: retail
`CFrontendGameComponent::ProcessTextInputCharacter` (`0x004944E0`) uses a
128-byte buffer, accepts the low byte of each input character while the
logical length is below 127, and handles character `8` as backspace. The
checkpoint now uses that exact buffer/limit/backspace contract; the native
`CVirtualKeyboard::Confirm` and `CFrontEndManager::Action` callbacks still own
validation and profile creation.

### Credits screen (`UI_FRONTEND_CREDITS_MENU`)

The shipped Credits root is a Type-10 screen with children `FOREST`,
`UI_SCROLLING_TEST`, `UI_HELPERS_CREDITS`, and the two widescreen bars. Its
scrolling child starts at `(0,480)` and transitions to `(0,0)` over 180
seconds. The initial checkpoint frame therefore contains the retail title
sprites and graphic 334 bars while credit text is still below the viewport.
The renderer validates this authored boundary against `frontend.bin`, and the
runtime routes action 67 into the frame with the shared Back helper live.
This proves routing and initial composition; continuous text scrolling and a
same-state retail pixel diff remain open.

The renderer now also consumes the six authored `TEXT_GUI_CRE_*` type-1
groups directly from `text.big`, preserving member IDs, order, and explicit
single-space rows. This is an extraction oracle for the future live text
component; it does not invent line spacing or promote an unverified baked
scroll surface into the runtime.

The remaining four routes are now fully recovered from the same
`CFrontEndManager::Action @ 0x0059A238` decompilation:

* **Change Profile (16, `0x10`)** does not resolve a used key; the dispatcher
  calls `CFrontEndManager::GotoProfileMenu @ 0x00597b20` directly, which enters
  the `UI_FRONTEND_LIST_FOR_PROFILES` profile-management subsystem
  (create/delete/load-profile plus the virtual keyboard).
* **Games for Windows - LIVE (10, `0xa`)** has **no case** in the switch. The
  action is dispatched but falls to the default `return`, so the row is
  intentionally inert on PC: the main menu stays put with no forward sound.
  The visual checkpoint models this as an explicit consumed no-op.
* **Credits (67, `0x43`)** resolves used key `0x09` and continues through
  `GotoNextScreen(..., false)`. The `0x09 -> UI_FRONTEND_CREDITS_MENU`
  binding (the multi-page `UI_FRONTEND_BG_CREDITS_1..3_0N` animated scroll) is
  inferred from the sole matching screen def and the established Init2
  key->screen pattern; direct `Init2 @ 0x00598A1C` confirmation is pending.
* **About (321, `0x141`)** resolves used key `0x1c` and continues through
  `GotoNextScreen(..., false)`. The `0x1c -> UI_FRONTEND_ABOUT_MENU` binding
  (title `UI_TABLE_TITLE_WHOLE_ABOUT` + `UI_FRONTEND_ABOUT_MESSAGE` text) is
  likewise inferred from the sole matching `ABOUT` screen def and the Init2
  pattern; direct `Init2` confirmation is pending.

The action->used-key half of both routes is proven directly:
`Action()` sets `param_1 = 9` for `0x43` and `param_1 = 0x1c` for `0x141`
before the shared `map<EUsedKeys,long>[used_key] -> GotoNextScreen` tail.

### About screen (`UI_FRONTEND_ABOUT_MENU`, #449) — decoded structure

Decoded from the shipped `frontend.bin` (schema `ghidra_out/def_schema.json`),
the About screen is a `Type 10` frontend screen structurally identical to
`UI_FRONTEND_OPTIONS_SUB_MENU` (#211) with the interactive list swapped for a
static wrapped message. Its five children (`[451, 450, 737, 121, 577]`) are:

| Idx | Name | Type | Key fields |
|---|---|---|---|
| 451 | `UI_FRONTEND_ABOUT_MESSAGE` | 6 (text) | `TEXT_GUI_MENU_ABOUT_MESSAGE`, `ENG_ARIAL_12`, state0 pos `(320,60)` A=1, text window BR `(700,5000)`, flag 34 |
| 450 | `UI_TEXT_ABOUT_MENU_TITLE` | 6 (text) | `TEXT_GUI_MENU_ABOUT_TITLE`, `ENG_ARIAL_24`, state0 pos `(65,14)` A=1, flag 34 |
| 737 | `UI_BLENDING_BACKGROUNDS_SPOOKY` | 5 | swap children `[739 UI_SWAPPING_SPOOKY_SUNBEAM, 738 UI_SWAPPING_SPOOKY]` (vs Options' `FOREST` #632) |
| 121 | `UI_TABLE_TITLE_WHOLE_ABOUT` | 2 | W=640, pos `(0,5)`, sprites `TOP_LEFT/TOP_RIGHT/HORIZONTAL_TOP` from component 122 |
| 577 | `UI_HELPERS` | 5 | shared helper bar, child `[563]` |

Versus the Options children `[219, 338, 632, 120, 577]`, the only structural
deltas are the message-for-list swap, the SPOOKY-for-FOREST background, the
`WHOLE_ABOUT` title rule, and the title text sitting higher (child y=14, rule
table y=5) than the Options header (title child y=44). The screen has no
selectable rows: the only exit is Back/action 86 (`GotoPreviousScreen`) back to
the main menu.

The Continue route is no longer ambiguous. `CFrontEndManager::Action
@ 0x0059A238` handles action 66 by calling
`RefreshAvailableSavedGamesForProfile @ 0x00598463`; if no save rows and no
fallback count exist it takes the no-save path, otherwise it resolves used key
`0x08`, bound by `Init2` to `UI_FRONTEND_PROFILE_SAVED_GAMES_MENU`.
The refresh body replaces `UI_FRONTEND_LIST_FOR_SAVES`, then attaches
`UI_SCROLLING_VIEWPORT_TEXT_AREA` and `UI_SCREENSHOT_VIEWPORT` in that order.
Rows are autosave-first followed by ascending non-empty manual
`SaveGameNames1..50` slots, with exact 30-pixel spacing. Each row uses
`UI_FRONTEND_BUTTON_FOR_SAVE_LIST`/`UI_BUTTON_FOR_SAVE_NAME`, carries the
original filename in a `CActionParamString`, and receives action `0x11` only
when its primary/companion validity checks pass; invalid rows receive action
`0xDC`. The visual checkpoint now implements that browser contract: action 66
enters the screen, Up/Down and mouse hover move the four-row highlight, and
Back/action 86 returns to the main menu. Its live rows begin at `(10,90)`,
advance by 30 pixels inside the 150-pixel list viewport, and use the exact
120-pixel-inner-width `TS_BUTTON_L/M/R` ornament. The four save frames are
packed beside the existing options/detail atlas frames at x=1024..1664,
y=1920..3840, keeping the texture at 1664x3840 for D3D9-era limits. A
pixel-level smoke requires a visible selection delta and a distinct hash after
scrolling. Enter on action `0x11` remains intentionally disabled until the
main-game/world-load ownership boundary is connected.

Those gates prove routing, decoded structural geometry, and visible state
change; they do not prove final screenshot parity. User review of the current
Saved Games/Redefine presentation remains negative. A same-state retail
capture and alpha-aware diff is required before changing or signing off text
scale/baselines, highlight placement, metadata composition, or the background.

`AddRegionAndTimeInfo @ 0x00597228` supplies the corresponding row metadata.
It always appends one synchronized region-name, minimap-graphic, and
world-frame record. The primary save supplies `CurrentRegionName` and
`CurrentRegionMinimapGraphicName`; a valid `path + L"."` sidecar takes
precedence for `WorldFrame`. Both compressed `FableSav` and legacy text
persist paths converge on the exact `HEADER` schema. This isolates the future
browser's metadata extraction from its component construction.

`ConstructFileDescription @ 0x00595CC1` consumes those arrays. It creates
`UI_TEXT_FILE_DESCRIPTION` at `(65,261)`, the runtime detail text at
`(95,293)`, and either the minimap-backed
`UI_RING_PIC_DRAW_FROM_VIEWPORT` or corrupt/empty
`UI_RING_PIC_SAVE_VIEWPORT` centered at `(442,165)`. Detail text is assembled
from profile name, file date/time, localized region, and formatted playtime.
The picture uses `CurrentRegionMinimapGraphicName`; it is not a bitmap captured
inside the save. Exact viewport installation, row-child append, and counted
release order are recovered for the live implementation.

Frontend audio is now tied to the same recovered decision paths. The decoded
`UI_MISC_THINGS_DEF_INSTANCE` maps `SoundUpDown=CS_GUI_2`,
`SoundError=CS_GUI_5`, `SoundBack=CS_GUI_6`, and
`SoundForward=CS_GUI_7`. `Frontend.lug` resolves those criteria to samples
3, 7, 4, and 5 respectively. The build extracts the complete retail RIFF
payloads, embeds them as WAVE resources 116-119, and plays movement/error from
the exact `CFrontEndList` plan and back/forward from screen transitions.
Mouse-hover audio and unimplemented actions remain deliberately unwired until
their retail dispatch paths are recovered.

### Initial frontend parity boundary

| Slice | Current evidence | Code-parity status |
|---|---|---|
| Boot movies | Retail files, order, end-of-stream advance, and Escape skip are live | Recovered sequencing with a compatibility presentation seam |
| Press-start and main menu | Retail definitions/assets; all 14 retail/BuffJesus frames recompose pixel-identically | Live component adapters; not whole-function byte parity |
| Options and detail frames | All eight frames recompose pixel-identically; Enter/mouse routes and Cancel/Apply/default behavior are smoke-gated | Several rows and controls still originate in component atlases |
| Main/Options list state | Exact 0x24-byte `CUIState` layout, decoded state maps, wrapping, and old/new state changes | `InitialiseOffsets` is exact 57/57, `ScrollUp` is 834/834, and `ScrollDown` is 977/977; both scroll bodies are complete-symbol relocation matches |
| Manager transitions/profile/save loading | Constructor/singleton/init layout, stack ownership, used-key lookup, transition fields, menu/title replacement, scoreboard profile round-trip, virtual-keyboard allocation, profile creation, component creation, button composition, press-start, load, draw, frame update, edit-box recursion, slider reset, delete-list paths, and the complete saved-game row-construction contract are recovered | Twenty-nine manager bodies match complete retail symbols; the large saved-game refresh body is decomp-backed while `RefreshAvailableProfiles` remains behavior-only |
| Frontend sound | Exact criteria mapping and byte-identical RIFF resources from `Frontend.lug` | Playback is a Win32 resource bridge, not the recovered retail sound manager |
| Controller/full actions | Keyboard/mouse plus WinMM navigation, retail-timed held-direction repeat, detail-row focus/Left/Right, accept, and back cover the implemented initial routes | Remaining main-menu actions are incomplete |

“Pixel-identical” here means the reconstructed component composition equals
the retained retail-derived oracle for every enumerated frame. It does not
mean the surrounding manager/update/draw functions are byte-identical. The
C++23 port should fork only after the remaining controller/action and live
detail-control boundaries are fixed, while the VC7.1 sources remain the
behavior and object-code oracle.

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
 The checkpoint now preserves the decoded left origin `(65,44)` and the decoded
 font/outline. The Options submenu and every detail frame call the same
 `_draw_title` path, and a focused test locks the left-aligned header origin.
 The table rule
itself is now live: its three owned definition-122 children are adapted into
clipped/scaled Render2D quads, while a retained baked oracle proves the
no-rule sheet plus those components recompose pixel-identically. Header text
remains part of the authored flat-composition boundary until text components
are emitted live.

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
oracle sheet is 1024x3840: full 640x480 frames occupy its left side and 124
compact 200x30 control states occupy a transparent atlas on the right. The
live component sheet is 1664x3840; it preserves those coordinates and adds
four independent 640x480 Options row canvases at x=1024. Each is submitted
 through Render2D with its decoded `CUIState` position and colour. Nine generated
 Redefine rows occupy a sixth 640x480 helper-sheet frame as complete 588x35
 strips. Each row combines the 280-pixel `FE_SLOT_TEST_L/M/R_ON` action-name
 table with the compiled 220-pixel `UI_OPTIONS_HORIZONTAL_BAR` key-value
 table. That component resolves to `FE_OPTIONS_HORIZONTAL_BAR_SPRITE`, not a
 second rounded slot. Keeping each strip intact lets D3D9 replace a row with
 one quad, avoiding filtering seams while retaining safe texture sizes.

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
 records: list origin `(40,115)`, table offset `(-32,-2)`, right-table offset
 `(368,-3)`, and action/key text offsets `(0,3)`/`(380,3)`. Both dynamic text
 children use `ENG_ARIAL_12`, not the screen title's `ENG_ARIAL_24`. The
 invisible hover child is `600x24` at `(-40,0)`, leaving the serialized
 two-pixel gap between successive 26-pixel list rows.
 `CText::Draw @ 0x0054EF00` reads and rounds the component x/y render origin;
 its alignment branch changes only x. There is no separate runtime vertical
 centering adjustment, scale never modifies the origin, and
 `CEnginePrimitive2DText` stores y unchanged. Consequently the serialized
 `+3` y origin plus the retail `ENG_ARIAL_12` glyph offsets is the key/action
 baseline oracle, rather than a visually estimated center of either table
 primitive.
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

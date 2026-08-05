#!/usr/bin/env python3
"""Render recovered retail frontend Options, detail, and Quit overlays.

The layout is taken directly from the shipped ``frontend.bin`` records:

* ``UI_FRONTEND_OPTIONS_SUB_MENU`` (#211)
* ``UI_FRONTEND_LIST_OPTIONS_SUB_MENU`` (#219), origin (200, 150)
* ``UI_FRONTEND_QUIT_PROMPT`` (#631)
* ``UI_FRONTEND_QUIT_YES/NO`` (#595/#596)

Final screen-title composition uses the serialized x=65 left-aligned title
child from the shipped frontend layout.

The primary output consists of two vertical sheets.  The Options sheet has
one 640x480 frame per selected row followed by the Gameplay, Audio, Video,
and Redefine Keys destination screens, then four selected-row views of the
recovered initial saved-games browser.  The helper sheet has Back off/on,
the Quit prompt with neither/No/Yes hovered, and one compact atlas frame
holding the nine complete retail Redefine Keys ON rows in the initial
viewport.  Optional component-path outputs retain the primary sheet as the
oracle while removing its baked title rule and exporting that rule's retail
sprite prototype separately for live CTable/Render2D composition.
"""

import argparse
import os
import sys

from PIL import Image, ImageChops

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from render_fable_frontend_menu import (  # noqa: E402
    MENU_CONTENT_CENTER_X,
    _decode_named,
    build_selected_button,
    selected_button_left,
)
from frontend_layout_oracle import (  # noqa: E402
    FrontendLayoutOracle,
    require_equal,
    require_position,
)
from render_fable_static_font import (  # noqa: E402
    add_outline,
    load_font,
    render_line,
    text_advance,
)
from texture_build import load_big  # noqa: E402
from text_build import TextBank  # noqa: E402


CANVAS_SIZE = (640, 480)

# These are the six text widgets in UI_CREDITS_CONTAINER.  Keep this table
# beside the compiled-layout validator so a future live text renderer cannot
# silently substitute literals or reorder the retail groups.
CREDITS_TEXT_GROUPS = (
    ("UI_CREDITS_TEXT_MAIN1", "TEXT_GUI_CRE_MAIN1", "ENG_ARIAL_24"),
    ("UI_CREDITS_TEXT_MAIN2", "TEXT_GUI_CRE_MAIN2", "ENG_ARIAL_24"),
    ("UI_CREDITS_TEXT_TESTSUP", "TEXT_GUI_CRE_TESTSUP", "ENG_ARIAL_24"),
    ("UI_CREDITS_TEXT_MICROSOFT", "TEXT_GUI_CRE_MICROSOFT", "ENG_ARIAL_24"),
    ("UI_CREDITS_TEXT_TEST", "TEXT_GUI_CRE_TEST", "ENG_ARIAL_12"),
    ("UI_CREDITS_TEXT_THANKS", "TEXT_GUI_CRE_THANKS", "ENG_ARIAL_12"),
)
HEADER_RULE_POSITION = (0, 35)
HEADER_TEXT_POSITION = (65, 44)

# UI_FRONTEND_ABOUT_MENU (#449) header sits 30px higher than the Options
# header: UI_TABLE_TITLE_WHOLE_ABOUT (#121) is serialized at (0,5) and
# UI_TEXT_ABOUT_MENU_TITLE (#450) at (65,14).
ABOUT_RULE_POSITION = (0, 5)
ABOUT_TITLE_TEXT_POSITION = (65, 14)
# UI_FRONTEND_ABOUT_MESSAGE (#451): ENG_ARIAL_12, serialized (320,60), text
# window BR (700,5000).  The screen scrolls, so a baked still shows the
# message at its initial (top) scroll position.
ABOUT_MESSAGE_ORIGIN = (CANVAS_SIZE[0] // 2, 60)
ABOUT_MESSAGE_WRAP_WIDTH = 600
ABOUT_MESSAGE_LINE_STEP = 16
ABOUT_MESSAGE_PARAGRAPH_GAP = 8
ABOUT_TITLE = "About Fable"
# Transcribed from retail text.big (verified names):
#   TEXT_GUI_MENU_ABOUT_TITLE = "About Fable"
#   TEXT_GUI_MENU_ABOUT_MESSAGE_PART_1 group = PART_1_01/_02/_03
#   TEXT_GUI_MENU_ABOUT_MESSAGE_PART_2 group = PART_2_01
# Content is parity-faithful; retail line-break/scroll positions are not yet
# reverse-engineered, so the wrap below is a greedy best-effort layout.
ABOUT_MESSAGE_PARAGRAPHS = (
    "Fable: The Lost Chapters © 2005 Lionhead Studios Limited. "
    "Lionhead, the Lionhead logo, and Fable are registered trademarks "
    "owned by Lionhead Studios Limited. All rights reserved.",
    "Published and distributed by Microsoft Corporation. "
    "All rights reserved.",
    "Microsoft, DirectSound, DirectX, the Microsoft Game Studios logo, "
    "and Windows are either registered trademarks or trademarks of "
    "Microsoft Corporation in the United States and/or other countries.",
    "Warning: This computer program is protected by copyright law and "
    "international treaties. Unauthorized reproduction or distribution of "
    "this program, or any portion of it, may result in severe civil and "
    "criminal penalties, and will be prosecuted to the maximum extent "
    "possible under the law.",
)
OPTIONS_SHEET_WIDTH = 1024
OPTIONS_COMPONENT_SHEET_WIDTH = 1664
OPTIONS_ROW_ATLAS_ORIGIN_X = OPTIONS_SHEET_WIDTH
CONTROL_TILE_SIZE = (200, 30)
CONTROL_ATLAS_ORIGIN_X = 640
CONTROL_ATLAS_COLUMNS = 1
REDEFINE_KEY_ATLAS_ORIGIN_X = 840
REDEFINE_KEY_TILE_SIZE = (176, 26)
HELPER_SHEET_FRAME_COUNT = 6
REDEFINE_HOVER_HELPER_FRAME = 5
REDEFINE_HOVER_STRIP_SIZE = (588, 35)
REDEFINE_RESET_HOVER_ATLAS_Y = 320
# Detail-screen (Gameplay/Video/Audio) hover overlays live in the OPTIONS
# component-atlas (1664 wide, 3840 tall) genuinely-free region x[1024,1664),
# y>=2400: option-row layers occupy the row-atlas column only for y[0,1920),
# the save component for y[1920,2400), and the control/redefine columns end
# well left of x=1024.  These origins are MIRRORED as C++ constants in
# visual_boot_d3d9.cpp (kDetailHoverAtlasOriginX / kDetailArrowHover* /
# kDetailFooterHover*) -- keep the two in lock-step.
DETAIL_HOVER_ATLAS_ORIGIN_X = 1024
DETAIL_HOVER_ATLAS_ORIGIN_Y = 2400
# Arrow HOVERED tiles are full 200x30 control tiles so the hovered arrow sits
# at the IDENTICAL control-tile-local offset (left x=8, right x=162) as the
# baked arrow and renders over the same design rect (x=400, width 200) after
# the recovered UI_FRONTEND_LIST parent translation is applied.
DETAIL_ARROW_HOVER_TILE_SIZE = CONTROL_TILE_SIZE  # (200, 30)
DETAIL_ARROW_HOVER_LEFT_ATLAS_Y = DETAIL_HOVER_ATLAS_ORIGIN_Y          # 2400
DETAIL_ARROW_HOVER_RIGHT_ATLAS_Y = DETAIL_HOVER_ATLAS_ORIGIN_Y + 30    # 2430
# Footer ON tiles reproduce _draw_helper's 256x64 GLYPH (local 0,0) + the same
# LABEL so the ON overlay fully covers the baked OFF glyph+label.  Local origin
# of each tile == design (parent_x, parent_y-16).
DETAIL_FOOTER_HOVER_TILE_SIZE = (256, 64)
DETAIL_FOOTER_HOVER_CANCEL_ATLAS_Y = DETAIL_HOVER_ATLAS_ORIGIN_Y + 64      # 2464
DETAIL_FOOTER_HOVER_DEFAULTS_ATLAS_Y = DETAIL_HOVER_ATLAS_ORIGIN_Y + 128   # 2528
DETAIL_FOOTER_HOVER_APPLY_ATLAS_Y = DETAIL_HOVER_ATLAS_ORIGIN_Y + 192      # 2592
# Design rects the footer ON overlays are rendered at (origin = glyph top-left
# = parent_y-16), mirrored in the C++ render side.
DETAIL_FOOTER_HOVER_CANCEL_DESIGN = (20, 424)
DETAIL_FOOTER_HOVER_DEFAULTS_DESIGN = (192, 384)
DETAIL_FOOTER_HOVER_APPLY_DESIGN = (362, 424)
REDEFINE_RESET_HOVER_SIZE = (320, 64)
OPTIONS_LIST_ORIGIN = (200, 150)
OPTIONS_ROWS = (
    ("Gameplay Options", 0),
    ("Video Options", 30),
    ("Audio Options", 60),
    ("Redefine Keys", 90),
)
DETAIL_SCREEN_COUNT = 4
SAVE_BROWSER_ROWS = (
    "AutoSave",
    "Manual - Save1",
    "Manual - Save2",
    "Manual - Save3",
)
SAVE_LIST_ORIGIN = (10, 90)
SAVE_ROW_STEP_Y = 30
SAVE_LIST_HEIGHT = 150
# UI_VIEW_RING_SMALL child of UI_FRONTEND_PROFILE_SAVED_GAMES_MENU, serialized at
# (314, 37): the save-preview viewport on the right. It frames a 256x256 region
# minimap (MINIMAP_*_FRONT_END) behind the ring ornament
# (UI_VIEW_RING_SMALL_SPRITE_FE, 256x256).
SAVE_VIEW_RING_ORIGIN = (314, 37)
SAVE_VIEW_RING_SPRITE = "UI_VIEW_RING_SMALL_SPRITE_FE"
# The runtime selects the region minimap from the decoded save-region field;
# that field is not recovered yet, so the checkpoint shows the starting region
# (Oakvale) as a deterministic, asset-backed placeholder.
SAVE_PREVIEW_MINIMAP = "MINIMAP_STARTOAKVALE_FRONT_END"
# Saved-games UI_TITLE_AREA and UI_TEXT_AREA are two asymmetric CTable pairs.
# CTable consumes sprite keys in the order key 0 (corner), key 4 (horizontal
# edge), key 1 (corner); the table record's Width is the inner edge span.
SAVE_TITLE_AREA_ORIGIN = (0, 35)
SAVE_TITLE_AREA_LEFT_WIDTH = 287
SAVE_TITLE_AREA_RIGHT = (463, 40)
SAVE_TEXT_AREA_ORIGIN = (0, 254)
SAVE_TEXT_AREA_LEFT_WIDTH = 287
SAVE_TEXT_AREA_RIGHT = (463, 40)
SAVE_TEXT_BOTTOM_ORIGIN = (0, 404)
SAVE_BOTTOM_BACKDROP_ORIGIN = (0, 292)
SAVE_SCREEN_FRAME_BASE = len(OPTIONS_ROWS) + DETAIL_SCREEN_COUNT
# Save selection-dependent pieces are live Render2D components. Keep one
# static save base instead of duplicating it once per sample selection.
SAVE_BROWSER_FRAME_COUNT = 1
OPTIONS_SHEET_FRAME_COUNT = SAVE_SCREEN_FRAME_BASE + SAVE_BROWSER_FRAME_COUNT
SAVE_COMPONENT_ATLAS_ORIGIN = (
    OPTIONS_ROW_ATLAS_ORIGIN_X,
    len(OPTIONS_ROWS) * CANVAS_SIZE[1],
)
# The component sheet's free tail is used for runtime profile-name glyphs.
# ENG_ARIAL_16 is the compiled UI_FRONTEND_BUTTON_FOR_PROFILES_LIST font;
# copying its authored atlas here keeps the later C++ glyph quads sourced from
# fonts.big rather than from a platform font.
PROFILE_GLYPH_ATLAS_ORIGIN = (1024, 2704)
PROFILE_ROW_STEP = 28
# Small static save-screen sources promoted into the component atlas free tail
# so the live D3D9 path can submit their authored quads without another
# texture/resource attachment.
SAVE_BOTTOM_BACKDROP_ATLAS_ORIGIN = (1024, 2960)
SAVE_TEXT_BOTTOM_ATLAS_ORIGIN = (1032, 2960)
SAVE_TEXT_AREA_MIDDLE_ATLAS_ORIGIN = (1040, 2960)
SAVE_TEXT_AREA_BL_ATLAS_ORIGIN = (1048, 2960)
SAVE_TEXT_AREA_BR_ATLAS_ORIGIN = (1176, 2960)
SAVE_TITLE_AREA_TL_ATLAS_ORIGIN = (1304, 2960)
SAVE_TITLE_AREA_TR_ATLAS_ORIGIN = (1432, 2960)
# Detail title text is authored as ENG_ARIAL_24 in frontend.bin.  Keep its
# runtime atlas separate from the ENG_ARIAL_16 profile-row atlas above; both
# are copied into the component texture's otherwise-unused right-hand tail.
DETAIL_TITLE_GLYPH_ATLAS_ORIGIN = (1024, 3328)
DETAIL_TITLE_GLYPH_ATLAS_SIZE = (256, 256)
# Redefine row children are authored with ENG_ARIAL_12, not the ENG_ARIAL_24
# title font.  Keep this small atlas beside the title atlas in the component
# texture so the live row labels and key values can use their retail metrics.
REDEFINE_GLYPH_ATLAS_ORIGIN = (1536, 3328)
REDEFINE_GLYPH_ATLAS_SIZE = (128, 128)

GAMEPLAY_ROWS = (
    ("Game Camera", "Normal", 0.0),
    ("Bow Camera", "Normal", 0.0),
    ("Follow Camera", "ON", 0.0),
    ("Camera Sensitivity", None, 0.5),
    ("Subtitles", "ON", 0.0),
    ("Interface Opacity", None, 1.0),
    ("Tutorials", "ON", 0.0),
    ("Guildmaster Help", "ON", 0.0),
    ("Interface Tooltips", "ON", 0.0),
    ("Show Target Lock", "ON", 0.0),
)

AUDIO_ROWS = (
    ("Music Volume", None, 0.6),
    ("Sound Volume", None, 0.8),
    ("Dialogue Volume", None, 0.9),
)

# CONFIG_OPTIONS_DEFAULTS_DEF_INSTANCE supplies resolution and detail values.
# The remaining initial text values are the first entries in their compiled
# text-slider children.
VIDEO_ROWS = (
    ("Resolution", "2560X1440X32", 1.0 / 3.0),
    ("Refresh Rate", "100 Hz", 0.0),
    ("Anti-Aliasing", "8X", 0.0),
    ("Texture Detail", None, 1.0),
    ("Mesh Detail", None, 1.0),
    ("Shadow Detail", None, 1.0),
    ("Brightness", None, 1.0 / 17.0),
    ("Vertical Sync", "OFF", 0.0),
    ("Effects Detail", None, 1.0 / 3.0),
    ("Screen Aspect Ratio", "Auto", 0.0),
)

# Value order matches the retail text-slider children or the recovered
# CSlider min/max/step triplets.  The C++ checkpoint stores the selected
# index, so every mutable value can be composed from this compact atlas
# without pre-rendering every possible full-screen combination.
GAMEPLAY_CONTROL_VALUES = (
    ("Normal", "Inverted"),
    ("Normal", "Inverted"),
    ("OFF", "ON"),
    tuple(0.1 * value for value in range(1, 11)),
    ("OFF", "ON"),
    tuple(0.25 + 0.05 * value for value in range(16)),
    ("OFF", "ON"),
    ("OFF", "ON"),
    ("OFF", "ON"),
    ("OFF", "ON"),
)
AUDIO_CONTROL_VALUES = (
    tuple(0.1 * value for value in range(11)),
    tuple(0.1 * value for value in range(11)),
    tuple(0.1 * value for value in range(11)),
)
VIDEO_CONTROL_VALUES = (
    ("1920X1080X32", "2560X1440X32", "3840X2160X32"),
    ("100 Hz", "60 Hz", "75 Hz", "85 Hz"),
    ("8X", "OFF", "4X"),
    tuple(value / 3.0 for value in range(4)),
    tuple(value / 3.0 for value in range(4)),
    tuple(value / 3.0 for value in range(4)),
    tuple(0.3 + 0.1 * value for value in range(18)),
    ("OFF", "ON"),
    tuple(value / 3.0 for value in range(4)),
    ("Auto", "4:3", "16:9"),
)
CONTROL_VALUE_GROUPS = (
    GAMEPLAY_CONTROL_VALUES,
    AUDIO_CONTROL_VALUES,
    VIDEO_CONTROL_VALUES,
)

# UI_FRONTEND_LIST_REDEFINE_KEYS_MENU.ActionOrder and the shipped
# FABLE_PC_CONTROL_SCHEME_GDD_WASD records provide this first visible page.
#
# The full decoded retail ActionOrder is 31 entries -- action 60 expands into
# four W/A/S/D movement rows (CRedefinerList::RefreshScriptThings @ 0x00556A40),
# so the nine visible rows below cover action ids 60(x4),9,7,8,31,45. The
# remaining 25 ids scroll off the first page:
#   REDEFINE_FULL_ACTION_ORDER (below). Every id's default *key binding* is
#   sourced from FABLE_PC_CONTROL_SCHEME_GDD_WASD (see docs/CONTROLLER_ENUMS.md).
# The debug FableWin.pdb preserves the authoritative EGameAction enum names
# and values. Every ActionOrder entry also has a direct TEXT_GUI_ACTION_* entry
# in the shipped English text.big; keep the decoded localized wording beside
# the enum so later off-page materialization does not need guessed labels. The
# first-page strings remain the exact retail wording recovered from the capture.
REDEFINE_FULL_ACTION_ORDER = (
    60, 9, 7, 8, 31, 45, 6, 13, 14, 1, 32, 26, 86, 94, 78, 4, 113, 112, 72,
    56, 92, 90, 96, 91, 97, 93, 98, 99, 100, 55, 53)
# Recovered from debug_build/FableWin.pdb with DIA (EGameAction). These are
# enum identifiers recovered from the PDB.
REDEFINE_ACTION_ENUM_NAMES = {
    60: "GAME_ACTION_MOVEMENT",
    9: "GAME_ACTION_ATTACK",
    7: "GAME_ACTION_BLOCK",
    8: "GAME_ACTION_SPECIAL_ATTACK",
    31: "GAME_ACTION_RUN",
    45: "GAME_ACTION_TOGGLE_FIRST_PERSON_TARGETING",
    6: "GAME_ACTION_INTERACT",
    13: "GAME_ACTION_UNSHEATHE_MELEE_WEAPON",
    14: "GAME_ACTION_UNSHEATHE_RANGED_WEAPON",
    1: "GAME_ACTION_LOCK_TARGET",
    32: "GAME_ACTION_SNEAK",
    26: "GAME_ACTION_ATTRACT_EXPERIENCE_ORBS",
    86: "GAME_ACTION_ACTIVATE_SPELL_MODE",
    94: "GAME_ACTION_CYCLE_THROUGH_SPELLS_KEYBOARD",
    78: "GAME_ACTION_CHARGE_GUILD_SEAL",
    4: "GAME_ACTION_TOGGLE_MINI_MAP",
    113: "GAME_ACTION_CENTRE_CAMERA",
    112: "GAME_ACTION_TOGGLE_VIEW_HERO_MODE",
    72: "GAME_ACTION_TOGGLE_LIVE_GUI",
    56: "GAME_ACTION_CONTEXT_SENSITIVE_ITEM",
    92: "GAME_ACTION_OPEN_ITEMS_MENU",
    90: "GAME_ACTION_OPEN_WEAPONS_MENU",
    96: "GAME_ACTION_OPEN_MAGIC_MENU",
    91: "GAME_ACTION_OPEN_CLOTHING_MENU",
    97: "GAME_ACTION_OPEN_EXPRESSIONS_MENU",
    93: "GAME_ACTION_OPEN_CURRENT_QUESTS_MENU",
    98: "GAME_ACTION_OPEN_PERSONALITY_MENU",
    99: "GAME_ACTION_OPEN_LOGBOOK_MENU",
    100: "GAME_ACTION_OPEN_MAP_MENU",
    55: "GAME_ACTION_QUICK_ACCESS_ITEM",
    53: "GAME_ACTION_TAKE_PHOTO_FOR_PHOTOJOURNAL",
}
REDEFINE_ACTION_DISPLAY_TEXT = {
    60: "Movement",
    9: "Attack",
    7: "Block",
    8: "Flourish",
    31: "Run",
    45: "Toggle First-Person Targeting",
    6: "Interact",
    13: "Unsheathe Melee Weapon",
    14: "Unsheathe Ranged Weapon",
    1: "Toggle Target Lock",
    32: "Toggle Sneak Mode",
    26: "Suck Experience Orbs",
    86: "Activate Spell Mode",
    94: "Cycle Spells (Alternative To Mouse Wheel)",
    78: "Charge Guild Seal",
    4: "Toggle Minimap Display",
    113: "Reset Camera",
    112: "Toggle View Hero Mode",
    72: "Toggle In-Game Menu",
    56: "Use Context Sensitive Item",
    92: "Shortcut: Items Inventory",
    90: "Shortcut: Weapons Inventory",
    96: "Shortcut: Magic Menu",
    91: "Shortcut: Clothing Inventory",
    97: "Shortcut: Expressions Menu",
    93: "Shortcut: Current Quests Screen",
    98: "Shortcut: Personality Screen",
    99: "Shortcut: Logbook",
    100: "Shortcut: Map Screen",
    55: "Use Hotbar Item",
    53: "Take Photojournal Photo",
}
# Raw default controls from FABLE_PC_CONTROL_SCHEME_GDD_WASD.  These retain
# the source notation where the retail record has multiple bindings or an
# input key that is not yet assigned a localized tile label.  The first-page
# capture remains the display oracle for mouse-button wording; the raw tuples
# preserve the complete off-page binding evidence for the live renderer.
REDEFINE_ACTION_DEFAULT_INPUTS = {
    60: ("K:W<0,+1>", "K:A<-1,0>", "K:S<0,-1>", "K:D<+1,0>"),
    9: ("M1",),
    7: ("M2",),
    8: ("M3",),
    31: ("M2",),
    45: ("M2",),
    6: ("K:5",),
    13: ("K:Q",),
    14: ("K:E",),
    1: ("K:SPACE",),
    32: ("K:CAPS",),
    26: ("K:BSLASH",),
    86: ("K:LSHIFT/BSLASH",),
    94: ("K:O",),
    78: ("K:G",),
    4: ("K:M",),
    113: ("K:R",),
    112: ("K:H",),
    72: ("K:RETURN", "K:ESC"),
    56: ("K:0x00,F1,F2,F3",),
    92: ("K:F4",),
    90: ("K:F5",),
    96: ("K:F6",),
    91: ("K:F7",),
    97: ("K:F8",),
    93: ("K:F9",),
    98: ("K:F10",),
    99: ("K:0x54",),
    100: ("K:0x55",),
    55: ("K:1..9 (0x02..0x0A)",),
    53: ("K:PRNT SCRN",),
}
# Retail-facing labels for bindings whose DirectInput or mouse records have a
# stable display spelling. Keep the raw map above as the evidence source and
# leave opaque/reserved records in source notation instead of guessing.
REDEFINE_ACTION_DISPLAY_INPUTS = {
    9: "LEFT MOUSE BUTTON",
    7: "MIDDLE MOUSE BUTTON",
    8: "RIGHT MOUSE BUTTON",
    31: "RIGHT MOUSE BUTTON",
    45: "RIGHT MOUSE BUTTON",
    6: "TAB",
    13: "Q",
    14: "E",
    1: "SPACE",
    32: "CAPS LOCK",
    26: "LEFT SHIFT",
    86: "LEFT SHIFT",
    94: "O",
    78: "G",
    4: "M",
    113: "R",
    112: "H",
    72: "ENTER",
    92: "F4",
    90: "F5",
    96: "F6",
    91: "F7",
    97: "F8",
    93: "F9",
    98: "F10",
    53: "PRNT SCRN",
}
REDEFINE_ACTION_ORDER = (60, 9, 7, 8, 31, 45, 6, 13, 14)
REDEFINE_ROWS = (
    ("Move Forward", "W"),
    ("Move Left", "A"),
    ("Move Backward", "S"),
    ("Move Right", "D"),
    ("Attack", "LEFT MOUSE BUTTON"),
    ("Block", "MIDDLE MOUSE BUTTON"),
    ("Flourish", "RIGHT MOUSE BUTTON"),
    ("Run", "RIGHT MOUSE BUTTON"),
    ("Toggle First-Person Targeting", "RIGHT MOUSE BUTTON"),
)
# Logical rows after expanding the authored movement action. The first nine
# entries are the capture-backed initial viewport above; the remaining rows
# are the exact localized labels recovered from text.big. Keep the action id
# beside each label so the live scroll renderer can later bind the row to its
# control record without re-parsing the source tables.
REDEFINE_MATERIALIZED_ROWS = (
    (60, "Move Forward"),
    (60, "Move Left"),
    (60, "Move Backward"),
    (60, "Move Right"),
    (9, "Attack"),
    (7, "Block"),
    (8, "Flourish"),
    (31, "Run"),
    (45, "Toggle First-Person Targeting"),
    (6, "Interact"),
    (13, "Unsheathe Melee Weapon"),
    (14, "Unsheathe Ranged Weapon"),
    (1, "Toggle Target Lock"),
    (32, "Toggle Sneak Mode"),
    (26, "Suck Experience Orbs"),
    (86, "Activate Spell Mode"),
    (94, "Cycle Spells (Alternative To Mouse Wheel)"),
    (78, "Charge Guild Seal"),
    (4, "Toggle Minimap Display"),
    (113, "Reset Camera"),
    (112, "Toggle View Hero Mode"),
    (72, "Toggle In-Game Menu"),
    (56, "Use Context Sensitive Item 1"),
    (56, "Use Context Sensitive Item 2"),
    (56, "Use Context Sensitive Item 3"),
    (92, "Shortcut: Items Inventory"),
    (90, "Shortcut: Weapons Inventory"),
    (96, "Shortcut: Magic Menu"),
    (91, "Shortcut: Clothing Inventory"),
    (97, "Shortcut: Expressions Menu"),
    (93, "Shortcut: Current Quests Screen"),
    (98, "Shortcut: Personality Screen"),
    (99, "Shortcut: Logbook"),
    (100, "Shortcut: Map Screen"),
    (55, "Use Hotbar Item 1"),
    (55, "Use Hotbar Item 2"),
    (55, "Use Hotbar Item 3"),
    (55, "Use Hotbar Item 4"),
    (55, "Use Hotbar Item 5"),
    (55, "Use Hotbar Item 6"),
    (55, "Use Hotbar Item 7"),
    (55, "Use Hotbar Item 8"),
    (55, "Use Hotbar Item 9"),
    (53, "Take Photojournal Photo"),
)
# The list planner selects authored ActionOrder children, while rendering
# consumes the movement-expanded row stream. ActionOrder child zero owns the
# first four visual rows; every later child starts one row farther on.
REDEFINE_ACTION_MATERIALIZED_OFFSETS = (
    0, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 43)
REDEFINE_SCROLL_PAGE_OFFSETS = tuple(range(1, 36))
REDEFINE_SCROLL_PAGE_COLUMNS = 5
REDEFINE_SCROLL_PAGE_ATLAS_SIZE = (3200, 3360)


def redefine_materialized_row_offset(action_index):
    """Return the expanded visual-row offset for a logical ActionOrder child."""
    if not 0 <= action_index < len(REDEFINE_FULL_ACTION_ORDER):
        raise ValueError("redefine action index is outside ActionOrder")
    return REDEFINE_ACTION_MATERIALIZED_OFFSETS[action_index]


def redefine_scrolled_rows(materialized_row_offset):
    """Return one nine-row Redefine viewport from the expanded row stream."""
    max_offset = len(REDEFINE_MATERIALIZED_ROWS) - len(REDEFINE_ROWS)
    if not 0 <= materialized_row_offset <= max_offset:
        raise ValueError("redefine viewport offset is outside the list")
    return REDEFINE_MATERIALIZED_ROWS[
        materialized_row_offset:materialized_row_offset + len(REDEFINE_ROWS)]


def build_redefine_scroll_pages(frontend_bank, font_bank):
    """Pack every nonzero Redefine viewport into a D3D9-sized page atlas."""
    atlas = Image.new("RGBA", REDEFINE_SCROLL_PAGE_ATLAS_SIZE, (0, 0, 0, 0))
    page_width, page_height = CANVAS_SIZE
    for page_index, row_offset in enumerate(REDEFINE_SCROLL_PAGE_OFFSETS):
        frame = build_redefine_frame(
            frontend_bank,
            font_bank,
            include_title_rule=False,
            include_title_text=False,
            include_key_text=False,
            include_action_text=False,
            materialized_row_offset=row_offset)
        column = page_index % REDEFINE_SCROLL_PAGE_COLUMNS
        row = page_index // REDEFINE_SCROLL_PAGE_COLUMNS
        atlas.alpha_composite(
            frame,
            (column * page_width, row * page_height))
    return atlas


def _redefine_materialized_key_text(row_index, action_id):
    if action_id == 60:
        return ("W", "A", "S", "D")[row_index]
    if action_id == 56:
        return ("F1", "F2", "F3")[row_index - 24]
    if action_id == 55:
        # Expanded row 34 is Hotbar 1; convert its zero-based row index to
        # the one-based hotbar value.
        return str(row_index - 33)
    if action_id in REDEFINE_ACTION_DISPLAY_INPUTS:
        return REDEFINE_ACTION_DISPLAY_INPUTS[action_id]
    return " / ".join(REDEFINE_ACTION_DEFAULT_INPUTS[action_id])


REDEFINE_KEY_VALUES = (
    "W / A / S / D",
    "LEFT MOUSE BUTTON",
    "MIDDLE MOUSE BUTTON",
    "RIGHT MOUSE BUTTON",
    "TAB",
    "Q",
    "E",
    "PRESS CONTROL",
    "UNDEFINED",
) + tuple(chr(code) for code in range(ord("A"), ord("Z") + 1)) + (
    "SPACE",
    "LEFT SHIFT",
    "Left Ctrl",
    "Left Alt",
    "Enter",
    "Backspace",
    "Up",
    "Down",
    "Left",
    "Right",
) + tuple(str(value) for value in range(10)) + (
    "CAPS LOCK",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
    "F11", "F12", "PRNT SCRN",
)

# Serialized component transforms:
# list #217 (40,115), table #91 (-32,-2), right table child #470
# (368,-3), and generated text children #410/#411 at (0,3)/(380,3).
REDEFINE_LIST_ORIGIN = (40, 115)
REDEFINE_ROW_STEP_Y = 26
REDEFINE_TABLE_OFFSET = (-32, -2)
REDEFINE_RIGHT_SLOT_OFFSET = (368, -3)
REDEFINE_ACTION_TEXT_OFFSET = (0, 3)
REDEFINE_KEY_TEXT_OFFSET = (380, 3)
# The compiled child retains its authored position/Zoom values, but the
# supplied retail Redefine capture measures action-name advances at roughly
# 9/8 of the raw ENG_ARIAL_12 bake. Keep the key-value tiles at native scale;
# this correction applies only to the action-name atlas layer.
REDEFINE_ACTION_TEXT_SCALE = 9.0 / 8.0
# CText::Draw rounds and forwards the serialized y origin unchanged. Text
# alignment adjusts x only; the static renderer must not invent vertical
# centering on either table primitive.
REDEFINE_TEXT_RENDER_Y_BIAS = 0
REDEFINE_MOUSE_OFFSET = (-40, 0)
REDEFINE_MOUSE_SIZE = (600, 24)


def validate_compiled_subscreen_layout(game_root, schema_path):
    """Gate submenu/header/Redefine constants against frontend.bin."""
    layout = FrontendLayoutOracle(game_root, schema_path)

    options_list = "UI_FRONTEND_LIST_OPTIONS_SUB_MENU"
    options_values = layout.decoded(options_list)
    require_position(
        "Options list origin",
        layout.initial_position(options_list),
        OPTIONS_LIST_ORIGIN)
    require_equal(
        "Options list step",
        options_values["PositionOffsetY"],
        30.0)
    require_equal(
        "Options list wrapping",
        options_values["Wrapping"],
        True)
    require_equal(
        "Options list scrolling",
        options_values["Scrolling"],
        False)
    require_equal(
        "Options list alpha offset",
        options_values["AlphaOffset"],
        "00000000")
    sound_values = layout.decoded("UI_MISC_THINGS_DEF_INSTANCE")
    require_equal(
        "Options movement sound",
        sound_values["SoundUpDown"],
        "CS_GUI_2")
    require_equal(
        "Options invalid movement sound",
        sound_values["SoundError"],
        "CS_GUI_5")
    option_buttons = (
        "UI_OPTIONS_BUTTON_GAME_OPTIONS",
        "UI_OPTIONS_BUTTON_VIDEO",
        "UI_FRONTEND_BUTTON_AUDIO_OPTIONS",
        "UI_OPTIONS_BUTTON_REDEFINE_KEYS",
    )
    require_equal(
        "Options child order",
        layout.child_names(options_list),
        option_buttons)
    require_equal(
        "Options actions",
        tuple(layout.decoded(name)["Action"] for name in option_buttons),
        (9, 13, 12, 283))
    require_equal(
        "Options row offsets",
        tuple(offset for _, offset in OPTIONS_ROWS),
        tuple(
            int(options_values["PositionOffsetY"]) * index
            for index in range(len(option_buttons))))

    save_list = "UI_FRONTEND_LIST_FOR_SAVES"
    save_values = layout.decoded(save_list)
    require_position(
        "Saved-games list origin",
        layout.initial_position(save_list),
        SAVE_LIST_ORIGIN)
    require_equal(
        "Saved-games list step",
        save_values["PositionOffsetY"],
        float(SAVE_ROW_STEP_Y))
    require_equal(
        "Saved-games list visible height",
        save_values["Height"],
        float(SAVE_LIST_HEIGHT))
    require_equal(
        "Saved-games list wrapping",
        save_values["Wrapping"],
        True)
    require_equal(
        "Saved-games list scrolling",
        save_values["Scrolling"],
        True)
    require_equal(
        "Saved-games list up arrow",
        save_values["UpArrow"],
        428)
    require_equal(
        "Saved-games list down arrow",
        save_values["DownArrow"],
        425)
    require_position(
        "Saved-games up-arrow position",
        layout.initial_position("UI_SAVE_LIST_ARROW_UP"),
        (270, 80))
    require_position(
        "Saved-games down-arrow position",
        layout.initial_position("UI_SAVE_LIST_ARROW_DOWN"),
        (270, 220))
    require_equal(
        "Saved-games button child",
        layout.child_names("UI_FRONTEND_BUTTON_FOR_SAVE_LIST"),
        ("UI_BUTTON_MOUSE_AREA_SAVE_GAME",))
    require_equal(
        "Saved-games button table width",
        layout.decoded("UI_BUTTON_FOR_SAVE_NAME")["Width"],
        120.0)
    save_menu = "UI_FRONTEND_PROFILE_SAVED_GAMES_MENU"
    require_equal(
        "Saved-games parent child order",
        layout.child_names(save_menu),
        (
            "UI_TEXT_PROFILE_SAVED_GAMES_MENU_TITLE",
            "UI_BLENDING_BACKGROUNDS_FORREST",
            "UI_TITLE_AREA",
            "UI_HELPERS",
            "UI_VIEW_RING_SMALL",
            "UI_TEXT_AREA",
            "UI_BOTTOM_BACKDROP",
        ))
    require_position(
        "Saved-games title-area origin",
        layout.initial_position("UI_TITLE_AREA"),
        SAVE_TITLE_AREA_ORIGIN)
    require_equal(
        "Saved-games title-area children",
        layout.child_names("UI_TITLE_AREA"),
        ("UI_TABLE_TITLE_LEFT", "UI_TABLE_TITLE_RIGHT"))
    require_equal(
        "Saved-games title-left width",
        layout.decoded("UI_TABLE_TITLE_LEFT")["Width"],
        float(SAVE_TITLE_AREA_LEFT_WIDTH))
    require_position(
        "Saved-games title-right position",
        layout.initial_position("UI_TABLE_TITLE_RIGHT"),
        (SAVE_TITLE_AREA_RIGHT[0], 0))
    require_equal(
        "Saved-games title-right width",
        layout.decoded("UI_TABLE_TITLE_RIGHT")["Width"],
        float(SAVE_TITLE_AREA_RIGHT[1]))
    require_equal(
        "Saved-games title-left table sprites",
        layout.table_sprites("UI_TABLE_TITLE_LEFT"),
        (
            (0, 122, "UI_TEXTBOX_MIDDLE"),
            (1, 297, "UI_TEXTBOX_TL"),
            (4, 122, "UI_TEXTBOX_MIDDLE"),
        ))
    require_equal(
        "Saved-games title-right table sprites",
        layout.table_sprites("UI_TABLE_TITLE_RIGHT"),
        (
            (0, 298, "UI_TEXTBOX_TR"),
            (1, 122, "UI_TEXTBOX_MIDDLE"),
            (4, 122, "UI_TEXTBOX_MIDDLE"),
        ))
    require_equal(
        "Saved-games text-area children",
        layout.child_names("UI_TEXT_AREA"),
        ("UI_TABLE_TEXT_LEFT", "UI_TABLE_TEXT_RIGHT", "UI_TEXT_BOTTOM"))
    require_position(
        "Saved-games text-area origin",
        layout.initial_position("UI_TEXT_AREA"),
        SAVE_TEXT_AREA_ORIGIN)
    require_equal(
        "Saved-games text-left table sprites",
        layout.table_sprites("UI_TABLE_TEXT_LEFT"),
        (
            (0, 122, "UI_TEXTBOX_MIDDLE"),
            (1, 538, "UI_TEXTBOX_BL"),
            (4, 122, "UI_TEXTBOX_MIDDLE"),
        ))
    require_equal(
        "Saved-games text-right table sprites",
        layout.table_sprites("UI_TABLE_TEXT_RIGHT"),
        (
            (0, 299, "UI_TEXTBOX_BR"),
            (1, 122, "UI_TEXTBOX_MIDDLE"),
            (4, 122, "UI_TEXTBOX_MIDDLE"),
        ))
    text_bottom = layout.decoded("UI_TEXT_BOTTOM")
    require_equal(
        "Saved-games text-bottom graphic",
        text_bottom["States"][0]["GraphicIndex"],
        89)
    require_position(
        "Saved-games text-bottom resolved position",
        (
            layout.initial_position("UI_TEXT_AREA")[0] +
            text_bottom["States"][0]["PositionX"],
            layout.initial_position("UI_TEXT_AREA")[1] +
            text_bottom["States"][0]["PositionY"]),
        SAVE_TEXT_BOTTOM_ORIGIN)
    require_equal(
        "Saved-games text-bottom zoom",
        (
            text_bottom["States"][0]["ZoomX"],
            text_bottom["States"][0]["ZoomY"]),
        (160.0, 1.0))
    bottom_backdrop = layout.decoded("UI_BOTTOM_BACKDROP")
    require_position(
        "Saved-games bottom-backdrop origin",
        layout.initial_position("UI_BOTTOM_BACKDROP"),
        SAVE_BOTTOM_BACKDROP_ORIGIN)
    require_equal(
        "Saved-games bottom-backdrop graphic",
        bottom_backdrop["States"][0]["GraphicIndex"],
        98)
    require_equal(
        "Saved-games bottom-backdrop zoom",
        (
            bottom_backdrop["States"][0]["ZoomX"],
            bottom_backdrop["States"][0]["ZoomY"]),
        (160.0, 62.0))
    option_text_names = (
        "UI_TEXT_GAME_OPTIONS",
        "UI_OPTIONS_BUTTON_VIDEO_TEXT",
        "UI_TEXT_AUDIO_OPTIONS",
        "UI_OPTIONS_BUTTON_REDEFINE_KEYS_TEXT",
    )
    for index, (button_name, text_name) in enumerate(
            zip(option_buttons, option_text_names)):
        inner_name = layout.child_names(button_name)[0]
        require_equal(
            "Options text child %d" % index,
            layout.child_names(inner_name)[0],
            text_name)
        text_values = layout.decoded(text_name)
        require_equal(
            "Options text font %d" % index,
            text_values["Font"],
            "ENG_ARIAL_24")
        states = text_values["States"]
        require_equal(
            "Options text state count %d" % index,
            len(states),
            5)
        require_equal(
            "Options text state x %d" % index,
            tuple(state["PositionX"] for state in states),
            (120.0, 120.0, 0.0, 120.0, 120.0))
        require_equal(
            "Options text state y %d" % index,
            tuple(state["PositionY"] for state in states),
            (0.0, 0.0, 0.0, 0.0, 0.0))
        require_equal(
            "Options text state alpha %d" % index,
            tuple(state["ColourA"] for state in states),
            (1.0, 1.0, 0.0, 1.0, 1.0))
        require_equal(
            "Options text state time %d" % index,
            tuple(state["UpdateTime"] for state in states),
            (-1.0, -1.0, 0.20000000298023224, -1.0, -1.0))
        require_equal(
            "Options text state flags %d" % index,
            tuple(state["StateChangeFlag"] for state in states),
            (7, 7, 7, 7, 7))

    title_rule = "UI_TABLE_TITLE_WHOLE"
    require_position(
        "screen-title rule position",
        layout.initial_position(title_rule),
        HEADER_RULE_POSITION)
    require_equal(
        "screen-title rule width",
        layout.decoded(title_rule)["Width"],
        float(CANVAS_SIZE[0]))
    require_equal(
        "screen-title expansion type",
        layout.decoded(title_rule)["ExpansionType"],
        1)
    require_equal(
        "screen-title table sprites",
        layout.table_sprites(title_rule),
        (
            (0, 122, "UI_TEXTBOX_MIDDLE"),
            (1, 122, "UI_TEXTBOX_MIDDLE"),
            (4, 122, "UI_TEXTBOX_MIDDLE"),
        ))
    require_equal(
        "Options selected-button width",
        layout.decoded("UI_BUTTON")["Width"],
        280.0)
    require_position(
        "Options selected-button table offset",
        layout.initial_position("UI_BUTTON"),
        (-80, -7))
    require_equal(
        "Options selected-button table sprites",
        layout.table_sprites("UI_BUTTON"),
        (
            (0, 129, "UI_FRONTEND_BUTTON_L"),
            (1, 130, "UI_FRONTEND_BUTTON_R"),
            (4, 131, "UI_FRONTEND_BUTTON_M"),
        ))
    require_equal(
        "Options selected-button graphic indices",
        tuple(
            layout.decoded(component)["States"][0]["GraphicIndex"]
            for component in (
                "UI_FRONTEND_BUTTON_L",
                "UI_FRONTEND_BUTTON_R",
                "UI_FRONTEND_BUTTON_M",
            )),
        (364, 366, 365))
    for name in (
            "UI_TEXT_OPTIONS_MENU_TITLE",
            "UI_TEXT_GAME_OPTIONS_MENU_TITLE",
            "UI_TEXT_AUDIO_OPTIONS_MENU_TITLE",
            "UI_TEXT_VIDEO_MENU_TITLE",
            "UI_TEXT_REDEFINE_KEYS_MENU_TITLE"):
        values = layout.decoded(name)
        require_position(
            "%s serialized position" % name,
            layout.initial_position(name),
            (65, HEADER_TEXT_POSITION[1]))
        require_equal(
            "%s font" % name,
            values["Font"],
            "ENG_ARIAL_24")

    redefine_list = "UI_FRONTEND_LIST_REDEFINE_KEYS_MENU"
    redefine_values = layout.decoded(redefine_list)
    require_position(
        "Redefine list origin",
        layout.initial_position(redefine_list),
        REDEFINE_LIST_ORIGIN)
    require_equal(
        "Redefine list step",
        redefine_values["PositionOffsetY"],
        float(REDEFINE_ROW_STEP_Y))
    require_equal(
        "Redefine visible height",
        redefine_values["Height"],
        250.0)
    require_equal(
        "Redefine list up arrow",
        redefine_values["UpArrow"],
        417)
    require_equal(
        "Redefine list down arrow",
        redefine_values["DownArrow"],
        420)
    require_equal(
        "Redefine list scrolling",
        redefine_values["Scrolling"],
        True)
    require_position(
        "Redefine up-arrow position",
        layout.initial_position("UI_REDEFINER_LIST_ARROW_UP"),
        (304, 80))
    require_position(
        "Redefine down-arrow position",
        layout.initial_position("UI_REDEFINER_LIST_ARROW_DOWN"),
        (304, 350))
    require_equal(
        "Redefine action order",
        tuple(redefine_values["ActionOrder"][:len(REDEFINE_ACTION_ORDER)]),
        REDEFINE_ACTION_ORDER)
    require_position(
        "Redefine table offset",
        layout.initial_position("UI_OPTIONS_REDEFINE_TABLES"),
        REDEFINE_TABLE_OFFSET)
    require_position(
        "Redefine right slot offset",
        layout.initial_position("UI_BUTTON_REDEFINE_RIGHT"),
        REDEFINE_RIGHT_SLOT_OFFSET)
    require_equal(
        "Redefine left slot sprites",
        layout.table_sprites("UI_BUTTON_REDEFINE_LEFT"),
        (
            (0, 356, "UI_OPTIONS_TEXT_SLOT_L"),
            (1, 354, "UI_OPTIONS_TEXT_SLOT_R"),
            (4, 355, "UI_OPTIONS_TEXT_SLOT_M"),
        ))
    require_equal(
        "Redefine right bar sprites",
        layout.table_sprites("UI_BUTTON_REDEFINE_RIGHT"),
        (
            (0, 357, "UI_OPTIONS_HORIZONTAL_BAR"),
            (1, 357, "UI_OPTIONS_HORIZONTAL_BAR"),
            (4, 357, "UI_OPTIONS_HORIZONTAL_BAR"),
        ))
    require_equal(
        "Redefine right bar graphic",
        layout.decoded("UI_OPTIONS_HORIZONTAL_BAR")["States"][0][
            "GraphicIndex"],
        385)
    require_position(
        "Redefine action text offset",
        layout.initial_position("UI_KEY_REDEFINER_ACTION_TEXT"),
        REDEFINE_ACTION_TEXT_OFFSET)
    require_position(
        "Redefine key text offset",
        layout.initial_position("UI_KEY_REDEFINER_KEY_TEXT"),
        REDEFINE_KEY_TEXT_OFFSET)
    for name in (
            "UI_KEY_REDEFINER_ACTION_TEXT",
            "UI_KEY_REDEFINER_KEY_TEXT"):
        require_equal(
            "%s font" % name,
            layout.decoded(name)["Font"],
            "ENG_ARIAL_12")
    require_position(
        "Redefine mouse offset",
        layout.initial_position("UI_REDEFINER_MOUSE_AREA"),
        REDEFINE_MOUSE_OFFSET)
    mouse_values = layout.decoded("UI_REDEFINER_MOUSE_AREA")
    require_equal(
        "Redefine mouse size",
        (mouse_values["Width"], mouse_values["Height"]),
        tuple(float(value) for value in REDEFINE_MOUSE_SIZE))


def validate_compiled_about_layout(game_root, schema_path):
    """Gate the recovered About screen constants against frontend.bin.

    ``UI_FRONTEND_ABOUT_MENU`` (#449) is the destination of main-menu action
    321 -> used key 0x1c.  It is a Type-10 frontend screen structurally
    identical to ``UI_FRONTEND_OPTIONS_SUB_MENU`` (#211) with the interactive
    list replaced by a single static wrapped message, so it carries no
    selectable rows -- the only exit is Back/action 86.  These asserts lock the
    decoded child order, the title and message text symbols/fonts/origins, the
    SPOOKY background, and the shared 640-pixel title rule.
    """
    layout = FrontendLayoutOracle(game_root, schema_path)

    about_menu = "UI_FRONTEND_ABOUT_MENU"
    about_values = layout.decoded(about_menu)
    require_equal(
        "About screen type",
        about_values["Type"],
        10)
    require_equal(
        "About child order",
        layout.child_names(about_menu),
        (
            "UI_FRONTEND_ABOUT_MESSAGE",
            "UI_TEXT_ABOUT_MENU_TITLE",
            "UI_BLENDING_BACKGROUNDS_SPOOKY",
            "UI_TABLE_TITLE_WHOLE_ABOUT",
            "UI_HELPERS",
        ))

    title_text = layout.decoded("UI_TEXT_ABOUT_MENU_TITLE")
    require_equal(
        "About title symbol",
        title_text["TextValue"],
        "TEXT_GUI_MENU_ABOUT_TITLE")
    require_equal(
        "About title font",
        title_text["Font"],
        "ENG_ARIAL_24")
    require_position(
        "About title serialized position",
        layout.initial_position("UI_TEXT_ABOUT_MENU_TITLE"),
        (65, 14))

    message_text = layout.decoded("UI_FRONTEND_ABOUT_MESSAGE")
    require_equal(
        "About message symbol",
        message_text["TextValue"],
        "TEXT_GUI_MENU_ABOUT_MESSAGE")
    require_equal(
        "About message font",
        message_text["Font"],
        "ENG_ARIAL_12")
    require_position(
        "About message serialized position",
        layout.initial_position("UI_FRONTEND_ABOUT_MESSAGE"),
        (320, 60))
    require_equal(
        "About message text window",
        (message_text["TextWindowBRX"], message_text["TextWindowBRY"]),
        (700.0, 5000.0))

    about_rule = "UI_TABLE_TITLE_WHOLE_ABOUT"
    require_equal(
        "About title rule width",
        layout.decoded(about_rule)["Width"],
        float(CANVAS_SIZE[0]))
    require_position(
        "About title rule position",
        layout.initial_position(about_rule),
        (0, 5))
    require_equal(
        "About title rule sprites",
        layout.table_sprites(about_rule),
        (
            (0, 122, "UI_TEXTBOX_MIDDLE"),
            (1, 122, "UI_TEXTBOX_MIDDLE"),
            (4, 122, "UI_TEXTBOX_MIDDLE"),
        ))

    require_equal(
        "About background blend children",
        layout.child_names("UI_BLENDING_BACKGROUNDS_SPOOKY"),
        (
            "UI_SWAPPING_SPOOKY_SUNBEAM",
            "UI_SWAPPING_SPOOKY",
        ))


def validate_compiled_credits_layout(game_root, schema_path):
    """Gate the recovered Credits screen structure against frontend.bin.

    The Credits route is a scrolling screen, not a static title panel.  Keep
    its authored child order, scroll transition, source text groups, and
    shared Back/widescreen helpers explicit before wiring a runtime surface.
    """
    layout = FrontendLayoutOracle(game_root, schema_path)
    credits_menu = "UI_FRONTEND_CREDITS_MENU"
    require_equal(
        "Credits screen type",
        layout.decoded(credits_menu)["Type"],
        10)
    require_equal(
        "Credits child order",
        layout.child_names(credits_menu),
        (
            "UI_BLENDING_BACKGROUNDS_FORREST",
            "UI_SCROLLING_TEST",
            "UI_HELPERS_CREDITS",
            "UI_WIDESCREEN_BAR_TOP",
            "UI_WIDESCREEN_BAR_BOTTOM",
        ))

    require_equal(
        "Credits background swap children",
        layout.child_names("UI_BLENDING_BACKGROUNDS_FORREST"),
        (
            "UI_SWAPPING_FORREST_SUNBEAM",
            "UI_SWAPPING_FORREST",
        ))
    scrolling = layout.decoded("UI_SCROLLING_TEST")
    require_equal("Credits scrolling type", scrolling["Type"], 19)
    require_position(
        "Credits scroll start",
        layout.initial_position("UI_SCROLLING_TEST"),
        (0, 480))
    require_equal(
        "Credits scroll transition",
        [(state["PositionX"], state["PositionY"], state["UpdateTime"])
         for state in scrolling["States"][:4]],
        [
            (0.0, 480.0, 0.0),
            (0.0, 480.0, 0.0),
            (0.0, 480.0, 0.0),
            (0.0, 0.0, 180.0),
        ])

    require_equal(
        "Credits container children",
        layout.child_names("UI_CREDITS_CONTAINER"),
        (
            "UI_TITLE",
            "UI_CREDITS_TEXT_EMPTY",
            "UI_CREDITS_TEXT_EMPTY",
            "UI_CREDITS_TEXT_EMPTY",
            "UI_CREDITS_MAIN1",
            "UI_CREDITS_MAIN2",
            "UI_CREDITS_TESTSUP",
            "UI_CREDITS_MICROSOFT",
            "UI_CREDITS_TEST",
            "UI_CREDITS_THANKS",
        ))
    require_equal(
        "Credits title children",
        layout.child_names("UI_TITLE"),
        ("UI_TITLE_01", "UI_TITLE_02"))
    require_position(
        "Credits title origin",
        layout.initial_position("UI_TITLE"),
        (70, 30))

    for name, symbol, font in CREDITS_TEXT_GROUPS:
        values = layout.decoded(name)
        require_equal("%s text symbol" % name, values["TextValue"], symbol)
        require_equal("%s font" % name, values["Font"], font)
        require_position("%s serialized position" % name,
                         layout.initial_position(name), (75, 0))

    require_equal(
        "Credits helper child",
        layout.child_names("UI_HELPERS_CREDITS"),
        ("UI_BACK",))
    require_equal(
        "Credits top bar graphic",
        layout.decoded("UI_WIDESCREEN_BAR_TOP")["States"][0]["GraphicIndex"],
        334)
    require_equal(
        "Credits bottom bar origin",
        layout.initial_position("UI_WIDESCREEN_BAR_BOTTOM"),
        (0, 408))


def validate_compiled_profiles_layout(game_root, schema_path):
    """Gate the shipped Change Profile graph against ``frontend.bin``.

    Profile rows are populated by the game at runtime.  The compiled list
    records therefore intentionally have no serialized children; this oracle
    locks the container/template contract and the two authored empty-profile
    branches without inventing profile names or row pixels.
    """
    layout = FrontendLayoutOracle(game_root, schema_path)

    require_equal(
        "Profiles screen type",
        layout.decoded("UI_FRONTEND_PROFILES_MENU")["Type"],
        10)
    require_equal(
        "Profiles screen child order",
        layout.child_names("UI_FRONTEND_PROFILES_MENU"),
        (
            "UI_TEXT_PROFILES_MENU_TITLE",
            "UI_BLENDING_BACKGROUNDS_COASTAL",
            "UI_TABLE_TITLE_WHOLE",
            "UI_HELPERS_PROFILE_DELETE",
        ))
    require_equal(
        "Profiles delete screen child order",
        layout.child_names("UI_FRONTEND_PROFILES_MENU_E3"),
        (
            "UI_TEXT_PROFILES_MENU_TITLE",
            "UI_BLENDING_BACKGROUNDS_SPOOKY",
            "UI_TABLE_TITLE_WHOLE",
            "UI_HELPERS",
        ))

    delete_confirm = layout.decoded("UI_FRONTEND_DELETE_PROFILE_MENU")
    require_equal(
        "Delete confirmation child order",
        layout.child_names("UI_FRONTEND_DELETE_PROFILE_MENU"),
        (
            "UI_TEXT_DELETE_PROFILE_MENU_TITLE",
            "UI_BLENDING_BACKGROUNDS_SPOOKY",
            "UI_TABLE_TITLE_WHOLE",
            "UI_FRONTEND_DELETE_PROFILE_YES",
            "UI_FRONTEND_DELETE_PROFILE_NO",
            "UI_DELETE_PROFILE_TEXT",
        ))
    require_equal(
        "Delete confirmation type",
        delete_confirm["Type"],
        10)
    require_equal(
        "Delete confirmation title text",
        layout.decoded("UI_TEXT_DELETE_PROFILE_MENU_TITLE")["TextValue"],
        "TEXT_GUI_MENU_DELETE_PROFILE")
    require_equal(
        "Delete confirmation prompt text",
        layout.decoded("UI_TEXT_DELETE_PROFILE_PROMPT")["TextValue"],
        "TEXT_GUI_MENU_DELETE_PROFILE_PROMPT")
    require_equal(
        "Delete confirmation explanation text",
        layout.decoded("UI_DELETE_PROFILE_TEXT")["TextValue"],
        "TEXT_GUI_MENU_DELETE_PROFILE_EXPLANATION")
    require_equal(
        "Delete confirmation title font",
        layout.decoded("UI_TEXT_DELETE_PROFILE_MENU_TITLE")["Font"],
        "ENG_ARIAL_24")
    require_equal(
        "Delete confirmation prompt font",
        layout.decoded("UI_TEXT_DELETE_PROFILE_PROMPT")["Font"],
        "ENG_ARIAL_24")
    require_equal(
        "Delete confirmation explanation font",
        layout.decoded("UI_DELETE_PROFILE_TEXT")["Font"],
        "ENG_ARIAL_24")
    require_position(
        "Delete confirmation title position",
        layout.initial_position("UI_TEXT_DELETE_PROFILE_MENU_TITLE"),
        (65, 44))
    require_position(
        "Delete confirmation prompt position",
        layout.initial_position("UI_TEXT_DELETE_PROFILE_PROMPT"),
        (320, 100))
    require_position(
        "Delete confirmation explanation position",
        layout.initial_position("UI_DELETE_PROFILE_TEXT"),
        (320, 240))
    require_position(
        "Delete confirmation yes position",
        layout.initial_position("UI_FRONTEND_DELETE_PROFILE_YES"),
        (362, 405))
    require_position(
        "Delete confirmation no position",
        layout.initial_position("UI_FRONTEND_DELETE_PROFILE_NO"),
        (20, 405))

    title = layout.decoded("UI_TEXT_PROFILES_MENU_TITLE")
    require_equal(
        "Profiles title symbol",
        title["TextValue"],
        "TEXT_GUI_MENU_SELECT_PROFILE")
    require_equal("Profiles title font", title["Font"], "ENG_ARIAL_24")
    require_position(
        "Profiles title serialized position",
        layout.initial_position("UI_TEXT_PROFILES_MENU_TITLE"),
        (65, 44))

    profile_list = layout.decoded("UI_FRONTEND_LIST_FOR_PROFILES")
    require_equal("Profiles list type", profile_list["Type"], 43)
    require_equal("Profiles list serialized children", profile_list["Children"], [])
    require_position(
        "Profiles list origin",
        layout.initial_position("UI_FRONTEND_LIST_FOR_PROFILES"),
        (200, 120))
    require_equal("Profiles list height", profile_list["Height"], 260.0)
    require_equal(
        "Profiles list row step",
        profile_list["PositionOffsetY"],
        float(PROFILE_ROW_STEP))
    require_equal("Profiles list layer", profile_list["Layer"], 7)
    require_equal("Profiles list up arrow", profile_list["UpArrow"], 426)
    require_equal("Profiles list down arrow", profile_list["DownArrow"], 423)
    require_equal("Profiles list scrolling", profile_list["Scrolling"], True)
    require_position(
        "Profiles up-arrow position",
        layout.initial_position("UI_PROFILE_LIST_ARROW_UP"),
        (304, 80))
    require_position(
        "Profiles down-arrow position",
        layout.initial_position("UI_PROFILE_LIST_ARROW_DOWN"),
        (304, 380))

    delete_list = layout.decoded("UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE")
    require_equal("Profiles delete list type", delete_list["Type"], 43)
    require_equal(
        "Profiles delete list serialized children",
        delete_list["Children"],
        [])
    require_position(
        "Profiles delete list origin",
        layout.initial_position("UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE"),
        (200, 180))
    require_equal("Profiles delete list height", delete_list["Height"], 210.0)
    require_equal("Profiles delete list layer", delete_list["Layer"], 7)
    require_equal("Profiles delete list up arrow", delete_list["UpArrow"], 427)
    require_equal("Profiles delete list down arrow", delete_list["DownArrow"], 424)
    require_equal("Profiles delete list scrolling", delete_list["Scrolling"], True)
    require_position(
        "Profiles delete up-arrow position",
        layout.initial_position("UI_DELETE_LIST_ARROW_UP"),
        (304, 140))
    require_position(
        "Profiles delete down-arrow position",
        layout.initial_position("UI_DELETE_LIST_ARROW_DOWN"),
        (304, 380))

    require_equal(
        "Profiles button template type",
        layout.decoded("UI_FRONTEND_BUTTON_FOR_PROFILES_LIST")["Type"],
        11)
    require_equal(
        "Profiles helper children",
        layout.child_names("UI_HELPERS_PROFILE_DELETE"),
        ("UI_DELETE", "UI_BACK"))
    require_equal(
        "Profiles shared helper children",
        layout.child_names("UI_HELPERS"),
        ("UI_BACK",))

    title_rule = layout.decoded("UI_TABLE_TITLE_WHOLE")
    require_equal("Profiles title rule type", title_rule["Type"], 2)
    require_equal("Profiles title rule width", title_rule["Width"], 640.0)
    require_equal("Profiles title rule layer", title_rule["Layer"], 6)
    require_position(
        "Profiles title rule origin",
        layout.initial_position("UI_TABLE_TITLE_WHOLE"),
        (0, 35))

    require_equal(
        "No-profile screen child order",
        layout.child_names("UI_FRONTEND_NO_PROFILES_MENU"),
        (
            "UI_TEXT_NO_PROFILES_MENU_TITLE",
            "UI_BLENDING_BACKGROUNDS_FORREST",
            "UI_TABLE_TITLE_WHOLE",
            "UI_HELPERS",
            "UI_NO_PROFILES_TEXT",
        ))
    no_profiles = layout.decoded("UI_NO_PROFILES_TEXT")
    require_equal(
        "No-profile message symbol",
        no_profiles["TextValue"],
        "TEXT_GUI_MENU_NEED_NEW_PROFILE")
    require_equal("No-profile message font", no_profiles["Font"], "ENG_ARIAL_24")
    require_position(
        "No-profile message origin",
        layout.initial_position("UI_NO_PROFILES_TEXT"),
        (320, 200))

    require_equal(
        "New-profile screen child order",
        layout.child_names("UI_FRONTEND_NEW_PROFILE_SCREEN"),
        (
            "UI_TEXT_NEW_PROFILE_MENU_TITLE",
            "UI_BLENDING_BACKGROUNDS_COASTAL",
            "UI_TABLE_TITLE_WHOLE",
            "UI_NEW_PROFILE_MENU",
            "UI_HELPERS_NEW_PROFILE",
        ))
    new_profile = layout.decoded("UI_NEW_PROFILE_MENU")
    require_equal("New-profile menu type", new_profile["Type"], 12)
    require_position(
        "New-profile menu origin",
        layout.initial_position("UI_NEW_PROFILE_MENU"),
        (40, 150))


def extract_credits_text_stream(text_bank):
    """Return the authored Credits groups without flattening or rewriting.

    ``TextValue`` points at type-1 groups in ``text.big``.  Keeping the
    member IDs and decoded strings together is deliberate: a future live
    renderer must preserve the retail group order, including the explicit
    single-space entries used as blank rows.  This helper is an extraction
    oracle only; it does not guess line spacing or scroll timing.
    """
    bank = TextBank(text_bank)
    result = []
    for widget, symbol, font in CREDITS_TEXT_GROUPS:
        group = bank.decode(symbol)
        if group["type"] != 1:
            raise ValueError("Credits text symbol is not a group: %s" % symbol)
        members = []
        for member_id in group["members"]:
            member = bank.by_id.get(member_id)
            if member is None or member["type"] != 0:
                raise ValueError(
                    "Credits group member is not a string: %s[%d]" %
                    (symbol, member_id))
            decoded = bank.decode(member["name"])
            members.append(decoded["content"])
        result.append({
            "widget": widget,
            "symbol": symbol,
            "font": font,
            "member_ids": tuple(group["members"]),
            "members": tuple(members),
        })
    return tuple(result)


def _build_stretched(left, middle, right, width):
    """Assemble one retail three-piece horizontal sprite."""
    if width < left.width + right.width:
        raise ValueError("three-piece sprite width is too small")
    result = Image.new("RGBA", (width, max(
        left.height, middle.height, right.height)), (0, 0, 0, 0))
    result.alpha_composite(left, (0, 0))
    inner_width = width - left.width - right.width
    for x in range(0, inner_width, middle.width):
        available = min(middle.width, inner_width - x)
        result.alpha_composite(
            middle.crop((0, 0, available, middle.height)),
            (left.width + x, 0))
    result.alpha_composite(right, (width - right.width, 0))
    return result


def _build_table_horizontal(left, primary, right, table_width):
    """Reproduce CTable key-0/key-4/key-1 horizontal composition.

    ``left`` is sprite key 0, ``primary`` is key 4, and ``right`` is key 1.
    The slightly non-visual argument order is intentional: it follows
    ``CTable::ConstructSpritesToDraw`` rather than treating the decoded sprite
    map as a left/middle/right list.
    """
    if primary.width <= 0:
        raise ValueError("table primary sprite has no width")
    repeat_count = int(table_width / primary.width)
    inner_width = primary.width * repeat_count
    result = Image.new(
        "RGBA",
        (
            left.width + inner_width + right.width,
            max(left.height, primary.height, right.height),
        ),
        (0, 0, 0, 0))
    result.alpha_composite(left, (0, 0))
    if inner_width:
        # ConstructHorizontalLine emits one primary component and applies the
        # repeat count as ZoomX; it does not emit one component per tile.
        stretched = primary.resize((inner_width, primary.height))
        result.alpha_composite(stretched, (left.width, 0))
    result.alpha_composite(right, (left.width + inner_width, 0))
    return result


def _draw_text(
        canvas,
        font,
        text,
        position,
        align="center",
        scale=2.0 / 3.0,
        colour=None):
    line = render_line(
        font,
        text,
        CANVAS_SIZE,
        position,
        align,
        scale)
    if colour is not None:
        tinted = Image.new("RGBA", line.size, colour)
        tinted.putalpha(line.getchannel("A"))
        line = tinted
    canvas.alpha_composite(add_outline(line, 1))


def _draw_title(
        canvas,
        frontend_bank_data,
        font,
        text,
        include_rule=True,
        include_text=True):
    buf, parsed = frontend_bank_data
    # UI_TABLE_TITLE_WHOLE maps TOP_LEFT, TOP_RIGHT, and HORIZONTAL_TOP to
    # the same UI_TEXTBOX_MIDDLE component (#122). Its GraphicIndex resolves
    # to this installed gold double-rule sprite; the blue FRONTEND_BUTTON
    # trio belongs to UI_BUTTON/UI_BUTTON_BIG selection tables.
    if include_rule:
        title_segment = _decode_named(
            buf, parsed, "UI_TEXTBOX_MIDDLE_FE_SPRITE")
        # CTable treats Width as the tiled inner span. Natural-size corner
        # components sit around that span; the 640-pixel design canvas clips
        # the outer tail exactly as the retail viewport does.
        title_rule = _build_table_horizontal(
            title_segment,
            title_segment,
            title_segment,
            640)
        canvas.alpha_composite(title_rule, HEADER_RULE_POSITION)
    if include_text:
        _draw_text(canvas, font, text, HEADER_TEXT_POSITION, "left")


def _draw_save_title(
        canvas,
        frontend_bank_data,
        font,
        text,
        include_rule=True,
        include_text=True):
    """Draw the saved-games UI_TITLE_AREA from its two retail tables."""
    if include_rule:
        buf, parsed = frontend_bank_data
        middle = _decode_named(
            buf, parsed, "UI_TEXTBOX_MIDDLE_FE_SPRITE")
        title_left = _build_table_horizontal(
            middle,
            middle,
            _decode_named(buf, parsed, "UI_TEXTBOX_TL_SPRITE_FE"),
            SAVE_TITLE_AREA_LEFT_WIDTH)
        title_right = _build_table_horizontal(
            _decode_named(buf, parsed, "UI_TEXTBOX_TR_SPRITE_FE"),
            middle,
            middle,
            SAVE_TITLE_AREA_RIGHT[1])
        canvas.alpha_composite(title_left, SAVE_TITLE_AREA_ORIGIN)
        canvas.alpha_composite(
            title_right,
            (SAVE_TITLE_AREA_RIGHT[0], SAVE_TITLE_AREA_ORIGIN[1]))
    if include_text:
        _draw_text(canvas, font, text, HEADER_TEXT_POSITION, "left")


def _draw_option_row(
        canvas,
        font,
        assets,
        label,
        y,
        text_value=None,
        slider_value=0.0):
    _draw_text(canvas, font, label, (32, y + 2), "left")
    canvas.alpha_composite(assets["left_arrow"], (308, y - 3))
    canvas.alpha_composite(assets["right_arrow"], (462, y - 3))
    if text_value is not None:
        _draw_text(canvas, font, text_value, (400, y))
        return

    bar = _build_stretched(
        assets["bar"], assets["bar"], assets["bar"], 122)
    canvas.alpha_composite(bar, (340, y - 3))
    slider_x = 340 + int(
        max(0.0, min(1.0, slider_value)) *
        (122 - assets["slider"].width))
    canvas.alpha_composite(assets["slider"], (slider_x, y + 5))


def _draw_option_label(canvas, font, label, y):
    _draw_text(canvas, font, label, (32, y + 2), "left")


def _build_control_tile(font, assets, value, include_text=True):
    width, height = CONTROL_TILE_SIZE
    tile = Image.new("RGBA", CONTROL_TILE_SIZE, (0, 0, 0, 0))
    tile.alpha_composite(assets["left_arrow"], (8, 0))
    tile.alpha_composite(assets["right_arrow"], (162, 0))
    if isinstance(value, str):
        if not include_text:
            return tile
        line = render_line(
            font,
            value,
            (width, height),
            (100, 3),
            "center",
            2.0 / 3.0)
        tile.alpha_composite(add_outline(line, 1))
        return tile

    bar = _build_stretched(
        assets["bar"], assets["bar"], assets["bar"], 122)
    tile.alpha_composite(bar, (40, 0))
    slider_x = 40 + int(
        max(0.0, min(1.0, value)) *
        (122 - assets["slider"].width))
    tile.alpha_composite(assets["slider"], (slider_x, 8))
    return tile


def _apply_compiled_graphic_angle(image, angle_turns):
    """Apply the serialized frontend Graphic.Angle to a bank sprite.

    The frontend records store angles as turns, not degrees.  The option
    arrow graphic records both carry Angle=-0.25, so the bank's vertical
    FE_SCROLL sprites are rendered with the exact quarter-turn used by the
    retail UI.  Keep the canvas fixed at the bank sprite dimensions: the
    retail sprite primitive rotates around its own centre.
    """
    if not angle_turns:
        return image
    return image.rotate(
        float(angle_turns) * 360.0,
        resample=Image.Resampling.BILINEAR,
        expand=False)


def _option_assets(buf, parsed):
    # LEFT_ARROW_GRAPHIC and RIGHT_ARROW_GRAPHIC in frontend.bin resolve to
    # GraphicIndex 380/379 and both serialize Angle=-0.25.  The corresponding
    # bank names are FE_SCROLL_DOWN/UP_SPRITE; resolve those names from the
    # retail bank, then apply the compiled transform before composition.
    left_arrow = _apply_compiled_graphic_angle(
        _decode_named(buf, parsed, "FE_SCROLL_DOWN_SPRITE"), -0.25)
    right_arrow = _apply_compiled_graphic_angle(
        _decode_named(buf, parsed, "FE_SCROLL_UP_SPRITE"), -0.25)
    left_arrow_hover = _apply_compiled_graphic_angle(
        _decode_named(buf, parsed, "FE_SCROLL_DOWN_HOVERED_SPRITE"), -0.25)
    right_arrow_hover = _apply_compiled_graphic_angle(
        _decode_named(buf, parsed, "FE_SCROLL_UP_HOVERED_SPRITE"), -0.25)
    return {
        "left_arrow": left_arrow,
        "right_arrow": right_arrow,
        "bar": _decode_named(
            buf, parsed, "FE_OPTIONS_HORIZONTAL_BAR_SPRITE"),
        "slider": _decode_named(
            buf, parsed, "FE_OPTIONS_SLIDER_SPRITE"),
        "back": _decode_named(buf, parsed, "FE_BUTTON_BACK_OFF"),
        "accept": _decode_named(buf, parsed, "FE_BUTTON_ACCEPT_OFF"),
        "button": _decode_named(buf, parsed, "FE_BUTTON_OFF"),
        # ON/hover art for the detail-screen footer + value arrows.  Routed
        # through the OPTIONS component atlas free region (x>=1016) so the
        # 6-frame helper sheet UV math (/6.0, height 2880) stays untouched.
        # IDs resolve by name in _decode_named; the ON footer glyphs are
        # FE_BUTTON_BACK_ON/ACCEPT_ON/ON, arrow hover is the SCROLL HOVERED
        # sprites (same names the About/Redefine/Quit hover art already uses).
        "back_on": _decode_named(buf, parsed, "FE_BUTTON_BACK_ON"),
        "accept_on": _decode_named(buf, parsed, "FE_BUTTON_ACCEPT_ON"),
        "button_on": _decode_named(buf, parsed, "FE_BUTTON_ON"),
        # These are the actual CTable pieces used by the detail-screen
        # option rows and by the Redefine list.  The compiled table Width is
        # the repeat span; it is not the final rendered width.
        "slot_left": _decode_named(buf, parsed, "FE_SLOT_TEST_L_OFF"),
        "slot_middle": _decode_named(buf, parsed, "FE_SLOT_TEST_M_OFF"),
        "slot_right": _decode_named(buf, parsed, "FE_SLOT_TEST_R_OFF"),
        "left_arrow_hover": left_arrow_hover,
        "right_arrow_hover": right_arrow_hover,
    }


def _draw_detail_helpers(canvas, font, assets, include_text=True):
    # UI_HELPERS_* contributes a parent y offset of 20.
    _draw_helper(
        canvas,
        assets["back"],
        font,
        (20, 440),
        "Cancel",
        include_text=include_text)
    # The initial detail transaction has no pending profile changes, so the
    # retail Apply child is disabled (gray) until a value/key is mutated.
    _draw_helper(
        canvas,
        assets["accept"],
        font,
        (362, 440),
        "Apply",
        (128, 128, 128, 255),
        include_text=include_text)
    _draw_helper(
        canvas,
        assets["button"],
        font,
        (192, 400),
        "Defaults",
        include_text=include_text)


def _draw_detail_row_tables(canvas, assets, row_ys):
    """Materialise the two serialized option-row CTable children.

    ``UI_BUTTON_OPTIONS_LEFT`` has Width=180 and
    ``UI_BUTTON_OPTIONS_RIGHT`` has Width=120.  CTable repeats the middle
    sprite across those spans and then places the natural-size corner pieces;
    passing either Width as the final bitmap width silently produces the
    undersized backgrounds seen in the transitional renderer.
    """
    left_table = _build_table_horizontal(
        assets["slot_left"],
        assets["slot_middle"],
        assets["slot_right"],
        180)
    right_table = _build_table_horizontal(
        assets["bar"],
        assets["bar"],
        assets["bar"],
        120)
    for row_y in row_ys:
        # UI_FRONTEND_LIST_* owns the (100, screen_y) parent origin.  The
        # slider child records are local (-100, 0), so their resolved table
        # origin is x=100, not the canvas edge.  Preserve that transform here
        # rather than silently flattening the list hierarchy.
        canvas.alpha_composite(left_table, (100, row_y))
        canvas.alpha_composite(right_table, (431, row_y - 3))


def build_settings_frame(
        frontend_bank,
        font_bank,
        title,
        rows,
        row_ys,
        include_title_rule=True,
        include_title_text=True,
        include_row_text=True,
        include_helper_text=True):
    buf, parsed = load_big(frontend_bank)
    font = load_font(font_bank, "ENG_ARIAL_24")
    assets = _option_assets(buf, parsed)
    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    _draw_title(
        canvas,
        (buf, parsed),
        font,
        title,
        include_title_rule,
        include_title_text)
    _draw_detail_row_tables(canvas, assets, row_ys)
    if include_row_text:
        for row, y in zip(rows, row_ys):
            _draw_text(canvas, font, row[0], (132, y + 2), "left")
    _draw_detail_helpers(
        canvas,
        font,
        assets,
        include_text=include_helper_text)
    return canvas


def build_redefine_frame(
        frontend_bank,
        font_bank,
        hovered_index=None,
        include_key_text=True,
        include_action_text=True,
        include_title_rule=True,
        include_title_text=True,
        materialized_row_offset=0,
        include_helper_text=True):
    visible_rows = REDEFINE_ROWS
    materialized_rows = None
    if materialized_row_offset:
        materialized_rows = redefine_scrolled_rows(materialized_row_offset)
        visible_rows = tuple(
            (
                label,
                _redefine_materialized_key_text(
                    materialized_row_offset + row_index,
                    action_id),
            )
            for row_index, (action_id, label) in enumerate(materialized_rows))
    if hovered_index is not None and not 0 <= hovered_index < len(visible_rows):
        raise ValueError("redefine hover is outside the compiled list")

    buf, parsed = load_big(frontend_bank)
    font = load_font(font_bank, "ENG_ARIAL_24")
    row_font = load_font(font_bank, "ENG_ARIAL_12")
    assets = _option_assets(buf, parsed)
    right_bar = _decode_named(
        buf, parsed, "FE_OPTIONS_HORIZONTAL_BAR_SPRITE")
    right_slot = _build_stretched(
        right_bar, right_bar, right_bar, 220)
    slots = {}
    for state in ("OFF", "ON"):
        slot_left = _decode_named(
            buf, parsed, "FE_SLOT_TEST_L_" + state)
        slot_middle = _decode_named(
            buf, parsed, "FE_SLOT_TEST_M_" + state)
        slot_right = _decode_named(
            buf, parsed, "FE_SLOT_TEST_R_" + state)
        slots[state] = (
            _build_table_horizontal(slot_left, slot_middle, slot_right, 280),
            _build_table_horizontal(right_bar, right_bar, right_bar, 220),
        )

    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    _draw_title(
        canvas,
        (buf, parsed),
        font,
        "Redefine Keys",
        include_title_rule,
        include_title_text)
    for index, (label, key) in enumerate(visible_rows):
        base_x = REDEFINE_LIST_ORIGIN[0]
        base_y = (
            REDEFINE_LIST_ORIGIN[1] +
            index * REDEFINE_ROW_STEP_Y
        )
        table_x = base_x + REDEFINE_TABLE_OFFSET[0]
        table_y = base_y + REDEFINE_TABLE_OFFSET[1]
        state = "ON" if index == hovered_index else "OFF"
        # The decoded left table has a natural width larger than its authored
        # child span.  Retail submits the right value table first and the left
        # rounded table second, so the left table owns the overlap instead of
        # exposing a dark value-bar strip through its right cap.
        canvas.alpha_composite(
            slots[state][1],
            (
                table_x + REDEFINE_RIGHT_SLOT_OFFSET[0],
                table_y + REDEFINE_RIGHT_SLOT_OFFSET[1],
            ))
        canvas.alpha_composite(
            slots[state][0],
            (table_x, table_y))
        if include_action_text:
            _draw_text(
                canvas,
                row_font,
                label,
                (
                    base_x + REDEFINE_ACTION_TEXT_OFFSET[0],
                    base_y + REDEFINE_ACTION_TEXT_OFFSET[1] +
                    REDEFINE_TEXT_RENDER_Y_BIAS,
                ),
                "left",
                REDEFINE_ACTION_TEXT_SCALE,
                (255, 255, 255, 255))
        if include_key_text:
            _draw_text(
                canvas,
                row_font,
                key,
                (
                    base_x + REDEFINE_KEY_TEXT_OFFSET[0],
                    base_y + REDEFINE_KEY_TEXT_OFFSET[1] +
                    REDEFINE_TEXT_RENDER_Y_BIAS,
                ),
                "left")

    # The initial viewport is at the top of the non-wrapping list, so retail
    # suppresses the UpArrow until the list has scrolled.  The live checkpoint
    # owns that state transition; this authored frame is the initial surface.
    if materialized_row_offset:
        canvas.alpha_composite(
            _decode_named(buf, parsed, "FE_SCROLL_UP_SPRITE"),
            (304, 80))
    if materialized_row_offset + len(visible_rows) < len(REDEFINE_MATERIALIZED_ROWS):
        canvas.alpha_composite(
            _decode_named(buf, parsed, "FE_SCROLL_DOWN_SPRITE"),
            (304, 350))
    # All nine initial bindings are defined by the WASD profile, so the
    # undefined-control warning is state-suppressed in the retail capture.

    # UI_HELPERS_REDEFINE contains Apply/Cancel and two 320-wide reset rows.
    _draw_helper(
        canvas,
        assets["back"],
        font,
        (20, 440),
        "Cancel",
        include_text=include_helper_text)
    _draw_helper(
        canvas,
        assets["accept"],
        font,
        (362, 440),
        "Apply",
        (128, 128, 128, 255),
        include_text=include_helper_text)
    reset = _build_stretched(
        _decode_named(buf, parsed, "FE_BUTTON_OFF_L"),
        _decode_named(buf, parsed, "FE_BUTTON_OFF_M"),
        _decode_named(buf, parsed, "FE_BUTTON_OFF_R"),
        320)
    canvas.alpha_composite(reset, (0, 389))
    canvas.alpha_composite(reset, (320, 389))
    _draw_text(canvas, font, "Reset to WASD", (160, 410))
    _draw_text(canvas, font, "Reset to Arrows", (480, 410))
    return canvas


def build_save_preview_viewport(frontend_bank):
    """Build the 256x256 save-preview viewport panel (region minimap under the
    UI_VIEW_RING_SMALL ornament).  It is composited as the LAST layer of the
    saved-games screen, matching retail, which draws the viewport panel on top
    of the whole screen.  The live D3D9 path emits it as its own Render2D quad
    after the title rule/list/helpers, so keeping it a discrete layer here lets
    the recomposition proof reproduce the baked oracle pixel-for-pixel."""
    buf, parsed = load_big(frontend_bank)
    ring = _decode_named(buf, parsed, SAVE_VIEW_RING_SPRITE)
    panel = Image.new("RGBA", ring.size, (0, 0, 0, 0))
    try:
        minimap = _decode_named(buf, parsed, SAVE_PREVIEW_MINIMAP)
        if minimap.size == ring.size:
            # MINIMAP_STARTOAKVALE_FRONT_END is a square map texture, while
            # the retail viewport clips it to the opaque footprint of
            # UI_VIEW_RING_SMALL before drawing the ring on top.  Compositing
            # the raw map directly leaves a visible rectangular halo outside
            # the circular frame.
            minimap = minimap.copy()
            minimap.putalpha(ImageChops.multiply(
                minimap.getchannel("A"),
                ring.getchannel("A")))
            panel.alpha_composite(minimap)
    except ValueError:
        pass
    panel.alpha_composite(ring)
    return panel


def build_save_browser_frame(
        frontend_bank,
        font_bank,
        selected_index=0,
        include_title_rule=True,
        include_viewport=True,
        include_highlight=True,
        include_save_text=True,
        include_save_info_text=True,
        include_bottom_backdrop=True,
        include_text_bottom=True,
        include_text_area=True,
        include_save_title_text=True,
        include_title_area=None):
    """Compose the first live save-list page from its recovered definitions."""
    if not 0 <= selected_index < len(SAVE_BROWSER_ROWS):
        raise ValueError("save selection is outside the recovered list")

    buf, parsed = load_big(frontend_bank)
    font = load_font(font_bank, "ENG_ARIAL_24")
    row_font = load_font(font_bank, "ENG_ARIAL_16")
    assets = _option_assets(buf, parsed)
    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    if include_title_area is None:
        include_title_area = include_title_rule
    _draw_save_title(
        canvas,
        (buf, parsed),
        font,
        "Saved Games",
        include_title_area,
        include_save_title_text)

    # The selection highlight is the same TS_BUTTON L/M/R ornament as the
    # Options selected row (total width 64+120+64 = 248).  The live D3D9 path
    # emits it as its own generated-component quad positioned by g_SaveSelection
    # (InitialiseButtonComponents(248)), drawn BEFORE the name-bearing save cell
    # so it sits behind the row text.  The component/atlas sheet omits it
    # (include_highlight=False) and the recomposition proof re-adds it as its own
    # layer under the names -> zero-mask PIXEL_IDENTICAL.
    selected = _build_table_horizontal(
        _decode_named(buf, parsed, "TS_BUTTON_L"),
        _decode_named(buf, parsed, "TS_BUTTON_M"),
        _decode_named(buf, parsed, "TS_BUTTON_R"),
        120)
    if include_highlight:
        canvas.alpha_composite(
            selected,
            (
                SAVE_LIST_ORIGIN[0],
                SAVE_LIST_ORIGIN[1] +
                selected_index * SAVE_ROW_STEP_Y - 7,
            ))
    # Centre each save name on the highlight button, exactly as the main-menu
    # and Options rows centre their labels on their selected ornament. The
    # ornament is composited at SAVE_LIST_ORIGIN[0] and spans ``selected.width``
    # (L + inner-span + R), so its centre is origin + width/2. Previously the
    # text was centred on origin+60 (the left-sprite end), leaving it 64px left
    # of the highlight centre and reading as left-aligned.
    save_name_center_x = SAVE_LIST_ORIGIN[0] + selected.width / 2.0
    if include_save_text:
        for row, filename in enumerate(SAVE_BROWSER_ROWS):
            _draw_text(
                canvas,
                row_font,
                filename,
                (
                    save_name_center_x,
                    SAVE_LIST_ORIGIN[1] + row * SAVE_ROW_STEP_Y,
                ),
                "center")

    # File Information text area (UI_TEXT_AREA @ 0,254): use the exact
    # asymmetric UI_TABLE_TEXT_LEFT/RIGHT sprite maps. CTable's horizontal
    # edge is key 4; the decoded key-1 corners are BL and BR respectively.
    if include_text_area:
        text_area_segment = _decode_named(
            buf, parsed, "UI_TEXTBOX_MIDDLE_FE_SPRITE")
        canvas.alpha_composite(
            _build_table_horizontal(
                text_area_segment,
                text_area_segment,
                _decode_named(buf, parsed, "UI_TEXTBOX_BL_SPRITE_FE"),
                SAVE_TEXT_AREA_LEFT_WIDTH),
            SAVE_TEXT_AREA_ORIGIN)
        canvas.alpha_composite(
            _build_table_horizontal(
                _decode_named(buf, parsed, "UI_TEXTBOX_BR_SPRITE_FE"),
                text_area_segment,
                text_area_segment,
                SAVE_TEXT_AREA_RIGHT[1]),
            (SAVE_TEXT_AREA_RIGHT[0], SAVE_TEXT_AREA_ORIGIN[1]))

    # UI_TEXT_BOTTOM is state-selected by ConstructFileDescription when the
    # selected save has a description. Its retail graphic is the 8x8 table
    # test horizontal rule (#89), stretched 160x1 at resolved (0,404).
    if include_text_bottom:
        text_bottom = _decode_named(buf, parsed, "UI_TABLE_TEST_H_T_FE")
        text_bottom = text_bottom.resize((text_bottom.width * 160, text_bottom.height))
        canvas.alpha_composite(text_bottom, SAVE_TEXT_BOTTOM_ORIGIN)

    # ConstructFileDescription installs the header + metadata children only after
    # selection metadata has been decoded (a runtime boundary not recovered yet).
    # Keep this visual checkpoint honest: show the recovered File Information
    # header on its backdrop plus the current local profile label.  The
    # component/atlas route leaves these runtime strings transparent so the
    # executable can submit the active profile value live.
    if include_save_info_text:
        _draw_text(canvas, font, "File Information", (65, 261), "left")
        _draw_text(
            canvas,
            row_font,
            "Cornelio",
            (95, 293),
            "left")
    _draw_helper(canvas, assets["back"], font, (20, 420), "Back")

    # UI_BOTTOM_BACKDROP is a 4x4 half-alpha black frontend texture (graphic
    # 98), stretched 160x62 at (0,292). It is the final static child in the
    # saved-games root at layer 5; the live preview quad still follows it.
    if include_bottom_backdrop:
        bottom_backdrop = _decode_named(buf, parsed, "HUD_TEXTBOX_BACK_FE")
        bottom_backdrop = bottom_backdrop.resize((640, 248))
        canvas.alpha_composite(bottom_backdrop, SAVE_BOTTOM_BACKDROP_ORIGIN)

    # Save-preview viewport (UI_VIEW_RING_SMALL @ 314,37): a region minimap
    # framed by the ring ornament, on the right of the screen.  It is the LAST
    # thing composited (retail draws the viewport panel on top of the whole
    # saved-games screen).  The live D3D9 path emits it as a standalone Render2D
    # quad drawn last while the underlying cell-background quads SKIP the 256x256
    # ring rect; to reproduce the live path pixel-for-pixel the oracle punches
    # the same hole (clearing the rule/list/File-Info under the panel) before
    # compositing the viewport on top.  The component/atlas variant omits the
    # panel entirely (include_viewport=False) and the recomposition proof
    # re-adds it as its own final layer -> zero-mask PIXEL_IDENTICAL.
    if include_viewport:
        ox, oy = SAVE_VIEW_RING_ORIGIN
        canvas.paste((0, 0, 0, 0), (ox, oy, ox + 256, oy + 256))
        canvas.alpha_composite(
            build_save_preview_viewport(frontend_bank),
            SAVE_VIEW_RING_ORIGIN)
    return canvas


def _wrap_text_lines(font, text, wrap_width, scale):
    """Greedy word-wrap ``text`` so each line advance stays <= wrap_width."""
    words = text.split()
    lines = []
    current = ""
    for word in words:
        candidate = word if not current else current + " " + word
        if current and text_advance(font, candidate, scale) > wrap_width:
            lines.append(current)
            current = word
        else:
            current = candidate
    if current:
        lines.append(current)
    return lines


def build_about_frame(
        frontend_bank,
        font_bank,
        include_title_rule=True,
        include_message=True):
    """Compose the UI_FRONTEND_ABOUT_MENU overlay panel (transparent).

    Mirrors the Options/Settings composition: shared UI_TEXTBOX_MIDDLE title
    rule + serialized left-aligned title, then the static legal-notice message, then the single
    UI_HELPERS Back button.  The SPOOKY background is a live D3D9 layer (drawn
    behind this panel), not part of the baked frame -- matching how the retail
    forest/coastal backgrounds are composited at runtime rather than baked.
    """
    buf, parsed = load_big(frontend_bank)
    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    title_font = load_font(font_bank, "ENG_ARIAL_24")
    if include_title_rule:
        title_segment = _decode_named(
            buf, parsed, "UI_TEXTBOX_MIDDLE_FE_SPRITE")
        title_rule = _build_table_horizontal(
            title_segment, title_segment, title_segment, 640)
        canvas.alpha_composite(title_rule, ABOUT_RULE_POSITION)
    _draw_text(canvas, title_font, ABOUT_TITLE, ABOUT_TITLE_TEXT_POSITION)

    if include_message:
        message_font = load_font(font_bank, "ENG_ARIAL_12")
        scale = 2.0 / 3.0
        x, y = ABOUT_MESSAGE_ORIGIN
        for paragraph in ABOUT_MESSAGE_PARAGRAPHS:
            for line in _wrap_text_lines(
                    message_font,
                    paragraph,
                    ABOUT_MESSAGE_WRAP_WIDTH,
                    scale):
                _draw_text(canvas, message_font, line, (x, y))
                y += ABOUT_MESSAGE_LINE_STEP
            y += ABOUT_MESSAGE_PARAGRAPH_GAP

    # The Back button is NOT baked here.  Like Options/Save, About draws its
    # UI_HELPERS Back solely from the live D3D9 helper quad (design (20,420),
    # OFF/ON hover swap), so baking a static glyph here would double-draw a
    # 20px-misaligned button and defeat the hover highlight.
    return canvas


def build_credits_frame(frontend_bank):
    """Compose the authored initial Credits viewport.

    ``UI_FRONTEND_CREDITS_MENU`` starts its scrolling container at (0, 480),
    so the first presented frame contains the title and the two widescreen
    bars while the first credit line is still below the viewport.  Keep this
    frame sourced entirely from frontend.big: graphic 334 is the compiled
    ``UI_WIDESCREEN_BAR_*`` asset and the title children are graphics 3/4.
    The scrolling text is intentionally a later live state; baking it into
    this frame would change the retail activation boundary.
    """
    buf, parsed = load_big(frontend_bank)
    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    title_left = _decode_named(buf, parsed, "FRONTEND_TITLE_01_SPRITE")
    title_right = _decode_named(buf, parsed, "FRONTEND_TITLE_02_SPRITE")
    canvas.alpha_composite(title_left, (70, 30))
    canvas.alpha_composite(title_right, (70 + title_left.width, 30))

    # UI_WIDESCREEN_BAR_TOP/BOTTOM use graphic 334 with Zoom=(180,18).
    # The source is a 4x4 solid bar; its authored width is 720 and the
    # 640x480 view clips the rightmost 80 pixels exactly as the retail view.
    bar = _decode_named(
        buf, parsed, "UI_TABLE_BACK_SPRITE_FOR_DIALOG_FE")
    bar = bar.resize((720, 18), Image.Resampling.NEAREST)
    canvas.alpha_composite(bar.crop((0, 0, CANVAS_SIZE[0], 18)), (0, 0))
    canvas.alpha_composite(
        bar.crop((0, 0, CANVAS_SIZE[0], 18)),
        (0, 408))
    return canvas


def build_profiles_frame(frontend_bank, font_bank):
    """Compose the static Select Profile surface.

    ``UI_FRONTEND_LIST_FOR_PROFILES`` is a dynamic Type-43 list, so profile
    names and selection ornaments are intentionally left to the runtime.  The
    title rule and title are the authored static portion of the normal profile
    screen; the coastal background and Back helper remain live layers.
    """
    buf, parsed = load_big(frontend_bank)
    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    title_segment = _decode_named(
        buf, parsed, "UI_TEXTBOX_MIDDLE_FE_SPRITE")
    canvas.alpha_composite(
        _build_table_horizontal(
            title_segment, title_segment, title_segment, CANVAS_SIZE[0]),
        HEADER_RULE_POSITION)
    title_font = load_font(font_bank, "ENG_ARIAL_24")
    _draw_text(canvas, title_font, "Select Profile", HEADER_TEXT_POSITION)
    return canvas


def build_delete_profile_confirmation_frame(
        frontend_bank,
        font_bank):
    """Compose the authored delete-profile confirmation surface."""
    buf, parsed = load_big(frontend_bank)
    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    title_segment = _decode_named(
        buf, parsed, "UI_TEXTBOX_MIDDLE_FE_SPRITE")
    canvas.alpha_composite(
        _build_table_horizontal(
            title_segment, title_segment, title_segment, CANVAS_SIZE[0]),
        HEADER_RULE_POSITION)
    title_font = load_font(font_bank, "ENG_ARIAL_24")
    _draw_text(canvas, title_font, "Delete Profile", (65, 44), "left")
    _draw_text(
        canvas,
        title_font,
        "Select the profile you wish to delete:",
        (320, 100))
    _draw_text(
        canvas,
        title_font,
        "By deleting a profile you will lose any saved games associated with it.",
        (320, 240))
    assets = _option_assets(buf, parsed)
    _draw_helper(canvas, assets["back"], title_font, (20, 405), "NO")
    _draw_helper(canvas, assets["accept"], title_font, (362, 405), "YES")
    return canvas


def build_profiles_screen_sheet(frontend_bank, font_bank):
    """Return normal profiles plus the recovered delete-confirmation frame."""
    normal = build_profiles_frame(frontend_bank, font_bank)
    confirmation = build_delete_profile_confirmation_frame(
        frontend_bank, font_bank)
    sheet = Image.new(
        "RGBA",
        (CANVAS_SIZE[0], CANVAS_SIZE[1] * 2),
        (0, 0, 0, 0))
    sheet.alpha_composite(normal, (0, 0))
    sheet.alpha_composite(confirmation, (0, CANVAS_SIZE[1]))
    return sheet


def build_options_frame(
        frontend_bank,
        font_bank,
        selected_index=0,
        include_title_rule=True,
        include_selected_button=True,
        include_text=True):
    if not 0 <= selected_index < len(OPTIONS_ROWS):
        raise ValueError("options selection is outside the compiled list")

    buf, parsed = load_big(frontend_bank)
    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    font = load_font(font_bank, "ENG_ARIAL_24")
    _draw_title(
        canvas,
        (buf, parsed),
        font,
        "Options",
        include_title_rule)
    if include_selected_button:
        selected = build_selected_button(
            _decode_named(buf, parsed, "TS_BUTTON_L"),
            _decode_named(buf, parsed, "TS_BUTTON_M"),
            _decode_named(buf, parsed, "TS_BUTTON_R"),
            280)
        selected_y = OPTIONS_ROWS[selected_index][1]
        canvas.alpha_composite(
            selected,
            (
                selected_button_left(selected.width),
                OPTIONS_LIST_ORIGIN[1] + selected_y - 7,
            ))

    if include_text:
        for row_layer in build_options_row_layers(font_bank):
            canvas.alpha_composite(row_layer)
    return canvas


def build_options_row_layers(font_bank):
    """Render each Options text child into an independent design canvas."""
    font = load_font(font_bank, "ENG_ARIAL_24")
    layers = []
    for text, y_offset in OPTIONS_ROWS:
        canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
        _draw_text(
            canvas,
            font,
            text,
            (
                MENU_CONTENT_CENTER_X,
                OPTIONS_LIST_ORIGIN[1] + y_offset,
            ))
        layers.append(canvas)
    return layers


def build_options_sheet(
        frontend_bank,
        font_bank,
        include_title_rule=True,
        include_selected_button=True,
        include_options_text=True,
        include_options_row_atlas=False,
        include_save_title_area=None,
        include_detail_title_text=True):
    if include_save_title_area is None:
        include_save_title_area = include_title_rule
    frames = [
        build_options_frame(
            frontend_bank,
            font_bank,
            selected,
            include_title_rule,
            include_selected_button,
            include_options_text)
        for selected in range(len(OPTIONS_ROWS))
    ]
    frames.extend((
        build_settings_frame(
            frontend_bank,
            font_bank,
            "Gameplay Options",
            GAMEPLAY_ROWS,
            tuple(90 + row * 30 for row in range(len(GAMEPLAY_ROWS))),
            include_title_rule,
            include_detail_title_text,
            not include_options_row_atlas,
            not include_options_row_atlas),
        build_settings_frame(
            frontend_bank,
            font_bank,
            "Audio Options",
            AUDIO_ROWS,
            (180, 210, 240),
            include_title_rule,
            include_detail_title_text,
            not include_options_row_atlas,
            not include_options_row_atlas),
        build_settings_frame(
            frontend_bank,
            font_bank,
            "Video Options",
            VIDEO_ROWS,
            tuple(90 + row * 30 for row in range(len(VIDEO_ROWS))),
            include_title_rule,
            include_detail_title_text,
            not include_options_row_atlas,
            not include_options_row_atlas),
        build_redefine_frame(
            frontend_bank,
            font_bank,
            include_key_text=False,
            include_action_text=not include_options_row_atlas,
            include_title_rule=include_title_rule,
            include_title_text=include_detail_title_text,
            include_helper_text=not include_options_row_atlas),
    ))
    # The oracle sheet (include_options_row_atlas=False) bakes the preview
    # viewport into each save frame as ground truth.  The component/atlas sheet
    # (include_options_row_atlas=True) is built ring-free so the live
    # recomposition can re-add the viewport as its own final layer, matching the
    # live D3D9 quad and proving a zero-mask PIXEL_IDENTICAL recomposition.
    frames.extend(
        build_save_browser_frame(
            frontend_bank,
            font_bank,
            selected,
            include_title_rule,
            include_viewport=not include_options_row_atlas,
            include_highlight=not include_options_row_atlas,
            include_save_text=not include_options_row_atlas,
            include_save_info_text=not include_options_row_atlas,
            include_bottom_backdrop=not include_options_row_atlas,
            include_text_bottom=not include_options_row_atlas,
            include_text_area=not include_options_row_atlas,
            include_save_title_text=not include_options_row_atlas,
            include_title_area=(
                include_save_title_area and not include_options_row_atlas))
        for selected in range(SAVE_BROWSER_FRAME_COUNT))
    sheet = Image.new(
        "RGBA",
        (
            OPTIONS_COMPONENT_SHEET_WIDTH
            if include_options_row_atlas
            else OPTIONS_SHEET_WIDTH,
            CANVAS_SIZE[1] * (
                SAVE_SCREEN_FRAME_BASE
                if include_options_row_atlas
                else len(frames)
            ),
        ),
        (0, 0, 0, 0))
    for index, frame in enumerate(frames):
        if include_options_row_atlas and index >= SAVE_SCREEN_FRAME_BASE:
            sheet.alpha_composite(
                frame,
                (
                    SAVE_COMPONENT_ATLAS_ORIGIN[0],
                    SAVE_COMPONENT_ATLAS_ORIGIN[1] +
                    (index - SAVE_SCREEN_FRAME_BASE) *
                    CANVAS_SIZE[1],
                ))
        else:
            sheet.alpha_composite(frame, (0, index * CANVAS_SIZE[1]))

    if include_options_row_atlas:
        # The save frames were built ring-free above.  The live D3D9 ring quad
        # samples the preview viewport from each save cell at cell-local
        # (314,37); bake ONLY that 256x256 rect back so the live quad has its
        # source pixels while the rest of every cell stays ring-free (so the
        # four cell-background quads never double-draw the ring).  Clear the rect
        # first so the source is the panel over transparency, matching the pixels
        # the live ring quad composites over the screen -- not the panel blended
        # with whatever ring-free cell content sat under it.
        viewport = build_save_preview_viewport(frontend_bank)
        ox, oy = SAVE_VIEW_RING_ORIGIN
        for save_row in range(SAVE_BROWSER_FRAME_COUNT):
            source_left = SAVE_COMPONENT_ATLAS_ORIGIN[0] + ox
            source_top = (
                SAVE_COMPONENT_ATLAS_ORIGIN[1] +
                save_row * CANVAS_SIZE[1] +
                oy
            )
            sheet.paste(
                (0, 0, 0, 0),
                (
                    source_left,
                    source_top,
                    source_left + 256,
                    source_top + 256,
                ))
            sheet.alpha_composite(viewport, (source_left, source_top))

    buf, parsed = load_big(frontend_bank)
    font = load_font(font_bank, "ENG_ARIAL_24")
    assets = _option_assets(buf, parsed)
    if include_options_row_atlas:
        bottom_backdrop_source = _decode_named(
            buf, parsed, "HUD_TEXTBOX_BACK_FE")
        source_x, source_y = SAVE_BOTTOM_BACKDROP_ATLAS_ORIGIN
        if (
                source_x + bottom_backdrop_source.width > sheet.width or
                source_y + bottom_backdrop_source.height > sheet.height):
            raise ValueError("save bottom-backdrop source does not fit component sheet")
        sheet.alpha_composite(
            bottom_backdrop_source,
            SAVE_BOTTOM_BACKDROP_ATLAS_ORIGIN)
        text_bottom_source = _decode_named(
            buf, parsed, "UI_TABLE_TEST_H_T_FE")
        source_x, source_y = SAVE_TEXT_BOTTOM_ATLAS_ORIGIN
        if (
                source_x + text_bottom_source.width > sheet.width or
                source_y + text_bottom_source.height > sheet.height):
            raise ValueError("save text-bottom source does not fit component sheet")
        sheet.alpha_composite(
            text_bottom_source,
            SAVE_TEXT_BOTTOM_ATLAS_ORIGIN)
        text_area_middle = _decode_named(
            buf, parsed, "UI_TEXTBOX_MIDDLE_FE_SPRITE")
        text_area_bl = _decode_named(
            buf, parsed, "UI_TEXTBOX_BL_SPRITE_FE")
        text_area_br = _decode_named(
            buf, parsed, "UI_TEXTBOX_BR_SPRITE_FE")
        for source, origin in (
                (text_area_middle, SAVE_TEXT_AREA_MIDDLE_ATLAS_ORIGIN),
                (text_area_bl, SAVE_TEXT_AREA_BL_ATLAS_ORIGIN),
                (text_area_br, SAVE_TEXT_AREA_BR_ATLAS_ORIGIN)):
            source_x, source_y = origin
            if (
                    source_x + source.width > sheet.width or
                    source_y + source.height > sheet.height):
                raise ValueError("save text-area source does not fit component sheet")
            sheet.alpha_composite(source, origin)
        title_area_tl = _decode_named(
            buf, parsed, "UI_TEXTBOX_TL_SPRITE_FE")
        title_area_tr = _decode_named(
            buf, parsed, "UI_TEXTBOX_TR_SPRITE_FE")
        for source, origin in (
                (title_area_tl, SAVE_TITLE_AREA_TL_ATLAS_ORIGIN),
                (title_area_tr, SAVE_TITLE_AREA_TR_ATLAS_ORIGIN)):
            source_x, source_y = origin
            if (
                    source_x + source.width > sheet.width or
                    source_y + source.height > sheet.height):
                raise ValueError("save title-area source does not fit component sheet")
            sheet.alpha_composite(source, origin)
    tile_index = 0
    for screen_values in CONTROL_VALUE_GROUPS:
        for row_values in screen_values:
            for value in row_values:
                column = tile_index % CONTROL_ATLAS_COLUMNS
                tile_row = tile_index // CONTROL_ATLAS_COLUMNS
                sheet.alpha_composite(
                    _build_control_tile(
                        font,
                        assets,
                        value,
                        include_text=not include_options_row_atlas),
                    (
                        CONTROL_ATLAS_ORIGIN_X +
                        column * CONTROL_TILE_SIZE[0],
                        tile_row * CONTROL_TILE_SIZE[1],
                    ))
                tile_index += 1

    row_font = load_font(font_bank, "ENG_ARIAL_12")
    for key_index, value in enumerate(REDEFINE_KEY_VALUES):
        key_tile = Image.new(
            "RGBA", REDEFINE_KEY_TILE_SIZE, (0, 0, 0, 0))
        line = render_line(
            row_font,
            value,
            REDEFINE_KEY_TILE_SIZE,
            (
                0,
                REDEFINE_KEY_TEXT_OFFSET[1] +
                REDEFINE_TEXT_RENDER_Y_BIAS,
            ),
            "left",
            1.0)
        alpha = line.getchannel("A")
        line = Image.new(
            "RGBA",
            REDEFINE_KEY_TILE_SIZE,
            (255, 255, 0, 0) if key_index == 7 else (255, 255, 255, 0),
        )
        line.putalpha(alpha)
        key_tile.alpha_composite(add_outline(line, 1))
        sheet.alpha_composite(
            key_tile,
            (
                REDEFINE_KEY_ATLAS_ORIGIN_X,
                key_index * REDEFINE_KEY_TILE_SIZE[1],
            ))

    # Detail-screen hover overlays live in the OPTIONS COMPONENT atlas free
    # region (x>=1016, only exists in the 1664-wide component sheet).  The
    # 1024-wide oracle sheet has no room there, and the live loader only ever
    # binds the component sheet (g_OptionsWidth==1664), so gate the bake to it.
    if include_options_row_atlas:
        _bake_detail_hover_overlays(sheet, font, assets)

    if include_options_row_atlas:
        for row, row_layer in enumerate(
                build_options_row_layers(font_bank)):
            sheet.alpha_composite(
                row_layer,
                (
                    OPTIONS_ROW_ATLAS_ORIGIN_X,
                    row * CANVAS_SIZE[1],
                ))
        profile_font = load_font(font_bank, "ENG_ARIAL_16")
        profile_atlas = profile_font["atlas"]
        expected_origin = PROFILE_GLYPH_ATLAS_ORIGIN
        if (
                expected_origin[0] + profile_atlas.width > sheet.width or
                expected_origin[1] + profile_atlas.height > sheet.height):
            raise ValueError("profile glyph atlas does not fit component sheet")
        sheet.alpha_composite(profile_atlas, expected_origin)
        title_font = load_font(font_bank, "ENG_ARIAL_24")
        title_atlas = title_font["atlas"]
        title_origin = DETAIL_TITLE_GLYPH_ATLAS_ORIGIN
        if title_atlas.size != DETAIL_TITLE_GLYPH_ATLAS_SIZE:
            raise ValueError(
                "detail title glyph atlas has unexpected dimensions: %r" %
                (title_atlas.size,))
        if (
                title_origin[0] + title_atlas.width > sheet.width or
                title_origin[1] + title_atlas.height > sheet.height):
            raise ValueError("detail title glyph atlas does not fit component sheet")
        sheet.alpha_composite(title_atlas, title_origin)
        redefine_font = load_font(font_bank, "ENG_ARIAL_12")
        redefine_atlas = redefine_font["atlas"]
        redefine_origin = REDEFINE_GLYPH_ATLAS_ORIGIN
        if redefine_atlas.size != REDEFINE_GLYPH_ATLAS_SIZE:
            raise ValueError(
                "Redefine glyph atlas has unexpected dimensions: %r" %
                (redefine_atlas.size,))
        if (
                redefine_origin[0] + redefine_atlas.width > sheet.width or
                redefine_origin[1] + redefine_atlas.height > sheet.height):
            raise ValueError("Redefine glyph atlas does not fit component sheet")
        sheet.alpha_composite(redefine_atlas, redefine_origin)

    return sheet


def _bake_detail_hover_overlays(sheet, font, assets):
    # Arrows: full 200x30 control tiles carrying only the HOVERED arrow at the
    # SAME local offset as the baked control-tile arrow (left 8, right 162), so
    # the overlay pixel-aligns with the OFF arrow when drawn over design x=400.
    left_hover_tile = Image.new(
        "RGBA", DETAIL_ARROW_HOVER_TILE_SIZE, (0, 0, 0, 0))
    left_hover_tile.alpha_composite(assets["left_arrow_hover"], (8, 0))
    sheet.alpha_composite(
        left_hover_tile,
        (DETAIL_HOVER_ATLAS_ORIGIN_X, DETAIL_ARROW_HOVER_LEFT_ATLAS_Y))
    right_hover_tile = Image.new(
        "RGBA", DETAIL_ARROW_HOVER_TILE_SIZE, (0, 0, 0, 0))
    right_hover_tile.alpha_composite(assets["right_arrow_hover"], (162, 0))
    sheet.alpha_composite(
        right_hover_tile,
        (DETAIL_HOVER_ATLAS_ORIGIN_X, DETAIL_ARROW_HOVER_RIGHT_ATLAS_Y))

    # Footer ON tiles: bake the ON glyph+label with the EXACT _draw_helper the
    # OFF footer uses (same glyph position, same render_line/align for the
    # label), onto a full design canvas at the OFF parent, then crop the tile
    # rect (top-left == (parent_x, parent_y-16)).  This makes the ON overlay a
    # pixel-for-pixel superset of the baked OFF glyph AND its separate text
    # label, so no OFF label shows through beside the ON glyph.
    footer_specs = (
        (assets["back_on"], "Cancel",
            DETAIL_FOOTER_HOVER_CANCEL_DESIGN,
            DETAIL_FOOTER_HOVER_CANCEL_ATLAS_Y),
        (assets["button_on"], "Defaults",
            DETAIL_FOOTER_HOVER_DEFAULTS_DESIGN,
            DETAIL_FOOTER_HOVER_DEFAULTS_ATLAS_Y),
        (assets["accept_on"], "Apply",
            DETAIL_FOOTER_HOVER_APPLY_DESIGN,
            DETAIL_FOOTER_HOVER_APPLY_ATLAS_Y),
    )
    fw, fh = DETAIL_FOOTER_HOVER_TILE_SIZE
    for glyph, label, design_origin, atlas_y in footer_specs:
        # design_origin == glyph top-left == (parent_x, parent_y-16).
        footer_canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
        # The label is emitted by the live component text path. Keep this
        # hover overlay to the ON-state glyph so the label cannot be doubled
        # when the pointer enters the helper.
        footer_canvas.alpha_composite(glyph, design_origin)
        footer_tile = footer_canvas.crop((
            design_origin[0],
            design_origin[1],
            design_origin[0] + fw,
            design_origin[1] + fh,
        ))
        sheet.alpha_composite(
            footer_tile,
            (DETAIL_HOVER_ATLAS_ORIGIN_X, atlas_y))


def _draw_helper(
        canvas,
        image,
        font,
        parent,
        text,
        text_colour=None,
        include_text=True):
    canvas.alpha_composite(image, (parent[0], parent[1] - 16))
    if include_text:
        _draw_text(
            canvas,
            font,
            text,
            (parent[0] + 128, parent[1] + 5),
            colour=text_colour)


def build_helper_sheet(frontend_bank, font_bank):
    buf, parsed = load_big(frontend_bank)
    font = load_font(font_bank, "ENG_ARIAL_24")
    back_off = _decode_named(buf, parsed, "FE_BUTTON_BACK_OFF")
    back_on = _decode_named(buf, parsed, "FE_BUTTON_BACK_ON")
    accept_off = _decode_named(buf, parsed, "FE_BUTTON_ACCEPT_OFF")
    accept_on = _decode_named(buf, parsed, "FE_BUTTON_ACCEPT_ON")
    reset_on = _build_stretched(
        _decode_named(buf, parsed, "FE_BUTTON_ON_L"),
        _decode_named(buf, parsed, "FE_BUTTON_ON_M"),
        _decode_named(buf, parsed, "FE_BUTTON_ON_R"),
        REDEFINE_RESET_HOVER_SIZE[0])

    frames = []
    for hovered in (False, True):
        canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
        _draw_helper(
            canvas,
            back_on if hovered else back_off,
            font,
            (20, 420),
            "Back")
        frames.append(canvas)

    # UI_FRONTEND_QUIT_PROMPT is deliberately background-free.  The retail
    # GotoNextScreen special case for used-key 0x1a applies the screen filter.
    for hover in ("none", "no", "yes"):
        canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
        _draw_text(
            canvas,
            font,
            "Are you sure you want to quit?",
            (320, 150))
        _draw_helper(
            canvas,
            back_on if hover == "no" else back_off,
            font,
            (20, 405),
            "NO")
        _draw_helper(
            canvas,
            accept_on if hover == "yes" else accept_off,
            font,
            (362, 405),
            "YES")
        frames.append(canvas)

    # CKeyRedefiner::OnHovered selects visual state 3 on entry and state 4 on
    # exit.  Keep each complete 588x35 ON row intact in a sixth helper frame;
    # one runtime quad can then replace the corresponding OFF row without a
    # filtering seam or any change to the Options component texture.
    hover_atlas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    for row in range(len(REDEFINE_ROWS)):
        hovered = build_redefine_frame(
            frontend_bank,
            font_bank,
            hovered_index=row,
            include_key_text=False,
            include_action_text=False)
        base_y = REDEFINE_LIST_ORIGIN[1] + row * REDEFINE_ROW_STEP_Y
        strip_top = (
            base_y +
            REDEFINE_TABLE_OFFSET[1] +
            REDEFINE_RIGHT_SLOT_OFFSET[1]
        )
        hover_atlas.alpha_composite(
            hovered.crop((
                8,
                strip_top,
                596,
                strip_top + REDEFINE_HOVER_STRIP_SIZE[1],
            )),
            (0, row * REDEFINE_HOVER_STRIP_SIZE[1]))

    # The two reset clickables use the same ON table and different labels.
    # Both complete 320x64 overlays fit in the unused tail of this atlas.
    for column, label in enumerate(("Reset to WASD", "Reset to Arrows")):
        tile = Image.new(
            "RGBA", REDEFINE_RESET_HOVER_SIZE, (0, 0, 0, 0))
        tile.alpha_composite(reset_on, (0, 0))
        line = render_line(
            font,
            label,
            REDEFINE_RESET_HOVER_SIZE,
            (REDEFINE_RESET_HOVER_SIZE[0] // 2, 21),
            "center",
            2.0 / 3.0)
        tile.alpha_composite(add_outline(line, 1))
        hover_atlas.alpha_composite(
            tile,
            (
                column * REDEFINE_RESET_HOVER_SIZE[0],
                REDEFINE_RESET_HOVER_ATLAS_Y,
            ))
    frames.append(hover_atlas)
    assert len(frames) == HELPER_SHEET_FRAME_COUNT

    sheet = Image.new(
        "RGBA",
        (CANVAS_SIZE[0], CANVAS_SIZE[1] * len(frames)),
        (0, 0, 0, 0))
    for index, frame in enumerate(frames):
        sheet.alpha_composite(frame, (0, index * CANVAS_SIZE[1]))
    return sheet


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("frontend_bank")
    parser.add_argument("font_bank")
    parser.add_argument("options_output")
    parser.add_argument("helpers_output")
    parser.add_argument("--about-output")
    parser.add_argument("--credits-output")
    parser.add_argument("--profiles-output")
    parser.add_argument("--components-output")
    parser.add_argument("--redefine-scroll-pages-output")
    parser.add_argument("--title-segment-output")
    parser.add_argument("--button-left-output")
    parser.add_argument("--button-middle-output")
    parser.add_argument("--button-right-output")
    parser.add_argument("--game-root")
    parser.add_argument("--schema")
    args = parser.parse_args()

    if bool(args.game_root) != bool(args.schema):
        parser.error("--game-root and --schema must be supplied together")
    component_outputs = (
        args.components_output,
        args.title_segment_output,
        args.button_left_output,
        args.button_middle_output,
        args.button_right_output,
    )
    if any(component_outputs) and not all(component_outputs):
        parser.error(
            "all component output arguments must be supplied together")
    if args.game_root:
        validate_compiled_subscreen_layout(args.game_root, args.schema)
        validate_compiled_credits_layout(args.game_root, args.schema)
        validate_compiled_profiles_layout(args.game_root, args.schema)

    options = build_options_sheet(args.frontend_bank, args.font_bank)
    helpers = build_helper_sheet(args.frontend_bank, args.font_bank)
    options.save(args.options_output)
    helpers.save(args.helpers_output)
    if args.about_output:
        about = build_about_frame(args.frontend_bank, args.font_bank)
        about.save(args.about_output)
    if args.credits_output:
        credits = build_credits_frame(args.frontend_bank)
        credits.save(args.credits_output)
    if args.profiles_output:
        profiles = build_profiles_screen_sheet(
            args.frontend_bank, args.font_bank)
        profiles.save(args.profiles_output)
    if args.redefine_scroll_pages_output:
        scroll_pages = build_redefine_scroll_pages(
            args.frontend_bank, args.font_bank)
        scroll_pages.save(args.redefine_scroll_pages_output)
    component_output = ""
    if args.components_output:
        components = build_options_sheet(
            args.frontend_bank,
            args.font_bank,
            include_title_rule=False,
            include_selected_button=False,
            include_options_text=False,
            include_options_row_atlas=True,
            include_save_title_area=True,
            include_detail_title_text=False)
        components.save(args.components_output)
        buf, parsed = load_big(args.frontend_bank)
        title_segment = _decode_named(
            buf, parsed, "UI_TEXTBOX_MIDDLE_FE_SPRITE")
        button_left = _decode_named(buf, parsed, "TS_BUTTON_L")
        button_middle = _decode_named(buf, parsed, "TS_BUTTON_M")
        button_right = _decode_named(buf, parsed, "TS_BUTTON_R")
        selected_button = build_selected_button(
            button_left,
            button_middle,
            button_right,
            280)
        title_rule = _build_table_horizontal(
            title_segment,
            title_segment,
            title_segment,
            CANVAS_SIZE[0])
        option_row_layers = [
            components.crop((
                OPTIONS_ROW_ATLAS_ORIGIN_X,
                row * CANVAS_SIZE[1],
                OPTIONS_ROW_ATLAS_ORIGIN_X + CANVAS_SIZE[0],
                (row + 1) * CANVAS_SIZE[1],
            ))
            for row in range(len(OPTIONS_ROWS))
        ]
        # The save-preview viewport is the last-composited layer of every save
        # frame in the live path; rebuild it once (from the same decoded
        # sprites the oracle uses) for the recomposition proof.
        save_viewport_layer = build_save_preview_viewport(
            args.frontend_bank)
        # The save-list selection highlight is a live generated-component quad
        # (InitialiseButtonComponents(248)) positioned by g_SaveSelection, drawn
        # behind the row names.  Rebuild the same 248px TS_BUTTON table the
        # oracle bakes so the recomposition re-adds it as its own under-names
        # layer per save frame.
        save_highlight = _build_table_horizontal(
            button_left,
            button_middle,
            button_right,
            120)
        save_row_font = load_font(args.font_bank, "ENG_ARIAL_16")
        save_title_font = load_font(args.font_bank, "ENG_ARIAL_24")
        text_area_middle = _decode_named(
            buf, parsed, "UI_TEXTBOX_MIDDLE_FE_SPRITE")
        text_area_left = _build_table_horizontal(
            text_area_middle,
            text_area_middle,
            _decode_named(buf, parsed, "UI_TEXTBOX_BL_SPRITE_FE"),
            SAVE_TEXT_AREA_LEFT_WIDTH)
        text_area_right = _build_table_horizontal(
            _decode_named(buf, parsed, "UI_TEXTBOX_BR_SPRITE_FE"),
            text_area_middle,
            text_area_middle,
            SAVE_TEXT_AREA_RIGHT[1])
        title_area_left = _build_table_horizontal(
            text_area_middle,
            text_area_middle,
            _decode_named(buf, parsed, "UI_TEXTBOX_TL_SPRITE_FE"),
            SAVE_TITLE_AREA_LEFT_WIDTH)
        title_area_right = _build_table_horizontal(
            _decode_named(buf, parsed, "UI_TEXTBOX_TR_SPRITE_FE"),
            text_area_middle,
            text_area_middle,
            SAVE_TITLE_AREA_RIGHT[1])
        for frame_index in range(OPTIONS_SHEET_FRAME_COUNT):
            composed = Image.new(
                "RGBA", CANVAS_SIZE, (0, 0, 0, 0))
            if frame_index < SAVE_SCREEN_FRAME_BASE:
                composed.alpha_composite(
                    title_rule,
                    HEADER_RULE_POSITION)
            if frame_index < len(OPTIONS_ROWS):
                composed.alpha_composite(
                    selected_button,
                    (
                        selected_button_left(selected_button.width),
                        OPTIONS_LIST_ORIGIN[1] +
                        OPTIONS_ROWS[frame_index][1] -
                        7,
                    ))
            if frame_index >= SAVE_SCREEN_FRAME_BASE:
                save_frame_index = frame_index - SAVE_SCREEN_FRAME_BASE
                # Live save-list highlight quad: behind the row names (which
                # arrive with the component crop below). The static save base
                # has one frame; selection is live and is not an atlas index.
                composed.alpha_composite(
                    save_highlight,
                    (
                        SAVE_LIST_ORIGIN[0],
                        SAVE_LIST_ORIGIN[1] +
                        save_frame_index *
                        SAVE_ROW_STEP_Y - 7,
                    ))
            if frame_index >= SAVE_SCREEN_FRAME_BASE:
                component_left = SAVE_COMPONENT_ATLAS_ORIGIN[0]
                component_top = SAVE_COMPONENT_ATLAS_ORIGIN[1]
            else:
                component_left = 0
                component_top = frame_index * CANVAS_SIZE[1]
            composed.alpha_composite(components.crop((
                component_left,
                component_top,
                component_left + CANVAS_SIZE[0],
                component_top + CANVAS_SIZE[1],
            )))
            if frame_index >= SAVE_SCREEN_FRAME_BASE:
                # The title frame is a separate asymmetric table family; both
                # its table artwork and Saved Games text are recomposed as
                # live geometry in the executable path.
                composed.alpha_composite(
                    title_area_left,
                    SAVE_TITLE_AREA_ORIGIN)
                composed.alpha_composite(
                    title_area_right,
                    (SAVE_TITLE_AREA_RIGHT[0], SAVE_TITLE_AREA_ORIGIN[1]))
                _draw_text(
                    composed,
                    save_title_font,
                    "Saved Games",
                    HEADER_TEXT_POSITION,
                    "left")
                # UI_TEXT_AREA is a live table family in the executable; add
                # its asymmetric left/right surfaces before runtime text.
                composed.alpha_composite(
                    text_area_left,
                    SAVE_TEXT_AREA_ORIGIN)
                composed.alpha_composite(
                    text_area_right,
                    (SAVE_TEXT_AREA_RIGHT[0], SAVE_TEXT_AREA_ORIGIN[1]))
                # Save row labels are live ENG_ARIAL_16 glyph quads in the
                # executable; mirror that layer in the atlas recomposition
                # proof after the static component cell is attached.
                save_name_center_x = (
                    SAVE_LIST_ORIGIN[0] + save_highlight.width / 2.0)
                for row, filename in enumerate(SAVE_BROWSER_ROWS):
                    _draw_text(
                        composed,
                        save_row_font,
                        filename,
                        (
                            save_name_center_x,
                            SAVE_LIST_ORIGIN[1] + row * SAVE_ROW_STEP_Y,
                        ),
                        "center")
                # File Information and the active profile line are runtime
                # strings as well; mirror their live glyph layer here so the
                # component recomposition remains an exact oracle proof.
                _draw_text(
                    composed,
                    save_title_font,
                    "File Information",
                    (65, 261),
                    "left")
                _draw_text(
                    composed,
                    save_row_font,
                    "Cornelio",
                    (95, 293),
                    "left")
                text_bottom = _decode_named(
                    buf, parsed, "UI_TABLE_TEST_H_T_FE")
                text_bottom = text_bottom.resize(
                    (text_bottom.width * 160, text_bottom.height))
                composed.alpha_composite(
                    text_bottom,
                    SAVE_TEXT_BOTTOM_ORIGIN)
                bottom_backdrop = _decode_named(
                    buf, parsed, "HUD_TEXTBOX_BACK_FE").resize((640, 248))
                composed.alpha_composite(
                    bottom_backdrop,
                    SAVE_BOTTOM_BACKDROP_ORIGIN)
            if frame_index < len(OPTIONS_ROWS):
                for row_layer in option_row_layers:
                    composed.alpha_composite(row_layer)
            if 4 <= frame_index < SAVE_SCREEN_FRAME_BASE:
                # Detail titles are deliberately live in the component atlas
                # path.  Re-add the profile-less retail title here for the
                # zero-dynamic-state recomposition proof; the executable adds
                # the active profile prefix through its runtime glyph path.
                title_font = load_font(args.font_bank, "ENG_ARIAL_24")
                _draw_text(
                    composed,
                    title_font,
                    ("Gameplay Options", "Audio Options", "Video Options",
                     "Redefine Keys")[frame_index - 4],
                    HEADER_TEXT_POSITION,
                    "left")
                if frame_index < 7:
                    detail_rows = (
                        GAMEPLAY_ROWS,
                        AUDIO_ROWS,
                        VIDEO_ROWS,
                    )[frame_index - 4]
                    detail_y = (
                        tuple(90 + row * 30 for row in range(len(GAMEPLAY_ROWS))),
                        (180, 210, 240),
                        tuple(90 + row * 30 for row in range(len(VIDEO_ROWS))),
                    )[frame_index - 4]
                    for row, y in zip(detail_rows, detail_y):
                        _draw_text(
                            composed,
                            title_font,
                            row[0],
                            (132, y + 2),
                            "left")
                _draw_text(
                    composed,
                    title_font,
                    "Cancel",
                    (148, 445),
                    "center")
                if frame_index != 7:
                    _draw_text(
                        composed,
                        title_font,
                        "Defaults",
                        (320, 405),
                        "center")
                _draw_text(
                    composed,
                    title_font,
                    "Apply",
                    (490, 445),
                    "center",
                    colour=(128, 128, 128, 255))
            if frame_index >= SAVE_SCREEN_FRAME_BASE:
                # Reproduce the live save-preview viewport path exactly: the live
                # cell-background quads SKIP the 256x256 ring rect (drawn as four
                # surrounding quads), so punch the same hole here -- clearing the
                # rule/list/File-Info under the panel -- then emit the viewport
                # as the final layer, mirroring the live ring quad drawn last.
                # This makes the recomposition PIXEL_IDENTICAL to the holed
                # oracle with no band mask.
                ox, oy = SAVE_VIEW_RING_ORIGIN
                composed.paste((0, 0, 0, 0), (ox, oy, ox + 256, oy + 256))
                composed.alpha_composite(
                    save_viewport_layer,
                    SAVE_VIEW_RING_ORIGIN)
            oracle = options.crop((
                0,
                frame_index * CANVAS_SIZE[1],
                CANVAS_SIZE[0],
                (frame_index + 1) * CANVAS_SIZE[1],
            ))
            recomposition_diff = ImageChops.difference(composed, oracle)
            if recomposition_diff.getbbox():
                raise ValueError(
                    "live title/selection/row composition differs from "
                    "the baked sheet oracle in frame %d" %
                    frame_index)
        title_segment.save(args.title_segment_output)
        button_left.save(args.button_left_output)
        button_middle.save(args.button_middle_output)
        button_right.save(args.button_right_output)
        component_output = (
            " components=%dx%d title=%dx%d button=%dx%d+%dx%d+%dx%d "
            "bake_selfcheck=BAKE_SELF_CONSISTENT outputs=%s,%s,%s,%s,%s" %
            (
                components.width,
                components.height,
                title_segment.width,
                title_segment.height,
                button_left.width,
                button_left.height,
                button_middle.width,
                button_middle.height,
                button_right.width,
                button_right.height,
                args.components_output,
                args.title_segment_output,
                args.button_left_output,
                args.button_middle_output,
                args.button_right_output,
            ))
    print(
        "FABLE_FRONTEND_SUBSCREENS PASS "
        "options=%dx%d helpers=%dx%d outputs=%s,%s%s" %
        (
            options.width,
            options.height,
            helpers.width,
            helpers.height,
            args.options_output,
            args.helpers_output,
            component_output,
        ))


if __name__ == "__main__":
    main()

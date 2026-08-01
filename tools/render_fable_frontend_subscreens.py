#!/usr/bin/env python3
"""Render recovered retail frontend Options, detail, and Quit overlays.

The layout is taken directly from the shipped ``frontend.bin`` records:

* ``UI_FRONTEND_OPTIONS_SUB_MENU`` (#211)
* ``UI_FRONTEND_LIST_OPTIONS_SUB_MENU`` (#219), origin (200, 150)
* ``UI_FRONTEND_QUIT_PROMPT`` (#631)
* ``UI_FRONTEND_QUIT_YES/NO`` (#595/#596)

Final screen-title composition uses the shared 640-pixel header center x=320.
The serialized x=65/left-aligned title child remains a table-local record and
is not treated as the final flat-surface text origin.

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


CANVAS_SIZE = (640, 480)
HEADER_RULE_POSITION = (0, 35)
HEADER_TEXT_POSITION = (CANVAS_SIZE[0] // 2, 44)

# UI_FRONTEND_ABOUT_MENU (#449) header sits 30px higher than the Options
# header: UI_TABLE_TITLE_WHOLE_ABOUT (#121) is serialized at (0,5) and
# UI_TEXT_ABOUT_MENU_TITLE (#450) at (65,14) -> resolved centre x=320.
ABOUT_RULE_POSITION = (0, 5)
ABOUT_TITLE_TEXT_POSITION = (CANVAS_SIZE[0] // 2, 14)
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
# baked arrow and renders over the same design rect (x=300, width 200).
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
    ("Resolution", "1024 x 768", 0.0),
    ("Refresh Rate", "60 Hz", 0.0),
    ("Anti-Aliasing", "OFF", 0.0),
    ("Texture Detail", None, 1.0 / 3.0),
    ("Mesh Detail", None, 1.0 / 3.0),
    ("Shadow Detail", None, 1.0 / 3.0),
    ("Brightness", None, 0.5),
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
    ("800 x 600", "1024 x 768", "1280 x 720"),
    ("60 Hz", "75 Hz", "85 Hz", "100 Hz"),
    ("OFF", "2x", "4x"),
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
# What is NOT sourced is each id's on-screen *display name*: retail strips the
# EGameAction integer->name strings (docs/HANDOFF.md "REMAINING GAP"). Three
# sourcing passes have now failed -- two binary scans and a web pass
# (StrategyWiki/GameFAQs 403; fabletlcmod wiki carries no id->name table). The
# off-page rows therefore stay unrendered rather than being labelled with
# assumed names, per the project's evidence-not-assumption rule. Closing this
# needs controls_def.hpp/inputkey.h headers or a live in-game binding capture.
REDEFINE_FULL_ACTION_ORDER = (
    60, 9, 7, 8, 31, 45, 6, 13, 14, 1, 32, 26, 86, 94, 78, 4, 113, 112, 72,
    56, 92, 90, 96, 91, 97, 93, 98, 99, 100, 55, 53)
REDEFINE_ACTION_ORDER = (60, 9, 7, 8, 31, 45, 6, 13, 14)
REDEFINE_ROWS = (
    ("Move Forward", "W"),
    ("Move Back", "S"),
    ("Move Left", "A"),
    ("Move Right", "D"),
    ("Attack", "Mouse 1"),
    ("Block", "Mouse 3"),
    ("Flourish", "Mouse 2"),
    ("Run", "Mouse 2"),
    ("Toggle First Person Targeting", "Mouse 2"),
)
REDEFINE_KEY_VALUES = (
    "W / A / S / D",
    "Mouse 1",
    "Mouse 3",
    "Mouse 2",
    "Tab",
    "Q",
    "E",
    "PRESS CONTROL",
    "UNDEFINED",
) + tuple(chr(code) for code in range(ord("A"), ord("Z") + 1)) + (
    "Space",
    "Left Shift",
    "Left Ctrl",
    "Left Alt",
    "Enter",
    "Backspace",
    "Up",
    "Down",
    "Left",
    "Right",
) + tuple(str(value) for value in range(10))

# Serialized component transforms:
# list #217 (40,115), table #91 (-32,-2), right table child #470
# (368,-3), and generated text children #410/#411 at (0,3)/(380,3).
REDEFINE_LIST_ORIGIN = (40, 115)
REDEFINE_ROW_STEP_Y = 26
REDEFINE_TABLE_OFFSET = (-32, -2)
REDEFINE_RIGHT_SLOT_OFFSET = (368, -3)
REDEFINE_ACTION_TEXT_OFFSET = (0, 3)
REDEFINE_KEY_TEXT_OFFSET = (380, 3)
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
    require_equal(
        "resolved screen-title center",
        HEADER_TEXT_POSITION[0],
        int(
            layout.initial_position(title_rule)[0] +
            layout.decoded(title_rule)["Width"] / 2))
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


def _draw_text(canvas, font, text, position, align="center"):
    line = render_line(
        font,
        text,
        CANVAS_SIZE,
        position,
        align,
        2.0 / 3.0)
    canvas.alpha_composite(add_outline(line, 1))


def _draw_title(
        canvas,
        frontend_bank_data,
        font,
        text,
        include_rule=True):
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
    _draw_text(canvas, font, text, HEADER_TEXT_POSITION, "center")


def _draw_save_title(
        canvas,
        frontend_bank_data,
        font,
        text,
        include_rule=True):
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
    _draw_text(canvas, font, text, HEADER_TEXT_POSITION, "center")


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


def _build_control_tile(font, assets, value):
    width, height = CONTROL_TILE_SIZE
    tile = Image.new("RGBA", CONTROL_TILE_SIZE, (0, 0, 0, 0))
    tile.alpha_composite(assets["left_arrow"], (8, 0))
    tile.alpha_composite(assets["right_arrow"], (162, 0))
    if isinstance(value, str):
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


def _option_assets(buf, parsed):
    return {
        "left_arrow": _decode_named(
            buf, parsed, "FE_SCROLL_DOWN_SPRITE"),
        "right_arrow": _decode_named(
            buf, parsed, "FE_SCROLL_UP_SPRITE"),
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
        "left_arrow_hover": _decode_named(
            buf, parsed, "FE_SCROLL_DOWN_HOVERED_SPRITE"),
        "right_arrow_hover": _decode_named(
            buf, parsed, "FE_SCROLL_UP_HOVERED_SPRITE"),
    }


def _draw_detail_helpers(canvas, font, assets):
    # UI_HELPERS_* contributes a parent y offset of 20.
    _draw_helper(canvas, assets["back"], font, (20, 440), "Cancel")
    _draw_helper(canvas, assets["accept"], font, (362, 440), "Apply")
    _draw_helper(canvas, assets["button"], font, (192, 400), "Defaults")


def build_settings_frame(
        frontend_bank,
        font_bank,
        title,
        rows,
        row_ys,
        include_title_rule=True):
    buf, parsed = load_big(frontend_bank)
    font = load_font(font_bank, "ENG_ARIAL_24")
    assets = _option_assets(buf, parsed)
    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    _draw_title(
        canvas,
        (buf, parsed),
        font,
        title,
        include_title_rule)
    for row, y in zip(rows, row_ys):
        _draw_option_label(canvas, font, row[0], y)
    _draw_detail_helpers(canvas, font, assets)
    return canvas


def build_redefine_frame(
        frontend_bank,
        font_bank,
        hovered_index=None,
        include_key_text=True,
        include_title_rule=True):
    if hovered_index is not None and not 0 <= hovered_index < len(REDEFINE_ROWS):
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
            _build_stretched(slot_left, slot_middle, slot_right, 280),
            right_slot,
        )

    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    _draw_title(
        canvas,
        (buf, parsed),
        font,
        "Redefine Keys",
        include_title_rule)
    for index, (label, key) in enumerate(REDEFINE_ROWS):
        base_x = REDEFINE_LIST_ORIGIN[0]
        base_y = (
            REDEFINE_LIST_ORIGIN[1] +
            index * REDEFINE_ROW_STEP_Y
        )
        table_x = base_x + REDEFINE_TABLE_OFFSET[0]
        table_y = base_y + REDEFINE_TABLE_OFFSET[1]
        state = "ON" if index == hovered_index else "OFF"
        canvas.alpha_composite(
            slots[state][0],
            (table_x, table_y))
        canvas.alpha_composite(
            slots[state][1],
            (
                table_x + REDEFINE_RIGHT_SLOT_OFFSET[0],
                table_y + REDEFINE_RIGHT_SLOT_OFFSET[1],
            ))
        _draw_text(
            canvas,
            row_font,
            label,
            (
                base_x + REDEFINE_ACTION_TEXT_OFFSET[0],
                base_y + REDEFINE_ACTION_TEXT_OFFSET[1] +
                REDEFINE_TEXT_RENDER_Y_BIAS,
            ),
            "left")
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

    canvas.alpha_composite(
        _decode_named(buf, parsed, "FE_SCROLL_UP_SPRITE"),
        (304, 80))
    canvas.alpha_composite(
        _decode_named(buf, parsed, "FE_SCROLL_DOWN_SPRITE"),
        (304, 350))
    _draw_text(
        canvas,
        font,
        "Warning: Some controls are undefined",
        (320, 380))

    # UI_HELPERS_REDEFINE contains Apply/Cancel and two 320-wide reset rows.
    _draw_helper(canvas, assets["back"], font, (20, 440), "Cancel")
    _draw_helper(canvas, assets["accept"], font, (362, 440), "Apply")
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
        include_title_area)

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
    text_bottom = _decode_named(buf, parsed, "UI_TABLE_TEST_H_T_FE")
    text_bottom = text_bottom.resize((text_bottom.width * 160, text_bottom.height))
    canvas.alpha_composite(text_bottom, SAVE_TEXT_BOTTOM_ORIGIN)

    # ConstructFileDescription installs the header + metadata children only after
    # selection metadata has been decoded (a runtime boundary not recovered yet).
    # Keep this visual checkpoint honest: show the recovered File Information
    # header on its backdrop plus the current local profile label.
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
    rule + centred title, then the static legal-notice message, then the single
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
        include_save_title_area=None):
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
            include_title_rule),
        build_settings_frame(
            frontend_bank,
            font_bank,
            "Audio Options",
            AUDIO_ROWS,
            (130, 190, 250),
            include_title_rule),
        build_settings_frame(
            frontend_bank,
            font_bank,
            "Video Options",
            VIDEO_ROWS,
            tuple(90 + row * 30 for row in range(len(VIDEO_ROWS))),
            include_title_rule),
        build_redefine_frame(
            frontend_bank,
            font_bank,
            include_key_text=False,
            include_title_rule=include_title_rule),
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
            include_title_area=include_save_title_area)
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
    tile_index = 0
    for screen_values in CONTROL_VALUE_GROUPS:
        for row_values in screen_values:
            for value in row_values:
                column = tile_index % CONTROL_ATLAS_COLUMNS
                tile_row = tile_index // CONTROL_ATLAS_COLUMNS
                sheet.alpha_composite(
                    _build_control_tile(font, assets, value),
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
            2.0 / 3.0)
        if key_index == 7:
            alpha = line.getchannel("A")
            line = Image.new(
                "RGBA", REDEFINE_KEY_TILE_SIZE, (255, 255, 0, 0))
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

    return sheet


def _bake_detail_hover_overlays(sheet, font, assets):
    # Arrows: full 200x30 control tiles carrying only the HOVERED arrow at the
    # SAME local offset as the baked control-tile arrow (left 8, right 162), so
    # the overlay pixel-aligns with the OFF arrow when drawn over design x=300.
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
        parent = (design_origin[0], design_origin[1] + 16)
        footer_canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
        _draw_helper(footer_canvas, glyph, font, parent, label)
        footer_tile = footer_canvas.crop((
            design_origin[0],
            design_origin[1],
            design_origin[0] + fw,
            design_origin[1] + fh,
        ))
        sheet.alpha_composite(
            footer_tile,
            (DETAIL_HOVER_ATLAS_ORIGIN_X, atlas_y))


def _draw_helper(canvas, image, font, parent, text):
    canvas.alpha_composite(image, (parent[0], parent[1] - 16))
    _draw_text(
        canvas,
        font,
        text,
        (parent[0] + 128, parent[1] + 5))


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
            include_key_text=False)
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
    parser.add_argument("--components-output")
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

    options = build_options_sheet(args.frontend_bank, args.font_bank)
    helpers = build_helper_sheet(args.frontend_bank, args.font_bank)
    options.save(args.options_output)
    helpers.save(args.helpers_output)
    if args.about_output:
        about = build_about_frame(args.frontend_bank, args.font_bank)
        about.save(args.about_output)
    component_output = ""
    if args.components_output:
        components = build_options_sheet(
            args.frontend_bank,
            args.font_bank,
            include_title_rule=False,
            include_selected_button=False,
            include_options_text=False,
            include_options_row_atlas=True,
            include_save_title_area=True)
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
            if frame_index < len(OPTIONS_ROWS):
                for row_layer in option_row_layers:
                    composed.alpha_composite(row_layer)
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

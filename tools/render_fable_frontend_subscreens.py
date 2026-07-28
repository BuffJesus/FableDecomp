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

The output consists of two vertical sheets.  The Options sheet has one
640x480 frame per selected row followed by the Gameplay, Audio, Video, and
Redefine Keys destination screens.  The helper sheet has Back off/on, the
Quit prompt with neither/No/Yes hovered, and one compact atlas frame holding
the nine complete retail Redefine Keys ON rows in the initial viewport.
"""

import argparse
import os
import sys

from PIL import Image

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
)
from texture_build import load_big  # noqa: E402


CANVAS_SIZE = (640, 480)
HEADER_RULE_POSITION = (0, 35)
HEADER_TEXT_POSITION = (CANVAS_SIZE[0] // 2, 44)
OPTIONS_SHEET_WIDTH = 1024
CONTROL_TILE_SIZE = (200, 30)
CONTROL_ATLAS_ORIGIN_X = 640
CONTROL_ATLAS_COLUMNS = 1
REDEFINE_KEY_ATLAS_ORIGIN_X = 840
REDEFINE_KEY_TILE_SIZE = (176, 26)
HELPER_SHEET_FRAME_COUNT = 6
REDEFINE_HOVER_HELPER_FRAME = 5
REDEFINE_HOVER_STRIP_SIZE = (588, 35)
REDEFINE_RESET_HOVER_ATLAS_Y = 320
REDEFINE_RESET_HOVER_SIZE = (320, 64)
OPTIONS_LIST_ORIGIN = (200, 150)
OPTIONS_ROWS = (
    ("Gameplay Options", 0),
    ("Video Options", 30),
    ("Audio Options", 60),
    ("Redefine Keys", 90),
)
DETAIL_SCREEN_COUNT = 4

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
    """Reproduce CTable's horizontal inner-span and corner composition."""
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


def _draw_title(canvas, frontend_bank_data, font, text):
    buf, parsed = frontend_bank_data
    # UI_TABLE_TITLE_WHOLE maps TOP_LEFT, TOP_RIGHT, and HORIZONTAL_TOP to
    # the same UI_TEXTBOX_MIDDLE component (#122). Its GraphicIndex resolves
    # to this installed gold double-rule sprite; the blue FRONTEND_BUTTON
    # trio belongs to UI_BUTTON/UI_BUTTON_BIG selection tables.
    title_segment = _decode_named(
        buf, parsed, "UI_TEXTBOX_MIDDLE_FE_SPRITE")
    # CTable treats Width as the tiled inner span. Natural-size corner
    # components sit around that span; the 640-pixel design canvas clips the
    # outer tail exactly as the retail viewport does.
    title_rule = _build_table_horizontal(
        title_segment,
        title_segment,
        title_segment,
        640)
    canvas.alpha_composite(title_rule, HEADER_RULE_POSITION)
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
    }


def _draw_detail_helpers(canvas, font, assets):
    # UI_HELPERS_* contributes a parent y offset of 20.
    _draw_helper(canvas, assets["back"], font, (20, 440), "Cancel")
    _draw_helper(canvas, assets["accept"], font, (362, 440), "Apply")
    _draw_helper(canvas, assets["button"], font, (192, 400), "Defaults")


def build_settings_frame(frontend_bank, font_bank, title, rows, row_ys):
    buf, parsed = load_big(frontend_bank)
    font = load_font(font_bank, "ENG_ARIAL_24")
    assets = _option_assets(buf, parsed)
    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    _draw_title(canvas, (buf, parsed), font, title)
    for row, y in zip(rows, row_ys):
        _draw_option_label(canvas, font, row[0], y)
    _draw_detail_helpers(canvas, font, assets)
    return canvas


def build_redefine_frame(
        frontend_bank,
        font_bank,
        hovered_index=None,
        include_key_text=True):
    if hovered_index is not None and not 0 <= hovered_index < len(REDEFINE_ROWS):
        raise ValueError("redefine hover is outside the compiled list")

    buf, parsed = load_big(frontend_bank)
    font = load_font(font_bank, "ENG_ARIAL_24")
    row_font = load_font(font_bank, "ENG_ARIAL_12")
    assets = _option_assets(buf, parsed)
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
            _build_stretched(slot_left, slot_middle, slot_right, 220),
        )

    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    _draw_title(canvas, (buf, parsed), font, "Redefine Keys")
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
                base_y + REDEFINE_ACTION_TEXT_OFFSET[1],
            ),
            "left")
        if include_key_text:
            _draw_text(
                canvas,
                row_font,
                key,
                (
                    base_x + REDEFINE_KEY_TEXT_OFFSET[0],
                    base_y + REDEFINE_KEY_TEXT_OFFSET[1],
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


def build_options_frame(frontend_bank, font_bank, selected_index=0):
    if not 0 <= selected_index < len(OPTIONS_ROWS):
        raise ValueError("options selection is outside the compiled list")

    buf, parsed = load_big(frontend_bank)
    selected = build_selected_button(
        _decode_named(buf, parsed, "TS_BUTTON_L"),
        _decode_named(buf, parsed, "TS_BUTTON_M"),
        _decode_named(buf, parsed, "TS_BUTTON_R"),
        280)
    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    font = load_font(font_bank, "ENG_ARIAL_24")
    _draw_title(canvas, (buf, parsed), font, "Options")
    selected_y = OPTIONS_ROWS[selected_index][1]
    canvas.alpha_composite(
        selected,
        (
            selected_button_left(selected.width),
            OPTIONS_LIST_ORIGIN[1] + selected_y - 7,
        ))

    for text, y_offset in OPTIONS_ROWS:
        _draw_text(
            canvas,
            font,
            text,
            (
                MENU_CONTENT_CENTER_X,
                OPTIONS_LIST_ORIGIN[1] + y_offset,
            ))
    return canvas


def build_options_sheet(frontend_bank, font_bank):
    frames = [
        build_options_frame(frontend_bank, font_bank, selected)
        for selected in range(len(OPTIONS_ROWS))
    ]
    frames.extend((
        build_settings_frame(
            frontend_bank,
            font_bank,
            "Gameplay Options",
            GAMEPLAY_ROWS,
            tuple(90 + row * 30 for row in range(len(GAMEPLAY_ROWS)))),
        build_settings_frame(
            frontend_bank,
            font_bank,
            "Audio Options",
            AUDIO_ROWS,
            (130, 190, 250)),
        build_settings_frame(
            frontend_bank,
            font_bank,
            "Video Options",
            VIDEO_ROWS,
            tuple(90 + row * 30 for row in range(len(VIDEO_ROWS)))),
        build_redefine_frame(
            frontend_bank,
            font_bank,
            include_key_text=False),
    ))
    sheet = Image.new(
        "RGBA",
        (OPTIONS_SHEET_WIDTH, CANVAS_SIZE[1] * len(frames)),
        (0, 0, 0, 0))
    for index, frame in enumerate(frames):
        sheet.alpha_composite(frame, (0, index * CANVAS_SIZE[1]))

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
            (0, REDEFINE_KEY_TEXT_OFFSET[1]),
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

    return sheet


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
    # filtering seam or any change to the 1024x3840 Options texture.
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
    parser.add_argument("--game-root")
    parser.add_argument("--schema")
    args = parser.parse_args()

    if bool(args.game_root) != bool(args.schema):
        parser.error("--game-root and --schema must be supplied together")
    if args.game_root:
        validate_compiled_subscreen_layout(args.game_root, args.schema)

    options = build_options_sheet(args.frontend_bank, args.font_bank)
    helpers = build_helper_sheet(args.frontend_bank, args.font_bank)
    options.save(args.options_output)
    helpers.save(args.helpers_output)
    print(
        "FABLE_FRONTEND_SUBSCREENS PASS "
        "options=%dx%d helpers=%dx%d outputs=%s,%s" %
        (
            options.width,
            options.height,
            helpers.width,
            helpers.height,
            args.options_output,
            args.helpers_output,
        ))


if __name__ == "__main__":
    main()

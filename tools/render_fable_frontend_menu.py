#!/usr/bin/env python3
"""Render the retail PC first-main-menu overlay from shipped assets.

The geometry comes from frontend.bin:

* UI_FRONTEND_MAIN_MENU (#212)
* UI_FRONTEND_LIST_MAIN_MENU (#242), origin (200, 200)
* its seven button offsets: 0, 30, 60, 120, 180, 210, 240
* each text child at x=120, producing design-space x=320
* UI_BUTTON_BIG (#74), selected at x=-140/y=-7 with width 400

The serialized button positions are table-local.  The retail ``CTable``
materialization step biases the generated ornament pieces 60 pixels to the
right, so the final ornament and its centered text child share design-space
x=320.

The title, selected-button ornaments, and font atlas are all decoded from the
retail banks.  Only the English text-bank resolution is expressed here.
"""

import argparse
import os
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from render_fable_static_font import (  # noqa: E402
    add_outline,
    load_font,
    render_line,
)
from frontend_layout_oracle import (  # noqa: E402
    FrontendLayoutOracle,
    require_equal,
    require_position,
)
from texture_build import decode_entry, find_entry, load_big  # noqa: E402


CANVAS_SIZE = (640, 480)
TITLE_POSITION = (70, 30)
LIST_ORIGIN = (200, 200)
TEXT_X_OFFSET = 120
MENU_CONTENT_CENTER_X = LIST_ORIGIN[0] + TEXT_X_OFFSET
BUTTON_TABLE_X_BY_WIDTH = {
    400: -140,
    280: -80,
}
# ConstructHorizontalLine @ 0x005518E0 advances through the 64-pixel left
# sprite; its component origin is 60 in the recovered UI table state.
GENERATED_BUTTON_CHILD_X = 60
MENU_ROWS = (
    ("Continue Game", 0),
    ("Change Profile", 30),
    ("Options", 60),
    ("Games for Windows - LIVE", 120),
    ("Credits", 180),
    ("About", 210),
    ("Quit", 240),
)
BUFF_JESUS_MENU_ROWS = (
    ("Continue the Gains", 0),
    ("Summon BuffJesus", 30),
    ("Sacred Settings", 60),
    ("Gains for Windows - LIFT", 120),
    ("Disciples", 180),
    ("About BuffJesus", 210),
    ("Skip Leg Day", 240),
)
MENU_TEXT_VARIANTS = {
    "retail": MENU_ROWS,
    "buff-jesus": BUFF_JESUS_MENU_ROWS,
}


def _decode_named(buf, parsed, name):
    hits = find_entry(parsed, name)
    if not hits:
        raise ValueError("frontend texture is absent: %s" % name)
    _, entry = hits[0]
    payload = buf[entry["offset"]:entry["offset"] + entry["size"]]
    descriptor, mips = decode_entry(entry["info"], payload, max_mips=1)
    return Image.fromarray(
        mips[0][:descriptor["height"], :descriptor["width"]])


def build_selected_button(left, middle, right, width=400):
    if left.size != (64, 64) or middle.size != (8, 64) or right.size != (64, 64):
        raise ValueError("retail selected-button pieces have unexpected sizes")
    if width < left.width + right.width:
        raise ValueError("selected-button width is too small")
    selected = Image.new("RGBA", (width, 64), (0, 0, 0, 0))
    selected.alpha_composite(left, (0, 0))
    inner_width = width - left.width - right.width
    for x in range(0, inner_width, middle.width):
        available = min(middle.width, inner_width - x)
        selected.alpha_composite(
            middle.crop((0, 0, available, middle.height)),
            (left.width + x, 0))
    selected.alpha_composite(right, (width - right.width, 0))
    return selected


def selected_button_left(width):
    """Resolve the table and generated-child positions used by retail."""
    try:
        table_x = BUTTON_TABLE_X_BY_WIDTH[width]
    except KeyError:
        raise ValueError("unknown compiled selected-button width: %s" % width)
    return LIST_ORIGIN[0] + table_x + GENERATED_BUTTON_CHILD_X


def menu_rows_for_variant(text_variant):
    try:
        return MENU_TEXT_VARIANTS[text_variant]
    except KeyError:
        raise ValueError("unknown frontend text variant: %s" % text_variant)


def validate_compiled_menu_layout(game_root, schema_path):
    """Gate every authored main-menu constant against the shipped records."""
    layout = FrontendLayoutOracle(game_root, schema_path)
    list_name = "UI_FRONTEND_LIST_MAIN_MENU"
    list_values = layout.decoded(list_name)
    require_position(
        "main-menu list origin",
        layout.initial_position(list_name),
        LIST_ORIGIN)
    require_equal(
        "main-menu list step",
        list_values["PositionOffsetY"],
        30.0)

    button_names = (
        "UI_FRONTEND_BUTTON_LOAD_GAME",
        "UI_FRONTEND_BUTTON_CHANGE_PROFILE",
        "UI_FRONTEND_BUTTON_OPTIONS",
        "UI_FRONTEND_BUTTON_LIVE_AWARE",
        "UI_FRONTEND_BUTTON_CREDITS",
        "UI_FRONTEND_BUTTON_ABOUT",
        "UI_FRONTEND_BUTTON_QUIT",
    )
    require_equal(
        "main-menu child order",
        layout.child_names(list_name),
        button_names)
    require_equal(
        "main-menu row offsets",
        tuple(layout.initial_position(name)[1] for name in button_names),
        tuple(float(offset) for _, offset in MENU_ROWS))
    require_equal(
        "main-menu actions",
        tuple(layout.decoded(name)["Action"] for name in button_names),
        (66, 16, 297, 10, 67, 321, 314))

    require_equal(
        "large button width",
        layout.decoded("UI_BUTTON_BIG")["Width"],
        400.0)
    require_position(
        "large button table position",
        layout.initial_position("UI_BUTTON_BIG"),
        (-140, -7))
    require_equal(
        "regular button width",
        layout.decoded("UI_BUTTON")["Width"],
        280.0)
    require_position(
        "regular button table position",
        layout.initial_position("UI_BUTTON"),
        (-80, -7))

    for index, outer_name in enumerate(button_names):
        inner_index = layout.child_indices(outer_name)[0]
        inner_children = layout.child_indices(inner_index)
        text_values = layout.decoded(inner_children[0])
        require_position(
            "main-menu text position %d" % index,
            layout.initial_position(inner_children[0]),
            (TEXT_X_OFFSET, 0))
        require_equal(
            "main-menu text font %d" % index,
            text_values["Font"],
            "ENG_ARIAL_24")
        require_equal(
            "main-menu table child %d" % index,
            inner_children[1],
            74 if index == 0 else 73)

    require_position(
        "main-menu title position",
        layout.initial_position("UI_TITLE"),
        TITLE_POSITION)


def build_menu_overlay(
        frontend_bank,
        font_bank,
        selected_index=0,
        menu_rows=MENU_ROWS):
    if not 0 <= selected_index < len(menu_rows):
        raise ValueError("main-menu selection is outside the compiled list")
    buf, parsed = load_big(frontend_bank)
    title_left = _decode_named(buf, parsed, "FRONTEND_TITLE_01_SPRITE")
    title_right = _decode_named(buf, parsed, "FRONTEND_TITLE_02_SPRITE")
    selected_width = 400 if selected_index == 0 else 280
    selected = build_selected_button(
        _decode_named(buf, parsed, "TS_BUTTON_L"),
        _decode_named(buf, parsed, "TS_BUTTON_M"),
        _decode_named(buf, parsed, "TS_BUTTON_R"),
        selected_width)

    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    canvas.alpha_composite(title_left, TITLE_POSITION)
    canvas.alpha_composite(
        title_right, (TITLE_POSITION[0] + title_left.width, TITLE_POSITION[1]))

    # UI_BUTTON_BIG (#74) backs Continue Game; UI_BUTTON (#73) backs the
    # remaining six rows. CTable materializes both around the text anchor.
    selected_y = menu_rows[selected_index][1]
    canvas.alpha_composite(
        selected,
        (
            selected_button_left(selected_width),
            LIST_ORIGIN[1] + selected_y - 7,
        ))

    font = load_font(font_bank, "ENG_ARIAL_24")
    for text, y_offset in menu_rows:
        line = render_line(
            font,
            text,
            CANVAS_SIZE,
            (MENU_CONTENT_CENTER_X, LIST_ORIGIN[1] + y_offset),
            "center",
            2.0 / 3.0)
        canvas.alpha_composite(add_outline(line, 1))
    return canvas


def build_menu_sheet(frontend_bank, font_bank, menu_rows=MENU_ROWS):
    frames = [
        build_menu_overlay(
            frontend_bank,
            font_bank,
            selected_index,
            menu_rows)
        for selected_index in range(len(menu_rows))
    ]
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
    parser.add_argument("output")
    parser.add_argument(
        "--text-variant",
        choices=tuple(sorted(MENU_TEXT_VARIANTS)),
        default="retail")
    parser.add_argument("--game-root")
    parser.add_argument("--schema")
    args = parser.parse_args()

    if bool(args.game_root) != bool(args.schema):
        parser.error("--game-root and --schema must be supplied together")
    if args.game_root:
        validate_compiled_menu_layout(args.game_root, args.schema)

    menu_rows = menu_rows_for_variant(args.text_variant)
    image = build_menu_sheet(args.frontend_bank, args.font_bank, menu_rows)
    image.save(args.output)
    print(
        "FABLE_FRONTEND_MENU PASS sheet=%dx%d frames=%d variant=%s output=%s" %
        (
            image.width,
            image.height,
            len(menu_rows),
            args.text_variant,
            args.output,
        ))


if __name__ == "__main__":
    main()

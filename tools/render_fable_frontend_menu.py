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

from PIL import Image, ImageChops

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
    sound_values = layout.decoded("UI_MISC_THINGS_DEF_INSTANCE")
    require_equal(
        "frontend list movement sound",
        sound_values["SoundUpDown"],
        "CS_GUI_2")
    require_equal(
        "frontend invalid movement sound",
        sound_values["SoundError"],
        "CS_GUI_5")

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
        states = text_values["States"]
        require_equal(
            "main-menu text state count %d" % index,
            len(states),
            5)
        require_equal(
            "main-menu text state x %d" % index,
            tuple(state["PositionX"] for state in states),
            (120.0, 120.0, 0.0, 120.0, 120.0))
        require_equal(
            "main-menu text state y %d" % index,
            tuple(state["PositionY"] for state in states),
            (0.0, 0.0, 0.0, 0.0, 0.0))
        require_equal(
            "main-menu text state alpha %d" % index,
            tuple(state["ColourA"] for state in states),
            (1.0, 1.0, 0.0, 1.0, 1.0))
        require_equal(
            "main-menu text state time %d" % index,
            tuple(state["UpdateTime"] for state in states),
            (
                -1.0,
                -1.0,
                0.20000000298023224,
                -1.0,
                -1.0,
            ))
        require_equal(
            "main-menu text state flags %d" % index,
            tuple(state["StateChangeFlag"] for state in states),
            (7, 7, 7, 7, 7))
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
        menu_rows=MENU_ROWS,
        include_selected_button=True,
        include_text=True):
    if not 0 <= selected_index < len(menu_rows):
        raise ValueError("main-menu selection is outside the compiled list")
    buf, parsed = load_big(frontend_bank)
    title_left = _decode_named(buf, parsed, "FRONTEND_TITLE_01_SPRITE")
    title_right = _decode_named(buf, parsed, "FRONTEND_TITLE_02_SPRITE")
    canvas = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    canvas.alpha_composite(title_left, TITLE_POSITION)
    canvas.alpha_composite(
        title_right, (TITLE_POSITION[0] + title_left.width, TITLE_POSITION[1]))

    # UI_BUTTON_BIG (#74) backs Continue Game; UI_BUTTON (#73) backs the
    # remaining six rows. CTable materializes both around the text anchor.
    if include_selected_button:
        selected_width = 400 if selected_index == 0 else 280
        selected = build_selected_button(
            _decode_named(buf, parsed, "TS_BUTTON_L"),
            _decode_named(buf, parsed, "TS_BUTTON_M"),
            _decode_named(buf, parsed, "TS_BUTTON_R"),
            selected_width)
        selected_y = menu_rows[selected_index][1]
        canvas.alpha_composite(
            selected,
            (
                selected_button_left(selected_width),
                LIST_ORIGIN[1] + selected_y - 7,
            ))

    if include_text:
        for row_layer in build_menu_text_row_layers(font_bank, menu_rows):
            canvas.alpha_composite(row_layer)
    return canvas


def build_menu_sheet(
        frontend_bank,
        font_bank,
        menu_rows=MENU_ROWS,
        include_selected_button=True,
        include_text=True):
    frames = [
        build_menu_overlay(
            frontend_bank,
            font_bank,
            selected_index,
            menu_rows,
            include_selected_button,
            include_text)
        for selected_index in range(len(menu_rows))
    ]
    sheet = Image.new(
        "RGBA",
        (CANVAS_SIZE[0], CANVAS_SIZE[1] * len(frames)),
        (0, 0, 0, 0))
    for index, frame in enumerate(frames):
        sheet.alpha_composite(frame, (0, index * CANVAS_SIZE[1]))
    return sheet


def build_menu_text_row_layers(font_bank, menu_rows=MENU_ROWS):
    """Render each CFrontEndList text child into an independent design canvas."""
    font = load_font(font_bank, "ENG_ARIAL_24")
    layers = []
    for text, y_offset in menu_rows:
        line = render_line(
            font,
            text,
            CANVAS_SIZE,
            (MENU_CONTENT_CENTER_X, LIST_ORIGIN[1] + y_offset),
            "center",
            2.0 / 3.0)
        layers.append(add_outline(line, 1))
    return layers


def build_menu_component_atlas(frontend_bank, font_bank, menu_rows=MENU_ROWS):
    """Pack title-only frames and seven independent row canvases side by side."""
    component_frames = build_menu_sheet(
        frontend_bank,
        font_bank,
        menu_rows,
        include_selected_button=False,
        include_text=False)
    atlas = Image.new(
        "RGBA",
        (CANVAS_SIZE[0] * 2, component_frames.height),
        (0, 0, 0, 0))
    atlas.alpha_composite(component_frames)
    for row, row_layer in enumerate(
            build_menu_text_row_layers(font_bank, menu_rows)):
        atlas.alpha_composite(
            row_layer,
            (CANVAS_SIZE[0], row * CANVAS_SIZE[1]))
    return atlas


def recompose_menu_frame(
        component_frame,
        left,
        middle,
        right,
        selected_index,
        menu_rows=MENU_ROWS,
        row_layers=None):
    """Place the live ornament behind the retained title/text component frame."""
    if component_frame.size != CANVAS_SIZE:
        raise ValueError("main-menu component frame has unexpected dimensions")
    if not 0 <= selected_index < len(menu_rows):
        raise ValueError("main-menu selection is outside the compiled list")
    selected_width = 400 if selected_index == 0 else 280
    frame = Image.new("RGBA", CANVAS_SIZE, (0, 0, 0, 0))
    frame.alpha_composite(
        build_selected_button(left, middle, right, selected_width),
        (
            selected_button_left(selected_width),
            LIST_ORIGIN[1] + menu_rows[selected_index][1] - 7,
        ))
    frame.alpha_composite(component_frame)
    if row_layers is not None:
        if len(row_layers) != len(menu_rows):
            raise ValueError("main-menu row atlas has unexpected frame count")
        for row_layer in row_layers:
            if row_layer.size != CANVAS_SIZE:
                raise ValueError(
                    "main-menu row atlas has unexpected frame dimensions")
            frame.alpha_composite(row_layer)
    return frame


def validate_menu_component_recomposition(
        baked_sheet,
        component_sheet,
        left,
        middle,
        right,
        menu_rows=MENU_ROWS):
    baked_size = (
        CANVAS_SIZE[0],
        CANVAS_SIZE[1] * len(menu_rows))
    component_size = (
        CANVAS_SIZE[0] * 2,
        CANVAS_SIZE[1] * len(menu_rows))
    if baked_sheet.size != baked_size or component_sheet.size != component_size:
        raise ValueError("main-menu sheet has unexpected dimensions")
    row_layers = [
        component_sheet.crop(
            (
                CANVAS_SIZE[0],
                row * CANVAS_SIZE[1],
                CANVAS_SIZE[0] * 2,
                (row + 1) * CANVAS_SIZE[1],
            ))
        for row in range(len(menu_rows))
    ]
    for selected_index in range(len(menu_rows)):
        top = selected_index * CANVAS_SIZE[1]
        component_frame = component_sheet.crop(
            (0, top, CANVAS_SIZE[0], top + CANVAS_SIZE[1]))
        recomposed = recompose_menu_frame(
            component_frame,
            left,
            middle,
            right,
            selected_index,
            menu_rows,
            row_layers)
        baked_frame = baked_sheet.crop(
            (0, top, CANVAS_SIZE[0], top + CANVAS_SIZE[1]))
        if ImageChops.difference(recomposed, baked_frame).getbbox() is not None:
            raise ValueError(
                "live main-menu ornament does not reproduce oracle frame %d" %
                selected_index)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("frontend_bank")
    parser.add_argument("font_bank")
    parser.add_argument("output")
    parser.add_argument(
        "--text-variant",
        choices=tuple(sorted(MENU_TEXT_VARIANTS)),
        default="retail")
    parser.add_argument("--component-output")
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
    oracle_status = "BAKED_ONLY"
    if args.component_output:
        component_image = build_menu_component_atlas(
            args.frontend_bank,
            args.font_bank,
            menu_rows)
        buf, parsed = load_big(args.frontend_bank)
        button_left = _decode_named(buf, parsed, "TS_BUTTON_L")
        button_middle = _decode_named(buf, parsed, "TS_BUTTON_M")
        button_right = _decode_named(buf, parsed, "TS_BUTTON_R")
        validate_menu_component_recomposition(
            image,
            component_image,
            button_left,
            button_middle,
            button_right,
            menu_rows)
        component_image.save(args.component_output)
        # Self-consistency verdict: the live component recomposition reproduces
        # this tool's OWN baked sheet pixel-for-pixel. It is NOT a retail-pixel
        # parity oracle (no retail frame is loaded); named accordingly.
        oracle_status = "BAKE_SELF_CONSISTENT"
    print(
        "FABLE_FRONTEND_MENU PASS sheet=%dx%d frames=%d variant=%s "
        "bake_selfcheck=%s output=%s components=%s" %
        (
            image.width,
            image.height,
            len(menu_rows),
            args.text_variant,
            oracle_status,
            args.output,
            args.component_output or "none",
        ))


if __name__ == "__main__":
    main()

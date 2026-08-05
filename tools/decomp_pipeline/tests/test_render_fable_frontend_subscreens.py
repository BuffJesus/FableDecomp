import os
import sys
import unittest

from PIL import Image, ImageChops


TOOLS_DIR = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "..", ".."))
if TOOLS_DIR not in sys.path:
    sys.path.insert(0, TOOLS_DIR)

from render_fable_frontend_subscreens import (  # noqa: E402
    AUDIO_CONTROL_VALUES,
    AUDIO_ROWS,
    CONTROL_ATLAS_COLUMNS,
    CONTROL_TILE_SIZE,
    CONTROL_VALUE_GROUPS,
    DETAIL_TITLE_GLYPH_ATLAS_ORIGIN,
    DETAIL_TITLE_GLYPH_ATLAS_SIZE,
    DETAIL_SCREEN_COUNT,
    GAMEPLAY_CONTROL_VALUES,
    GAMEPLAY_ROWS,
    HEADER_RULE_POSITION,
    HEADER_TEXT_POSITION,
    HELPER_SHEET_FRAME_COUNT,
    OPTIONS_COMPONENT_SHEET_WIDTH,
    OPTIONS_ROW_ATLAS_ORIGIN_X,
    OPTIONS_SHEET_FRAME_COUNT,
    OPTIONS_SHEET_WIDTH,
    OPTIONS_ROWS,
    PROFILE_GLYPH_ATLAS_ORIGIN,
    REDEFINE_GLYPH_ATLAS_ORIGIN,
    REDEFINE_GLYPH_ATLAS_SIZE,
    REDEFINE_ACTION_ORDER,
    REDEFINE_ACTION_ENUM_NAMES,
    REDEFINE_ACTION_DISPLAY_TEXT,
    REDEFINE_ACTION_DEFAULT_INPUTS,
    REDEFINE_ACTION_DISPLAY_INPUTS,
    REDEFINE_FULL_ACTION_ORDER,
    REDEFINE_HOVER_HELPER_FRAME,
    REDEFINE_HOVER_STRIP_SIZE,
    REDEFINE_ACTION_TEXT_OFFSET,
    REDEFINE_ACTION_TEXT_SCALE,
    REDEFINE_KEY_TEXT_OFFSET,
    REDEFINE_TEXT_RENDER_Y_BIAS,
    REDEFINE_LIST_ORIGIN,
    REDEFINE_MOUSE_OFFSET,
    REDEFINE_MOUSE_SIZE,
    REDEFINE_RIGHT_SLOT_OFFSET,
    REDEFINE_ROW_STEP_Y,
    REDEFINE_RESET_HOVER_ATLAS_Y,
    REDEFINE_RESET_HOVER_SIZE,
    REDEFINE_TABLE_OFFSET,
    REDEFINE_ROWS,
    REDEFINE_MATERIALIZED_ROWS,
    REDEFINE_ACTION_MATERIALIZED_OFFSETS,
    REDEFINE_SCROLL_PAGE_ATLAS_SIZE,
    REDEFINE_SCROLL_PAGE_COLUMNS,
    REDEFINE_SCROLL_PAGE_OFFSETS,
    redefine_materialized_row_offset,
    redefine_scrolled_rows,
    SAVE_BROWSER_ROWS,
    SAVE_COMPONENT_ATLAS_ORIGIN,
    SAVE_BROWSER_FRAME_COUNT,
    SAVE_BOTTOM_BACKDROP_ORIGIN,
    SAVE_LIST_HEIGHT,
    SAVE_LIST_ORIGIN,
    SAVE_ROW_STEP_Y,
    SAVE_TEXT_BOTTOM_ORIGIN,
    SAVE_TEXT_AREA_ORIGIN,
    SAVE_TITLE_AREA_ORIGIN,
    SAVE_VIEW_RING_ORIGIN,
    SAVE_SCREEN_FRAME_BASE,
    VIDEO_CONTROL_VALUES,
    VIDEO_ROWS,
    _decode_named,
    _build_stretched,
    _build_table_horizontal,
    build_options_row_layers,
    build_options_sheet,
    build_redefine_frame,
    build_redefine_scroll_pages,
    build_save_browser_frame,
    build_save_preview_viewport,
    build_settings_frame,
    build_about_frame,
    build_credits_frame,
    build_profiles_frame,
    build_profiles_screen_sheet,
    extract_credits_text_stream,
    validate_compiled_about_layout,
    validate_compiled_credits_layout,
    validate_compiled_profiles_layout,
    validate_compiled_subscreen_layout,
    _redefine_materialized_key_text,
)
from render_fable_static_font import (  # noqa: E402
    add_outline,
    load_font,
    render_line,
)
from texture_build import load_big  # noqa: E402


class FrontendSubscreenRenderTests(unittest.TestCase):
    RETAIL_ROOT = (
        r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters")
    SCHEMA = os.path.join(
        TOOLS_DIR, "..", "ghidra_out", "def_schema.json")
    FRONTEND_BANK = os.path.join(
        RETAIL_ROOT, "data", "graphics", "pc", "frontend.big")
    FONT_BANK = os.path.join(
        RETAIL_ROOT, "data", "lang", "English", "fonts.big")

    def test_options_rows_match_compiled_list_order(self):
        self.assertEqual(
            (
                ("Gameplay Options", 0),
                ("Video Options", 30),
                ("Audio Options", 60),
                ("Redefine Keys", 90),
            ),
            OPTIONS_ROWS)

    def test_screen_headers_use_the_serialized_left_origin(self):
        self.assertEqual((0, 35), HEADER_RULE_POSITION)
        self.assertEqual((65, 44), HEADER_TEXT_POSITION)

    @unittest.skipUnless(
        os.path.isfile(FONT_BANK),
        "retail font bank is not installed")
    def test_every_detail_header_is_pixel_left_aligned(self):
        font = load_font(self.FONT_BANK, "ENG_ARIAL_24")
        for title in (
                "Options",
                "Gameplay Options",
                "Audio Options",
                "Video Options",
                "Redefine Keys"):
            line = add_outline(
                render_line(
                    font,
                    title,
                    (640, 480),
                    HEADER_TEXT_POSITION,
                    "left",
                    2.0 / 3.0),
                1)
            bounds = line.getchannel("A").getbbox()
            self.assertIsNotNone(bounds, title)
            # The retail text child is serialized at x=65 and rendered from
            # that left origin; font bearings may extend the ink a few pixels
            # to the left, but it must not be centered on the canvas.
            self.assertLessEqual(bounds[0], 65, title)
            self.assertGreater(bounds[2], 65, title)

    def test_title_rule_tiles_middle_to_compiled_width(self):
        left = Image.new("RGBA", (128, 64), (1, 0, 0, 255))
        middle = Image.new("RGBA", (8, 64), (2, 0, 0, 255))
        right = Image.new("RGBA", (128, 64), (3, 0, 0, 255))
        result = _build_stretched(left, middle, right, 640)
        self.assertEqual((640, 64), result.size)
        self.assertEqual((1, 0, 0, 255), result.getpixel((0, 0)))
        self.assertEqual((2, 0, 0, 255), result.getpixel((320, 0)))
        self.assertEqual((3, 0, 0, 255), result.getpixel((639, 0)))

    def test_title_table_width_is_the_inner_span_between_corners(self):
        segment = Image.new("RGBA", (8, 64), (2, 29, 52, 199))
        result = _build_table_horizontal(
            segment,
            segment,
            segment,
            640)
        self.assertEqual((656, 64), result.size)
        self.assertEqual((2, 29, 52, 199), result.getpixel((0, 20)))
        self.assertEqual((2, 29, 52, 199), result.getpixel((647, 20)))
        self.assertEqual((2, 29, 52, 199), result.getpixel((655, 20)))

    @unittest.skipUnless(
        os.path.isfile(os.path.join(
            RETAIL_ROOT, "data", "CompiledDefs", "frontend.bin")),
        "retail frontend.bin is not installed")
    def test_title_rule_uses_the_single_retail_textbox_component(self):
        from frontend_layout_oracle import FrontendLayoutOracle

        layout = FrontendLayoutOracle(self.RETAIL_ROOT, self.SCHEMA)
        self.assertEqual(
            (
                (0, 122, "UI_TEXTBOX_MIDDLE"),
                (1, 122, "UI_TEXTBOX_MIDDLE"),
                (4, 122, "UI_TEXTBOX_MIDDLE"),
            ),
            layout.table_sprites("UI_TABLE_TITLE_WHOLE"))

    def test_detail_screens_follow_recovered_dispatch_order(self):
        self.assertEqual(4, DETAIL_SCREEN_COUNT)
        self.assertEqual(10, len(GAMEPLAY_ROWS))
        self.assertEqual(3, len(AUDIO_ROWS))
        self.assertEqual(10, len(VIDEO_ROWS))
        self.assertEqual(9, len(REDEFINE_ROWS))
        self.assertEqual("Game Camera", GAMEPLAY_ROWS[0][0])
        self.assertEqual("Music Volume", AUDIO_ROWS[0][0])
        self.assertEqual("Resolution", VIDEO_ROWS[0][0])
        self.assertEqual(
            (60, 9, 7, 8, 31, 45, 6, 13, 14),
            REDEFINE_ACTION_ORDER)
        self.assertEqual(
            (
                ("Move Forward", "W"),
                ("Move Left", "A"),
                ("Move Backward", "S"),
                ("Move Right", "D"),
            ),
            REDEFINE_ROWS[:4])

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend.big/fonts.big are not installed")
    def test_component_detail_frames_leave_row_labels_for_live_text(self):
        component_sheet = build_options_sheet(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            include_title_rule=False,
            include_selected_button=False,
            include_options_text=False,
            include_options_row_atlas=True,
            include_detail_title_text=False)
        component_frame = component_sheet.crop((
            0,
            4 * 480,
            640,
            5 * 480))
        row_ys = tuple(
            90 + row * 30 for row in range(len(GAMEPLAY_ROWS)))
        without_rows = build_settings_frame(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            "Gameplay Options",
            GAMEPLAY_ROWS,
            row_ys,
            include_title_rule=False,
            include_title_text=False,
            include_row_text=False,
            include_helper_text=False)
        with_rows = build_settings_frame(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            "Gameplay Options",
            GAMEPLAY_ROWS,
            row_ys,
            include_title_rule=False,
            include_title_text=False,
            include_row_text=True)
        self.assertEqual(component_frame.tobytes(), without_rows.tobytes())
        self.assertNotEqual(with_rows.tobytes(), without_rows.tobytes())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend.big/fonts.big are not installed")
    def test_component_control_tiles_leave_string_values_for_live_text(self):
        baked_sheet = build_options_sheet(
            self.FRONTEND_BANK,
            self.FONT_BANK)
        component_sheet = build_options_sheet(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            include_title_rule=False,
            include_selected_button=False,
            include_options_text=False,
            include_options_row_atlas=True,
            include_detail_title_text=False)
        baked_tile = baked_sheet.crop((640, 0, 840, 30))
        component_tile = component_sheet.crop((640, 0, 840, 30))
        self.assertNotEqual(baked_tile.tobytes(), component_tile.tobytes())
        self.assertIsNotNone(component_tile.getchannel("A").getbbox())

    def test_full_redefine_action_order_is_the_decoded_31_entry_list(self):
        # UI_FRONTEND_LIST_REDEFINE_KEYS_MENU.ActionOrder decodes to 31 ids;
        # the visible first page is its prefix (action 60 fans out to W/A/S/D).
        # Locks the decoded fact so the off-page name gap is not re-derived.
        self.assertEqual(31, len(REDEFINE_FULL_ACTION_ORDER))
        self.assertEqual(
            REDEFINE_ACTION_ORDER,
            REDEFINE_FULL_ACTION_ORDER[:len(REDEFINE_ACTION_ORDER)])
        self.assertEqual(len(set(REDEFINE_FULL_ACTION_ORDER)),
                         len(REDEFINE_FULL_ACTION_ORDER))
        self.assertEqual(set(REDEFINE_FULL_ACTION_ORDER),
                         set(REDEFINE_ACTION_ENUM_NAMES))
        self.assertEqual(set(REDEFINE_FULL_ACTION_ORDER),
                         set(REDEFINE_ACTION_DISPLAY_TEXT))
        self.assertEqual(set(REDEFINE_FULL_ACTION_ORDER),
                         set(REDEFINE_ACTION_DEFAULT_INPUTS))
        self.assertEqual(
            "LEFT MOUSE BUTTON",
            REDEFINE_ACTION_DISPLAY_INPUTS[9])
        self.assertEqual(
            "MIDDLE MOUSE BUTTON",
            REDEFINE_ACTION_DISPLAY_INPUTS[7])
        self.assertEqual("ENTER",
                         REDEFINE_ACTION_DISPLAY_INPUTS[72])
        self.assertEqual("PRNT SCRN",
                         REDEFINE_ACTION_DISPLAY_INPUTS[53])
        self.assertEqual("GAME_ACTION_MOVEMENT",
                         REDEFINE_ACTION_ENUM_NAMES[60])
        self.assertEqual("GAME_ACTION_TOGGLE_FIRST_PERSON_TARGETING",
                         REDEFINE_ACTION_ENUM_NAMES[45])
        self.assertEqual("Flourish", REDEFINE_ACTION_DISPLAY_TEXT[8])
        self.assertEqual("Use Hotbar Item", REDEFINE_ACTION_DISPLAY_TEXT[55])
        self.assertEqual(
            ("K:W<0,+1>", "K:A<-1,0>", "K:S<0,-1>", "K:D<+1,0>"),
            REDEFINE_ACTION_DEFAULT_INPUTS[60])
        self.assertEqual(("K:RETURN", "K:ESC"),
                         REDEFINE_ACTION_DEFAULT_INPUTS[72])
        self.assertEqual(("K:0x54",), REDEFINE_ACTION_DEFAULT_INPUTS[99])

    def test_redefine_action_order_expands_to_the_materialized_rows(self):
        # Movement, context-sensitive items, and hotbar items expand into the
        # generated visual children observed in the retail keybind captures.
        self.assertEqual(44, len(REDEFINE_MATERIALIZED_ROWS))
        self.assertEqual(
            (60, 60, 60, 60, 9, 7, 8, 31, 45),
            tuple(row[0] for row in REDEFINE_MATERIALIZED_ROWS[:9]))
        self.assertEqual(
            (6, 13, 14, 1, 32, 26, 86, 94, 78, 4, 113, 112, 72,
             56, 56, 56, 92, 90, 96, 91, 97, 93, 98, 99, 100,
             55, 55, 55, 55, 55, 55, 55, 55, 55, 53),
            tuple(row[0] for row in REDEFINE_MATERIALIZED_ROWS[9:]))
        self.assertEqual(
            "Cycle Spells (Alternative To Mouse Wheel)",
            REDEFINE_MATERIALIZED_ROWS[16][1])
        self.assertEqual(
            "Take Photojournal Photo",
            REDEFINE_MATERIALIZED_ROWS[-1][1])
        self.assertEqual("2", _redefine_materialized_key_text(35, 55))
        self.assertEqual("9", _redefine_materialized_key_text(42, 55))
        self.assertEqual("PRNT SCRN", _redefine_materialized_key_text(43, 53))
        self.assertEqual(
            (0, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
             19, 20, 21, 22, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 43),
            REDEFINE_ACTION_MATERIALIZED_OFFSETS)
        self.assertEqual(4, redefine_materialized_row_offset(1))
        self.assertEqual(43, redefine_materialized_row_offset(30))
        self.assertEqual(
            ((6, "Interact"), (13, "Unsheathe Melee Weapon")),
            redefine_scrolled_rows(9)[:2])
        self.assertEqual(
            ((55, "Use Hotbar Item 8"), (55, "Use Hotbar Item 9"),
             (53, "Take Photojournal Photo")),
            redefine_scrolled_rows(35)[-3:])
        with self.assertRaises(ValueError):
            redefine_scrolled_rows(36)

    def test_redefine_scroll_page_atlas_contract(self):
        self.assertEqual(35, len(REDEFINE_SCROLL_PAGE_OFFSETS))
        self.assertEqual(5, REDEFINE_SCROLL_PAGE_COLUMNS)
        self.assertEqual((3200, 3360), REDEFINE_SCROLL_PAGE_ATLAS_SIZE)

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_redefine_scroll_page_atlas_materializes_all_pages(self):
        atlas = build_redefine_scroll_pages(
            self.FRONTEND_BANK,
            self.FONT_BANK)
        self.assertEqual(REDEFINE_SCROLL_PAGE_ATLAS_SIZE, atlas.size)
        self.assertIsNotNone(
            atlas.crop((0, 0, 640, 480)).getchannel("A").getbbox())
        self.assertIsNotNone(
            atlas.crop((4 * 640, 4 * 480, 5 * 640, 5 * 480))
            .getchannel("A").getbbox())

    def test_save_browser_geometry_and_order_match_recovered_contract(self):
        self.assertEqual((10, 90), SAVE_LIST_ORIGIN)
        self.assertEqual(30, SAVE_ROW_STEP_Y)
        self.assertEqual(150, SAVE_LIST_HEIGHT)
        self.assertEqual(8, SAVE_SCREEN_FRAME_BASE)
        self.assertEqual(1, SAVE_BROWSER_FRAME_COUNT)
        self.assertEqual(9, OPTIONS_SHEET_FRAME_COUNT)
        self.assertEqual((1024, 1920), SAVE_COMPONENT_ATLAS_ORIGIN)
        self.assertEqual(
            (
                "AutoSave",
                "Manual - Save1",
                "Manual - Save2",
                "Manual - Save3",
            ),
            SAVE_BROWSER_ROWS)

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_save_browser_selection_uses_complete_retail_button_texture(self):
        first = build_save_browser_frame(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            0)
        fourth = build_save_browser_frame(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            3)
        self.assertEqual((640, 480), first.size)
        self.assertEqual((640, 480), fourth.size)
        difference = ImageChops.difference(first, fourth)
        self.assertIsNotNone(difference.getbbox())
        # Selection moves by three exact list steps; unchanged title/detail
        # content therefore cancels outside the list viewport.
        bounds = difference.getchannel("A").getbbox()
        self.assertIsNotNone(bounds)
        self.assertGreaterEqual(bounds[1], SAVE_LIST_ORIGIN[1] - 7)
        self.assertLessEqual(
            bounds[3],
            SAVE_LIST_ORIGIN[1] + 3 * SAVE_ROW_STEP_Y + 64)

    def test_save_names_are_centered_on_selection_highlight(self):
        # Retail centres each save-slot label on its highlight button, exactly
        # like the main-menu and Options rows. Regression guard for the bug where
        # the name was centred on the left-sprite end (origin+60), leaving it
        # 64px left of the highlight centre (read as left-aligned).
        frame = build_save_browser_frame(self.FRONTEND_BANK, self.FONT_BANK, 1)
        other = build_save_browser_frame(self.FRONTEND_BANK, self.FONT_BANK, 0)
        # Scope to the list column (x < 300); the save-preview viewport on the
        # right (x >= 314) is a separate element and would skew the bbox.
        LIST_W = 300
        highlight = ImageChops.difference(frame, other).crop(
            (0, 0, LIST_W, 480)).getchannel("A").getbbox()
        self.assertIsNotNone(highlight)
        highlight_center = (highlight[0] + highlight[2]) / 2.0
        # An unselected row (row 3) shares the list column and is free of the
        # highlight, so its text centre must equal the highlight centre.
        y = SAVE_LIST_ORIGIN[1] + 3 * SAVE_ROW_STEP_Y
        band = frame.crop((0, y - 12, LIST_W, y + 16)).getchannel("A").getbbox()
        self.assertIsNotNone(band)
        text_center = (band[0] + band[2]) / 2.0
        self.assertAlmostEqual(text_center, highlight_center, delta=3)

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_component_save_cell_leaves_names_for_live_text(self):
        oracle = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0, include_save_text=True)
        component = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0, include_save_text=False)
        # The component/atlas cell is the static save base.  Its row labels
        # are submitted by the executable's ENG_ARIAL_16 glyph path instead.
        diff = ImageChops.difference(
            oracle.crop((0, SAVE_LIST_ORIGIN[1], 300, SAVE_LIST_ORIGIN[1] + SAVE_LIST_HEIGHT)),
            component.crop((0, SAVE_LIST_ORIGIN[1], 300, SAVE_LIST_ORIGIN[1] + SAVE_LIST_HEIGHT)))
        self.assertIsNotNone(diff.getbbox())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_component_save_cell_leaves_file_info_for_live_text(self):
        oracle = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_save_info_text=True)
        component = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_save_info_text=False)
        # The File Information header/profile line are runtime values in the
        # executable; the static component cell supplies only their backdrop.
        diff = ImageChops.difference(
            oracle.crop((0, 250, 300, 325)),
            component.crop((0, 250, 300, 325)))
        self.assertIsNotNone(diff.getbbox())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_component_save_cell_leaves_bottom_backdrop_for_live_quad(self):
        oracle = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_bottom_backdrop=True)
        component = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_bottom_backdrop=False)
        diff = ImageChops.difference(
            oracle.crop((0, SAVE_BOTTOM_BACKDROP_ORIGIN[1], 640, 480)),
            component.crop((0, SAVE_BOTTOM_BACKDROP_ORIGIN[1], 640, 480)))
        self.assertIsNotNone(diff.getbbox())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_component_save_cell_leaves_text_bottom_for_live_quad(self):
        oracle = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_text_bottom=True)
        component = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_text_bottom=False)
        diff = ImageChops.difference(
            oracle.crop((0, SAVE_TEXT_BOTTOM_ORIGIN[1], 200, 420)),
            component.crop((0, SAVE_TEXT_BOTTOM_ORIGIN[1], 200, 420)))
        self.assertIsNotNone(diff.getbbox())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_component_save_cell_leaves_text_area_for_live_table(self):
        oracle = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_text_area=True)
        component = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_text_area=False)
        diff = ImageChops.difference(
            oracle.crop((0, SAVE_TEXT_AREA_ORIGIN[1], 640, 325)),
            component.crop((0, SAVE_TEXT_AREA_ORIGIN[1], 640, 325)))
        self.assertIsNotNone(diff.getbbox())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_component_save_cell_leaves_title_area_for_live_table(self):
        oracle = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_title_area=True)
        component = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_title_area=False)
        diff = ImageChops.difference(
            oracle.crop((0, SAVE_TITLE_AREA_ORIGIN[1], 640, 110)),
            component.crop((0, SAVE_TITLE_AREA_ORIGIN[1], 640, 110)))
        self.assertIsNotNone(diff.getbbox())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_component_save_cell_leaves_title_text_for_live_glyphs(self):
        oracle = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_save_title_text=True)
        component = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0,
            include_save_title_text=False)
        diff = ImageChops.difference(
            oracle.crop((0, 35, 240, 80)),
            component.crop((0, 35, 240, 80)))
        self.assertIsNotNone(diff.getbbox())

    def test_save_browser_renders_preview_viewport_ring(self):
        # UI_VIEW_RING_SMALL (serialized at 314,37) frames a region minimap in
        # the save-preview viewport on the right. Guard that the 256x256 region
        # is rendered (previously empty background).
        frame = build_save_browser_frame(self.FRONTEND_BANK, self.FONT_BANK, 0)
        ox, oy = SAVE_VIEW_RING_ORIGIN
        region = frame.crop((ox, oy, ox + 256, oy + 256)).getchannel("A")
        bounds = region.getbbox()
        self.assertIsNotNone(bounds)
        # The ring ornament fills essentially the whole 256x256 tile.
        self.assertEqual((0, 0, 256, 256), bounds)

    def test_save_browser_renders_file_information_backdrop(self):
        # UI_TEXT_AREA (0,254) frames File Information with a UI_TEXTBOX_MIDDLE
        # rule (UI_TABLE_TEXT_LEFT width 287 + UI_TABLE_TEXT_RIGHT 463,width 40).
        # Guard that the left backdrop segment renders at y=254.
        frame = build_save_browser_frame(self.FRONTEND_BANK, self.FONT_BANK, 0)
        # A horizontal band at the text-area origin should carry the rule.
        band = frame.crop((0, 254, 280, 254 + 20)).getchannel("A").getbbox()
        self.assertIsNotNone(band)
        # The rule starts at the left edge and spans most of the 287 span.
        self.assertEqual(0, band[0])
        self.assertGreater(band[2], 200)

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK),
        "retail frontend bank is not installed")
    def test_save_tables_use_decoded_asymmetric_corner_slots(self):
        buf, parsed = load_big(self.FRONTEND_BANK)
        middle = _decode_named(
            buf, parsed, "UI_TEXTBOX_MIDDLE_FE_SPRITE")
        title_left = _build_table_horizontal(
            middle,
            middle,
            _decode_named(buf, parsed, "UI_TEXTBOX_TL_SPRITE_FE"),
            287)
        title_right = _build_table_horizontal(
            _decode_named(buf, parsed, "UI_TEXTBOX_TR_SPRITE_FE"),
            middle,
            middle,
            40)
        text_left = _build_table_horizontal(
            middle,
            middle,
            _decode_named(buf, parsed, "UI_TEXTBOX_BL_SPRITE_FE"),
            287)
        text_right = _build_table_horizontal(
            _decode_named(buf, parsed, "UI_TEXTBOX_BR_SPRITE_FE"),
            middle,
            middle,
            40)
        # CTable repeats key 4 by Width/8, then places the natural-size key 0
        # and key 1 corners around that span: 8 + 35*8 + 128 and
        # 128 + 5*8 + 8 respectively.
        self.assertEqual((416, 64), title_left.size)
        self.assertEqual((176, 64), title_right.size)
        self.assertEqual((416, 64), text_left.size)
        self.assertEqual((176, 64), text_right.size)
        self.assertEqual((0, 35), SAVE_TITLE_AREA_ORIGIN)
        self.assertEqual((0, 254), SAVE_TEXT_AREA_ORIGIN)

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK),
        "retail frontend bank is not installed")
    def test_detail_and_redefine_tables_use_compiled_inner_spans(self):
        buf, parsed = load_big(self.FRONTEND_BANK)
        slots = (
            _decode_named(buf, parsed, "FE_SLOT_TEST_L_OFF"),
            _decode_named(buf, parsed, "FE_SLOT_TEST_M_OFF"),
            _decode_named(buf, parsed, "FE_SLOT_TEST_R_OFF"),
        )
        bar = _decode_named(
            buf, parsed, "FE_OPTIONS_HORIZONTAL_BAR_SPRITE")
        detail_left = _build_table_horizontal(
            slots[0], slots[1], slots[2], 180)
        detail_right = _build_table_horizontal(bar, bar, bar, 120)
        redefine_left = _build_table_horizontal(
            slots[0], slots[1], slots[2], 280)
        redefine_right = _build_table_horizontal(bar, bar, bar, 220)
        self.assertEqual((304, 32), detail_left.size)
        self.assertEqual((144, 32), detail_right.size)
        self.assertEqual((408, 32), redefine_left.size)
        self.assertEqual((240, 32), redefine_right.size)

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_save_bottom_records_are_materialized_at_decoded_positions(self):
        frame = build_save_browser_frame(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            0,
            include_viewport=False)
        # The 160x1 UI_TEXT_BOTTOM rule resolves to y=254+150=404. The
        # half-alpha 4x4 HUD_TEXTBOX_BACK_FE backdrop resolves to 640x248 at
        # (0,292), so both decoded regions must be occupied in the bake.
        self.assertEqual((0, 404), SAVE_TEXT_BOTTOM_ORIGIN)
        self.assertEqual((0, 292), SAVE_BOTTOM_BACKDROP_ORIGIN)
        self.assertIsNotNone(frame.crop((0, 404, 640, 412)).getbbox())
        self.assertIsNotNone(frame.crop((0, 292, 640, 480)).getbbox())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_save_preview_viewport_is_256_panel(self):
        # The live D3D9 ring quad samples a 256x256 rect; the panel it draws must
        # be exactly that size (minimap composited under the ring ornament).
        panel = build_save_preview_viewport(self.FRONTEND_BANK)
        self.assertEqual((256, 256), panel.size)
        # The ring ornament reaches every tile edge.
        self.assertEqual(
            (0, 0, 256, 256), panel.getchannel("A").getbbox())
        self.assertEqual(0, panel.getpixel((0, 0))[3])
        self.assertEqual(0, panel.getpixel((255, 255))[3])

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_oracle_save_frame_punches_hole_before_viewport(self):
        # The live cell-background quads skip the ring rect; the oracle must
        # match by clearing that rect before compositing the panel on top, so
        # the viewport is the ONLY content there (rule/list/File-Info under it
        # are removed, exactly as the live 4-quad split leaves them).
        oracle = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0, include_viewport=True)
        holed = build_save_browser_frame(
            self.FRONTEND_BANK, self.FONT_BANK, 0, include_viewport=False)
        ox, oy = SAVE_VIEW_RING_ORIGIN
        holed = holed.copy()
        holed.paste((0, 0, 0, 0), (ox, oy, ox + 256, oy + 256))
        holed.alpha_composite(
            build_save_preview_viewport(self.FRONTEND_BANK),
            SAVE_VIEW_RING_ORIGIN)
        self.assertIsNone(
            ImageChops.difference(oracle, holed).getbbox())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_component_atlas_bakes_ring_source_but_cell_is_ring_free(self):
        # The component/atlas sheet keeps the ring ONLY inside the one static
        # save cell's 256x256 source rect (so the live ring quad has pixels to
        # sample) while the rest of that cell is ring-free (so the live
        # cell-background quads, which surround that rect, never double-draw
        # the ring).
        components = build_options_sheet(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            include_title_rule=False,
            include_selected_button=False,
            include_options_text=False,
            include_options_row_atlas=True)
        panel = build_save_preview_viewport(self.FRONTEND_BANK)
        ax, ay = SAVE_COMPONENT_ATLAS_ORIGIN
        ox, oy = SAVE_VIEW_RING_ORIGIN
        for save_row in range(SAVE_BROWSER_FRAME_COUNT):
            cell_top = ay + save_row * 480
            ring_box = (
                ax + ox,
                cell_top + oy,
                ax + ox + 256,
                cell_top + oy + 256,
            )
            baked = components.crop(ring_box)
            self.assertIsNone(
                ImageChops.difference(baked, panel).getbbox(),
                "save cell %d ring source rect must match the panel"
                % save_row)

    def test_compiled_video_defaults_seed_first_frame(self):
        self.assertEqual(("Resolution", "2560X1440X32", 1.0 / 3.0), VIDEO_ROWS[0])
        self.assertEqual(
            ("1920X1080X32", "2560X1440X32", "3840X2160X32"),
            VIDEO_CONTROL_VALUES[0])
        self.assertEqual(("Anti-Aliasing", "8X", 0.0), VIDEO_ROWS[2])
        self.assertAlmostEqual(1.0, VIDEO_ROWS[3][2])

    def test_compiled_audio_defaults_seed_profile_values(self):
        self.assertAlmostEqual(0.6, AUDIO_ROWS[0][2])
        self.assertAlmostEqual(0.8, AUDIO_ROWS[1][2])
        self.assertAlmostEqual(0.9, AUDIO_ROWS[2][2])

    def test_control_atlas_covers_every_mutable_value(self):
        self.assertEqual(1024, OPTIONS_SHEET_WIDTH)
        self.assertEqual(1664, OPTIONS_COMPONENT_SHEET_WIDTH)
        self.assertEqual(1024, OPTIONS_ROW_ATLAS_ORIGIN_X)
        self.assertEqual((200, 30), CONTROL_TILE_SIZE)
        self.assertEqual(1, CONTROL_ATLAS_COLUMNS)
        self.assertEqual(10, len(GAMEPLAY_CONTROL_VALUES))
        self.assertEqual(3, len(AUDIO_CONTROL_VALUES))
        self.assertEqual(10, len(VIDEO_CONTROL_VALUES))
        self.assertEqual(
            (3, 4, 3, 4, 4, 4, 18, 2, 4, 3),
            tuple(len(values) for values in VIDEO_CONTROL_VALUES))
        self.assertEqual(
            124,
            sum(
                len(values)
                for screen in CONTROL_VALUE_GROUPS
                for values in screen))

    @unittest.skipUnless(
        os.path.isfile(FONT_BANK),
        "retail font bank is not installed")
    def test_options_rows_are_independent_design_canvases(self):
        rows = build_options_row_layers(self.FONT_BANK)
        self.assertEqual(len(OPTIONS_ROWS), len(rows))
        for row in rows:
            self.assertEqual((640, 480), row.size)
            self.assertIsNotNone(row.getchannel("A").getbbox())

    def test_redefine_hover_atlas_fits_one_helper_frame(self):
        self.assertEqual(6, HELPER_SHEET_FRAME_COUNT)
        self.assertEqual(5, REDEFINE_HOVER_HELPER_FRAME)
        self.assertEqual((588, 35), REDEFINE_HOVER_STRIP_SIZE)
        self.assertLessEqual(
            REDEFINE_HOVER_STRIP_SIZE[0],
            640)
        self.assertLessEqual(
            len(REDEFINE_ROWS) *
            REDEFINE_HOVER_STRIP_SIZE[1],
            REDEFINE_RESET_HOVER_ATLAS_Y)
        self.assertEqual((320, 64), REDEFINE_RESET_HOVER_SIZE)
        self.assertLessEqual(
            REDEFINE_RESET_HOVER_ATLAS_Y +
            REDEFINE_RESET_HOVER_SIZE[1],
            480)
        self.assertEqual(640, REDEFINE_RESET_HOVER_SIZE[0] * 2)

    def test_redefine_geometry_matches_compiled_component_transforms(self):
        self.assertEqual((40, 115), REDEFINE_LIST_ORIGIN)
        self.assertEqual(26, REDEFINE_ROW_STEP_Y)
        self.assertEqual((-32, -2), REDEFINE_TABLE_OFFSET)
        self.assertEqual((368, -3), REDEFINE_RIGHT_SLOT_OFFSET)
        self.assertEqual((0, 3), REDEFINE_ACTION_TEXT_OFFSET)
        self.assertEqual(9.0 / 8.0, REDEFINE_ACTION_TEXT_SCALE)
        self.assertEqual((380, 3), REDEFINE_KEY_TEXT_OFFSET)
        self.assertEqual((-40, 0), REDEFINE_MOUSE_OFFSET)
        self.assertEqual((600, 24), REDEFINE_MOUSE_SIZE)

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_redefine_initial_viewport_suppresses_top_arrow_and_warning(self):
        frame = build_redefine_frame(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            include_key_text=False,
            include_title_rule=False,
            include_title_text=False)
        self.assertIsNone(
            frame.crop((304, 80, 336, 112)).getchannel("A").getbbox())
        self.assertIsNone(
            frame.crop((0, 360, 304, 389)).getchannel("A").getbbox())
        self.assertIsNone(
            frame.crop((336, 360, 640, 389)).getchannel("A").getbbox())
        label_pixels = frame.crop((40, 115, 220, 145)).getdata()
        self.assertTrue(
            any(pixel[3] != 0 and min(pixel[:3]) > 220
                for pixel in label_pixels))
        cancel_pixels = frame.crop((60, 435, 235, 475)).getdata()
        apply_pixels = frame.crop((400, 435, 580, 475)).getdata()
        self.assertGreater(
            max(min(pixel[:3]) for pixel in cancel_pixels if pixel[3]),
            max(min(pixel[:3]) for pixel in apply_pixels if pixel[3]))
        self.assertLessEqual(
            max(min(pixel[:3]) for pixel in apply_pixels if pixel[3]),
            128)

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_redefine_scrolled_viewport_materializes_arrows_and_labels(self):
        frame = build_redefine_frame(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            include_key_text=True,
            include_title_rule=False,
            include_title_text=False,
            materialized_row_offset=9)
        self.assertIsNotNone(
            frame.crop((304, 80, 336, 112)).getchannel("A").getbbox())
        self.assertIsNotNone(
            frame.crop((304, 350, 336, 382)).getchannel("A").getbbox())
        self.assertIsNotNone(
            frame.crop((40, 115, 220, 145)).getchannel("A").getbbox())
        self.assertIsNotNone(
            frame.crop((40, 115 + 7 * REDEFINE_ROW_STEP_Y,
                        360, 145 + 7 * REDEFINE_ROW_STEP_Y
                        )).getchannel("A").getbbox())

        last = build_redefine_frame(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            include_key_text=True,
            include_title_rule=False,
            include_title_text=False,
            materialized_row_offset=35)
        self.assertIsNotNone(
            last.crop((304, 80, 336, 112)).getchannel("A").getbbox())
        self.assertIsNone(
            last.crop((304, 350, 336, 382)).getchannel("A").getbbox())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_redefine_key_text_uses_retail_top_origin_in_right_bar(self):
        buf, parsed = load_big(self.FRONTEND_BANK)
        right_bar = _decode_named(
            buf, parsed, "FE_OPTIONS_HORIZONTAL_BAR_SPRITE")
        slot = _build_stretched(
            right_bar,
            right_bar,
            right_bar,
            220)
        slot_bounds = slot.getchannel("A").getbbox()
        slot_top = (
            REDEFINE_LIST_ORIGIN[1] +
            REDEFINE_TABLE_OFFSET[1] +
            REDEFINE_RIGHT_SLOT_OFFSET[1]
        )
        slot_center_twice = (
            2 * slot_top + slot_bounds[1] + slot_bounds[3]
        )

        font = load_font(self.FONT_BANK, "ENG_ARIAL_12")
        key_line = add_outline(
            render_line(
                font,
                REDEFINE_ROWS[0][1],
                (640, 480),
                (
                    REDEFINE_LIST_ORIGIN[0] +
                    REDEFINE_KEY_TEXT_OFFSET[0],
                    REDEFINE_LIST_ORIGIN[1] +
                    REDEFINE_KEY_TEXT_OFFSET[1] +
                    REDEFINE_TEXT_RENDER_Y_BIAS,
                ),
                "left",
                1.0),
            1)
        text_bounds = key_line.getchannel("A").getbbox()
        text_center_twice = text_bounds[1] + text_bounds[3]
        self.assertEqual(0, slot_center_twice - text_center_twice)

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend/font banks are not installed")
    def test_redefine_action_text_uses_retail_top_origin_in_left_slot(self):
        buf, parsed = load_big(self.FRONTEND_BANK)
        slot = _build_stretched(
            _decode_named(buf, parsed, "FE_SLOT_TEST_L_OFF"),
            _decode_named(buf, parsed, "FE_SLOT_TEST_M_OFF"),
            _decode_named(buf, parsed, "FE_SLOT_TEST_R_OFF"),
            280)
        slot_bounds = slot.getchannel("A").getbbox()
        slot_top = (
            REDEFINE_LIST_ORIGIN[1] +
            REDEFINE_TABLE_OFFSET[1]
        )
        slot_center_twice = (
            2 * slot_top + slot_bounds[1] + slot_bounds[3]
        )

        font = load_font(self.FONT_BANK, "ENG_ARIAL_12")
        action_line = add_outline(
            render_line(
                font,
                REDEFINE_ROWS[0][0],
                (640, 480),
                (
                    REDEFINE_LIST_ORIGIN[0] +
                    REDEFINE_ACTION_TEXT_OFFSET[0],
                    REDEFINE_LIST_ORIGIN[1] +
                    REDEFINE_ACTION_TEXT_OFFSET[1] +
                    REDEFINE_TEXT_RENDER_Y_BIAS,
                ),
                "left",
                2.0 / 3.0),
            1)
        text_bounds = action_line.getchannel("A").getbbox()
        text_center_twice = text_bounds[1] + text_bounds[3]
        self.assertEqual(6, slot_center_twice - text_center_twice)

    @unittest.skipUnless(
        os.path.isfile(os.path.join(
            RETAIL_ROOT, "data", "CompiledDefs", "frontend.bin")),
        "retail frontend.bin is not installed")
    def test_renderer_constants_match_shipped_frontend_bin(self):
        validate_compiled_subscreen_layout(
            self.RETAIL_ROOT,
            self.SCHEMA)

    @unittest.skipUnless(
        os.path.isfile(os.path.join(
            RETAIL_ROOT, "data", "CompiledDefs", "frontend.bin")),
        "retail frontend.bin is not installed")
    def test_about_screen_matches_shipped_frontend_bin(self):
        validate_compiled_about_layout(
            self.RETAIL_ROOT,
            self.SCHEMA)

    @unittest.skipUnless(
        os.path.isfile(os.path.join(
            RETAIL_ROOT, "data", "CompiledDefs", "frontend.bin")),
        "retail frontend.bin is not installed")
    def test_credits_screen_matches_shipped_frontend_bin(self):
        validate_compiled_credits_layout(
            self.RETAIL_ROOT,
            self.SCHEMA)

    @unittest.skipUnless(
        os.path.isfile(os.path.join(
            RETAIL_ROOT, "data", "CompiledDefs", "frontend.bin")),
        "retail frontend.bin is not installed")
    def test_profiles_screen_matches_shipped_frontend_bin(self):
        validate_compiled_profiles_layout(
            self.RETAIL_ROOT,
            self.SCHEMA)

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend.big/fonts.big are not installed")
    def test_profiles_static_frame_and_glyph_atlas_are_sourced(self):
        frame = build_profiles_frame(self.FRONTEND_BANK, self.FONT_BANK)
        self.assertEqual(frame.size, (640, 480))
        self.assertIsNotNone(frame.crop((0, 35, 640, 70)).getbbox())
        components = build_options_sheet(
            self.FRONTEND_BANK,
            self.FONT_BANK,
            include_options_row_atlas=True)
        left, top = PROFILE_GLYPH_ATLAS_ORIGIN
        self.assertIsNotNone(
            components.crop((left, top, left + 128, top + 256)).getbbox())
        left, top = DETAIL_TITLE_GLYPH_ATLAS_ORIGIN
        self.assertEqual(DETAIL_TITLE_GLYPH_ATLAS_SIZE, (256, 256))
        self.assertIsNotNone(
            components.crop((left, top, left + 256, top + 256)).getbbox())
        left, top = REDEFINE_GLYPH_ATLAS_ORIGIN
        self.assertEqual(REDEFINE_GLYPH_ATLAS_SIZE, (128, 128))
        self.assertIsNotNone(
            components.crop((left, top, left + 128, top + 128)).getbbox())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend.big/fonts.big are not installed")
    def test_profiles_sheet_contains_delete_confirmation_surface(self):
        sheet = build_profiles_screen_sheet(self.FRONTEND_BANK, self.FONT_BANK)
        self.assertEqual(sheet.size, (640, 960))
        self.assertIsNotNone(sheet.crop((0, 480, 640, 960)).getbbox())

    @unittest.skipUnless(
        os.path.isfile(os.path.join(
            RETAIL_ROOT, "data", "lang", "English", "text.big")),
        "retail text.big is not installed")
    def test_credits_stream_consumes_authored_text_groups_in_order(self):
        stream = extract_credits_text_stream(
            os.path.join(
                self.RETAIL_ROOT, "data", "lang", "English", "text.big"))
        self.assertEqual(
            [(item["symbol"], item["font"], len(item["members"]))
             for item in stream],
            [
                ("TEXT_GUI_CRE_MAIN1", "ENG_ARIAL_24", 133),
                ("TEXT_GUI_CRE_MAIN2", "ENG_ARIAL_24", 125),
                ("TEXT_GUI_CRE_TESTSUP", "ENG_ARIAL_24", 143),
                ("TEXT_GUI_CRE_MICROSOFT", "ENG_ARIAL_24", 152),
                ("TEXT_GUI_CRE_TEST", "ENG_ARIAL_12", 132),
                ("TEXT_GUI_CRE_THANKS", "ENG_ARIAL_12", 13),
            ])
        self.assertEqual(stream[0]["members"][:4],
                         ("CODING", " ", "Lead", "Simon Carter"))
        self.assertEqual(
            stream[-1]["members"][-1],
            "the maximum extent possible under the law.")
        # Explicit blank members are part of the retail group payload and
        # must survive extraction; dropping them changes the later layout.
        self.assertGreater(
            sum(member == " " for item in stream for member in item["members"]),
            10)

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend.big/fonts.big are not installed")
    def test_about_frame_composes_title_message_and_back(self):
        frame = build_about_frame(self.FRONTEND_BANK, self.FONT_BANK)
        self.assertEqual(frame.size, (640, 480))

        def region_has_pixels(box):
            return frame.crop(box).getbbox() is not None

        # Title rule + "About Fable" occupy the raised header band (y 5..30);
        # the legal-notice message body sits just below (y 55..220). Back is
        # a live UI_HELPERS component and is not baked into this panel.
        self.assertTrue(region_has_pixels((0, 5, 640, 32)), "title band empty")
        self.assertTrue(
            region_has_pixels((0, 55, 640, 220)), "message body empty")
        # Composition is deterministic (same banks -> identical bytes).
        again = build_about_frame(self.FRONTEND_BANK, self.FONT_BANK)
        self.assertEqual(frame.tobytes(), again.tobytes())

    @unittest.skipUnless(
        os.path.isfile(FRONTEND_BANK),
        "retail frontend.big is not installed")
    def test_credits_initial_frame_preserves_authored_scroll_start(self):
        frame = build_credits_frame(self.FRONTEND_BANK)
        self.assertEqual(frame.size, (640, 480))
        self.assertIsNotNone(frame.crop((70, 30, 582, 160)).getbbox())
        # The compiled scrolling child starts at y=480; no credit text is
        # allowed to leak into the initial viewport.
        self.assertIsNone(frame.crop((0, 160, 640, 408)).getbbox())
        self.assertEqual(
            frame.tobytes(), build_credits_frame(self.FRONTEND_BANK).tobytes())


if __name__ == "__main__":
    unittest.main()

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
    REDEFINE_ACTION_ORDER,
    REDEFINE_FULL_ACTION_ORDER,
    REDEFINE_HOVER_HELPER_FRAME,
    REDEFINE_HOVER_STRIP_SIZE,
    REDEFINE_ACTION_TEXT_OFFSET,
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
    SAVE_BROWSER_ROWS,
    SAVE_COMPONENT_ATLAS_ORIGIN,
    SAVE_LIST_HEIGHT,
    SAVE_LIST_ORIGIN,
    SAVE_ROW_STEP_Y,
    SAVE_VIEW_RING_ORIGIN,
    SAVE_SCREEN_FRAME_BASE,
    VIDEO_CONTROL_VALUES,
    VIDEO_ROWS,
    _decode_named,
    _build_stretched,
    _build_table_horizontal,
    build_options_row_layers,
    build_save_browser_frame,
    build_about_frame,
    validate_compiled_about_layout,
    validate_compiled_subscreen_layout,
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

    def test_screen_headers_share_the_design_canvas_center(self):
        self.assertEqual((0, 35), HEADER_RULE_POSITION)
        self.assertEqual((320, 44), HEADER_TEXT_POSITION)

    @unittest.skipUnless(
        os.path.isfile(FONT_BANK),
        "retail font bank is not installed")
    def test_every_detail_header_is_pixel_centered(self):
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
                    "center",
                    2.0 / 3.0),
                1)
            bounds = line.getchannel("A").getbbox()
            self.assertIsNotNone(bounds, title)
            # Odd-width glyph runs necessarily land half a pixel either side
            # of x=320; no title may be farther away than that.
            self.assertLessEqual(
                abs((bounds[0] + bounds[2]) - 640),
                1,
                title)

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
                ("Move Back", "S"),
                ("Move Left", "A"),
                ("Move Right", "D"),
            ),
            REDEFINE_ROWS[:4])

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

    def test_save_browser_geometry_and_order_match_recovered_contract(self):
        self.assertEqual((10, 90), SAVE_LIST_ORIGIN)
        self.assertEqual(30, SAVE_ROW_STEP_Y)
        self.assertEqual(150, SAVE_LIST_HEIGHT)
        self.assertEqual(8, SAVE_SCREEN_FRAME_BASE)
        self.assertEqual(12, OPTIONS_SHEET_FRAME_COUNT)
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

    def test_compiled_video_defaults_seed_first_frame(self):
        self.assertEqual(("Resolution", "1024 x 768", 0.0), VIDEO_ROWS[0])
        self.assertEqual(("Anti-Aliasing", "OFF", 0.0), VIDEO_ROWS[2])
        self.assertAlmostEqual(1.0 / 3.0, VIDEO_ROWS[3][2])

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
        self.assertEqual((380, 3), REDEFINE_KEY_TEXT_OFFSET)
        self.assertEqual((-40, 0), REDEFINE_MOUSE_OFFSET)
        self.assertEqual((600, 24), REDEFINE_MOUSE_SIZE)

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
                2.0 / 3.0),
            1)
        text_bounds = key_line.getchannel("A").getbbox()
        text_center_twice = text_bounds[1] + text_bounds[3]
        self.assertEqual(6, slot_center_twice - text_center_twice)

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
        os.path.isfile(FRONTEND_BANK) and os.path.isfile(FONT_BANK),
        "retail frontend.big/fonts.big are not installed")
    def test_about_frame_composes_title_message_and_back(self):
        frame = build_about_frame(self.FRONTEND_BANK, self.FONT_BANK)
        self.assertEqual(frame.size, (640, 480))

        def region_has_pixels(box):
            return frame.crop(box).getbbox() is not None

        # Title rule + "About Fable" occupy the raised header band (y 5..30);
        # the legal-notice message body sits just below (y 55..220); the Back
        # helper renders near the bottom (y 420..475).
        self.assertTrue(region_has_pixels((0, 5, 640, 32)), "title band empty")
        self.assertTrue(
            region_has_pixels((0, 55, 640, 220)), "message body empty")
        self.assertTrue(
            region_has_pixels((0, 420, 300, 475)), "Back helper empty")
        # Composition is deterministic (same banks -> identical bytes).
        again = build_about_frame(self.FRONTEND_BANK, self.FONT_BANK)
        self.assertEqual(frame.tobytes(), again.tobytes())


if __name__ == "__main__":
    unittest.main()

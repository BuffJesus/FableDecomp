import os
import sys
import unittest

from PIL import Image


TOOLS_DIR = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "..", ".."))
if TOOLS_DIR not in sys.path:
    sys.path.insert(0, TOOLS_DIR)

from render_fable_frontend_menu import (  # noqa: E402
    BUTTON_TABLE_X_BY_WIDTH,
    BUFF_JESUS_MENU_ROWS,
    GENERATED_BUTTON_CHILD_X,
    MENU_CONTENT_CENTER_X,
    MENU_ROWS,
    build_selected_button,
    menu_rows_for_variant,
    selected_button_left,
    validate_compiled_menu_layout,
)


class FrontendMenuRenderTests(unittest.TestCase):
    RETAIL_ROOT = (
        r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters")
    SCHEMA = os.path.join(
        TOOLS_DIR, "..", "ghidra_out", "def_schema.json")

    def test_selected_button_tiles_middle_to_compiled_width(self):
        left = Image.new("RGBA", (64, 64), (1, 0, 0, 255))
        middle = Image.new("RGBA", (8, 64), (2, 0, 0, 255))
        right = Image.new("RGBA", (64, 64), (3, 0, 0, 255))
        selected = build_selected_button(left, middle, right, 400)
        self.assertEqual((400, 64), selected.size)
        self.assertEqual((1, 0, 0, 255), selected.getpixel((0, 0)))
        self.assertEqual((2, 0, 0, 255), selected.getpixel((200, 0)))
        self.assertEqual((3, 0, 0, 255), selected.getpixel((399, 0)))

    def test_compiled_menu_contains_seven_selectable_rows(self):
        self.assertEqual(7, len(MENU_ROWS))
        self.assertEqual(("Continue Game", 0), MENU_ROWS[0])
        self.assertEqual(("Quit", 240), MENU_ROWS[-1])

    def test_materialized_ornaments_share_center_with_text(self):
        self.assertEqual(320, MENU_CONTENT_CENTER_X)
        self.assertEqual({400: -140, 280: -80}, BUTTON_TABLE_X_BY_WIDTH)
        self.assertEqual(60, GENERATED_BUTTON_CHILD_X)
        self.assertEqual(180, selected_button_left(280))
        self.assertEqual(120, selected_button_left(400))
        for width in (280, 400):
            self.assertEqual(
                MENU_CONTENT_CENTER_X,
                selected_button_left(width) + width // 2)

    def test_buff_jesus_variant_preserves_retail_layout(self):
        self.assertIs(MENU_ROWS, menu_rows_for_variant("retail"))
        self.assertIs(
            BUFF_JESUS_MENU_ROWS,
            menu_rows_for_variant("buff-jesus"))
        self.assertEqual(
            tuple(offset for _, offset in MENU_ROWS),
            tuple(offset for _, offset in BUFF_JESUS_MENU_ROWS))
        self.assertIn("BuffJesus", " ".join(
            text for text, _ in BUFF_JESUS_MENU_ROWS))
        self.assertEqual(("Continue Game", 0), MENU_ROWS[0])

    @unittest.skipUnless(
        os.path.isfile(os.path.join(
            RETAIL_ROOT, "data", "CompiledDefs", "frontend.bin")),
        "retail frontend.bin is not installed")
    def test_renderer_constants_match_shipped_frontend_bin(self):
        validate_compiled_menu_layout(self.RETAIL_ROOT, self.SCHEMA)


if __name__ == "__main__":
    unittest.main()

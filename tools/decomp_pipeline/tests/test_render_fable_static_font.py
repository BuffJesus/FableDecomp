import os
import sys
import unittest


TOOLS_DIR = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "..", ".."))
if TOOLS_DIR not in sys.path:
    sys.path.insert(0, TOOLS_DIR)

from render_fable_static_font import text_advance  # noqa: E402


class StaticFontRenderTests(unittest.TestCase):
    def test_text_advance_uses_retail_metric(self):
        glyphs = [(0, 0, 0, 0, 0, 0, advance)
                  for advance in (3, 5, 7)]
        font = {
            "min_char": ord("A"),
            "max_char": ord("C"),
            "glyphs": glyphs,
        }
        self.assertEqual(15, text_advance(font, "ABC"))
        self.assertEqual(10, text_advance(font, "A?C"))
        self.assertEqual(10, text_advance(font, "ABC", 2.0 / 3.0))


if __name__ == "__main__":
    unittest.main()

import os
import sys
import unittest


TOOLS_DIR = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "..", ".."))
if TOOLS_DIR not in sys.path:
    sys.path.insert(0, TOOLS_DIR)

from render_fable_streaming_font import (  # noqa: E402
    glyph_metric,
    metric_advance,
    text_advance,
    wrap_text,
)


def synthetic_font():
    banks = [{"first": 64, "indices": ()} for _ in range(1024)]
    banks[0] = {
        "first": ord(" "),
        "indices": (1,),
    }
    banks[ord("A") // 64] = {
        "first": ord("A") % 64,
        "indices": (2, 3),
    }
    banks[ord("\u00a9") // 64] = {
        "first": ord("\u00a9") % 64,
        "indices": (4,),
    }
    return {
        "glyph_banks": banks,
        "metrics": [
            (0, 0, 0, 0, 3 << 23),
            (0, 0, 0, 0, 5 << 23),
            (0, 0, 0, 0, 7 << 23),
            (0, 0, 0, 0, 11 << 23),
        ],
    }


class StreamingFontRenderTests(unittest.TestCase):
    def test_unicode_bank_lookup_uses_one_based_metric_index(self):
        font = synthetic_font()
        metric = glyph_metric(font, "\u00a9")
        self.assertIsNotNone(metric)
        self.assertEqual(11, metric_advance(metric))

    def test_text_advance_uses_streaming_metrics(self):
        self.assertEqual(26, text_advance(synthetic_font(), "AB \u00a9"))
        self.assertAlmostEqual(
            13, text_advance(synthetic_font(), "AB \u00a9", 0.5))

    def test_word_wrap_preserves_internal_double_space(self):
        font = synthetic_font()
        self.assertEqual(
            ["AB", "A  B"],
            wrap_text(font, "AB A  B", 19))

    def test_word_wrap_uses_scaled_advance(self):
        font = synthetic_font()
        self.assertEqual(
            ["AB A", "B"],
            wrap_text(font, "AB A B", 13, 0.5))


if __name__ == "__main__":
    unittest.main()

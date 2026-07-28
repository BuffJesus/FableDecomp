import sys
import tempfile
import unittest
from pathlib import Path

from PIL import Image


TOOLS_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(TOOLS_ROOT))

import report_environment_lookup as report  # noqa: E402


class EnvironmentLookupReportTests(unittest.TestCase):
    def test_retail_row_mapping(self):
        self.assertEqual(0, report.LOOKUP_ROWS["diffuse"])
        self.assertEqual(1, report.LOOKUP_ROWS["ambient"])
        self.assertEqual(2, report.LOOKUP_ROWS["cloud"])
        self.assertEqual(3, report.LOOKUP_ROWS["backlight"])
        self.assertEqual(13, report.LOOKUP_ROWS["sky_gradient_top"])
        self.assertEqual(15, report.LOOKUP_ROWS["sky_gradient_bottom"])
        self.assertEqual(20, report.LOOKUP_ROWS["glow_bloom"])
        self.assertEqual(
            report.LOOKUP_ROWS["sun"],
            report.LOOKUP_ROWS["sunlight_attenuator"],
        )

    def test_byte_to_float_uses_half_texel_bias(self):
        self.assertEqual(0.5 / 255.0, report.engine_float_channel(0))
        self.assertEqual(255.5 / 255.0, report.engine_float_channel(255))

    def test_samples_rgba_at_exact_column_and_row(self):
        image = Image.new("RGBA", (2, 21), (0, 0, 0, 255))
        image.putpixel((1, 13), (12, 34, 56, 78))
        sample = report.sample_lookup(image, 1, 13)
        self.assertEqual([12, 34, 56, 78], sample["rgba8"])
        self.assertEqual(
            report.engine_float_channel(12),
            sample["rgba_float"][0],
        )

    def test_build_report_rejects_out_of_range_column(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "lookup.png"
            Image.new("RGBA", (2, 21), (0, 0, 0, 255)).save(path)
            with self.assertRaisesRegex(ValueError, "outside"):
                report.build_report(path, [("bad", 2)])


if __name__ == "__main__":
    unittest.main()

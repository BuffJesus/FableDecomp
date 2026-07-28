import os
import struct
import sys
import unittest

from PIL import Image


TOOLS_DIR = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "..", ".."))
if TOOLS_DIR not in sys.path:
    sys.path.insert(0, TOOLS_DIR)

from png_to_windows_cursor import build_cursor  # noqa: E402


class WindowsCursorTests(unittest.TestCase):
    def test_cursor_header_and_hotspot(self):
        image = Image.new("RGBA", (32, 32), (10, 20, 30, 255))
        payload = build_cursor(image, (2, 3))
        self.assertEqual((0, 2, 1), struct.unpack_from("<HHH", payload))
        self.assertEqual(
            (32, 32, 0, 0, 2, 3),
            struct.unpack_from("<BBBBHH", payload, 6))
        self.assertEqual(22, struct.unpack_from("<I", payload, 18)[0])
        self.assertEqual((40, 32, 64), struct.unpack_from("<III", payload, 22))


if __name__ == "__main__":
    unittest.main()

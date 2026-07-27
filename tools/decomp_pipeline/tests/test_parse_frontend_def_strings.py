import os
import struct
import sys
import unittest


TOOLS_DIR = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "..", ".."))
if TOOLS_DIR not in sys.path:
    sys.path.insert(0, TOOLS_DIR)

from parse_frontend import fval  # noqa: E402


class FrontendDefStringTests(unittest.TestCase):
    def test_resolves_names_bin_offset(self):
        names = {0x65C3: "ENG_ARIAL_24"}
        self.assertEqual(
            "ENG_ARIAL_24",
            fval("CDefString", struct.pack("<I", 0x65C3), names=names))

    def test_unresolved_offset_remains_inspectable(self):
        self.assertEqual(
            "@names+0x000048F5",
            fval("CDefString", struct.pack("<I", 0x48F5)))


if __name__ == "__main__":
    unittest.main()

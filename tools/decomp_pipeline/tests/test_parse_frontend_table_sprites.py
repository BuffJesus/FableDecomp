import os
import struct
import sys
import unittest

TOOLS = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
if TOOLS not in sys.path:
    sys.path.insert(0, TOOLS)

from parse_frontend import decode_table_sprites, fval  # noqa: E402


class ParseFrontendTableSpritesTests(unittest.TestCase):
    def test_decodes_retail_enum_order_and_component_indices(self):
        pairs = [
            (0, 19),
            (1, 20),
            (4, 23),
            (5, 25),
            (6, 24),
            (12, 31),
        ]
        raw = struct.pack("<I", len(pairs)) + b"".join(
            struct.pack("<Ii", key, component)
            for key, component in pairs
        )

        self.assertEqual(
            decode_table_sprites(raw),
            [
                {"sprite": "TOP_LEFT", "value": 0, "component": 19},
                {"sprite": "TOP_RIGHT", "value": 1, "component": 20},
                {
                    "sprite": "HORIZONTAL_TOP",
                    "value": 4,
                    "component": 23,
                },
                {
                    "sprite": "HORIZONTAL_BOTTOM",
                    "value": 5,
                    "component": 25,
                },
                {
                    "sprite": "VERTICAL_LEFT",
                    "value": 6,
                    "component": 24,
                },
                {"sprite": "CROSS", "value": 12, "component": 31},
            ],
        )
        self.assertEqual(
            fval("Map_JW4ETableSprites_NUISystem__", raw),
            decode_table_sprites(raw),
        )

    def test_rejects_truncated_map(self):
        self.assertIsNone(
            decode_table_sprites(struct.pack("<III", 2, 0, 19)))


if __name__ == "__main__":
    unittest.main()

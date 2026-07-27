import struct
import sys
import unittest
from pathlib import Path


TOOLS_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(TOOLS_ROOT))

from parse_frontend import decode_ui_states  # noqa: E402
from decode_game_bin_field import field_tag  # noqa: E402


STATE_FIELDS = [
    ("GraphicIndex", "uint32", 4),
    ("PositionX", "float", 70.0),
    ("PositionY", "float", 30.0),
    ("ZoomX", "float", 1.0),
    ("ZoomY", "float", 1.0),
    ("ColourR", "float", 1.0),
    ("ColourG", "float", 1.0),
    ("ColourB", "float", 1.0),
    ("ColourA", "float", 0.5),
    ("UpdateTime", "float", -1.0),
    ("StateChangeType", "enum EStateChangeType_NUISystem", 0),
    ("LinearChange", "bool", False),
    ("StateChangeFlag", "uint32", 7),
    ("ChildrenNotAffected", "Vector_uint32", [2, 9]),
]


def make_state_vector():
    raw = bytearray(struct.pack("<I", 1))
    for name, typ, value in STATE_FIELDS:
        raw += struct.pack("<I", field_tag(name))
        if typ == "float":
            raw += struct.pack("<f", value)
        elif typ == "bool":
            raw += bytes([value])
        elif typ == "Vector_uint32":
            raw += struct.pack("<I", len(value))
            raw += struct.pack("<%dI" % len(value), *value)
        else:
            raw += struct.pack("<I", value)
    return bytes(raw)


class UiStateDecodeTests(unittest.TestCase):
    def setUp(self):
        self.schema = {
            "CUIStateDef": {
                "fields": [
                    {"name": name, "type": typ}
                    for name, typ, _ in STATE_FIELDS
                ]
            }
        }

    def test_decode_tagged_ui_state_vector(self):
        states = decode_ui_states(make_state_vector(), self.schema)

        self.assertEqual(len(states), 1)
        self.assertEqual(states[0]["GraphicIndex"], 4)
        self.assertEqual(states[0]["PositionX"], 70.0)
        self.assertEqual(states[0]["PositionY"], 30.0)
        self.assertEqual(states[0]["ColourA"], 0.5)
        self.assertEqual(states[0]["ChildrenNotAffected"], [2, 9])

    def test_rejects_malformed_ui_state_vector(self):
        self.assertIsNone(
            decode_ui_states(make_state_vector()[:-1], self.schema)
        )


if __name__ == "__main__":
    unittest.main()

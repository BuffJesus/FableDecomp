import sys
import unittest
from pathlib import Path

HERE = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(HERE))
import class_struct


class ConstantArrayBoundsTests(unittest.TestCase):
    def test_hex_subtraction_preserves_following_offset(self):
        fields, size = class_struct.parse_struct(
            "void* vtable; char pad[0x62 - 4]; bool interrupted;",
            packed=True,
        )
        self.assertEqual([(0x62, "bool", "interrupted", 1)], fields)
        self.assertEqual(0x63, size)

    def test_parentheses_and_multiplication(self):
        self.assertEqual(0x20, class_struct.const_int("(0x8 + 8) * 2"))

    def test_rejects_names_and_calls(self):
        with self.assertRaises((SyntaxError, ValueError)):
            class_struct.const_int("sizeof(void*)")


if __name__ == "__main__":
    unittest.main()

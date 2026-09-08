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


class ImplicitVirtualPointerTests(unittest.TestCase):
    def test_virtual_method_places_first_data_member_after_vptr(self):
        fields, size = class_struct.parse_struct(
            "virtual void Reset(); char pad[0x2c]; int state;",
            packed=True,
        )
        self.assertEqual([(0x30, "int", "state", 4)], fields)
        self.assertEqual(0x34, size)

    def test_non_virtual_struct_still_starts_at_zero(self):
        fields, size = class_struct.parse_struct("int value;", packed=True)
        self.assertEqual([(0, "int", "value", 4)], fields)
        self.assertEqual(4, size)

    def test_explicit_vtable_like_member_follows_implicit_vptr(self):
        fields, size = class_struct.parse_struct(
            "void* vtbl; virtual void Reset(); int value;",
            packed=True,
        )
        self.assertEqual([(8, "int", "value", 4)], fields)
        self.assertEqual(12, size)


if __name__ == "__main__":
    unittest.main()

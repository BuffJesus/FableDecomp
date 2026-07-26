import unittest

from tools.decomp_pipeline.auto_author_tiny import const_from_bytes


class TinyPatternTests(unittest.TestCase):
    def test_supported_patterns(self):
        cases = {
            "c3": ("void", None),
            "c21800": ("void_stdcall_pop", 24),
            "33c0c3": ("int", 0),
            "b001c3": ("bool", True),
            "6aff58c3": ("int", -1),
            "b878563412c3": ("int", 0x12345678),
            "8bc1c3": ("return_self", None),
            "8bc1c20400": ("return_self_pop4", None),
            "8b4128c3": ("load_int_field", 0x28),
            "8b8128020000c3": ("load_int_field", 0x228),
            "dd4128c3": ("load_double_field", 0x28),
            "8bc183e00fc3": ("and_self", 0x0F),
            "c70178563412c3": ("store_imm32", 0x12345678),
        }
        for encoded, expected in cases.items():
            with self.subTest(encoded=encoded):
                self.assertEqual(const_from_bytes(bytes.fromhex(encoded)), expected)

    def test_unknown_pattern_is_rejected(self):
        self.assertIsNone(const_from_bytes(bytes.fromhex("558bec5dc3")))


if __name__ == "__main__":
    unittest.main()

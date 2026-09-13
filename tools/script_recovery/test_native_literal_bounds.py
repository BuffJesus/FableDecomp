import unittest

from tools.script_recovery.lift_native_lua import RData


class LiteralBoundsTests(unittest.TestCase):
    def image(self, data, raw_size=8, virtual_size=64):
        image = RData.__new__(RData)
        image.ok = True
        image.base = 0x400000
        image.data = data
        image.raw_secs = [(0x1000, raw_size, 0)]
        image.secs = [(0x1000, virtual_size, 0)]
        return image

    def test_virtual_padding_cannot_read_an_unrelated_file_string(self):
        image = self.image(b'VALID\0xx' + b'FAKE_LITERAL\0')
        self.assertEqual(image.string_at(0x401000), 'VALID')
        self.assertIsNone(image.string_at(0x401008))
        self.assertIsNone(image.bytes_at(0x401008, 4))

    def test_terminator_must_be_inside_same_raw_section(self):
        self.assertIsNone(self.image(b'ABCDEFGH\0').string_at(0x401000))
        self.assertEqual(self.image(b'ABCDEFG\0').string_at(0x401000), 'ABCDEFG')

    def test_next_section_is_read_using_its_own_rva_mapping(self):
        image = self.image(b'FIRST\0xxSECOND\0')
        image.raw_secs.append((0x2000, 7, 8))
        image.secs.append((0x2000, 7, 8))
        self.assertIsNone(image.string_at(0x401008))
        self.assertEqual(image.string_at(0x402000), 'SECOND')

    def test_truncated_file_and_nonliteral_bytes_reject(self):
        self.assertIsNone(self.image(b'SHORT', raw_size=20).string_at(0x401000))
        self.assertIsNone(self.image(b'bad\x01\0xxx').string_at(0x401000))
        self.assertIsNone(self.image(b'\0xxxxxxx').string_at(0x401000))

    def test_existing_scan_limit_is_preserved(self):
        self.assertEqual(self.image(b'x' * 511 + b'\0', 512, 512).string_at(0x401000), 'x' * 511)
        self.assertIsNone(self.image(b'x' * 512 + b'\0', 513, 513).string_at(0x401000))

    def test_actual_retail_fallback_vector_is_not_file_backed(self):
        image = RData()
        rva = 0x143E8E0 - image.base
        self.assertTrue(any(start <= rva < start + size for start, size, _ in image.secs))
        self.assertFalse(any(start <= rva < start + size for start, size, _ in image.raw_secs))
        self.assertIsNone(image.bytes_at(0x143E8E0, 12))
        self.assertIsNone(image.string_at(0x143E8E0))


if __name__ == '__main__':
    unittest.main()

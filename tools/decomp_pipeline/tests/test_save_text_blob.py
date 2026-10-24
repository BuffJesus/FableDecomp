import struct
import unittest

from tools.parse_save_entities import build_lz_text_blob, lz_text_blob


class SaveTextBlobTests(unittest.TestCase):
    def test_empty_text_has_only_a_zero_length_word(self):
        self.assertEqual(build_lz_text_blob(b""), struct.pack("<I", 0))

    def test_short_text_uses_stored_framing(self):
        for text in (b"a", b"ab", b"abc"):
            with self.subTest(text=text):
                encoded = build_lz_text_blob(text)
                self.assertEqual(encoded, struct.pack("<Ih", len(text), 0) + text)
                self.assertEqual(lz_text_blob(encoded, 0), (text, len(encoded)))

    def test_zero_length_reader_preserves_the_next_field(self):
        data = b"prefix" + struct.pack("<I", 0) + b"next field"
        self.assertEqual(lz_text_blob(data, 6), (b"", 10))

    def test_signed_nonpositive_lengths_mean_stored_bytes(self):
        text = b"raw\x00text"
        for compressed_length in (0, -2, -32768):
            with self.subTest(compressed_length=compressed_length):
                encoded = struct.pack("<Ih", len(text), compressed_length) + text
                self.assertEqual(lz_text_blob(encoded, 0), (text, len(encoded)))

    def test_signed_length_boundary_and_extended_header(self):
        for text_size, stream_size in ((32637, 32767), (32638, 32768)):
            with self.subTest(stream_size=stream_size):
                text = b"A" * text_size
                encoded = build_lz_text_blob(text)
                self.assertEqual(struct.unpack_from("<I", encoded)[0], text_size)
                short_length = struct.unpack_from("<h", encoded, 4)[0]
                if stream_size == 32767:
                    self.assertEqual(short_length, stream_size)
                    self.assertEqual(len(encoded), 6 + stream_size + 3)
                else:
                    self.assertEqual(short_length, -1)
                    self.assertEqual(struct.unpack_from("<I", encoded, 6)[0], stream_size)
                    self.assertEqual(len(encoded), 10 + stream_size + 3)
                self.assertEqual(lz_text_blob(encoded, 0), (text, len(encoded)))

    def test_extended_stream_larger_than_unsigned_short_round_trips(self):
        text = b"ABCD" * 18000
        encoded = build_lz_text_blob(text)
        self.assertGreater(struct.unpack_from("<I", encoded, 6)[0], 65535)
        self.assertEqual(lz_text_blob(encoded, 0), (text, len(encoded)))

    def test_adjacent_blobs_preserve_offsets_and_raw_tails(self):
        texts = (b"", b"a", b"header and raw tail", b"ABCD" * 18000)
        data = b"prefix" + b"".join(map(build_lz_text_blob, texts)) + b"suffix"
        offset = 6
        for text in texts:
            decoded, offset = lz_text_blob(data, offset)
            self.assertEqual(decoded, text)
        self.assertEqual(data[offset:], b"suffix")

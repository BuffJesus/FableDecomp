import struct
import sys
import unittest
from pathlib import Path


TOOLS_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(TOOLS_ROOT))

import dump_shader_asm as shader_dump  # noqa: E402


class ShaderDumpTests(unittest.TestCase):
    def test_extracts_original_shader_token_stream(self):
        tokens = struct.pack("<II", 0xFFFE0101, 0x0000FFFF)
        payload = struct.pack("<I", len(tokens)) + tokens
        declared, version, bytecode = (
            shader_dump.shader_bytecode_from_payload(payload)
        )
        self.assertEqual(len(tokens), declared)
        self.assertEqual("vs_1_1", version)
        self.assertEqual(tokens, bytecode)

    def test_rejects_truncated_declared_stream(self):
        payload = struct.pack("<II", 64, 0xFFFF0101)
        with self.assertRaisesRegex(ValueError, "exceeds"):
            shader_dump.shader_bytecode_from_payload(payload)

    def test_rejects_unknown_shader_version(self):
        payload = struct.pack("<II", 4, 0x12345678)
        with self.assertRaisesRegex(ValueError, "unsupported"):
            shader_dump.shader_bytecode_from_payload(payload)


if __name__ == "__main__":
    unittest.main()

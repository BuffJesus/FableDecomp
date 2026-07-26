import json
import tempfile
import unittest
from pathlib import Path

from tools.decomp_pipeline.auto_author_tiny import (
    candidate,
    const_from_bytes,
    rows_with_target_metadata,
)


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
            (
                "568bf1c706dcae2301e8e42d1b00f6442408017407"
                "56e863af7200598bc65ec20400"
            ): ("vector_deleting_destructor_with_vftable", None),
            (
                "568bf1e80be2fffff644240801740756e8d2c07100"
                "598bc65ec20400"
            ): ("scalar_deleting_destructor", None),
            (
                "568bf18d4e2ce828704c008bcee871ed1a00"
                "f644240801740756e8f06e7200598bc65ec20400"
            ): ("composite_scalar_deleting_destructor", 0x2C),
            (
                "568bf18d4e28e828704c008bcee871ed1a00"
                "f644240801740956e8f06e720083c4048bc65ec20400"
            ): ("composite_scalar_deleting_destructor_speed", 0x28),
            (
                "568bf18d8ea8000000e828704c008bcee871ed1a00"
                "f644240801740956e8f06e720083c4048bc65ec20400"
            ): ("composite_scalar_deleting_destructor_speed", 0xA8),
            (
                "51568bf18b56048b0e8d44240750e8e5dafdff8b36"
                "85f6740756e8d2b97800595e59c3"
            ): ("finish_async_read_then_release", None),
            (
                "568bf1837e0400741e8b06ff7004e8defcffff8b06894008"
                "8b06836004008b0689400c836604005ec3"
            ): ("free_and_reset_intrusive_list", None),
            (
                "53568b74240c85f68bd9741c57ff760c8bcbe8e9ffffff"
                "8b7e0856e82948720085ff598bf775e65f5e5bc20400"
            ): ("consume_linked_tree", None),
            (
                "568bf18b0e578b7c240c3b0f741c85c9740dff49047505"
                "8b01ff50048326008b0785c089067403ff40045f5ec20400"
            ): ("assign_intrusive_counted_handle", None),
        }
        for encoded, expected in cases.items():
            with self.subTest(encoded=encoded):
                self.assertEqual(const_from_bytes(bytes.fromhex(encoded)), expected)

    def test_counted_assignment_candidate_has_ownership_fixture(self):
        authored = candidate(
            {
                "address": "00485657",
                "name": "GetMeshEffect",
                "module": "CEngineInternalPrimitiveMeshBase",
                "bytes": (
                    "568bf18b0e578b7c240c3b0f741c85c9740dff49047505"
                    "8b01ff50048326008b0785c089067403ff40045f5ec20400"
                ),
            }
        )

        self.assertIsNotNone(authored)
        self.assertIn("current != source->object", authored["source_cpp"])
        self.assertIn("--current->references", authored["source_cpp"])
        self.assertIn("++incoming->references", authored["source_cpp"])
        self.assertIn("destination.Assign(&destination)", authored["test_cpp"])
        self.assertIn("retained.references != 1", authored["test_cpp"])

    def test_unknown_pattern_is_rejected(self):
        self.assertIsNone(const_from_bytes(bytes.fromhex("558bec5dc3")))
        self.assertIsNone(
            const_from_bytes(
                bytes.fromhex(
                    "568bf18b0e578b7c240c3b0f741c85c9740dff49047505"
                    "8b01ff50048326008b0785c089067403ff40085f5ec20400"
                )
            )
        )

    def test_oracle_rows_are_enriched_from_sibling_target_catalog(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            oracle = root / "batch_oracle.tsv"
            oracle.write_text(
                "address\tname\tlength\tbytes\n"
                "00401000\tOnReadFinished\t1\tc3\n",
                encoding="utf-8",
            )
            (root / "batch_targets.json").write_text(
                json.dumps(
                    [
                        {
                            "address": "00401000",
                            "module": "CActiveFile",
                            "cc": "__thiscall",
                            "ret": "void",
                            "pc": "1",
                        }
                    ]
                ),
                encoding="utf-8",
            )

            row = next(rows_with_target_metadata(oracle))

        self.assertEqual(row["module"], "CActiveFile")
        self.assertEqual(row["cc"], "__thiscall")
        self.assertEqual(row["ret"], "void")
        self.assertEqual(row["pc"], "1")


if __name__ == "__main__":
    unittest.main()

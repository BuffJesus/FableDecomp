import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.build_native_catalog import address_from_allocator, load_registry


class NativeCatalogTests(unittest.TestCase):
    def test_allocator_address(self):
        self.assertEqual(address_from_allocator("(code *)&LAB_00e2ab00"), "0x00E2AB00")
        self.assertIsNone(address_from_allocator("NScript::CQ_TestScript::Alloc"))

    def test_non_quest_registry_kinds(self):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "registry.tsv"
            path.write_text(
                "quest_name\tid\tmaster\tallocFunc\tdataAlloc\tsection\n"
                "Q_One\t1\t0\t(code *)&LAB_00123456\tdata\tS_Q1\n"
                "V_Two\t1\t0\tNamed::Alloc\tdata\tS_V2\n"
                "Gameflow\t1\t0\tNamed::Alloc\tdata\tS_GF\n",
                encoding="utf-8",
            )
            rows = load_registry(path)
            self.assertEqual([row["kind"] for row in rows], ["quest", "village", "global"])

    def test_registry_operand_evidence_overrides_bad_fid_name(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            registry = root / "registry.tsv"
            registry.write_text(
                "quest_name\tid\tmaster\tallocFunc\tdataAlloc\tsection\n"
                "Q_One\t1\t0\tWrong::Alloc\tdata\tS_Q1\n",
                encoding="utf-8",
            )
            evidence = root / "evidence.tsv"
            evidence.write_text(
                "quest_name\tallocator_address\tname_xref\tallocator_operand\tmethod\n"
                "Q_One\t00abcdef\t00100000\t00100020\tregistry-pattern\n",
                encoding="utf-8",
            )
            row = load_registry(registry, evidence)[0]
            self.assertEqual(row["allocatorAddress"], "0x00ABCDEF")
            self.assertEqual(row["evidenceLevel"], "registry-operand")


if __name__ == "__main__":
    unittest.main()

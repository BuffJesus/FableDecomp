import unittest
from pathlib import Path

from tools.script_recovery.audit_affair_wife_position_chain import audit


class AffairWifePositionChainAuditTests(unittest.TestCase):
    def test_retail_and_forge_position_chains_agree(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root, Path(r"D:\Code\ForgeFSE-retail-shadow"))
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["nativeFunctions"], 3)
        self.assertEqual(result["forgeChecksPassed"], 9)

    def test_export_is_manifest_driven_and_read_only(self):
        root = Path(__file__).resolve().parents[2]
        source = (root / "tools/script_recovery/export_affair_wife_position_chain.ps1").read_text()
        self.assertIn('Import-Csv -Delimiter "`t"', source)
        self.assertIn('"-readOnly", "-noanalysis"', source)
        self.assertNotIn("0x00", source)


if __name__ == "__main__":
    unittest.main()

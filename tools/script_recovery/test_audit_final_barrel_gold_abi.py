import unittest
from pathlib import Path

from tools.script_recovery.audit_final_barrel_gold_abi import audit


class FinalBarrelGoldAbiAuditTests(unittest.TestCase):
    def test_retail_forge_and_lua_agree(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["vtableIndex"], 585)
        if result["installedExeSha256"] is not None:
            self.assertEqual(result["installedSlotTarget"], "0x0089E780")


if __name__ == "__main__":
    unittest.main()

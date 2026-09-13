import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_constants import audit


class NewOakValeConstantFidelityAuditTests(unittest.TestCase):
    def test_native_constants_match_lua_literals(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/new_oakvale_intro/entities",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["inventoryConstants"], 214)
        self.assertEqual(result["literalMatched"], 200)
        self.assertEqual(result["contextualVerified"], 14)


if __name__ == "__main__":
    unittest.main()

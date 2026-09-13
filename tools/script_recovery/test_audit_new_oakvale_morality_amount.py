import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_morality_amount import audit


class NewOakValeMoralityAmountAuditTests(unittest.TestCase):
    def test_installed_data_native_forge_and_lua_amounts_are_locked(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root, Path(r"D:\Code\ForgeFSE-retail-shadow"))
        self.assertTrue(result["ok"], result)
        self.assertAlmostEqual(result["decodedAmount"], 0.001)


if __name__ == "__main__":
    unittest.main()

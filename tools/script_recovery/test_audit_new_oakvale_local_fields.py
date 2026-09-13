import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_local_fields import audit


class NewOakvaleLocalFieldAuditTests(unittest.TestCase):
    def test_all_native_local_fields_and_defaults_exist_in_lua(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/new_oakvale_intro/entities",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["localFields"], 32)
        self.assertGreaterEqual(result["constantResolvedDefaults"], 4)


if __name__ == "__main__":
    unittest.main()

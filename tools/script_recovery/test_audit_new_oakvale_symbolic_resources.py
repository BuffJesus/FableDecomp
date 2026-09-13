import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_symbolic_resources import audit


class NewOakvaleSymbolicResourceAuditTests(unittest.TestCase):
    def test_every_executable_resource_is_inventoried(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/new_oakvale_intro/entities",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["declaredResources"], 235)
        self.assertEqual(result["executableResources"], 238)
        self.assertEqual(result["exactResources"], 224)
        self.assertEqual(len(result["dynamicPrefixes"]), 14)
        self.assertEqual(result["diagnosticStringsExcluded"], 18)


if __name__ == "__main__":
    unittest.main()

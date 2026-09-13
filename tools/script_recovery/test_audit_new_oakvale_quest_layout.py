import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_quest_layout import audit


class NewOakValeQuestLayoutAuditTests(unittest.TestCase):
    def test_all_named_fields_fit_the_retail_quest_object(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["objectSize"], 268)
        self.assertEqual(result["namedFields"], 37)
        self.assertEqual(result["layoutRegions"], 6)


if __name__ == "__main__":
    unittest.main()

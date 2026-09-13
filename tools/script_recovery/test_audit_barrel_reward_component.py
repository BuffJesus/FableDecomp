import unittest
from pathlib import Path

from tools.script_recovery.audit_barrel_reward_component import audit


class BarrelRewardComponentAuditTests(unittest.TestCase):
    def test_all_new_oakvale_barrels_have_reward_component(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["barrelCount"], 9)
        self.assertEqual(result["definitionTypes"], ["OBJECT_BARREL_BREAKABLE"])
        self.assertTrue(result["hasContainerRewardHero"])


if __name__ == "__main__":
    unittest.main()

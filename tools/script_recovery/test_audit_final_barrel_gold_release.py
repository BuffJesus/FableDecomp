import unittest
from pathlib import Path

from tools.script_recovery.audit_final_barrel_gold_release import audit


class FinalBarrelGoldReleaseAuditTests(unittest.TestCase):
    def test_installed_insertion_to_pickup_chain(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["addresses"]["rewardOnDie"], "0x0074F080")
        self.assertEqual(result["resolvedCalls"]["rewardOnDieToCreatePickup"], "0x0074EC50")
        self.assertEqual(result["resolvedCalls"]["createPickupToInsertItem"], "0x008ED1F0")
        self.assertTrue(result["rewardEnabledByConstructor"])
        self.assertTrue(result["barrelHasRewardComponent"])
        self.assertFalse(result["barrelHasSummonableCreatureComponent"])


if __name__ == "__main__":
    unittest.main()

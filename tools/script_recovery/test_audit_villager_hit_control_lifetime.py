import unittest
from pathlib import Path

from tools.script_recovery.audit_villager_hit_control_lifetime import audit


class VillagerHitControlLifetimeAuditTests(unittest.TestCase):
    def test_lua_matches_native_loop_head_cleanup(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Villager.lua",
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Villager.json",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()

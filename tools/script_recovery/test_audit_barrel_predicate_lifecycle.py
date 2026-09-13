import unittest
from pathlib import Path

from tools.script_recovery.audit_barrel_predicate_lifecycle import audit


class BarrelPredicateLifecycleAuditTests(unittest.TestCase):
    def test_bound_thing_death_dispatches_barrel_callback_once(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/new_oakvale_intro/translation_unit.json",
            root / "ghidra_out/egor_pdb_names.tsv",
            Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender\LuaEntityHost.cpp"),
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Barrel.lua",
            root / "refs/script_recovery/new_oakvale_intro/runtime_evidence/single-authority-childhood-complete-20260910.log",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()

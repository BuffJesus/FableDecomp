import unittest
from pathlib import Path

from tools.script_recovery.audit_barrel_man_phase_reachability import audit


class BarrelManPhaseReachabilityAuditTests(unittest.TestCase):
    def test_phase_one_is_dead_from_clean_lifecycle(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/barrel_man_main_disasm.txt",
            root / "ghidra_out/script_recovery/barrel_man_donor.c",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BarrelMan.lua",
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_BarrelMan.json",
            root / "ghidra_out/struct_layouts_egor.tsv",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()

import unittest
from pathlib import Path

from tools.script_recovery.audit_victim_init_identity import audit


class VictimInitIdentityAuditTests(unittest.TestCase):
    def test_retail_vtable_body_and_donor_layout_prove_identity(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/new_oakvale_intro/translation_unit.json",
            root / "ghidra_out/egor_pdb_names.tsv",
            root / "ghidra_out/struct_layouts_egor.tsv",
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Victim.json",
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_BookTrader.json",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()

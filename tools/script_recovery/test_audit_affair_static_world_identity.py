import unittest
from pathlib import Path

from tools.script_recovery.audit_affair_static_world_identity import audit


class AffairStaticWorldIdentityAuditTests(unittest.TestCase):
    def test_authored_husband_identity_is_unique(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/affair_static_world_inventory.txt",
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_AffairWife.json")
        self.assertTrue(result["ok"], result)
        self.assertTrue(result["checks"]["manifestHasNoStaticUncertainty"])
        self.assertTrue(result["checks"]["manifestExcludesAuthoredDuplicateButRetainsRuntimeObservation"])


if __name__ == "__main__":
    unittest.main()

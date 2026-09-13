import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_lookup_position_abi import audit


class LookupPositionAbiAuditTests(unittest.TestCase):
    def test_current_abi_and_forwarding_pass(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            Path(r"D:\Code\ForgeFSE-retail-shadow"),
            root / "ghidra_out/egor_pdb_names.tsv",
            root / "ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv",
            root / "ghidra_out/script_recovery/thing_position_donor.c",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()

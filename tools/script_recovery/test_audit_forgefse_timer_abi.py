import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.audit_forgefse_timer_abi import audit


class TimerAbiAuditTests(unittest.TestCase):
    def test_current_evidence_passes(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            Path(r"D:\Code\ForgeFSE-retail-shadow"),
            root / "ghidra_out/egor_pdb_names.tsv",
            root / "ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv",
            root / "ghidra_out/scriptvm_decomp.c",
            root / "refs/script_recovery/new_oakvale_intro/runtime_evidence/single-authority-guard-success-20260910.log",
            root / "ghidra_out/script_recovery/timer_world_update_donor.c",
        )
        self.assertTrue(result["ok"], result)

    def test_rejects_non_countdown_trace(self):
        root = Path(__file__).resolve().parents[2]
        with tempfile.TemporaryDirectory() as tmp:
            bad_log = Path(tmp) / "bad.log"
            bad_log.write_text(
                "\n".join(f"NOVI_PROBE StartBarrelTimer update bar=2 value={n}"
                          for n in (45, 44, 44, 42, 0)), encoding="utf-8")
            result = audit(
                Path(r"D:\Code\ForgeFSE-retail-shadow"),
                root / "ghidra_out/egor_pdb_names.tsv",
                root / "ghidra_out/script_recovery/gamescriptinterface_vtable_slots.tsv",
                root / "ghidra_out/scriptvm_decomp.c",
                bad_log,
                root / "ghidra_out/script_recovery/timer_world_update_donor.c",
            )
            self.assertFalse(result["checks"]["runtimeCountsDown45ToZero"])


if __name__ == "__main__":
    unittest.main()

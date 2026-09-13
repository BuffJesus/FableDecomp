import unittest
from pathlib import Path

from tools.script_recovery.audit_barrel_thug_speech_operands import audit


class BarrelThugSpeechOperandAuditTests(unittest.TestCase):
    def test_current_evidence_passes(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/barrel_thug_speech_disasm.txt",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BarrelThug.lua",
            Path(r"D:\Code\ForgeFSE-retail-shadow"),
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()

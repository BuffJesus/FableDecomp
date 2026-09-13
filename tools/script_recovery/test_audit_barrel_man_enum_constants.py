import unittest
from pathlib import Path

from tools.script_recovery.audit_barrel_man_enum_constants import audit


class BarrelManEnumConstantsAuditTests(unittest.TestCase):
    def test_numeric_operands_have_exact_named_enums(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            Path(r"D:\Code\ForgeFSE-retail-shadow\FableScriptExtender\EntityScriptingAPI.h"),
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BarrelMan.lua",
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_BarrelMan.json")
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()

import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_phases import audit


class NewOakValePhaseAuditTests(unittest.TestCase):
    def test_phase_records_resolve_to_lua_functions_and_enums(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "refs/script_recovery/new_oakvale_intro/entities",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["filesWithPhases"], 16)
        self.assertEqual(result["phases"], 74)
        self.assertEqual(result["enumPhases"], 6)
        self.assertGreaterEqual(result["luaFunctionAnchors"], 95)


if __name__ == "__main__":
    unittest.main()

import unittest
from pathlib import Path

from tools.script_recovery.audit_theresa_given_chocs_lifetime import audit


class TheresaGivenChocsLifetimeAuditTests(unittest.TestCase):
    def test_stack_local_and_lua_lifetime_match(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/theresa_presented_item_disasm.txt",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Theresa.lua",
            root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Theresa.json",
        )
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()

import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_child_morph_boundary import audit


class NewOakValeChildMorphBoundaryAuditTests(unittest.TestCase):
    def test_retail_forge_and_lua_child_morph_boundary_is_locked(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root, Path(r"D:\Code\ForgeFSE-retail-shadow"))
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()

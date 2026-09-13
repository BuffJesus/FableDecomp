import unittest
from pathlib import Path

from tools.script_recovery.audit_bully_cutscene_wrapper_boundary import audit


class BullyCutsceneWrapperBoundaryAuditTests(unittest.TestCase):
    def test_native_forge_runtime_and_skip_boundary_is_locked(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(root, Path(r"D:\Code\ForgeFSE-retail-shadow"))
        self.assertTrue(result["ok"], result)


if __name__ == "__main__":
    unittest.main()

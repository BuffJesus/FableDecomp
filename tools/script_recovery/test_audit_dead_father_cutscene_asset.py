import unittest
from pathlib import Path

from tools.script_recovery.audit_dead_father_cutscene_asset import audit


class DeadFatherCutsceneAssetAuditTests(unittest.TestCase):
    def test_macro_and_every_camera_spline_are_retail_decoded(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["macroCommandCount"], 89)
        self.assertEqual(len(result["cameraRecords"]), 11)


if __name__ == "__main__":
    unittest.main()

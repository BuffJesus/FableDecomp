import unittest
from pathlib import Path

from tools.script_recovery.audit_gold_piece_definition import audit


class GoldPieceDefinitionAuditTests(unittest.TestCase):
    def test_object_gold_one_is_collectible_one_gold(self):
        result = audit(Path(__file__).resolve().parents[2])
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["objectIndex"], 4644)
        self.assertEqual(result["linkedGoldDefIndex"], 13648)
        self.assertEqual(result["linkedGoldAmount"], 1)
        self.assertTrue(result["isCollectibleGold"])


if __name__ == "__main__":
    unittest.main()

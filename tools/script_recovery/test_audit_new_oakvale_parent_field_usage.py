import json
import unittest
from pathlib import Path

from tools.script_recovery.audit_new_oakvale_parent_field_usage import audit


class NewOakvaleParentFieldUsageAuditTests(unittest.TestCase):
    def test_direct_entity_state_access_is_declared_and_defined(self):
        root = Path(__file__).resolve().parents[2]
        package = root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro"
        result = audit(root / "refs/script_recovery/new_oakvale_intro/entities",
                       package / "Entities", package / "fields.lua")
        self.assertTrue(result["ok"], result)
        self.assertEqual(result["canonicalFields"], 37)
        self.assertEqual(result["recognizedStaticParentData"], 8)
        self.assertEqual(result["canonicalMasterFields"], ["TeddySolution"])
        self.assertEqual(result["masterReadFields"], [])
        self.assertEqual(result["masterWriteFields"], ["TeddySolution"])
        self.assertEqual(result["masterWriteSites"], 2)
        self.assertEqual(len(result["directReadFields"]), 30)
        self.assertEqual(len(result["directWriteFields"]), 28)
        self.assertEqual(len(result["packageReadFields"]), 34)
        self.assertEqual(len(result["packageWriteFields"]), 32)

    def test_generated_persistence_manifest_preserves_array_and_indirect_owners(self):
        root = Path(__file__).resolve().parents[2]
        manifest = json.loads((root / "refs/script_recovery/new_oakvale_intro/persistence_manifest.json")
                              .read_text(encoding="utf-8-sig"))
        fields = {row["name"]: row for row in manifest["fields"]}
        self.assertEqual(len(fields), 37)
        array = fields["WhichBadDeedsPerformed"]
        self.assertEqual(array["type"], "bool[5]")
        self.assertEqual(array["retailOffset"], "0xfc..0x100")
        self.assertEqual(array["readBy"], ["NOVI_Guard"])
        self.assertEqual(array["writtenBy"], [
            "NOVI_AffairMan", "NOVI_AffairWife", "NOVI_AffairWoman", "NOVI_BarrelMan",
            "NOVI_BarrelThug", "NOVI_BookTrader", "NOVI_Bully", "NOVI_LiveFather",
            "NOVI_TeddyGirl", "NOVI_Theresa", "NOVI_Victim", "NOVI_Villager"])
        self.assertEqual(fields["BadDeedsPerformed"]["writtenBy"], array["writtenBy"])
        self.assertEqual(fields["GoodDeedsPerformed"]["writtenBy"], [
            "NOVI_AffairWife", "NOVI_BarrelMan", "NOVI_Bully", "NOVI_TeddyGirl"])
        self.assertEqual(
            [(row["script"], row["field"], row["value"]) for row in manifest["masterData"]],
            [("NOVI_TeddyGirl", "TeddySolution", "B"),
             ("NOVI_TeddyGirl", "TeddySolution", "C")])


if __name__ == "__main__":
    unittest.main()

import json
import unittest
from pathlib import Path

from tools.script_recovery.audit_barrel_man_consequence import audit


class BarrelManConsequenceAuditTests(unittest.TestCase):
    def test_retail_judgment_and_followup_are_separate(self):
        root = Path(__file__).resolve().parents[2]
        result = audit(
            root / "ghidra_out/script_recovery/barrel_man_main_disasm.txt",
            root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_BarrelMan.lua",
        )
        self.assertTrue(result["ok"], result)

    def test_nearby_broken_stock_rebukes_without_good_or_unattended_deed(self):
        root = Path(__file__).resolve().parents[2]
        trace = json.loads((root / "refs/script_recovery/new_oakvale_intro/traces/entity_barrel_man_nearby_broken_scolds.json").read_text())
        events = trace["events"]
        spoken = [event.get("arguments", [None, None])[1] for event in events if event.get("name") == "Speak"]
        self.assertIn("TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN", spoken)
        self.assertNotIn("TEXT_QST_048_BARRELMAN_THANKS", spoken)
        deed_writes = [event.get("arguments", [None])[0] for event in events if event.get("name") == "SetStateInt"]
        self.assertNotIn("GoodDeedsPerformed", deed_writes)
        self.assertNotIn("BadDeedsPerformed", deed_writes)

    def test_absent_broken_route_awards_bad_deed_then_scolds_on_interaction(self):
        root = Path(__file__).resolve().parents[2]
        trace = json.loads((root / "refs/script_recovery/new_oakvale_intro/traces/entity_barrel_man_absent_broken_then_scolds.json").read_text())
        events = trace["events"]

        def first_index(name, argument=None):
            for index, event in enumerate(events):
                if event.get("name") != name:
                    continue
                if argument is None or argument in event.get("arguments", []):
                    return index
            self.fail(f"missing {name} event containing {argument!r}")

        unattended = first_index("DisplayGameInfo", "TEXT_QST_048_INSTRUCTION_LEFT_WAREHOUSE_UNATTENDED")
        bad_deed = first_index("SetStateInt", "BadDeedsPerformed")
        interaction = first_index("IsTalkedToByHero")
        broken_rebuke = first_index("Speak", "TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN")
        self.assertLess(unattended, bad_deed)
        self.assertLess(bad_deed, interaction)
        self.assertLess(interaction, broken_rebuke)

        spoken = [event.get("arguments", [None, None])[1] for event in events if event.get("name") == "Speak"]
        self.assertNotIn("TEXT_QST_048_BARRELMAN_THANKS", spoken)
        deed_writes = [event.get("arguments", [None])[0] for event in events if event.get("name") == "SetStateInt"]
        self.assertNotIn("GoodDeedsPerformed", deed_writes)


if __name__ == "__main__":
    unittest.main()

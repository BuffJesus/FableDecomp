import tempfile
import unittest
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
from audit_new_oakvale_npc_baseline import MAPS, extract


CREATURE = """NewThing AICreature;
UID 42;
DefinitionType \"CREATURE_TEST\";
ScriptName NOVI_Villager;
ThingGamePersistent FALSE;
ThingLevelPersistent TRUE;
StartCTCPhysicsNavigator;
PositionX 1.25;
PositionY 2.5;
PositionZ 3.75;
RHSetForwardX -1.0;
RHSetForwardY 0.0;
RHSetForwardZ 0.0;
EndCTCPhysicsNavigator;
Health 12.0;
OverridingBrainName NULL;
HasInformation FALSE;
WanderWithInformation TRUE;
ContinueAIWithInformation FALSE;
EnableCreatureAutoPlacing FALSE;
AllowedToFollowHero FALSE;
InitialPosX 101.25;
InitialPosY 202.5;
InitialPosZ 3.75;
EndThing;
"""


class BaselineAuditTests(unittest.TestCase):
    def test_extracts_values_without_coordinate_conversion(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / MAPS[0]).write_text(CREATURE, encoding="utf-8")
            (root / MAPS[1]).write_text("Version 2;\n", encoding="utf-8")
            result = extract(root)
        self.assertEqual(result["recordCount"], 1)
        self.assertEqual(result["countsByScript"], {"NOVI_Villager": 1})
        record = result["records"][0]
        self.assertEqual(record["positionLocal"], [1.25, 2.5, 3.75])
        self.assertEqual(record["positionInitialWorld"], [101.25, 202.5, 3.75])
        self.assertEqual(record["forward"], [-1.0, 0.0, 0.0])
        self.assertEqual(record["health"], 12.0)
        self.assertFalse(record["thingGamePersistent"])
        self.assertTrue(record["thingLevelPersistent"])

    def test_ignores_non_novi_scripts(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            text = CREATURE.replace("NOVI_Villager", "V_GenericVillager")
            for name in MAPS:
                (root / name).write_text(text, encoding="utf-8")
            result = extract(root)
        self.assertEqual(result["recordCount"], 0)


if __name__ == "__main__":
    unittest.main()

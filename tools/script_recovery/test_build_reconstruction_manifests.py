import unittest

from tools.script_recovery.build_reconstruction_manifests import fixture_covers, status_for


class ReconstructionCoverageTests(unittest.TestCase):
    def test_entity_fixture_is_source_scoped(self):
        meta = {"function": "Main", "source": "NewOakValeIntro/Entities/NOVI_Barrel.lua"}
        passed = {"status": "ran", "trace": "match"}
        self.assertTrue(fixture_covers("entity", meta["source"], "Main", meta, passed))
        self.assertFalse(fixture_covers(
            "entity", "NewOakValeIntro/Entities/NOVI_Bully.lua", "Main", meta, passed))

    def test_failed_or_different_function_is_not_coverage(self):
        meta = {"function": "Init", "source": "NewOakValeIntro/Entities/NOVI_Barrel.lua"}
        self.assertFalse(fixture_covers("entity", meta["source"], "Main", meta,
                                       {"status": "ran", "trace": "match"}))
        self.assertFalse(fixture_covers("entity", meta["source"], "Init", meta,
                                       {"status": "expectation-failed", "trace": "match"}))

    def test_default_source_only_covers_quest(self):
        meta = {"function": "Main", "source": None}
        passed = {"status": "ran", "trace": "written"}
        self.assertTrue(fixture_covers("quest", "NewOakValeIntro/NewOakValeIntro.lua",
                                      "Main", meta, passed))
        self.assertFalse(fixture_covers("entity", "NewOakValeIntro/Entities/NOVI_Barrel.lua",
                                       "Main", meta, passed))

    def test_untraced_status_is_not_mistaken_for_traced(self):
        operation = {"function": "GivenTeddy", "forgeBinding": "GiveHeroGold",
                     "evidence": "native-decompile", "argsKnown": True}
        self.assertEqual(status_for(operation, set(), False), "implemented-but-untraced")


if __name__ == "__main__":
    unittest.main()

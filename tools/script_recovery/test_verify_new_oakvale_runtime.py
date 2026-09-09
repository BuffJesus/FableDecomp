import unittest

from tools.script_recovery.verify_new_oakvale_runtime import analyze


class RuntimeLogTest(unittest.TestCase):
    def test_tracks_identity_preserving_startup(self):
        text = "\n".join((
            "Retail allocator override hook installed",
            "Armed identity-preserving retail override for 'Q_NewOakValeIntro'",
            "Replacing retail allocator for 'Q_NewOakValeIntro'",
            "LuaQuestHost for 'NewOakValeIntro/NewOakValeIntro' created",
            "Quest 'NewOakValeIntro/NewOakValeIntro': C++ Init() phase",
            "Quest 'NewOakValeIntro/NewOakValeIntro': C++ Main() phase",
        ))
        result = analyze(text)
        self.assertEqual(result["phase"], "quest-main")
        self.assertTrue(result["milestones"]["allocatorReplaced"])
        self.assertFalse(result["playthroughComplete"])

    def test_runtime_error_wins_phase(self):
        result = analyze("LuaQuestHost for 'NewOakValeIntro/NewOakValeIntro' created\n!!! LUA RUNTIME ERROR boom")
        self.assertEqual(result["phase"], "runtime-error")
        self.assertEqual(len(result["errors"]), 1)

    def test_ignores_other_quest_errors_before_host_creation(self):
        result = analyze("!!! LUA RUNTIME ERROR unrelated\nLuaQuestHost for 'NewOakValeIntro/NewOakValeIntro' created")
        self.assertEqual(result["errors"], [])


if __name__ == "__main__":
    unittest.main()

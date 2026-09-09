import tempfile
import unittest
from pathlib import Path

from tools.script_recovery.run_lua_trace import LuaRuntime, run_trace


CORPUS = Path("refs/script_recovery/seed_corpus/sources/MeetSister/FSE/MeetSister")
NOVI_FSE = Path("refs/script_recovery/reconstructed/NewOakValeIntro/FSE")


@unittest.skipIf(LuaRuntime is None, "lupa is not installed")
class LuaTraceTests(unittest.TestCase):
    def test_quest_main_records_exact_arguments(self):
        trace = run_trace(
            CORPUS / "MeetSister.lua", "Main", "quest",
            {"Quest.GetActiveQuestName": ["QS_MeetSister"]},
        )
        calls = [(event["name"], event["arguments"]) for event in trace["events"]]
        self.assertIn(("AddEntityBinding", ["MeetSisterMessenger", "MeetSister/Entities/MeetSisterMessenger"]), calls)
        self.assertIn(("SetQuestCardObjective", ["QS_MeetSister", "TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY",
                                                  "HauntedHouse", "BarrowFields"]), calls)

    def test_entity_fixture_drives_wait_loop_and_handles(self):
        trace = run_trace(
            CORPUS / "Entities/MeetSisterMessenger.lua", "Main", "entity",
            {
                "Quest.GetThingWithScriptName": [{"handle": "messenger"}],
                "Quest.IsRegionLoaded": [False, True],
                "Quest.NewScriptFrame": [True],
                "Quest.GetHero": [{"handle": "hero"}],
            },
        )
        names = [event["name"] for event in trace["events"]]
        self.assertEqual(names.count("IsRegionLoaded"), 2)
        self.assertEqual(names.count("NewScriptFrame"), 1)
        cutscene = next(event for event in trace["events"] if event["name"] == "PlayCutscene")
        self.assertEqual(cutscene["arguments"][0], "CS_ARENA_LEAVE_THERESA_01")
        self.assertEqual(cutscene["arguments"][1]["Hero"]["handle"], "hero")
        self.assertEqual(cutscene["arguments"][1]["Messenger"]["handle"], "me")
        get_hero = next(event for event in trace["events"] if event["name"] == "GetHero")
        self.assertEqual(get_hero["arguments"], [])

    def test_instruction_budget_stops_unbounded_script(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "loop.lua"
            source.write_text("function Main(q) while true do end end", encoding="utf-8")
            with self.assertRaisesRegex(RuntimeError, "instruction budget exceeded"):
                run_trace(source, "Main", "quest", {}, instruction_budget=2_000)

    def test_math_random_can_be_driven_deterministically(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "random.lua"
            source.write_text("function Main(q) q:Record(math.random(0, 99), math.random(0, 5)) end",
                              encoding="utf-8")
            trace = run_trace(source, "Main", "quest", {"Lua.math.random": [0, 4]})
            random_events = [event for event in trace["events"] if event["name"] == "random"]
            self.assertEqual([event["arguments"] for event in random_events], [[0, 99], [0, 5]])
            record = next(event for event in trace["events"] if event["name"] == "Record")
            self.assertEqual(record["arguments"], [0, 4])

    def test_novi_pc_platform_and_distance_fallbacks(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "common_fallbacks.lua"
            source.write_text(
                'local N=require("NewOakValeIntro.common")\n'
                'function Main(q,me) q:Record(N.is_xbox(), '
                'N.distance_from_thing_to_position_over(me,{x=0,y=0,z=0},2), '
                'N.things_over(q,me,q:GetHero(),20)) end', encoding="utf-8")
            trace = run_trace(source, "Main", "entity", {
                "Entity.GetPos": [{"x": 3, "y": 0, "z": 0}],
                "Quest.GetHero": [{"handle": "hero", "scope": "Entity"}],
                "Quest.IsDistanceBetweenThingsUnder": [False],
            }, package_path=NOVI_FSE)
            record = next(event for event in trace["events"] if event["name"] == "Record")
            self.assertEqual(record["arguments"], [False, True, True])


if __name__ == "__main__":
    unittest.main()

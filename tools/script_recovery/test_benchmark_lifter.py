import unittest

from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker, recovered_sources


class SyntaxScoringTests(unittest.TestCase):
    def test_compiles_without_executing_source_and_reports_location(self):
        checker = LuaSyntaxChecker()
        result = checker.check({"good.lua": 'error("must never execute")',
                                "bad.lua": "function Main()\nlocal v = *ptr\nend"})
        self.assertEqual(result["runtime"], "Lua 5.4")
        self.assertFalse(result["ok"])
        self.assertEqual((result["passed"], result["checked"]), (1, 2))
        self.assertEqual(result["errors"][0]["path"], "bad.lua")
        self.assertEqual(result["errors"][0]["line"], 2)

    def test_only_recovered_bodies_count_not_binding_stubs(self):
        report = {"package": "Demo", "lua": "function Main() end",
                  "entities": ["Recovered", "Unrecovered"],
                  "entityBodies": {"Recovered": {"lua": "function Main() local v = *ptr end"}}}
        sources = recovered_sources(report)
        self.assertEqual(set(sources), {"Demo/FSE/Demo/Demo.lua",
                                       "Demo/FSE/Demo/Entities/Recovered.lua"})
        result = LuaSyntaxChecker().check(sources)
        self.assertEqual((result["passed"], result["checked"]), (1, 2))

    def test_entity_mode_is_scored_as_one_recovered_file(self):
        sources = recovered_sources({"entity": "Actor", "lua": "function Main() end"})
        self.assertEqual(set(sources), {"entities/Actor.lua"})
        self.assertTrue(LuaSyntaxChecker().check(sources)["ok"])


if __name__ == "__main__":
    unittest.main()

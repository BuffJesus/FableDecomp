import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.affair_man_complete import LABELS as HUSBAND_LABELS
from tools.script_recovery.generate_affair_man_resource_candidate import generate
from tools.script_recovery.readable_lua import readable_source, rename_labels
from tools.script_recovery.structure_affair_man_lua import structure_cleanup
from tools.script_recovery.test_generate_affair_man_resource_candidate import HARNESS


class HusbandStructureTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.native_candidate, _ = generate()
        readable, _ = readable_source(cls.native_candidate, inline_literals=True)
        cls.before = rename_labels(readable, HUSBAND_LABELS)
        cls.after, cls.report = structure_cleanup(cls.before)

    def events(self, source, scenario):
        lua = LuaRuntime(unpack_returned_tuples=True)
        harness = HARNESS.replace('    local quest = {}', '''    local quest = {}
    function quest:ClearThingHasInformation(actor) rec('ClearThingHasInformation', actor.name) end
''')
        harness = harness.replace("function resources:ThingAlive(id) assert(entries[id] == 'thing'); return true end",
            "function resources:ThingAlive(id) assert(entries[id] == 'thing'); return not ((id == 2 and scenario.womanDead) or (id == 3 and scenario.wifeDead)) end")
        return list(lua.execute(harness)(source, lua.table_from(scenario)).values())

    def test_cleanup_helpers_preserve_trace_for_cancellation_and_errors(self):
        cases = []
        for scenario in ({'hit': True}, {'talk': True}, {'talk': True, 'retryTalk': True},
                         {'talk': True, 'womanDead': True}, {'talk': True, 'womanDead': True, 'wifeDead': True},
                         {'idle': True}, {'acquire': False}):
            for check in range(1, 31):
                cases.append(dict(acquire=True, terminateAtFrame=5, terminateAtCheck=check, **
                                  {k: v for k, v in scenario.items() if k != 'acquire'}))
                if scenario.get('acquire') is False:
                    cases[-1]['acquire'] = False
            if scenario.get('hit') or scenario.get('talk'):
                for failure in ('failSpeak', 'failHealth', 'failLookup'):
                    cases.append(dict(acquire=True, terminateAtFrame=5, **scenario, **{failure: True}))
        for scenario in cases:
            with self.subTest(scenario=scenario):
                expected = self.events(self.native_candidate, scenario)
                self.assertEqual(self.events(self.before, scenario), expected)
                self.assertEqual(self.events(self.after, scenario), expected)

    def test_cleanup_destinations_are_functions_and_returns(self):
        self.assertNotIn('goto releaseAffairActors', self.after)
        self.assertNotIn('goto releaseManControl', self.after)
        self.assertNotIn('goto checkInteractions', self.after)
        self.assertIn('while true do', self.after)
        self.assertNotIn('goto ', self.after)
        self.assertIn('local function facePartnerAndFinishConversation()', self.after)
        self.assertIn('local function runInteractions()', self.after)
        self.assertIn('local function acquireAndRunInteractions()', self.after)
        self.assertGreater(self.report['actorCleanupReturns'], 10)
        with self.assertRaisesRegex(ValueError, 'structure changed'):
            structure_cleanup(self.before.replace('::releaseAffairActors::', '::unknown::'))


if __name__ == '__main__':
    unittest.main()

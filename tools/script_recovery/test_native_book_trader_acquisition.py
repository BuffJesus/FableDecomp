import copy
import unittest
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_book_trader_termination as fixtures
from tools.script_recovery.native_book_trader_termination import recover_book_trader_termination
from tools.script_recovery.native_book_trader_acquisition import recover_book_trader_acquisition
from tools.script_recovery.native_post_attack_resources import map_book_trader_resources
from tools.script_recovery.lift_native_lua import Lifter


class BookTraderAcquisitionTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.BookTraderTerminationTests().inputs()
        source, _ = recover_book_trader_termination(fn, source, data)
        return fn, source, data, manifest

    def test_initial_and_retry_use_native_priority_three(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_book_trader_acquisition(fn, source, map_book_trader_resources(fn, data))
        self.assertEqual(evidence[0]['status'], 'recovered')
        assignments = [line.strip() for line in evidence[0]['new'].splitlines() if 'cVar5 = GSI->StartScriptingEntity' in line]
        self.assertEqual(len(assignments), 2)
        body = '{\n' + assignments[0] + '\nwhile (cVar5 == \'\\0\') {\nGSI->NewScriptFrame();\n' + assignments[1] + '\n}\nreturn cVar5;\n}'
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        emitted = '\n'.join(lifter.lift('Acquire', body))
        self.assertEqual(lifter.todo, ['collapse the StartScriptingEntity retry loop around AcquireControl'] * 2)
        lua, events, answers = LuaRuntime(), [], [False, False, True]
        def acquire(me, priority):
            events.append(('acquire', priority))
            return answers.pop(0)
        me = lua.table_from({'AcquireControl': acquire})
        quest = lua.table_from({'NewScriptFrame': lambda q, *args: events.append(('frame',)) or True})
        self.assertTrue(lua.execute('return function(quest,me)\n' + emitted + '\nend')(quest, me))
        self.assertEqual(events, [('acquire', 3), ('frame',), ('acquire', 3), ('frame',), ('acquire', 3)])
        self.assertEqual(result.count('StartScriptingEntity(pCVar1,&ppuStack_124,3)'), 2)

    def test_missing_or_changed_resource_proof_rejects(self):
        fn, source, data, manifest = self.inputs()
        changed = copy.deepcopy(map_book_trader_resources(fn, data))
        event = next(e for e in changed[0]['events'] if e['site'] == '0xdb4078')
        event['setup']['stack_arguments'][2][1] = 4
        for evidence in ([], changed):
            result, status = recover_book_trader_acquisition(fn, source, evidence)
            self.assertEqual(result, source)
            self.assertEqual(status[0]['status'], 'rejected')

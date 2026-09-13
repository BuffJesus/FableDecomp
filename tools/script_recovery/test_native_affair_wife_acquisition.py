import unittest
from collections import Counter
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_allies as fixtures
from tools.script_recovery.native_book_trader_allies import recover_affair_wife_allies
from tools.script_recovery.native_affair_wife_acquisition import recover_affair_wife_acquisition, CALL
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class AffairWifeAcquisitionTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeAlliesTests().inputs()
        source, _ = recover_affair_wife_allies(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_all_five_acquisition_pairs_keep_actor_priority_and_retry(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_acquisition(fn, source, data)
        self.assertEqual(evidence[0]['status'], 'recovered')
        events = evidence[0]['resourceEvidence']['events']
        self.assertEqual(Counter(e['name'] for e in events), {'acquire': 10, 'has_resource': 5, 'reset': 5, 'destructor': 6, 'task_query': 14})
        self.assertEqual(CALL.sub('CALL;', source), CALL.sub('CALL;', result))
        calls = CALL.findall(result)
        for index, priority in enumerate((3, 4, 4, 4, 4)):
            snippet = '{\ncVar5 = ' + calls[2*index] + '\nwhile (cVar5 == \'\\0\') {\nGSI->NewScriptFrame();\ncVar5 = ' + calls[2*index+1] + '\n}\nreturn cVar5;\n}'
            lifter = Lifter(manifest, {}, 'quest', True, '', data)
            body = '\n'.join(lifter.lift('Acquire', snippet))
            self.assertEqual(lifter.todo, ['collapse the StartScriptingEntity retry loop around AcquireControl'] * 2)
            lua, trace, answers = LuaRuntime(), [], [False, False, True]
            def acquire(me, value):
                trace.append(('acquire', value))
                return answers.pop(0)
            me = lua.table_from({'AcquireControl': acquire})
            quest = lua.table_from({'NewScriptFrame': lambda q, *args: trace.append(('frame',)) or True})
            self.assertTrue(lua.execute('return function(quest,me)\n' + body + '\nend')(quest, me))
            self.assertEqual(trace, [('acquire', priority), ('frame',), ('acquire', priority), ('frame',), ('acquire', priority)])

    def test_changed_resource_slot_rejects(self):
        fn, source, data, _ = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, ecx=('stack', 20)) if args[3] == 0xDB3808 else setup
        with patch('tools.script_recovery.native_post_attack_resources.read_call_window', side_effect=changed):
            result, evidence = recover_affair_wife_acquisition(fn, source, data)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

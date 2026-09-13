import re
import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_termination as fixtures
from tools.script_recovery.native_book_trader_termination import recover_affair_wife_termination
from tools.script_recovery.native_post_attack_resources import map_affair_wife_resources
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter, load_slots, load_thing_tables, thing_signatures


class AffairWifeTaskQueryTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeTerminationTests().inputs()
        source, _ = recover_affair_wife_termination(fn, source, data)
        return fn, source, data, manifest

    def test_all_native_resource_queries_use_self_and_preserve_boolean_result(self):
        fn, source, data, manifest = self.inputs()
        evidence = map_affair_wife_resources(fn, data)
        self.assertEqual(evidence[0]['status'], 'mapped')
        queries = [e for e in evidence[0]['events'] if e['name'] == 'task_query']
        calls = re.findall(r'CScriptThing::\s*_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase__UBE_NXZ\s*\([^;]+;', source)
        self.assertEqual(len(queries), 14)
        self.assertEqual(len(calls), 14)
        things, _ = load_thing_tables(manifest, load_slots())
        for call in calls:
            lifter = Lifter(manifest, {}, 'quest', True, '', data, thing_sigs=thing_signatures(things))
            body = '\n'.join(lifter.lift('Query', '{\nuVar9 = GSI->GetHero();\nbVar4 = ' + call + '\nreturn bVar4;\n}'))
            self.assertEqual(lifter.todo, [])
            lua = LuaRuntime()
            run = lua.execute('return function(quest,me)\n' + body + '\nend')
            for answer in (False, True):
                events = []
                me = lua.table_from({'IsPerformingScriptTask': lambda actor: events.append('wife') or answer})
                hero = lua.table_from({'IsPerformingScriptTask': lambda actor: self.fail('wrong actor')})
                quest = lua.table_from({'GetHero': lambda q: hero})
                self.assertEqual(run(quest, me), answer)
                self.assertEqual(events, ['wife'])

    def test_changed_query_resource_rejects_mapping(self):
        fn, _, data, _ = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, ecx=('stack', 48)) if args[3] == 0xDB3C8B else setup
        with patch('tools.script_recovery.native_post_attack_resources.read_call_window', side_effect=changed):
            evidence = map_affair_wife_resources(fn, data)
        self.assertEqual(evidence[0]['status'], 'rejected')

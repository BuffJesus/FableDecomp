import re
import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_reply as fixtures
from tools.script_recovery.native_affair_wife_reply import recover_affair_wife_reply
from tools.script_recovery.native_book_trader_health import recover_affair_wife_health
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class AffairWifeHealthTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeReplyTests().inputs()
        source, _ = recover_affair_wife_reply(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_all_six_queries_use_wife_despite_staged_hero(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_health(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertNotIn('_DAT_0122dedc', result)
        self.assertEqual(result.count('fVar3 = (float10)0.0;'), 6)
        statements = re.findall(r'fVar22 = \(float10\)GSI->GetHealth\(me\);', result)
        self.assertEqual(len(statements), 6)
        lua, actors = LuaRuntime(), []
        for statement in statements:
            lifter = Lifter(manifest, {}, 'quest', True, '', data)
            body = '\n'.join(lifter.lift('Health', '{\nuVar11 = GSI->GetHero();\n' + statement + '\nfVar3 = (float10)0.0;\nreturn fVar3 < fVar22;\n}'))
            self.assertEqual(lifter.todo, [])
            run = lua.execute('return function(quest,me)\n' + body + '\nend')
            for health in (-1, 0, 0.25, 1, 100):
                quest = lua.table_from({'GetHero': lambda q: 'hero',
                    'GetHealth': lambda q, actor: actors.append(actor) or health})
                self.assertEqual(run(quest, 'wife'), health > 0)
        self.assertEqual(actors, ['wife'] * 30)

    def test_wrong_resource_and_consumer_reject(self):
        fn, source, data, manifest = self.inputs()
        for site in (0xdb2ded, 0xdb2df6, 0xdb3a7c, 0xdb3a85):
            def changed(*args, **kwargs):
                setup = read_call_window(*args, **kwargs)
                return replace(setup, stack_arguments=(('constant', 0),)) if args[3] == site else setup
            with patch('tools.script_recovery.native_book_trader_health.read_call_window', side_effect=changed):
                result, evidence = recover_affair_wife_health(fn, source, data, manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')

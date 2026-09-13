import re
import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_affair_wife_distance as fixtures
from tools.script_recovery.native_teddy_distance import recover_affair_wife_distance
from tools.script_recovery.native_hero_distance import recover_affair_wife_hero_distances
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class AffairWifeHeroDistanceTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeDistanceTests().inputs()
        source, _ = recover_affair_wife_distance(fn, source, data)
        return fn, source, data, manifest

    def test_distinct_thresholds_and_fresh_hero_queries(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_hero_distances(fn, source, data)
        self.assertEqual(evidence[0]['status'], 'recovered')
        statements = []
        for local in ('pCVar8', 'pCVar9'):
            statements.append(re.search(local + r' = \(CScriptThing \*\)GSI->GetHero\(\);', result)[0])
            statements.append(re.search(r'bVar4 = IsDistanceBetweenThingsUnder\(pCVar1,' + local + r',[^;]+;', result)[0])
        snippet = '{\npCVar1 = (CScriptThing *)(param_1 + 8);\nfVar33 = 99.0;\n' + '\n'.join(statements) + '\n}'
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('HeroChecks', snippet))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        heroes = iter(('hero first', 'hero later'))
        def hero(q):
            value = next(heroes)
            events.append(('hero', value))
            return value
        quest = lua.table_from({'GetHero': hero,
            'IsDistanceBetweenThingsUnder': lambda q, actor, target, threshold:
                events.append(('distance', actor, target, threshold)) or True})
        lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'wife')
        self.assertEqual(events, [('hero', 'hero first'), ('distance', 'wife', 'hero first', 5.0),
                                 ('hero', 'hero later'), ('distance', 'wife', 'hero later', 15.0)])

    def test_changed_second_threshold_or_actor_rejects_atomically(self):
        fn, source, data, _ = self.inputs()
        for change in ({'stack_arguments': (('constant', 0x40A00000),)},
                       {'edx': ('stack', 48)}):
            with self.subTest(change=change):
                def changed(*args, **kwargs):
                    setup = read_call_window(*args, **kwargs)
                    return replace(setup, **change) if args[3] == 0xDB35E4 else setup
                with patch('tools.script_recovery.native_hero_distance.read_call_window', side_effect=changed):
                    result, evidence = recover_affair_wife_hero_distances(fn, source, data)
                self.assertEqual(result, source)
                self.assertEqual(evidence[0]['status'], 'rejected')

import re
import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_affair_wife_route as fixtures
from tools.script_recovery.native_affair_wife_route import recover_affair_wife_route
from tools.script_recovery.native_teddy_distance import recover_affair_wife_distance
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class AffairWifeDistanceTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeRouteTests().inputs()
        source, _ = recover_affair_wife_route(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_checks_share_cached_husband_and_native_threshold(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_distance(fn, source, data)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual(len(evidence[0]['distanceSetups']), 2)
        lookup = re.search(r'GSI->GetThingWithScriptName\(&local_native_cached_husband[^;]+;', result)[0]
        distance = 'IsDistanceBetweenThingsUnder(pCVar1,local_native_cached_husband,3.0)'
        self.assertEqual(result.count(distance), 2)
        snippet = ('{\npCVar1 = (CScriptThing *)(param_1 + 8);\n' + lookup +
                   '\nbVar4 = ' + distance + ';\nif (!bVar4) {\nGSI->NewScriptFrame();\n'
                   'bVar4 = ' + distance + ';\n}\nreturn bVar4;\n}')
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('Distance', snippet))
        self.assertEqual(lifter.todo, [])
        lua = LuaRuntime()
        for answers in ([True], [False, True], [False, False]):
            pending, events = list(answers), []
            def check(q, actor, target, threshold):
                events.append(('distance', actor, target, threshold))
                return pending.pop(0)
            quest = lua.table_from({'GetThingWithScriptName': lambda q, name: events.append(('lookup', name)) or 'husband',
                'IsDistanceBetweenThingsUnder': check,
                'NewScriptFrame': lambda q, *args: events.append(('frame',)) or True})
            value = lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'wife')
            self.assertEqual(value, answers[-1])
            expected = [('lookup', 'NOVI_AffairMan'), ('distance', 'wife', 'husband', 3.0)]
            if not answers[0]:
                expected += [('frame',), ('distance', 'wife', 'husband', 3.0)]
            self.assertEqual(events, expected)

    def test_wrong_cached_slot_rejects_both_checks(self):
        fn, source, data, manifest = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, edx=('stack', 52)) if args[3] == 0xDB3586 else setup
        with patch('tools.script_recovery.native_teddy_distance.read_call_window', side_effect=changed):
            result, evidence = recover_affair_wife_distance(fn, source, data)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

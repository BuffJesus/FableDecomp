import re
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_loop_timer as fixtures
from tools.script_recovery.native_loop_timer import recover_loop_timer
from tools.script_recovery.native_teddy_distance import recover_barrel_distance
from tools.script_recovery.lift_native_lua import Lifter, RData, strip_declarations
from tools.script_recovery.native_flow_ir import build_flow_graph
from tools.script_recovery.native_call_effects import ScalarCallEffects, ProducedValue
from tools.script_recovery.native_value_ir import NativeValues


class BarrelDistanceTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.LoopTimerTests().inputs()
        source = recover_loop_timer(fn, source, RData(), {'0x108': ('WatchTimer', 'Int')})[0]
        return fn, source, manifest

    def test_cached_guard_point_and_current_hero_reach_each_distance_query(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_barrel_distance(fn, source, RData())
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual(evidence[0]['lookupSetup']['stack_arguments'][0], ('stack', 0x20))
        self.assertEqual(evidence[0]['distanceSetup']['edx'], ('stack', 0x20))
        lookup = re.search(r'GSI->GetThingWithScriptName\(&local_native_guard_point[^;]+;', recovered)[0]
        sequence = re.search(r'fVar7 = 2.0;\s*pCVar4 = \(CScriptThing \*\)GSI->GetHero\(\);\s*'
                             r'bVar2 = IsDistanceBetweenThingsUnder\(pCVar4,local_native_guard_point,fVar7\);', recovered)[0]
        native = ('{\n' + lookup + '\niVar3 = 0;\nwhile (iVar3 < 2) {\n' + sequence +
                  '\nGSI->NewScriptFrame();\niVar3 = iVar3 + 1;\n}\nreturn bVar2;\n}')
        lifter = Lifter(manifest, {}, 'quest', False, '', RData())
        body = '\n'.join(lifter.lift('Distance', native))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        heroes = iter(('hero1', 'hero2'))
        def lookup_thing(_q, name):
            events.append(('lookup', name))
            return 'guard-point'
        def hero(_q):
            value = next(heroes)
            events.append(('hero', value))
            return value
        def distance(_q, left, right, threshold):
            events.append(('distance', left, right, threshold))
            return left == 'hero1'
        def frame(_q):
            events.append(('frame',))
            return True
        quest = lua.table_from({'GetThingWithScriptName': lookup_thing, 'GetHero': hero,
                               'IsDistanceBetweenThingsUnder': distance, 'NewScriptFrame': frame})
        self.assertFalse(lua.execute('return function(quest)\n' + body + '\nend')(quest))
        self.assertEqual(events, [('lookup', 'M_WHouse_GuardPoint'),
                                  ('hero', 'hero1'), ('distance', 'hero1', 'guard-point', 2.0), ('frame',),
                                  ('hero', 'hero2'), ('distance', 'hero2', 'guard-point', 2.0), ('frame',)])

    def test_whole_native_function_preserves_cached_output_to_loop_use(self):
        fn, source, manifest = self.inputs()
        recovered = recover_barrel_distance(fn, source, RData())[0]
        graph = build_flow_graph(strip_declarations(recovered))
        self.assertIsNotNone(graph)
        facts = NativeValues(graph, ScalarCallEffects(manifest, fn['calls']))
        producer = next(n.index for n in graph.statements if 'GetThingWithScriptName(&local_native_guard_point' in n.source)
        use = next(n.index for n in graph.statements if 'IsDistanceBetweenThingsUnder(pCVar4,local_native_guard_point' in n.source)
        self.assertEqual(facts.at(use, 'local_native_guard_point'),
                         ProducedValue('thing', producer, 'GetThingWithScriptName'))

    def test_changed_source_literal_or_bytes_reject(self):
        fn, source, _ = self.inputs()
        data = RData()
        for changed in (SimpleNamespace(bytes_at=lambda *_: None, string_at=data.string_at),
                        SimpleNamespace(bytes_at=data.bytes_at, string_at=lambda *_: 'OTHER_MARKER')):
            self.assertEqual(recover_barrel_distance(fn, source, changed)[1][0]['status'], 'rejected')
        self.assertEqual(recover_barrel_distance(fn, source.replace('fVar7 = 2.0;', 'fVar7 = 3.0;'), data)[1][0]['status'],
                         'rejected')

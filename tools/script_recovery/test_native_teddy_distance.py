import re
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_presented_items import recover_presented_item_comparison
from tools.script_recovery.native_teddy_distance import recover_teddy_distance, recover_theresa_distance
from tools.script_recovery import test_native_presented_items as fixtures
from tools.script_recovery.lift_native_lua import Lifter, RData, thing_signatures
from tools.script_recovery.lift_native_lua import strip_declarations
from tools.script_recovery.native_flow_ir import build_flow_graph
from tools.script_recovery.native_value_ir import NativeValues
from tools.script_recovery.native_call_effects import ScalarCallEffects, ProducedValue


class TeddyDistanceTests(unittest.TestCase):
    def test_shared_analysis_tracks_both_actual_cached_outputs_to_distance_uses(self):
        for address, recover, slot in (
                ('0x00DAF080', recover_teddy_distance, 'local_native_cached_bully'),
                ('0x00DB97A0', recover_theresa_distance, 'local_native_outro_marker')):
            fn, source, manifest, _ = fixtures.PresentedItemComparisonTests().inputs(address)
            source = recover_presented_item_comparison(fn, source, RData())[0]
            source = recover(fn, source, RData())[0]
            graph = build_flow_graph(strip_declarations(source))
            facts = NativeValues(graph, ScalarCallEffects(manifest, fn['calls']))
            producer = next(n.index for n in graph.statements if 'GetThingWithScriptName(&' + slot in n.source)
            use = next(n.index for n in graph.statements if 'IsDistanceBetweenThingsUnder(' in n.source and slot in n.source)
            self.assertEqual(facts.at(use, slot), ProducedValue('thing', producer, 'GetThingWithScriptName'))

    def test_theresa_uses_cached_outro_marker_and_current_hero(self):
        fn, source, manifest, things = fixtures.PresentedItemComparisonTests().inputs('0x00DB97A0')
        data = RData()
        source = recover_presented_item_comparison(fn, source, data)[0]
        recovered, evidence = recover_theresa_distance(fn, source, data)
        self.assertEqual(evidence[0]['status'], 'recovered')
        lookup = re.search(r'GSI->GetThingWithScriptName\(&local_native_outro_marker[^;]+;', recovered)[0]
        call_sequence = re.search(r'fVar26 = 2.0;\s*pCVar13 = \(CScriptThing \*\)GSI->GetHero\(\);\s*'
                                  r'bVar4 = IsDistanceBetweenThingsUnder\(pCVar13,local_native_outro_marker,fVar26\);', recovered)[0]
        lifter = Lifter(manifest, {}, 'quest', True, '', data, thing_sigs=thing_signatures(things))
        body = '\n'.join(lifter.lift('Main', '{\n' + lookup + '\n' + call_sequence + '\nreturn bVar4;\n}'))
        self.assertEqual(lifter.todo, [])
        for expected in (False, True):
            lua, events = LuaRuntime(), []
            def lookup(_q, name):
                events.append(('lookup', name))
                return 'marker'
            def hero(_q):
                events.append(('hero',))
                return 'hero'
            def distance(_q, left, right, threshold):
                events.append(('distance', left, right, threshold))
                return expected
            quest = lua.table_from({'GetThingWithScriptName': lookup, 'GetHero': hero,
                                   'IsDistanceBetweenThingsUnder': distance})
            self.assertEqual(lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'theresa'), expected)
            self.assertEqual(events, [('lookup', 'M_TriggerOutro'), ('hero',), ('distance', 'hero', 'marker', 2.0)])
        altered = source.replace('fVar26 = 2.0;', 'fVar26 = 3.0;')
        self.assertEqual(recover_theresa_distance(fn, altered, data)[1][0]['status'], 'rejected')
        changed_bytes = SimpleNamespace(bytes_at=lambda *_: None, string_at=data.string_at)
        self.assertEqual(recover_theresa_distance(fn, source, changed_bytes)[1][0]['status'], 'rejected')

    def inputs(self):
        fn, source, manifest, things = fixtures.PresentedItemComparisonTests().inputs('0x00DAF080')
        source = recover_presented_item_comparison(fn, source, RData())[0]
        return fn, source, manifest, things

    def test_native_lookup_result_and_ten_unit_distance_survive_lifting(self):
        fn, source, manifest, things = self.inputs()
        recovered, evidence = recover_teddy_distance(fn, source, RData())
        self.assertEqual(evidence[0]['status'], 'recovered')
        lookup = re.search(r'GSI->GetThingWithScriptName\(&local_native_cached_bully[^;]+;', recovered)[0]
        distance = re.search(r'bVar6 = IsDistanceBetweenThingsUnder\(pCVar8,local_native_cached_bully,10.0\);', recovered)[0]
        lifter = Lifter(manifest, {}, 'quest', True, '', RData(), thing_sigs=thing_signatures(things))
        body = '\n'.join(lifter.lift('Main', '{\n' + lookup +
            '\npCVar8 = (CScriptThing *)(param_1 + 8);\n' + distance + '\nreturn bVar6;\n}'))
        self.assertEqual(lifter.todo, [])
        for answer in (False, True):
            lua, events = LuaRuntime(), []
            def lookup(_q, name):
                events.append(('lookup', name))
                return 'bully_handle'
            def distance(_q, left, right, limit):
                events.append(('distance', left, right, limit))
                return answer
            quest = lua.table_from({'GetThingWithScriptName': lookup, 'IsDistanceBetweenThingsUnder': distance})
            self.assertEqual(lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'teddy_handle'), answer)
            self.assertEqual(events, [('lookup', 'NOVI_Bully'), ('distance', 'teddy_handle', 'bully_handle', 10.0)])

    def test_changed_evidence_cannot_rebind_a_stack_operand(self):
        fn, source, _, _ = self.inputs()
        data = RData()
        for altered in (source.replace('10.0', '11.0'), source + '\nchanged();'):
            self.assertEqual(recover_teddy_distance(fn, altered, data)[1][0]['status'], 'rejected')
        for altered in (SimpleNamespace(bytes_at=lambda *_: None, string_at=data.string_at),
                        SimpleNamespace(bytes_at=data.bytes_at, string_at=lambda _: 'OTHER')):
            unchanged, evidence = recover_teddy_distance(fn, source, altered)
            self.assertEqual(unchanged, source)
            self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()

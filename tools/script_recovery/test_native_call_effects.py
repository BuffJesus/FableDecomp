import unittest

from tools.script_recovery.lift_native_lua import load_manifest
from tools.script_recovery.native_call_effects import ScalarCallEffects, ProducedValue
from tools.script_recovery.native_flow_ir import build_flow_graph
from tools.script_recovery.native_value_ir import NativeValues, ScalarValue


class CallEffectsTests(unittest.TestCase):
    def test_cached_output_survives_verified_const_distance_loop(self):
        lines = ['GSI->GetThingWithScriptName(&local_bully,"NOVI_Bully");',
                 'while (running) {', 'bVar1 = IsDistanceBetweenThingsUnder(me,local_bully,10.0);', '}']
        graph = build_flow_graph(lines)
        calls = [{'currentName': 'IsDistanceBetweenThingsUnder', 'target': '0x00CBE2FF'}]
        self.assertEqual(NativeValues(graph, ScalarCallEffects(load_manifest(), calls)).at(2, 'local_bully'),
                         ProducedValue('thing', 0, 'GetThingWithScriptName'))
        for unknown in ([], calls + [{'currentName': 'IsDistanceBetweenThingsUnder', 'target': '0x12345678'}]):
            self.assertIsNone(NativeValues(graph, ScalarCallEffects(load_manifest(), unknown)).at(2, 'local_bully'))
    def test_explicit_entity_output_slot_is_a_value_definition_not_an_escape(self):
        graph = build_flow_graph(['GSI->GetThingWithScriptName(&local_bully,"NOVI_Bully");',
                                  'pCVar1 = local_bully;', 'use(pCVar1);'])
        facts = NativeValues(graph, ScalarCallEffects(load_manifest()))
        self.assertEqual(facts.at(2, 'pCVar1'), ProducedValue('thing', 0, 'GetThingWithScriptName'))

    def test_other_address_use_still_invalidates_output_slot(self):
        graph = build_flow_graph(['GSI->GetThingWithScriptName(&local_bully,"NOVI_Bully");',
                                  'save(&local_bully);', 'use(local_bully);'])
        self.assertIsNone(NativeValues(graph, ScalarCallEffects(load_manifest())).at(2, 'local_bully'))

    def test_hidden_output_requires_exact_supported_contract(self):
        effects = ScalarCallEffects(load_manifest())
        for source in ('GSI->GetThingWithScriptName(&local_bully,name);',
                       'GSI->GetThingWithScriptName(&local_bully,"NOVI_Bully",extra);',
                       'Unknown(&local_bully,"NOVI_Bully");'):
            self.assertEqual(effects.outputs(source, 0), {})
        self.assertEqual(ScalarCallEffects({}).outputs('GSI->GetThingWithScriptName(&local_bully,"NOVI_Bully");', 0), {})
    def test_known_scalar_api_preserves_input_local_but_not_result(self):
        graph = build_flow_graph(['iVar1 = 7;', 'iVar2 = GSI->GetTimer(iVar1);', 'use(iVar1,iVar2);'])
        self.assertIsNone(NativeValues(graph).at(2, 'iVar1'))
        values = NativeValues(graph, ScalarCallEffects(load_manifest()))
        self.assertEqual(values.at(2, 'iVar1'), ScalarValue('integer', 7))
        self.assertEqual(values.at(2, 'iVar2'), ProducedValue('integer', 1, 'GetTimer'))

    def test_unknown_calls_and_hidden_outputs_remain_conservative(self):
        effects = ScalarCallEffects(load_manifest())
        self.assertTrue(effects('GSI->SetTimer(iVar1,7);'))
        for call in ('GSI->GetTimer(&iVar1);', 'GSI->GetTimer(iVar1,7);',
                     'GSI->GetTimer(other());', 'GetTimer(iVar1);',
                     'GSI->GetThingWithScriptName(iVar1);', 'GSI->Unknown(iVar1);'):
            self.assertFalse(effects(call), call)

    def test_api_knowledge_does_not_make_escaped_values_known(self):
        graph = build_flow_graph(['iVar1 = 7;', 'save(&iVar1);', 'GSI->SetTimer(iVar1,7);', 'use(iVar1);'])
        self.assertIsNone(NativeValues(graph, ScalarCallEffects(load_manifest())).at(3, 'iVar1'))

    def test_hero_copy_keeps_original_producer_when_source_is_reassigned(self):
        graph = build_flow_graph(['pCVar1 = (CScriptThing *)GSI->GetHero();',
                                  'pCVar2 = pCVar1;', 'pCVar1 = GSI->GetHero();', 'use(pCVar2);'])
        value = NativeValues(graph, ScalarCallEffects(load_manifest())).at(3, 'pCVar2')
        self.assertEqual(value, ProducedValue('thing', 0, 'GetHero'))

    def test_separate_producers_do_not_become_one_value_at_a_join(self):
        graph = build_flow_graph(['if (choose) {', 'iVar1 = GSI->RegisterTimer();', '}',
                                  'else {', 'iVar1 = GSI->RegisterTimer();', '}', 'use(iVar1);'])
        self.assertIsNone(NativeValues(graph, ScalarCallEffects(load_manifest())).at(6, 'iVar1'))

    def test_loop_producer_is_not_treated_as_one_runtime_instance(self):
        graph = build_flow_graph(['while (running) {', 'iVar1 = GSI->RegisterTimer();', 'use(iVar1);', '}'])
        self.assertIsNone(NativeValues(graph, ScalarCallEffects(load_manifest())).at(2, 'iVar1'))

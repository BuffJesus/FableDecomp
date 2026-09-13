import json
import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import (ROOT, RData, Lifter, load_manifest,
    load_slots, load_thing_tables, annotate, fold_self_wrapper_arguments)
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_affair_wife_route import recover_affair_wife_route


class AffairWifeRouteTests(unittest.TestCase):
    def inputs(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DB2B10')
        manifest, slots, data = load_manifest(), load_slots(), RData()
        things, returning = load_thing_tables(manifest, slots)
        source = annotate(rename_parameters(fn['decompile'], function_parameters(fn['decompile'], member=True)),
                          slots, things, returning, entity=True)
        return fn, fold_self_wrapper_arguments(source)[0], data, manifest

    def test_route_flags_apply_to_wife_not_staged_husband(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_route(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual(evidence[0]['positionLowering'], 'unresolved')
        snippet = '{\nuVar9 = GSI->GetHero();\n' + '\n'.join(e['new'] for e in evidence[0]['edits']) + '\n}'
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('RouteFlags', snippet))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        quest = lua.table_from({'GetHero': lambda q: 'unrelated staged actor',
            'EntitySetAsUseMovementInActions': lambda q, actor, flag: events.append(('movement', actor, flag)),
            'ClearThingHasInformation': lambda q, actor: events.append(('clear', actor))})
        lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'wife')
        self.assertEqual(events, [('movement', 'wife', True), ('clear', 'wife')])
        self.assertIn('DAT_0143e8e0', result)

    def test_changed_self_operand_rejects_both_edits(self):
        fn, source, data, manifest = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, stack_arguments=(('register', 'ebp'),) + setup.stack_arguments[1:])
        with patch('tools.script_recovery.native_affair_wife_route.read_call_window', side_effect=changed):
            result, evidence = recover_affair_wife_route(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

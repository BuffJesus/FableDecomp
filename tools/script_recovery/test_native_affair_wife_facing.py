import re
import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_mask as fixtures
from tools.script_recovery.native_affair_wife_mask import recover_affair_wife_mask
from tools.script_recovery.native_affair_wife_facing import recover_affair_wife_facing
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter, converter_signatures


class AffairWifeFacingTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeMaskTests().inputs()
        source, _ = recover_affair_wife_mask(fn, source, data)
        return fn, source, data, manifest

    def test_cached_husband_and_snap_flags(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_facing(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        lookup = re.search(r'GSI->GetThingWithScriptName\(&local_native_cached_husband[^;]+;', result)[0]
        calls = re.findall(r'GSI->EntitySetFacingAngleTowardsThing\([^;]+;', result)
        self.assertEqual(len(calls), 3)
        snippet = '{\npCVar1 = (CScriptThing *)(param_1 + 8);\n' + lookup + '\nuVar9 = GSI->GetHero();\n' + '\n'.join(calls) + '\n}'
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('Facing', snippet))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        quest = lua.table_from({'GetThingWithScriptName': lambda q, name: events.append(('lookup', name)) or 'husband',
            'GetHero': lambda q: 'unrelated hero',
            'EntitySetFacingAngleTowardsThing': lambda q, who, target, snap: events.append((who, target, snap))})
        lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'wife')
        self.assertEqual(events, [('lookup', 'NOVI_AffairMan'), ('wife', 'husband', False),
                                 ('wife', 'husband', True), ('wife', 'husband', True)])

    def test_changed_cached_operand_rejects(self):
        fn, source, data, manifest = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, stack_arguments=(('register', 'edi'), ('stack', 52), ('constant', 1)))
        with patch('tools.script_recovery.native_affair_wife_facing.read_call_window', side_effect=changed):
            result, evidence = recover_affair_wife_facing(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

    def test_signature_preserves_newer_contract_and_does_not_mutate_input(self):
        spec = {'scope': 'Quest', 'returnType': 'void', 'parameters': [
            {'name': 'pThing', 'type': 'CScriptThing*'}, {'name': 'pTarget', 'type': 'CScriptThing*'}]}
        manifest = {'EntitySetFacingAngleTowardsThing': spec}
        updated = converter_signatures(manifest)
        self.assertEqual(len(spec['parameters']), 2)
        self.assertEqual(updated['EntitySetFacingAngleTowardsThing']['parameters'][-1]['name'], 'snap')
        self.assertEqual(converter_signatures(updated), updated)

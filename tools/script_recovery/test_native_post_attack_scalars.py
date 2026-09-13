import copy
import unittest
from dataclasses import replace
from types import SimpleNamespace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_post_attack_cleanup as fixtures
from tools.script_recovery.lift_native_lua import Lifter, RData, strip_declarations
from tools.script_recovery.native_post_attack_cleanup import recover_post_attack_cleanup
from tools.script_recovery.native_post_attack_scalars import recover_post_attack_scalars
from tools.script_recovery.native_call_setup_ir import read_call_window


class PostAttackScalarTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest, _ = fixtures.PostAttackCleanupTests().inputs()
        return fn, recover_post_attack_cleanup(fn, source, RData())[0], manifest

    def test_actual_native_scalar_calls_emit_typed_values_in_order(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_post_attack_scalars(fn, source, RData(), manifest)
        self.assertEqual([e['value'] for e in evidence], [45, 0.0, 57])
        self.assertTrue(all(e['status'] == 'recovered' for e in evidence))
        calls = [s.strip() for s in strip_declarations(recovered)
                 if s.strip().startswith(('GSI->CacheMusicSet(', 'GSI->CameraResetToViewBehindHero('))]
        lifter = Lifter(manifest, {}, 'quest', False, '', RData())
        body = '\n'.join(lifter.lift('Calls', '{\n' + '\n'.join(calls) + '\n}'))
        self.assertEqual(lifter.todo, [])
        self.assertIn('CameraResetToViewBehindHero(0.0)', body)
        lua, events = LuaRuntime(), []
        quest = lua.table_from({
            'CacheMusicSet': lambda _q, value: events.append(('music', value)),
            'CameraResetToViewBehindHero': lambda _q, value: events.append(('camera', value)),
        })
        lua.execute('return function(quest)\n' + body + '\nend')(quest)
        self.assertEqual(events, [('music', 45), ('camera', 0.0), ('music', 57)])

    def test_changed_bytes_source_contract_and_vtable_fail_closed(self):
        fn, source, manifest = self.inputs()
        data = RData()
        for address_to_change in (0xDBEB20, 0x1260F0C + 0xAE8, 0x1260F0C + 0x684):
            def read(address, size):
                raw = data.bytes_at(address, size)
                return bytes([raw[0] ^ 1]) + raw[1:] if address == address_to_change else raw
            result, evidence = recover_post_attack_scalars(fn, source, SimpleNamespace(bytes_at=read), manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')
        for name in ('CacheMusicSet', 'CameraResetToViewBehindHero'):
            changed = copy.deepcopy(manifest)
            changed[name]['parameters'][0]['type'] = 'bool'
            self.assertEqual(recover_post_attack_scalars(fn, source, data, changed)[0], source)
        self.assertEqual(recover_post_attack_scalars(fn, source + '\n', data, manifest)[1][0]['status'], 'rejected')

    def test_decoder_operand_or_receiver_disagreement_is_not_ignored(self):
        fn, source, manifest = self.inputs()
        for field, value in (('stack_arguments', (('constant', 999),)),
                             ('ecx', ('unknown',)), ('target', ('constant', 0))):
            def decode(*args, **kwargs):
                return replace(read_call_window(*args, **kwargs), **{field: value})
            with patch('tools.script_recovery.native_post_attack_scalars.read_call_window', side_effect=decode):
                result, evidence = recover_post_attack_scalars(fn, source, RData(), manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()

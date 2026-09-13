import unittest
from dataclasses import replace
from types import SimpleNamespace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_post_attack_logbook as fixtures
from tools.script_recovery.lift_native_lua import Lifter, RData
from tools.script_recovery.native_post_attack_logbook import recover_post_attack_logbook
from tools.script_recovery.native_post_attack_teleport import recover_post_attack_teleport
from tools.script_recovery.native_call_setup_ir import read_call_window


class PostAttackTeleportTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.PostAttackLogbookTests().inputs()
        source = recover_post_attack_logbook(fn, source, RData(), manifest)[0]
        return fn, source, Lifter(manifest, {}, 'quest', False, '', RData()).manifest

    def test_actual_teleport_block_looks_up_target_then_hero_and_passes_false(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_post_attack_teleport(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        first = recovered.rindex('CCharString::CCharString', 0, recovered.index('    CVar4 = GSI->'))
        last = recovered.index('    C3DClothPrimitive::~', first)
        lifter = Lifter(manifest, {}, 'quest', False, '', RData())
        body = '\n'.join(lifter.lift('Teleport', '{\n' + recovered[first:last] + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        def lookup(_q, name):
            events.append(('lookup', name))
            return 'target'
        def hero(_q):
            events.append(('hero',))
            return 'hero'
        quest = lua.table_from({'GetThingWithScriptName': lookup, 'GetHero': hero,
            'EntityTeleportToThing': lambda _q, who, target, flag: events.append(('teleport', who, target, flag))})
        lua.execute('return function(quest)\n' + body + '\nend')(quest)
        self.assertEqual(events, [('lookup', 'M_PostAttackStart'), ('hero',), ('teleport', 'hero', 'target', False)])

    def test_changed_callees_or_source_cannot_discard_staging(self):
        fn, source, manifest = self.inputs()
        data = RData()
        for changed_address in (0xDBEB20, 0x8A7D60, 0x891CA0, 0x88E540):
            def read(address, size):
                raw = data.bytes_at(address, size)
                return bytes([raw[0] ^ 1]) + raw[1:] if address == changed_address else raw
            result, evidence = recover_post_attack_teleport(fn, source, SimpleNamespace(bytes_at=read), manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')

        def swapped(*args, **kwargs):
            result = read_call_window(*args, **kwargs)
            if args[3] == 0xDBEC2B:
                first, second, flag = result.stack_arguments
                return replace(result, stack_arguments=(second, first, flag))
            return result
        with patch('tools.script_recovery.native_post_attack_teleport.read_call_window', side_effect=swapped):
            self.assertEqual(recover_post_attack_teleport(fn, source, RData(), manifest)[1][0]['status'], 'rejected')
        self.assertEqual(recover_post_attack_teleport(fn, source + '\n', data, manifest)[1][0]['status'], 'rejected')

    def test_intervening_call_targets_and_final_operand_order_are_verified(self):
        fn, source, manifest = self.inputs()
        for site in (0xDBEC16, 0xDBEC21, 0xDBEC2B):
            def decode(*args, **kwargs):
                result = read_call_window(*args, **kwargs)
                return replace(result, target=('constant', 0)) if args[3] == site else result
            with patch('tools.script_recovery.native_post_attack_teleport.read_call_window', side_effect=decode):
                result, evidence = recover_post_attack_teleport(fn, source, RData(), manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()

import unittest

from tools.script_recovery.native_register_origins import register_origins
from tools.script_recovery.test_native_call_setup_ir import decode
from tools.script_recovery.lift_native_lua import RData


class RegisterOriginTests(unittest.TestCase):
    def test_copy_survives_loop_only_with_explicit_call_preservation(self):
        instructions = decode(bytes.fromhex('89ce e800000000 75f9 c3'))
        facts = register_origins(instructions, {0x1002: ('esi',)})
        self.assertEqual(facts[0x1007]['esi'], 'ecx')
        self.assertIsNone(facts[0x1007]['ecx'])
        self.assertIsNone(register_origins(instructions)[0x1007]['esi'])

    def test_disagreeing_branches_do_not_choose_one_origin(self):
        instructions = decode(bytes.fromhex('89ce 7402 89de 90 c3'))
        self.assertIsNone(register_origins(instructions)[0x1006]['esi'])

    def test_partial_register_write_and_stack_reload_lose_origin(self):
        for raw in ('89ce 66be0100 90 c3', '89ce 568b3424 90 c3'):
            instructions = decode(bytes.fromhex(raw))
            nop = next(ins for ins in instructions if ins.mnemonic == 'nop')
            self.assertIsNone(register_origins(instructions)[nop.address]['esi'])

    def test_unreachable_copy_does_not_change_origin(self):
        instructions = decode(bytes.fromhex('89ce eb02 89de 90 c3'))
        facts = register_origins(instructions)
        self.assertIsNone(facts[0x1004])
        self.assertEqual(facts[0x1006]['esi'], 'ecx')

    def test_unsupported_control_and_misaligned_targets_reject(self):
        for raw in ('ebff', 'ffe0', 'cd80', '0f34', '0f0b'):
            self.assertIsNone(register_origins(decode(bytes.fromhex(raw))))

    def test_actual_barrel_loop_keeps_quest_in_esi_with_reviewed_call_abi(self):
        instructions = decode(RData().bytes_at(0xDB4F70, 562), 0xDB4F70)
        # GetTimer, NewScriptFrame and IsActiveThreadTerminating are the three
        # calls on paths reaching the second timer query. ESI is callee-saved
        # under their native MSVC x86 ABI; no other call gets that assumption.
        preserved = {site: ('esi',) for site in (0xDB4F85, 0xDB4F95, 0xDB4F9A, 0xDB4FB6)}
        facts = register_origins(instructions, preserved)
        self.assertEqual(facts[0xDB4FA7]['esi'], 'ecx')
        self.assertEqual(facts[0xDB4FB6]['esi'], 'ecx')
        self.assertIsNone(register_origins(instructions)[0xDB4FB6]['esi'])

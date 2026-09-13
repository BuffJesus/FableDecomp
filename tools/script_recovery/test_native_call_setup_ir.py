import unittest
from types import SimpleNamespace

from capstone import Cs, CS_ARCH_X86, CS_MODE_32

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_call_setup_ir import call_setup, read_call_window


def decode(raw, address=0x1000):
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    return list(decoder.disasm(raw, address))


class CallSetupTests(unittest.TestCase):
    def test_zeroed_low_byte_load_preserves_dynamic_unsigned_byte(self):
        for prefix,load,push in (('31c9','8a0d48573701','51'),('31d2','8a1548573701','52')):
            setup=call_setup(decode(bytes.fromhex(prefix+load+push+'e800000000')),argument_count=1)
            self.assertEqual(setup.stack_arguments,(('unsigned_byte',('memory',('constant',0x1375748))),))
        # Unknown upper bytes, high-byte writes and wider loads are not this idiom.
        for raw in ('8a0d4857370151','31c98a2d4857370151','31c9668b0d4857370151'):
            self.assertIsNone(call_setup(decode(bytes.fromhex(raw+'e800000000')),argument_count=1))

    def test_memory_write_invalidates_unsigned_byte_fact(self):
        setup=call_setup(decode(bytes.fromhex('31c98a0d48573701c605485737010051e800000000')),argument_count=1)
        self.assertEqual(setup.stack_arguments,(('unknown',),))

    def test_full_register_xor_zeroing_and_partial_write_limits(self):
        for code, expected in (('31c9', ('constant', 0)), ('30c9', ('unknown',)),
                               ('6631c9', ('unknown',)), ('31d1', ('unknown',))):
            setup = call_setup(decode(bytes.fromhex(code + ' e800000000')))
            self.assertEqual(setup.ecx, expected)
        self.assertIsNone(call_setup(decode(bytes.fromhex('31e4 e800000000'))))

    def test_immediate_arithmetic_preserves_addresses_and_wraps_constants(self):
        for raw, expected in (
                ('8b4614 0504010000 8b08', ('memory', ('address', ('memory', ('address', ('register', 'esi'), 20)), 260))),
                ('8d4e20 83e908', ('address', ('register', 'esi'), 24)),
                ('b9ffffffff 83c102', ('constant', 1)),
                ('b900000000 83e901', ('constant', 0xffffffff)),
                ('b001 83c004 89c1', ('unknown',))):
            with self.subTest(raw=raw):
                setup = call_setup(decode(bytes.fromhex(raw + ' e800000000')))
                self.assertEqual(setup.ecx, expected)

    def test_reviewed_arity_separates_arguments_from_frame_and_saved_registers(self):
        raw = bytes.fromhex('83ec20 56 89ce 8b9608010000 52 e800000000')
        instructions = decode(raw)
        self.assertIsNone(call_setup(instructions))
        setup = call_setup(instructions, argument_count=1)
        self.assertEqual(setup.stack_arguments,
                         (('memory', ('address', ('register', 'ecx'), 0x108)),))
        self.assertIsNone(call_setup(instructions, argument_count=3))

    def test_frame_reservation_does_not_invent_argument_contents(self):
        for code in ('83ec04', '83ec02', '83ecfc'):
            self.assertIsNone(call_setup(decode(bytes.fromhex(code + ' e800000000')),
                                         argument_count=1))
        for count in (-1, True, 1.5):
            self.assertIsNone(call_setup(decode(bytes.fromhex('6a07 e800000000')),
                                         argument_count=count))

    def test_actual_first_barrel_timer_argument_is_quest_watch_timer_field(self):
        setup = read_call_window(RData(), 0xDB4F70, 562, 0xDB4F85, argument_count=1)
        self.assertIsNotNone(setup)
        self.assertEqual(setup.stack_arguments,
                         (('memory', ('address', ('register', 'ecx'), 0x108)),))
        self.assertEqual(setup.target, ('memory', ('address',
                         ('memory', ('address', ('memory', ('constant', 0x143e8f8)), 0)), 0x168)))

    def test_incoming_jump_cannot_borrow_a_register_setup_it_bypasses(self):
        # A later jump enters at the call, bypassing mov ecx,7.
        raw = bytes.fromhex('b907000000 e800000000 ebf9')
        data = SimpleNamespace(bytes_at=lambda *_: raw)
        setup = read_call_window(data, 0x1000, len(raw), 0x1005)
        self.assertEqual(setup.ecx, ('register', 'ecx'))

    def test_automatic_windows_recover_both_cached_entity_arguments(self):
        data = RData()
        for base, size, lookup, distance, known in (
                (0xDAF080, 5498, 0xDAF110, 0xDAFAB7, {}),
                (0xDB97A0, 7010, 0xDB9844, 0xDBA3F9, {0xDBA3ED: (0, 'GetHero')})):
            returned = read_call_window(data, base, size, lookup)
            consumed = read_call_window(data, base, size, distance, known)
            self.assertEqual(returned.stack_arguments[0], consumed.edx)
            self.assertIsNone(read_call_window(data, base, size, distance + 1, known))

    def test_actual_cached_lookup_and_distance_stack_addresses_agree(self):
        data = RData()
        for start, end, check_start, check_end, offset, threshold, calls in (
                (0xDAF101, 0xDAF116, 0xDAFAA9, 0xDAFABC, 0x28, 0x41200000, {}),
                (0xDB9835, 0xDB984A, 0xDBA3E3, 0xDBA3FE, 0x2c, 0x40000000,
                 {0xDBA3ED: (0, 'GetHero')})):
            lookup = call_setup(decode(data.bytes_at(start, end-start), start))
            distance = call_setup(decode(data.bytes_at(check_start, check_end-check_start), check_start), calls)
            self.assertEqual(lookup.stack_arguments, (('stack', offset), ('stack', 0x10)))
            self.assertEqual(distance.edx, lookup.stack_arguments[0])
            self.assertEqual(distance.stack_arguments, (('constant', threshold),))
            self.assertEqual(distance.target, ('constant', 0xCBE2FF))
            self.assertEqual(distance.ecx, ('result', 0xDBA3ED, 'GetHero') if calls else
                             ('address', ('register', 'ebp'), 8))

    def test_unknown_call_and_control_transfer_reject_window(self):
        self.assertIsNone(call_setup(decode(bytes.fromhex('e800000000 e800000000'))))
        self.assertIsNone(call_setup(decode(bytes.fromhex('7400 e800000000'))))

    def test_partial_register_write_does_not_preserve_old_whole_value(self):
        result = call_setup(decode(bytes.fromhex('b878563412 b001 89c1 e800000000')))
        self.assertEqual(result.ecx, ('unknown',))

    def test_unmodeled_stack_changes_and_memory_writes_reject(self):
        for raw in ('83ec04 e800000000', '890424 e800000000'):
            self.assertIsNone(call_setup(decode(bytes.fromhex(raw))))

    def test_explicit_abi_store_keeps_computed_address_but_forgets_memory_values(self):
        # lea ecx,[esp+28h]; mov byte [esi+51h],1; call: native hero resource ctor.
        setup = call_setup(decode(bytes.fromhex('8d4c2428 c6465101 e800000000')), argument_count=0)
        self.assertEqual(setup.ecx, ('stack', 40))
        # The saved load could alias the write; it cannot be reused as a target.
        setup = call_setup(decode(bytes.fromhex('8b0e c60701 e800000000')), argument_count=0)
        self.assertEqual(setup.ecx, ('unknown',))
        # An address derived from a prior memory load is forgotten too.
        setup = call_setup(decode(bytes.fromhex('8b0e 8d4904 c60701 e800000000')), argument_count=0)
        self.assertEqual(setup.ecx, ('unknown',))

    def test_store_cannot_preserve_or_invent_staged_arguments(self):
        for store in ('c60701', '890424', 'c6042401', '66c704240100'):
            with self.subTest(store=store):
                self.assertIsNone(call_setup(decode(bytes.fromhex('6a07 ' + store + ' e800000000')),
                                             argument_count=1))
        # A fresh push after the write provides independent argument evidence.
        setup = call_setup(decode(bytes.fromhex('c60701 6a09 e800000000')), argument_count=1)
        self.assertEqual(setup.stack_arguments, (('constant', 9),))

    def test_actual_post_attack_actor_constructor_address_survives_field_store(self):
        setup = read_call_window(RData(), 0xDBEB20, 1095, 0xDBEDAD, argument_count=0)
        self.assertEqual(setup.ecx, ('stack', 40))
        self.assertEqual(setup.target, ('constant', 0x7E72A0))

    def test_implicit_effects_cannot_preserve_pushed_arguments(self):
        for effect in ('ab', 'f3ab', 'a5', 'f3a5', 'cd80', 'cc', '0f0b', '0f34'):
            with self.subTest(effect=effect):
                # EDI aliases the pushed argument; STOS/MOVS could replace it.
                raw = bytes.fromhex('6a07 89e7 ' + effect + ' e800000000')
                self.assertIsNone(call_setup(decode(raw)))

    def test_exchange_memory_in_second_operand_rejects(self):
        raw = bytes.fromhex('6a07 870424 e800000000')
        self.assertIsNone(call_setup(decode(raw)))

    def test_read_only_comparison_preserves_argument(self):
        raw = bytes.fromhex('6a07 833c2400 e800000000')
        self.assertEqual(call_setup(decode(raw)).stack_arguments, (('constant', 7),))

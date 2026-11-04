"""The float Ghidra drops after a by-value CScriptThing (TraderConflictEvil Main 0x00DF66D1:
`SetCombatNearbyBreakOffRange(thing, 10.0)` printed with the thing as two words and no range)."""
import struct
import unittest
import json
from pathlib import Path
from types import SimpleNamespace

from tools.script_recovery.native_evidence_lowering import lost_trailing_float_operand

START = 0x1000


def _spec(code, sites):
    image = bytes.fromhex(code)
    return SimpleNamespace(native_address=START,
                           byte_at=lambda va: image[va - START] if 0 <= va - START < len(image) else 0xcc,
                           indirect_calls=[{'site': hex(s), 'slot': '0xcc4'} for s in sites])


def _lost_shape(value):
    # push imm32; sub esp, 0xc; mov eax, esp; mov dword ptr [eax], 0x1238c8c; mov ecx, [ebx+0x40];
    # mov eax, [ecx]; call [eax+0xcc4]
    imm = struct.unpack('<I', struct.pack('<f', value))[0]
    return ('68' + imm.to_bytes(4, 'little').hex() + '83ec0c' + '8bc4' + 'c7008c8c2301' + '8b4b40' + '8b01'
            + 'ff90c40c0000')


CALL_LEN = 6


class LostTrailingFloatOperandTests(unittest.TestCase):
    def test_retail_shape_recovers_the_pushed_float(self):
        code = _lost_shape(10.0)
        self.assertEqual(lost_trailing_float_operand(_spec(code, [START + len(code) // 2 - CALL_LEN]), 0xcc4), '10.0')

    def test_sites_that_disagree_make_no_claim(self):
        a, b = _lost_shape(10.0), _lost_shape(5.0)
        sites = [START + len(a) // 2 - CALL_LEN, START + (len(a) + len(b)) // 2 - CALL_LEN]
        self.assertIsNone(lost_trailing_float_operand(_spec(a + b, sites), 0xcc4))

    def test_a_call_between_push_and_site_is_not_evidence(self):
        # push imm32; sub esp, 0xc; push eax; call rel32; mov ecx,[ebx+0x40]; call [edi+0xcc4] (the 0x00DF689A shape)
        code = '6800002041' + '83ec0c' + '50' + 'e800000000' + '8b4b40' + 'ff97c40c0000'
        self.assertIsNone(lost_trailing_float_operand(_spec(code, [START + len(code) // 2 - CALL_LEN]), 0xcc4))

    def test_no_bytes_or_sites_make_no_claim(self):
        self.assertIsNone(lost_trailing_float_operand(SimpleNamespace(), 0xcc4))
        self.assertIsNone(lost_trailing_float_operand(_spec(_lost_shape(10.0), []), 0xcc4))

    def test_stack_writes_after_reservation_invalidate_the_pushed_float(self):
        for operation in ('83c404', '58', '8d642404', '8be0', '44', '6683c404', 'c9'):
            with self.subTest(operation=operation):
                code = _lost_shape(10.0)
                code = code[:-2 * CALL_LEN] + operation + code[-2 * CALL_LEN:]
                site = START + len(code) // 2 - CALL_LEN
                self.assertIsNone(lost_trailing_float_operand(_spec(code, [site]), 0xcc4))

    def test_one_unchanged_site_does_not_override_a_stack_clobber(self):
        first = _lost_shape(10.0)
        second = first[:-2 * CALL_LEN] + '83c404' + first[-2 * CALL_LEN:]
        sites = [START + len(first) // 2 - CALL_LEN,
                 START + (len(first) + len(second)) // 2 - CALL_LEN]
        self.assertIsNone(lost_trailing_float_operand(_spec(first + second, sites), 0xcc4))

    def test_arithmetic_on_a_different_register_does_not_change_the_stack_slot(self):
        code = _lost_shape(10.0)
        code = code[:-2 * CALL_LEN] + '83c104' + code[-2 * CALL_LEN:]
        site = START + len(code) // 2 - CALL_LEN
        self.assertEqual(lost_trailing_float_operand(_spec(code, [site]), 0xcc4), '10.0')

    def test_retail_functions_preserve_the_three_lost_ranges(self):
        from tools.script_recovery.lift_native_lua import RData
        root = Path(__file__).resolve().parents[2]
        functions = json.loads((root / 'refs/script_recovery/trader_conflict/translation_unit_typed.json').read_text())['functions']
        image = RData()
        expected = {0xdf6010: '10.0', 0xdf7bf0: None, 0xdfa450: '10.0', 0xdfc920: '10.0'}
        for address, value in expected.items():
            with self.subTest(address=hex(address)):
                fn = next(f for f in functions if int(f['address'], 16) == address)
                spec = SimpleNamespace(native_address=address,
                                       byte_at=lambda va: (image.bytes_at(va, 1) or bytes([255]))[0],
                                       indirect_calls=fn['indirectCalls'])
                self.assertEqual(lost_trailing_float_operand(spec, 0xcc4), value)


if __name__ == '__main__':
    unittest.main()

import struct
import unittest
from types import SimpleNamespace

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_call_setup_ir import read_call_window


class JumpTableWindowTests(unittest.TestCase):
    def input(self, targets=(0x100C, 0x1011), suffix=b'', guard=b'\x77\x07'):
        code = bytes.fromhex('83f801') + guard + bytes.fromhex('ff248500110000 b907000000 e800000000 c3') + suffix
        table = b''.join(struct.pack('<I', target) for target in targets)
        def read(address, size):
            for base, data in ((0x1000, code), (0x1100, table)):
                if base <= address and address + size <= base + len(data):
                    return data[address - base:address - base + size]
            return None
        return SimpleNamespace(bytes_at=read), len(code)

    def test_case_jump_to_call_cannot_borrow_skipped_argument_setup(self):
        data, size = self.input()
        result = read_call_window(data, 0x1000, size, 0x1011, argument_count=0)
        self.assertEqual(result.ecx, ('register', 'ecx'))
        data, size = self.input(targets=(0x100C, 0x100C))
        self.assertEqual(read_call_window(data, 0x1000, size, 0x1011, argument_count=0).ecx, ('constant', 7))

    def test_bypassed_guard_invalid_target_and_wrong_condition_reject(self):
        cases = [self.input(targets=(0x1005, 0x100C)),
                 self.input(targets=(0x100D, 0x100C)),
                 self.input(targets=(0x2000, 0x100C)),
                 self.input(guard=b'\x7f\x07')]
        # Direct jump after the return targets the dispatch, bypassing CMP.
        cases.append(self.input(suffix=b'\xe9' + struct.pack('<i', 0x1005 - 0x101C)))
        for data, size in cases:
            self.assertIsNone(read_call_window(data, 0x1000, size, 0x1011, argument_count=0))

    def test_missing_table_entries_reject(self):
        data, size = self.input(targets=(0x100C,))
        self.assertIsNone(read_call_window(data, 0x1000, size, 0x1011, argument_count=0))

    def test_direct_jumps_into_guard_or_argument_instruction_interiors_reject(self):
        for target in (0x1001, 0x1004, 0x1006, 0x100D):
            data, size = self.input(suffix=b'\xe9' + struct.pack('<i', target - 0x101C))
            self.assertIsNone(read_call_window(data, 0x1000, size, 0x1011, argument_count=0))

    def test_actual_barrel_lookup_now_has_checked_incoming_case_edges(self):
        result = read_call_window(RData(), 0xDB5330, 6128, 0xDB55E2, argument_count=2)
        self.assertIsNotNone(result)
        self.assertEqual(result.stack_arguments, (('stack', 172), ('stack', 160)))


if __name__ == '__main__':
    unittest.main()

import unittest
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


class ResourceLifetimeTests(unittest.TestCase):
    def test_shared_destructor_checks_each_incoming_resource(self):
        # Two independent starts converge on one destructor with selected ECX.
        raw = bytes.fromhex('740b e800000000 8d4c2448 eb09 e800000000 8d4c2460 e800000000 c3')
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        ins = list(decoder.disasm(raw, 0x1000))
        events = {0x1002: ('start', 72), 0x100d: ('start', 96), 0x1016: ('end', ('register', 'ecx'))}
        selections = {0x1007: 72, 0x1012: 96}
        self.assertTrue(check_single_resource_lifetime(ins, events, receiver_register='ecx', selections=selections))
        for changed in ({0x1007: 72, 0x1012: 72}, {0x1007: 72}):
            self.assertFalse(check_single_resource_lifetime(ins, events, receiver_register='ecx', selections=changed))

    def test_selected_receiver_is_forgotten_after_writes_and_calls(self):
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        for middle in ('b101', '31c9', 'e800000000'):
            raw = bytes.fromhex('e800000000 8d4c2448 ' + middle + ' e800000000 c3')
            ins = list(decoder.disasm(raw, 0x1000))
            events = {0x1000: ('start', 72), ins[-2].address: ('end', ('register', 'ecx'))}
            self.assertFalse(check_single_resource_lifetime(ins, events, receiver_register='ecx', selections={0x1005: 72}))

    def check(self, raw, events):
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        return check_single_resource_lifetime(list(decoder.disasm(raw, 0x1000)), events)

    def test_balanced_lifetime_and_mismatched_resource(self):
        raw = bytes.fromhex('e800000000 e800000000 c3')
        self.assertTrue(self.check(raw, {0x1000: ('start', 72), 0x1005: ('end', 72)}))
        self.assertFalse(self.check(raw, {0x1000: ('start', 72), 0x1005: ('end', 96)}))
        self.assertFalse(self.check(raw, {0x1000: ('start', 72), 0x1005: ('start', 96)}))
        self.assertFalse(self.check(raw, {0x1005: ('end', 72)}))

    def test_branch_bypassing_cleanup_and_unknown_edges_are_rejected(self):
        raw = bytes.fromhex('e800000000 7405 e800000000 c3')
        self.assertFalse(self.check(raw, {0x1000: ('start', 72), 0x1007: ('end', 72)}))
        self.assertFalse(self.check(bytes.fromhex('ffe0'), {}))
        self.assertFalse(self.check(bytes.fromhex('ebff c3'), {}))
        self.assertFalse(self.check(bytes.fromhex('90'), {}))


if __name__ == '__main__':
    unittest.main()

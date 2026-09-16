import unittest
from types import SimpleNamespace
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_bounded_switch import resolve


class BoundedSwitchTests(unittest.TestCase):
    raw=bytes.fromhex('83f801 7713 ff248500200000 e800000000 c3 e800000000 c3 c3')

    def check(self,raw=None,targets=(0x100C,0x1012)):
        decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
        def read(address,size):
            self.assertEqual((address,size),(0x2000,8))
            return b''.join(t.to_bytes(4,'little') for t in targets)
        return resolve(list(decoder.disasm(raw or self.raw,0x1000)),SimpleNamespace(bytes_at=read))

    def test_exhaustive_aligned_targets(self):
        self.assertEqual(self.check(),{0x1005:(0x100C,0x1012)})

    def test_bound_bypass_misalignment_and_wrong_selector_reject(self):
        for targets in ((0x1003,0x1012),(0x1005,0x1012),(0x100D,0x1012),(0x3000,0x1012)):
            with self.subTest(targets=targets),self.assertRaises(ValueError):self.check(targets=targets)
        for offset,value in ((3,0x72),(1,0xF9)):
            raw=bytearray(self.raw);raw[offset]=value
            with self.assertRaises(ValueError):self.check(bytes(raw))

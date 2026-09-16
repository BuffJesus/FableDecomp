import struct
import unittest
from types import SimpleNamespace
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_health import verify
from tools.script_recovery.test_native_barrel_man_health_branches import native


class TheresaHealthTests(unittest.TestCase):
    def test_eight_native_branches_match_lua_with_destructor_clobber(self):
        data = RData()
        w = verify(data)
        positive = LuaRuntime().eval('function(health) return health > 0.0 end')
        for region in w['branches']:
            for bits in (0, 0x80000000, 1, 0x80000001, 0x3f800000,
                         0xbf800000, 0x7f800000, 0xff800000, 0x7fc12345):
                with self.subTest(address=hex(region['address']), bits=hex(bits)):
                    value = struct.unpack('<f', struct.pack('<I', bits))[0]
                    self.assertEqual(native(data, region, bits), positive(value))

    def test_rejects_changed_zero_threshold(self):
        data = RData()
        def read(address, size):
            return b'\1\0\0\0' if (address, size) == (0x122dedc, 4) else data.bytes_at(address, size)
        with self.assertRaisesRegex(ValueError, 'threshold'): verify(SimpleNamespace(bytes_at=read))

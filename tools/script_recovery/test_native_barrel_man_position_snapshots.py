import struct
import unittest
from types import SimpleNamespace

from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EBP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_EIP
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_barrel_man_position_snapshots import verify


def execute(data, region, empty, value):
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, size in ((0xDB5000, 0x1000), (0x143E000, 0x1000),
                          (0x100000, 0x10000), (0x200000, 0x1000)):
        uc.mem_map(address, size)
    uc.mem_write(region['address'], data.bytes_at(region['address'], region['size']))
    def put(address, value):
        uc.mem_write(address, struct.pack('<I', value))
    def get(address):
        return struct.unpack('<I', uc.mem_read(address, 4))[0]
    stack, implementation, table, position, getter = 0x108000, 0x200000, 0x200100, 0x200200, 0x200300
    put(stack + region['markerStack'] + 4, 0 if empty else implementation)
    put(implementation, table)
    put(table + 0x18, getter)
    uc.mem_write(position, value)
    uc.mem_write(region['fallbackAddress'], value)
    uc.reg_write(UC_X86_REG_ESP, stack)
    uc.reg_write(UC_X86_REG_EBP, 0)
    calls = []
    def hook(machine, address, size, user):
        if address != getter:
            return
        calls.append(machine.reg_read(UC_X86_REG_ECX))
        esp = machine.reg_read(UC_X86_REG_ESP)
        machine.reg_write(UC_X86_REG_EAX, position)
        machine.reg_write(UC_X86_REG_EIP, get(esp))
        machine.reg_write(UC_X86_REG_ESP, esp + 4)
    uc.hook_add(UC_HOOK_CODE, hook)
    end = region['address'] + region['size']
    uc.emu_start(region['address'], end, count=100)
    assert uc.reg_read(UC_X86_REG_EIP) == end
    assert calls == ([] if empty else [implementation])
    # Two pending arguments belong to the upcoming controlled-Thing/distance calls.
    assert uc.reg_read(UC_X86_REG_ESP) == stack - 8
    assert get(stack - 8) == stack + region['temporaryOutputStack']
    assert get(stack - 4) == region['thresholdBits']
    assert uc.reg_read(UC_X86_REG_ECX) == stack + region['resourceStack']
    snapshot = bytes(uc.mem_read(stack + region['snapshotStack'], 12))
    # Neither later marker movement nor a fallback update may change this snapshot.
    uc.mem_write(position, b'\xA5' * 12)
    uc.mem_write(region['fallbackAddress'], b'\x5A' * 12)
    assert bytes(uc.mem_read(stack + region['snapshotStack'], 12)) == snapshot
    return snapshot


class BarrelPositionSnapshotTests(unittest.TestCase):
    def test_native_copies_all_bits_from_live_marker_or_fallback(self):
        data = RData()
        vectors = [struct.pack('<fff', 1.25, -2.5, 4.0),
                   struct.pack('<III', 0x80000000, 0x7FC12345, 0x7F800000),
                   struct.pack('<III', 1, 0xFF800000, 0xFFFFFFFF)]
        for region in verify(data)['snapshots']:
            for empty in (False, True):
                for value in vectors:
                    with self.subTest(marker=region['name'], empty=empty, value=value):
                        self.assertEqual(execute(data, region, empty, value), value)

    def test_changed_snapshot_bytes_reject(self):
        data = RData()
        for region in verify(data)['snapshots']:
            for offset in (0, 12, region['size'] - 1):
                site = region['address'] + offset
                def read(address, size):
                    raw = data.bytes_at(address, size)
                    if raw is not None and address <= site < address + size:
                        raw = bytearray(raw)
                        raw[site - address] ^= 1
                        raw = bytes(raw)
                    return raw
                with self.assertRaisesRegex(ValueError, 'snapshot instructions changed'):
                    verify(SimpleNamespace(bytes_at=read))

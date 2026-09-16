"""Original SKIP call sites and resource thunk; CString/engine bodies are doubles."""
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_control import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX


class TheresaAnimationTests(unittest.TestCase):
    def test_both_skip_scopes_forward_all_flags_and_destroy_even_when_empty(self):
        data = RData()
        verify(data)
        self.assertEqual(data.bytes_at(0x12d9928, 5), b'SKIP\0')
        self.assertEqual(data.bytes_at(0x7e73e0, 15), bytes.fromhex('8b490885c974058b01ff604cc21c00'))
        for start, end, output in ((0xdb98f3, 0xdb992f, 188), (0xdbae1e, 0xdbae51, 56)):
            for live in (False, True):
                for payload in (0, 0x202800):
                    with self.subTest(start=hex(start), live=live, payload=payload):
                        uc = Uc(UC_ARCH_X86, UC_MODE_32)
                        for address, size in ((0xdb9000, 0x2000), (0x7e7000, 0x1000),
                                              (0x99e000, 0x1000), (0x100000, 0x10000), (0x200000, 0x4000)):
                            uc.mem_map(address, size)
                        uc.mem_write(start, data.bytes_at(start, end - start))
                        uc.mem_write(0x7e73e0, data.bytes_at(0x7e73e0, 15))
                        def put(a, v): uc.mem_write(a, v.to_bytes(4, 'little'))
                        def get(a): return int.from_bytes(uc.mem_read(a, 4), 'little')
                        stack, expert, table, target = 0x108000, 0x201000, 0x202000, 0x203000
                        put(stack + 24 + 8, expert if live else 0)
                        put(expert, table)
                        put(table + 0x4c, target)
                        events = []
                        def hook(machine, address, size, user):
                            if address not in (0x99ebf0, 0x99eae0, target): return
                            esp = machine.reg_read(UC_X86_REG_ESP)
                            receiver = machine.reg_read(UC_X86_REG_ECX)
                            count = 0
                            if address == 0x99ebf0:
                                self.assertEqual(receiver, stack + output)
                                self.assertEqual([get(esp + 4), get(esp + 8)], [0x12d9928, 0xffffffff])
                                put(receiver, payload)
                                events.append('construct')
                                count = 2
                            elif address == target:
                                self.assertEqual(receiver, expert)
                                self.assertEqual([get(esp + 4 + 4*i) for i in range(7)],
                                                 [stack + output, 1, 0, 0, 1, 0, 0])
                                events.append('combat-animation')
                                count = 7
                            else:
                                self.assertEqual(receiver, stack + output)
                                self.assertEqual(get(receiver), payload)
                                events.append('destroy')
                            machine.reg_write(UC_X86_REG_EAX, 0xBADBAD)
                            machine.reg_write(UC_X86_REG_ESP, esp + 4 + 4*count)
                            machine.reg_write(UC_X86_REG_EIP, get(esp))
                        uc.reg_write(UC_X86_REG_ESP, stack)
                        uc.hook_add(UC_HOOK_CODE, hook)
                        uc.emu_start(start, end, count=100)
                        self.assertEqual(events, ['construct'] + (['combat-animation'] if live else []) + ['destroy'])
                        self.assertEqual(uc.reg_read(UC_X86_REG_ESP), stack)

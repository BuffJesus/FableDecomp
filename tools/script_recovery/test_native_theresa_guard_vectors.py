"""Execute the actual removal loop and vector destructor with engine doubles."""
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_guard_vectors import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EAX


class TheresaGuardVectorTests(unittest.TestCase):
    def test_native_removal_and_destruction_of_every_owned_thing(self):
        data = RData(); verify(data)
        for count in range(5):
            for mask in range(1 << count):
                for allocated in (False, True) if count == 0 else (True,):
                    with self.subTest(count=count, mask=mask, allocated=allocated):
                        uc = Uc(UC_ARCH_X86, UC_MODE_32)
                        for address, size in ((0xcbe000, 0x1000), (0x8ac000, 0x1000), (0xbfe000, 0x1000),
                                              (0x100000, 0x10000), (0x200000, 0x5000)):
                            uc.mem_map(address, size)
                        uc.mem_write(0xcbed82, data.bytes_at(0xcbed82, 56))
                        uc.mem_write(0x8ac970, data.bytes_at(0x8ac970, 50))
                        def put(a, v): uc.mem_write(a, v.to_bytes(4, 'little'))
                        def get(a): return int.from_bytes(uc.mem_read(a, 4), 'little')
                        stack, vector, storage, game, table, thing_table = 0x108000, 0x200000, 0x201000, 0x202000, 0x203000, 0x203800
                        begin = storage if allocated else 0
                        put(vector, begin); put(vector+4, begin+12*count); put(vector+8, begin+12*count)
                        put(game, table); put(table+0x1b0, 0x204000)
                        put(thing_table+0x12c, 0x204010); put(thing_table, 0x204020)
                        for i in range(count): put(storage+12*i, thing_table)
                        for target in (0x204000, 0x204010, 0x204020, 0xbfea14): uc.mem_write(target, b'\xc3')
                        events = []
                        def hook(machine, address, size, user):
                            if address not in (0x204000, 0x204010, 0x204020, 0xbfea14): return
                            esp, receiver = machine.reg_read(UC_X86_REG_ESP), machine.reg_read(UC_X86_REG_ECX)
                            pop, result = 0, 0xabcd0000
                            if address == 0x204010:
                                index = (receiver-storage)//12
                                self.assertEqual(receiver, storage+index*12)
                                events.append(('alive', index)); result |= int(bool(mask & (1 << index)))
                            elif address == 0x204000:
                                self.assertEqual(receiver, game)
                                index = (get(esp+4)-storage)//12
                                self.assertEqual([get(esp+8),get(esp+12)], [0,1])
                                events.append(('remove', index)); pop = 3
                            elif address == 0x204020:
                                self.assertEqual(get(esp+4), 0)
                                events.append(('destroy', (receiver-storage)//12)); pop = 1
                            else:
                                self.assertEqual(get(esp+4), storage); events.append(('free',))
                            machine.reg_write(UC_X86_REG_EAX, result)
                            machine.reg_write(UC_X86_REG_ESP, esp+4+pop*4)
                            machine.reg_write(UC_X86_REG_EIP, get(esp))
                        uc.hook_add(UC_HOOK_CODE, hook)
                        put(stack, 0x204f00); put(stack+4, 0)
                        uc.reg_write(UC_X86_REG_ESP, stack); uc.reg_write(UC_X86_REG_ECX, game); uc.reg_write(UC_X86_REG_EDX, vector)
                        uc.emu_start(0xcbed82, 0x204f00, count=300)
                        self.assertEqual(uc.reg_read(UC_X86_REG_ESP), stack+8)
                        put(stack, 0x204f00); uc.reg_write(UC_X86_REG_ESP, stack); uc.reg_write(UC_X86_REG_ECX, vector)
                        uc.emu_start(0x8ac970, 0x204f00, count=300)
                        expected = []
                        for i in range(count):
                            expected.append(('alive',i))
                            if mask & (1 << i): expected.append(('remove',i))
                        expected += [('destroy',i) for i in range(count)] + ([('free',)] if allocated else [])
                        self.assertEqual(events, expected)
                        self.assertEqual(uc.reg_read(UC_X86_REG_ESP), stack+4)

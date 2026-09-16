"""All three gift tails: state, item, returned quest-name CString, objective, info."""
import itertools
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_control import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_EBP


class TheresaGiftCommitTests(unittest.TestCase):
    def test_three_native_tails_preserve_returned_name_and_cleanup_order(self):
        data = RData(); verify(data)
        for (start, end), alias, old_state, empty in itertools.product(
                ((0xdb9d8b, 0xdb9e6d), (0xdba213, 0xdba301), (0xdba70b, 0xdba7f0)),
                (False, True), (0, 255), (False, True)):
            with self.subTest(start=hex(start), alias=alias, old_state=old_state, empty=empty):
                uc = Uc(UC_ARCH_X86, UC_MODE_32)
                targets = (0x99ebf0, 0x99eae0, 0x203000, 0x203010, 0x203020, 0x203030)
                for address, size in ((0xdb9000, 0x3000), (0x99e000, 0x1000),
                                      (0x100000, 0x10000), (0x200000, 0x5000)):
                    uc.mem_map(address, size)
                uc.mem_write(start, data.bytes_at(start, end-start))
                for target in targets: uc.mem_write(target, b'\xc3')
                def put(a, v): uc.mem_write(a, v.to_bytes(4, 'little'))
                def get(a): return int.from_bytes(uc.mem_read(a, 4), 'little')
                stack, thread, actor, game, table, parent = 0x108000, 0x200000, 0x200008, 0x201000, 0x202000, 0x204000
                put(thread+4, game); put(thread+0x14, parent); put(game, table)
                uc.mem_write(parent+0x95, bytes([old_state])); uc.mem_write(stack+0x17, b'\0')
                for slot, target in ((0x1f4, 0x203000), (0xa3c, 0x203010), (0x4a0, 0x203020), (0x5a4, 0x203030)):
                    put(table+slot, target)
                strings, events, returned_name = {}, [], []
                def hook(machine, address, size, user):
                    if address not in targets: return
                    esp, receiver = machine.reg_read(UC_X86_REG_ESP), machine.reg_read(UC_X86_REG_ECX)
                    self.assertEqual(bytes(uc.mem_read(parent+0x95, 1)), b'\1')
                    count, result = 0, 0xBADBAD
                    if address == 0x99ebf0:
                        self.assertEqual(get(esp+8), 0xffffffff)
                        key = data.bytes_at(get(esp+4), 100).split(b'\0')[0].decode()
                        self.assertNotIn(receiver, strings); strings[receiver] = key
                        put(receiver, 0 if empty else 1)
                        events.append(('new', key)); count = 2
                    elif address == 0x99eae0:
                        events.append(('destroy', strings.pop(receiver)))
                    else:
                        self.assertEqual(receiver, game)
                        if address == 0x203000:
                            events.append(('take', strings[get(esp+4)])); count = 1
                        elif address == 0x203010:
                            output = get(esp+4)
                            self.assertNotIn(output, strings); strings[output] = 'quest-name'
                            put(output, 0 if empty else 1)
                            result = 0x203800 if alias else output
                            returned_name[:] = [result]
                            events.append(('quest-name',)); count = 1
                        elif address == 0x203020:
                            self.assertEqual(get(esp+4), returned_name[0])
                            events.append(('objective', *(strings[get(esp+8+4*i)] for i in range(3)))); count = 4
                        else:
                            self.assertEqual(get(esp+4), actor)
                            self.assertEqual(bytes(uc.mem_read(stack+0x17, 1)), b'\1')
                            events.append(('clear-info',)); count = 1
                    machine.reg_write(UC_X86_REG_EAX, result)
                    machine.reg_write(UC_X86_REG_ESP, esp+4+count*4)
                    machine.reg_write(UC_X86_REG_EIP, get(esp))
                uc.reg_write(UC_X86_REG_ESP, stack); uc.reg_write(UC_X86_REG_EBP, thread)
                uc.hook_add(UC_HOOK_CODE, hook); uc.emu_start(start, end, count=200)
                item, objective = 'OBJECT_CHOCOLATE_BOX_UNGIVEABLE', 'TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_05'
                self.assertEqual(events, [('new', item), ('take', item), ('destroy', item),
                                          ('new', ''), ('new', ''), ('new', objective), ('quest-name',),
                                          ('objective', objective, '', ''), ('destroy', 'quest-name'),
                                          ('destroy', objective), ('destroy', ''), ('destroy', ''), ('clear-info',)])
                self.assertEqual(strings, {})
                self.assertEqual(uc.reg_read(UC_X86_REG_ESP), stack)

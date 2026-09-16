"""Execute complete original Init; API bodies are explicit call-boundary doubles."""
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_init import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX


class NativeTheresaInitTests(unittest.TestCase):
    def test_readable_init_resets_both_flags_before_engine_calls(self):
        for fail in (False,True):
            lua=LuaRuntime();phase=lua.execute(Path(__file__).with_name('theresa_init_body.lua').read_text())
            state=lua.table();resources=lua.table();events=[]
            def store(self,key,value):
                self[key]=value;events.append((key,value))
            state.SetStateBool=store;state.DoneIntro=True;state.AskedForPresent=True
            def initialize(self,actor):
                self_test.assertEqual(actor,17)
                self_test.assertFalse(state.DoneIntro);self_test.assertFalse(state.AskedForPresent)
                events.append(('initialize',))
                if fail:raise RuntimeError('INIT')
            self_test=self
            resources.InitializeTheresaActor=initialize
            if fail:
                with self.assertRaisesRegex(RuntimeError,'INIT'):phase(lua.table(),17,resources,state)
            else:phase(lua.table(),17,resources,state)
            self.assertEqual(events,[('DoneIntro',False),('AskedForPresent',False),('initialize',)])

    def test_bound_calls_and_by_value_pushability_copy(self):
        data = RData()
        witness = verify(data)
        for payload, refs, old_state in itertools.product((0, 1, 0xBADBAD), (None, 1, 7), (0, 255)):
            with self.subTest(payload=payload, refs=refs, old_state=old_state):
                uc = Uc(UC_ARCH_X86, UC_MODE_32)
                for address, size in ((0xDAC000, 0x1000), (0x100000, 0x10000), (0x200000, 0x4000)):
                    uc.mem_map(address, size)
                uc.mem_write(witness['address'], data.bytes_at(witness['address'], witness['size']))
                def put(a, v): uc.mem_write(a, v.to_bytes(4, 'little'))
                def get(a): return int.from_bytes(uc.mem_read(a, 4), 'little')
                thread, actor, game, table, counter = 0x200000, 0x200008, 0x201000, 0x202000, 0x202f00
                put(thread + 4, game)
                put(game, table)
                put(actor, 0x1238c8c)
                put(actor + 4, payload)
                put(actor + 8, counter if refs is not None else 0)
                put(counter, refs or 0)
                uc.mem_write(thread + 0x1c, bytes([old_state, old_state]))
                calls = [(0x810, [actor, 0]), (0x814, [actor, 0, 0]),
                         (0x838, [actor, 0]), (0x5a0, [actor, 0, 1, 0]),
                         (0xd30, [0x1238c8c, payload, counter if refs is not None else 0, 0]),
                         (0x844, [actor, 0])]
                targets = {0x203000 + i * 16: (slot, args) for i, (slot, args) in enumerate(calls)}
                for target, (slot, _) in targets.items(): put(table + slot, target)
                events = []
                def hook(machine, address, size, user):
                    if address not in targets: return
                    slot, expected = targets[address]
                    esp = machine.reg_read(UC_X86_REG_ESP)
                    self.assertEqual(machine.reg_read(UC_X86_REG_ECX), game)
                    self.assertEqual(bytes(uc.mem_read(thread + 0x1c, 2)), b'\0\0')
                    self.assertEqual([get(esp + 4 + i * 4) for i in range(len(expected))], expected)
                    if slot == 0xd30 and refs is not None:
                        self.assertEqual(get(counter), refs + 1)
                        # Model the callee consuming its copied Thing. This is not a callee proof.
                        put(counter, refs)
                    events.append(slot)
                    machine.reg_write(UC_X86_REG_EAX, 0xBADBAD)
                    machine.reg_write(UC_X86_REG_ESP, esp + 4 + len(expected) * 4)
                    machine.reg_write(UC_X86_REG_EIP, get(esp))
                uc.hook_add(UC_HOOK_CODE, hook)
                put(0x108000, 0x203f00)
                uc.reg_write(UC_X86_REG_ESP, 0x108000)
                uc.reg_write(UC_X86_REG_ECX, thread)
                uc.emu_start(witness['address'], 0x203f00, count=100)
                self.assertEqual(events, [slot for slot, _ in calls])
                self.assertEqual(get(actor + 4), payload)
                self.assertEqual(get(actor + 8), counter if refs is not None else 0)
                self.assertEqual(uc.reg_read(UC_X86_REG_ESP), 0x108004)

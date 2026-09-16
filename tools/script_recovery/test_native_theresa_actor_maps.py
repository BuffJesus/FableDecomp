import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_actor_maps import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX


class TheresaActorMapTests(unittest.TestCase):
    def test_original_binding_keys_returned_entries_and_resource_identity(self):
        data = RData(); verify(data)
        regions = ((0xdb9a10, 0xdb9a9d, 228, 344, 'THER'),
                   (0xdb9cdd, 0xdb9d58, 60, 344, 'THER'),
                   (0xdbb1a6, 0xdbb218, 60, 328, 'Theresa'))
        for start, end, map_slot, hero_slot, theresa_key in regions:
            for alias in (0x203000, 0x203800):
                uc = Uc(UC_ARCH_X86, UC_MODE_32)
                for address, size in ((0xdb9000, 0x3000), (0x99e000, 0x1000),
                                      (0xcdb000, 0x1000), (0xcd3000, 0x1000), (0x8ab000, 0x1000),
                                      (0x100000, 0x10000), (0x200000, 0x4000)):
                    uc.mem_map(address, size)
                uc.mem_write(start, data.bytes_at(start, end-start))
                for target in (0xcdbf70, 0x99ebf0, 0x99eae0, 0xcd3d2e, 0x8abd10):
                    uc.mem_write(target, b'\xc3')
                def get(a): return int.from_bytes(uc.mem_read(a, 4), 'little')
                stack = 0x108000
                keys, events, selected = {}, [], []
                def hook(machine, address, size, user):
                    if address not in (0xcdbf70, 0x99ebf0, 0x99eae0, 0xcd3d2e, 0x8abd10): return
                    esp, receiver = machine.reg_read(UC_X86_REG_ESP), machine.reg_read(UC_X86_REG_ECX)
                    count, result = 0, 0xBADBAD
                    if address == 0xcdbf70:
                        self.assertEqual(receiver, stack + map_slot); events.append('map')
                    elif address == 0x99ebf0:
                        self.assertEqual(get(esp+8), 0xffffffff)
                        key = data.bytes_at(get(esp+4), 32).split(b'\0')[0].decode()
                        self.assertNotIn(receiver, keys); keys[receiver] = key
                        events.append(('key', key)); count = 2
                    elif address == 0xcd3d2e:
                        self.assertEqual(receiver, stack + map_slot)
                        selected[:] = [keys[get(esp+4)]]
                        result = alias; count = 1
                    elif address == 0x8abd10:
                        self.assertEqual(receiver, alias)
                        events.append(('bind', selected[0], get(esp+4)-stack)); count = 1
                    else:
                        events.append(('destroy', keys.pop(receiver)))
                    machine.reg_write(UC_X86_REG_EAX, result)
                    machine.reg_write(UC_X86_REG_ESP, esp + 4 + count*4)
                    machine.reg_write(UC_X86_REG_EIP, get(esp))
                uc.reg_write(UC_X86_REG_ESP, stack)
                uc.hook_add(UC_HOOK_CODE, hook); uc.emu_start(start, end, count=150)
                self.assertEqual(events, ['map', ('key', 'HERO'), ('bind', 'HERO', hero_slot), ('destroy', 'HERO'),
                                          ('key', theresa_key), ('bind', theresa_key, 24), ('destroy', theresa_key)])
                self.assertEqual(keys, {})
                self.assertEqual(uc.reg_read(UC_X86_REG_ESP), stack)
                lua = LuaRuntime()
                build = lua.execute(Path(__file__).with_name('theresa_cutscene_actors.lua').read_text())
                resources, actual = lua.table(), []
                def new_map(self): actual.append('map'); return map_slot
                def set_actor(self, map_id, key, resource):
                    self_test.assertEqual(map_id, map_slot)
                    actual.extend([('key', key), ('bind', key, resource), ('destroy', key)])
                self_test = self
                resources.NewActorMap = new_map
                resources.SetActor = set_actor
                self.assertEqual(build(resources, hero_slot, 24, theresa_key), map_slot)
                self.assertEqual(actual, events)

    def test_partial_map_cleanup_preserves_original_error(self):
        lua = LuaRuntime()
        build = lua.execute(Path(__file__).with_name('theresa_cutscene_actors.lua').read_text())
        for fail_at in (1, 2):
            resources = lua.execute('''
                local failAt = ...
                return {
                    calls = 0, cleaned = {},
                    NewActorMap = function() return 77 end,
                    SetActor = function(self)
                        self.calls = self.calls + 1
                        if self.calls == failAt then error("binding failed") end
                    end,
                    DestroyActorMap = function(self, actors)
                        table.insert(self.cleaned, actors)
                        error("cleanup failed")
                    end
                }
            ''', fail_at)
            with self.assertRaisesRegex(Exception, 'binding failed'): build(resources, 344, 24, 'THER')
            self.assertEqual(list(resources.cleaned.values()), [77])

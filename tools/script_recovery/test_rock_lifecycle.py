import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_lifecycle import recover
from tools.script_recovery.rock_lifecycle_native import execute
from tools.script_recovery.lift_native_lua import RData

class LifecycleTests(unittest.TestCase):
    def test_init_native_and_lua_keep_empty_bound_actor_and_string_scope(self):
        for empty in (False,True):
            lua=LuaRuntime();lua.execute(recover()[0]);quest=lua.table();actor=lua.table();events=[]
            def init(_,received):
                self.assertTrue(lua.eval('rawequal')(actor,received))
                events.extend([('persistent',1),('string.new','HUD_ORB_RED_SMALL'),('marker','HUD_ORB_RED_SMALL'),('string.destroy',)])
            quest.InitializeRockTrollMarker=init
            lua.globals().Init(quest,actor);lua.globals().OnPredicateFail(quest,actor)
            self.assertEqual(events,execute(empty=empty))

    def test_original_constructor_does_not_initialize_quest_tail(self):
        for sentinel in (1,0x7f,0xa5,0xff):
            memory=execute(constructor=True,sentinel=sentinel)
            self.assertEqual(memory[0x48:0x58],bytes([sentinel])*16)
            self.assertEqual(memory[0x4c],sentinel)

    def test_changed_native_operands_and_constructor_writes_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for address in (0xec454a,0xec4552,0xec4559,0xec4573,0xec4590,0xec5379,0xcb815a,0x99a2f2,0xec3c8a):
            data=Changed();data.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError):recover(data)

if __name__=='__main__':unittest.main()

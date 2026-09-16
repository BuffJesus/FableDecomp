import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_final_targeting import recover
from tools.script_recovery.rock_final_targeting_native import execute
from tools.script_recovery.lift_native_lua import RData


def run(targets=(1,2),stop=1):
    source,_=recover();lua=LuaRuntime();lua.execute(source);q,me=lua.table(),lua.table();events=[];queries=0
    def target(_,actor):
        assert lua.eval('rawequal')(actor,me)
        events.extend([('hero',targets[0]),('look',targets[0]),('hero',targets[1]),('enemy',targets[1])])
    q.TargetRockTrollAtHero=target;q.NewScriptFrame=lambda _:events.append(('frame',))
    def term(_):
        nonlocal queries
        queries+=1;value=queries>=stop;events.append(('term',value));return value
    q.IsActiveThreadTerminating=term
    lua.globals().RockTrollFinalTargetingPhase(q,me);events.append(('resource.destroy',));return events


class FinalTargetingTests(unittest.TestCase):
    def test_original_fresh_target_null_output_and_first_frame_policies(self):
        for targets in ((1,2),(0,2),(1,0),(0,0)):
            for stop in (1,3):
                with self.subTest(targets=targets,stop=stop):self.assertEqual(run(targets,stop),execute(targets,stop))
        self.assertEqual(run()[4:],[('frame',),('term',True),('resource.destroy',)])

    def test_changed_source_target_slot_and_first_yield_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for address in (0xec4fb2,0xec4fbf,0xec4fc0,0xec4fcf,0xec4fd9,0xec4fda,0xec4fe5,0xec4ff7):
            data=Changed();data.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError):recover(data)


if __name__=='__main__':unittest.main()

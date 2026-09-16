import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.victim_init import SOURCE,generate,prove
from tools.script_recovery.victim_init_native import execute
from tools.script_recovery.lift_native_lua import RData

class VictimInitTests(unittest.TestCase):
    def test_original_init_state_reset_pointer_flags_and_consumed_copy(self):
        generate();expected=[('set','DoneThanks',False),('set','DisplayedGameInfo',False),('damage',False),('kill',False,False),('combo',False),('information',False,False,False),('pushable',False),('copy.destroy',),('movement',False),('scared',True)]
        for kind,info,initial in itertools.product(('empty','invalid','valid'),(False,True),itertools.product((False,True),repeat=2)):
            with self.subTest(kind=kind,info=info,initial=initial):self.assertEqual(execute(kind,info,initial),expected)
        lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();s=lua.table();r=lua.table();events=[]
        s.SetStateBool=lambda _,key,value:events.append(('set',key,value));q.WithRetailResources=lambda _,fn:fn(r);r.InitializeVictimActor=lambda _,actor:events.extend(expected[2:])
        lua.globals().VictimInit(q,7,s);self.assertEqual(events,expected)
    def test_changed_init_assignment_copy_increment_or_true_flag_rejected(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=bytearray(original.bytes_at(at,size))
                if at<=self.at<at+size:raw[self.at-at]^=1
                return bytes(raw)
        for at in (0xdaeeb9,0xdaeebd,0xdaef20,0xdaef32,0xdaef40):
            with self.subTest(at=at),self.assertRaises(ValueError):prove(Changed(at))

if __name__=='__main__':unittest.main()

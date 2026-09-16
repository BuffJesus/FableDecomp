import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.live_father_init import SOURCE,generate,prove
from tools.script_recovery.live_father_init_native import execute
from tools.script_recovery.lift_native_lua import RData

class LiveFatherInitTests(unittest.TestCase):
    def test_original_init_and_real_consuming_callee_lifetime(self):
        generate()
        expected=[('set','PenniesGiven',0),('damage',False),('kill',False,False),('combo',False),('information',False,True,False),('pushable',False),('copy.destroy',),('deeds',False)]
        for kind,info,paid in itertools.product(('empty','invalid','valid'),(False,True),(-2147483648,0,17,2147483647)):
            with self.subTest(kind=kind,info=info,paid=paid):self.assertEqual(execute(kind,info,paid),expected)
        lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();s=lua.table();r=lua.table();events=[]
        s.SetStateInt=lambda _,key,value:events.append(('set',key,value));q.WithRetailResources=lambda _,fn:fn(r)
        r.InitializeLiveFatherActor=lambda _,actor:events.extend(expected[1:])
        lua.globals().LiveFatherInit(q,7,s);self.assertEqual(events,expected)
    def test_changed_init_copy_or_callee_cleanup_rejected(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=bytearray(original.bytes_at(at,size))
                if at<=self.at<at+size:raw[self.at-at]^=1
                return bytes(raw)
        for at in (0xdac399,0xdac3ff,0xdac406,0x8a6df9,0x8a6e77,0x8a6ea7,0x1260f0c+0xd30):
            with self.subTest(at=at),self.assertRaises(ValueError):prove(Changed(at))

if __name__=='__main__':unittest.main()

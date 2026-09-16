import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.wife_init_recovery import SOURCE,prove
from tools.script_recovery.wife_init_native import execute
from tools.script_recovery.lift_native_lua import RData

class WifeInitRecoveryTests(unittest.TestCase):
    def test_original_init_and_actual_copied_reference_consumer(self):
        for data,info,initial in itertools.product(('empty','invalid','valid'),(False,True),itertools.product((False,True),repeat=3)):
            trace=[];lua=LuaRuntime();q,r,state=lua.table(),lua.table(),lua.table()
            q.WithRetailResources=lambda _,body:body(r)
            state.SetStateBool=lambda _,key,value:trace.append(('set',key,value))
            r.InitializeWifeActor=lambda _,me:trace.extend([('damage',False),('kill',False,False),('combo',False),('information',False,True,False),('movement',False),('pushable',False),('copy.destroy',)])
            r.SetWifeDeedReactionsDisabled=lambda _,me:trace.append(('deeds',False))
            lua.execute(SOURCE);lua.globals().WifeRecoveredInit(q,'me',state)
            self.assertEqual(trace,execute(data,info,initial))

    def test_mutated_copy_store_flags_and_api_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.pc<address+size:raw=bytearray(raw);raw[self.pc-address]^=1;return bytes(raw)
                return raw
        for pc in (0xdb2a79,0xdb2ab2,0xdb2aee,0xdb2af5,0xdb2b00,0x8a6e77,0x1260f0c+0xd30):
            data=Changed();data.pc=pc
            with self.assertRaises(ValueError):prove(data)

if __name__=='__main__':unittest.main()

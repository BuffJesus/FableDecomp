import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_lifecycle import prove
from tools.script_recovery.bully_lifecycle_native import execute
from tools.script_recovery.bully_full_resource_candidate import generate
from tools.script_recovery.lift_native_lua import RData

class BullyLifecycleTests(unittest.TestCase):
    def test_original_init_fields_raw_calls_and_actual_callee_copy_lifetime(self):
        source,_=generate()
        anchor='__native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end'
        self.assertEqual(source.count(anchor),1)
        source=source.replace(anchor,'__native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value; recordState(name,value) end')
        for data_kind,info,hero in itertools.product(('empty','invalid','valid'),(False,True),(False,True)):
            lua=LuaRuntime();trace=[];lua.globals().recordState=lambda name,value:trace.append(('state','entity',name,value))
            lua.execute(source);q,r=lua.table(),lua.table()
            q.SetStateBool=lambda _,name,value:trace.append(('state','quest',name,value))
            q.WithRetailResources=lambda _,body:body(r)
            def initialize(_,actor):
                self.assertEqual(actor,'me')
                trace.extend([('damage',False),('kill',False,False),('combo',False),('information',False,False,False),('hero',),('ally',),('pushable',False)])
            r.InitializeBullyActor=initialize;lua.globals().Init(q,'me')
            self.assertEqual(execute(True,data_kind,info,hero),trace)

    def test_original_given_teddy_effects_and_scoped_removal_match_generated_function(self):
        source,_=generate();lua=LuaRuntime();trace=[];q=lua.table()
        lua.execute('package={preload={}}; function require(n) return package.preload[n]() end')
        helpers=lua.table();helpers.AddBadDeed=lambda quest,actor,value:trace.append(('bad.deed',value))
        lua.globals().package.preload['NewOakValeIntro.native_quest_helpers']=lambda:helpers
        q.GiveHeroGold=lambda _,amount:trace.append(('gold',amount))
        q.TakeObjectFromHero=lambda _,name:trace.extend([('string.new',name),('take',name),('string.destroy',name)])
        q.SetStateBool=lambda _,name,value:trace.append(('state','quest',name,value))
        lua.execute(source);lua.globals().GivenTeddy(q,'me')
        self.assertEqual(execute(False),trace)

    def test_changed_native_initialization_flags_copy_retention_and_deed_operands_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for pc in (0xdaed41,0xdaed61,0xdaedca,0xdaedd1,0xdbcd09,0xdbcd3d,0xdbcd4e,0x8a6df9,0x8a6e77):
            data=Changed();data.changed=pc
            with self.assertRaises(ValueError):prove(data)

if __name__=='__main__':unittest.main()

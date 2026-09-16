import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.affair_man_complete import baseline,generate,prove
from tools.script_recovery.affair_man_complete_animation_native import execute as animation
from tools.script_recovery.affair_man_complete_init_native import execute as init
from tools.script_recovery.test_generate_affair_man_resource_candidate import HARNESS
from tools.script_recovery.lift_native_lua import RData

def run(source,scenario):
    harness=HARNESS.replace('    local quest = {}','''    function resources:PlayAffairManAnimation(id,key,second)
        return self:PlayAnimation(id,key,false,second,false,true,self:ReadAnimationArgument5(),false,false)
    end
    local quest = {}
    function quest:ClearThingHasInformation(actor) rec('ClearThingHasInformation', actor.name) end
''')
    harness=harness.replace("function resources:ThingAlive(id) assert(entries[id] == 'thing'); return true end",
        "function resources:ThingAlive(id) assert(entries[id] == 'thing'); return not ((id == 2 and scenario.womanDead) or (id == 3 and scenario.wifeDead)) end")
    lua=LuaRuntime(unpack_returned_tuples=True);return list(lua.execute(harness)(source,lua.table_from(scenario)).values())

class AffairManCompleteTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.before,_=baseline();cls.after,cls.report=generate()

    def test_native_init_state_and_actual_copy_consumer(self):
        source=self.after.replace('fields[name] = value end','fields[name] = value;recordState(name,value) end',1)
        for data,info,initial,badger in itertools.product(('empty','invalid','valid'),(False,True),itertools.product((False,True),repeat=4),(-2147483648,-1,0,2147483647)):
            lua=LuaRuntime();events=[];q,r=lua.table(),lua.table()
            lua.globals().recordState=lambda key,value:events.append(('set',key,value))
            q.WithRetailResources=lambda _,body:body(r)
            r.InitializeAffairManActor=lambda _,actor:events.extend([('damage',False),('kill',False,False),('combo',False),('pushable',False),('copy.destroy',),('movement',False),('information',False,False,False)])
            lua.execute(source);lua.globals().Init(q,'me')
            self.assertEqual(events,init(data,info,initial,badger))

    def test_original_animation_sites_raw_byte_and_empty_control(self):
        keys=('ST_OPINION_FEAR_IDLE_COWERING','GIVE_KISS','GIVE_HUG')
        for kind,raw,populated in itertools.product(range(3),(0,1,2,255),(False,True)):
            expected=[]
            if kind:expected.append(('state','ReceiveKiss' if kind==1 else 'ReceiveHug',True))
            expected.extend([('key.new',keys[kind]),('raw',raw)])
            if populated:expected.append(('animation',keys[kind],0,int(kind!=0),0,1,raw,0,0))
            expected.append(('key.destroy',keys[kind]))
            self.assertEqual(animation(kind,raw,populated),expected)

    def test_whole_main_existing_phase_and_error_traces(self):
        for scenario,stop in itertools.product(({'hit':True},{'talk':True},{'talk':True,'retryTalk':True},
                {'talk':True,'womanDead':True,'wifeDead':True},{'idle':True},{'acquire':False}),range(1,31)):
            case=dict(acquire=True,terminateAtFrame=5,terminateAtCheck=stop);case.update(scenario)
            self.assertEqual(run(self.before,case),run(self.after,case))
        for failure,interaction in itertools.product(('failSpeak','failHealth','failLookup'),('hit','talk')):
            case=dict(acquire=True,terminateAtFrame=5);case[failure]=case[interaction]=True
            self.assertEqual(run(self.before,case),run(self.after,case))

    def test_native_operand_changes_rejected_and_todos_classified(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.pc<address+size:raw=bytearray(raw);raw[self.pc-address]^=1;return bytes(raw)
                return raw
        for pc in (0xdb095b,0xdb09b3,0xdb09ba,0xdb168e,0xdb1697,0xdb1beb,0xdb1c41,0x7e73d9,0x8a6e77):
            d=Changed();d.pc=pc
            with self.assertRaises(ValueError):prove(d)
        self.assertNotIn('TODO(native)',self.after);self.assertNotIn('goto ',self.after)
        self.assertEqual(len(self.report['completePass']['evidence']['historicalComments']),4)

if __name__=='__main__':unittest.main()

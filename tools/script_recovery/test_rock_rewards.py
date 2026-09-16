import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_rewards import recover
from tools.script_recovery.rock_rewards_native import execute
from tools.script_recovery.lift_native_lua import RData,ROOT


def run(added=False,cancel=False,first=0,second=16,replace_definition=False,error_item=0):
    source,_=recover();lua=LuaRuntime();lua.execute(source);q,me=lua.table(),lua.table();events=[]
    state={'added':added,'live':True,'fields':{1:first,2:second},'calls':0};pool={-1:'',0:'REWARD_A',16:'REWARD_B',32:'REWARD_CHANGED'}
    def getter(_,name):assert name=='AddedItemsToRockTroll';return state['added']
    q.GetStateBool=getter
    q.IsActiveThreadTerminating=lambda _:(events.append(('term',cancel)),cancel)[1]
    def consume(_,target,index):
        assert state['live'] and lua.eval('rawequal')(target,me)
        events.append(('definition',0x730+(index-1)*4));name=pool[state['fields'][index]];events.append(('string.new',name))
        try:
            events.append(('add',name));state['calls']+=1
            if state['calls']==1 and replace_definition:state['fields']={2:32}
            if state['calls']==error_item:raise RuntimeError('consumer error')
        finally:events.append(('string.destroy',name))
    q.AddRockTrollReward=consume
    def setter(_,name,value):assert name=='AddedItemsToRockTroll';state['added']=value;events.append(('state',value))
    q.SetStateBool=setter
    continuation=lambda:events.append(('continuation',))
    try:lua.globals().WithRockTrollRewardsPhase(q,me,continuation)
    except RuntimeError:
        if not error_item:raise
    # This phase is inside the outer self-resource continuation.
    if not events or events[-1]!=('continuation',):state['live']=False;events.append(('self.destroy',))
    return events,state['added']


class RewardTests(unittest.TestCase):
    def test_original_conversion_and_phase_traces(self):
        for added in (False,True):
            for cancel in (False,True):
                for first,second in ((0,16),(-1,16),(0,-1),(-1,-1)):
                    for replace in (False,True):
                        with self.subTest(added=added,cancel=cancel,first=first,second=second,replace=replace):
                            self.assertEqual(run(added,cancel,first,second,replace),execute(added,cancel,first,second,replace))

    def test_reload_definition_between_consumers_and_keep_empty_item(self):
        events,added=run(first=-1,replace_definition=True)
        self.assertTrue(added)
        self.assertEqual([event for event in events if event[0]=='add'],[('add',''),('add','REWARD_CHANGED')])
        self.assertEqual(events[-3:],[('string.destroy','REWARD_CHANGED'),('state',True),('continuation',)])

    def test_consumer_error_destroys_text_then_self_without_marking_done(self):
        for index in (1,2):
            events,added=run(error_item=index);self.assertFalse(added)
            self.assertEqual(events[-1],('self.destroy',));self.assertEqual(events[-2][0],'string.destroy')
            self.assertEqual(sum(event[0]=='add' for event in events),index)

    def test_native_field_token_lookup_target_and_state_mutations(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for address in (0xec4ba5,0xec4bba,0xec4bca,0xec4bfd,0x415d79,0x9d49b5,0x9d49d3,0xf308dd):
            data=Changed();data.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError):recover(data)

    def test_generated_reward_phase_keeps_outer_self_resource_owned(self):
        from tools.script_recovery.rock_state_candidate import generate
        generate();lua=LuaRuntime()
        lua.execute((ROOT/'work/rock_trigger_converter/state_candidate/RockTrollFirstEncounter/Phases/RTFE_RockTroll.resources.lua').read_text())
        lua.execute('''
            held=false; added=false; count=0; continued=false; me={}
            resources={NewResource=function() return 71 end,PrepareResource=function() end,
                TryAcquire=function(_,id,actor,priority) assert(id==71 and actor==me and priority==4);held=true;return true end}
            quest={
                CreateRetainedThingThread=function(_,name,actor,flag,region)
                    assert(name=='WatchForRockTrollKilled' and actor==me and flag==false and region=='')
                end,
                WithRetailResources=function(_,callback) callback(resources);held=false end,
                IsActiveThreadTerminating=function() return false end,
                GetStateBool=function(_,name) assert(name=='AddedItemsToRockTroll');return added end,
                SetStateBool=function(_,name,value) assert(held and count==2 and name=='AddedItemsToRockTroll');added=value end,
                AddRockTrollReward=function(_,actor,index) assert(held and actor==me);count=count+1;assert(index==count) end
            }
            WithRockTrollSelfPhase(quest,me,function(owner,self_id)
                assert(owner==resources and self_id==71 and held)
                WithRockTrollRewardsPhase(quest,me,function() assert(held and added);continued=true end)
                assert(held)
            end)
            assert(continued and added and not held)
        ''')


if __name__=='__main__':unittest.main()

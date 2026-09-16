import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_killed import recover
from tools.script_recovery.rock_killed_native import execute
from tools.script_recovery.lift_native_lua import RData,ROOT


def run(killed_poll=1,stop=100,empty=False,info=True,healthbar=0x12345678,error_at=None):
    body,_=recover();lua=LuaRuntime();lua.execute('function WatchForRockTrollKilled(quest,troll)\n'+body+'end')
    q,actor,troll=lua.table(),lua.table(),lua.table();events=[];state={'queries':0,'polls':0,'MissionOver':False,'refs':2 if info else 0,'open':False}
    def condition(_):
        assert state['open'];state['refs']+=int(info);events.append(('condition',))
    actor.RegisterParentAliveCondition=condition
    def poll(_,filter):
        assert state['open'] and filter=='';events.append(('filter.new',))
        try:
            if empty:return False
            state['polls']+=1;result=state['polls']>=killed_poll;events.append(('killed','',result))
            if error_at=='poll':raise RuntimeError('poll error')
            return result
        finally:events.append(('filter.destroy',))
    actor.MsgIsKilledBy=poll
    q.NewScriptFrame=lambda _:events.append(('frame',))
    def term(_):state['queries']+=1;result=state['queries']>=stop;events.append(('term',result));return result
    q.IsActiveThreadTerminating=term
    def getter(_,name):assert name=='RockTrollHealthBarID';return healthbar
    q.GetStateInt=getter
    def remove(_,id):
        assert state['open'];events.append(('remove',id))
        if error_at=='remove':raise RuntimeError('remove error')
    q.RemoveQuestInfoElement=remove
    q.DisplayQuestInfo=lambda _,flag:events.append(('display',flag))
    def setter(_,name,value):assert name=='MissionOver';state[name]=value;events.append((name,value))
    q.SetStateBool=setter
    def scope(_,source,callback):
        assert lua.eval('rawequal')(source,troll);state['open']=True
        try:callback(actor)
        finally:state['open']=False;state['refs']-=int(info);events.append(('argument.destroy',))
    q.WithCopiedThreadThing=scope
    try:lua.globals().WatchForRockTrollKilled(q,troll)
    except RuntimeError:
        if error_at is None:raise
    assert not state['open'] and state['refs']==(2 if info else 0)
    return events,state['MissionOver']


class KilledTests(unittest.TestCase):
    def test_native_trace_polls_cancellation_empty_and_retained_argument(self):
        for killed in (1,3):
            for stop in (1,2,3,5,100):
                for empty in (False,True):
                    for info in (False,True):
                        with self.subTest(killed=killed,stop=stop,empty=empty,info=info):
                            self.assertEqual(run(killed,stop,empty,info),execute(killed,stop,empty,info))

    def test_completion_full_int_order_and_error_cleanup(self):
        for id in (0x12345678,-1,-2147483648):
            events,over=run(healthbar=id);self.assertTrue(over)
            self.assertEqual(events[-4:],[('remove',id),('display',False),('MissionOver',True),('argument.destroy',)])
            self.assertEqual((events,over),execute(healthbar=id))
        for error in ('poll','remove'):
            events,over=run(error_at=error);self.assertFalse(over)
            self.assertEqual(events[-1],('argument.destroy',))

    def test_native_width_flags_filter_and_parent_condition_mutations_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for address in (0xec4a8d,0xec5033,0xec50a3,0xec50c3,0xec5107,0xec5119,0xec5123):
            data=Changed();data.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError):recover(data)

    def test_composed_helper_and_root_share_mission_and_full_width_id(self):
        from tools.script_recovery.rock_state_candidate import generate
        report=generate();self.assertNotIn('WatchForRockTrollKilled',report['missingFunctions'])
        lua=LuaRuntime();lua.execute((ROOT/'work/rock_trigger_converter/state_candidate/RockTrollFirstEncounter/RockTrollFirstEncounter.lua').read_text())
        lua.execute('''
            state={Activated=true,MissionOver=false,RockTrollHealthBarID=0x65432109}
            removes={}; displays=0; closed=false
            quest={
                GetStateBool=function(_,key) return state[key] end,
                SetStateBool=function(_,key,value) state[key]=value end,
                GetStateInt=function(_,key) return state[key] end,
                WithCopiedThreadThing=function(_,troll,callback)
                    callback({RegisterParentAliveCondition=function() end,
                        MsgIsKilledBy=function(_,filter) assert(filter=='');return true end})
                    closed=true
                end,
                NewScriptFrame=function() end, IsActiveThreadTerminating=function() return false end,
                RemoveQuestInfoElement=function(_,id) table.insert(removes,id) end,
                DisplayQuestInfo=function(_,value) assert(value==false);displays=displays+1 end,
                AddEntityBinding=function() end, FinalizeEntityBindings=function() end,
                IsRegionLoaded=function() return true end,
                CreateThread=function(_,name,args) assert(name=='WatchForRegionExit' and args.region=='') end,
                WithRegionLoadedMessage=function(_,callback) callback({Poll=function() return true end}) end,
                SetCreatureGeneratorsEnabled=function(_,region,enabled) assert(region=='Witchwood1' and enabled) end,
                DeactivateQuestLater=function(_,name,delay) assert(delay==0) end,
                GetActiveQuestName=function() return 'V_RockTrollFirstEncounter' end
            }
            WatchForRockTrollKilled(quest,{})
            assert(closed and state.MissionOver and #removes==1)
            Main(quest)
            assert(#removes==2 and displays==2)
            assert(removes[1]==0x65432109 and removes[2]==0x65432109)
        ''')


if __name__=='__main__':unittest.main()

import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_root_recovery import generate,recover
from tools.script_recovery.lift_native_lua import ROOT,RData


def run(stop=100,region=True,over=True,activated=False,messages=((True,''),)):
    generate();lua=LuaRuntime();lua.execute((ROOT/'work/rock_trigger_converter/root_candidate/RockTrollFirstEncounter.lua').read_text())
    quest=lua.table();events=[];state={'queries':0,'frames':0,'Activated':activated,'MissionOver':over,'polls':0}
    quest.AddEntityBinding=lambda _,*args:events.append(['bind',*args])
    quest.FinalizeEntityBindings=lambda _:events.append(['finalize'])
    quest.IsRegionLoaded=lambda _,name:(events.append(['region',name]),region or state['frames']>0)[1]
    def frame(_):state['frames']+=1;events.append(['frame']);state['MissionOver']=True
    quest.NewScriptFrame=frame
    def term(_):
        state['queries']+=1;value=state['queries']>=stop;events.append(['term',value]);return value
    quest.IsActiveThreadTerminating=term
    quest.GetStateBool=lambda _,name:state[name]
    quest.GetStateInt=lambda _,name:0x12345678 if name=='RockTrollHealthBarID' else None
    def setter(_,name,value):state[name]=value;events.append(['state',name,value])
    quest.SetStateBool=setter
    def thread(_,name,args):assert args['region']=='' and args['args'] is None;events.append(['thread',name,''])
    quest.CreateThread=thread
    for api,event in [('ActivateQuest','activate'),('RemoveQuestInfoElement','remove.info'),('DisplayQuestInfo','display'),('SetCreatureGeneratorsEnabled','generators'),('DeactivateQuestLater','deactivate')]:
        quest[api]=lambda _,*args,event=event:events.append([event,*args])
    quest.GetActiveQuestName=lambda _:'V_RockTrollFirstEncounter'
    def with_message(_,callback):
        events.append(['message.new']);holder=lua.table()
        def poll(_):
            received,name=messages[min(state['polls'],len(messages)-1)];state['polls']+=1
            events.append(['message.poll',received,name]);return received
        holder.Poll=poll
        try:callback(holder)
        finally:events.append(['message.destroy'])
    quest.WithRegionLoadedMessage=with_message
    lua.globals().Main(quest)
    return events,state


class RockRootRecoveryTests(unittest.TestCase):
    def test_full_width_id_display_and_both_zero_delays(self):
        events,state=run()
        self.assertEqual(events[:4],[
            ['bind','RTFE_Sparrow','RockTrollFirstEncounter/Entities/RTFE_Sparrow',0],
            ['bind','RTFE_RockTroll','RockTrollFirstEncounter/Entities/RTFE_RockTroll',0],
            ['bind','M_RTFERockTrollTrigger','RockTrollFirstEncounter/Entities/M_RTFERockTrollTrigger',0],['finalize']])
        self.assertEqual(events[4:],[['region','Witchwood1'],['term',False],['thread','WatchForRegionExit',''],
            ['term',False],['activate','V_RockTrollFirstEncounter_Activate'],['state','Activated',True],['term',False],
            ['remove.info',0x12345678],['display',False],['message.new'],['message.poll',True,''],['term',False],
            ['generators','Witchwood1',True],['deactivate','V_RockTrollFirstEncounter_Activate',0],
            ['deactivate','V_RockTrollFirstEncounter',0],['message.destroy']])

    def test_false_populated_message_does_not_end_wait(self):
        events,_=run(messages=((False,'Witchwood1'),(True,'')))
        start=events.index(['message.new'])
        self.assertEqual(events[start:start+7],[['message.new'],['message.poll',False,'Witchwood1'],
            ['frame'],['term',False],['message.poll',True,''],['term',False],['generators','Witchwood1',True]])
        self.assertEqual(events.count(['message.new']),1)
        self.assertEqual(events[-1],['message.destroy'])

    def test_cancellation_at_each_wait_boundary(self):
        for stop in (1,2,3):
            events,_=run(stop=stop)
            self.assertFalse(any(event[0]=='message.new' for event in events))
            self.assertEqual(events[-1],['term',True])
        for messages in (((False,''),),((True,''),)):
            events,_=run(stop=4,messages=messages)
            self.assertEqual(events[-2:],[['term',True],['message.destroy']])
            self.assertFalse(any(event[0]=='deactivate' for event in events))
        events,_=run(stop=1,region=False)
        self.assertEqual(events[-3:],[['region','Witchwood1'],['frame'],['term',True]])

    def test_already_activated_and_delayed_mission(self):
        events,_=run(activated=True,over=False)
        self.assertFalse(any(event[0]=='activate' for event in events))
        self.assertEqual(events.count(['frame']),1)
        self.assertEqual(events[-1],['message.destroy'])

    def test_native_operand_mutation_rejected(self):
        real=RData()
        for address in (0xec4031,0xec4042,0xec40f2,0xec405d):
            class Changed:
                string_at=real.string_at
                def bytes_at(self,start,size):
                    raw=real.bytes_at(start,size)
                    if raw is not None and start<=address<start+size:
                        raw=bytearray(raw);raw[address-start]^=1;raw=bytes(raw)
                    return raw
            with self.subTest(address=hex(address)),self.assertRaisesRegex(ValueError,'native bytes changed'):
                recover(Changed())

    def test_candidate_reports_whole_port_limits(self):
        report=generate();self.assertFalse(report['registrationEnabled'])
        self.assertIn('WithRegionLoadedMessage',report['requiredCapability'])
        self.assertIn('full32-bit',report['stateContract']['RockTrollHealthBarID'])
        with self.assertRaises(ValueError):generate(ROOT/'refs/script_recovery/lifted/RockTrollFirstEncounter')


if __name__=='__main__':unittest.main()

import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_trigger_recovery import generate,recover
from tools.script_recovery.lift_native_lua import ROOT,RData


def run_case(stop=100,triggered=False,distances=(3,),radii=(5,),empty=False):
    generate();lua=LuaRuntime()
    lua.execute((ROOT/'work/rock_trigger_converter/draft/M_RTFERockTrollTrigger.lua').read_text())
    quest,me,resources=(lua.table() for _ in range(3));events=[];state={'queries':0,'polls':0,'triggered':triggered,'live':False}
    quest.RegisterBoundAliveCondition=lambda _:events.append(['alive.condition'])
    quest.NewScriptFrame=lambda _:events.append(['frame'])
    def term(_):
        state['queries']+=1;value=state['queries']>=stop;events.append(['term',value]);return value
    quest.IsActiveThreadTerminating=term
    quest.GetStateBool=lambda _,name:state['triggered'] if name=='RockTrollTriggered' else None
    def setter(_,name,value):
        assert name=='RockTrollTriggered';state['triggered']=value;events.append(['triggered',value])
    quest.SetStateBool=setter
    def proximity(_):
        value=radii[min(state['polls'],len(radii)-1)];events.append(['radius',value]);return value
    quest.GetRockTrollTriggerProximity=proximity
    quest.GetHero=lambda _:(events.append(['hero']),'hero')[1]
    def near(_,hero,target,radius):
        assert hero=='hero' and lua.eval('rawequal')(target,me)
        d=distances[min(state['polls'],len(distances)-1)];state['polls']+=1
        events.append(['distance','hero','bound.trigger',radius]);return d<radius
    quest.IsDistanceBetweenThingsUnder=near
    def lookup(_,name):
        assert name=='M_RTFERockTrollSpawnPos';state['live']=True
        events.append(['lookup',name,empty]);return 71
    resources.NewThingFromScriptName=lookup
    def spawn(_,marker,definition,name,flag):
        assert marker==71 and state['live'];events.append(['spawn',marker,definition,name,flag])
    resources.CreateCreatureAtThingPosition=spawn
    def destroy(_,marker):
        assert marker==71 and state['live'];state['live']=False;events.append(['marker.destroy'])
    resources.DestroyThing=destroy
    def remove(_,target,a,b):
        assert lua.eval('rawequal')(target,me);events.append(['remove','bound.trigger',a,b])
    quest.RemoveThing=remove
    quest.WithRetailResources=lambda _,callback:callback(resources)
    lua.globals().Main(quest,me)
    assert not state['live']
    return events,state


class RockTriggerRecoveryTests(unittest.TestCase):
    def test_native_target_flags_and_dynamic_poll_order(self):
        events,state=run_case(distances=(6,6),radii=(5,8))
        self.assertEqual(events,[['alive.condition'],['frame'],['term',False],['term',False],
            ['lookup','M_RTFERockTrollSpawnPos',False],['radius',5],['hero'],['distance','hero','bound.trigger',5],
            ['frame'],['term',False],['radius',8],['hero'],['distance','hero','bound.trigger',8],['term',False],
            ['spawn',71,'CREATURE_ROCK_TROLL_START_STANDING','RTFE_RockTroll',False],
            ['triggered',True],['remove','bound.trigger',False,True],['marker.destroy']])
        self.assertTrue(state['triggered'])

    def test_cancellation_and_existing_state(self):
        events,_=run_case(stop=1)
        self.assertEqual(events,[['alive.condition'],['frame'],['term',True]])
        events,_=run_case(triggered=True)
        self.assertEqual(events,[['alive.condition'],['frame'],['term',False]])
        events,_=run_case(stop=2)
        self.assertEqual(events,[['alive.condition'],['frame'],['term',False],['term',True]])
        for distances in ((3,),(9,)):
            events,state=run_case(stop=3,distances=distances)
            self.assertEqual(events[-2:],[['term',True],['marker.destroy']])
            self.assertFalse(state['triggered'])
            self.assertFalse(any(e[0]=='spawn' for e in events))

    def test_empty_lookup_still_reaches_native_spawn_consumer(self):
        events,state=run_case(empty=True)
        self.assertIn(['lookup','M_RTFERockTrollSpawnPos',True],events)
        self.assertTrue(state['triggered'])
        self.assertEqual(events[-4:],[['spawn',71,'CREATURE_ROCK_TROLL_START_STANDING','RTFE_RockTroll',False],
            ['triggered',True],['remove','bound.trigger',False,True],['marker.destroy']])

    def test_changed_native_operands_and_strings_reject(self):
        real=RData()
        for address in (0xec4434,0xec4389,0xf308c2,0xec446a):
            class Changed:
                string_at=real.string_at
                def bytes_at(self,start,size):
                    raw=real.bytes_at(start,size)
                    if raw is not None and start<=address<start+size:
                        raw=bytearray(raw);raw[address-start]^=1;raw=bytes(raw)
                    return raw
            with self.subTest(address=hex(address)),self.assertRaisesRegex(ValueError,'native bytes changed'):
                recover(Changed())
        class Literal:
            bytes_at=real.bytes_at
            def string_at(self,address):return 'changed' if address==0x12f1688 else real.string_at(address)
        with self.assertRaisesRegex(ValueError,'literal changed'):recover(Literal())

    def test_disabled_scope_and_capability_report(self):
        report=generate()
        self.assertFalse(report['registrationEnabled'])
        self.assertEqual(len(report['requiredCapabilities']),2)
        with self.assertRaises(ValueError):generate(ROOT/'refs/script_recovery/lifted/RockTrollFirstEncounter')


if __name__=='__main__':unittest.main()

import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.bully_main_dialogue import SOURCE
from tools.script_recovery.bully_main_dialogue_native import execute
from tools.script_recovery.bully_full_resource_candidate import generate

def lua_case(done=False,attacked=False,said=False,hits=0,health=1.0,busy=0,cancel=999):
    lua=LuaRuntime();lua.execute(recover()[0]+SOURCE);q,r,state=lua.table(),lua.table(),lua.table()
    fields={'DoneIntro':done,'SaidPieceAboutAttackingVictim':said,'HitsTaken':hits};trace=[];queries=0;remaining=busy
    state.GetStateBool=lambda _,k:fields[k];state.GetStateInt=lambda _,k:fields[k]
    def set_state(_,k,v):fields[k]=v;trace.append(('state',k,v))
    state.SetStateBool=set_state;q.GetStateBool=lambda _,k:attacked if k=='HeroAttackedVictim' else None
    def term(_):
        nonlocal queries
        queries+=1;value=queries>=cancel;trace.append(('term',value));return value
    q.IsActiveThreadTerminating=term;q.GetHero=lambda _:trace.append(('hero',)) or 'hero';q.NewScriptFrame=lambda _:trace.append(('frame',))
    r.NewThingFromResource=lambda _,c:trace.append(('thing.new',)) or 7
    r.ThingHealth=lambda _,a:trace.append(('health',)) or health
    r.DestroyThing=lambda _,a:trace.append(('thing.destroy',))
    def speak(_,control,hero,key,selection,listen,sound,fade):
        assert (control,hero,selection,listen,sound,fade)==(1,'hero',0,False,True,False);trace.append(('speak',key))
    r.Speak=speak
    def task(_,control):
        nonlocal remaining
        value=remaining>0;remaining-=1;trace.append(('busy',value));return value
    r.IsPerformingScriptTask=task
    result=lua.globals().BullyMainDialogue(q,r,1,state)
    return result,trace

class BullyMainDialogueTests(unittest.TestCase):
    def test_five_original_dialogues_health_waits_and_all_query_cancellations(self):
        branches=[(False,False,False,0),(True,False,False,0),(True,True,False,0),(True,True,True,2),(True,True,True,3)]
        for branch,health,busy,cancel in itertools.product(branches,(1.0,0.0,float('nan')),(0,2),(1,2,3,4,5,6,7,999)):
            case=dict(zip(('done','attacked','said','hits'),branch),health=health,busy=busy,cancel=cancel)
            with self.subTest(case=case):self.assertEqual(execute(**case),lua_case(**case))

    def test_structured_dialogue_replaces_five_staging_blocks_in_body(self):
        source,report=generate()
        self.assertIn('alive = BullyMainDialogue(quest, resources, bully_control, __native_entity_state)',source)
        self.assertNotIn('::LAB_00dbc289::',source)
        self.assertNotIn('::LAB_00dbc851::',source)
        self.assertEqual(report['mainDialogue']['cancelDestinations'],[0xdbc1c3,0xdbc851])

if __name__=='__main__':unittest.main()

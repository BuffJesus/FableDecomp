import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.victim_talk import SOURCE,generate,recover
from tools.script_recovery.victim_talk_native import execute
from tools.script_recovery.lift_native_lua import RData

def lua_case(talk=True,subdued=False,attacked=False,displayed=False,xbox=False,health=1.0,busy=0,clicked_after=0,failures=0,cancel=999,prepare=False,populated=True,fault=None,cleanup_fault=False):
    lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();r=lua.table();s=lua.table();events=[];state={'queries':0,'acquires':0,'clicks':0,'busy':0,'displayed':displayed}
    def term(_):state['queries']+=1;value=state['queries']>=cancel;events.append(('term',value));return value
    def prepare_fn(_,control):
        assert control==1;events.append(('prepare',prepare))
        if prepare:events.append(('prepare.release',))
    def acquire(_,control,actor,priority):assert (control,actor,priority)==(1,7,4);value=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',value,populated));return value
    def get(_,key):value={'BullySubdued':subdued,'HeroAttackedVictim':attacked,'DisplayedGameInfo':state['displayed']}[key];events.append(('get',key,value));return value
    def put(_,key,value):assert key=='DisplayedGameInfo';state['displayed']=value;events.append(('set',key,value))
    def busy_fn(_,control):assert control==1;value=state['busy']>0;state['busy']-=1;events.append(('busy',value));return value
    def speak(_,control,hero,key,*flags):assert (control,hero,flags)==(1,8,(0,False,True,False));events.append(('speak',key));state['busy']=busy
    def clicked(_):value=state['clicks']>=clicked_after;state['clicks']+=1;events.append(('clicked',value));return value
    q.GetStateBool=get;s.GetStateBool=get;s.SetStateBool=put;q.IsActiveThreadTerminating=term;q.NewScriptFrame=lambda _:events.append(('frame',));q.GetHero=lambda _:events.append(('hero',)) or 8
    q.PauseAllNonScriptedEntities=lambda _,v:events.append(('pause',v));q.IsXbox=lambda _:events.append(('xbox',xbox)) or xbox;q.MsgIsGameInfoClickedPast=clicked
    r.IsTalkedToByHero=lambda _,actor:events.extend([('text.new','SCRIPT_NAME_HERO'),('talk',talk),('text.destroy','SCRIPT_NAME_HERO')]) or talk
    r.SetRawScared=lambda _,actor,v:events.append(('scared',v));r.VictimFaceHero=lambda _,actor,snap:events.extend([('hero',),('face','hero',snap)]);r.FaceTowardsRetainedThing=lambda _,actor,bully,snap:events.append(('face','bully',snap))
    r.NewMovie=lambda _:events.append(('movie.new',)) or 3;r.StartOwnedMovie=lambda _,movie,key:events.extend([('text.new',key),('movie.start',),('text.destroy',key)]);r.DestroyMovie=lambda _,movie:events.append(('movie.destroy',))
    r.PrepareResource=prepare_fn;r.TryAcquire=acquire;r.NewThingFromResource=lambda _,control:events.append(('thing.new',populated)) or 4;r.ThingHealth=lambda _,thing:events.append(('health',)) or health;r.DestroyThing=lambda _,thing:events.append(('thing.destroy',));r.Speak=speak;r.IsPerformingScriptTask=busy_fn
    r.DisplayRawGameInfo=lambda _,key:events.extend([('text.new',key),('info',key),('text.destroy',key)])
    wrap=lua.eval('function(f,key) return function(...) f(...); error(key,0) end end')
    if fault:r[fault]=wrap(r[fault],'BODY ERROR')
    if cleanup_fault:r.DestroyMovie=wrap(r.DestroyMovie,'CLEANUP ERROR')
    try:complete=lua.globals().VictimTalk(q,7,r,1,2,s)
    except Exception as error:complete=str(error)
    return complete,events,state['displayed']

class VictimTalkTests(unittest.TestCase):
    def test_original_talk_dialogue_health_ui_and_movie_cleanup(self):
        generate();cases=0;policies=((0,0,0,False,True),(2,2,0,True,True),(0,2,2,False,False),(2,0,2,True,False))
        for subdued,attacked,displayed,xbox,health,cancel,policy in itertools.product((False,True),(False,True),(False,True),(False,True),(0.0,1.0,float('nan')),(1,2,3,4,5,6,7,8,9,10,999),policies):
            busy,clicked,failures,prepare,populated=policy;args=(True,subdued,attacked,displayed,xbox,health,busy,clicked,failures,cancel,prepare,populated)
            with self.subTest(args=args):self.assertEqual(execute(*args),lua_case(*args));cases+=1
        self.assertEqual(cases,2112)
        for subdued,attacked,displayed in itertools.product((False,True),repeat=3):self.assertEqual(execute(False,subdued,attacked,displayed),lua_case(False,subdued,attacked,displayed))
    def test_health_errors_destroy_thing_before_unpause_and_movie(self):
        for fault,cleanup in itertools.product(('ThingHealth','Speak','TryAcquire'),(False,True)):
            result,events,displayed=lua_case(fault=fault,cleanup_fault=cleanup)
            self.assertIn('BODY ERROR',result);self.assertFalse(displayed);self.assertEqual(events[-2:],[('pause',False),('movie.destroy',)])
            if fault=='ThingHealth':self.assertEqual(events[-3],('thing.destroy',))
    def test_changed_health_flags_threshold_and_text_fail_closed(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=bytearray(original.bytes_at(at,size))
                if at<=self.at<at+size:raw[self.at-at]^=1
                return bytes(raw)
            def string_at(self,at):return 'CHANGED' if at==self.at else original.string_at(at)
        for address in (0xdbd348,0xdbd456,0xdbd4b8,0xdbd60b,0x122dedc,0x12d9c60):
            with self.subTest(address=address),self.assertRaises(ValueError):recover(Changed(address))

if __name__=='__main__':unittest.main()

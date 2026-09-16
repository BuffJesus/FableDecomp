import itertools,tempfile,unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.live_father_hit import SOURCE,recover,DRAFT,generate
from tools.script_recovery.live_father_hit_native import execute
from tools.script_recovery.lift_native_lua import RData

def lua_case(answers=(True,False,False),health=1.0,busy=0,failures=0,prepare=False,cancel=999,populated=True,fault=None,cleanup_fault=False):
    lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();r=lua.table();events=[];state={'query':0,'acquire':0,'busy':0}
    def predicate(_,actor,ability):
        assert actor==7 and ability==14
        count=1 if answers[0] else (3 if answers[1] else 2)
        for i in range(count):events.extend([('text.new','SCRIPT_NAME_HERO'),('predicate',i,answers[i])])
        events.extend([('text.destroy','SCRIPT_NAME_HERO')]*count)
        return answers[0] or (answers[1] and not answers[2])
    def term(_):state['query']+=1;value=state['query']>=cancel;events.append(('term',value));return value
    def prep(_,control):
        assert control==1;events.append(('prepare',prepare))
        if prepare:events.append(('prepare.release',))
    def acquire(_,control,actor,priority):
        assert (control,actor,priority)==(1,7,4);value=state['acquire']>=failures;state['acquire']+=1;events.append(('acquire',value,populated));return value
    def speak(_,control,hero,key,*flags):
        assert (control,hero,flags)==(1,8,(0,False,True,False));events.append(('speak',key));state['busy']=busy
    def task(_,control):assert control==1;value=state['busy']>0;state['busy']-=1;events.append(('busy',value));return value
    q.IsActiveThreadTerminating=term;q.GetHero=lambda _:events.append(('hero',)) or 8;q.NewScriptFrame=lambda _:events.append(('frame',))
    q.PauseAllNonScriptedEntities=lambda _,value:events.append(('pause',value))
    r.IsHitByHeroExceptAbility=predicate;r.SetThingAsAlly=lambda _,first,second:events.append(('ally','father' if first==7 else 'hero','father' if second==7 else 'hero'))
    r.NewMovie=lambda _:events.append(('movie.new',)) or 5
    r.StartOwnedMovie=lambda _,movie,key:events.extend([('text.new',''),('movie.start',),('text.destroy','')])
    r.DestroyMovie=lambda _,movie:events.append(('movie.destroy',));r.PrepareResource=prep;r.TryAcquire=acquire
    r.NewThingFromResource=lambda _,control:events.append(('thing.new',not populated)) or 9
    r.ThingHealth=lambda _,actor:events.append(('health',)) or health;r.DestroyThing=lambda _,actor:events.append(('thing.destroy',));r.Speak=speak;r.IsPerformingScriptTask=task
    if fault:
        obj,name={'start':(r,'StartOwnedMovie'),'pause':(q,'PauseAllNonScriptedEntities'),'acquire':(r,'TryAcquire'),'health':(r,'ThingHealth'),'speak':(r,'Speak')}[fault]
        obj[name]=lua.eval('function(f,key,pause) return function(self,...) local result=f(self,...); if not pause or select(1,...) then error("BODY "..key,0) end; return result end end')(obj[name],fault,fault=='pause')
    if cleanup_fault:
        q.PauseAllNonScriptedEntities=lua.eval('function(f) return function(self,value) f(self,value); if not value then error("CLEANUP",0) end end end')(q.PauseAllNonScriptedEntities)
        r.DestroyMovie=lua.eval('function(f) return function(...) f(...); error("CLEANUP",0) end end')(r.DestroyMovie)
    try:return lua.globals().LiveFatherHandleHit(q,7,r,1,lambda amount:events.append(('badDeed',amount))),events
    except Exception as error:return str(error),events

class LiveFatherHitTests(unittest.TestCase):
    def test_original_predicate_movie_before_acquisition_health_waits_and_cleanup(self):
        recover();cases=0
        for answers,health,busy,failures,prepare,cancel,populated in itertools.product(itertools.product((False,True),repeat=3),(0.0,1.0,float('nan')),(0,2),(0,2),(False,True),(1,2,3,4,5,999),(False,True)):
            with self.subTest(answers=answers,health=health,busy=busy,failures=failures,prepare=prepare,cancel=cancel,populated=populated):
                args=(answers,health,busy,failures,prepare,cancel,populated)
                self.assertEqual(execute(*args,mask=256),lua_case(*args));cases+=1
        self.assertEqual(cases,2304)

    def test_callback_errors_preserve_inner_thing_and_movie_cleanup_order(self):
        for fault,cleanup in itertools.product(('start','pause','acquire','health','speak'),(False,True)):
            with self.subTest(fault=fault,cleanup=cleanup):
                error,events=lua_case(fault=fault,cleanup_fault=cleanup)
                self.assertIn('BODY '+fault,error);self.assertEqual(events[-1],('movie.destroy',))
                if fault!='start':self.assertIn(('pause',False),events)
                if fault in ('health','speak'):self.assertLess(events.index(('thing.destroy',)),events.index(('pause',False)))

    def test_changed_operands_threshold_and_source_are_rejected(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=bytearray(original.bytes_at(at,size))
                if at<=self.at<at+size:raw[self.at-at]^=1
                return bytes(raw)
            def string_at(self,at):return original.string_at(at)
        for address in (0xdb950e,0xdb9595,0xdb95b5,0xdb95f8,0xdb961d,0xdb9677,0xdb968a,0xdb96b4,0xdb9770,0x122dedc):
            with self.subTest(address=address),self.assertRaises(ValueError):recover(Changed(address))
        with tempfile.TemporaryDirectory() as directory:
            path=Path(directory)/'changed.lua';path.write_text(DRAFT.read_text()+'\n')
            with self.assertRaises(ValueError):recover(draft_path=path)
        source,report=generate();self.assertIn('false, true, false)',source);self.assertEqual(report['movieOffset'],100)

if __name__=='__main__':unittest.main()

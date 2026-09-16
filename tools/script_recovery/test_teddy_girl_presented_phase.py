import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.teddy_girl_health import SOURCE as HEALTH
from tools.script_recovery.teddy_girl_question import SOURCE as QUESTION
from tools.script_recovery.teddy_girl_presented_phase import recover
from tools.script_recovery.teddy_girl_question_native import execute
from tools.script_recovery.teddy_girl_hit import SOURCE as HIT

class TeddyGirlPresentedPhaseTests(unittest.TestCase):
    def test_native_acquisition_movie_and_cancel_joins(self):
        source,_=recover()
        for kind,found,health,busy,failures,prepare,cancel in itertools.product(('other','teddy','hit'),(False,True),(0.0,1.0,float('nan')),(0,2),(0,2),(False,True),(1,2,3,4,6,999)):
            with self.subTest(kind=kind,found=found,health=health,busy=busy,failures=failures,prepare=prepare,cancel=cancel):
                lua=LuaRuntime();lua.execute(HEALTH+QUESTION+source+HIT);q=lua.table();r=lua.table();s=lua.table();events=[];counters={'term':0,'acquire':0,'busy':0}
                def term(_):counters['term']+=1;value=counters['term']>=cancel;events.append(('term',value));return value
                def acquire(_,control,me,priority):
                    self.assertEqual((control,me,priority),(1,7,4));value=counters['acquire']>=failures;counters['acquire']+=1;events.append(('acquire',value));return value
                def prep(_,control):
                    events.append(('prepare.test',prepare))
                    if prepare:events.append(('prepare.release',))
                def speak(_,control,hero,key,*flags):
                    self.assertEqual(flags,(0,False,True,False));events.append(('speak',key));counters['busy']=busy
                def task(_,control):value=counters['busy']>0;counters['busy']-=1;events.append(('busy',value));return value
                q.IsActiveThreadTerminating=term;q.NewScriptFrame=lambda _:events.append(('frame',));q.GetHero=lambda _:events.append(('hero',)) or 3
                q.PauseAllNonScriptedEntities=lambda _,value:events.append(('pause',value))
                r.PrepareResource=prep;r.TryAcquire=acquire;r.NewMovie=lambda _:events.append(('movie.new',)) or 6
                r.StartOwnedMovie=lambda _,movie,key:events.extend([('text.new',''),('movie.start',),('text.destroy','')])
                r.DestroyMovie=lambda _,movie:events.append(('movie.destroy',))
                r.SetThingAsAlly=lambda _,first,second:events.append(('ally','girl' if first==7 else 'hero','girl' if second==7 else 'hero'))
                r.NewThingFromResource=lambda _,control:events.append(('thing.new',)) or 8;r.ThingHealth=lambda _,actor:events.append(('health',)) or health
                r.DestroyThing=lambda _,actor:events.append(('thing.destroy',));r.Speak=speak;r.IsPerformingScriptTask=task
                s.GetStateBool=lambda _,key:events.append(('get',key)) or found;s.SetStateBool=lambda _,key,value:events.append(('set',key,value))
                if kind=='hit':result=lua.globals().TeddyGirlHit(q,r,7,1,s,lambda amount:events.append(('badDeed',amount)))
                else:result=lua.globals().TeddyGirlPresentedResponse(q,r,7,1,s,kind,lambda:events.append(('given',)))
                native=execute(health=health,busy=busy,cancel=cancel,found=found,presented=None if kind=='hit' else kind,hit_phase=kind=='hit',failures=failures,prepare=prepare)
                self.assertEqual((result,events),native)

if __name__=='__main__':unittest.main()

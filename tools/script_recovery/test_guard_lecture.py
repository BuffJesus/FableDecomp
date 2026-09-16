import itertools
import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.guard_health import SOURCE as HEALTH
from tools.script_recovery.guard_lecture import recover
from tools.script_recovery.guard_lecture_native import execute
LECTURE=recover()[0]

def lua_case(repeated=False,crimes=0,health=1.0,busy=0,cancel=999):
    lua=LuaRuntime();lua.execute(HEALTH+LECTURE);q=lua.table();r=lua.table();events=[];queries=0;remaining=0
    def get(_,key):
        events.append(('get',key));return repeated if key=='GuardsSpokenOnce' else bool(crimes&(1<<int(key[-1])))
    def term(_):
        nonlocal queries
        queries+=1;value=queries>=cancel;events.append(('term',value));return value
    def speak(_,control,hero,key,*flags):
        nonlocal remaining
        assert (control,hero,flags)==(1,7,(0,False,True,False));events.append(('speak',key));remaining=busy
    def task(_,control):
        nonlocal remaining
        value=remaining>0;remaining-=1;events.append(('busy',value));return value
    q.GetStateBool=get;q.SetStateBool=lambda _,k,v:events.append(('set',k,v));q.IsActiveThreadTerminating=term
    q.NewScriptFrame=lambda _:events.append(('frame',));q.GetHero=lambda _:events.append(('hero',)) or 7
    r.NewThingFromResource=lambda _,c:events.append(('thing.new',)) or 9
    r.ThingHealth=lambda _,a:events.append(('health',)) or health
    r.DestroyThing=lambda _,a:events.append(('thing.destroy',));r.Speak=speak;r.IsPerformingScriptTask=task
    return lua.globals().GuardLecture(q,r,1),events

class GuardLectureTests(unittest.TestCase):
    def test_original_first_repeat_crime_combinations(self):
        for repeated,crimes in itertools.product((False,True),range(32)):
            with self.subTest(repeated=repeated,crimes=crimes):
                case=dict(repeated=repeated,crimes=crimes)
                self.assertEqual(execute(**case),lua_case(**case))

    def test_health_wait_and_cancellation_queries(self):
        for repeated,health,busy,cancel in itertools.product((False,True),(0.0,float('nan'),1.0),(0,2),(1,2,3,5,8,12,20,999)):
            with self.subTest(repeated=repeated,health=health,busy=busy,cancel=cancel):
                case=dict(repeated=repeated,crimes=31,health=health,busy=busy,cancel=cancel)
                self.assertEqual(execute(**case),lua_case(**case))

if __name__=='__main__':unittest.main()

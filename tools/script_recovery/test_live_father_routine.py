import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.live_father_candidate import BODY,generate
from tools.script_recovery.live_father_routine_native import execute

def lua_case(talk=True,cancel=12,failures=0,prepare=False,payment=True,hit=True,populated=True,fault=None,cleanup_fault=False):
    lua=LuaRuntime();lua.execute(BODY);q=lua.table();r=lua.table();events=[];state={'queries':0,'acquires':0}
    def term(_):state['queries']+=1;value=state['queries']>=cancel;events.append(('term',value));return value
    def prepare_fn(_,control):
        assert control==1;events.append(('prepare',prepare))
        if prepare:events.append(('prepare.release',))
    def acquire(_,control,actor,priority):
        assert (control,actor)==(1,7);value=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',priority,value,populated));return value
    def talk_fn(_,actor):assert actor==7;events.extend([('text.new','SCRIPT_NAME_HERO'),('talk',talk),('text.destroy','SCRIPT_NAME_HERO')]);return talk
    q.IsActiveThreadTerminating=term;q.NewScriptFrame=lambda _:events.append(('frame',));q.PauseAllNonScriptedEntities=lambda _,v:events.append(('pause',v))
    r.PrepareResource=prepare_fn;r.TryAcquire=acquire;r.IsTalkedToByHero=talk_fn;r.NewMovie=lambda _:events.append(('movie.new',)) or 2
    r.StartOwnedMovie=lambda _,movie,key:events.extend([('text.new',key),('movie.start',),('text.destroy',key)])
    r.DestroyMovie=lambda _,movie:events.append(('movie.destroy',))
    lua.globals().LiveFatherPaymentDialogue=lambda *args:events.append(('payment',payment)) or payment
    lua.globals().LiveFatherHandleHit=lambda *args:events.append(('hit',hit)) or hit
    wrap=lua.eval('function(f, message) return function(...) f(...); error(message,0) end end')
    if fault=='payment':lua.globals().LiveFatherPaymentDialogue=wrap(lua.globals().LiveFatherPaymentDialogue,'BODY ERROR')
    if fault=='start':r.StartOwnedMovie=wrap(r.StartOwnedMovie,'BODY ERROR')
    if cleanup_fault:r.DestroyMovie=wrap(r.DestroyMovie,'CLEANUP ERROR')
    result=None
    try:lua.globals().LiveFatherRoutine(q,7,r,1,lua.table(),lambda _:None)
    except Exception as e:result=str(e)
    events.append(('control.destroy',))
    return events,result

class LiveFatherRoutineTests(unittest.TestCase):
    def test_original_outer_branches_priorities_movie_cleanup_and_retained_control(self):
        generate();cases=0
        for args in itertools.product((False,True),range(1,17),(0,2),(False,True),(False,True),(False,True),(False,True)):
            with self.subTest(args=args):self.assertEqual(execute(*args),lua_case(*args)[0]);cases+=1
        self.assertEqual(cases,1024)
    def test_callback_errors_close_movie_before_control_and_preserve_primary(self):
        for fault,cleanup in itertools.product(('start','payment'),(False,True)):
            events,error=lua_case(fault=fault,cleanup_fault=cleanup)
            self.assertIn('BODY ERROR',error);self.assertEqual(events[-2:],[('movie.destroy',),('control.destroy',)])
            self.assertEqual(('pause',False) in events,fault=='payment')

if __name__=='__main__':unittest.main()

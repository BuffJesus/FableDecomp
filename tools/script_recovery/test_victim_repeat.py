import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.victim_repeat import SOURCE,recover,generate
from tools.script_recovery.victim_hit import SOURCE as HIT
from tools.script_recovery.victim_talk import SOURCE as TALK
from tools.script_recovery.victim_repeat_native import execute
from tools.script_recovery.lift_native_lua import RData

def lua_case(has_data=True,alive=True,health=1.0,busy=0,failures=0,cancel=999,prepare=False,conversation=-1,populated=True,fault=None,cleanup_fault=False):
    lua=LuaRuntime();lua.execute(HIT+TALK+SOURCE);q=lua.table();r=lua.table();events=[];state={'queries':0,'acquires':0,'busy':0}
    def term(_):state['queries']+=1;value=state['queries']>=cancel;events.append(('term',value));return value
    def prepare_fn(_,control):
        assert control==1;events.append(('prepare',prepare))
        if prepare:events.append(('prepare.release',))
    def acquire(_,control,actor,priority):assert (control,actor,priority)==(1,7,4);value=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',value,populated));return value
    def destroy(_,id):assert id in (3,5);events.append(('bully.destroy',) if id==3 else ('healthThing.destroy',))
    def lines(_,id,actor,bully):
        assert (id,actor,bully)==(conversation,7,3)
        for key in ('TEXT_QST_048_VICTIM_EVIL_BROS','TEXT_QST_048_BULLY_HERO_ATTACKS_VICTIM'):events.extend([('text.new',key),('line',conversation,key,'victim','bully'),('text.destroy',key)])
    def speak(_,control,hero,key,*flags):assert (control,hero,flags)==(1,8,(2,False,True,False));events.append(('speak',key,2));state['busy']=busy
    def busy_fn(_,control):assert control==1;value=state['busy']>0;state['busy']-=1;events.append(('busy',value));return value
    q.IsActiveThreadTerminating=term;q.NewScriptFrame=lambda _:events.append(('frame',));q.GetHero=lambda _:events.append(('hero',)) or 8;q.PauseAllNonScriptedEntities=lambda _,v:events.append(('pause',v))
    r.NewConversation=lambda _,actor,a,b:events.append(('conversation',conversation)) or conversation;r.NewThingFromScriptName=lambda _,key:events.extend([('text.new',key),('bully.new',has_data),('text.destroy',key)]) or 3
    r.ThingAlive=lambda _,bully:events.append(('alive',has_data and alive)) or (has_data and alive);r.DestroyThing=destroy;r.AddConversationPerson=lambda _,id,bully:events.append(('person',id,'bully'));r.AddVictimRepeatConversationLines=lines
    r.NewMovie=lambda _:events.append(('movie.new',)) or 4;r.StartOwnedMovie=lambda _,movie,key:events.extend([('text.new',key),('movie.start',),('text.destroy',key)]);r.DestroyMovie=lambda _,movie:events.append(('movie.destroy',))
    r.PrepareResource=prepare_fn;r.TryAcquire=acquire;r.NewThingFromResource=lambda _,control:events.append(('healthThing.new',populated)) or 5;r.ThingHealth=lambda _,thing:events.append(('health',)) or health;r.Speak=speak;r.IsPerformingScriptTask=busy_fn
    wrap=lua.eval('function(f,key) return function(...) f(...); error(key,0) end end')
    if fault:r[fault]=wrap(r[fault],'BODY ERROR')
    if cleanup_fault:r.DestroyMovie=wrap(r.DestroyMovie,'CLEANUP ERROR')
    try:complete=lua.globals().VictimRepeatHit(q,7,r,1)
    except Exception as error:complete=str(error)
    return complete,events

class VictimRepeatTests(unittest.TestCase):
    def test_original_conversation_alive_callee_health_and_cancel_cleanup(self):
        generate();cases=0
        for kind,health,busy,failures,cancel,prepare,id,populated in itertools.product(((False,False),(True,False),(True,True)),(0.0,1.0,float('nan')),(0,2),(0,2),(1,2,3,4,5,6,7,999),(False,True),(-1,7),(False,True)):
            args=(*kind,health,busy,failures,cancel,prepare,id,populated)
            with self.subTest(args=args):self.assertEqual(execute(*args),lua_case(*args));cases+=1
        self.assertEqual(cases,2304)
    def test_errors_preserve_primary_and_close_distinct_bully_after_movie(self):
        for fault,cleanup in itertools.product(('ThingHealth','Speak'),(False,True)):
            result,events=lua_case(False,fault=fault,cleanup_fault=cleanup)
            self.assertIn('BODY ERROR',result);self.assertEqual(events[-3:],[('pause',False),('movie.destroy',),('bully.destroy',)])
            if fault=='ThingHealth':self.assertEqual(events[-4],('healthThing.destroy',))
        result,events=lua_case(fault='AddVictimRepeatConversationLines');self.assertIn('BODY ERROR',result);self.assertEqual(events[-1],('bully.destroy',));self.assertNotIn(('movie.new',),events)
    def test_changed_alive_implementation_or_conversation_speaker_rejected(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=bytearray(original.bytes_at(at,size))
                if at<=self.at<at+size:raw[self.at-at]^=1
                return bytes(raw)
            def string_at(self,at):return original.string_at(at)
        for at in (0xdbda6b,0xdbdaa3,0xdbdbc8,0xdbdc27,0x4ab139,0x1238c8c+0x12c):
            with self.subTest(at=at),self.assertRaises(ValueError):recover(Changed(at))

if __name__=='__main__':unittest.main()

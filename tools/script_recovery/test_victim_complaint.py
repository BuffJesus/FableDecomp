import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.victim_complaint import SOURCE,generate
from tools.script_recovery.victim_hit import SOURCE as HIT
from tools.script_recovery.victim_talk import SOURCE as TALK
from tools.script_recovery.victim_complaint_native import execute
def lua_case(complaint=True,health=1.0,busy=0,failures=0,cancel=999,prepare=False,populated=True):
    lua=LuaRuntime();lua.execute(HIT+TALK+SOURCE);q=lua.table();r=lua.table();events=[];state={'queries':0,'acquires':0,'busy':0,'flag':complaint}
    def term(_):state['queries']+=1;v=state['queries']>=cancel;events.append(('term',v));return v
    def prepare_fn(_,id):
        assert id==1;events.append(('prepare',prepare))
        if prepare:events.append(('prepare.release',))
    def acquire(_,id,actor,p):assert (id,actor,p)==(1,7,4);v=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',v,populated));return v
    def put(_,key,v):assert key=='VictimComplainsAboutLosingTeddy';state['flag']=v;events.append(('set',v))
    def speak(_,id,hero,key,*flags):assert (id,hero,key,flags)==(1,8,'TEXT_QST_048_VICTIM_EVIL_BROS_10',(0,False,True,False));events.append(('speak',));state['busy']=busy
    def busy_fn(_,id):assert id==1;v=state['busy']>0;state['busy']-=1;events.append(('busy',v));return v
    q.GetStateBool=lambda _,key:events.append(('get',state['flag'])) or state['flag'];q.SetStateBool=put;q.IsActiveThreadTerminating=term;q.NewScriptFrame=lambda _:events.append(('frame',));q.GetHero=lambda _:events.append(('hero',)) or 8
    r.PrepareResource=prepare_fn;r.TryAcquire=acquire;r.NewThingFromResource=lambda _,id:events.append(('thing.new',populated)) or 3;r.ThingHealth=lambda _,id:events.append(('health',)) or health;r.DestroyThing=lambda _,id:events.append(('thing.destroy',));r.Speak=speak;r.IsPerformingScriptTask=busy_fn
    complete=lua.globals().VictimComplaint(q,7,r,1);return complete,events,state['flag']
class VictimComplaintTests(unittest.TestCase):
    def test_original_reacquisition_health_and_flag_reset_or_cancel(self):
        generate();cases=0
        for args in itertools.product((False,True),(0.0,1.0,float('nan')),(0,2),(0,2),(1,2,3,4,5,6,7,999),(False,True),(False,True)):
            with self.subTest(args=args):self.assertEqual(execute(*args),lua_case(*args));cases+=1
        self.assertEqual(cases,768)
if __name__=='__main__':unittest.main()

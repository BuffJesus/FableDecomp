import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.victim_candidate import generate
from tools.script_recovery.victim_dispatch_native import execute
PLANS=((True,True,'continue',True,True),(True,True,'repeat',True,True),(False,True,'continue',True,True),(True,False,'continue',True,True),(True,True,'cancel',True,True),(True,True,'repeat',False,True),(True,True,'continue',True,False),(True,True,'repeat',True,False))
def lua_case(source,plan=PLANS[0],cancel=6,failures=0,prepare=False,populated=True,fault=None):
    lua=LuaRuntime();lua.execute(source);q=lua.table();r=lua.table();events=[];state={'queries':0,'acquires':0,'control':False,'bully':False}
    def term(_):state['queries']+=1;v=state['queries']>=cancel;events.append(('term',v));return v
    def new(_):state['control']=True;events.append(('control.new',));return 1
    def lookup(_,key):state['bully']=True;events.extend([('text.new',key),('bully.new',populated),('text.destroy',key)]);return 2
    def prepare_fn(_,id):
        assert id==1;events.append(('prepare',prepare))
        if prepare:events.append(('prepare.release',))
    def acquire(_,id,actor,p):assert (id,actor,p)==(1,7,4);v=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',v,populated));return v
    def scope(_,fn):
        try:fn(r)
        finally:
            if state['bully']:events.append(('bully.destroy',));state['bully']=False
            if state['control']:events.append(('control.destroy',));state['control']=False
    q.RegisterBoundConsciousCondition=lambda _,actor:events.append(('condition',));q.NewScriptFrame=lambda _:events.append(('frame',));q.IsActiveThreadTerminating=term;q.WithRetailResources=scope;r.NewResource=new;r.NewThingFromScriptName=lookup;r.PrepareResource=prepare_fn;r.TryAcquire=acquire
    for index,(function,name) in enumerate((('VictimUpdateSubdued','subdued'),('VictimTalk','talk'),('VictimBeginHit','hit'),('VictimRepeatHit','repeat'),('VictimComplaint','complaint'))):
        callback=lambda *args,index=index,name=name:events.append((name,plan[index])) or plan[index]
        if fault==name:callback=lua.eval('function(f) return function(...) f(...); error("BODY ERROR",0) end end')(callback)
        lua.globals()[function]=callback
    error=None
    try:lua.globals().VictimMain(q,7,lua.table(),lambda amount:None)
    except Exception as e:error=str(e)
    return events,error
class VictimDispatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.source=generate()[0]
    def test_original_entry_outer_branches_and_retained_bully_control_cleanup(self):
        cases=0
        for args in itertools.product(PLANS,(1,2,3,4,5,6,7,9),(0,2),(False,True),(False,True)):
            with self.subTest(args=args):self.assertEqual(execute(*args),lua_case(self.source,*args)[0]);cases+=1
        self.assertEqual(cases,512)
    def test_abstracted_phase_error_closes_outer_bully_before_control(self):
        for fault in ('subdued','talk','hit','repeat','complaint'):
            events,error=lua_case(self.source,PLANS[1],fault=fault)
            self.assertIn('BODY ERROR',error);self.assertEqual(events[-2:],[('bully.destroy',),('control.destroy',)])
if __name__=='__main__':unittest.main()

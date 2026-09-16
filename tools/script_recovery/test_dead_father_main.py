import itertools,struct,unittest
from lupa import LuaRuntime
from tools.script_recovery.dead_father_candidate import SOURCE,generate,prove
from tools.script_recovery.dead_father_native import execute
from tools.script_recovery.lift_native_lua import RData
def lua_case(cancel=8,failures=0,found_after=0,prepare=False,populated=True,markers=(False,True),angle=1.5,flag5=1):
    lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();r=lua.table();events=[];state={'queries':0,'acquires':0,'polls':0,'control':False}
    def term(_):state['queries']+=1;v=state['queries']>=cancel;events.append(('term',v));return v
    def scope(_,fn):
        try:fn(r)
        finally:
            if state['control']:events.append(('control.destroy',));state['control']=False
    def new(_):state['control']=True;events.append(('control.new',));return 1
    def prepare_fn(_,id):
        assert id==1;events.append(('prepare',prepare))
        if prepare:events.append(('prepare.release',))
    def acquire(_,id,actor,p):assert (id,actor,p)==(1,7,4);v=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',v,populated));return v
    def place(_,actor):
        assert actor==7;bits=struct.pack('<f',angle).hex()
        for index in (1,2):
            events.extend([('text.new','MK_OVID_DAD'),('lookup',index,markers[index-1])])
            events.extend([('teleport',1,False)] if index==1 else [('angle',bits),('face',bits,True)])
            events.extend([('marker.destroy',index),('text.destroy','MK_OVID_DAD')])
    def pose(_,id):
        assert id==1;events.extend([('text.new','CS_DEAD_DAD'),('rawFlag',flag5)])
        if populated:events.append(('loop',-1,False,True,False,True,flag5,False,False))
        events.append(('text.destroy','CS_DEAD_DAD'))
    def found(_,key):assert key=='DadFound';v=state['polls']>=found_after;state['polls']+=1;events.append(('found',v));return v
    q.RegisterBoundAliveCondition=lambda _,actor:events.append(('condition.alive',));q.NewScriptFrame=lambda _:events.append(('frame',));q.IsActiveThreadTerminating=term;q.WithRetailResources=scope;q.GetStateBool=found
    r.NewResource=new;r.PrepareResource=prepare_fn;r.TryAcquire=acquire;r.PlaceDeadFatherAtMarker=place;r.PlayDeadFatherPose=pose;r.RemoveDeadFatherMarker=lambda _,actor:events.append(('remove','self'))
    lua.globals().DeadFatherMain(q,7);return events
class DeadFatherMainTests(unittest.TestCase):
    def test_whole_original_main_and_real_loop_forwarder(self):
        generate();cases=0
        for args in itertools.product((1,2,3,4,6,9),(0,2),(0,2),(False,True),(False,True),tuple(itertools.product((False,True),repeat=2)),(-1.25,float('nan')),(0,1,2,255)):
            with self.subTest(args=args):self.assertEqual(execute(*args),lua_case(*args));cases+=1
        self.assertEqual(cases,3072)
    def test_changed_marker_facing_loop_byte_operand_or_condition_rejected(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=bytearray(original.bytes_at(at,size))
                if at<=self.at<at+size:raw[self.at-at]^=1
                return bytes(raw)
            def string_at(self,at):return 'CHANGED' if at==self.at else original.string_at(at)
        for at in (0xdb830f,0xdb83f4,0xdb844f,0xdb848a,0xdb8496,0xdb84ed,0x7e73f9,0x12d95a4,0x1260f0c+0x580):
            with self.subTest(at=at),self.assertRaises(ValueError):prove(Changed(at))
    def test_native_predicate_failure_override_is_empty(self):
        lua=LuaRuntime();lua.execute(SOURCE);self.assertIsNone(lua.globals().DeadFatherOnPredicateFail(lua.table(),None));self.assertEqual(RData().bytes_at(0xdb8260,1),b'\xc3')
if __name__=='__main__':unittest.main()

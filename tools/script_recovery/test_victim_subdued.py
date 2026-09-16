import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.victim_subdued import SOURCE,recover,generate
from tools.script_recovery.victim_subdued_native import execute
from tools.script_recovery.lift_native_lua import RData

def lua_case(subdued=True,done=False,shake=False,ran_after=0,cancel=999,prepare=False,data_kind='empty',has_info=True,fault=False):
    lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();r=lua.table();s=lua.table();events=[];state={'queries':0,'frames':0,'DoneThanks':done,'VictimShake':shake}
    def get(_,key):
        value=subdued if key=='BullySubdued' else (state['frames']>=ran_after if key=='BullyRanOff' else state[key]);events.append(('get',key,value));return value
    def put(_,key,value):state[key]=value;events.append(('set',key,value))
    def term(_):state['queries']+=1;value=state['queries']>=cancel;events.append(('term',value));return value
    def frame(_):state['frames']+=1;events.append(('frame',))
    def prepare_fn(_,control):
        assert control==1;events.append(('prepare',prepare))
        if prepare:events.append(('prepare.release',))
    def release(_,actor):assert actor==7;events.extend([('copy.new',),('pushable',True),('copy.destroy',),('movement',True),('clear',)])
    q.GetStateBool=get;s.GetStateBool=get;q.SetStateBool=put;s.SetStateBool=put;q.IsActiveThreadTerminating=term;q.NewScriptFrame=frame
    r.PrepareResource=prepare_fn;r.SetRawScared=lambda _,actor,value:events.append(('scared',value));r.SetVictimReleasedState=release
    r.FaceTowardsRetainedThing=lambda _,actor,bully,snap:events.append(('face','victim','retainedBully',snap))
    if fault:r.SetRawScared=lua.eval('function(f) return function(...) f(...); error("SCARED ERROR",0) end end')(r.SetRawScared)
    try:complete=lua.globals().VictimUpdateSubdued(q,7,r,1,2,s)
    except Exception as error:complete=str(error)
    return complete,events,state['DoneThanks'],state['VictimShake']

class VictimSubduedTests(unittest.TestCase):
    def test_original_state_branches_and_real_copied_thing_consumption(self):
        generate();cases=0
        for args in itertools.product((False,True),(False,True),(False,True),(0,2),(1,2,3,4,5,999),(False,True),('empty','invalid','valid'),(False,True)):
            with self.subTest(args=args):self.assertEqual(execute(*args),lua_case(*args));cases+=1
        self.assertEqual(cases,1152)
    def test_done_flag_is_not_rolled_back_when_later_callback_fails(self):
        result,events,done,shake=lua_case(fault=True)
        self.assertIn('SCARED ERROR',result);self.assertTrue(done)
        self.assertLess(events.index(('set','DoneThanks',True)),events.index(('scared',False)))
        self.assertFalse(any(e[0]=='get' and e[1]=='BullyRanOff' for e in events))
    def test_changed_true_flags_resource_or_state_operands_fail_closed(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=bytearray(original.bytes_at(at,size))
                if at<=self.at<at+size:raw[self.at-at]^=1
                return bytes(raw)
        for address in (0xdbceb2,0xdbcf15,0xdbcf30,0xdbcf60,0xdbcf6a,0x8a6e77,0x1260f0c+0xd30):
            with self.subTest(address=address),self.assertRaises(ValueError):recover(Changed(address))

if __name__=='__main__':unittest.main()

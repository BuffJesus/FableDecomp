import itertools,unittest
from lupa import LuaRuntime
from tools.script_recovery.live_father_candidate import BODY,generate,prove
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.live_father_entry_native import execute

def lua_case(finished=False,cancel=999,failures=0,prepare=False,intro=True,populated=True):
    lua=LuaRuntime();lua.execute(BODY);q=lua.table();r=lua.table();events=[];state={'queries':0,'acquires':0,'control':False}
    def term(_):state['queries']+=1;value=state['queries']>=cancel;events.append(('term',value));return value
    def new(_):state['control']=True;events.append(('control.new',));return 1
    def scope(_,fn):
        try:fn(r)
        finally:
            if state['control']:events.append(('control.destroy',));state['control']=False
    def prepare_fn(_,control):
        assert control==1;events.append(('prepare',prepare))
        if prepare:events.append(('prepare.release',))
    def acquire(_,control,actor,priority):
        assert (control,actor,priority)==(1,7,4);value=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',value,populated));return value
    q.RegisterBoundConsciousCondition=lambda _,actor:events.append(('condition',));q.NewScriptFrame=lambda _:events.append(('frame',));q.IsActiveThreadTerminating=term;q.GetStateBool=lambda _,key:events.append(('finished',finished)) or finished;q.WithRetailResources=scope
    r.NewResource=new;r.PrepareResource=prepare_fn;r.TryAcquire=acquire
    lua.globals().LiveFatherIntro=lambda *args:events.append(('intro',intro)) or intro;lua.globals().LiveFatherRoutine=lambda *args:events.append(('routine',)) or False
    lua.globals().LiveFatherMain(q,7,lua.table(),lambda _:None);return events

class LiveFatherEntryTests(unittest.TestCase):
    def test_original_condition_frame_state_branch_and_initial_control_lifetime(self):
        source,_=generate();LuaRuntime().execute(source);cases=0
        for args in itertools.product((False,True),(1,2,3,4,999),(0,2),(False,True),(False,True),(False,True)):
            with self.subTest(args=args):self.assertEqual(execute(*args),lua_case(*args));cases+=1
        self.assertEqual(cases,160)
    def test_changed_intro_operand_api_or_literal_fails_closed(self):
        original=RData()
        class Changed:
            def __init__(self,at):self.at=at
            def bytes_at(self,at,size):
                raw=bytearray(original.bytes_at(at,size))
                if at<=self.at<at+size:raw[self.at-at]^=1
                return bytes(raw)
            def string_at(self,at):return 'CHANGED' if at==self.at else original.string_at(at)
        for address in (0xdb8b1b,0xdb88eb,0xdb88ef,0xdb8aac,0x1260f0c+0x684,0x1260f0c+0x51c,0x12d977c,0x12d9714):
            with self.subTest(address=address),self.assertRaises(ValueError):prove(Changed(address))

if __name__=='__main__':unittest.main()

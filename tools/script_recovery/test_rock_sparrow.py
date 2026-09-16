import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_sparrow_recovery import recover
from tools.script_recovery.rock_sparrow_native import execute
from tools.script_recovery.lift_native_lua import RData


def run(failures=0,stop=100,awake_frame=1,populated=True,error=False):
    body,_=recover();lua=LuaRuntime();lua.execute('function Main(quest,me)\n'+body+'end')
    q,r,me=lua.table(),lua.table(),lua.table();events=[];state={'frames':0,'queries':0,'attempts':0,'held':False}
    q.RegisterBoundAliveCondition=lambda _:events.append(('condition',))
    def frame(_):state['frames']+=1;events.append(('frame',))
    q.NewScriptFrame=frame
    def term(_):state['queries']+=1;value=state['queries']>=stop;events.append(('term',value));return value
    q.IsActiveThreadTerminating=term
    def getter(_,name):assert name=='TrollAwake';return state['frames']>=awake_frame
    q.GetStateBool=getter
    def new(_):events.append(('new',));return 71
    r.NewResource=new
    def prepare(_,id):assert id==71;events.append(('prepare',))
    r.PrepareResource=prepare
    def acquire(_,id,target,priority):
        assert id==71 and lua.eval('rawequal')(target,me) and priority==4
        result=state['attempts']>=failures;state['attempts']+=1
        events.append(('acquire',priority,result,state['held']));state['held']=bool(result or populated)
        if error:raise RuntimeError('acquire failure')
        return result
    r.TryAcquire=acquire
    def scope(_,callback):
        try:callback(r)
        finally:events.append(('destroy',state['held']));state['held']=False
    q.WithRetailResources=scope
    try:lua.globals().Main(q,me)
    except RuntimeError:
        if not error:raise
    assert not state['held']
    return events


class SparrowTests(unittest.TestCase):
    def test_native_traces_acquisition_cancellation_and_awake_wait(self):
        for failures in (0,2):
            for stop in (1,2,3,4,100):
                for awake in (1,5):
                    for populated in (False,True):
                        with self.subTest(failures=failures,stop=stop,awake=awake,populated=populated):
                            self.assertEqual(run(failures,stop,awake,populated),execute(failures,stop,awake,populated))

    def test_populated_failure_survives_retry_and_cancellation(self):
        self.assertIn(('acquire',4,False,True),run(failures=2))
        self.assertEqual(run(failures=2,stop=2)[-2:],[('term',True),('destroy',True)])
        self.assertEqual(run(failures=2,error=True)[-1],('destroy',True))

    def test_native_binding_priority_and_termination_mutations_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;raw=bytes(raw)
                return raw
        for address in (0xec3d93,0xec45f4,0x12f16e4,0xec4735,0xec475c,0xec477a,0xec479f):
            data=Changed();data.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError):recover(data)


if __name__=='__main__':unittest.main()

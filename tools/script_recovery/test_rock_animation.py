import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_animation import recover
from tools.script_recovery.rock_animation_native import execute
from tools.script_recovery.lift_native_lua import RData


def run(flags=(1,0),busy_polls=0,stop=100,empty=False,error=False):
    source,_=recover();lua=LuaRuntime();lua.execute(source);q,r=lua.table(),lua.table();events=[]
    state={'queries':0,'polls':0,'control':not empty,'open':True,'flag':flags[0]};snapshot=[]
    def play(_,id,name):
        assert state['open'] and id==71;events.append(('string.new',name));events.append(('flag',state['flag']))
        try:
            if state['control']:
                events.append(('play',name,(0,0,0,1,state['flag'],0,0)))
                if error:raise RuntimeError('animation error')
        finally:
            events.append(('string.destroy',name))
            if name=='SPECIAL_BOAST':state['flag']=flags[1]
    r.PlayRockTrollAnimation=play
    def task(_,id):
        assert state['open'] and id==71;result=state['control'] and state['polls']<busy_polls;state['polls']+=1;events.append(('task',result));return result
    r.IsPerformingScriptTask=task
    q.NewScriptFrame=lambda _:events.append(('frame',))
    def term(_):state['queries']+=1;result=state['queries']>=stop;events.append(('term',result));return result
    q.IsActiveThreadTerminating=term
    def prepare(_,id):
        assert state['open'] and id==71;events.append(('prepare',state['control']))
        if state['control']:state['control']=False;events.append(('release.control',))
    r.PrepareResource=prepare
    def continuation():events.append(('continuation',));snapshot.append((list(events),state['control'],state['open']))
    try:lua.globals().WithRockTrollAnimationPhase(q,r,71,continuation)
    except RuntimeError:
        if not error:raise
    finally:events.append(('destroy',state['control']));state['control']=False;state['open']=False
    return snapshot[0] if snapshot else (events,False,False),events


class AnimationTests(unittest.TestCase):
    def test_original_phase_helper_raw_flags_and_cancellation_traces(self):
        for flags in ((1,0),(0,1),(2,255)):
            for busy in (0,2):
                for stop in (1,2,3,100):
                    for empty in (False,True):
                        with self.subTest(flags=flags,busy=busy,stop=stop,empty=empty):
                            self.assertEqual(run(flags,busy,stop,empty)[0],execute(flags,busy,stop,empty))

    def test_control_release_keeps_object_live_until_continuation_returns(self):
        snapshot,events=run()
        self.assertEqual(snapshot[1:],(False,True))
        self.assertEqual(events[-3:],[('release.control',),('continuation',),('destroy',False)])
        snapshot,events=run(empty=True)
        self.assertFalse(any(event[0]=='play' for event in events))
        self.assertEqual(sum(event[0]=='string.destroy' for event in events),2)
        self.assertFalse(any(event[0]=='release.control' for event in events))

    def test_engine_double_error_closes_string_before_outer_resource(self):
        _,events=run(error=True)
        self.assertEqual(events[-2:],[('string.destroy','SPECIAL_BOAST'),('destroy',True)])
        self.assertFalse(any(event[0]=='task' for event in events))

    def test_changed_dynamic_byte_flags_helper_target_and_release_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for address in (0xec4f01,0xec4f0d,0xec4f37,0xec4f16,0x7e73d9,0x7e745c,0xec4f8a,0xec4fa4):
            data=Changed();data.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError):recover(data)


if __name__=='__main__':unittest.main()

import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_actor_phases import recover
from tools.script_recovery.rock_actor_resources_native import execute
from tools.script_recovery.rock_actor_dispatch_native import execute as dispatch
from tools.script_recovery.lift_native_lua import RData


def run(kind='self',failures=0,stop=100,populated=True,error=False):
    source,_=recover();lua=LuaRuntime();lua.execute(source);q,r,me=lua.table(),lua.table(),lua.table()
    events=[];held={'self':kind=='hero','hero':False};live=['self'] if kind=='hero' else []
    state={'queries':0,'attempts':0,'heroes':0};snapshot=[];dispatches=[]
    def create(_,name,actor,flag,region):
        assert lua.eval('rawequal')(actor,me);dispatches.append((name,flag,region))
    q.CreateRetainedThingThread=create
    def new(_):
        name='self' if not live else 'hero';live.append(name);events.append(('new',name));return name
    r.NewResource=new;r.PrepareResource=lambda _,name:events.append(('prepare',name))
    def acquire(_,name,target,priority):
        assert priority==4 and (lua.eval('rawequal')(target,me) if kind=='self' else target==state['heroes'])
        result=state['attempts']>=failures;state['attempts']+=1;events.append(('acquire',name,result,held[name]));held[name]=bool(result or populated);return result
    r.TryAcquire=acquire
    def destroy(_,name):
        assert name in live;events.append(('destroy',name,held[name]));held[name]=False;live.remove(name)
    r.ReleaseResource=destroy
    q.NewScriptFrame=lambda _:events.append(('frame',))
    def term(_):state['queries']+=1;result=state['queries']>=stop;events.append(('term',result));return result
    q.IsActiveThreadTerminating=term
    def hero(_):state['heroes']+=1;events.append(('hero',state['heroes']));return state['heroes']
    q.GetHero=hero
    def continuation(*args):
        events.append(('continuation',));snapshot.append((list(events),dict(held)))
        if error:raise RuntimeError('continuation error')
    def close():
        for name in list(reversed(live)):destroy(None,name)
    def scope(_,callback):
        try:callback(r)
        finally:close()
    q.WithRetailResources=scope
    try:
        if kind=='self':lua.globals().WithRockTrollSelfPhase(q,me,continuation)
        else:
            try:lua.globals().WithRockTrollHeroPhase(q,r,continuation)
            finally:close()
    except RuntimeError:
        if not error:raise
    assert not live and not any(held.values())
    assert dispatches==([('WatchForRockTrollKilled',False,'')] if kind=='self' else [])
    return snapshot[0] if snapshot else (events,held),events


class ActorPhaseTests(unittest.TestCase):
    def test_original_resource_windows_and_retained_continuation(self):
        for kind in ('self','hero'):
            for failures in (0,2):
                for stop in (1,2,3,5,100):
                    for populated in (False,True):
                        with self.subTest(kind=kind,failures=failures,stop=stop,populated=populated):
                            actual,_=run(kind,failures,stop,populated)
                            self.assertEqual(actual,execute(kind,failures,stop,populated))

    def test_continuation_error_unwinds_hero_before_self(self):
        snapshot,events=run('hero',error=True)
        self.assertEqual(snapshot[1],{'self':True,'hero':True})
        self.assertEqual(events[-2:],[('destroy','hero',True),('destroy','self',True)])
        _,events=run('self',error=True);self.assertEqual(events[-1],('destroy','self',True))

    def test_native_dispatch_copies_argument_separately_from_thread_lifetime(self):
        for empty in (False,True):
            for info in (False,True):
                self.assertEqual(dispatch(empty,info),[('call.by.value',empty,info),('dispatch.complete',),
                    ('stored.argument.destroy',),('thread.base.destroy',)])

    def test_changed_priority_target_dispatch_and_cleanup_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for address in (0xec4a75,0xec4b36,0xec4b6a,0xec4c47,0xec4c7f,0xec4c9f,0xec5314,0xec5336):
            data=Changed();data.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError):recover(data)


if __name__=='__main__':unittest.main()

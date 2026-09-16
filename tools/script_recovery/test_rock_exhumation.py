import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_exhumation import recover
from tools.script_recovery.rock_exhumation_native import execute
from tools.script_recovery.lift_native_lua import RData


def run(empty_hero=False,empty_troll=False,empty_movie=False,allocation_fails=False,
        played=False,hero_cancel=False,macro_cancel=False,error_at=None):
    source,_=recover();lua=LuaRuntime();lua.execute(source);q,r,me=lua.table(),lua.table(),lua.table();events=[]
    state={'played':played,'paused':False,'map':False,'movie':False,'hero':False,'self':True,'continued':False};mapping={}
    q.GetStateBool=lambda _,key:state['played'] if key=='PlayedExhumeCutScene' else None
    def hero_phase(_,resources,callback):
        if hero_cancel:return
        state['hero']=True;callback('hero')
        state['hero']=False;events.append(('hero.destroy',empty_hero))
    lua.globals().WithRockTrollHeroPhase=hero_phase
    def newmap(_):state['map']=True;events.append(('map.new',));return 1
    r.NewActorMap=newmap
    def setactor(_,id,name,resource):
        assert id==1 and state[resource];mapping[name]=resource;events.append(('map.actor',name,resource))
    r.SetActor=setactor
    def movie(_,name):
        assert name=='';state['movie']=True;events.extend([('movie.new',),('movie.start','',empty_movie)]);return 2
    r.StartMovie=movie
    def pause(_,value):state['paused']=value;events.append(('pause',value))
    r.Pause=pause
    def macro(_,name,id,setup,skippable):
        assert mapping=={'HERO':'hero','TROLL':'self'} and state['movie'] and state['paused']
        events.append(('macro',name,setup,skippable,empty_hero,empty_troll))
        if error_at=='macro':raise RuntimeError('macro error')
        # Native has no intervening termination query after macro returns.
        q.IsActiveThreadTerminating=lambda _:macro_cancel
    r.RunMacro=macro
    def thread(_,name,target,flag,region):
        assert lua.eval('rawequal')(target,me) and state['paused'] and state['movie']
        events.append(('thread',name,flag,region))
        if error_at=='thread':raise RuntimeError('thread error')
        return not allocation_fails
    q.CreateRetainedThingThread=thread
    def setter(_,key,value):assert key=='PlayedExhumeCutScene' and state['paused'] and state['movie'];state['played']=value;events.append(('state',value))
    q.SetStateBool=setter
    def destmovie(_,id):assert id==2 and not state['paused'];state['movie']=False;events.append(('movie.destroy',empty_movie))
    r.DestroyMovie=destmovie
    def destmap(_,id):assert id==1 and not state['movie'];state['map']=False;events.append(('map.destroy',))
    r.DestroyActorMap=destmap
    def continuation():assert state['self'] and not state['hero'];state['continued']=True
    try:lua.globals().WithRockTrollExhumationPhase(q,me,r,'self',continuation)
    except RuntimeError:
        if not error_at:raise
        # Existing outer WithRetailResources closes active locals in reverse order.
        if state['paused']:pause(None,False)
        if state['movie']:destmovie(None,2)
        if state['map']:destmap(None,1)
        if state['hero']:state['hero']=False;events.append(('hero.destroy',empty_hero))
        state['self']=False;events.append(('self.destroy',empty_troll))
    return events,state


class ExhumationTests(unittest.TestCase):
    def test_original_map_movie_thread_and_resource_cleanup_traces(self):
        for hero in (False,True):
            for troll in (False,True):
                for movie in (False,True):
                    for failed in (False,True):
                        with self.subTest(hero=hero,troll=troll,movie=movie,failed=failed):
                            events,state=run(hero,troll,movie,failed)
                            self.assertEqual(events,execute(hero,troll,movie,failed))
                            self.assertTrue(state['continued'] and state['played'] and state['self'])

    def test_replayed_skip_hero_cancel_and_macro_cancel_policy(self):
        events,state=run(played=True);self.assertEqual(events,[]);self.assertTrue(state['continued'])
        events,state=run(hero_cancel=True);self.assertEqual(events,[]);self.assertFalse(state['continued'])
        self.assertEqual(run(macro_cancel=True)[0],run()[0])

    def test_error_unpause_movie_map_hero_self_order_and_no_completed_flag(self):
        for where in ('macro','thread'):
            events,state=run(error_at=where)
            self.assertFalse(state['played'] or state['continued'])
            self.assertEqual(events[-5:],[('pause',False),('movie.destroy',False),('map.destroy',),('hero.destroy',False),('self.destroy',False)])

    def test_native_map_sources_flags_state_and_destroy_order_mutations(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for address in (0xec4c04,0xec4d14,0xec4d47,0xec4dc1,0xec4dc7,0xec4e51,0xec4ec0,0xec4ed5,0xec4ee7):
            data=Changed();data.changed=address
            with self.subTest(address=address),self.assertRaises(ValueError):recover(data)


if __name__=='__main__':unittest.main()

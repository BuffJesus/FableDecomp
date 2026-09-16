import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.bully_retained_phases import recover
from tools.script_recovery.bully_runoff_controls_native import execute

class BullyRetainedPhaseTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.source,_=recover()

    def test_native_late_control_order_retry_outputs_and_cancellation(self):
        for hero_fail in (0,2):
            for victim_fail in (0,2):
                for cancel in (1,2,3,4,5,999):
                    lua=LuaRuntime();lua.execute(self.source);q=lua.table();r=lua.table();events=[]
                    state={'new':0,'hero':0,'victim':0,'heroes':0,'queries':0}
                    def new(_):
                        state['new']+=1;name='hero' if state['new']==1 else 'victim';events.append(('new',name));return name
                    r.NewResource=new;r.PrepareResource=lambda _,name:events.append(('prepare',name))
                    def hero(_):state['heroes']+=1;events.append(('hero',state['heroes']));return state['heroes']
                    q.GetHero=hero
                    def acquire(_,name,actor,priority):
                        self.assertEqual(priority,4)
                        self.assertEqual(actor,state['heroes'] if name=='hero' else 77)
                        state[name]+=1;ok=state[name]>(hero_fail if name=='hero' else victim_fail);events.append(('acquire',name,ok));return ok
                    r.TryAcquire=r.TryAcquireThing=acquire
                    q.NewScriptFrame=lambda _:events.append(('frame',))
                    def term(_):state['queries']+=1;ok=state['queries']>=cancel;events.append(('term',ok));return ok
                    q.IsActiveThreadTerminating=term;r.ReleaseResource=lambda _,name:events.append(('destroy',name))
                    lua.globals().WithBullyRunoffControls(q,r,77,lambda *args:events.append(('continuation',)))
                    self.assertEqual(events,execute(hero_fail,victim_fail,cancel))

    def test_retained_victim_continuation_scope_and_initial_queries(self):
        for cancel in (1,2,999):
            lua=LuaRuntime();lua.execute(self.source);q=lua.table();r=lua.table();events=[];checks=0
            def term(_):
                nonlocal checks
                checks+=1;events.append('term');return checks>=cancel
            q.IsActiveThreadTerminating=term
            r.NewThingFromScriptName=lambda _,name:events.append(('lookup',name)) or 77
            r.DestroyThing=lambda _,thing:events.append(('destroy',thing))
            lua.globals().WithBullyRetainedVictim(q,r,lambda victim:events.append(('continuation',victim)))
            expected=['term']
            if cancel!=1:
                expected += [('lookup','NOVI_Victim'),'term']
                if cancel!=2:expected += [('continuation',77)]
                expected += [('destroy',77)]
            self.assertEqual(events,expected)

    def test_movie_scope_normal_early_return_and_outer_error_unwind(self):
        for error in (False,True):
            lua=LuaRuntime();lua.execute(self.source);r=lua.table();events=[];live={'movie':False,'paused':False}
            def start(_,key):self.assertEqual(key,'');live['movie']=True;events.append('movie.start');return 9
            def pause(_,value):live['paused']=value;events.append(('pause',value))
            def destroy(_,movie):self.assertEqual(movie,9);live['movie']=False;events.append('movie.destroy')
            r.StartMovie=start;r.Pause=pause;r.DestroyMovie=destroy
            def body(movie):
                self.assertEqual(movie,9);self.assertTrue(live['movie'] and live['paused']);events.append('body')
                if error:raise RuntimeError('BODY')
            try:
                lua.globals().WithBullyPausedMovie(r,body)
            except RuntimeError as exc:
                self.assertTrue(error);self.assertIn('BODY',str(exc))
                # Existing outer WithRetailResources Close contract, not an
                # invented native exception edge or scheduler cancellation.
                if live['paused']:pause(r,False)
                if live['movie']:destroy(r,9)
            self.assertEqual(events,['movie.start',('pause',True),'body',('pause',False),'movie.destroy'])

import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.scythe_runtime_candidate import build
from tools.script_recovery.scythe_cleanup import recover


class ScytheCleanupTests(unittest.TestCase):
    def test_errors_close_only_open_callback_resources_and_keep_primary_error(self):
        build()
        source=(ROOT/'work/scythe_converter/runtime_proposal/draft/Entities/ScytheNearOracle.lua').read_text()
        for fault in ('GetHero','PauseAllNonScriptedEntities','MiniMapRemoveMarker','RunScytheOracleCutscene','SetStateBool'):
            for cleanup_fault in (False,True):
                with self.subTest(fault=fault,cleanup_fault=cleanup_fault):
                    lua,events=LuaRuntime(),[]
                    def call(name,result=None):
                        def action(_q,*args):
                            events.append((name,*args))
                            return result
                        # Match sol's Lua error values, avoiding Lupa's sticky Python exception bridge.
                        if name==fault:
                            return lua.eval('function(f,message,pause) return function(self,...) if not pause or select(1,...) then error(message,0) end return f(self,...) end end')(
                                action,'BODY '+fault,name=='PauseAllNonScriptedEntities')
                        if cleanup_fault and name=='EndMovieSequence':
                            return lua.eval('function(f) return function(...) f(...); error("CLEANUP error",0) end end')(action)
                        return action
                    callbacks={name:call(name) for name in ('RegisterBoundAliveCondition','NewScriptFrame',
                        'MiniMapAddMarker','FaceThingByScriptName','EntitySetFacingAngleTowardsThing','StartMovieSequence',
                        'EndMovieSequence','PauseAllNonScriptedEntities','MiniMapRemoveMarker',
                        'RunScytheOracleCutscene','SetStateBool','DeregisterTimer')}
                    callbacks.update({'IsActiveThreadTerminating':lambda _q:False,
                        'GetThingWithScriptName':lambda _q,key:'marker', 'RegisterTimer':lambda _q:81,
                        'GetTimer':lambda _q,t:1,'GetHero':call('GetHero','hero'),
                        'IsDistanceBetweenThingsUnder':lambda _q,a,b,d:d==12})
                    me=lua.table_from({'AcquireControl':lambda _me,p:True,
                        'ReleaseControl':lambda _me:events.append(('release',))})
                    lua.execute(source)
                    with self.assertRaisesRegex(Exception,'BODY '+fault):lua.globals().Main(lua.table_from(callbacks),me)
                    self.assertEqual(events[-2:],[('DeregisterTimer',81),('release',)])
                    movie_open=fault!='GetHero'
                    paused=fault not in ('GetHero','PauseAllNonScriptedEntities')
                    self.assertEqual(events.count(('EndMovieSequence',)),int(movie_open))
                    self.assertEqual(events.count(('PauseAllNonScriptedEntities',False)),int(paused))

    def test_unreviewed_source_rejects(self):
        with self.assertRaisesRegex(ValueError,'correspondence changed'):recover('changed')


if __name__=='__main__':unittest.main()

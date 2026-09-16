import itertools,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.teddy_girl_presented_phase import SOURCE

class TeddyGirlMovieErrorTests(unittest.TestCase):
    def test_callback_primary_error_survives_both_cleanup_faults(self):
        for fault,unpause_fault,destroy_fault in itertools.product(('new','start','pause','body',None),(False,True),(False,True)):
            with self.subTest(fault=fault,unpause=unpause_fault,destroy=destroy_fault):
                lua=LuaRuntime();lua.execute(SOURCE);q=lua.table();r=lua.table();events=[]
                def action(name):
                    events.append(name)
                    return 5 if name=='new' else True
                # Engine/sol failures are Lua errors. Python exceptions keep a
                # separate pending exception in Lupa and cannot model this ABI.
                dispatch=lua.eval('function(record, fault, unpause, destroy) return function(name) local value=record(name); if name==fault then error("PRIMARY "..name,0) end; if name=="unpause" and unpause then error("UNPAUSE",0) end; if name=="destroy" and destroy then error("DESTROY",0) end; return value end end')(action,fault,unpause_fault,destroy_fault)
                r.NewMovie=lua.eval('function(f) return function() return f("new") end end')(dispatch)
                r.StartOwnedMovie=lua.eval('function(f) return function() return f("start") end end')(dispatch)
                r.DestroyMovie=lua.eval('function(f) return function() return f("destroy") end end')(dispatch)
                q.PauseAllNonScriptedEntities=lua.eval('function(f) return function(_,value) return f(value and "pause" or "unpause") end end')(dispatch)
                body=lua.eval('function(f) return function() return f("body") end end')(dispatch)
                call=lambda:lua.globals().TeddyGirlWithMovie(q,r,body)
                expected='PRIMARY '+fault if fault else ('UNPAUSE' if unpause_fault else ('DESTROY' if destroy_fault else None))
                if expected:
                    with self.assertRaisesRegex(Exception,expected):call()
                else:self.assertTrue(call())
                wanted=['new']
                if fault!='new':
                    wanted.append('start')
                    if fault!='start':
                        wanted.append('pause')
                        if fault!='pause':wanted.append('body')
                        wanted.append('unpause')
                    wanted.append('destroy')
                self.assertEqual(events,wanted)

if __name__=='__main__':unittest.main()

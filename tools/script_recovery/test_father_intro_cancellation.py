"""Actual Father intro closure versus native post-macro termination gates.

Engine operations are doubles. The archived native slice runs both termination
helpers, but stops before resource destruction or HUD counter construction.
"""
import json
from pathlib import Path
import unittest
from lupa import LuaRuntime

ROOT = Path(__file__).resolve().parents[2]
FSE = ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'


class FatherIntroCancellationTests(unittest.TestCase):
    def test_matches_native_gates(self):
        proof = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/runtime_evidence/native-father-intro-gates-20260912.json').read_text())
        for case in proof['cases']:
            with self.subTest(case=case):
                lua = LuaRuntime(unpack_returned_tuples=True)
                lua.globals().package.path = FSE.as_posix() + '/?.lua;' + lua.globals().package.path
                lua.execute((FSE / 'NewOakValeIntro/Entities/NOVI_LiveFather.lua').read_text())
                run = lua.eval(r'''function(xbox, stop_at, waiting)
                    local function upvalue(f, wanted)
                        for i=1,100 do
                            local name,value=debug.getupvalue(f,i)
                            if name==wanted then return value end
                            if name==nil then break end
                        end
                        error('Missing closure: '..wanted)
                    end
                    local intro=upvalue(upvalue(Main,'run'),'intro_cutscene')
                    local state={queries=0,tutorial_calls=0,counters=0,frames=0,closed=0,paused=false}
                    local q=setmetatable({}, {__index=function() return function() end end})
                    local hero_held=false
                    function q:GetHero()
                        return {AcquireControl=function() hero_held=true;return true end,
                                ReleaseControl=function()
                                    assert(state.closed==1 and not state.paused,'Hero released before movie cleanup')
                                    assert(hero_held,'Hero released twice')
                                    hero_held=false;state.hero_releases=(state.hero_releases or 0)+1
                                end}
                    end
                    local after_macro=false
                    function q:RunCutsceneWithSetup() assert(hero_held);after_macro=true end
                    function q:IsXbox() return xbox end
                    function q:IsActiveThreadTerminating()
                        if not after_macro then return false end
                        state.queries=state.queries+1
                        return stop_at>0 and state.queries>=stop_at
                    end
                    function q:NewScriptFrame()
                        state.frames=state.frames+1
                        assert(state.frames<10, 'unexpected wait loop')
                        return not self:IsActiveThreadTerminating()
                    end
                    function q:DisplayGameInfo() assert(hero_held);state.tutorial_calls=state.tutorial_calls+1 end
                    function q:MsgIsGameInfoClickedPast() return not waiting end
                    function q:AddQuestInfoCounter() state.counters=state.counters+1; return 501 end
                    function q:PauseAllNonScriptedEntities(value) state.paused=value end
                    function q:EndMovieSequence() state.closed=state.closed+1 end
                    state.completed=intro(q,{})
                    return state
                end''')
                state = run(case['xbox'], case['stop_at_query'], case['waiting'])
                self.assertEqual(state['tutorial_calls'], case['tutorial_calls'])
                self.assertEqual(state['counters'], int(case['counter_reached']))
                self.assertEqual(state['completed'], case['counter_reached'])
                self.assertEqual(state['queries'], case['queries'])
                self.assertEqual(state['frames'], case['frames'])
                self.assertEqual(state['closed'], 1)
                self.assertEqual(state['hero_releases'], 1)
                self.assertFalse(state['paused'])


if __name__ == '__main__':
    unittest.main()

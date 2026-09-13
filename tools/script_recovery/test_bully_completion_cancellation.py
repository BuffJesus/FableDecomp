"""Compare actual run_off completion effects with the executed native tail.

Resource-map lifetime is outside this test: the legacy macro wrapper owns its
temporary map. Engine calls and AddGoodDeed are doubled on both sides.
"""
import json
from pathlib import Path
import unittest
from lupa import LuaRuntime

ROOT = Path(__file__).resolve().parents[2]
FSE = ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'


class BullyCompletionCancellationTests(unittest.TestCase):
    def test_native_tail_effects(self):
        proof = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/runtime_evidence/native-bully-completion-gates-20260912.json').read_text())
        for case in proof['cases']:
            with self.subTest(case=case):
                lua = LuaRuntime(unpack_returned_tuples=True)
                lua.globals().package.path = FSE.as_posix() + '/?.lua;' + lua.globals().package.path
                lua.execute((FSE / 'NewOakValeIntro/Entities/NOVI_Bully.lua').read_text(encoding='utf-8'))
                result = lua.eval(r'''function(given, termination)
                    local events, fields, recording, stopped = {}, {GivenHeroTeddy=given}, false, false
                    local function emit(name) if recording then events[#events+1]=name end end
                    local function actor(name)
                        return {AcquireControl=function() return true end,
                                ReleaseControl=function() emit('release_'..name) end,
                                GetPos=function() return {x=0,y=0,z=0} end,
                                IsPerformingScriptTask=function() return false end}
                    end
                    local me,hero,victim=actor('bully'),actor('hero'),actor('victim')
                    local q=setmetatable({}, {__index=function() return function() end end})
                    function q:GetHero() return hero end
                    function q:GetStateBool(key) return fields[key] or false end
                    function q:SetStateBool(key,value) fields[key]=value end
                    function q:RunCutsceneWithSetup(name)
                        if name=='CS_OAKVALEINTRO_BULLYRUN1' then
                            recording=true; stopped=termination=='run1'
                        else
                            emit(given and 'dummy' or 'run2')
                            if termination=='run2' then stopped=true end
                        end
                    end
                    function q:IsActiveThreadTerminating() emit('termination_query'); return stopped end
                    function q:FixMovieSequenceCamera(value) if not value then emit('camera_unfix') end end
                    function q:PauseAllNonScriptedEntities(value) if not value then emit('unpause') end end
                    function q:EndMovieSequence() emit('movie_destroy') end
                    function q:ClearThingHasInformation() emit('clear_victim_info') end
                    function q:RemoveThing() emit('remove') end
                    require('NewOakValeIntro.deeds').add_good=function() emit('good_deed') end
                    local function upvalue(f,wanted)
                        for i=1,100 do
                            local name,value=debug.getupvalue(f,i)
                            if name==wanted then return value end
                            if name==nil then break end
                        end
                        error('Missing '..wanted)
                    end
                    local run_off=upvalue(upvalue(Main,'handle_hit'),'run_off')
                    local completed=run_off(q,me,victim)
                    return {events=events,ran_off=fields.BullyRanOff or false,
                            teddy_after=fields.GivenHeroTeddy,completed=completed}
                end''')(case['given_teddy'], case['termination'])
                expected = [e for e in case['events'] if e != 'map_destroy']
                self.assertEqual(list(result['events'].values()), expected)
                self.assertEqual(result['ran_off'], case['ran_off'])
                self.assertEqual(result['teddy_after'], case['teddy_after'])
                self.assertEqual(result['completed'], case['ran_off'])


if __name__ == '__main__':
    unittest.main()

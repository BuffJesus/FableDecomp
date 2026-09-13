"""Compare successful acquisition/termination boundaries with native execution."""
import json
from pathlib import Path
import unittest
from lupa import LuaRuntime

ROOT=Path(__file__).resolve().parents[2]
FSE=ROOT/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'


class BullyAcquiredGatesTests(unittest.TestCase):
    def test_native_success_gates(self):
        proof=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/runtime_evidence/native-bully-acquired-gates-complete-20260912.json').read_text())
        for case in proof['cases']:
            with self.subTest(stop_at=case['stop_at']):
                lua=LuaRuntime(unpack_returned_tuples=True)
                lua.globals().package.path=FSE.as_posix()+'/?.lua;'+lua.globals().package.path
                lua.execute((FSE/'NewOakValeIntro/Entities/NOVI_Bully.lua').read_text())
                actual=lua.eval('''function(stop_at,attacked)
                    local events,queries={},0
                    local function emit(e) events[#events+1]=e end
                    local function actor(name)
                        return {AcquireControl=function() emit('acquire_'..name);return true end,
                                ReleaseControl=function() emit('release_'..name) end,
                                GetPos=function() return {x=0,y=0,z=0} end}
                    end
                    local me,hero,victim=actor('bully'),actor('hero'),actor('victim')
                    local q={GetHero=function() return hero end}
                    function q:IsActiveThreadTerminating()
                        queries=queries+1;return stop_at>0 and queries>=stop_at
                    end
                    function q:GetStateBool() return attacked end
                    function q:Log() end
                    function q:StartMovieSequence() emit('ready_for_movie');error('STOP_BEFORE_SETUP') end
                    local function up(f,wanted)
                        for i=1,100 do local n,v=debug.getupvalue(f,i);if n==wanted then return v end;if not n then break end end
                        error('missing '..wanted)
                    end
                    local ok,err=pcall(up(up(Main,'handle_hit'),'run_off'),q,me,victim)
                    if not ok then assert(tostring(err):find('STOP_BEFORE_SETUP'),err) end
                    return {events=events,queries=queries}
                end''')(case['stop_at'],case['attacked'])
                self.assertEqual(list(actual['events'].values()),case['events'])
                self.assertEqual(actual['queries'],case['queries'])


if __name__=='__main__':unittest.main()

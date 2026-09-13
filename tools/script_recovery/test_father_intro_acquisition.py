"""Actual Father intro entry against six native acquisition/cancellation cases.

The AcquireControl double implements its reviewed blocking-loop contract,
including release on failure. Macro playback is stopped at entry on both sides.
"""
import json
from pathlib import Path
import unittest
from lupa import LuaRuntime

ROOT=Path(__file__).resolve().parents[2]
FSE=ROOT/'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'


class FatherIntroAcquisitionTests(unittest.TestCase):
    def test_native_entry_order(self):
        proof=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/runtime_evidence/native-father-intro-acquisition-20260912.json').read_text())
        for case in proof['cases']:
            with self.subTest(case=case['name']):
                lua=LuaRuntime(unpack_returned_tuples=True)
                lua.globals().package.path=FSE.as_posix()+'/?.lua;'+lua.globals().package.path
                lua.execute((FSE/'NewOakValeIntro/Entities/NOVI_LiveFather.lua').read_text())
                result=lua.eval('''function(acquisitions,stop_at)
                    local events,queries,attempt,held,finished={},0,0,false,false
                    local function emit(e) events[#events+1]=e end
                    local q=setmetatable({}, {__index=function() return function() end end})
                    function q:IsActiveThreadTerminating()
                        queries=queries+1;return stop_at>0 and queries>=stop_at
                    end
                    local hero={}
                    function hero:AcquireControl(priority)
                        assert(priority==4)
                        while true do
                            attempt=attempt+1
                            assert(acquisitions[attempt]~=nil,'unexpected acquisition')
                            local ok=acquisitions[attempt]
                            emit('acquire_hero_'..(ok and '1' or '0'))
                            if ok then held=true;return true end
                            emit('frame')
                            if q:IsActiveThreadTerminating() then emit('release_hero');return false end
                        end
                    end
                    function hero:ReleaseControl()
                        if held then held=false;emit('release_hero') end
                    end
                    function q:GetHero() return hero end
                    function q:StartMovieSequence() emit('movie_start') end
                    function q:PauseAllNonScriptedEntities(v) if v then emit('pause') end end
                    function q:FixMovieSequenceCamera(v) if v then emit('fix_camera') end end
                    function q:RunCutsceneWithSetup() emit('macro');error('STOP_AT_MACRO') end
                    function q:SetStateBool(key,v) if key=='DadFinishedIntro' then finished=v end end
                    local function up(f,wanted)
                        for i=1,100 do local n,v=debug.getupvalue(f,i);if n==wanted then return v end;if not n then break end end
                        error('missing '..wanted)
                    end
                    local ok,err=pcall(up(up(Main,'run'),'intro_cutscene'),q,{})
                    if not ok then assert(tostring(err):find('STOP_AT_MACRO'),err)
                    else emit('release_father') end
                    return {events=events,queries=queries,finished=finished}
                end''')(lua.table_from(case['acquisitions']),case['stop_at'])
                self.assertEqual(list(result['events'].values()),case['events'])
                self.assertEqual(result['queries'],case['queries'])
                self.assertEqual(result['finished'],case['dad_finished'])


if __name__=='__main__':unittest.main()

from pathlib import Path
import hashlib
import json
import unittest

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_cleanup_regions import hoist_cleanup_regions
from lupa.lua54 import LuaRuntime

ROOT = Path(__file__).resolve().parents[2]
AFTER = ROOT / 'refs/script_recovery/lifted/TraderEscort/readable/FSE/TraderEscort/Entities/EndTrader.lua'


def run(path, *, second_speech_ok=True, cancel_at_speech=0):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.globals().SECOND_SPEECH_OK = second_speech_ok
    lua.globals().STOP_AT_SPEECH = cancel_at_speech
    lua.execute('''
        calls = {}
        local function note(k) table.insert(calls,k) end
        hero = {}; me = {}
        local speechCount = 0
        setmetatable(me,{__index=function(_,k)
            return function(self,...)
                if k == 'Speak' then speechCount=speechCount+1; note('Speak:'..speechCount); return speechCount == 1 or SECOND_SPEECH_OK end
                if k == 'IsPerformingScriptTask' then return speechCount >= 2 end
                if k == 'IsTalkedToByHero' then return true end
                return false
            end
        end})
        resources=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                if k == 'NewResource' then return 'resource' end
                if k == 'TryAcquire' then return true end
                if k == 'ScriptThing' then return me end
                if k == 'StartMovie' then movies=(movies or 0)+1;return 'movie'..movies end
                if k == 'DestroyMovie' then note('DestroyMovie:'..tostring((...)));return end
                if k == 'ReleaseResource' then note('ReleaseResource:'..tostring((...)));return end
                return true
            end
        end})
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                if k == 'GetHero' then return hero end
                if k == 'RetailResources' then return resources end
                if k == 'NewScriptFrame' then frames=(frames or 0)+1;return true end
                if k == 'IsActiveThreadTerminating' then return (frames or 0)>=2 and speechCount>=STOP_AT_SPEECH end
                if k == 'GetThingWithScriptName' then return {} end
                if k == 'GetStateBool' then return (...) == 'IntroFinished' end
                if k == 'IsDistanceBetweenThingsUnder' then return true end
                if k == 'GetStateInt' then return 0 end
                if k == 'GetAllThingsWithScriptName' then return {} end
                if k == 'GetHealth' then return 10 end
                if k == 'IsRegionLoaded' then return true end
                if k == 'PauseAllNonScriptedEntities' then note('Pause:'..tostring((...)));return end
                return nil
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().Main(lua.globals().quest, lua.globals().me)
    return list(lua.globals().calls.values())



SAFE = '''function Main(log, stop)
    if stop then
        if true then return end  -- TODO(native): goto LAB_00d00002
    end
    if true then
        -- LAB_00d00002: (native jump target)
        log("movie")
        goto FLOW_done
    else
        log("other")
    end
    ::FLOW_done::
    if stop then log("resource") end
end
'''
UNSAFE = '''function Main(log, stop)
    if stop then
        if true then return end  -- TODO(native): goto LAB_00d00002
    end
    if true then
        -- LAB_00d00002: (native jump target)
        log("movie")
        goto FLOW_done
        ::FLOW_done::
        if stop then log("resource") end
    end
end
'''


class EndTraderCleanupReview(unittest.TestCase):
    def test_native_termination_tail(self):
        typed = json.loads((ROOT / 'refs/script_recovery/trader_escort/translation_unit_typed.json').read_text())
        fn = next(fn for fn in typed['functions'] if fn['address'].lower() == '0x00e055b0')
        rdata = RData()
        begin, end = int(fn['address'],16), int(fn['bodyEndExclusive'],16)
        self.assertEqual(end, 0xe0604c)
        self.assertEqual(hashlib.sha256(fn['decompile'].encode()).hexdigest(),
                         'aa449761d062b317c37b237c3551465551c566f4a289211e74ece4fc1498e5de')
        self.assertEqual(hashlib.sha256(rdata.bytes_at(begin,end-begin)).hexdigest(),
                         '1f82b0ec2a6f964c3f808d165acbc837e3cad42c293c103e975ef8b722ad29a2')
        tail = rdata.bytes_at(0xe0600e,0xe0604c-0xe0600e)
        self.assertEqual(hashlib.sha256(tail).hexdigest(),
                         'c34a4837e65c88b6a3d59e3abe3771cdac00a2d4787f5928c663476f75e1da4b')
        instructions = {i.address:(i.mnemonic,i.op_str) for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(rdata.bytes_at(begin,end-begin),begin)}
        self.assertEqual(instructions[0xe05a4d], ('jne','0xe0600e'))
        self.assertEqual(instructions[0xe06021], ('call','0x6e7b80'))
        self.assertEqual(instructions[0xe0602a], ('call','0x8ac970'))
        self.assertEqual(instructions[0xe0603c], ('call','0x7e74d0'))

    def test_legal_continuation_and_unsafe_fallback(self):
        safe, _ = hoist_cleanup_regions(SAFE)
        unsafe, _ = hoist_cleanup_regions(UNSAFE)
        self.assertIn('__region_LAB_00d00002(); goto FLOW_done',safe)
        self.assertIn('return end  -- TODO(native): goto FLOW_done',unsafe)

    def test_termination_releases_resource(self):
        source=AFTER.read_text()
        self.assertIn('ReleaseEverything(); goto FLOW_after_lab_00e0602a',source)
        self.assertNotIn('ReleaseEverything(); return  -- TODO(native)',source)
        early=run(AFTER)
        self.assertEqual(early[-2:],['DestroyMovie:movie1','ReleaseResource:resource'])
        self.assertNotIn('Speak:2',early)
        calls=run(AFTER,cancel_at_speech=2)
        self.assertEqual(calls[-2:],['DestroyMovie:movie2','ReleaseResource:resource'])
        cancelled=run(AFTER,second_speech_ok=False,cancel_at_speech=2)
        self.assertEqual(cancelled[-2:],['DestroyMovie:movie2','ReleaseResource:resource'])


if __name__ == '__main__':
    unittest.main()

import json
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
try:
    from tools.script_recovery.native_reviewed_singing_effect_push import BLOCK, DIAGNOSTICS, restore
except ImportError:
    from work.codex_lua_singing_effect_push_20261004.native_reviewed_singing_effect_push import BLOCK, DIAGNOSTICS, restore

ROOT = Path(__file__).resolve().parents[2]
W = ROOT / 'work/codex_lua_singing_effect_push_20261004'
BEFORE = W / 'readable_comparison/before/singing_stones/FSE/V_SingingStones/Entities/SingingStone.lua'
AFTER = ROOT / 'refs/script_recovery/lifted/SingingStones/readable/FSE/V_SingingStones/Entities/SingingStone.lua'
if not (ROOT / 'tools/script_recovery/native_reviewed_singing_effect_push.py').exists():
    AFTER = W / 'readable_comparison/after/singing_stones/FSE/V_SingingStones/Entities/SingingStone.lua'
QUEST = ROOT / 'refs/script_recovery/lifted/SingingStones/readable/FSE/V_SingingStones/V_SingingStones.lua'


def play(path, hit):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.globals().HIT = hit
    lua.execute('''
        events={}
        effects={}
        state={CurrentPlayListIndex=0}
        terminated=false
        me=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                if k=='GetDataString' then return '2' end
                if k=='GetPos' then return {x=1,y=2,z=3} end
                if k=='MsgIsHitByHero' then return HIT end
                if k=='MsgIsHitByAnySpecialAbilityFromHero' then return false end
                return false
            end
        end})
        local regionChecks=0
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='GetStateInt' then return state[args[1]] or -1 end
                if k=='SetStateInt' then state[args[1]]=args[2];return end
                if k=='IsActiveThreadTerminating' then return terminated end
                if k=='NewScriptFrame' then terminated=true;return false end
                if k=='GetHero' or k=='GetNearestWithScriptName' or k=='GetThingWithScriptName' then return {} end
                if k=='AddNewConversation' then return 1 end
                if k=='CreateEffectAtPos' then
                    local pos=args[2]
                    table.insert(events,{'create',pos.x,pos.y,pos.z})
                    return {id=#events,IsAlive=function(self) return true end}
                end
                if k=='StateListPush' then table.insert(effects,args[2]);table.insert(events,{'push',args[1]});return end
                if k=='GetStateListCopy' then table.insert(events,{'copy',args[1]});return effects end
                if k=='RemoveThing' then table.insert(events,{'remove',args[1].id,args[2],args[3]});return end
                if k=='StateListClear' then table.insert(events,{'clear',args[1]});effects={};return end
                if k=='IsRegionLoaded' then regionChecks=regionChecks+1;return regionChecks==1 end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().Init(lua.globals().quest, lua.globals().me)
    lua.globals().Main(lua.globals().quest, lua.globals().me)
    lua.globals().terminated = False
    lua.execute(QUEST.read_text())
    lua.globals().WatchForRegionLeaving(lua.globals().quest)
    return [tuple(row.values()) for row in lua.globals().events.values()]


class SingingEffectPushReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit_path = ROOT / 'refs/script_recovery/singing_stones/units/V_SingingStones.json'
        if not (ROOT / 'tools/script_recovery/native_reviewed_singing_effect_push.py').exists():
            unit_path = W / 'candidate_unit.json'
        unit = json.loads(unit_path.read_text())
        cls.row = unit['reviewedSingingEffectPush']['0x00ed3370']
        cls.quest = unit['quest']
        typed = json.loads((ROOT / 'refs/script_recovery/singing_stones/translation_unit_typed.json').read_text())
        cls.fn = next(f for f in typed['functions'] if f['address'].lower() == '0x00ed3370')
        cls.rdata = RData()

    def test_native_and_lifted_gates(self):
        source = (W / 'baseline_draft.lua').read_text()
        report = json.loads((W / 'baseline_report.json').read_text())
        todo = next(f['todo'] for f in report['units'][0]['functions'] if f['address'].lower() == '0x00ed3370')
        result, remaining, calls = restore(source, todo[:], [], self.fn, self.row,
                                           self.rdata, self.quest)
        self.assertNotIn(BLOCK, result)
        self.assertIn('quest:StateListPush("EffectList", pCVar3)', result)
        self.assertEqual(len(remaining), len(todo)-len(DIAGNOSTICS))
        self.assertIn('StateListPush', calls)
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(source, todo[:], [], self.fn, {**self.row, 'nativeSha256': '0'*64},
                    self.rdata, self.quest)
        with self.assertRaisesRegex(ValueError, 'effect list layout changed'):
            restore(source, todo[:], [], self.fn, self.row, self.rdata,
                    {**self.quest, 'unmappedFields': []})

    def test_created_effect_is_owned_and_removed(self):
        before = play(BEFORE, True)
        after = play(AFTER, True)
        self.assertIn(('create', 1, 2, 4), after)
        self.assertNotIn(('push', 'EffectList'), before)
        self.assertIn(('push', 'EffectList'), after)
        self.assertIn(('remove', 1, False, True), after)
        self.assertEqual(after[-3:], [('copy', 'EffectList'),
                                      ('remove', 1, False, True),
                                      ('clear', 'EffectList')])
        no_hit = play(AFTER, False)
        self.assertNotIn(('push', 'EffectList'), no_hit)


if __name__ == '__main__':
    unittest.main()

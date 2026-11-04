import json
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
try:
    from tools.script_recovery.native_reviewed_singing_effect_named import HIT, PRIOR, POSITION_DIAGNOSTICS, restore
except ImportError:
    from work.codex_lua_singing_effect_named_20261004.native_reviewed_singing_effect_named import HIT, PRIOR, POSITION_DIAGNOSTICS, restore

ROOT = Path(__file__).resolve().parents[2]
W = ROOT / 'work/codex_lua_singing_effect_named_20261004'
BEFORE = W / 'readable_comparison/before/singing_stones/FSE/V_SingingStones/Entities/SingingStone.lua'
AFTER = ROOT / 'refs/script_recovery/lifted/SingingStones/readable/FSE/V_SingingStones/Entities/SingingStone.lua'
if not (ROOT / 'tools/script_recovery/native_reviewed_singing_effect_named.py').exists():
    AFTER = W / 'readable_comparison/after/singing_stones/FSE/V_SingingStones/Entities/SingingStone.lua'


def effects(path, prior):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.globals().PRIOR = prior
    lua.execute('''
        events={}
        state={CurrentPlayListIndex=PRIOR and 1 or 0}
        if PRIOR then state.YourPersonalPlayList_0=2 end
        terminated=false
        me=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                if k=='GetDataString' then return '2' end
                if k=='GetPos' then return {x=1,y=2,z=3} end
                if k=='MsgIsHitByHero' then return not PRIOR end
                if k=='MsgIsHitByAnySpecialAbilityFromHero' then return false end
                return false
            end
        end})
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='GetStateInt' then return state[args[1]] or -1 end
                if k=='SetStateInt' then state[args[1]]=args[2];return end
                if k=='IsActiveThreadTerminating' then return terminated end
                if k=='NewScriptFrame' then terminated=true;return false end
                if k=='GetHero' or k=='GetNearestWithScriptName' or k=='GetThingWithScriptName' then return {} end
                if k=='IsDistanceBetweenThingsUnder' or k=='IsCameraPosOnScreen' then return true end
                if k=='AddNewConversation' then return 1 end
                if k=='CreateEffectAtPos' then
                    local p=args[2]
                    table.insert(events,{'create',args[1],p.x,p.y,p.z,
                                         tostring(args[3]),tostring(args[4]),type(args[5]),tostring(args[6])})
                    return {id=#events}
                end
                if k=='StateListPush' then table.insert(events,{'push',args[1]});return end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().Init(lua.globals().quest, lua.globals().me)
    lua.globals().Main(lua.globals().quest, lua.globals().me)
    return [tuple(row.values()) for row in lua.globals().events.values()]


class SingingEffectNamedReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit_path = ROOT / 'refs/script_recovery/singing_stones/units/V_SingingStones.json'
        if not (ROOT / 'tools/script_recovery/native_reviewed_singing_effect_named.py').exists():
            unit_path = W / 'candidate_unit.json'
        unit = json.loads(unit_path.read_text())
        cls.row = unit['reviewedSingingEffectNamed']['0x00ed3370']
        cls.sidecar = unit['reviewedSingingEffectNamed']['sidecar']
        typed = json.loads((ROOT / 'refs/script_recovery/singing_stones/translation_unit_typed.json').read_text())
        cls.fn = next(f for f in typed['functions'] if f['address'].lower() == '0x00ed3370')
        cls.rdata = RData()

    def test_native_and_sidecar_gates(self):
        source = (W / 'baseline_draft.lua').read_text()
        report = json.loads((W / 'baseline_report.json').read_text())
        todo = next(f['todo'] for f in report['units'][0]['functions'] if f['address'].lower() == '0x00ed3370')
        result, remaining, _ = restore(source, todo[:], [], self.fn, self.row, self.rdata, self.sidecar)
        self.assertNotIn(HIT, result)
        self.assertNotIn(PRIOR, result)
        self.assertEqual(len(remaining), len(todo)-len(POSITION_DIAGNOSTICS))
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(source, todo[:], [], self.fn, {**self.row, 'nativeSha256': '0'*64},
                    self.rdata, self.sidecar)
        with self.assertRaisesRegex(ValueError, 'sidecar build report changed'):
            restore(source, todo[:], [], self.fn, self.row, self.rdata,
                    {**self.sidecar, 'buildReportSha256': '0'*64})

    def test_hit_and_prior_effect_arguments(self):
        expected = ('create', 'MARKTELEPORTER', 1, 2, 4.0,
                    '0.0', 'false', 'boolean', 'StonesEffect')
        for prior in (False, True):
            with self.subTest(prior=prior):
                after = effects(AFTER, prior)
                self.assertIn(expected, after)
                self.assertIn(('push', 'EffectList'), after)
                before = effects(BEFORE, prior)
                self.assertNotIn(expected, before)
                if prior:
                    self.assertIn(('create', 'MARKTELEPORTER', 1, 2, 3, '0.0', 'false', 'table', 'nil'), before)


if __name__ == '__main__':
    unittest.main()

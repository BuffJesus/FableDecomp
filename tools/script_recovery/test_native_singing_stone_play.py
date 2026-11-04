import json
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
try:
    from tools.script_recovery.native_reviewed_singing_stone_play import restore
except ImportError:
    from work.codex_lua_singing_stone_play_20261004.native_reviewed_singing_stone_play import restore

ROOT = Path(__file__).resolve().parents[2]
W = ROOT / 'work/codex_lua_singing_stone_play_20261004'
BEFORE = W / 'readable_comparison/before/singing_stones/FSE/V_SingingStones/Entities/SingingStone.lua'
AFTER = ROOT / 'refs/script_recovery/lifted/SingingStones/readable/FSE/V_SingingStones/Entities/SingingStone.lua'
if not (ROOT / 'tools/script_recovery/native_reviewed_singing_stone_play.py').exists():
    AFTER = W / 'readable_comparison/after/singing_stones/FSE/V_SingingStones/Entities/SingingStone.lua'


def play(path, stone, initial, hero_hit=False, special_hit=False, heal_hit=False, prior=None):
    lua = LuaRuntime(unpack_returned_tuples=True)
    values = {'CurrentPlayListIndex': initial}
    if prior is not None:
        values['YourPersonalPlayList_0'] = prior
    lua.globals().VALUES = lua.table_from(values)
    lua.globals().STONE = str(stone)
    lua.globals().HERO_HIT = hero_hit
    lua.globals().SPECIAL_HIT = special_hit
    lua.globals().HEAL_HIT = heal_hit
    lua.execute('''
        events={}
        terminated=false
        me=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                if k=='GetDataString' then return STONE end
                if k=='GetPos' then return {x=1,y=2,z=3} end
                if k=='MsgIsHitByHero' then return HERO_HIT end
                if k=='MsgIsHitByAnySpecialAbilityFromHero' then return SPECIAL_HIT end
                if k=='MsgIsHitByHeroSpecialAbility' then return HEAL_HIT end
                return false
            end
        end})
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='GetStateInt' then return VALUES[args[1]] or -1 end
                if k=='SetStateInt' then VALUES[args[1]]=args[2];table.insert(events,{'state',args[1],args[2]});return end
                if k=='IsActiveThreadTerminating' then return terminated end
                if k=='NewScriptFrame' then terminated=true;return false end
                if k=='GetHero' or k=='GetNearestWithScriptName' or k=='GetThingWithScriptName' then return {} end
                if k=='IsDistanceBetweenThingsUnder' or k=='IsCameraPosOnScreen' then return true end
                if k=='AddNewConversation' then return 1 end
                if k=='AddLineToConversation' then table.insert(events,{'line',args[2]});return end
                if k=='CreateEffectAtPos' then return {IsAlive=function() return true end} end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().Init(lua.globals().quest, lua.globals().me)
    lua.globals().Main(lua.globals().quest, lua.globals().me)
    return [tuple(row.values()) for row in lua.globals().events.values()]


class SingingStonePlayReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit_path = ROOT / 'refs/script_recovery/singing_stones/units/V_SingingStones.json'
        if not (ROOT / 'tools/script_recovery/native_reviewed_singing_stone_play.py').exists():
            unit_path = W / 'candidate_unit.json'
        unit = json.loads(unit_path.read_text())
        cls.row = unit['reviewedSingingStonePlay']['0x00ed3370']
        cls.quest_fields = unit['quest']['fields']
        cls.entity_fields = unit['entities']['SingingStone']['fields']
        typed = json.loads((ROOT / 'refs/script_recovery/singing_stones/translation_unit_typed.json').read_text())
        cls.fn = next(f for f in typed['functions'] if f['address'].lower() == '0x00ed3370')
        cls.rdata = RData()

    def test_native_and_lifted_gates(self):
        source = (W / 'baseline_draft.lua').read_text()
        report = json.loads((W / 'baseline_report.json').read_text())
        todo = next(f['todo'] for f in report['units'][0]['functions'] if f['address'].lower() == '0x00ed3370')
        result, remaining, calls = restore(source, todo[:], [], self.fn, self.row, self.rdata,
                                           self.quest_fields, self.entity_fields)
        self.assertIn('YourPersonalPlayList_', result)
        self.assertIn('SetStateInt', calls)
        self.assertEqual(len(remaining), len(todo)-5)
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(source, todo[:], [], self.fn, {**self.row, 'nativeSha256': '0'*64},
                    self.rdata, self.quest_fields, self.entity_fields)
        with self.assertRaisesRegex(ValueError, 'field layout changed'):
            restore(source, todo[:], [], self.fn, self.row, self.rdata,
                    {**self.quest_fields, '0x6c': ['Wrong', 'Int']}, self.entity_fields)

    def test_hit_and_playlist_paths(self):
        cases = (
            (0, True, False, False, [('state', 'YourPersonalPlayList_0', 2), ('state', 'CurrentPlayListIndex', 1)]),
            (0, False, True, False, [('state', 'YourPersonalPlayList_0', 2), ('state', 'CurrentPlayListIndex', 1)]),
            (0, False, True, True, []),
            (0, False, False, False, []),
        )
        for initial, hero, special, heal, expected in cases:
            with self.subTest(hero=hero, special=special, heal=heal):
                after = play(AFTER, 2, initial, hero, special, heal)
                states = [event for event in after if event[0] == 'state']
                self.assertEqual(states, expected)
                before = play(BEFORE, 2, initial, hero, special, heal)
                self.assertNotIn(('state', 'YourPersonalPlayList_0', 2), before)

    def test_repeated_stone_is_skipped(self):
        repeated = play(AFTER, 2, 1, hero_hit=True, prior=2)
        self.assertEqual([event for event in repeated if event[0] == 'state'], [])
        new_stone = play(AFTER, 2, 1, hero_hit=True, prior=1)
        self.assertEqual([event for event in new_stone if event[0] == 'state'],
                         [('state', 'YourPersonalPlayList_1', 2),
                          ('state', 'CurrentPlayListIndex', 2)])


if __name__ == '__main__':
    unittest.main()

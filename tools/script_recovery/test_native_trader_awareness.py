import json
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_trader_awareness import (
    DIAGNOSTIC, NEW, OLD, restore,
)

ROOT = Path(__file__).resolve().parents[2]
W = ROOT / 'work/codex_lua_trader_awareness_20261004'
BEFORE = W / 'readable_comparison/before/trader_conflict/FSE/TraderConflictGood/TraderConflictGood.lua'
AFTER = W / 'readable_comparison/after/trader_conflict/FSE/TraderConflictGood/TraderConflictGood.lua'


def play(path, distance=False, aware=False):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.globals().DISTANCE = distance
    lua.globals().AWARE = aware
    lua.execute('''
        events={}
        terminated=false
        awareCount=0
        targetCount=0
        HERO={id='hero'}
        CREATURES={}
        for i=1,2 do
            CREATURES[i]=setmetatable({id=i}, {__index=function(_,k)
                return function() return k=='MsgIsHitBy' end
            end})
        end
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='GetHero' then return HERO end
                if k=='GetStateInt' then return 0 end
                if k=='IsActiveThreadTerminating' then return terminated end
                if k=='NewScriptFrame' then return true end
                if k=='GetStateListCount' then return 2 end
                if k=='GetStateListAt' then return CREATURES[args[2]+1] end
                if k=='IsDistanceBetweenThingsUnder' then return DISTANCE end
                if k=='IsThingAwareOfOtherThingInAnyWay' then
                    table.insert(events,{'aware',args[1]==HERO and 'hero' or 'other',
                        args[2] and args[2].id or '<nil>'})
                    awareCount=awareCount+1
                    if awareCount==2 then terminated=true end
                    return AWARE
                end
                if k=='GiveThingBestEnemyTarget' then
                    table.insert(events,{'target',args[1].id,args[2]==HERO and 'hero' or 'other'})
                    targetCount=targetCount+1
                    if targetCount==2 then terminated=true end
                end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().WatchForHittingEnemies(lua.globals().quest)
    return [tuple(row.values()) for row in lua.globals().events.values()]


class TraderAwarenessReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit = json.loads((W / 'candidate_unit.json').read_text())
        cls.row = unit['reviewedTraderAwareness']['0x00dfc630']
        typed = json.loads((ROOT / 'refs/script_recovery/trader_conflict/translation_unit_typed.json').read_text())
        cls.fn = next(f for f in typed['functions'] if f['address'].lower() == '0x00dfc630')
        cls.rdata = RData()

    def test_native_and_lifted_gates(self):
        source = (W / 'baseline_draft.lua').read_text()
        report = json.loads((W / 'baseline_report.json').read_text())
        todo = next(f['todo'] for unit in report['units'] for f in unit['functions']
                    if f['address'].lower() == '0x00dfc630')
        result, remaining, calls = restore(source, todo[:], [], self.fn, self.row, self.rdata)
        self.assertEqual(result.count(NEW), 1)
        self.assertEqual(len(remaining), len(todo)-1)
        self.assertEqual(calls, [])
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(source, todo[:], [], self.fn,
                    {**self.row, 'nativeSha256': '0'*64}, self.rdata)
        with self.assertRaisesRegex(ValueError, 'lifted awareness operands changed'):
            restore(source.replace(OLD, 'quest:OtherAwareness()'), todo[:], [],
                    self.fn, self.row, self.rdata)
        self.assertIn(DIAGNOSTIC, todo)

    def test_full_watcher_uses_inner_creature(self):
        self.assertEqual(play(BEFORE), [('aware', 'hero', '<nil>'),
                                        ('aware', 'hero', '<nil>')])
        self.assertEqual(play(AFTER), [('aware', 'hero', 1),
                                       ('aware', 'hero', 2)])
        for distance, aware in ((True, False), (False, True)):
            with self.subTest(distance=distance, aware=aware):
                before = play(BEFORE, distance, aware)
                after = play(AFTER, distance, aware)
                self.assertEqual([e for e in before if e[0] == 'target'],
                                 [e for e in after if e[0] == 'target'])
                if distance:
                    self.assertFalse(any(e[0] == 'aware' for e in after))


if __name__ == '__main__':
    unittest.main()

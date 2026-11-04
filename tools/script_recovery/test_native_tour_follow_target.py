import json
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_tour_follow_target import (
    DIAGNOSTIC, NEW, OLD, restore,
)

ROOT = Path(__file__).resolve().parents[2]
W = ROOT / 'work/codex_lua_tour_follower_20261004'
BEFORE = W / 'readable_comparison/before/tour_guide/FSE/V_TourGuide/Entities/TourGuideFollower.lua'
AFTER = W / 'readable_comparison/after/tour_guide/FSE/V_TourGuide/Entities/TourGuideFollower.lua'


def play(path):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.execute('''
        events={}
        GUIDE={id='TourGuideGuide'}
        terminated=false
        resources=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                if k=='NewResource' then return {} end
                if k=='TryAcquire' then return true end
                return 0
            end
        end})
        me=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='IsTalkedToByHero' then return true end
                if k=='FollowThing' then
                    table.insert(events,{'follow',args[1]==GUIDE and 'guide' or '<nil>',args[2],args[3]})
                    if #events==2 then terminated=true end
                end
                return 0
            end
        end})
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='GetHero' then return {} end
                if k=='RetailResources' then return resources end
                if k=='RegisterTimer' then return 3 end
                if k=='IsActiveThreadTerminating' then return terminated end
                if k=='GetThingWithScriptName' then return GUIDE end
                if k=='GetStateBool' then return false end
                if k=='GetTimer' or k=='EntityGetSex' or k=='RetailRandModulo' then return 0 end
                if k=='ReadGlobalGameData' then return 7 end
                if k=='AddNewConversation' then return 1 end
                if k=='NewScriptFrame' then return true end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().Main(lua.globals().quest, lua.globals().me)
    return [tuple(row.values()) for row in lua.globals().events.values()]


class TourFollowTargetReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit = json.loads((W / 'candidate_unit.json').read_text())
        cls.row = unit['reviewedTourFollowTarget']['0x00ee4dd0']
        typed = json.loads((ROOT / 'refs/script_recovery/tour_guide/translation_unit_typed.json').read_text())
        cls.fn = next(f for f in typed['functions'] if f['address'].lower() == '0x00ee4dd0')
        cls.rdata = RData()

    def test_native_and_lifted_gates(self):
        source = (W / 'baseline_draft.lua').read_text()
        report = json.loads((W / 'baseline_report.json').read_text())
        todo = next(f['todo'] for unit in report['units'] for f in unit['functions']
                    if f['address'].lower() == '0x00ee4dd0')
        result, remaining, calls = restore(source, todo[:], [], self.fn, self.row, self.rdata)
        self.assertEqual(result.count(NEW), 2)
        self.assertEqual(len(remaining), len(todo)-1)
        self.assertEqual(calls, [])
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(source, todo[:], [], self.fn,
                    {**self.row, 'nativeSha256': '0'*64}, self.rdata)
        with self.assertRaisesRegex(ValueError, 'lifted follow calls changed'):
            restore(source.replace(OLD, 'me:OtherFollow()'), todo[:], [],
                    self.fn, self.row, self.rdata)
        self.assertIn(DIAGNOSTIC, todo)

    def test_full_main_second_follow_retains_guide(self):
        self.assertEqual(play(BEFORE), [('follow', 'guide', 7, True),
                                        ('follow', '<nil>', 7, True)])
        self.assertEqual(play(AFTER), [('follow', 'guide', 7, True),
                                       ('follow', 'guide', 7, True)])


if __name__ == '__main__':
    unittest.main()

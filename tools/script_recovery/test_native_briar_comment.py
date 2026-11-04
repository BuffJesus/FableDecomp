"""Retail-gated Briar Rose comment selection and successful-call latch."""

import json
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_briar_comment import OLD, OLD_END, NEW, restore


ROOT = Path(__file__).resolve().parents[2]
UNIT = ROOT / 'refs/script_recovery/summoning_the_ship/units/Q_SummoningTheShip.json'
TYPED = ROOT / 'refs/script_recovery/summoning_the_ship/translation_unit_typed.json'
READABLE = ROOT / 'refs/script_recovery/lifted/SummoningTheShip/readable/FSE/SummoningTheShip/Entities/STS_BriarRose.lua'


class BriarCommentReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.row = json.loads(UNIT.read_text())['reviewedBriarComment']['0x00df1830']
        typed = json.loads(TYPED.read_text())
        cls.fn = next(fn for fn in typed['functions'] if fn['address'].lower() == '0x00df1830')
        cls.source = OLD + '\n' + OLD_END
        cls.rdata = RData()

    def test_reviewed_branch_and_unknown_source(self):
        restored = restore(self.source, self.fn, self.row, self.rdata)
        self.assertIn(NEW, restored)
        with self.assertRaisesRegex(ValueError, 'source shape changed'):
            restore(self.source.replace('&& (iVar9 < 4)', '&& (iVar9 < 5)'), self.fn, self.row, self.rdata)

    def test_unknown_native_refused(self):
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(self.source, self.fn, {**self.row, 'nativeSha256': '0' * 64}, self.rdata)

    def test_count_keys_and_success_latch(self):
        source = READABLE.read_text()
        for count, key in ((0, None), (1, 'SUMMONER_ONE_LEFT'), (2, 'SUMMONER_TWO_LEFT'),
                           (3, 'SUMMONER_THREE_LEFT'), (4, None)):
            for result in (False, True):
                with self.subTest(count=count, result=result):
                    lua = LuaRuntime(unpack_returned_tuples=True)
                    lua.globals().COUNT = count
                    lua.globals().RESULT = result
                    lua.execute('''
                        calls = {}
                        package.preload["SummoningTheShip.native_quest_helpers"] = function()
                            return {MakeBriarRoseComment=function(q,m,k)
                                table.insert(calls, tostring(k))
                                return RESULT
                            end}
                        end
                        RemoveNeighbours=function() end
                        quest=setmetatable({}, {__index=function(_,k)
                            return function(self,...)
                                if k == 'NewScriptFrame' then
                                    frames = (frames or 0) + 1
                                    if frames > 5 then done = true end
                                    return not done
                                end
                                if k == 'IsActiveThreadTerminating' then return done or false end
                                if k == 'GetStateBool' then return true end
                                if k == 'GetStateInt' then
                                    local name = ...
                                    if name == 'SummonersAlive' then return COUNT end
                                    return 0
                                end
                                if k == 'RegisterTimer' then return 1 end
                                if k == 'GetActiveQuestName' then return 'Q' end
                                if k == 'GetTimer' then return 1 end
                                return 0
                            end
                        end})
                        me=setmetatable({}, {__index=function(_,k)
                            return function() return 0 end
                        end})
                    ''')
                    lua.execute(source)
                    lua.globals().Main(lua.globals().quest, lua.globals().me)
                    selected = [call for call in lua.globals().calls.values() if str(call).startswith('SUMMONER_')]
                    self.assertEqual(selected, ([key] if result else [key] * 5) if key else [])


if __name__ == '__main__':
    unittest.main()

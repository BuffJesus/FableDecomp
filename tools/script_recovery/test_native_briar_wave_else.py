from pathlib import Path
import json
import unittest

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_briar_wave_else import OLD, NEW, restore
from lupa.lua54 import LuaRuntime

ROOT = Path(__file__).resolve().parents[2]
AFTER = ROOT / 'refs/script_recovery/lifted/SummoningTheShip/readable/FSE/SummoningTheShip/Entities/STS_BriarRose.lua'


def run(source, waves, *, dead_after=2):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.globals().waves = lua.table_from(waves)
    lua.globals().dead_after = dead_after
    lua.execute('''
        calls = {}
        local function note(s) table.insert(calls, s) end
        package.preload['SummoningTheShip.native_quest_helpers'] = function()
            return {MakeBriarRoseComment=function() return true end}
        end
        RemoveNeighbours=function() end
        local function thing(name, id)
            return setmetatable({name=name,id=id}, {__index=function(t,k)
                return function()
                    if k == 'IsAlive' then return id ~= 1 or (frames or 0) < dead_after end
                    return 0
                end
            end})
        end
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                if k == 'NewScriptFrame' then
                    frames = (frames or 0) + 1
                    if frames > #waves then done = true end
                    return not done
                end
                if k == 'IsActiveThreadTerminating' then return done or false end
                if k == 'GetStateBool' then return true end
                if k == 'GetStateInt' then
                    if (...) == 'CurrentAttackWave' then return waves[math.min(frames or 1,#waves)] end
                    return 0
                end
                if k == 'RegisterTimer' then return 1 end
                if k == 'GetActiveQuestName' then return 'Q' end
                if k == 'GetTimer' then return 1 end
                if k == 'GetNearestWithScriptName' then
                    local name = select(2,...)
                    serial = (serial or 0) + 1
                    note('nearest:'..name..':'..serial)
                    return thing(name,serial)
                end
                if k == 'GetDistanceBetweenThings' then
                    local target = select(2,...)
                    return target.name == 'SummonerAttacker' and 2 or 3
                end
                if k == 'GiveThingBestEnemyTarget' then
                    local target = select(2,...)
                    note('target:'..target.name..':'..target.id)
                end
                return 0
            end
        end})
        me=setmetatable({}, {__index=function(_,k)
            return function() return 0 end
        end})
    ''')
    lua.execute(source.read_text())
    lua.globals().Main(lua.globals().quest, lua.globals().me)
    return list(lua.globals().calls.values())



class BriarWaveElseReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit = json.loads((ROOT / 'refs/script_recovery/summoning_the_ship/units/Q_SummoningTheShip.json').read_text())
        cls.row = unit['reviewedBriarWaveElse']['0x00df1830']
        typed = json.loads((ROOT / 'refs/script_recovery/summoning_the_ship/translation_unit_typed.json').read_text())
        cls.fn = next(fn for fn in typed['functions'] if fn['address'].lower() == '0x00df1830')
        cls.rdata = RData()

    def test_reviewed_split_and_unknown_source(self):
        self.assertEqual(restore(OLD, self.fn, self.row, self.rdata), NEW)
        with self.assertRaisesRegex(ValueError, 'printed else changed'):
            restore(OLD.replace('x_stk_7c == 1', 'x_stk_7c == 2'), self.fn, self.row, self.rdata)

    def test_unknown_native_refused(self):
        with self.assertRaisesRegex(ValueError, 'native body changed'):
            restore(OLD, self.fn, {**self.row, 'nativeSha256': '0' * 64}, self.rdata)

    def test_wave_transition_retargets_only_after_first_wave(self):
        self.assertEqual(run(AFTER, [1, 1, 1]),
                         ['nearest:SummonerAttacker:1', 'target:SummonerAttacker:1'])
        self.assertEqual(run(AFTER, [1, 2, 2]),
                         ['nearest:SummonerAttacker:1', 'target:SummonerAttacker:1',
                          'nearest:SummonerAttacker:2', 'nearest:SummonerMinion:3',
                          'target:SummonerAttacker:2'])
        self.assertEqual(run(AFTER, [2, 2, 2]), [])
        self.assertEqual(run(AFTER, [1, 2, 2], dead_after=9),
                         ['nearest:SummonerAttacker:1', 'target:SummonerAttacker:1'])


if __name__ == '__main__':
    unittest.main()

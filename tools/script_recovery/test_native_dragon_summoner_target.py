from pathlib import Path
import json
import unittest

from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_dragon_summoner_target import OLD, NEW, restore
from lupa.lua54 import LuaRuntime

ROOT = Path(__file__).resolve().parents[2]
AFTER = ROOT / 'refs/script_recovery/lifted/DragonBossFight/readable/FSE/DragonBossFight/Entities/Dragon.lua'


def run(path, phase, health):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.globals().PHASE = phase
    lua.globals().HEALTH = health
    lua.execute('''
        events = {}
        package.preload['DragonBossFight.native_quest_helpers'] = function()
            return {SpawnEnemies=function() end, helper_D258C0=function() end}
        end
        me={}
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                if k == 'NewScriptFrame' then frames=(frames or 0)+1;return frames<=2 end
                if k == 'GetStateInt' then
                    local key=...
                    if key == 'DragonState' then return PHASE end
                    return 0
                end
                if k == 'SetStateInt' then
                    local key,value=...
                    table.insert(events,key..':'..value)
                    if key == 'DragonState' then PHASE=value end
                    return
                end
                if k == 'GetHealth' then return HEALTH end
                if k == 'IsCreatureFlying' then return false end
                if k == 'IsActiveThreadTerminating' then return false end
                if k == 'ReadGlobalGameData' then
                    local offset=...
                    if offset==2996 then return 75 end
                    if offset==3000 then return 50 end
                    if offset==3004 then return 25 end
                    return offset
                end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().Init(lua.globals().quest, lua.globals().me)
    lua.globals().Main(lua.globals().quest, lua.globals().me)
    return lua.globals().PHASE, list(lua.globals().events.values())



class DragonSummonerTargetReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit=json.loads((ROOT/'refs/script_recovery/dragon_boss_fight/units/Q_DragonBossFight.json').read_text())
        cls.row=unit['reviewedDragonSummonerTarget']['0x00d25a00']
        typed=json.loads((ROOT/'refs/script_recovery/dragon_boss_fight/translation_unit_typed.json').read_text())
        cls.fn=next(fn for fn in typed['functions'] if fn['address'].lower()=='0x00d25a00')
        cls.source=('this_00 = *(CScriptBase **)(this + 0x14);\n' * 3) + OLD
        cls.rdata=RData()

    def test_reviewed_parent_write_and_unknown_source(self):
        self.assertEqual(restore(self.source,self.fn,self.row,self.rdata),self.source.replace(OLD,NEW))
        with self.assertRaisesRegex(ValueError,'alias source changed'):
            restore(self.source.replace(OLD,OLD.replace('0x6c','0x68')),self.fn,self.row,self.rdata)

    def test_unknown_native_refused(self):
        with self.assertRaisesRegex(ValueError,'native body changed'):
            restore(self.source,self.fn,{**self.row,'nativeSha256':'0'*64},self.rdata)

    def test_health_phase_summoner_targets(self):
        expected={
            (0,60):(1,['DragonState:1','TargetNumMinions:3012','TargetNumSummoners:3028']),
            (1,40):(2,['DragonState:2','TargetNumMinions:3016','TargetNumSummoners:3032']),
            (2,10):(3,['DragonState:3','TargetNumMinions:3020','TargetNumSummoners:3036']),
            (3,10):(3,[]),(0,100):(0,[]),(1,60):(1,[]),(2,30):(2,[]),
        }
        for (phase,health),want in expected.items():
            with self.subTest(phase=phase,health=health):
                self.assertEqual(run(AFTER,phase,health),want)


if __name__=='__main__':
    unittest.main()

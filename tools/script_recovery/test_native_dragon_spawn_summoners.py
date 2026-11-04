import json
import unittest
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_dragon_spawn_summoners import (
    restore, TODO_LINE, POS_LINE, EFFECT_OLD, EFFECT_NEW, CREATURE_OLD, CREATURE_NEW, GET_POS,
)
from lupa.lua54 import LuaRuntime

ROOT = Path(__file__).resolve().parents[2]
AFTER = ROOT / "refs/script_recovery/lifted/DragonBossFight/readable/FSE/DragonBossFight/DragonBossFight.lua"

def run(path, rolls, count, start=0, terminating=False):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.globals().ROLLS = lua.table_from(rolls)
    lua.globals().COUNT = count
    lua.globals().START = start
    lua.globals().TERMINATING = terminating
    lua.execute('''
        events={}
        state={TargetNumSummoners=COUNT,NumSummoners=0,NextSummonerSpawnPoint=START}
        rollIndex=0
        quest=setmetatable({}, {__index=function(_,k)
            return function(self,...)
                local args={...}
                if k=='GetStateInt' then return state[args[1]] or 0 end
                if k=='SetStateInt' then state[args[1]]=args[2];return end
                if k=='IsActiveThreadTerminating' then return TERMINATING end
                if k=='RetailRandModulo' then rollIndex=rollIndex+1;return ROLLS[rollIndex] or 0 end
                if k=='GetStateListCount' then return 2 end
                if k=='GetStateListAt' then
                    local index=args[2]
                    return {GetPos=function() return 'position'..index end}
                end
                if k=='CreateEffectAtPos' or k=='CreateCreature' then
                    table.insert(events,{method=k,arg1=args[1],arg2=args[2],arg3=args[3]})
                    return {}
                end
                return 0
            end
        end})
    ''')
    lua.execute(path.read_text())
    lua.globals().SpawnSummoners(lua.globals().quest)
    return [(e['method'],e['arg1'],e['arg2'],e['arg3']) for e in lua.globals().events.values()],lua.globals().state['NextSummonerSpawnPoint']


class DragonSpawnSummonersReview(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        unit=json.loads((ROOT/'refs/script_recovery/dragon_boss_fight/units/Q_DragonBossFight.json').read_text())
        cls.row=unit['reviewedDragonSpawnSummoners']['0x00d26e60']
        cls.fields=unit['quest']['fields']
        typed=json.loads((ROOT/'refs/script_recovery/dragon_boss_fight/translation_unit_typed.json').read_text())
        cls.fn=next(fn for fn in typed['functions'] if fn['address'].lower()=='0x00d26e60')
        cls.rdata=RData()
        cls.source=('function SpawnSummoners(quest)\n' + TODO_LINE + POS_LINE + EFFECT_OLD +
                    TODO_LINE + POS_LINE + CREATURE_OLD + 'end\n')
        cls.todo=['CreateEffectAtPos: missing position (3 args for 5 params)',
                  'CreateCreature: missing position (3 args for 4 params)',
                  'unlifted: pCVar4 = (**(*(QUESTLIST_BeginValue("SummonerSpawnPoints")',
                  'unlifted: pCVar4 = (**(*(QUESTLIST_BeginValue("SummonerSpawnPoints")']

    def test_reviewed_rewrite_and_unknown_shape(self):
        source,todo,calls=restore(self.source,self.todo,[],self.fn,self.row,self.rdata,self.fields)
        self.assertEqual(source,'function SpawnSummoners(quest)\n'+GET_POS+EFFECT_NEW+GET_POS+CREATURE_NEW+'end\n')
        self.assertEqual(todo,[])
        self.assertEqual(calls,['GetStateListAt','GetPos'])
        with self.assertRaisesRegex(ValueError,'lifted spawn shape changed'):
            restore(self.source.replace('DBSummoner','Other'),self.todo,[],self.fn,self.row,self.rdata,self.fields)

    def test_unknown_native_refused(self):
        with self.assertRaisesRegex(ValueError,'native body changed'):
            restore(self.source,self.todo,[],self.fn,{**self.row,'nativeSha256':'0'*64},self.rdata,self.fields)
        with self.assertRaisesRegex(ValueError,'field mapping changed'):
            restore(self.source,self.todo,[],self.fn,self.row,self.rdata,{})

    def test_creature_choice_position_wrap_and_termination(self):
        cases=[([0,1],2,0,False),([1,0],2,1,False),([0],1,0,False),([0],0,0,False),([0],2,0,True)]
        for rolls,count,start,terminating in cases:
            with self.subTest(rolls=rolls,count=count,start=start,terminating=terminating):
                events,next_index=run(AFTER,rolls,count,start,terminating)
                self.assertEqual(next_index,(start+count)%2 if count and not terminating else start)
                self.assertEqual(len(events),count*2 if not terminating else 0)
                for i in range(count if not terminating else 0):
                    index=(start+i)%2
                    self.assertEqual(events[2*i],('CreateEffectAtPos','SUMMON_CREATURE_SUMMON_EVIL','position'+str(index),0.0))
                    self.assertEqual(events[2*i+1],('CreateCreature','CREATURE_SUMMONER_01' if rolls[i]==0 else 'CREATURE_SUMMONER_02','position'+str(index),'DBSummoner'))

if __name__=='__main__':
    unittest.main()

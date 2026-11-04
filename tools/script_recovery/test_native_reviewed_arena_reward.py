import copy
import json
import itertools
from pathlib import Path
from unittest.mock import patch
from functools import lru_cache

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
from tools.script_recovery.native_reviewed_arena_reward import PLAY_WAVE, ROTATE, recover_arena_reward

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def reward(tmp_path_factory):
    unit = json.loads((ROOT / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/arena/translation_unit_typed.json')
    seen = {}
    def capture(source, fn, rdata):
        if int(fn['address'], 16) == PLAY_WAVE:
            seen['input'] = source, fn, rdata
        return recover_arena_reward(source, fn, rdata)
    out = tmp_path_factory.mktemp('reward')
    with patch('tools.script_recovery.native_reviewed_arena_reward.recover_arena_reward', capture), \
            patch.object(converter, 'constructor_defaults', return_value={}):
        converter.convert(unit, out)
    return (*seen['input'], (out / 'FSE/Arena/Arena.lua').read_text())


def test_reward_source_and_native_helper_drift_remain_unresolved(reward):
    source, fn, rdata, _ = reward
    assert recover_arena_reward(source, fn, rdata) != source
    for old, new in [('xStack_f4 = 14.0;', 'xStack_f4 = 15.0;'),
                     ('iVar4 - iVar6 >> 2', 'iVar4 - iVar6 >> 3'),
                     ('f_stk_ec = 5.0;', 'f_stk_ec = 0.0;'),
                     ('this + 0xf4) / 100 < 6', 'this + 0xf4) / 100 < 7')]:
        changed = source.replace(old, new)
        assert changed != source
        assert recover_arena_reward(changed, fn, rdata) == changed
    moved = copy.deepcopy(fn); moved['address'] = '0x00F1EED4'
    assert recover_arena_reward(source, moved, rdata) == source
    class Drifted:
        def bytes_at(self, at, size):
            raw = bytearray(rdata.bytes_at(at, size))
            if at == ROTATE: raw[0] ^= 0xff
            return bytes(raw)
    assert recover_arena_reward(source, fn, Drifted()) == source


HARNESS = r'''return function(points, count, choice, angle, centerX)
 local actor={IsNull=function() return false end, IsAlive=function() return false end,
              GetDataString=function() return "0" end, GetPos=function() return {x=centerX,y=20.25,z=-3.5} end}
 local q,draws,reads,rotations,spawns = {},0,{},0,0
 local state={ArenaRound=0,ArenaRoundWave=0,NewCrowdPoints=points,
              Rounds_0_NumWaves=1,Rounds_0_Waves_0_NumWaveCreatures=3}
 function q:RetailResources() return {} end
 function q:GetHero() return actor end
 function q:GetThingWithScriptName(n) return actor end
 function q:GetFurthestWithScriptName(...) return actor end
 function q:GetAllThingsWithScriptName(n) if n == "ArenaSpawn" then return {actor} end; return {} end
 function q:GetStateInt(k) return state[k] or 0 end
 function q:SetStateInt(k,v) state[k]=v end
 function q:GetStateBool(k) assert(k:find("ShortWave")); return true end
 function q:SetStateBool(k,v) end
 function q:RegisterTimer() return 11 end
 function q:DeregisterTimer(id) assert(id == 11) end
 function q:IsActiveThreadTerminating() return false end
 function q:DisplayQuestInfo(v) end
 function q:Pause(v) assert(v==1.0); error("STOP_WITHOUT_REWARD") end
 function q:ReadGlobalGameData(offset)
  reads[#reads+1]=offset
  if offset==0xa70 then return 4096+count*4 end
  assert(offset==0xa6c); return 4096
 end
 function q:RetailRandModulo(n)
  assert(n==32768);draws=draws+1
  assert(draws<=2,"extra RNG draw"); if draws==1 then return choice end; return angle
 end
 function q:RotateVectorHorizontal(v,turns)
  assert(v.x==14 and v.y==0 and v.z==5)
  assert(math.abs(turns-(angle%360)*0.0027777778450399637)<1e-12)
  assert(draws==2);rotations=rotations+1
  return {x=1000*turns,y=-100*turns,z=5.125}
 end
 function q:ReadGlobalGameDataDefStringAt(offset,index)
  assert(offset==0xa6c and index==choice%count and draws==2 and rotations==1)
  return "OBJECT_TOKEN_"..index
 end
 function q:CreateObject(def,pos,name)
  assert(def=="OBJECT_TOKEN_"..(choice%count) and name=="")
  local turns=(angle%360)*0.0027777778450399637
  assert(math.abs(pos.x-(1000*turns+centerX))<1e-10)
  assert(math.abs(pos.y-(-100*turns+20.25))<1e-10)
  assert(pos.z==1.625,"dropped center or rotated Z")
  spawns=spawns+1;error("STOP_AFTER_OBJECT")
 end
 debug.sethook(function() error("budget") end,"",100000)
 local ok,err=pcall(PlayWave,q);debug.sethook()
 assert(not ok)
 if points<100 then
  assert(tostring(err):find("STOP_WITHOUT_REWARD"),tostring(err))
  assert(draws==0 and #reads==0 and rotations==0 and spawns==0)
 elseif count==0 then
  assert(draws==1 and rotations==0 and spawns==0,"division must precede angle RNG")
  assert(tostring(err):find("zero") or tostring(err):find("n%0",1,true),tostring(err))
 else
  assert(tostring(err):find("STOP_AFTER_OBJECT"),tostring(err))
  assert(draws==2 and rotations==1 and spawns==1)
  assert(reads[1]==0xa70 and reads[2]==0xa6c and #reads==2)
 end
 return draws,rotations,spawns
end'''


@lru_cache(maxsize=1)
def readable(source):
    return readable_file(source)[0]


@pytest.mark.parametrize('points', [-100, 0, 1, 99, 100, 599, 600, 900])
@pytest.mark.parametrize('style', ['draft', 'readable'])
def test_actual_reward_branch_preserves_indices_rng_order_positions_and_threshold(reward, points, style):
    source = reward[3] if style == 'draft' else readable(reward[3])
    lua = LuaRuntime(); lua.execute(source); check = lua.execute(HARNESS)
    for args in itertools.product((1,3,7), (0,1,32767), (0,90,180,270,359,360,32767), (-25,0,200.5)):
        check(points,*args)
    check(points,0,0,0,0)

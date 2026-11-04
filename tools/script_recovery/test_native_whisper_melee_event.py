import json,itertools
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
ROOT=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def whisper(tmp_path_factory):
 unit=json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())
 c=UnitConverter(ROOT/'refs/script_recovery/arena/translation_unit_typed.json');out=tmp_path_factory.mktemp('whisper-event')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 draft=(out/'FSE/Arena/Entities/WhisperAlly.lua').read_text()
 return draft,readable_file(draft)[0]
HARNESS="return function(final, round, event, hit, heroHit, limit)\n local frames,queries,timers=0,0,0\n local trace={}\n local actor={}\n function actor:IsAlive()return true end\n function actor:IsNull()return false end\n function actor:MsgIsHitBy(name)assert(name=='');return hit end\n function actor:MsgIsHitByHero()return heroHit end\n function actor:MsgIsHitByAnySpecialAbilityFromHero()return false end\n function actor:MsgHitEnemyWithMeleeWeapon()queries=queries+1;trace[#trace+1]='melee:'..tostring(event);return event end\n local resources={}\n function resources:NewResource()return 90 end\n function resources:PrepareResource(id)assert(id==90)end\n function resources:TryAcquire(id,who,flags)assert(id==90 and who==actor and flags==4);return true end\n function resources:ReleaseResource(id)assert(id==90);trace[#trace+1]='release' end\n local q={}\n function q:RetailResources()return resources end\n function q:GetHero()return actor end\n function q:GetStateBool(name)assert(name=='WhisperNeededForCutscene' or name=='FinalBattleCS');return name=='FinalBattleCS' end\n function q:GetStateInt(name)\n  if name=='ArenaState' then return final and 8 or 2 end\n  if name=='ArenaRound' then return round end\n  if name:match('^TotalCreatures_')then return name=='TotalCreatures_0' and 1 or 0 end\n  error('unexpected state '..name)\n end\n function q:IsActiveThreadTerminating()return frames>=limit end\n function q:NewScriptFrame(who)assert(who==actor);frames=frames+1;assert(frames<=limit);return frames<limit end\n function q:RegisterTimer()timers=timers+1;return timers end\n function q:SetTimer(id,value)trace[#trace+1]='timer:'..tostring(id)..':'..tostring(value)end\n function q:GetTimer(id)return 0 end\n function q:DeregisterTimer(id)trace[#trace+1]='deregister:'..tostring(id)end\n function q:GetNearestWithScriptName(who,name)assert(who==actor);return name=='ArenaEnemy' and actor or nil end\n function q:GetHealth(who)assert(who==actor);return 100 end\n function q:AddNewConversation(who,a,b)assert(who==actor and not a and not b);return 80 end\n function q:AddPersonToConversation(id,who)assert(id==80 and who==actor)end\n function q:AddLineToConversation(id,line,who,listener,last)assert(id==80 and who==actor and listener==actor and not last);trace[#trace+1]=line end\n function q:AddQuestInfoBarHealth(...)return 70 end\n setmetatable(q,{__index=function(_,name)\n  if name:match('^Get')or name:match('^Is')or name:match('^Has')or name:match('^Read')then return function()error('unexpected '..name)end end\n  return function()end\n end})\n debug.sethook(function()error('instruction budget')end,'',100000)\n Main(q,actor)\n debug.sethook()\n assert(frames==limit)\n if not hit then assert(queries>0,'event query not reached')end\n return table.concat(trace,',')\nend"

@pytest.mark.parametrize('style',[0,1])
@pytest.mark.parametrize('final,round,event,hit,hero_hit,limit',list(itertools.product((False,True),(0,7),(False,True),(False,True),(False,True),(3,5))))
def test_actual_whisper_melee_query_and_frame_cancellation(whisper,style,final,round,event,hit,hero_hit,limit):
 lua=LuaRuntime();lua.execute(whisper[style]);check=lua.execute(HARNESS)
 assert check(final,round,event,hit,hero_hit,limit)

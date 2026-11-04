"""Actual WillWhisper dispatch retains the native fifth animation byte."""
import itertools,json,os,sys
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 c=UnitConverter(ROOT/'refs/script_recovery/guild_training/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/guild_training/units/Q_GuildTrainingWoodsWill.json').read_text())
 unit['entities']={'WillWhisper':unit['entities']['WillWhisper']};unit['quest']['functions']={};unit['quest']['helpers']={}
 out=tmp_path_factory.mktemp('will_whisper_animation_byte')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/GuildTrainingWoodsWill/Entities/WillWhisper.lua').read_text()
 return source,readable_file(source)[0]
def plain(v):
 if hasattr(v,'items'):return [plain(x) for x in v.values()]
 return v
HARNESS="\nreturn function(raw,delay,limit)\n local frames,events,prepared,live,request=0,{},false,false,nil\n local cleared={};local q,me={},{}\n local function byte()return frames<4 and raw or (raw+128)%256 end\n local function event(...)events[#events+1]={...}end\n function q:NewScriptFrame(actor)assert(actor==me);frames=frames+1;event('frame',frames);return frames<limit end\n function q:IsActiveThreadTerminating()return frames>=limit end\n function q:GetStateBool(name)\n  if name=='BanditsAlive'then return frames<6 end\n  assert(name=='WhisperAnimate');return (frames==2 or frames==4) and not cleared[frames]\n end\n function q:SetStateBool(name,value)assert(name=='WhisperAnimate' and not value);cleared[frames]=true;event('clearAnimate',frames)end\n function q:EntitySetAsKillable(actor,a,b)assert(actor==me and not a and not b)end\n function q:EntitySetAsToAddToComboMultiplierWhenHit(actor,a)assert(actor==me and not a)end\n function q:EntitySetInFaction(actor,name)assert(actor==me and name=='FACTION_HERO')end\n function q:EntitySetAllowBossPhaseChanges(actor,a)assert(actor==me and not a)end\n function me:MsgIsHitByHero()return false end\n function me:MsgIsHitByAnySpecialAbilityFromHero()return false end\n function me:PlayAnimation(key,a,b,c,d,e,f,g)\n  assert(key=='WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1' and not a and not b and not c and d and not f and not g)\n  event('animation',e and 1 or 0,byte(),frames)\n end\n local r={};function q:RetailResources()return r end\n function r:NewResource()assert(not live);live=true;event('resource',117);return 117 end\n function r:PrepareResource(id)assert(id==117 and live);prepared=true end\n function r:TryAcquire(id,actor,priority)assert(id==117 and live and prepared and actor==me and priority==4);request=request or frames;return frames>=request+delay end\n function r:ReleaseResource(id)assert(id==117 and live);live=false;event('release',id)end\n function r:PlayAnimationWithNativeArgument5(id,key,a,b,c,d,f,g)\n  assert(id==117 and live and key=='WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1' and not a and not b and not c and d and not f and not g)\n  event('animation',byte(),byte(),frames)\n end\n debug.sethook(function()error('instruction budget')end,'',30000)\n Main(q,me);debug.sethook();assert(not live,'resource leak');return events\nend\n"

@pytest.mark.parametrize('style',(0,1))
@pytest.mark.parametrize('raw,delay,limit',list(itertools.product((0,1,2,128,255),(0,2),(1,3,5,8))))
def test_actual_animation_byte_sample(generated,style,raw,delay,limit):
 lua=LuaRuntime();lua.execute(generated[style]);events=plain(lua.execute(HARNESS)(raw,delay,limit))
 assert all(e[1]==e[2] for e in events if e[0]=='animation')
 assert sum(e[0]=='resource' for e in events)==sum(e[0]=='release' for e in events)

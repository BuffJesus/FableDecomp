"""Guard native TalkingTrader2 hit-line identity and generated speech behavior."""
import json,copy
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())
HERE=Path(__file__).resolve().parent

@pytest.fixture(scope='module')
def captured(tmp_path_factory):
 from tools.script_recovery import native_reviewed_stack_aliases as aliases
 from tools.script_recovery.convert_quest_unit import UnitConverter
 from tools.script_recovery.build_readable_unit import readable_file
 candidate=HERE/'candidate_unit.json'
 unit=json.loads((candidate if candidate.exists() else ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 unit['entities']={'TalkingTrader2':unit['entities']['TalkingTrader2']};unit['quest']['functions']={};unit['quest']['helpers']={}
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json');out=tmp_path_factory.mktemp('trader_hit');result={};original=aliases.restore
 def restore(text,fn,row,rd):
  fixed=original(text,fn,row,rd)
  if fn['address'].lower()=='0x00eccad0':result.update(source=text,fixed=fixed,fn=fn,row=row,rd=rd)
  return fixed
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(aliases,'restore',restore):c.convert(unit,out)
 draft=(out/'FSE/V_SickChild/Entities/TalkingTrader2.lua').read_text(encoding='utf-8')
 result.update(draft=draft,readable=readable_file(draft,rel='FSE/V_SickChild/Entities/TalkingTrader2.lua')[0]);return result

@pytest.mark.parametrize('fault',['native','source','instruction','count','collision','missing_edit','duplicate_edit','missing_anchor','extra_use','invalid_count'])
def test_changed_evidence_rejected(captured,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=captured;row=copy.deepcopy(c['row']);fn=copy.deepcopy(c['fn']);source=c['source']
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='instruction':row['nativeInstructions'][next(iter(row['nativeInstructions']))]='00'
 elif fault=='count':row['editCounts'][0]+=1
 elif fault=='collision':row['alias']='xStack_98'
 elif fault=='missing_edit':row['edits'][1][0]='missing_alias'
 elif fault=='duplicate_edit':row['edits'][1]=row['edits'][0]
 elif fault=='missing_anchor':row['nativeInstructions']={}
 elif fault=='extra_use':source+=' xStack_8e;'
 else:row['editCounts'][0]=True
 with pytest.raises(ValueError):restore(source,fn,row,c['rd'])

def test_unreviewed_source_unchanged(captured):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=captured;assert restore(c['source'],c['fn'],None,c['rd'])==c['source']

def test_only_reviewed_string_lifetime_changed(captured):
 c=captured;assert c['fixed']==c['source'].replace('xStack_98','hitLine').replace('xStack_8e','hitLine')
 assert c['row']['editCounts']==[5,1]

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('kind',['physical','special','healing'])
@pytest.mark.parametrize('last_key',[0,30])
@pytest.mark.parametrize('speak',[False,True])
def test_complete_main_selected_hit_key(captured,style,kind,last_key,speak):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(captured[style])
 q,me,done=lua.execute(HARNESS,lua.table_from(dict(kind=kind,lastKey=last_key,tasks=2,speak=speak,limit=16)))
 lua.globals().Init(q,me);lua.globals().Main(q,me);speeches,frames=done()
 assert (speeches==0)==(kind=='healing')

# HARNESS is embedded by prepare_tests.py to keep the promoted test self-contained.

HARNESS="\nlocal s=...;local q,me,hero,other,r,resource={},{},{},{},{},{}\nlocal frames,hits,speeches,counter=0,0,0,10;local active=nil;local released=false;local pending=0;local expected\nfunction q:GetHero()return hero end\nfunction q:RetailResources()return r end\nfunction q:IsActiveThreadTerminating()return frames>=s.limit end\nfunction q:NewScriptFrame(actor)assert(actor==me);frames=frames+1;assert(frames<100);return not self:IsActiveThreadTerminating()end\nfunction q:GetThingWithScriptName(key)assert(key=='TalkingTrader1');return other end\nfunction q:GetStateBool(key)assert(key=='GotFishingSpotMushroom');return false end\nfunction q:GetHealth(actor)assert(actor==me);return 10 end\nfunction q:PauseAllNonScriptedEntities(v)assert(type(v)=='boolean')end\nfunction q:TextEntryExists(key)\n assert(key=='TEXT_QST_B10_TRADERB_ONHIT_'..counter,'wrong candidate key');local exists=counter<=s.lastKey\n if not exists then counter=10 end;expected='TEXT_QST_B10_TRADERB_ONHIT_'..counter;counter=counter+10;return exists\nend\nfunction q:SetIsPushableByHero(actor,v)assert(actor==me and not v)end\nfunction q:EntitySetAsAllowedToFollowHero(actor,v)assert(actor==me and not v)end\nfunction q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and not b and not c)end\nfunction q:SetThingPersistent(actor,v)assert(actor==me and v)end\nfunction q:EntitySetAsKillable(actor,a,b)assert(actor==me and a==b)end\nfunction q:EntitySetAsToAddToComboMultiplierWhenHit(actor,v)assert(actor==me)end\nfunction me:IsTalkedToByHero()return false end\nfunction me:MsgIsHitByHero()return s.kind=='physical' end\nfunction me:MsgIsHitByAnySpecialAbilityFromHero()return s.kind~='physical' end\nfunction me:MsgIsHitByHeroSpecialAbility(id)assert(id==14);return s.kind=='healing' end\nfunction me:Speak(actor,key,group,a,b,c)\n assert(actor==hero and group==0 and not a and b and not c)\n assert(key==expected,'WRONG_HIT_LINE:'..tostring(key)..' expected '..expected)\n speeches=speeches+1;pending=s.tasks;return s.speak\nend\nfunction me:IsPerformingScriptTask()local value=pending>0;pending=pending-1;return value end\nfunction r:NewResource()return resource end\nfunction r:PrepareResource(handle)assert(handle==resource)end\nfunction r:TryAcquire(handle,actor,kind)assert(handle==resource and actor==me and kind==4);return true end\nfunction r:ReleaseResource(handle)assert(handle==resource and not released);released=true end\nfunction r:ScriptThing(handle)assert(handle==resource);return me end\nfunction r:StartMovie(key)assert(key=='' and active==nil);active={};return active end\nfunction r:DestroyMovie(handle)assert(handle==active and active~=nil);active=nil end\nfunction r:IsHitByHeroExceptAbilityAndHealthAbove(actor,ability,minimum)\n assert(actor==me and ability==14 and minimum==0)\n return (actor:MsgIsHitByHero() or (actor:MsgIsHitByAnySpecialAbilityFromHero() and not actor:MsgIsHitByHeroSpecialAbility(ability))) and q:GetHealth(actor)>minimum\nend\nreturn q,me,function()assert(released and active==nil);return speeches,frames end\n"

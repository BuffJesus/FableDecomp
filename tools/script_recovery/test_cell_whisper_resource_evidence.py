"""CellWhisper resource identity, call arity and normal cleanup evidence gates."""
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
 unit=json.loads((candidate if candidate.exists() else ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())
 unit['entities']={'CellWhisper':unit['entities']['CellWhisper']};unit['quest']['functions']={};unit['quest']['helpers']={}
 c=UnitConverter(ROOT/'refs/script_recovery/arena/translation_unit_typed.json');out=tmp_path_factory.mktemp('cell_whisper');result={};original=aliases.restore
 def restore(text,fn,row,rd):
  fixed=original(text,fn,row,rd)
  if fn['address'].lower()=='0x00f170c0':result.update(source=text,fixed=fixed,fn=fn,row=row,rd=rd)
  return fixed
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(aliases,'restore',restore):c.convert(unit,out)
 draft=(out/'FSE/Arena/Entities/CellWhisper.lua').read_text(encoding='utf-8')
 result.update(draft=draft,readable=readable_file(draft,rel='FSE/Arena/Entities/CellWhisper.lua')[0]);return result

@pytest.mark.parametrize('fault',['native','source','instruction','count','collision','missing_edit','duplicate_edit','missing_anchor','extra_use','invalid_count'])
def test_stale_or_ambiguous_evidence_rejected(captured,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=captured;row=copy.deepcopy(c['row']);fn=copy.deepcopy(c['fn']);source=c['source']
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='instruction':row['nativeInstructions'][next(iter(row['nativeInstructions']))]='00'
 elif fault=='count':row['editCounts'][3]+=1
 elif fault=='collision':row['alias']='xStack_9c'
 elif fault=='missing_edit':row['edits'][0][0]='missing_constructor'
 elif fault=='duplicate_edit':row['edits'][1]=row['edits'][0]
 elif fault=='missing_anchor':row['nativeInstructions']={}
 elif fault=='extra_use':source+=' xStack_9c;'
 else:row['editCounts'][0]=True
 with pytest.raises(ValueError):restore(source,fn,row,c['rd'])

def test_unreviewed_source_unchanged(captured):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=captured;assert restore(c['source'],c['fn'],None,c['rd'])==c['source']

def test_exact_resource_and_call_scope(captured):
 c=captured;text=c['source'];row=c['row'];assert row['editCounts']==[1,41,2,6,1]
 for (old,new),count in zip(row['edits'],row['editCounts']):
  assert text.count(old)==count;text=text.replace(old,new)
 assert text==c['fixed']
 for style in ['draft','readable']:
  assert captured[style].count('me:Speak(')==6 and captured[style].count('me:ClearCommands()')==2
  assert '-- TODO(native):' not in captured[style]

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('route',range(8))
@pytest.mark.parametrize('busy',[False,True])
@pytest.mark.parametrize('speak',[False,True])
def test_complete_main_lifetimes_and_selected_speech(captured,style,route,busy,speak):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(captured[style])
 q,me,done=lua.execute(HARNESS,lua.table_from(dict(route=route,busy=busy,retries=2,speak=speak,health=10,limit=8,bypass=False)))
 lua.globals().Main(q,me);speeches,frames,clears=done()
 assert speeches==int(route!=7) and clears==int(busy and route!=7)

HARNESS="\nlocal s=...;local q,me,hero,r,handle,marker={},{},{},{},{},{};local pos={x=1,y=2,z=3}\nlocal frame,retries,speeches,expectedSpeech,clears=0,0,0,0,0;local sent=false;local released=false;local active=nil;local paused=false;local pending=0;local busy=s.busy;local inHit=false\nlocal keys={'THIRD','FOURTH','FIFTH','SIXTH','SEVENTH'}\nlocal function check(h)assert(h==handle or (s.bypass and h==nil),'WRONG_RESOURCE:'..tostring(h))end\nfunction q:GetHero()return hero end\nfunction q:RetailResources()return r end\nfunction q:IsActiveThreadTerminating()return frame>=s.limit end\nfunction q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;assert(frame<20);return not self:IsActiveThreadTerminating()end\nfunction q:EntityUnsetAsOpinionSource(actor,v)assert(actor==me and not v)end\nfunction q:EntitySetInFaction(actor,name)assert(actor==me and name=='FACTION_HERO')end\nfunction q:EntitySetAsKillable(actor,a,b)assert(actor==me and not a and not b)end\nfunction q:EntitySetAsToAddToComboMultiplierWhenHit(actor,v)assert(actor==me and not v)end\nfunction q:EntitySetOpinionReactionsEnabled(actor,v)assert(actor==me and not v)end\nfunction q:SetCreatureBrain(actor,name)assert(actor==me and name=='BRAIN_PASSIVE_OVERRIDE')end\nfunction q:GetThingWithScriptName(name)assert(name=='FlickPoint');return marker end\nfunction marker:GetPos()return pos end\nfunction q:SetWanderCentrePoint(actor,p)assert(actor==me and p==pos)end\nfunction q:SetWanderMinDistance(actor,v)assert(actor==me and v==0)end\nfunction q:SetWanderMaxDistance(actor,v)assert(actor==me and v==5)end\nfunction q:SetScriptingStateGroup(actor,v)assert(actor==me and v==4)end\nfunction me:IsTalkedToByHero()if s.route<5 and not sent then sent=true;return true end;return false end\nfunction me:MsgIsHitByHero()if s.route==5 and not sent then sent=true;return true end;return false end\nfunction me:MsgIsHitByAnySpecialAbilityFromHero()if s.route>=6 and not sent then sent=true;return true end;return false end\nfunction me:MsgIsHitByHeroSpecialAbility(id)assert(id==14);return s.route==7 end\nfunction me:IsPerformingScriptTask()if busy then return true end;local v=pending>0;pending=pending-1;return v end\nfunction me:ClearCommands()assert(busy);busy=false;pending=0;clears=clears+1 end\nfunction me:Speak(actor,key,group,a,b,c)\n local expected=s.route<5 and ('TEXT_QST_005_V2_ARENA_WHISPER_COMMENT_'..keys[s.route+1]..'_ROUND') or 'TEXT_QST_005_WHISPER_ENEMY_BEENHITATSTART_10'\n assert(actor==hero and key==expected and group==0 and not a and b and not c,'WRONG_SPEECH')\n speeches=speeches+1;pending=2;return s.speak\nend\nfunction q:GetStateInt(key)assert(key=='ArenaRound');return 7 end\nfunction q:GetStateBool(key)assert(key=='InHitCutsceneAlready');return inHit end\nfunction q:SetStateBool(key,v)assert(key=='InHitCutsceneAlready');inHit=v end\nfunction q:RetailRandModulo(v)assert(v==32768);return s.route end\nfunction q:GetHealth(actor)assert(actor==me);if active and s.health>0 then expectedSpeech=expectedSpeech+1 end;return s.health end\nfunction q:ModifyThingHealth(actor,amount,v)assert(actor==me and amount==10000 and not v)end\nfunction q:EntitySetThingAsAllyOfThing(a,b)assert((a==me and b==hero) or (a==hero and b==me))end\nfunction q:PauseAllNonScriptedEntities(v)paused=v end\nfunction r:NewResource()return handle end\nfunction r:PrepareResource(h)check(h);assert(not released)end\nfunction r:TryAcquire(h,actor,kind)check(h);assert(actor==me and kind==4);retries=retries+1;return retries>s.retries end\nfunction r:ScriptThing(h)check(h);return me end\nfunction r:ReleaseResource(h)check(h);assert(not released);released=true end\nfunction r:StartMovie(key)assert(key=='' and active==nil);active={};return active end\nfunction r:DestroyMovie(h)assert(h==active and active~=nil and not paused);active=nil end\nreturn q,me,function()\n assert(speeches==expectedSpeech,'LOST_SPEECH:'..speeches..'/'..expectedSpeech)\n if not s.bypass then assert(released,'RESOURCE_LEAK');assert(active==nil and not paused,'MOVIE_LEAK')end\n return speeches,frame,clears\nend\n"

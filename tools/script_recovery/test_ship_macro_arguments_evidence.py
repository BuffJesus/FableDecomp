"""SummoningTheShip outgoing macro string arguments evidence gates."""
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
 unit=json.loads((candidate if candidate.exists() else ROOT/'refs/script_recovery/summoning_the_ship/units/Q_SummoningTheShip.json').read_text(encoding='utf-8'))
 unit['entities']={};unit['quest']['functions']={k:v for k,v in unit['quest']['functions'].items() if int(v['address'],16) in (0xdf36e0,0xdf3e50)}
 c=UnitConverter(ROOT/'refs/script_recovery/summoning_the_ship/translation_unit_typed.json');out=tmp_path_factory.mktemp('ship_macro_arguments');result={};original=aliases.restore
 def restore(text,fn,row,rd):
  fixed=original(text,fn,row,rd)
  if fn['address'].lower()=='0x00df36e0':result.update(source=text,fixed=fixed,fn=fn,row=row,rd=rd)
  return fixed
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(aliases,'restore',restore):c.convert(unit,out)
 draft=(out/'FSE/SummoningTheShip/SummoningTheShip.lua').read_text(encoding='utf-8')
 result.update(draft=draft,readable=readable_file(draft,rel='FSE/SummoningTheShip/SummoningTheShip.lua')[0]);return result

@pytest.mark.parametrize('fault',['native','source','instruction','count','collision','missing_edit','duplicate_edit','missing_anchor','extra_use','invalid_count'])
def test_stale_or_ambiguous_evidence_rejected(captured,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=captured;row=copy.deepcopy(c['row']);fn=copy.deepcopy(c['fn']);source=c['source']
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='instruction':row['nativeInstructions'][next(iter(row['nativeInstructions']))]='00'
 elif fault=='count':row['editCounts'][1]+=1
 elif fault=='collision':row['alias']='stack0xffffffc4'
 elif fault=='missing_edit':row['edits'][0][0]='missing_constructor'
 elif fault=='duplicate_edit':row['edits'][1]=row['edits'][0]
 elif fault=='missing_anchor':row['nativeInstructions']={}
 elif fault=='extra_use':source+=' stack0xffffffc4;'
 else:row['editCounts'][0]=True
 with pytest.raises(ValueError):restore(source,fn,row,c['rd'])

def test_unreviewed_source_unchanged(captured):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=captured;assert restore(c['source'],c['fn'],None,c['rd'])==c['source']

def test_exact_resource_and_call_scope(captured):
 c=captured;text=c['source'];row=c['row'];assert row['editCounts']==[6,6]
 for (old,new),count in zip(row['edits'],row['editCounts']):
  assert text.count(old)==count;text=text.replace(old,new)
 assert text==c['fixed']
 for style in ['draft','readable']:
  assert captured[style].count('helper_DF3E50(quest,')==7
  assert '-- TODO(native):' not in captured[style]

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('route',range(3))
@pytest.mark.parametrize('briar',range(3))
@pytest.mark.parametrize('acquired',[False,True])
@pytest.mark.parametrize('result',[False,True])
def test_complete_mission_macro_sequence(captured,style,route,briar,acquired,result):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(captured[style])
 q,done=lua.execute(HARNESS,lua.table_from(dict(route=route,briar=briar,delay=2,acquired=acquired,macroResult=result,limit=30)))
 lua.globals().DoMission(q);count,frames,complete=done();assert count==4 and complete

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('limit',[0,1,3])
@pytest.mark.parametrize('delay',[0,2])
def test_interrupted_mission_closes_cutscene_resources(captured,style,limit,delay):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(captured[style])
 q,done=lua.execute(HARNESS,lua.table_from(dict(route=0,briar=2,delay=delay,acquired=True,macroResult=False,limit=limit)))
 lua.globals().DoMission(q);done()

HARNESS="\nlocal s=...;local q,r,hero={},{},{};local frame,stage,deadline=0,0,s.delay;local macros={};local resource,map,movie=nil,nil,nil;local paused,camera=false,false;local deactivated=false;local actors={}\nfor _,name in ipairs({'LightHouseDoor','HookCoastShip1','HookCoastShip2','HookCoastVillage','STS_BriarRose','LostBayHSP'})do actors[name]={name=name}end\nfunction actors.STS_BriarRose:IsAlive()return s.briar==2 end\nlocal expected={'CS_SUMSHIP_PREINTRO','CS_SUMSHIP_INTRO',s.route==1 and 'CS_SUMSHIP_OUTRO_FAILED' or 'CS_SUMSHIP_WAVE2',s.route==0 and 'CS_SUMSHIP_OUTRO' or 'CS_SUMSHIP_OUTRO_FAILED'}\nfunction q:GetHero()return hero end\nfunction q:RetailResources()return r end\nfunction q:GetActiveQuestName()return 'Q_SummoningTheShip' end\nfunction q:IsActiveThreadTerminating()return frame>=s.limit end\nfunction q:NewScriptFrame()frame=frame+1;assert(frame<40);return frame<s.limit end\nfunction q:IsLevelLoaded(name)assert(name=='HookCoast');return frame>=s.delay end\nfunction q:SetQuestCardObjective(name,key,a,b)assert(name=='Q_SummoningTheShip' and a=='HookCoast' and b=='HookCoast')end\nfunction q:SetSummonerDeathExplosionAffectsHero(v)end\nfunction q:SetWeaponOutCrimeEnabled(v)end\nfunction q:GetThingWithScriptName(name)assert(actors[name]);if name=='STS_BriarRose' and s.briar==0 then return nil end;return actors[name]end\nfunction q:RemoveThing(actor,a,b)assert(not a);if actor==nil then assert(s.briar==0 and b)end end\nfunction q:SetVillageLimbo(actor,v)assert(actor==actors.HookCoastVillage)end\nfunction q:KickOffQuestStartScreen(name,a,b)assert(name=='Q_SummoningTheShip' and a and not b)end\nfunction q:FadeScreenIn()end\nfunction q:GetStateBool(key)\n if key=='LighthouseStarted'then return frame>=deadline end\n assert(key=='MissionFailed');return s.route==1 or(s.route==2 and stage>=3)\nend\nfunction q:SetStateBool(key,v)assert(key=='SummonerAttacksStarted' and v)end\nfunction q:GetStateInt(key)\n if key=='CurrentAttackWave'then return frame>=deadline and 2 or 1 end\n assert(key=='SummonersAlive');return s.route==0 and frame>=deadline and 0 or 3\nend\nfunction q:SetStateInt(key,v)assert(key=='ConversationIndex' and v==77)end\nfunction q:OverrideMusic(id,a,b)assert(id==23 and not a and not b)end\nfunction q:StopOverrideMusic(v)assert(not v)end\nfunction q:SetQuestAsFailed(name,a,key,b)assert(name=='Q_SummoningTheShip' and a and b and key=='TEXT_QUEST_SUMMONING_THE_SHIP_FAILED')end\nfunction q:SetQuestAsCompleted(name,a,b,c)assert(name=='Q_SummoningTheShip' and a and not b and not c)end\nfunction q:AddNewConversation(actor,a,b)assert(actor==actors.STS_BriarRose and s.briar==2 and not a and not b);return 77 end\nfunction q:AddPersonToConversation(id,actor)assert(id==77 and actor==hero)end\nfunction q:AddLineToConversation(id,key,actor,target,v)assert(id==77 and key=='TEXT_QST_B02_BRIARROSE_FIREHEART_POWERED' and actor==actors.STS_BriarRose and target==hero and not v)end\nfunction q:Pause(seconds)assert(seconds==3);frame=frame+1 end\nfunction q:PlayAVIMovie(path)assert(path:find('Summoning_The_Ship.xmv',1,true))end\nfunction q:EntityTeleportToThing(actor,target,v)assert(actor==hero and target==actors.LostBayHSP and not v)end\nfunction q:DeactivateQuestLater(name,delay)assert(name=='Q_SummoningTheShip' and delay==0);deactivated=true end\nfunction q:PauseAllNonScriptedEntities(v)paused=v end\nfunction q:FixMovieSequenceCamera(v)camera=v end\nfunction r:NewResource()assert(resource==nil);resource={};return resource end\nfunction r:TryAcquire(h,actor,kind)assert(h==resource and actor==hero and kind==4);return s.acquired end\nfunction r:NewActorMap()assert(map==nil);map={};return map end\nfunction r:SetActor(m,key,h)assert(m==map and key=='HERO' and h==resource);map.HERO=h end\nfunction r:StartMovie(key)assert(key=='' and movie==nil);movie={};return movie end\nfunction r:RunMacro(name,m,a,b)\n assert(type(name)=='string' and name==expected[stage+1],'WRONG_MACRO:'..tostring(name)..'/'..tostring(expected[stage+1]))\n assert(m==map and map.HERO==resource and not a and b and paused and camera and movie)\n macros[#macros+1]=name;stage=stage+1;frame=frame+1;deadline=frame+s.delay;return s.macroResult\nend\nfunction r:DestroyMovie(h)assert(h==movie and not paused and not camera);movie=nil end\nfunction r:DestroyActorMap(m)assert(m==map);map=nil end\nfunction r:ReleaseResource(h)assert(h==resource);resource=nil end\nreturn q,function()\n assert(resource==nil and map==nil and movie==nil and not paused and not camera,'RESOURCE_LEAK')\n if deactivated then assert(stage==4)end\n if s.limit>=12 then assert(deactivated and stage==4,'INCOMPLETE_MISSION')end\n return stage,frame,deactivated\nend\n"

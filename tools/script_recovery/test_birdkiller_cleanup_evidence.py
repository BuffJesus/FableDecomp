"""BirdKiller timer and movie cleanup: guarded evidence and generated Main behavior."""
import json,copy
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())
HERE=Path(__file__).resolve().parent
HARNESS="\nlocal s=...;local me,hero,r={},{},{};local phase='initial';local frames=0;local released=false;local timerLive=false;local movieLive=false\nlocal quest,resources={},{}\nfunction quest:GetHero()return hero end\nfunction quest:RetailResources()return resources end\nfunction quest:IsActiveThreadTerminating()return phase==s.abort or frames>8 end\nfunction quest:NewScriptFrame(actor)assert(actor==me);frames=frames+1;assert(frames<12);if phase=='answered_pending' then phase='pending_frame' end;return not self:IsActiveThreadTerminating() end\nfunction quest:SetThingPersistent(actor,v)assert(actor==me and v==true)end\nfunction quest:PersistTransferInt(_,key,value)if key=='BirdMode' then return 2 end;assert(key=='CurrentBirds');return 0 end\nfunction quest:EntitySetAsKillable(actor,a,b)assert(actor==me and not a and b)end\nfunction quest:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and b and not c)end\nfunction quest:RegisterTimer()timerLive=true;return 77 end\nfunction quest:SetTimer(id,value)assert(id==77 and value==15)end\nfunction quest:GetTimer(id)assert(id==77);return 15 end\nfunction quest:DeregisterTimer(id)assert(id==77,'WRONG_TIMER_OPERAND:'..type(id));assert(timerLive);timerLive=false end\nfunction quest:IsDistanceBetweenThingsUnder(a,b,d)assert(a==hero and b==me and d==5.5);return false end\nfunction quest:PauseAllNonScriptedEntities(value)end\nfunction quest:GetHealth(actor)assert(actor==me);return 10 end\nfunction quest:GiveHeroYesNoQuestion()phase='question' end\nfunction quest:MsgIsQuestionAnsweredYesOrNo()phase=s.answer<0 and 'answered_pending' or 'answered';return s.answer end\nfunction me:SetFriendsWithEverythingFlag(v)assert(v==true)end\nfunction me:IsTalkedToByHero()return true end\nfunction me:Speak(actor,key,a,b,c,d)\n assert(actor==hero and a==0 and not b and c and not d)\n if key=='TEXT_QST_028_BIRD_KILLER_GREET' then phase='greet_spoke'\n elseif key=='TEXT_QST_028_BIRD_KILLER_REFUSE' then phase='refuse_spoke'\n else error(key) end\n return not (s.failSpeech==phase)\nend\nfunction me:IsPerformingScriptTask()if phase=='greet_spoke' then phase='greet_done' elseif phase=='refuse_spoke' then phase='refuse_done' end;return false end\nfunction resources:NewResource()return r end\nfunction resources:PrepareResource(handle)assert(handle==r)end\nfunction resources:TryAcquire(handle,actor,kind)assert(handle==r and actor==me and kind==4);return true end\nfunction resources:ScriptThing(handle)assert(handle==r);return me end\nfunction resources:ReleaseResource(handle)assert(handle==r and not released);released=true end\nlocal movie={}\nfunction resources:StartMovie(key)assert(key=='' and not movieLive);movieLive=true;return movie end\nfunction resources:DestroyMovie(handle)assert(handle==movie,'WRONG_MOVIE_OPERAND:'..tostring(handle));assert(movieLive);movieLive=false end\nreturn quest,me,function()assert(released and not timerLive and not movieLive)end\n"

@pytest.fixture(scope='module')
def captured(tmp_path_factory):
 from tools.script_recovery import native_reviewed_stack_aliases as aliases,native_reviewed_movie_members as movies
 from tools.script_recovery.convert_quest_unit import UnitConverter
 from tools.script_recovery.build_readable_unit import readable_file
 candidate=HERE/'candidate_unit.json'
 unit=json.loads((candidate if candidate.exists() else ROOT/'refs/script_recovery/guild_training/units/Q_GuildTraining.json').read_text())
 unit['entities']={'BirdKiller':unit['entities']['BirdKiller']}
 c=UnitConverter(ROOT/'refs/script_recovery/guild_training/translation_unit_typed.json');out=tmp_path_factory.mktemp('bird_cleanup');result={}
 old_alias,old_movie=aliases.restore,movies.restore
 def alias_hook(text,fn,row,rd):
  fixed=old_alias(text,fn,row,rd)
  if fn['address'].lower()=='0x00d4dea0':result.update(alias_source=text,alias_fixed=fixed,alias_row=row,fn=fn,rd=rd)
  return fixed
 def movie_hook(text,fn,row,rd):
  fixed=old_movie(text,fn,row,rd)
  if fn['address'].lower()=='0x00d4dea0':result.update(movie_source=text,movie_fixed=fixed,movie_row=row)
  return fixed
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(aliases,'restore',alias_hook),patch.object(movies,'restore',movie_hook):c.convert(unit,out)
 draft=(out/'FSE/GuildTraining/Entities/BirdKiller.lua').read_text(encoding='utf-8')
 result.update(draft=draft,readable=readable_file(draft,rel='FSE/GuildTraining/Entities/BirdKiller.lua')[0])
 return result

@pytest.mark.parametrize('kind',['alias','movie'])
@pytest.mark.parametrize('fault',['native','source','instruction','count'])
def test_stale_evidence_rejected(captured,kind,fault):
 from tools.script_recovery import native_reviewed_stack_aliases as aliases,native_reviewed_movie_members as movies
 c=captured;row=copy.deepcopy(c[kind+'_row']);fn=copy.deepcopy(c['fn'])
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='instruction':row['nativeInstructions'][next(iter(row['nativeInstructions']))]='00'
 elif kind=='alias':row['editCounts'][1]+=1
 else:row['stackReceiverAliases'][0]['count']+=1
 with pytest.raises(ValueError):(aliases if kind=='alias' else movies).restore(c[kind+'_source'],fn,row,c['rd'])

@pytest.mark.parametrize('fault',['alias_collision','missing_edit','duplicate_edit','missing_anchor','source_count'])
def test_timer_ambiguity_rejected(captured,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=captured;row=copy.deepcopy(c['alias_row']);source=c['alias_source']
 if fault=='alias_collision':row['alias']='timerId'
 elif fault=='missing_edit':row['edits'][1][0]='missing_cleanup'
 elif fault=='duplicate_edit':row['edits'][1]=row['edits'][0]
 elif fault=='missing_anchor':row['nativeInstructions']={}
 else:source+=row['edits'][1][0]
 with pytest.raises(ValueError):restore(source,c['fn'],row,c['rd'])

@pytest.mark.parametrize('fault',['frame','slot','constructor','lea','missing_lea','missing_receiver','receiver_count'])
def test_movie_identity_rejected(captured,fault):
 from tools.script_recovery.native_reviewed_movie_members import restore
 c=captured;row=copy.deepcopy(c['movie_row']);alias=row['stackReceiverAliases'][0];source=c['movie_source']
 if fault=='frame':alias['frameBytes']+=4
 elif fault=='slot':alias['entrySlot']+=4
 elif fault=='constructor':alias['constructorTarget']='0x006e7b80'
 elif fault=='lea':alias['receiverLeas'][0]='0x00D4E364'
 elif fault=='missing_lea':alias['receiverLeas']=alias['receiverLeas'][:1]
 elif fault=='missing_receiver':source=source.replace('(CScriptGameResourceObjectMovieBase *)xStack_7c','lost')
 else:alias['count']=0
 with pytest.raises(ValueError):restore(source,c['fn'],row,c['rd'])

def test_unreviewed_sources_unchanged(captured):
 from tools.script_recovery import native_reviewed_stack_aliases as aliases,native_reviewed_movie_members as movies
 for kind,module in [('alias',aliases),('movie',movies)]:
  c=captured;assert module.restore(c[kind+'_source'],c['fn'],None,c['rd'])==c[kind+'_source']

def test_only_reviewed_operands_and_timer_name_change(captured):
 c=captured;row=c['alias_row'];expected=c['alias_source']
 for old,new in row['edits']:expected=expected.replace(old,new)
 assert expected==c['alias_fixed']
 assert c['movie_fixed']==c['movie_source'].replace('(CScriptGameResourceObjectMovieBase *)xStack_7c','(CScriptGameResourceObjectMovieBase *)xStack_78')
 assert c['draft'].count('quest:DeregisterTimer(birdTimerId)')==5
 assert 'resources:DestroyMovie(xStack_7c)' not in c['draft']

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('failure,abort,answer',[
 ('greet_spoke','',0),('','pending_frame',-1),('refuse_spoke','',0),
 ('','answered',0),('','refuse_done',0),('','greet_done',0)])
def test_complete_generated_main_cleanup(captured,style,failure,abort,answer):
 lua=LuaRuntime(unpack_returned_tuples=True)
 q,me,verify=lua.execute(HARNESS,lua.table_from(dict(failSpeech=failure,abort=abort,answer=answer)))
 lua.execute(captured[style]);lua.globals().Init(q,me);lua.globals().OnPersist(q,me,None)
 lua.globals().Main(q,me);verify()

"""Expression Follow target identity and follow command evidence gates."""
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
 unit=json.loads((candidate if candidate.exists() else ROOT/'refs/script_recovery/expressions/units/Expression_Follow.json').read_text(encoding='utf-8'))
 c=UnitConverter(ROOT/'refs/script_recovery/expressions/translation_unit_typed.json');out=tmp_path_factory.mktemp('follow_target');result={};original=aliases.restore
 def restore(text,fn,row,rd):
  fixed=original(text,fn,row,rd)
  if fn['address'].lower()=='0x00eea240':result.update(source=text,fixed=fixed,fn=fn,row=row,rd=rd)
  return fixed
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(aliases,'restore',restore):c.convert(unit,out)
 draft=(out/'FSE/Expression_Follow/Expression_Follow.lua').read_text(encoding='utf-8')
 result.update(draft=draft,readable=readable_file(draft,rel='FSE/Expression_Follow/Expression_Follow.lua')[0]);return result

@pytest.mark.parametrize('fault',['native','source','instruction','count','collision','missing_edit','duplicate_edit','missing_anchor','extra_use','invalid_count'])
def test_stale_or_ambiguous_evidence_rejected(captured,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=captured;row=copy.deepcopy(c['row']);fn=copy.deepcopy(c['fn']);source=c['source']
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='instruction':row['nativeInstructions'][next(iter(row['nativeInstructions']))]='00'
 elif fault=='count':row['editCounts'][3]+=1
 elif fault=='collision':row['alias']='xStack_1c'
 elif fault=='missing_edit':row['edits'][0][0]='missing_constructor'
 elif fault=='duplicate_edit':row['edits'][1]=row['edits'][0]
 elif fault=='missing_anchor':row['nativeInstructions']={}
 elif fault=='extra_use':source+=' xStack_1c;'
 else:row['editCounts'][0]=True
 with pytest.raises(ValueError):restore(source,fn,row,c['rd'])

def test_unreviewed_source_unchanged(captured):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=captured;assert restore(c['source'],c['fn'],None,c['rd'])==c['source']

def test_exact_resource_and_call_scope(captured):
 c=captured;text=c['source'];row=c['row'];assert row['editCounts']==[5,1,1,1]
 for (old,new),count in zip(row['edits'],row['editCounts']):
  assert text.count(old)==count;text=text.replace(old,new)
 assert text==c['fixed']
 for style in ['draft','readable']:
  assert captured[style].count('resources:FollowThing(')==1
  assert '-- TODO(native):' not in captured[style]

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('state',range(4))
@pytest.mark.parametrize('acquired',[False,True])
@pytest.mark.parametrize('limit',[0,1,3,8])
def test_complete_follow_thread(captured,style,state,acquired,limit):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(captured[style])
 q,done=lua.execute(HARNESS,lua.table_from(dict(state=state,acquired=acquired,limit=limit)))
 lua.globals().FollowThread(q);done()

HARNESS="\nlocal s=...;local q,r,target,hero,handle={},{},{},{},{};local frames,created,released,follow,deactivate=0,0,0,0,0;local held=false\nfunction q:RetailResources()return r end\nfunction q:WithRetailResources(body)return body(r)end\nfunction r:TrackThingOutput(t)assert(t==nil or t==target);return t and 1 or 0 end\nfunction r:RetireThingOutput(id)assert(id==0 or id==1)end\nfunction q:GetHeroTargetedThing()if s.state==0 then return nil end;return target end\nfunction target:IsAlive()return s.state==2 end\nfunction q:IsActiveThreadTerminating()return frames>=s.limit end\nfunction q:NewScriptFrame()frames=frames+1;assert(frames<20);return frames<s.limit end\nfunction q:GetActiveQuestName()return 'Expression_Follow' end\nfunction q:DeactivateQuestLater(name,delay)assert(name=='Expression_Follow' and delay==0);deactivate=deactivate+1 end\nfunction q:GetHero()return hero end\nfunction r:NewResource()created=created+1;return handle end\nfunction r:TryAcquire(h,actor,kind)assert(h==handle and actor==target and kind==4);held=s.acquired;return held end\nfunction r:FollowThing(h,actor,distance,avoid)assert(h==handle and actor==hero and distance==1.0 and avoid==true);follow=follow+1 end\nfunction r:ReleaseResource(h)assert(h==handle);released=released+1 end\nreturn q,function()\n local active=s.state==2 and s.limit>0\n assert(follow==(active and 1 or 0),'WRONG_FOLLOW:'..follow)\n assert(deactivate==((s.state~=2 and s.limit>0) and 1 or 0),'WRONG_DEACTIVATE:'..deactivate)\n assert(created==(active and 1 or 0) and released==created,'RESOURCE_LEAK')\n assert(frames==(active and s.limit or 0),'WRONG_FRAMES')\n return follow,deactivate,frames\nend\n"

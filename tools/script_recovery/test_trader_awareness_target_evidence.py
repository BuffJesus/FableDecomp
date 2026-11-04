"""Trader Conflict target decision evidence gates; full cleanup is outside this test."""
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
 unit=json.loads((candidate if candidate.exists() else ROOT/'refs/script_recovery/trader_conflict/units/Q_TraderConflictGood.json').read_text(encoding='utf-8'))
 c=UnitConverter(ROOT/'refs/script_recovery/trader_conflict/translation_unit_typed.json');out=tmp_path_factory.mktemp('trader_awareness_target');result={};original=aliases.restore
 def restore(text,fn,row,rd):
  fixed=original(text,fn,row,rd)
  if fn['address'].lower()=='0x00dfd600':result.update(source=text,fixed=fixed,fn=fn,row=row,rd=rd)
  return fixed
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(aliases,'restore',restore):c.convert(unit,out)
 draft=(out/'FSE/TraderConflictGood/TraderConflictGood.lua').read_text(encoding='utf-8')
 result.update(draft=draft,readable=readable_file(draft,rel='FSE/TraderConflictGood/TraderConflictGood.lua')[0]);return result

@pytest.mark.parametrize('fault',['native','source','instruction','count','collision','missing_edit','duplicate_edit','missing_anchor','extra_use','invalid_count'])
def test_stale_or_ambiguous_evidence_rejected(captured,fault):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=captured;row=copy.deepcopy(c['row']);fn=copy.deepcopy(c['fn']);source=c['source']
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='instruction':row['nativeInstructions'][next(iter(row['nativeInstructions']))]='00'
 elif fault=='count':row['editCounts'][1]+=1
 elif fault=='collision':row['alias']='xStack_18'
 elif fault=='missing_edit':row['edits'][0][0]='missing_constructor'
 elif fault=='duplicate_edit':row['edits'][1]=row['edits'][0]
 elif fault=='missing_anchor':row['nativeInstructions']={}
 elif fault=='extra_use':source+=' xStack_18;'
 else:row['editCounts'][0]=True
 with pytest.raises(ValueError):restore(source,fn,row,c['rd'])

def test_unreviewed_source_unchanged(captured):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 c=captured;assert restore(c['source'],c['fn'],None,c['rd'])==c['source']

def test_exact_resource_and_call_scope(captured):
 c=captured;text=c['source'];row=c['row'];assert row['editCounts']==[12,1,1]
 for (old,new),count in zip(row['edits'],row['editCounts']):
  assert text.count(old)==count;text=text.replace(old,new)
 assert text==c['fixed']
 for style in ['draft','readable']:
  assert 'piStack_14 + 0x12c' not in captured[style]

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('state',range(4))
@pytest.mark.parametrize('distance',[False,True])
@pytest.mark.parametrize('aware',[False,True])
def test_first_target_decision(captured,style,state,distance,aware):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(captured[style]);lua.execute('function UpdateLiveEnemies(quest) end')
 q,done=lua.execute(HARNESS,lua.table_from(dict(state=state,distance=distance,aware=aware)))
 lua.execute("""return function(q)
 function q:WithRetailResources(callback)
  -- This fixture stops at selection/scan; ownership is tested separately.
  return callback({TrackThingOutput=function(_,t)return t and 1 or 0 end,
   RetireThingOutput=function()end,
   AssignThingOutput=function()error('following target must not be cleared')end})
 end
end""")(q)
 with pytest.raises(Exception,match='DECISION_REACHED'):lua.globals().AttackPeople(q)
 chosen,calls=done();assert chosen==('trader' if distance or aware and state==2 else 'hero')
 expected=['entry_distance','distance']+(['awareness'] if not distance else [])+(['alive'] if not distance and aware and state!=0 else [])
 assert calls==','.join(expected)

HARNESS="\nlocal s=...;local q,hero,bandit,trader={},{},{},{};local calls={};local target=s.state==0 and nil or trader\nif s.state==0 then target=nil end\nlocal chosen=nil\nfunction q:GetHero()return hero end\nfunction q:GetStateListCopy(name)assert(name=='AllCreatures');return {bandit} end\nfunction q:RegisterTimer()return 19 end\nfunction q:GetStateBool(name)assert(name=='EnteredNewRegion');return false end\nfunction q:GetStateInt(name)if name=='TradersReachedTeleporter' then return 0 end;assert(name=='TradersFollowing');return 1 end\nfunction q:NewScriptFrame()return true end\nfunction q:IsActiveThreadTerminating()return false end\nfunction q:GetNearestWithScriptName(actor,name)assert(actor==bandit and name=='TraderToRescue');return target end\nfunction q:IsEntityFollowingHero(actor)assert(actor==target);return true end\nfunction q:IsDistanceBetweenThingsUnder(actor,other,distance)\n assert(actor==bandit and distance==15.0)\n if other==hero then calls[#calls+1]='entry_distance';return true end\n assert(other==target);calls[#calls+1]='distance';return s.distance\nend\nfunction q:IsThingAwareOfOtherThingInAnyWay(actor,other)assert(actor==bandit and other==target);calls[#calls+1]='awareness';return s.aware end\nfunction trader:IsAlive()calls[#calls+1]='alive';return s.state==2 end\nfunction q:GiveThingBestEnemyTarget(actor,other)\n assert(actor==bandit);chosen=other==hero and 'hero' or other==target and 'trader' or 'wrong';error('DECISION_REACHED')\nend\nreturn q,function()return chosen,table.concat(calls,',')end\n"

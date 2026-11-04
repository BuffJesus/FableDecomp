"""Keep mutations and reused locals explicit when retiring compiler ghosts."""
from copy import deepcopy
import hashlib,json,re
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_argument_temporaries import restore
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json')
 u=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
 text=(R/'tools/script_recovery/fixtures/argument_temporaries/madame_input.lua').read_text()
 source=re.search(r'^function Main\([^\n]*\n.*?^end$',text,re.M|re.S)[0]
 return source,c.native('0x00e3bb70'),u['reviewedArgumentTemporaries']['0x00e3bb70'],c.rdata
def todo(row):return [d for e in row['edits'] for d in e['diagnostics']]
def test_only_scoped_ghosts_are_removed(evidence):
 source,fn,row,rd=evidence;items=['other']+todo(row);out,left,review=restore(source,items,fn,row,rd)
 assert left==['other'] and len(items)==5 and review[0]['removedDiagnostics']==4
 assert out==source.replace(row['edits'][0]['source'],'',1).replace(row['edits'][1]['source'],'',1)
 assert out[out.index(row['scopeEnd']):]==source[source.index(row['scopeEnd']):]
 assert restore(source,items,fn,None,rd)==(source,items,[])
@pytest.mark.parametrize('field',['nativeSha256','sourceSha256','luaScopeSha256','spanBytes'])
def test_stale_evidence_fails(evidence,field):
 source,fn,row,rd=evidence;row=deepcopy(row);row[field]='0'*64
 with pytest.raises(ValueError,match='reviewed argument temporaries'):restore(source,todo(row),fn,row,rd)
def test_earlier_alias_assignment_cannot_be_removed(evidence):
 source,fn,row,rd=evidence;source=source.replace(row['scopeBegin'],'    xStack_174 = me\n'+row['scopeBegin'],1)
 with pytest.raises(ValueError,match='earlier use or write'):restore(source,todo(row),fn,row,rd)
def test_call_or_flag_change_requires_review(evidence):
 source,fn,row,rd=evidence;source=source.replace('quest:EntitySetAsKillable(me, false, false)','quest:EntitySetAsKillable(me, true, false)',1)
 with pytest.raises(ValueError,match='scope changed'):restore(source,todo(row),fn,row,rd)
def test_native_slot_change_fails(evidence):
 source,fn,row,rd=evidence;row=deepcopy(row);row['calls'][0]['slot']='0x838'
 with pytest.raises(ValueError,match='call metadata changed'):restore(source,todo(row),fn,row,rd)
def test_missing_diagnostic_fails(evidence):
 source,fn,row,rd=evidence
 with pytest.raises(ValueError,match='diagnostic inventory changed'):restore(source,[],fn,row,rd)

def test_actual_lua_prefix_calls_and_early_exits_are_preserved(evidence):
 from itertools import product
 from lupa.lua54 import LuaRuntime
 source,fn,row,rd=evidence;updated=restore(source,todo(row),fn,row,rd)[0]
 def execute(text,nunnery,first,second):
  lua=LuaRuntime();g=lua.globals();g.nunnery=nunnery;g.first=first;g.second=second
  lua.execute('''
   events={};local function event(s) events[#events+1]=s end
   me={};local queries=0
   resources={NewResource=function() event('resource');error('PREFIX_END') end}
   quest={
    RetailResources=function() return resources end,
    NewScriptFrame=function(_,actor) assert(actor==me);event('frame');return true end,
    IsActiveThreadTerminating=function() queries=queries+1;event('term');if queries==1 then return first else return second end end,
    GetStateBool=function(_,name) assert(name=='BecomeNunnery');event('state');return nunnery end,
    SetThingHasInformation=function(_,actor,a,b,c) assert(actor==me and a and not b and not c);event('information') end,
    SetIsPushableByHero=function(_,actor,a) assert(actor==me and not a);event('pushable') end,
    EntitySetAsKillable=function(_,actor,a,b) assert(actor==me and not a and not b);event('killable') end,
    EntitySetAsToAddToComboMultiplierWhenHit=function(_,actor,a) assert(actor==me and not a);event('combo') end
   }
  ''')
  lua.execute(text)
  ok,message=lua.eval('function() local ok,err=pcall(Main,quest,me);return ok,tostring(err) end')()
  assert ok or 'PREFIX_END' in message
  return list(g.events.values()),bool(ok)
 for flags in product([False,True],repeat=3):assert execute(source,*flags)==execute(updated,*flags)

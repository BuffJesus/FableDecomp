"""Dead private reads require a pure helper contract and no live target use."""
from copy import deepcopy
import json,re
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter,ENTITY_STATE
from tools.script_recovery.native_reviewed_argument_temporaries import restore
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');unit=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
 source=(R/'tools/script_recovery/fixtures/argument_temporaries/lady_input.lua').read_text();source=re.search(r'^function Main\([^\n]*\n.*?^end$',source,re.M|re.S)[0]
 row=unit['reviewedArgumentTemporaries']['0x00e3eb10'];todo=[d for edit in row['edits'] for d in edit['diagnostics']]
 return source,c.native('0x00e3eb10'),row,c.rdata,todo
def apply(evidence,**kwargs):
 source,fn,row,rd,todo=evidence;return restore(source,todo,fn,row,rd,private_state_factory=kwargs.get('factory',ENTITY_STATE))
def test_scoped_dead_reads_and_ghosts_only(evidence):
 source,fn,row,rd,todo=evidence;out,left,review=apply(evidence)
 expected=source
 for edit in row['edits']:expected=expected.replace(edit['source'],'',1)
 assert out==expected and left==[] and len(todo)==6 and review[0]['removedDiagnostics']==6
 assert 'xStack_1a0 = piVar1' in out and 'resources:ReleaseResource(ladyResource)' in out
 assert '-- TODO(native): xStack_15c = xStack_15c & 0xfffffffe;' in out
@pytest.mark.parametrize('factory',[None,'changed'])
def test_private_getter_contract_is_required(evidence,factory):
 with pytest.raises(ValueError,match='getter contract changed'):apply(evidence,factory=factory)
def test_later_read_blocks_elimination(evidence):
 source,fn,row,rd,todo=evidence;source=source.replace(row['scopeEnd'],row['scopeEnd']+'        quest:Observe(xStack_15c)\n',1)
 with pytest.raises(ValueError,match='value is live'):apply((source,fn,row,rd,todo))
def test_overwrite_read_of_old_value_is_rejected(evidence):
 source,fn,row,rd,todo=evidence;source=source.replace('xStack_1a0 = piVar1','xStack_1a0 = xStack_1a0',1)
 with pytest.raises(ValueError,match='read by overwrite'):apply((source,fn,row,rd,todo))
def test_conditional_write_does_not_hide_a_later_read(evidence):
 source,fn,row,rd,todo=evidence;source=source.replace('xStack_1a0 = piVar1','xStack_1a0 = piVar1\n            quest:Observe(xStack_1a0)',1)
 with pytest.raises(ValueError,match='value is live'):apply((source,fn,row,rd,todo))
@pytest.mark.parametrize('field',['nativeSha256','sourceSha256','luaScopeSha256','spanBytes'])
def test_stale_evidence_fails(evidence,field):
 source,fn,row,rd,todo=evidence;row=deepcopy(row);row[field]='0'*64
 with pytest.raises(ValueError,match='reviewed argument temporaries'):apply((source,fn,row,rd,todo))
def test_diagnostic_inventory_does_not_swallow_extra_same_field(evidence):
 source,fn,row,rd,todo=evidence
 with pytest.raises(ValueError,match='diagnostic inventory'):apply((source,fn,row,rd,todo+['unnamed field 0xc']))
def test_rebound_private_accessor_is_not_assumed_pure(evidence):
 source,fn,row,rd,todo=evidence;source=source.replace(row['scopeBegin'],'        __native_entity_state.GetStateInt = evil\n'+row['scopeBegin'],1)
 with pytest.raises(ValueError,match='getter is rebound'):apply((source,fn,row,rd,todo))
def test_quoted_comment_marker_cannot_hide_a_live_read(evidence):
 source,fn,row,rd,todo=evidence;source=source.replace(row['scopeEnd'],row['scopeEnd']+'        local message="--";quest:Observe(xStack_15c)\n',1)
 with pytest.raises(ValueError,match='value is live'):apply((source,fn,row,rd,todo))
def test_comment_and_string_mentions_are_not_value_reads(evidence):
 source,fn,row,rd,todo=evidence;source=source.replace(row['scopeEnd'],row['scopeEnd']+'        -- xStack_15c\n        local message="xStack_15c"\n',1)
 assert apply((source,fn,row,rd,todo))[1]==[]
def test_actual_lua_prefix_preserves_calls_for_private_values(evidence):
 from itertools import product
 from lupa.lua54 import LuaRuntime
 source=evidence[0];updated=apply(evidence)[0]
 def execute(text,stop,seed,variant):
  lua=LuaRuntime();g=lua.globals();g.stop=stop;g.variant=variant
  lua.execute('''
   events={};local function event(s) events[#events+1]=s end
   me={GetHomePos=function() event('home');return {x=variant,y=-2,z=3} end}
   resources={NewResource=function() event('resource');return 11 end}
   quest={RetailResources=function() return resources end,
    NewScriptFrame=function(_,actor) assert(actor==me);event('frame');return true end,
    IsActiveThreadTerminating=function() event('term');return stop end,
    SetCreatureBrain=function(_,actor,name) assert(actor==me and name=='BRAIN_PASSIVE_OVERRIDE');event('brain') end,
    SetWanderCentrePoint=function(_,actor,pos) assert(actor==me and pos.x==variant and pos.y==-2 and pos.z==3);event('centre') end,
    SetWanderMinDistance=function(_,actor,value) assert(actor==me and value==0);event('min') end,
    SetWanderMaxDistance=function(_,actor,value) assert(actor==me and value==16);event('max') end,
    SetScriptingStateGroup=function(_,actor,value) assert(actor==me and value==4);event('group');error('PREFIX_END') end}
  ''')
  lua.execute(ENTITY_STATE+'\n'+text+'\nfunction SeedPrivate(value) __native_entity_state:SetStateInt("self_0xc",value) end')
  g.SeedPrivate(seed)
  ok,message=lua.eval('function() local ok,err=pcall(Main,quest,me);return ok,tostring(err) end')()
  assert ok or 'PREFIX_END' in message
  return list(g.events.values()),bool(ok)
 for values in product([False,True],[None,0,42],[1,16]):assert execute(source,*values)==execute(updated,*values)

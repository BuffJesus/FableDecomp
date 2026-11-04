"""Native CString slots must agree before replacing an emitted speech alias."""
from copy import deepcopy
import hashlib,json,re
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_string_operands import restore
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');fn=c.native('0x00e44ea0')
 row=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedStringOperands']['0x00e44ea0']
 return (R/'tools/script_recovery/fixtures/string_operands/client_input.lua').read_text(),fn,row,c.rdata
def test_only_three_reviewed_aliases_change(evidence):
 source,fn,row,rd=evidence;out,proof=restore(source,fn,row,rd);expected=source
 for edit in row['edits']:expected=expected.replace(edit['luaScope'],edit['luaScope'].replace(edit['luaBefore'],'pvVar11 = '+edit['resultLocal'],1),1)
 assert out==expected and len(proof)==3 and source.count(':Speak(')==out.count(':Speak(')==9
 assert [p['resultLocal'] for p in proof]==['stack0xfffffee0','xStack_e4','xStack_e0']
 assert restore(source,fn,None,rd)==(source,[])
@pytest.mark.parametrize('fault',['nativeSha256','sourceSha256','luaBodySha256','helperAddress','helperBytes','accessorBytes','speakBytes','witness','producerMissing','producerOrder','consumerMissing','consumerOrder','frameOffset','indexField','lineField','prefix','suffix','resultLocal','luaScope','luaScopeSha256','luaBefore','speechOrder','duplicateSite','missingSite','emptyEdits'])
def test_changed_evidence_fails_closed(evidence,fault):
 source,fn,row,rd=evidence;row=deepcopy(row);edit=row['edits'][0]
 if fault.endswith('Sha256') and fault in row:row[fault]='0'*64
 elif fault=='helperAddress':row['helpers']['integer']['address']='0x99f831'
 elif fault=='helperBytes':row['helpers']['integer']['bytes']='00'*20
 elif fault=='accessorBytes':row['helpers']['accessor']['bytes']='00'*15
 elif fault=='speakBytes':row['helpers']['speak']['bytes']='00'*14
 elif fault=='witness':row['nativeInstructions'][edit['producer'][0]]='00'
 elif fault in ['producerMissing','consumerMissing']:edit[fault.replace('Missing','')].pop(1)
 elif fault in ['producerOrder','consumerOrder']:edit[fault.replace('Order','')].reverse()
 elif fault in ['frameOffset','indexField','lineField']:edit[fault]+=4
 elif fault in ['prefix','suffix','resultLocal','luaScope','luaBefore']:edit[fault]='wrong'
 elif fault=='luaScopeSha256':edit[fault]='0'*64
 elif fault=='speechOrder':row['speechOrder'].reverse()
 elif fault=='duplicateSite':row['edits'].append(deepcopy(edit))
 elif fault=='missingSite':edit['site']='0xe44ea0'
 elif fault=='emptyEdits':row['edits']=[]
 with pytest.raises(ValueError):restore(source,fn,row,rd)
def test_recovered_lua_passes_constructed_strings_in_adverse_prior_state(evidence):
 source,fn,row,rd=evidence;out,_=restore(source,fn,row,rd);lua=LuaRuntime();cases=0
 # Execute the actual emitted construction and consumer slices. Prior string
 # slots contain distinct stale values, exposing the two non-nil wrong aliases.
 for edit in row['edits']:
  scope=edit['luaScope'];a=source.index(scope);prefix=source[:a]
  begin=prefix.rfind('pCVar7 = tostring(')
  snippet=source[begin:a]+scope
  construction=snippet[:snippet.index(edit['resultLocal']+' = ')]+edit['resultLocal']+' = (pCVar9 .. pCVar7)\n'
  before=edit['luaBefore'];after='pvVar11 = '+edit['resultLocal']
  for client in [-1,0,1,7]:
   for line in [-2,0,1,2,3,99]:
    bindings={'x_stk_108':'stale-first','xStack_e8_2':'stale-leave','xStack_e4':'stale-chat'}
    bindings.setdefault(edit['resultLocal'],'stale-constructed')
    setup='local pCVar7,pcVar20,pCVar9,pvVar11; '+''.join('local '+name+' = '+json.dumps(value)+'; ' for name,value in bindings.items())
    state=lua.table_from({'GetStateInt':lambda _,name:client if name=='ClientID' else line});lua.globals()['__native_entity_state']=state
    actual=lua.execute(setup+construction+after+'\nreturn pvVar11')
    old=lua.execute(setup+construction+before+'\nreturn pvVar11')
    expected=edit['prefix']+str(client+1)+edit['suffix']+str(line)
    assert actual==expected and old!=expected;cases+=1
 assert cases==72

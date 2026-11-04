"""Regression for Lua truthiness applied to a native numeric byte predicate."""
from copy import deepcopy
import hashlib,json
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_numeric_conditions import restore
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');fn=c.native('0x00e44ea0')
 row=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedNumericConditions']['0x00e44ea0']
 source=(R/'tools/script_recovery/fixtures/numeric_conditions/client_input.lua').read_text()
 return source,fn,row,c.rdata
def test_single_zero_test_only(evidence):
 source,fn,row,rd=evidence;after,proof=restore(source,fn,row,rd);scope=row['edits'][0]['luaScope']
 assert after==source.replace(scope,scope.replace('if not cVar5 then','if cVar5 == 0 then'),1)
 assert proof[0]['nativeBranch']=='0xe45027' and proof[0]['changedConditions']==1
 assert restore(source,fn,None,rd)==(source,[])
@pytest.mark.parametrize('definition',[None,'','CREATURE_BS_VILLAGER_FEMALE','CREATURE_BS_VILLAGER_MALE','creature_bs_villager_female','CREATURE_BS_VILLAGER_FEMAL','CREATURE_BS_VILLAGER_FEMALE_EXTRA','X','A'*100])
def test_actual_lua_predicate_matches_verified_native_branch(evidence,definition):
 source,fn,row,rd=evidence;after,_=restore(source,fn,row,rd);scope=row['edits'][0]['luaScope'];fixed=scope.replace('if not cVar5 then','if cVar5 == 0 then')
 lua=LuaRuntime();compile=lambda s:lua.eval('function(pCVar7)local iVar12,cVar5\n'+s+'\nreturn "nonfemale" else return "female" end end')
 old,new=compile(scope),compile(fixed);expected='female' if definition=='CREATURE_BS_VILLAGER_FEMALE' else 'nonfemale'
 assert new(definition)==expected
 assert old(definition)=='female'
@pytest.mark.parametrize('fault',['nativeSha256','sourceSha256','comparisonBytes','comparisonTarget','comparisonSite','luaScopeSha256','normalization','test','branch','zeroTarget','local','duplicate','witness','metadata'])
def test_changed_evidence_fails_closed(evidence,fault):
 source,fn,row,rd=evidence;row=deepcopy(row);fn=deepcopy(fn);edit=row['edits'][0]
 if fault in ['nativeSha256','sourceSha256']:row[fault]='0'*64
 elif fault=='comparisonBytes':row['comparison']['bytes']='00'*128
 elif fault=='comparisonTarget':row['comparison']['target']='0x411571'
 elif fault=='comparisonSite':row['comparison']['site']='0xe45020'
 elif fault=='luaScopeSha256':edit[fault]='0'*64
 elif fault=='normalization':edit[fault][0]='0xe45016'
 elif fault=='test':edit[fault]='0xe45018'
 elif fault=='branch':edit[fault]='0xe45025'
 elif fault=='zeroTarget':edit[fault]='0xe4502d'
 elif fault=='local':edit[fault]='other'
 elif fault=='duplicate':row['edits'].append(deepcopy(edit))
 elif fault=='witness':row['nativeInstructions'].pop('0xe45027')
 elif fault=='metadata':fn['calls']=[c for c in fn['calls'] if int(c['site'],16)!=0xe4500f]
 with pytest.raises(ValueError,match='reviewed numeric condition'):restore(source,fn,row,rd)
def test_repeated_scope_is_ambiguous(evidence):
 source,fn,row,rd=evidence
 with pytest.raises(ValueError,match='scope changed'):restore(source+'\n'+row['edits'][0]['luaScope'],fn,row,rd)
def test_numeric_local_must_have_a_declaration(evidence):
 source,fn,row,rd=evidence;source=source.replace(', cVar5,',',',1)
 with pytest.raises(ValueError,match='unshadowed local'):restore(source,fn,row,rd)
@pytest.mark.parametrize('constant',['2','false'])
def test_non_numeric_boolean_definition_is_rejected(evidence,constant):
 source,fn,row,rd=evidence;row=deepcopy(row);edit=row['edits'][0];old=edit['luaScope'];new=old.replace('and 1 or 0','and '+constant+' or 0')
 source=source.replace(old,new,1);edit['luaScope']=new;edit['luaScopeSha256']=hashlib.sha256(new.encode()).hexdigest()
 with pytest.raises(ValueError,match='numeric0/1 definition'):restore(source,fn,row,rd)
@pytest.mark.parametrize('site,encoded,message',[(0xe45016,'f7d3','normalization'),(0xe45025,'84c0','zero test'),(0xe45027,'0f85ba060000','zero branch')])
def test_decoded_semantics_checked_after_rehash(evidence,site,encoded,message):
 source,fn,row,rd=evidence;row=deepcopy(row);start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=bytearray(rd.bytes_at(start,end-start));replacement=bytes.fromhex(encoded)
 raw[site-start:site-start+len(replacement)]=replacement;row['nativeSha256']=hashlib.sha256(raw).hexdigest();row['nativeInstructions'][hex(site)]=encoded
 class Changed:
  def bytes_at(self,a,n):return bytes(raw) if (a,n)==(start,end-start) else rd.bytes_at(a,n)
 with pytest.raises(ValueError,match=message):restore(source,fn,row,Changed())

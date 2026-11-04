from pathlib import Path
import importlib.util,json,copy,sys,hashlib
import pytest
W=Path(__file__).resolve().parent/'fixtures/chest_key_result';R=next(p for p in W.parents if (p/'tools/script_recovery/script_units.py').exists());sys.path.insert(0,str(R))
spec=importlib.util.spec_from_file_location('candidate_key',R/'tools/script_recovery/native_chest_key_result.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
from tools.script_recovery.lift_native_lua import RData
SOURCE=(W/'pre_lift.c').read_text();FN=json.loads((W/'function.json').read_text());ROW=json.loads((W/'reviewed_row.json').read_text())
def test_reviewed_return_shape_and_exact_binding_source():
 result=m.restore(SOURCE,FN,RData(),ROW)
 assert result!=SOURCE and result.count('ENGINE_ChestKeyResult(')==1
 assert result.count('ENGINE_InventoryCountWithKeyName(xStack_48)')==1
 assert 'CCharString::CCharString(&xStack_48)'not in result
 assert hashlib.sha256((W/'sdk_key_method.cpp').read_text().encode()).hexdigest()==m.METHOD_SHA256
@pytest.mark.parametrize('field',['schema','address','inputSha256','sourceSha256','nativeSha256','sdkMethodSha256','sdkBindingSha256','count','target','key','result','callSite','inventorySite','frameDepth','keySlot','targetSlot'])
def test_changed_metadata_preserves_unresolved_source(field):
 row=copy.deepcopy(ROW);row[field]=row[field]+1 if isinstance(row[field],int)else'wrong'
 assert m.restore(SOURCE,FN,RData(),row)==SOURCE
@pytest.mark.parametrize('field',['slot','depth','pushedStack'])
@pytest.mark.parametrize('site',['0x00EEC986','0x00EEC9A7'])
def test_changed_native_call_evidence_preserves_source(field,site):
 fn=copy.deepcopy(FN);call=next(c for c in fn['indirectCalls']if c['site']==site);call[field]=0 if field!='pushedStack'else[]
 assert m.restore(SOURCE,fn,RData(),ROW)==SOURCE
def test_extra_key_string_use_not_silently_retired():
 source=SOURCE+'\nUseKey(xStack_48);';row=copy.deepcopy(ROW);row['inputSha256']=hashlib.sha256(source.encode()).hexdigest()
 assert m.restore(source,FN,RData(),row)==source
@pytest.mark.parametrize('offset',[8,13,19])
def test_actual_native_operand_mutation_rejected_even_with_updated_body_hash(offset):
 rd=RData()
 class Changed:
  def bytes_at(self,a,n):
   raw=rd.bytes_at(a,n);address=int(ROW['callSite'],16)-15+offset
   if a<=address<a+n:
    raw=bytearray(raw);raw[address-a]^=1;raw=bytes(raw)
   return raw
 changed=Changed();row=copy.deepcopy(ROW);row['nativeSha256']=hashlib.sha256(changed.bytes_at(0xeec890,1544)).hexdigest()
 row['anchors']={a:changed.bytes_at(int(a,16),len(bytes.fromhex(v))).hex()for a,v in row['anchors'].items()}
 assert m.restore(SOURCE,FN,changed,row)==SOURCE

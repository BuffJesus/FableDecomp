"""Retire a diagnostic only while its restored operation and native argument agree."""
from copy import deepcopy
import hashlib,json
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_restored_call_diagnostics import classify
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');fn=c.native('0x00e3eb10')
 row=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedRestoredCallDiagnostics']['0x00e3eb10']
 source=(R/'tools/script_recovery/fixtures/restored_calls/lady_input.lua').read_text()
 return source,[row['calls'][0]['diagnostic'],'unlifted: retained pointer operand'],fn,row,c.rdata
def test_only_report_changes(evidence):
 source,todo,fn,row,rd=evidence;out,left,proof=classify(source,todo,fn,row,rd)
 assert out==source and left==[todo[1]] and todo==[row['calls'][0]['diagnostic'],todo[1]]
 assert proof[0]['nativeSite']=='0xe40074' and proof[0]['argument'] is False
 assert classify(source,todo,fn,None,rd)==(source,todo,[])
@pytest.mark.parametrize('fault',['nativeSha256','sourceSha256','luaBodySha256','slot','target','method','calleeBytes','savedReceiverOffset','callsite','region','diagnostic','duplicate','witness'])
def test_changed_review_fails_closed(evidence,fault):
 source,todo,fn,row,rd=evidence;row=deepcopy(row)
 if fault.endswith('Sha256'):row[fault]='0'*64
 elif fault=='slot':row[fault]='0x5e8'
 elif fault=='target':row[fault]='0x890ab1'
 elif fault=='method':row[fault]='FixMovieSequenceCamera'
 elif fault=='calleeBytes':row[fault]='00'*80
 elif fault=='savedReceiverOffset':row[fault]+=4
 elif fault=='callsite':row['calls'][0]['site']='0xe3eef0'
 elif fault=='region':row['calls'][0]['luaRegion']='unrelated'
 elif fault=='diagnostic':row['calls'][0]['diagnostic']='unlifted: unrelated'
 elif fault=='duplicate':row['calls'].append(deepcopy(row['calls'][0]))
 elif fault=='witness':row['nativeInstructions'].pop('0xe40072')
 with pytest.raises(ValueError,match='reviewed restored call diagnostics'):classify(source,todo,fn,row,rd)
def test_duplicate_diagnostic_is_not_silently_dropped(evidence):
 source,todo,fn,row,rd=evidence
 with pytest.raises(ValueError,match='multiplicity'):classify(source,todo+[todo[0]],fn,row,rd)
def test_restored_true_call_cannot_be_classified_as_false(evidence):
 source,todo,fn,row,rd=evidence;row=deepcopy(row)
 old=row['calls'][0]['luaRegion'];source=source.replace(old,old.replace('(false)','(true)'),1)
 row['luaBodySha256']=hashlib.sha256(source.encode()).hexdigest()
 with pytest.raises(ValueError,match='restored call region'):classify(source,todo,fn,row,rd)
@pytest.mark.parametrize('site,encoded,message',[(0xe40072,'6a01','false argument'),(0xe4006c,'8b4c2418','saved receiver')])
def test_decoded_operands_checked_even_with_updated_hashes(evidence,site,encoded,message):
 source,todo,fn,row,rd=evidence;row=deepcopy(row);start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
 raw=bytearray(rd.bytes_at(start,end-start));replacement=bytes.fromhex(encoded);raw[site-start:site-start+len(replacement)]=replacement
 row['nativeSha256']=hashlib.sha256(raw).hexdigest();row['nativeInstructions'][hex(site)]=encoded
 class Changed:
  def bytes_at(self,a,n):return bytes(raw) if (a,n)==(start,end-start) else rd.bytes_at(a,n)
 with pytest.raises(ValueError,match=message):classify(source,todo,fn,row,Changed())

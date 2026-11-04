"""Only witnessed by-value actor comments may change; reused aliases stay live."""
from copy import deepcopy
import hashlib,json
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_restored_call_diagnostics import classify_argument_copy
from tools.script_recovery.readable_lua import tokens
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/sick_child/translation_unit_typed.json');fn=c.native('0x00ecf7f0');row=json.loads((R/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())['reviewedArgumentCopyComments']['0x00ecf7f0'];source=(R/'tools/script_recovery/fixtures/argument_copies/woman_main.lua').read_text()
 return source,[*row['diagnostics'],'label retained'],fn,row,c.rdata
def test_classifies_comments_preserving_reused_alias_and_empty_branch(evidence):
 source,todo,fn,row,rd=evidence;out,remaining,proof=classify_argument_copy(source,todo,fn,row,rd)
 expected=source
 for d in row['diagnostics']:expected='\n'.join(line for line in expected.split('\n') if line.strip()!='-- TODO(native): '+d[10:])
 assert out==expected and remaining==['label retained'] and len(todo)==4
 significant=lambda s:[(t.lastgroup,t[0]) for t in tokens(s) if t.lastgroup not in ['space','comment','longcomment']]
 assert significant(out)==significant(source) and 'if xStack_c8 ~= nil then\n    end' in out and 'xStack_c8 = this_00' in out
 assert len(proof)==1 and proof[0]['executableLuaUnchanged'] and proof[0]['removedDiagnostics']==3
 assert classify_argument_copy(source,todo,fn,None,rd)==(source,todo,[])
@pytest.mark.parametrize('fault',['nativeSha256','sourceSha256','luaBodySha256','spanBegin','spanEnd','spanBytes','slot','method','target','calleeBytes','baseDestructorBytes','callSite','actorMemberOffset','scriptReceiverOffset','luaRegion','luaCall','diagnostics','duplicateDiagnostics','missingTodo','extraTodo','nativeSourceFragments','acceptedSdkEvidenceSha256','scopeLimit','comment','metadata','receiverMetadata','retainOpcode','falseArgument','copyWidth','branchTarget','calleeReturn','calleeRelease'])
def test_changed_or_unproved_contract_rejected(evidence,fault):
 source,todo,fn,row,rd=evidence;row=deepcopy(row);todo=list(todo)
 if fault.endswith('Sha256'):row[fault]='invalid'
 elif fault in ['spanBegin','spanEnd','callSite']:row[fault]=hex(int(row[fault],16)+1)
 elif fault in ['spanBytes','calleeBytes','baseDestructorBytes']:row[fault]='00'*40
 elif fault in ['slot','actorMemberOffset','scriptReceiverOffset']:row[fault]+=4
 elif fault=='target':row[fault]='0x008a6dd4'
 elif fault in ['method','luaRegion','luaCall','scopeLimit']:row[fault]='invalid'
 elif fault=='diagnostics':row['diagnostics'][0]='unlifted: other();'
 elif fault=='duplicateDiagnostics':row['diagnostics'][1]=row['diagnostics'][0]
 elif fault=='nativeSourceFragments':row['nativeSourceFragments'][0]='other();'
 elif fault=='missingTodo':todo.pop(0)
 elif fault=='extraTodo':todo.append(todo[0])
 elif fault=='comment':source=source.replace('-- TODO(native): '+row['diagnostics'][0][10:],'-- other',1);row['luaBodySha256']=hashlib.sha256(source.encode()).hexdigest();row['luaRegion']=row['luaRegion'].replace('-- TODO(native): '+row['diagnostics'][0][10:],'-- other')
 elif fault in ['metadata','receiverMetadata']:
  fn=deepcopy(fn);call=next(c for c in fn['indirectCalls'] if int(c['site'],16)==int(row['callSite'],16));call['slot' if fault=='metadata' else 'ecxValue']='0xd34' if fault=='metadata' else 8
 else:
  # Self-consistent changed bytes/hashes still require the semantic contract.
  if fault in ['retainOpcode','falseArgument','copyWidth','branchTarget']:
   old=bytes.fromhex(row['spanBytes']);new={'retainOpcode':old.replace(b'\xff\x01',b'\xff\x09'),'falseArgument':b'\x6a\x01'+old[2:],'copyWidth':old.replace(b'\x83\xec\x0c',b'\x83\xec\x08'),'branchTarget':old.replace(b'\x74\x02',b'\x74\x03')}[fault];address=int(row['spanBegin'],16);row['spanBytes']=new.hex()
  else:
   old=bytes.fromhex(row['calleeBytes']);new=old[:-2]+b'\x0c\x00' if fault=='calleeReturn' else old.replace(b'\xff\x08',b'\xff\x00');address=int(row['target'],16);row['calleeBytes']=new.hex()
  assert old!=new
  original=rd
  class Changed:
   def bytes_at(self,a,n):
    value=original.bytes_at(a,n)
    if a<=address and address+len(old)<=a+n:
     k=address-a;return value[:k]+new+value[k+len(old):]
    return value
  rd=Changed()
  if address<int(fn['bodyEndExclusive'],16) and address>=int(fn['address'],16):row['nativeSha256']=hashlib.sha256(rd.bytes_at(int(fn['address'],16),int(fn['bodyEndExclusive'],16)-int(fn['address'],16))).hexdigest()
 with pytest.raises(ValueError):classify_argument_copy(source,todo,fn,row,rd)

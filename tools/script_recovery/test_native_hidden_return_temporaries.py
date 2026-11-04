"""Hidden CString return setup requires receiver, frame, assignment and cleanup proof."""
from copy import deepcopy
import hashlib,json
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_restored_call_diagnostics import classify_hidden_result
from tools.script_recovery.readable_lua import tokens
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');fn=c.native('0x00e3ac70');row=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedHiddenReturnTemporaries']['0x00e3ac70'];source=(R/'tools/script_recovery/fixtures/hidden_returns/lady_init.lua').read_text()
 return source,['unlifted: puStack_1c = v_stk_4;'],fn,row,c.rdata
def test_only_hidden_return_comment_and_diagnostic_change(evidence):
 source,todo,fn,row,rd=evidence;out,remaining,proof=classify_hidden_result(source,todo,fn,row,rd)
 assert out==source.replace('    -- TODO(native): puStack_1c = v_stk_4;\n','',1) and remaining==[] and todo==['unlifted: puStack_1c = v_stk_4;']
 significant=lambda s:[(t.lastgroup,t[0]) for t in tokens(s) if t.lastgroup not in ['space','comment','longcomment']]
 assert significant(source)==significant(out) and len(proof)==1 and proof[0]['executableLuaUnchanged'] and 'unverified' in proof[0]['scopeLimit']
 assert classify_hidden_result(source,todo,fn,None,rd)==(source,todo,[])
@pytest.mark.parametrize('fault',['nativeSha256','sourceSha256','luaBodySha256','witness','missingWitness','method','slot','frameBytes','resultFrameOffset','resultEntryOffset','memberOffset','getterAddress','getterBytes','assignAddress','assignBytes','destructorAddress','destructorBytes','prologue','outAddress','getterCall','assignmentCall','destructorReceiver','fragment','diagnostic','missingTodo','aliases','region','sdkEvidence','scope','liveAlias','changedCallMetadata'])
def test_unproved_or_changed_hidden_return_rejected(evidence,fault):
 source,todo,fn,row,rd=evidence;row=deepcopy(row);todo=list(todo)
 if fault.endswith('Sha256'):row[fault]='0'*64
 elif fault=='witness':row['nativeInstructions'][row['roles']['outAddress']]='00'
 elif fault=='missingWitness':row['nativeInstructions'].pop(row['roles']['outAddress'])
 elif fault=='method':row['method']='GetOtherString'
 elif fault=='slot':row['slot']=0x10
 elif fault in ['frameBytes','resultFrameOffset','resultEntryOffset','memberOffset']:row[fault]+=4
 elif fault.endswith('Address') and fault[:-7] in row['helpers']:row['helpers'][fault[:-7]]['address']='0x99ebf0'
 elif fault.endswith('Bytes') and fault[:-5] in row['helpers']:row['helpers'][fault[:-5]]['bytes']='00'*12
 elif fault=='prologue':row['roles']['reserve']=row['roles']['saveEbx']
 elif fault in ['outAddress','getterCall','assignmentCall','destructorReceiver']:row['roles'][fault]=row['roles']['saveEbx']
 elif fault=='fragment':row['nativeSourceFragment']='missing native hidden result'
 elif fault=='diagnostic':row['diagnostic']='unlifted: real_operation();'
 elif fault=='missingTodo':todo.clear()
 elif fault=='aliases':row['nativeOnlyAliases']=['puStack_1c','other']
 elif fault=='region':row['luaRegion']+='other()\n'
 elif fault=='sdkEvidence':row['acceptedSdkEvidenceSha256']=''
 elif fault=='scope':row['scopeLimit']=''
 elif fault=='liveAlias':source+='\nprint(puStack_1c)\n';row['luaBodySha256']=hashlib.sha256(source.encode()).hexdigest()
 elif fault=='changedCallMetadata':fn=deepcopy(fn);next(c for c in fn['indirectCalls'] if int(c['site'],16)==0xe3ac99)['pushedStack']=[-8]
 with pytest.raises(ValueError):classify_hidden_result(source,todo,fn,row,rd)

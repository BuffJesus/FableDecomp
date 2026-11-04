"""A stale staging classification must fail closed when its witnesses drift."""
from copy import deepcopy
import hashlib,json
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_restored_call_diagnostics import classify_staging
from tools.script_recovery.readable_lua import tokens
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');fn=c.native('0x00e3eb10');row=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedRestoredVtableStaging']['0x00e3eb10'];source=(R/'tools/script_recovery/fixtures/restored_vtable/lady_input.lua').read_text()
 todo=['label LAB_00e40085','label LAB_00e40364','label LAB_00e3fdc4','label LAB_00e3f4e7','unlifted: ppuVar13 = *piVar1','unlifted: ppuVar13 = *pppuVar28']
 return source,todo,fn,row,c.rdata
def test_only_comments_and_two_diagnostics_change(evidence):
 source,todo,fn,row,rd=evidence;old=list(todo);out,remaining,proof=classify_staging(source,todo,fn,row,rd)
 expected=source
 for path in row['paths']:
  expected=expected.replace('            -- TODO(native): '+path['diagnostic'][10:]+'\n','',1)
 expected=expected.replace('ppuVar13 = nil --[[unresolved native value]]','ppuVar13 = nil')
 assert out==expected and remaining==todo[:-2] and todo==old
 significant=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ['space','comment','longcomment']]
 assert significant(out)==significant(source)
 assert out.count('ppuVar13 = nil')==2 and out.count('quest:PauseAllNonScriptedEntities(false)')==source.count('quest:PauseAllNonScriptedEntities(false)')
 assert proof[0]['executableLuaUnchanged'] and all(p['luaAssignmentRetained'] for p in proof[0]['paths']) and 'unverified' in proof[0]['scopeLimit']
 assert classify_staging(source,todo,fn,None,rd)==(source,todo,[])
@pytest.mark.parametrize('fault',['nativeSha256','sourceSha256','luaBodySha256','slot','target','callee','witness','missingWitness','sharedCall','luaCallLabel','variable','savedFrame','entryFrame','abi','evidence','cache','scope','missingPath','duplicatePath','diagnostic','missingTodo','region','label','sourceFragment','nativePath','noncontiguous','extraRead','falseCall','argumentWitness'])
def test_changed_or_unresolved_evidence_rejected(evidence,fault):
 source,todo,fn,row,rd=evidence;row=deepcopy(row);todo=list(todo);path=row['paths'][0]
 if fault.endswith('Sha256'):row[fault]='0'*64
 elif fault=='slot':row['slot']='0x5e8'
 elif fault=='target':row['target']='0x890ab1'
 elif fault=='callee':row['calleeBytes']='00'*32
 elif fault=='witness':row['nativeInstructions'][path['instructions'][0]]='00'
 elif fault=='missingWitness':row['nativeInstructions'].pop(path['instructions'][0])
 elif fault=='sharedCall':row['sharedCall']=path['instructions'][0]
 elif fault=='luaCallLabel':row['luaCallLabel']='::LAB_00e3f729::'
 elif fault=='variable':row['luaStagingVariable']='other'
 elif fault=='savedFrame':row['receiverProof']['savedFrameOffset']+=4
 elif fault=='entryFrame':row['receiverProof']['entryFrameOffset']+=4
 elif fault=='abi':row['receiverProof']['ordinaryMsvcAbiAssumed']=False
 elif fault=='evidence':row['receiverProof']['acceptedEvidenceSha256']=''
 elif fault=='cache':row['receiverProof']['cacheInstructions'][0]=path['instructions'][0]
 elif fault=='scope':row['scopeLimit']=''
 elif fault=='missingPath':row['paths'].pop()
 elif fault=='duplicatePath':row['paths'][1]=deepcopy(path)
 elif fault=='diagnostic':path['diagnostic']='unlifted: actual_effect()'
 elif fault=='missingTodo':todo.pop()
 elif fault=='region':path['luaRegion']+='unrelated()\n'
 elif fault=='label':path['luaLabel']='::LAB_00e3f729::'
 elif fault=='sourceFragment':path['nativeSourceFragment']='missing native staging'
 elif fault=='nativePath':path['instructions']=row['paths'][1]['instructions']
 elif fault=='noncontiguous':path['instructions'][1]=path['instructions'][-1]
 elif fault=='extraRead':
  source+='\nprint(ppuVar13)\n';row['luaBodySha256']=hashlib.sha256(source.encode()).hexdigest()
 elif fault=='falseCall':
  source=source.replace('quest:PauseAllNonScriptedEntities(false)','quest:PauseAllNonScriptedEntities(true)');row['luaBodySha256']=hashlib.sha256(source.encode()).hexdigest();path['luaRegion']=path['luaRegion'].replace('(false)','(true)')
 elif fault=='argumentWitness':
  row['paths'][1]['instructions'][-1]=row['paths'][0]['instructions'][1]
 with pytest.raises(ValueError):classify_staging(source,todo,fn,row,rd)

"""Only witnessed CString construction masks may leave the unresolved ledger."""
from copy import deepcopy
import json,re
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_temporary_mask_diagnostics import classify
from tools.script_recovery.readable_lua import tokens
R=Path(__file__).resolve().parents[2]
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');fn=c.native('0x00e3eb10');row=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedTemporaryMaskDiagnostics']['0x00e3eb10'];source=(R/'tools/script_recovery/fixtures/temporary_masks/lady_input.lua').read_text()
 todo=['unlifted: xStack_15c = xStack_15c & 0xfffffffe;','label LAB_00e40085','unlifted: xStack_15c = xStack_15c | 2;','unlifted: xStack_15c = uVar15 & 0xfffffffd;','label LAB_00e40364','label LAB_00e3fdc4','unlifted: xStack_15c = xStack_15c | 2;','unlifted: xStack_15c = uVar15 & 0xfffffffd;','label LAB_00e3f4e7','unlifted: ppuVar13 = *piVar1','unlifted: ppuVar13 = *pppuVar28']
 return source,todo,fn,row,c.rdata
def test_five_comments_classified_with_all_runtime_tokens_exact(evidence):
 source,todo,fn,row,rd=evidence;old=list(todo);out,remaining,proof=classify(source,todo,fn,row,rd)
 expected=source
 for item in row['diagnostics']:expected=re.sub(r'(?m)^[ \t]*-- TODO\(native\): '+re.escape(item['text'][10:])+r'\n','',expected)
 assert out==expected and len(todo)-len(remaining)==5 and todo==old and len(proof)==1
 significant=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ['space','comment','longcomment']]
 assert significant(source)==significant(out)
 assert remaining==[d for d in todo if not d.startswith('unlifted: xStack_15c = ')]
 assert 'timing' in proof[0]['scopeLimit'] and 'unverified' in proof[0]['scopeLimit'] and proof[0]['executableLuaUnchanged']
 assert classify(source,todo,fn,None,rd)==(source,todo,[])
@pytest.mark.parametrize('fault',['nativeSha256','sourceSha256','luaBodySha256','witness','constructorAddress','constructorBytes','destructorAddress','destructorBytes','literal','literalAddress','scopeLimit','bit','duplicateGroup','missingGroup','stringFrame','maskFrame','setStore','setDepth','test','clear','skip','destructorReceiver','fragment','predicate','diagnosticCount','diagnosticText','missingTodo'])
def test_stale_or_non_cleanup_review_rejected(evidence,fault):
 source,todo,fn,row,rd=evidence;row=deepcopy(row);todo=list(todo);group=row['groups'][0]
 if fault.endswith('Sha256'):row[fault]='0'*64
 elif fault=='witness':row['nativeInstructions'][group['set']]='00'
 elif fault=='constructorAddress':row['constructor']['address']='0x99ebf1'
 elif fault=='constructorBytes':row['constructor']['bytes']='00'*32
 elif fault=='destructorAddress':row['destructor']['address']='0x99eae1'
 elif fault=='destructorBytes':row['destructor']['bytes']='00'*32
 elif fault=='literal':row['literal']='OTHER_NAME'
 elif fault=='literalAddress':row['literalAddress']='0x125d1c9'
 elif fault=='scopeLimit':row['scopeLimit']=''
 elif fault=='bit':group['bit']=16
 elif fault=='duplicateGroup':row['groups'].append(deepcopy(group))
 elif fault=='missingGroup':row['groups'].pop()
 elif fault=='stringFrame':group['stringFrameOffset']+=4
 elif fault=='maskFrame':row['maskFrameOffset']+=4
 elif fault=='setStore':group['setStore']=group['clearStore']
 elif fault=='setDepth':group['setArgumentDepth']=4
 elif fault=='test':group['test']=group['clear']
 elif fault=='clear':group['clear']=group['set']
 elif fault=='skip':group['skip']=group['clear']
 elif fault=='destructorReceiver':group['destructorReceiver']=group['setStore']
 elif fault=='fragment':row['nativeSourceFragments'][0]='missing_native_mask'
 elif fault=='predicate':row['luaPredicates']['me:IsTalkedToByHero()']+=1
 elif fault=='diagnosticCount':row['diagnostics'][0]['count']+=1
 elif fault=='diagnosticText':row['diagnostics'][0]['text']='unlifted: real_behavior()'
 elif fault=='missingTodo':todo.pop(0)
 with pytest.raises(ValueError):classify(source,todo,fn,row,rd)

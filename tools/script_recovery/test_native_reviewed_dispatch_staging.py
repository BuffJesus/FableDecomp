from pathlib import Path
import hashlib,json
from copy import deepcopy
import pytest
W=Path(__file__).resolve().parent/'fixtures/dispatch_staging/book_comment'
from tools.script_recovery.native_evidence_lowering import LoweringSpec
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery import native_reviewed_dispatch_staging as classifier
from tools.script_recovery import native_member_resource_calls as resources
from tools.script_recovery import lua_staging_liveness as liveness

digest=lambda b:hashlib.sha256(b).hexdigest()
@pytest.fixture
def inputs():
    unit=json.loads((W/'unit_baseline.json').read_text())
    fn=json.loads((W/'native_function.json').read_text())
    source=(W/'phase_preimage.lua').read_text()
    row=dict(nativeExtentSha256=digest((W/'native_function.bin').read_bytes()),luaPhaseSha256=digest(source.encode()),reads=[dict(nativeRead='0x00e5779a',dispatch='0x00e577e3',alias='x_stk_14',diagnostic='unlifted: x_stk_14 = *pCVar12',nativeSourceFragment='local_18 = *pCVar12;')])
    return dict(source=source,todo=['unlifted: x_stk_14 = *pCVar12'],fn=fn,row=row,parent=deepcopy(unit['reviewedResourceOperands']['0x00e57530']),spec=LoweringSpec(unit,'BS_Teacher',entity=True,thing_slots={}),rdata=RData(),resource_evidence=resources.resource_call_evidence,liveness=liveness.prove_unused_staging)
def test_classifies_only_comment_preserves_executable_tokens(inputs):
    source,todo,evidence=classifier.classify(**inputs)
    assert not todo and evidence[0]['liveness']['visitedNodes']==23
    assert source.count('x_stk_14 = nil --[[reviewed native dispatch staging]]')==1
    assert 'x_stk_14 = ("TEXT_QST_B16_BOOK_REQUEST_" .. pCVar12)' in source
    assert evidence[0]['executableLuaUnchanged']
@pytest.mark.parametrize('fault',['native','phase','parent','parentSource','read','otherResource','dispatch','spill','reload','flags','text','helper','alias','fragment','duplicate','missingTodo','duplicateTodo','missingParent','emptyReads','live','conditionalLive','closure'])
def test_stale_ambiguous_or_live_staging_rejects(inputs,fault):
    row=inputs['row'];read=row['reads'][0];parent=inputs['parent'];call=parent['calls'][0]
    if fault=='native':row['nativeExtentSha256']='0'*64
    elif fault=='phase':row['luaPhaseSha256']='0'*64
    elif fault=='parent':parent['nativeSha256']='0'*64
    elif fault=='parentSource':parent['sourceSha256']='0'*64
    elif fault=='read':read['nativeRead']='0x00e5779c'
    elif fault=='otherResource':read['nativeRead']='0x00e57949'
    elif fault=='dispatch':read['dispatch']='0x00e577d6'
    elif fault=='spill':call['vtableStack'][0]['bytes']='90'
    elif fault=='reload':call['vtableStack'][1]['entryEspDelta']+=4
    elif fault=='flags':call['flagPushes'][0]['bytes']='6a01'
    elif fault=='text':call['textToken']['start']+=1
    elif fault=='helper':call['helpers'][0]['sha256']='0'*64
    elif fault=='alias':read['alias']='value'
    elif fault=='fragment':read['nativeSourceFragment']='not exported'
    elif fault=='duplicate':row['reads'].append(deepcopy(read))
    elif fault=='missingTodo':inputs['todo']=[]
    elif fault=='duplicateTodo':inputs['todo']*=2
    elif fault=='missingParent':inputs['parent']=None
    elif fault=='emptyReads':row['reads']=[]
    elif fault in ['live','conditionalLive','closure']:
        addition=dict(live='quest:Observe(x_stk_14)',conditionalLive='if value then\nquest:Observe(x_stk_14)\nend',closure='local callback = function() return x_stk_14 end')[fault]
        needle='x_stk_14 = nil --[[unresolved native value]]'
        inputs['source']=inputs['source'].replace(needle,needle+'\n'+addition,1)
        row['luaPhaseSha256']=digest(inputs['source'].encode())
    with pytest.raises(ValueError):classifier.classify(**inputs)
def test_no_metadata_is_identity(inputs):
    inputs['row']=None
    source,todo,evidence=classifier.classify(**inputs)
    assert (source,todo,evidence)==(inputs['source'],inputs['todo'],[])


@pytest.mark.parametrize('replacement', [
    'b00090',  # AL
    'b40090',  # AH
    '6631c0',  # AX
    '31c090',  # EAX
    'ffd090',  # indirect CALL
    'eb0190',  # jump past a byte
    'c39090',  # RET
    'cd0390',  # interrupt
])
def test_native_table_lifetime_rejects_clobbers_even_with_updated_hashes(inputs,replacement):
    original=inputs['rdata']
    start=int(inputs['fn']['address'],16)
    raw=bytearray((W/'native_function.bin').read_bytes())
    offset=0xe5779c-start
    raw[offset:offset+3]=bytes.fromhex(replacement)
    class Mutant:
        def bytes_at(self,address,size):
            if start <= address and address+size <= start+len(raw):
                return bytes(raw[address-start:address-start+size])
            return original.bytes_at(address,size)
    inputs['rdata']=Mutant()
    inputs['row']['nativeExtentSha256']=digest(raw)
    inputs['parent']['nativeSha256']=digest(raw[:inputs['fn']['size']])
    with pytest.raises(ValueError):classifier.classify(**inputs)

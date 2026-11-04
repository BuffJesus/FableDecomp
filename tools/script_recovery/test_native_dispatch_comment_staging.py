from pathlib import Path
from copy import deepcopy
import hashlib,json,sys
import pytest
W=Path(__file__).resolve().parent
from tools.script_recovery import native_reviewed_dispatch_staging as classifier
from tools.script_recovery.native_evidence_lowering import LoweringSpec
from tools.script_recovery.native_member_resource_calls import resource_call_evidence
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
F=W/'fixtures/dispatch_staging/book_wrong_time'
read=lambda name:json.loads((F/name).read_text(encoding='utf-8'))
digest=lambda b:hashlib.sha256(b).hexdigest()
@pytest.fixture
def inputs():
    unit=read('candidate_unit.json')
    class Image(FixtureImage):
        def string_at(self,address):return read('strings.json').get(hex(address))
    image=Image([(int(r['address'],16),bytes.fromhex(r['bytes'])) for r in read('image_parts.json')])
    return dict(source=(F/'phase.lua').read_text(encoding='utf-8'),todo=read('phase_todo.json'),fn=read('native_function.json'),
        row=read('candidate_row.json'),parent=unit['reviewedResourceOperands']['0x00e54e90'],
        spec=LoweringSpec(unit,'BS_Teacher',entity=True,thing_slots={}),rdata=image,resource_evidence=resource_call_evidence,liveness=prove_unused_staging)
def run(inputs):return classifier.classify(**inputs)
def test_one_comment_only_is_reviewed(inputs):
    out,todo,reviews=run(inputs)
    assert out==inputs['source'].replace('-- TODO(native): xStack_7c = *pCVar11;','-- reviewed native dispatch staging: xStack_7c = *pCVar11;',1)
    assert len(inputs['todo'])-len(todo)==1 and len(reviews)==1
    assert reviews[0]['liveness']['analysisOnlyAssignment'] and reviews[0]['liveness']['visitedNodes']==169
    assert reviews[0]['resource']['offset']==0x34
def test_without_metadata_is_identity(inputs):
    inputs['row']=None
    assert run(inputs)==(inputs['source'],inputs['todo'],[])
@pytest.mark.parametrize('fault',['native','phase','parent','parentSource','read','dispatch','spill','reload','flags','zero','text','helper','alias','fragment','duplicate','missingTodo','duplicateTodo','missingParent','emptyReads','live','conditionalLive','closure','withoutOriginOptIn','withoutCommentOptIn','badCommentOptIn','badOriginOptIn','wrongLifetimeLoad','wrongFamily','wrongMember'])
def test_stale_or_ambiguous_or_live_rejects(inputs,fault):
    row=inputs['row'];read=row['reads'][0];parent=inputs['parent'];call=parent['calls'][0]
    if fault=='native':row['nativeExtentSha256']='0'*64
    elif fault=='phase':row['luaPhaseSha256']='0'*64
    elif fault=='parent':parent['nativeSha256']='0'*64
    elif fault=='parentSource':parent['sourceSha256']='0'*64
    elif fault=='read':read['nativeRead']='0x00e55833'
    elif fault=='dispatch':read['dispatch']='0x00e55858'
    elif fault=='spill':call['vtableStack'][0]['bytes']='90'
    elif fault=='reload':call['vtableStack'][1]['entryEspDelta']+=4
    elif fault=='flags':call['flagPushes'][0]['bytes']='6a01'
    elif fault=='zero':call['flagPushes'][1]['zeroDefinition']['bytes']='33c0'
    elif fault=='text':call['textPush']['literal']='WRONG'
    elif fault=='helper':call['helpers'][0]['sha256']='0'*64
    elif fault=='alias':read['alias']='value'
    elif fault=='fragment':read['nativeSourceFragment']='not exported'
    elif fault=='duplicate':row['reads'].append(deepcopy(read))
    elif fault=='missingTodo':inputs['todo']=[]
    elif fault=='duplicateTodo':inputs['todo']*=2
    elif fault=='missingParent':inputs['parent']=None
    elif fault=='emptyReads':row['reads']=[]
    elif fault=='withoutOriginOptIn':read.pop('reviewedMemberLifetime')
    elif fault=='withoutCommentOptIn':read.pop('commentOnly')
    elif fault=='badCommentOptIn':read['commentOnly']=1
    elif fault=='badOriginOptIn':read['reviewedMemberLifetime']=1
    elif fault=='wrongLifetimeLoad':call['receiverLifetime']['tableLoad']['site']='0x00e55854'
    elif fault=='wrongFamily':call['family']='parent'
    elif fault=='wrongMember':call['offset']+=4
    elif fault in ['live','conditionalLive','closure']:
        addition=dict(live='quest:Observe(xStack_7c)',conditionalLive='if value then\nquest:Observe(xStack_7c)\nend',closure='local callback = function() return xStack_7c end')[fault]
        needle='-- TODO(native): xStack_7c = *pCVar11;'
        inputs['source']=inputs['source'].replace(needle,needle+'\n'+addition,1);row['luaPhaseSha256']=digest(inputs['source'].encode())
    with pytest.raises(ValueError):run(inputs)
@pytest.mark.parametrize('replacement',['b000','b400','6633','31c0','ffd0','eb00','c390','cd03'])
def test_intervening_native_register_clobbers_or_transfers_reject_after_rehash(inputs,replacement):
    original=inputs['rdata'];start=int(inputs['fn']['address'],16)
    raw=bytearray(original.bytes_at(start,int(inputs['fn']['bodyEndExclusive'],16)-start))
    raw[0xe55833-start:0xe55835-start]=bytes.fromhex(replacement)
    class Mutant:
        def bytes_at(self,address,size):
            if start<=address and address+size<=start+len(raw):return bytes(raw[address-start:address-start+size])
            return original.bytes_at(address,size)
        def string_at(self,address):return original.string_at(address)
    inputs['rdata']=Mutant();inputs['row']['nativeExtentSha256']=digest(raw);inputs['parent']['nativeSha256']=digest(raw[:inputs['fn']['size']])
    lifetime=inputs['parent']['calls'][0]['receiverLifetime'];a,b=int(lifetime['start'],16),int(lifetime['end'],16)
    lifetime['sha256']=digest(raw[a-start:b-start])
    with pytest.raises(ValueError):run(inputs)

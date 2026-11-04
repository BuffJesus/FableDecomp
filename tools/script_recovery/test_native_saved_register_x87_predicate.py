from pathlib import Path
import copy,json,sys
import pytest
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R))
from tools.script_recovery.native_saved_register_x87_predicate import prove
from tools.script_recovery.native_hit_health_entry import prove_frame
from tools.script_recovery.lift_native_lua import RData
P=W/'fixtures/register_resource_health'
PACKET=json.loads((P/'predicate_packet.json').read_text())
FN=PACKET['function'];RAW=bytes.fromhex(PACKET['nativeBytes']);PROFILES=PACKET['profiles']
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
IMAGE=FixtureImage([(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in PACKET['imageParts']])
@pytest.fixture(autouse=True)
def no_retail_image(monkeypatch):
    monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
def frame(raw=RAW):
    return prove_frame(raw,FN,IMAGE,entity_vtable=0x12f18dc,scope_start=0xec5deb,scope_end=int(FN['bodyEndExclusive'],16),frame_depth=152,mask_offset=0,this_definition=0xec5de8,actor_definition=0xec5deb,actor_profile='local-edi')
BASE=frame()
def check(ctx,p,reader=None,**kw):
    return prove(ctx,int(p['call'],16),reader or IMAGE.bytes_at,kw.pop('cleanup_target',0x4aa840),positive_exit=kw.pop('positive_exit',p['positiveExit']),false_exit=kw.pop('false_exit',p['falseExit']),**kw)
@pytest.mark.parametrize('p',PROFILES)
def test_original_noncontiguous_or_contiguous_predicate(p):
    result=check(BASE,p)
    assert result['relation']=='ordered-greater' and not result['unordered']
    assert len(result['nodes'])==11 and result['frameDepth']==156
    assert result['cleanupOffset']==p['thingOffset']
@pytest.mark.parametrize('p',PROFILES)
@pytest.mark.parametrize('fault',['mask','status-arm','true-value','false-register','cleanup-register','cleanup-target','flag-consumer','false-exit-relation'])
def test_changed_actual_native_operand_or_branch_rejects(p,fault):
    ctx=BASE;raw=bytearray(RAW);result=check(ctx,p);code=[ctx['ins'][ctx['by'][int(a,16)]] for a in result['nodes']]
    indices={'mask':2,'status-arm':3,'true-value':4,'false-register':6,'cleanup-register':7,'cleanup-target':8,'flag-consumer':9,'false-exit-relation':10}
    i=code[indices[fault]];offset=i.address-int(FN['address'],16)
    if fault=='mask':raw[offset+2]=0x40
    elif fault in ['status-arm','false-exit-relation']:raw[offset]^=1
    elif fault=='true-value':raw[offset+1]=0
    elif fault=='false-register':raw[offset+1]=0xd2
    elif fault=='cleanup-register':raw[offset+1]^=8
    elif fault=='cleanup-target':raw[offset+1]^=1
    elif fault=='flag-consumer':raw[offset+1]=0xc0
    with pytest.raises((ValueError,AssertionError)):check(frame(bytes(raw)),p)
@pytest.mark.parametrize('p',PROFILES)
@pytest.mark.parametrize('fault',['external-entry','extra-edge','missing-edge','depth','purge','object-bound','missing-contract'])
def test_changed_cfg_or_normal_abi_contract_rejects(p,fault):
    ctx=dict(BASE);ctx['preds']={k:list(v) for k,v in BASE['preds'].items()};ctx['depths']=dict(BASE['depths']);ctx['contracts']=copy.deepcopy(BASE['contracts'])
    result=check(BASE,p);nodes=[BASE['by'][int(a,16)] for a in result['nodes']]
    if fault=='external-entry':ctx['preds'][nodes[1]].append(0)
    elif fault=='extra-edge':ctx['preds'][nodes[2]].append(nodes[0])
    elif fault=='missing-edge':ctx['preds'][nodes[1]]=[]
    elif fault=='depth':ctx['depths'][nodes[1]]+=4
    elif fault=='purge':ctx['contracts'][result['cleanup']]['derivedNormalPurge']=4
    elif fault=='object-bound':ctx['depths'].update({k:8 for k in nodes})
    else:del ctx['contracts'][result['cleanup']]
    with pytest.raises(ValueError):check(ctx,p)
@pytest.mark.parametrize('p',PROFILES)
@pytest.mark.parametrize('fault',['missing','nan','infinity','wrong-cleanup','swapped-exits'])
def test_missing_or_invalid_external_predicate_inputs_reject(p,fault):
    kwargs={}
    if fault=='missing':kwargs['reader']=lambda a,n:None
    elif fault=='nan':kwargs['reader']=lambda a,n:bytes.fromhex('0000c07f')
    elif fault=='infinity':kwargs['reader']=lambda a,n:bytes.fromhex('0000807f')
    elif fault=='wrong-cleanup':kwargs['cleanup_target']=0x99a2e0
    else:kwargs.update(positive_exit=p['falseExit'],false_exit=p['positiveExit'])
    with pytest.raises(ValueError):check(BASE,p,**kwargs)

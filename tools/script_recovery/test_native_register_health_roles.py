from pathlib import Path
import json,sys
import pytest
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R))
from tools.script_recovery.native_register_health_roles import prove_roles
from tools.script_recovery.native_saved_register_x87_predicate import prove
from tools.script_recovery.native_hit_health_entry import prove_frame
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
P=W/'fixtures/register_resource_health/predicate_packet.json'
PACKET=json.loads(P.read_text());FN=PACKET['function'];RAW=bytes.fromhex(PACKET['nativeBytes']);PROFILES=PACKET['profiles']
EXTERNAL=FixtureImage([(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in PACKET['imageParts']])
class Image:
    def bytes_at(self,a,n):
        start=int(FN['address'],16)
        if start<=a and a+n<=start+len(RAW):return RAW[a-start:a-start+n]
        return EXTERNAL.bytes_at(a,n)
IMAGE=Image()
@pytest.fixture(autouse=True)
def no_retail_image(monkeypatch):
    monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
@pytest.fixture
def context():
    return prove_frame(RAW,FN,IMAGE,entity_vtable=0x12f18dc,scope_start=0xec5deb,scope_end=int(FN['bodyEndExclusive'],16),frame_depth=152,mask_offset=0,this_definition=0xec5de8,actor_definition=0xec5deb,actor_profile='local-edi')
def predicate(ctx,p):
    return prove(ctx,int(p['call'],16),IMAGE.bytes_at,0x4aa840,positive_exit=p['positiveExit'],false_exit=p['falseExit'])
def check(ctx,p,pred=None,**kw):
    return prove_roles(ctx,int(p['call'],16),pred or predicate(ctx,p),IMAGE,getter_target=kw.get('getter_target',0x7e7490),this_definition=kw.get('this_definition',0xec5de8),health_slot=kw.get('health_slot',0x420))
@pytest.mark.parametrize('p',PROFILES)
def test_original_roles(context,p):
    result=check(context,p)
    assert result['resourceEntryOffset']==-120 and result['objectEntryOffset']==p['thingOffset']-156
    assert result['purges']==dict(getter=4,query=4,cleanup=0)
@pytest.mark.parametrize('p',PROFILES)
@pytest.mark.parametrize('fault',['getter','owner','slot','output','register','parent','duplicate-node','missing-node','query-purge','getter-purge','cleanup-purge','resource-coordinate','object-coordinate'])
def test_role_changes_reject(context,p,fault):
    ctx=context;pred=predicate(ctx,p);kw={}
    if fault=='getter':kw['getter_target']=0x7e7491
    elif fault=='owner':kw['this_definition']=0xec5deb
    elif fault=='slot':kw['health_slot']=0x424
    elif fault=='output':pred['cleanupOffset']+=4
    elif fault=='register':pred['flagRegister']='AL'
    elif fault=='parent':pred['preservedParent']='EAX'
    elif fault=='duplicate-node':pred['nodes'][1]=pred['nodes'][0]
    elif fault=='missing-node':pred['nodes'].pop()
    elif fault.endswith('-purge'):
        key={'query-purge':hex(int(p['call'],16)),'getter-purge':check(ctx,p)['getter'],'cleanup-purge':pred['cleanup']}[fault]
        ctx['contracts'][key]['derivedNormalPurge']+=4
    elif fault=='resource-coordinate':ctx['calls'][int(check(ctx,p)['getter'],16)]['ecxStack']+=4
    else:ctx['calls'][int(pred['cleanup'],16)]['ecxStack']+=4
    with pytest.raises(ValueError):check(ctx,p,pred,**kw)

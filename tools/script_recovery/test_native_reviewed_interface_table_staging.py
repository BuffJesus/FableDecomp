from pathlib import Path
from copy import deepcopy
import hashlib,importlib.util,json,sys
import pytest
R=Path(__file__).resolve().parents[2];W=Path(__file__).resolve().parent/'fixtures/interface_table/book_main'
from tools.script_recovery.native_evidence_lowering import LoweringSpec
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
from tools.script_recovery.lua_terminal_cleanup_alias import prove_terminal_cleanup_alias
from tools.script_recovery.native_register_snapshot_provenance import analyze
from tools.script_recovery import native_reviewed_interface_table_staging as classifier
from tools.script_recovery import native_cleanup_regions as hoister
from tools.script_recovery.annotate_interface_slots import VTABLE_BASE

class FixtureImage:
    ok=True
    base=0
    def __init__(self,parts):
        self.data=b'';self.raw_secs=[]
        for address,raw in parts:
            self.raw_secs.append((address,len(raw),len(self.data)));self.data+=raw
    def bytes_at(self,address,size):
        for start,length,offset in self.raw_secs:
            if start<=address and address+size<=start+length:
                return self.data[offset+address-start:offset+address-start+size]
        return None
read=lambda p:json.loads(p.read_text())
sha=lambda value:hashlib.sha256(value).hexdigest()

@pytest.fixture(scope='module')
def baseline():
    unit=read(W/'unit_baseline.json');fn=read(W/'native_function.json');row=read(W/'candidate_row.json')
    target=int(row['target'],16)
    image=FixtureImage([(int(fn['address'],16),(W/'native_function.bin').read_bytes()),
                        (VTABLE_BASE+0x5ec,target.to_bytes(4,'little')),
                        (target,bytes.fromhex(row['calleeWitnessPrefix']))])
    # Unit tests use a compact byte-backed image; the full retail image and
    # actual stack convention are independently pinned/executed by the evidence packet.
    row['imageSha256']=sha(image.data)
    from tools.script_recovery import convert_quest_unit
    with pytest.MonkeyPatch.context() as patch:
        patch.setattr(convert_quest_unit,'callee_stack_words',lambda address:1 if address==target else None)
        yield dict(source=(W/'phase.lua').read_text(),todo=read(W/'phase_todo.json'),fn=fn,row=row,
                   spec=LoweringSpec(unit,'BS_Teacher',entity=True,thing_slots={}),image=image)

def run(b,**changes):
    args=dict(b);args.update(changes)
    return classifier.classify(**args,snapshot_evidence=analyze,liveness=prove_unused_staging,
                               hoister=hoister.hoist_cleanup_regions,whole_liveness=prove_terminal_cleanup_alias)

def test_no_evidence_is_noop(baseline):
    source,todo,reviews=run(baseline,row=None)
    assert (source,todo,reviews)==(baseline['source'],baseline['todo'],[])

def test_single_label_classification_retains_duplicate_and_cleanup(baseline):
    out,todo,reviews=run(baseline)
    assert out==(W/'candidate_phase.lua').read_text()
    assert len(todo)==len(baseline['todo'])-1 and todo.count(baseline['row']['diagnostic'])==1
    assert len(reviews[0]['postHoistLiveness']['terminalCallLines'])==6

@pytest.mark.parametrize('key,value',[
 ('imageSha256','0'*64),('nativeSha256','0'*64),('sourceSha256','0'*64),('luaPhaseSha256','0'*64),
 ('abiContract',{}),('scopeLimit','proof complete'),('acceptedEvidenceSha256','not a seal'),
 ('pointerLoad','0x00e551c9'),('nativeRead','0x00e55a17'),('sharedTail','0x00e55c41'),('dispatch','0x00e55c47'),
 ('pointerRegister','eax'),('pointerRegister','ebp'),('ownerRegister','edi'),('tableRegister','ecx'),('tableRegister','edx'),
 ('branchBytes',8),('argument',True),('argument',0),('method','GetHero'),('slot','0x118'),('target','0x00891ca0'),
 ('calleeWitnessPrefix','00'*100),('purgeWords',0),('alias','other'),('receiverAlias','other'),
 ('diagnostic','unlifted: other'),('diagnosticMultiplicity',1),('nativeSourceFragment','wrong'),('luaRegion','wrong'),
])
def test_evidence_or_shape_mismatch_rejects(baseline,key,value):
    row=deepcopy(baseline['row']);row[key]=value
    with pytest.raises(ValueError):run(baseline,row=row)

def test_absent_todo_rejects(baseline):
    todo=[t for t in baseline['todo'] if t!=baseline['row']['diagnostic']]
    with pytest.raises(ValueError):run(baseline,todo=todo)

def test_duplicated_lua_region_rejects_even_with_updated_seal(baseline):
    source=baseline['source'].replace(baseline['row']['luaRegion'],baseline['row']['luaRegion']*2)
    row=deepcopy(baseline['row']);row['luaPhaseSha256']=sha(source.encode())
    with pytest.raises(ValueError):run(baseline,source=source,row=row)

def test_alias_read_inside_cleanup_rejects_after_updating_seals(baseline):
    region=baseline['row']['luaRegion']
    new=region.replace('            -- TODO(native): goto','            quest:Observe(iVar8)\n            -- TODO(native): goto')
    assert new!=region
    source=baseline['source'].replace(region,new);row=deepcopy(baseline['row'])
    row.update(luaRegion=new,luaPhaseSha256=sha(source.encode()))
    with pytest.raises(ValueError):run(baseline,source=source,row=row)

def test_rejecting_hoister_cannot_accept_marker(baseline):
    def rejecting_hoister(text):
        return hoister.hoist_cleanup_regions(text.replace('-- reviewed native interface-table staging:', '-- opaque review:'))
    with pytest.raises(ValueError):
        classifier.classify(**baseline,snapshot_evidence=analyze,liveness=prove_unused_staging,
                            hoister=rejecting_hoister,whole_liveness=prove_terminal_cleanup_alias)

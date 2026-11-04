from pathlib import Path
from copy import deepcopy
import hashlib,json
import pytest
from tools.script_recovery import native_reviewed_saved_table_staging as classifier
from tools.script_recovery import convert_quest_unit
from tools.script_recovery.native_evidence_lowering import LoweringSpec
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
from tools.script_recovery.native_cleanup_regions import hoist_cleanup_regions
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
W=Path(__file__).resolve().parent/'fixtures/saved_table/book_main'
read=lambda name:json.loads((W/name).read_text(encoding='utf-8'))
sha=lambda data:hashlib.sha256(data).hexdigest()
@pytest.fixture(scope='module')
def baseline():
    unit=read('candidate_unit.json');row=read('candidate_row.json')
    image=FixtureImage([(int(r['address'],16),bytes.fromhex(r['bytes'])) for r in read('image_parts.json')])
    row['imageSha256']=sha(image.data)
    returns={int(r['target'],16):r['purgeBytes']//4 for r in row['callContracts']}
    with pytest.MonkeyPatch.context() as patch:
        patch.setattr(convert_quest_unit,'callee_stack_words',lambda target:returns.get(target))
        yield dict(source=(W/'phase.lua').read_text(encoding='utf-8'),todo=read('phase_todo.json'),fn=read('native_function.json'),row=row,
            spec=LoweringSpec(unit,'BS_Teacher',entity=True,thing_slots={}),image=image,copy_trace=read('copy_trace.json'))
def run(b,**changes):
    args=dict(b);args.update(changes)
    return classifier.classify(**args,liveness=prove_unused_staging,hoister=hoist_cleanup_regions)
def test_without_review_is_noop(baseline):
    assert run(baseline,row=None)==(baseline['source'],baseline['todo'],[])
def test_four_dead_aliases_are_reviewed_without_executable_changes(baseline):
    out,todo,reviews=run(baseline)
    assert len(baseline['todo'])-len(todo)==4 and len(reviews[0]['aliases'])==4
    assert out.count('-- reviewed native saved-table staging:')==4
    assert reviews[0]['nativeCache']['instructions']==261 and len(reviews[0]['callContracts'])==48
    assert sum(r['receiverValueEquivalenceProved'] is False for r in reviews[0]['callContracts'])==4
@pytest.mark.parametrize('key,value',[
    ('imageSha256','0'*64),('sourceSha256','0'*64),('luaPhaseSha256','0'*64),('cacheEvidenceSha256','no'),('prefixEvidenceSha256','no'),
    ('scopeLimit','fully verified'),('diagnosticMultiplicity',3),('readBranchBytes',10),('sharedTail','0xe55c41'),('dispatch','0xe55c47'),
    ('alias','bad alias'),('cacheAlias','wrongCache'),('sourceCacheAlias','xStack_7c'),('nativeSourceFragment','wrong'),('originalLuaRegion','missing'),
])
def test_changed_review_rejects(baseline,key,value):
    row=deepcopy(baseline['row']);row[key]=value
    with pytest.raises(ValueError):run(baseline,row=row)
@pytest.mark.parametrize('kind',[
    'missing_call','duplicate_call','wrong_purge','wrong_target','invent_receiver_equivalence','member_vtable','member_witness',
    'return_region_hash','predicate_offset','predicate_slot','native_branch','missing_copy','copied_guard','copied_order','copy_phase_hash',
    'missing_actual_trace','duplicate_actual_trace','caller_frame_contract','overlapping_string_buffer','missing_MOV_ESP_buffer',
    'invalid_api','bad_api_prefix','bad_cache_body_hash','bad_cache_pointer_origin',
])
def test_evidence_boundaries_reject(baseline,kind):
    row=deepcopy(baseline['row']);trace=deepcopy(baseline['copy_trace'])
    if kind=='missing_call':row['callContracts'].pop()
    elif kind=='duplicate_call':row['callContracts'].append(deepcopy(row['callContracts'][0]))
    elif kind=='wrong_purge':row['callContracts'][0]['purgeBytes']+=4
    elif kind=='wrong_target':row['callContracts'][0]['target']='0x891ca0'
    elif kind=='invent_receiver_equivalence':next(r for r in row['callContracts'] if r['receiverValueEquivalenceProved'] is False)['receiverValueEquivalenceProved']=True
    elif kind=='member_vtable':row['memberProfile']['vtable']='0x1270950'
    elif kind=='member_witness':row['memberProfile']['witnesses']['factory']['bytes']='90'+row['memberProfile']['witnesses']['factory']['bytes'][2:]
    elif kind=='return_region_hash':next(iter(row['boundedReturnRegions'].values()))[0]['sha256']='0'*64
    elif kind=='predicate_offset':row['predicate']['accessorOffset']=16
    elif kind=='predicate_slot':row['predicate']['vtable']='0x1238c8c'
    elif kind=='native_branch':row['nativePredicateBranches'][0]['bytes']='90'
    elif kind=='missing_copy':row['copies'].pop()
    elif kind in ('copied_guard','copied_order','copy_phase_hash'):
        before=deepcopy(row['copies'][0]['copyTrace']);after=row['copies'][0]['copyTrace']
        if kind=='copied_guard':after['originalJump']='if (!bVar6) goto LAB_00e55a13;'
        elif kind=='copied_order':after['copiedStatements'][1:3]=list(reversed(after['copiedStatements'][1:3]))
        else:after['inputSha256']='not a hash'
        trace[trace.index(before)]=deepcopy(after)
    elif kind=='missing_actual_trace':trace=[]
    elif kind=='duplicate_actual_trace':trace.append(deepcopy(row['copies'][0]['copyTrace']))
    elif kind=='caller_frame_contract':row['cacheProof']['abiContract']['callerFramePreservedExceptDeclaredBuffers']=False
    elif kind=='overlapping_string_buffer':row['cacheProof']['declaredBuffers']['0xe555d1']['bytes']=28
    elif kind=='missing_MOV_ESP_buffer':del row['cacheProof']['declaredBuffers']['0xe555d1']
    elif kind=='invalid_api':row['api']['method']='GetHero'
    elif kind=='bad_api_prefix':row['api']['prefix']='90'*16
    elif kind=='bad_cache_body_hash':row['cacheProof']['nativeSha256']='0'*64
    elif kind=='bad_cache_pointer_origin':row['cacheProof']['pointerLoad']='0xe551c9'
    with pytest.raises(ValueError):run(baseline,row=row,copy_trace=trace)
def test_phase_seal_rejects_added_observable_read(baseline):
    source=baseline['source'].replace('iVar8 = nil --[[unresolved native value]]','iVar8 = nil --[[unresolved native value]]\nprobe(iVar8)',1)
    with pytest.raises(ValueError):run(baseline,source=source)

@pytest.mark.parametrize('kind',[
    'store_coordinate','store_owner','read_coordinate','flag_overlap','wrong_table_base','nonstack_write','ESP_displacement',
    'bypass_store','direct_ESP_escape','buffer_overlap','missing_MOV_buffer','missing_call','wrong_purge','extra_call',
])
def test_native_epoch_semantic_guards_with_updated_hash(baseline,kind):
    from tools.script_recovery.native_cache_epoch_proof import prove
    from tools.script_recovery.native_register_snapshot_provenance import analyze
    import capstone as cs
    row=deepcopy(baseline['row']['cacheProof']);fn=baseline['fn'];start=int(fn['address'],16)
    raw=bytearray((W/'native_function.bin').read_bytes())
    contracts={int(r['site'],16):dict(purgeBytes=r['purgeBytes']) for r in baseline['row']['callContracts']}
    def patch(a,data):raw[a-start:a-start+len(data)]=data
    if kind=='store_coordinate':patch(0xe551d2,b'\x14')
    elif kind=='store_owner':patch(0xe551d0,b'\x74')
    elif kind=='read_coordinate':patch(0xe55a16,b'\x10')
    elif kind=='flag_overlap':patch(0xe55515,b'\x14')
    elif kind=='wrong_table_base':patch(0xe55a18,b'\x03')
    elif kind=='nonstack_write':patch(0xe55512,bytes.fromhex('c645130190'))
    elif kind=='ESP_displacement':patch(0xe556ac,b'\x04')
    elif kind=='bypass_store':
        md=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32)
        branch=next(i for i in md.disasm(raw,start) if i.address<int(row['cacheStore'],16) and i.mnemonic.startswith('j') and i.mnemonic!='jmp' and i.size==6)
        patch(branch.address+2,(int(row['pointerRead'],16)-branch.address-branch.size).to_bytes(4,'little',signed=True))
    elif kind=='direct_ESP_escape':patch(0xe555d0,b'\x54')
    elif kind=='buffer_overlap':patch(0xe554fb,b'\x14');row['declaredBuffers']['0xe554f8']['coordinate']=24
    elif kind=='missing_MOV_buffer':del row['declaredBuffers']['0xe555d1']
    elif kind=='missing_call':del contracts[0xe551e9]
    elif kind=='wrong_purge':contracts[0xe55506]['purgeBytes']+=4
    elif kind=='extra_call':contracts[0xe551cf]=dict(purgeBytes=0)
    row['nativeSha256']=sha(raw)
    with pytest.raises(ValueError):prove(bytes(raw),start,row,contracts,analyze)

def test_native_epoch_equivalent_LEA_encoding_preserves_proof(baseline):
    from tools.script_recovery.native_cache_epoch_proof import prove
    from tools.script_recovery.native_register_snapshot_provenance import analyze
    row=deepcopy(baseline['row']['cacheProof']);start=int(baseline['fn']['address'],16)
    raw=bytearray((W/'native_function.bin').read_bytes());contracts={int(r['site'],16):dict(purgeBytes=r['purgeBytes']) for r in baseline['row']['callContracts']}
    before=prove(bytes(raw),start,row,contracts,analyze)
    raw[0xe556ab-start]=0x64;row['nativeSha256']=sha(raw)
    assert prove(bytes(raw),start,row,contracts,analyze)==before

@pytest.mark.parametrize('hex_bytes,regions,expected',[
    ('c3c20800',[(0x1000,0x1001)],0),('85c07401c3c3',[(0x1000,0x1006)],0),
    ('eb02ccccc3',[(0x1000,0x1002),(0x1004,0x1005)],0),('e800010000c3',[(0x1000,0x1006)],0),
])
def test_bounded_return_positive_shapes(hex_bytes,regions,expected):
    from tools.script_recovery.native_bounded_return_contracts import inspect
    image=FixtureImage([(0x1000,bytes.fromhex(hex_bytes))]);proof=inspect(image,0x1000,regions)
    assert proof['purgeBytes']==expected
    if hex_bytes.startswith('e8'):assert len(proof['opaqueCalls'])==1
@pytest.mark.parametrize('hex_bytes',[
    '85c07401c3c20400','ffe0','eb10','c2','cc','ebfe','75016690c3',
])
def test_bounded_return_adverse_shapes(hex_bytes):
    from tools.script_recovery.native_bounded_return_contracts import inspect
    raw=bytes.fromhex(hex_bytes);image=FixtureImage([(0x1000,raw)])
    with pytest.raises(ValueError):inspect(image,0x1000,[(0x1000,0x1000+len(raw))])

def test_optional_member_roles_leave_default_unknown():
    from tools.script_recovery.native_register_snapshot_provenance import analyze
    raw=bytes.fromhex('568bf18d7e348b078bcfff50305ec3');default=analyze(raw,0x1000)
    call=next(i.address for i in default['instructions'] if i.mnemonic=='call')
    assert 'edi' not in default['states'][call]
    states=analyze(raw,0x1000,member_offsets=(0x34,))['states'][call]
    assert states['edi']==('member_address',0x34) and states['ecx']==('member_address',0x34)
    assert states['eax']==('member_table_snapshot',0x34,0x1006)
@pytest.mark.parametrize('lea_hex',[
    '8d7e30','8d7c8634','bf34120000',
])
def test_member_roles_reject_different_offset_index_or_unrelated_pointer(lea_hex):
    from tools.script_recovery.native_register_snapshot_provenance import analyze
    raw=bytes.fromhex('568bf1'+lea_hex+'8b078bcfff50305ec3')
    proof=analyze(raw,0x1000,member_offsets=(0x34,))
    calls=[i.address for i in proof['instructions'] if i.mnemonic=='call'];assert len(calls)==1
    assert not any(v and v[0]=='member_table_snapshot' for v in proof['states'][calls[0]].values())

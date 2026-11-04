from pathlib import Path
from copy import deepcopy
import hashlib,json,sys
import pytest
W=Path(__file__).resolve().parent/'fixtures/stack_byte_liveness/book_specific_args'
R=Path(__file__).resolve().parents[2]
from tools.script_recovery.native_reviewed_stack_byte_spills import classify
from tools.script_recovery.native_stack_byte_helper_access import summarize
from tools.script_recovery.native_stack_byte_liveness import prove
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.readable_lua import tokens
read=lambda p:json.loads(p.read_text(encoding='utf-8'))
sha=lambda b:hashlib.sha256(b).hexdigest()

@pytest.fixture
def inputs():
    row=read(W/'candidate_row.json')
    return dict(source=(W/'phase_preimage.lua').read_text(encoding='utf-8'),
        todo=['label retained',row['reads'][0]['diagnostic']],fn=read(W/'native_function.json'),
        row=row,image=RData(),unit=read(W/'unit_baseline.json'),owner='BS_Teacher',summarize=summarize,prove=prove)

def test_only_reviewed_marker_changes(inputs):
    source,todo,proof=classify(**inputs)
    assert todo==['label retained'] and len(proof)==1
    assert proof[0]['reads'][0]['proof']['visitedStates']==23
    assert '-- reviewed native scratch-byte spill: native_arg_param_2 = ' in source
    significant=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ('space','comment','longcomment')]
    assert significant(source)==significant(inputs['source'])

def test_missing_metadata_is_identity(inputs):
    inputs['row']=None
    assert classify(**inputs)==(inputs['source'],inputs['todo'],[])

@pytest.mark.parametrize('fault',[
    'nativeHash','sourceHash','phaseHash','sourceChanged','phaseChanged','wrongOwner','missingOwner',
    'wrongVtable','wrongGetter','helperHash','helperField','storeBytes','storeSite','slot','alias',
    'diagnostic','marker','nativeFragment','missingTodo','duplicateTodo','empty','duplicateRead',
    'duplicateMarker','duplicateNativeFragment',
])
def test_stale_ambiguous_or_mismatched_evidence_rejects(inputs,fault):
    row=inputs['row'];part=row['reads'][0]
    if fault=='nativeHash':row['nativeSha256']='0'*64
    elif fault=='sourceHash':row['sourceSha256']='0'*64
    elif fault=='phaseHash':row['luaPhaseSha256']='0'*64
    elif fault=='sourceChanged':inputs['fn']['decompile']+=' '
    elif fault=='phaseChanged':inputs['source']+=' '
    elif fault=='wrongOwner':inputs['owner']='other'
    elif fault=='missingOwner':inputs['unit']['entities'].pop('BS_Teacher')
    elif fault=='wrongVtable':inputs['unit']['entities']['BS_Teacher']['vtable']='0x012e5efc'
    elif fault=='wrongGetter':inputs['unit']['entities']['BS_Teacher']['functions']['GetParentScript']['address']='0x00e54630'
    elif fault=='helperHash':row['helperNativeProofs']['release']['sha256']='0'*64
    elif fault=='helperField':row['helperMetadata']['flagOffset']=4
    elif fault=='storeBytes':part['nativeStoreBytes']='90442418'
    elif fault=='storeSite':part['nativeStore']='0x00e573a5'
    elif fault=='slot':part['entryEspOffset']=20
    elif fault=='alias':part['alias']='other'
    elif fault=='diagnostic':part['diagnostic']='unlifted: native_arg_param_2 = changed'
    elif fault=='marker':part['marker']+='bad'
    elif fault=='nativeFragment':part['nativeSourceFragment']='not the exported expression'
    elif fault=='missingTodo':inputs['todo']=[]
    elif fault=='duplicateTodo':inputs['todo'].append(part['diagnostic'])
    elif fault=='empty':row['reads']=[]
    elif fault=='duplicateRead':row['reads'].append(deepcopy(part))
    elif fault=='duplicateMarker':
        inputs['source']+='\n-- TODO(native): '+part['marker']+'\n'
        row['luaPhaseSha256']=sha(inputs['source'].encode())
    elif fault=='duplicateNativeFragment':
        inputs['fn']['decompile']+='\n'+part['nativeSourceFragment']
        row['sourceSha256']=sha(inputs['fn']['decompile'].encode())
    with pytest.raises(ValueError):classify(**inputs)

@pytest.mark.parametrize('site,raw',[
    (0xe573a8,'8a442418'),  # Observe old byte on a reachable branch.
    (0xe573a4,'89442418'),  # A DWORD store is not this reviewed byte spill.
    (0xe5738f,'c20400'),    # Return leaves part of the slot caller-visible.
    (0xe573aa,'8d4424185090909090'),  # Address of old bytes escapes as an argument.
])
def test_native_observable_mutants_reject_even_with_updated_hash(inputs,site,raw):
    original=inputs['image'];patch=bytes.fromhex(raw)
    class Mutant:
        def __getattr__(self,name):return getattr(original,name)
        def bytes_at(self,address,size):
            data=original.bytes_at(address,size)
            if data is None:return None
            data=bytearray(data)
            for offset,byte in enumerate(patch):
                if address<=site+offset<address+size:data[site+offset-address]=byte
            return bytes(data)
    image=Mutant();inputs['image']=image
    start=int(inputs['fn']['address'],16);end=int(inputs['fn']['bodyEndExclusive'],16)
    inputs['row']['nativeSha256']=sha(image.bytes_at(start,end-start))
    if site==0xe573a4:inputs['row']['reads'][0]['nativeStoreBytes']=raw
    with pytest.raises(ValueError):classify(**inputs)

from pathlib import Path
from copy import deepcopy
import hashlib,json,sys
import pytest
W=Path(__file__).resolve().parent/'fixtures/definition_staging/book_vector_base'
P=W
from tools.script_recovery.native_reviewed_definition_staging import classify,json_bytes
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_evidence_lowering import LoweringSpec
from tools.script_recovery.native_member_resource_calls import resource_call_evidence
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
read=lambda p:json.loads(p.read_text(encoding='utf-8'))
sha=lambda data:hashlib.sha256(data).hexdigest()

@pytest.fixture
def inputs():
    row=read(W/'candidate_row.json')
    return dict(source=(P/'phase_preimage.lua').read_text(encoding='utf-8'),
        todo=['label retained',row['diagnostic']],fn=read(P/'native_function.json'),row=row,
        unit=read(P/'unit_baseline.json'),owner='BS_Teacher',image=RData(),
        resource_evidence=resource_call_evidence,liveness=prove_unused_staging)

def apply(inputs):
    return classify(**inputs,spec=LoweringSpec(inputs['unit'],inputs['owner'],entity=True,thing_slots={}))

def test_classifies_only_annotation_preserving_nil_and_fragment(inputs):
    source,todo,proof=apply(inputs)
    assert todo==['label retained'] and proof[0]['liveness']['visitedNodes']==381
    assert 'iVar1 = nil --[[reviewed native definition-vector staging]]' in source
    assert '-- reviewed native definition-vector staging: '+inputs['row']['marker'] in source

def test_no_metadata_is_identity(inputs):
    inputs['row']=None
    assert apply(inputs)==(inputs['source'],inputs['todo'],[])

@pytest.mark.parametrize('fault',[
    'nativeHash','sourceHash','phaseHash','nativeSource','phase','owner','vtableTarget','consumer',
    'definitionHash','indexName','indexKind','marker','diagnostic','alias','indexAlias','heroAlias','resultAlias',
    'spanWitness','spanOrder','spanEmpty','nativeRead','indirectSlot','duplicateIndirect','missingTodo','duplicateTodo',
    'fragment','duplicateFragment','duplicateMarker','liveRead','closureCapture',
])
def test_stale_ambiguous_wrong_consumer_or_observable_alias_rejects(inputs,fault):
    row=inputs['row']
    if fault=='nativeHash':row['nativeSha256']='0'*64
    elif fault=='sourceHash':row['sourceSha256']='0'*64
    elif fault=='phaseHash':row['luaPhaseSha256']='0'*64
    elif fault=='nativeSource':inputs['fn']['decompile']+=' '
    elif fault=='phase':inputs['source']+=' '
    elif fault=='owner':row['owner']='other'
    elif fault=='vtableTarget':row['heroTarget']='0x00891cb0'
    elif fault=='consumer':row['consumer']='IsObjectInHeroPossession'
    elif fault=='definitionHash':row['definitionSha256']='0'*64
    elif fault=='indexName':row['indexField'][0]='OtherIndex'
    elif fault=='indexKind':inputs['unit']['quest']['fields']['0x8c'][1]='Bool'
    elif fault=='marker':row['marker']+=' changed'
    elif fault=='diagnostic':row['diagnostic']='unlifted: iVar1 = changed'
    elif fault=='alias':row['alias']='other'
    elif fault=='indexAlias':row['indexAlias']='other'
    elif fault=='heroAlias':row['heroAlias']='other'
    elif fault=='resultAlias':row['resultAlias']='other'
    elif fault=='spanWitness':row['span'][0]['bytes']='90'
    elif fault=='spanOrder':row['span'][0],row['span'][1]=row['span'][1],row['span'][0]
    elif fault=='spanEmpty':row['span']=[]
    elif fault=='nativeRead':row['nativeRead']='0x00e5559b'
    elif fault=='indirectSlot':
        next(item for item in inputs['fn']['indirectCalls'] if int(item['site'],16)==0xe555b7)['slot']='0x2e4'
    elif fault=='duplicateIndirect':
        inputs['fn']['indirectCalls'].append(deepcopy(next(item for item in inputs['fn']['indirectCalls'] if int(item['site'],16)==0xe555b7)))
    elif fault=='missingTodo':inputs['todo']=[]
    elif fault=='duplicateTodo':inputs['todo'].append(row['diagnostic'])
    elif fault=='fragment':row['nativeSourceFragment']='not the exported load'
    elif fault=='duplicateFragment':
        inputs['fn']['decompile']+='\n'+row['nativeSourceFragment']
        row['sourceSha256']=sha(inputs['fn']['decompile'].encode())
    elif fault=='duplicateMarker':
        inputs['source']+='\n-- TODO(native): '+row['marker']+'\n'
        row['luaPhaseSha256']=sha(inputs['source'].encode())
    elif fault in ('liveRead','closureCapture'):
        needle='bVar6 = quest:IsObjectInThingsPossession(quest:GetStateStringVectorAt("BookDef", iVar8), pCVar9)'
        addition='quest:Observe(iVar1)' if fault=='liveRead' else 'local callback = function() return iVar1 end'
        inputs['source']=inputs['source'].replace(needle,needle+'\n'+addition,1)
        row['luaPhaseSha256']=sha(inputs['source'].encode())
    with pytest.raises(ValueError):apply(inputs)

@pytest.mark.parametrize('fault',['mutable','other_storage','wrong_stride','not_reader','initializer','wrong_element'])
def test_snapshot_contract_is_enforced_beyond_hash(inputs,fault):
    definition=inputs['unit']['quest']['definitionSnapshots']['0x94']
    if fault=='mutable':definition['writeFunctions']=['0x00e54e90']
    elif fault=='other_storage':definition['storage']='mutableVector'
    elif fault=='wrong_stride':definition['initialization']['stride']=8
    elif fault=='not_reader':definition['readFunctions'].remove('0x00e54e90')
    elif fault=='initializer':definition['initialization']['functions'].append('0x00e54e90')
    elif fault=='wrong_element':definition['element']='CConversation'
    inputs['row']['definitionSha256']=sha(json_bytes(definition))
    with pytest.raises(ValueError):apply(inputs)

@pytest.mark.parametrize('site,raw',[
    (0xe55593,'18'),(0xe55596,'90000000'),(0xe5559c,'98000000'),
    (0xe555a2,'ca'),(0xe555b2,'8be890'),(0xe55581,'18'),
])
def test_native_origin_index_stride_clobber_or_entry_rejects_even_with_updated_hash(inputs,site,raw):
    original=inputs['image'];patch=bytes.fromhex(raw)
    class Mutant:
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
    for witness in inputs['row']['span']:
        witness['bytes']=image.bytes_at(int(witness['address'],16),len(bytes.fromhex(witness['bytes']))).hex()
    with pytest.raises(ValueError):apply(inputs)

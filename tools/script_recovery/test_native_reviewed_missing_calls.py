from pathlib import Path
from copy import deepcopy
import hashlib,json,sys
import pytest
W=Path(__file__).resolve().parent
from types import SimpleNamespace
from tools.script_recovery import native_reviewed_missing_calls, native_retained_cstring_alias, convert_quest_unit
setup_candidate=SimpleNamespace(MISSING=native_reviewed_missing_calls,ALIAS=native_retained_cstring_alias,CONVERTER=convert_quest_unit)
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_stack_aliases import restore
F=W/'fixtures/missing_calls/talking_trader'
read=lambda name:json.loads((F/name).read_text())
digest=lambda data:hashlib.sha256(data).hexdigest()
@pytest.fixture
def inputs(monkeypatch):
    def deny(*args,**kwargs):raise AssertionError('Portable case opened a retail image')
    monkeypatch.setattr(RData,'__init__',deny)
    class Image(FixtureImage):
        def string_at(self,address):return read('strings.json').get(hex(address))
    return dict(fn=read('native_function.json'),row=read('candidate_row.json'),unit=read('candidate_unit.json'),image=Image([(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in read('image_parts.json')]))
def run(inputs):return setup_candidate.MISSING.augment(**inputs)
def test_21_exact_missing_records_complete_170_heads_without_mutating_input(inputs):
    before=deepcopy(inputs);out=run(inputs)
    assert len(out['calls'])==113 and len(out['indirectCalls'])==57
    assert out['decompile']==inputs['fn']['decompile'] and inputs['fn']==before['fn']
    assert out['reviewedMissingCallMetadata']['pairedHeads']==170 and len(out['reviewedRetainedCStringSites'])==7
@pytest.mark.parametrize('fault',['native','source','metadata','owner','vtable','main','ownerDefinition','actorDefinition','selector','cases','exits','idDefinition','idProducer','frameSize','frameBias','earlySave','lateSave','otherOffset','query','nameConstructor','name','lineBody','lineSize','recordDepth','recordSlot','missingRecord','extraRecord','duplicateRecord'])
def test_stale_or_ambiguous_missing_record_review_rejects(inputs,fault):
    row=inputs['row']
    if fault=='native':row['nativeExtentSha256']='0'*64
    elif fault=='source':row['sourceSha256']='0'*64
    elif fault=='metadata':row['originalMetadataSha256']='0'*64
    elif fault=='owner':row['owner']='Other'
    elif fault=='vtable':row['entityVtable']='0x1'
    elif fault=='main':inputs['unit']['entities']['TalkingTrader1']['functions']['Main']['address']='0x1'
    elif fault in ['ownerDefinition','actorDefinition','selector','idDefinition','idProducer']:row[fault]='0xecbdf0'
    elif fault=='cases':row['caseEntries'].reverse()
    elif fault=='exits':row['caseExits']=['0xecc299']
    elif fault=='frameSize':row['frame']['allocationBytes']+=4
    elif fault=='frameBias':row['frame']['bias']+=4
    elif fault=='earlySave':row['frame']['savedRegisters'][0]='0xecbe00'
    elif fault=='lateSave':row['frame']['savedRegisters'][-1]='0xecbff1'
    elif fault=='otherOffset':row['otherThing']['entryOffset']-=4
    elif fault=='query':row['otherThing']['querySite']='0xecc02b'
    elif fault=='nameConstructor':row['otherThing']['nameConstructor']='0xecc013'
    elif fault=='name':row['otherThing']['name']='WRONG'
    elif fault=='lineBody':row['lineAPI']['sha256']='0'*64
    elif fault=='lineSize':row['lineAPI']['size']-=1
    elif fault=='recordDepth':row['records'][0]['depth']+=4
    elif fault=='recordSlot':next(r for r in row['records'] if 'slot' in r)['slot']='0x5b4'
    elif fault=='missingRecord':row['records'].pop()
    elif fault=='extraRecord':row['records'].append(dict(site='0x00ECC013'))
    elif fault=='duplicateRecord':row['records'].append(deepcopy(row['records'][0]))
    with pytest.raises((ValueError,KeyError)):run(inputs)
def mutant(inputs,address,data):
    original=inputs['image'];start=int(inputs['fn']['address'],16);raw=bytearray(original.bytes_at(start,2910));assert raw[address-start:address-start+len(data)]!=data,'native fault must change bytes';raw[address-start:address-start+len(data)]=data
    class Mutant:
        def bytes_at(self,a,n):
            if start<=a and a+n<=start+len(raw):return bytes(raw[a-start:a-start+n])
            return original.bytes_at(a,n)
        def string_at(self,a):return original.string_at(a)
    inputs['image']=Mutant();inputs['row']['nativeExtentSha256']=digest(raw)
@pytest.mark.parametrize('address,data',[(0xecbdfd,'0c'),(0xecbfed,'c8'),(0xecbffc,'8f'),(0xecbf5c,'50'),(0xecc0a2,'50'),(0xecc0a5,'05'),(0xecc0af,'70'),(0xecc0a4,'91')])
def test_native_role_or_frame_mutations_reject_after_rehash(inputs,address,data):
    mutant(inputs,address,bytes.fromhex(data))
    with pytest.raises((ValueError,KeyError)):run(inputs)
def test_new_in_unit_children_require_reindexing(inputs):
    inputs['definition_addresses']={0x99ebf0}
    with pytest.raises(ValueError,match='reindexing'):run(inputs)
def test_duplicate_call_order_metadata_cannot_be_silently_overridden(inputs):
    inputs['fn']['callOrder'].append('0x00ECC088')
    inputs['row']['originalMetadataSha256']=digest(json.dumps(setup_candidate.MISSING.original_metadata(inputs['fn']),sort_keys=True,separators=(',',':')).encode())
    with pytest.raises(ValueError):run(inputs)
def test_early_scalar_profile_splits_only_numeric_lifetimes(inputs):
    fn=inputs['fn'];text=fn['decompile'];row=read('early_scalar_row.json')
    out=restore(text,fn,row,inputs['image'])
    assert 'switch(ambientConversationIndex)' in out and 'nextAmbientConversationIndex = ((ambientConversationIndex + 1 + 0x80000000) & 0xffffffff) - 0x80000000;' in out
    assert 'CCharString::CCharString((CCharString *)&uStack_b0,"TEXT_QST_B10_TRADERB_CHAT2_20",-1);' in out
    assert 'heroHitCleanupMask = uVar13 & 0xfffffffe;' in out
@pytest.fixture
def alias_inputs(inputs):
    fn=run(inputs);ordered=setup_candidate.CONVERTER._text_order_sites(fn['decompile'],fn,include_unslotted=True);by={int(e[3]['site'],16):e for e in ordered}
    return dict(fn=fn,ctor=by[0xecc088][3],site=by[0xecc0a3][3],ctor_args=by[0xecc088][2],image=inputs['image'])
def test_retained_array_string_has_native_single_entry_live_literal(alias_inputs):
    proof=setup_candidate.ALIAS.prove(**alias_inputs)
    assert proof and proof['stackOffset']==-108 and proof['singleNormalEntry'] and proof['noInterveningCallsOrWrites']
@pytest.mark.parametrize('fault',['ctorDepth','siteDepth','stackOffset','target','slot','literal','length','missingArgs'])
def test_retained_alias_rejects_stale_or_different_object(alias_inputs,fault):
    if fault=='ctorDepth':alias_inputs['ctor']['depth']+=4
    elif fault=='siteDepth':alias_inputs['site']['depth']+=4
    elif fault=='stackOffset':alias_inputs['ctor']['ecxStack']-=4
    elif fault=='target':alias_inputs['ctor']['target']='0x1'
    elif fault=='slot':alias_inputs['site']['slot']='0x5b4'
    elif fault=='literal':alias_inputs['ctor_args'][1]='"OTHER"'
    elif fault=='length':alias_inputs['ctor_args'][2]='0'
    elif fault=='missingArgs':alias_inputs['ctor_args'].pop()
    assert setup_candidate.ALIAS.prove(**alias_inputs) is None

def test_unit_scoped_overlay_restores_original_index_after_success(inputs):
    class Converter:pass
    obj=Converter();original={'0x00ecbdf0':inputs['fn']};obj.by_address=original;obj.rdata=inputs['image'];obj.retained_cstring_reviews=['stale']
    unit=deepcopy(inputs['unit']);unit['reviewedMissingCallMetadata']={'0x00ecbdf0':inputs['row']}
    @setup_candidate.MISSING.scoped_function_evidence
    def method(self,data,token):
        assert self.by_address is not original and len(self.by_address['0x00ecbdf0']['calls'])==113
        assert self.retained_cstring_reviews==[]
        return token
    assert method(obj,unit,37)==37 and obj.by_address is original and len(inputs['fn']['calls'])==99

def test_unit_scoped_overlay_restores_original_index_after_exception(inputs):
    class Converter:pass
    obj=Converter();original={'0x00ecbdf0':inputs['fn']};obj.by_address=original;obj.rdata=inputs['image']
    unit=deepcopy(inputs['unit']);unit['reviewedMissingCallMetadata']={'0x00ecbdf0':inputs['row']}
    @setup_candidate.MISSING.scoped_function_evidence
    def method(self,data):raise RuntimeError('deliberate downstream refusal')
    with pytest.raises(RuntimeError,match='deliberate'):method(obj,unit)
    assert obj.by_address is original and len(inputs['fn']['calls'])==99

def test_no_opt_in_leaves_converter_state_and_arguments_exact(inputs):
    class Converter:pass
    obj=Converter();original={'0x00ecbdf0':inputs['fn']};obj.by_address=original;obj.retained_cstring_reviews=['original']
    @setup_candidate.MISSING.scoped_function_evidence
    def method(self,data,*,token):return self.by_address,token
    assert method(obj,{},token=19)==(original,19) and obj.by_address is original and obj.retained_cstring_reviews==['original']

def test_invalid_overlay_never_rebinds_source_index(inputs):
    class Converter:pass
    obj=Converter();original={'0x00ecbdf0':inputs['fn']};obj.by_address=original;obj.rdata=inputs['image']
    unit=deepcopy(inputs['unit']);row=deepcopy(inputs['row']);row['nativeExtentSha256']='0'*64;unit['reviewedMissingCallMetadata']={'0x00ecbdf0':row}
    @setup_candidate.MISSING.scoped_function_evidence
    def method(self,data):raise AssertionError('must never reach conversion')
    with pytest.raises(ValueError):method(obj,unit)
    assert obj.by_address is original


@pytest.mark.parametrize('subset',['otherOwner','otherFunction','empty'])
def test_narrowed_unit_ignores_unselected_review_without_native_reads(inputs,subset):
    class Converter:pass
    obj=Converter();original={'0x00ecbdf0':inputs['fn']};obj.by_address=original
    obj.retained_cstring_reviews=['unchanged']
    unit=deepcopy(inputs['unit']);unit['reviewedMissingCallMetadata']={'0x00ecbdf0':inputs['row']}
    if subset=='otherOwner':unit['entities']={'Other':{'functions':{'Main':{'address':'0x00123456'}}}}
    elif subset=='otherFunction':unit['entities']['TalkingTrader1']['functions']={'Init':{'address':'0x00123456'}}
    else:unit['entities']={};unit['quest']={'functions':{},'helpers':{}}
    @setup_candidate.MISSING.scoped_function_evidence
    def method(self,data):return self.by_address
    assert method(obj,unit) is original and obj.by_address is original
    assert obj.retained_cstring_reviews==['unchanged']


@pytest.mark.parametrize('existing',[False,True])
@pytest.mark.parametrize('fail',[False,True])
def test_review_records_are_conversion_local_on_success_and_failure(inputs,existing,fail):
    class Converter:pass
    obj=Converter();original={'0x00ecbdf0':inputs['fn']};obj.by_address=original;obj.rdata=inputs['image']
    prior=[{'function':'other','proof':'retained caller state'}]
    if existing:obj.retained_cstring_reviews=prior
    unit=deepcopy(inputs['unit']);unit['reviewedMissingCallMetadata']={'0x00ecbdf0':inputs['row']}
    @setup_candidate.MISSING.scoped_function_evidence
    def method(self,data):
        assert self.retained_cstring_reviews==[]
        self.retained_cstring_reviews.append({'function':'0x00ecbdf0','proof':'this conversion only'})
        if fail:raise RuntimeError('deliberate report failure')
        return list(self.retained_cstring_reviews)
    if fail:
        with pytest.raises(RuntimeError,match='deliberate'):method(obj,unit)
    else:assert method(obj,unit)==[{'function':'0x00ecbdf0','proof':'this conversion only'}]
    assert obj.by_address is original
    assert hasattr(obj,'retained_cstring_reviews')==existing
    if existing:assert obj.retained_cstring_reviews is prior
    @setup_candidate.MISSING.scoped_function_evidence
    def unreviewed(self,data):return getattr(self,'retained_cstring_reviews',None)
    assert unreviewed(obj,{}) is (prior if existing else None)

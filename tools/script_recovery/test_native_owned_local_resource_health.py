from pathlib import Path
import copy,hashlib,json,sys
from types import SimpleNamespace
import pytest

W=Path(__file__).resolve().parent
R=W.parents[1]
F=W/'fixtures/beardy_resource_health'
from tools.script_recovery import native_local_resource_frame_contracts as FRAME
from tools.script_recovery import native_owned_local_resource_origin as ORIGIN
from tools.script_recovery import native_health_copy_correspondence as COPY
from tools.script_recovery import native_nullable_wrapper_contracts as NULLABLE
from tools.script_recovery import native_entity_factory_layout as ENTITY
from tools.script_recovery import native_external_import_contracts as EXTERNAL
from tools.script_recovery import native_reviewed_register_resource_health_lifetimes as MODULE
from tools.script_recovery import native_reviewed_register_health_comparisons as COMPARISON
from tools.script_recovery.lift_native_lua import RData
S=SimpleNamespace(FRAME=FRAME,ORIGIN=ORIGIN,COPY=COPY,NULLABLE=NULLABLE,ENTITY=ENTITY,EXTERNAL=EXTERNAL,MODULE=MODULE)

read=lambda p:json.loads(p.read_text(encoding='utf-8'))
serial=lambda v:json.dumps(v,separators=(',',':')).encode()


@pytest.fixture(scope='module')
def data():
    p=W/'fixtures/beardy_ordered_comparison'
    phase=read(p/'phase.json')
    unit=read(R/'refs/script_recovery/beardy_baldy/units/V_BeardyBaldy.json')
    unit['reviewedRegisterResourceHealthLifetimes']={'0x00e50fd0':read(F/'row.json')}
    image=RData()
    if not image.ok:pytest.skip('original retail image unavailable for native resource proof')
    before=(p/'phase.lua').read_text(encoding='utf-8')
    comparison=unit['reviewedRegisterHealthComparisons']['0x00e50fd0']
    corrected,_,_,_=COMPARISON.recover(before,phase['todo'],phase['calls'],phase['fn'],comparison,unit,
        'BB_BeardyBaldyMan',image,{'GetHealth':phase['hostContract']},phase['before'],phase['after'])
    result=dict(phase=phase,unit=unit,image=image,before=before,source=corrected,
        raw=image.bytes_at(int(phase['fn']['address'],16),int(phase['fn']['bodyEndExclusive'],16)-int(phase['fn']['address'],16)),
        primitives=read(F/'primitives.json'))
    # Every distinct native/frame/profile tuple is actually proved; repeated
    # immutable tuples reuse that proof during source mutation controls.
    original_frame,original_origin=S.FRAME.prove,S.ORIGIN.prove
    frames,origins={},{}
    def frame(raw,fn,image,row,**kw):
        key=(raw,serial(fn),id(image),serial(row),serial(kw))
        if key not in frames:frames[key]=original_frame(raw,fn,image,row,**kw)
        return frames[key]
    def origin(image,fn,raw,profile):
        key=(id(image),serial(fn),raw,serial(profile))
        if key not in origins:origins[key]=original_origin(image,fn,raw,profile)
        return origins[key]
    S.FRAME.prove,S.ORIGIN.prove=frame,origin
    yield result
    S.FRAME.prove,S.ORIGIN.prove=original_frame,original_origin



def context(data):
    p=data['phase']
    return dict(input=dict(source=data['before'],todo=p['todo'],calls=p['calls']),
        todo=p['todo'],calls=p['calls'],unit=copy.deepcopy(data['unit']),owner='BB_BeardyBaldyMan',
        image=data['image'],manifest={'GetHealth':p['hostContract']})


def correspondence(data,ctx=None,source=None,spacing=True):
    p=data['phase']
    return S.COPY.prove(p['fn'],p['before'],p['after'],data['source'] if source is None else source,
        allow_wrapped_call_spacing=spacing,prior_comparison=context(data) if ctx is None else ctx)


def recover(data,*,prior=True,flag=True,source=None):
    p=data['phase'];unit=copy.deepcopy(data['unit']);row=unit['reviewedRegisterResourceHealthLifetimes'][p['fn']['address'].lower()]
    row['replayPriorRegisterComparison']=flag
    kwargs=dict(prior_register_comparison_input=context(data)['input']) if prior else {}
    return S.MODULE.recover(data['source'] if source is None else source,p['todo'],p['calls'],p['fn'],row,unit,
        'BB_BeardyBaldyMan',data['image'],{'GetHealth':p['hostContract']},p['before'],p['after'],**kwargs)


def test_prior_replay_maps_all_twenty_four_prefixes(data):
    proof=correspondence(data)
    assert proof['nativeSites']==proof['emittedUses']==24
    inverse=[s for s in proof['scopes'] if 'priorComparisonReview' in s]
    assert len(inverse)==1 and int(inverse[0]['native']['call']['site'],16)==0xe52447
    for s in proof['scopes']:
        assert data['source'][s['start']:s['end']]==s['prefix']
    assert inverse[0]['condition']=='not (fret_10 > fVar4)'


def test_default_copy_still_rejects_repaired_inverse(data):
    p=data['phase']
    with pytest.raises(ValueError,match='Lua condition shape'):
        S.COPY.prove(p['fn'],p['before'],p['after'],data['source'],allow_wrapped_call_spacing=True)


def test_default_spacing_guard_still_rejects_even_with_prior_review(data):
    with pytest.raises(ValueError,match='binding/normalization correspondence'):
        correspondence(data,spacing=False)


@pytest.mark.parametrize('fault',['missing_review','wrong_input','wrong_todo','wrong_calls','wrong_native_exit','wrong_owner'])
def test_prior_review_forgery_rejected(data,fault):
    c=context(data)
    if fault=='missing_review':c['unit'].pop('reviewedRegisterHealthComparisons')
    elif fault=='wrong_input':c['input']['source']+='\n'
    elif fault=='wrong_todo':c['input']['todo']=c['input']['todo']+['forged']
    elif fault=='wrong_calls':c['input']['calls']=c['input']['calls']+['GetHealth']
    elif fault=='wrong_owner':c['owner']='Other'
    else:c['unit']['reviewedRegisterHealthComparisons']['0x00e50fd0']['scopes'][0]['predicate']['falseExit']='0xe52474'
    with pytest.raises((ValueError,AssertionError)):
        correspondence(data,ctx=c)


@pytest.mark.parametrize('replacement',['\n','-- outside change\n'])
def test_current_phase_change_outside_comparison_rejected(data,replacement):
    with pytest.raises(ValueError,match='replay output'):
        correspondence(data,source=data['source']+replacement)


def test_complete_recovery_keeps_reviewed_inverse_and_both_liveness_proofs(data):
    out,todo,calls,reviews=recover(data)
    assert out==(F/'expected_phase.lua').read_text(encoding='utf-8')
    assert todo==data['phase']['todo']
    assert out.count('if not resources:ResourceThingHealthAbove(')==1
    assert out.count('resources:ResourceThingHealthAbove(')==len(reviews)==24
    assert sum(len(r['deadValues']) for r in reviews)==sum(len(r['composedDeadValues']) for r in reviews)==96
    assert reviews[0]['reviewedPriorRegisterComparison']['reviews'][0]['nativeFalseLabel']=='LAB_00e52d63'
    for review in reviews:
        footprint=review['native']['localResourceFootprints']
        assert footprint['resource']==[-524,-508]
        a,b=footprint['thing'];assert b-a==12 and (b<=-524 or -508<=a)


@pytest.mark.parametrize('prior,flag',[(False,True),(True,False),(True,1)])
def test_prior_lowering_opt_in_cannot_be_bypassed(data,prior,flag):
    with pytest.raises(ValueError,match='prior comparison'):
        recover(data,prior=prior,flag=flag)


def test_original_inverse_without_prior_review_still_rejects(data):
    p=data['phase'];unit=copy.deepcopy(data['unit'])
    row=unit['reviewedRegisterResourceHealthLifetimes']['0x00e50fd0']
    row['replayPriorRegisterComparison']=False
    row['luaPhaseSha256']=S.MODULE.digest(data['before'].encode())
    with pytest.raises(ValueError,match='owned resource/source ordered scopes'):
        S.MODULE.recover(data['before'],p['todo'],p['calls'],p['fn'],row,unit,'BB_BeardyBaldyMan',
            data['image'],{'GetHealth':p['hostContract']},p['before'],p['after'])


@pytest.mark.parametrize('target',['0x7e7390','0x7e73d0','0x7e7450','0x4aabd0','0x4aad40','0x4aad20','0x4aab30','0x4aa980','0x7e72f0'])
def test_nullable_wrapper_conditions_remain_explicit(data,target):
    p=data['primitives']['conditionalNonNullWrappers'][target]
    result=S.NULLABLE.prove(data['image'],int(target,16),p)
    assert result['purgeBytes']==p['purgeBytes']
    assert result['implementationVerified'] is result['closedAllPathReturnProof'] is False
    assert result['nonNullChildABIRequired']['stackPurge']==p['purgeBytes']


@pytest.mark.parametrize('field,value',[('receiverOffset',12),('implementationSlot','0xfc'),('purgeBytes',4),
    ('implementationVerified',True),('closedAllPathReturnProof',True)])
def test_nullable_forged_offset_abi_or_certification_rejected(data,field,value):
    p=copy.deepcopy(data['primitives']['conditionalNonNullWrappers']['0x7e7390']);p[field]=value
    with pytest.raises(ValueError):S.NULLABLE.prove(data['image'],0x7e7390,p)


def test_initial_entity_layout_retains_uncertified_tail_and_lifetime(data):
    result=S.ENTITY.prove(data['image'],data['unit']['entities']['BB_BeardyBaldyMan'],data['primitives']['entityLayout'])
    assert result['allocationBytes']==36 and result['checkedInitialBytes']==28
    assert result['currentTypeAndLifetimeVerified'] is False


@pytest.mark.parametrize('field,value',[('allocationBytes',32),('entityThingOffset',12),('interfaceOffset',8),
    ('parentPointerOffset',24),('parentInterfaceOffset',68),('parentGetterAddress','0xe50884')])
def test_initial_entity_layout_forgery_rejected(data,field,value):
    p=copy.deepcopy(data['primitives']['entityLayout']);p[field]=value
    with pytest.raises(ValueError):S.ENTITY.prove(data['image'],data['unit']['entities']['BB_BeardyBaldyMan'],p)


def test_initial_local_resource_does_not_claim_acquisition_or_current_lifetime(data):
    result=S.ORIGIN.prove(data['image'],data['phase']['fn'],data['raw'],data['primitives']['resourceInitialization'])
    assert result['resourceEntryOffset']==-524 and result['resourceBytes']==16
    assert result['hostNewResourceImplementationVerified'] is result['currentLifetimeVerified'] is False


@pytest.mark.parametrize('field,value',[('resourceEntryOffset',-520),('resourceBytes',12),
    ('resourceVtable','0x1270950'),('prefixSha256','0'*64),('prefixEnd','0xe50fd1')])
def test_resource_initialization_forgery_rejected(data,field,value):
    p=copy.deepcopy(data['primitives']['resourceInitialization']);p[field]=value
    with pytest.raises((ValueError,AssertionError)):
        S.ORIGIN.prove(data['image'],data['phase']['fn'],data['raw'],p)


@pytest.mark.parametrize('target',['0xbfe9bc','0xbfea1a'])
def test_import_new_and_delete_remain_conditional_cdecl_zero(data,target):
    result=S.EXTERNAL.prove(data['image'],int(target,16),data['primitives']['externalNormalABIs'][target])
    assert result['purgeBytes']==0 and result['implementationVerified'] is False


@pytest.mark.parametrize('field,value',[('iatSlot',0x1440164),('symbol','??3@YAXPAX@Z'),('module','other.dll'),
    ('purgeBytes',4),('implementationVerified',True),('closedAllPathReturnProof',True)])
def test_import_new_wrong_identity_or_overclaim_rejected(data,field,value):
    p=copy.deepcopy(data['primitives']['externalNormalABIs']['0xbfea1a']);p[field]=value
    with pytest.raises(ValueError):S.EXTERNAL.prove(data['image'],0xbfea1a,p)


def test_delete_import_identity_and_conditions(data):
    result=S.EXTERNAL.prove(data['image'],0xbfe9bc,data['primitives']['externalNormalABIs']['0xbfe9bc'])
    assert result['symbol']=='??3@YAXPAX@Z' and result['normalABIRequired']['stackPurge']==0
    assert result['implementationVerified'] is result['closedAllPathReturnProof'] is False


@pytest.fixture(scope='module')
def bounded_window(data):
    row=read(F/'row.json');p=next(p for p in row['nativeProfiles'] if 'callWindow' in p)
    frame,report=S.FRAME.prove(data['raw'],data['phase']['fn'],data['image'],row['ownedLocalResourceFrame'],
        entity=data['unit']['entities']['BB_BeardyBaldyMan'],scope_start=int(row['scopeStart'],16),
        scope_end=int(data['phase']['fn']['bodyEndExclusive'],16),frame_depth=row['frameDepth'],
        this_definition=int(row['thisDefinition'],16),owner_register='esi')
    pred=S.MODULE.prove_predicate(frame,int(p['call'],16),data['image'].bytes_at,0x4aa840,
        positive_exit=p['positiveExit'],false_exit=p['falseExit'])
    return frame,p,pred,int(row['thisDefinition'],16)


def window_roles(data,context,*,window=True,owner=None,slot=0x420):
    frame,p,pred,this=context
    options={} if window is False else dict(call_window=p['callWindow'] if window is True else window)
    return S.MODULE.prove_roles(frame,int(p['call'],16),pred,data['image'],getter_target=0x7e7490,
        this_definition=this if owner is None else owner,health_slot=slot,**options)


def test_bounded_window_only_claims_fresh_load_correspondence(data,bounded_window):
    proof=window_roles(data,bounded_window)
    assert proof['query']=='0xe53251' and proof['resourceEntryOffset']==-524
    assert proof['boundedCallWindow']['freshInterfaceAndTableLoads'] is True
    assert proof['boundedCallWindow']['earlierHeapAliasEqualityAssumed'] is False


def test_without_window_earlier_aliases_are_not_assumed_equal(data,bounded_window):
    with pytest.raises(ValueError):window_roles(data,bounded_window,window=False)


@pytest.mark.parametrize('fault',['pin','start','bounds','extra','owner','slot'])
def test_bounded_window_forgery_rejected(data,bounded_window,fault):
    window=copy.deepcopy(bounded_window[1]['callWindow']);kw={}
    if fault=='pin':window['nativeSha256']='0'*64
    elif fault=='start':window['start']=hex(int(window['start'],16)+1)
    elif fault=='bounds':window['start']='0x1'
    elif fault=='extra':window['earlierHeapAliasEqualityAssumed']=True
    elif fault=='owner':kw['owner']=bounded_window[3]+1
    else:kw['slot']=0x424
    with pytest.raises(ValueError):window_roles(data,bounded_window,window=window,**kw)

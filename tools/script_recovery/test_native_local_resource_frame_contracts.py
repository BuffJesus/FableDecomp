from pathlib import Path
import copy,json,sys
import pytest
W=Path(__file__).resolve().parent
R=W.parents[1]
from types import SimpleNamespace
from tools.script_recovery import native_local_resource_frame_contracts as FRAME
from tools.script_recovery.lift_native_lua import RData
S=SimpleNamespace(FRAME=FRAME)
read=lambda p:json.loads(p.read_text())
@pytest.fixture(scope='module')
def data():
    p=read(W/'fixtures/beardy_ordered_comparison/phase.json')
    unit=read(R/'refs/script_recovery/beardy_baldy/units/V_BeardyBaldy.json')
    row=read(W/'fixtures/beardy_resource_health/row.json')
    image=RData()
    if not image.ok:pytest.skip('original retail image unavailable for native frame proof')
    start,end=int(p['fn']['address'],16),int(p['fn']['bodyEndExclusive'],16)
    return dict(raw=image.bytes_at(start,end-start),fn=p['fn'],image=image,row=row['ownedLocalResourceFrame'],entity=unit['entities']['BB_BeardyBaldyMan'],scope_start=int(row['scopeStart'],16),scope_end=end,frame_depth=row['frameDepth'],this_definition=int(row['thisDefinition'],16),owner_register='esi')

def fresh(data):
    return {k:(v if k=='image' else copy.deepcopy(v)) for k,v in data.items()}
def test_frame_derives_all_calls_and_retains_unresolved_targets(data):
    frame,report=S.FRAME.prove(**fresh(data))
    assert report['reachableInstructions']==2842 and report['callCount']==570 and report['knownStaticProfileCalls']==567
    assert set(report['requiredIndirectABI'])=={'0xe5342e','0xe53507','0xe53531'}
    assert all(r['requiredNormalPurge']==0 and r['targetProvenanceVerified'] is False for r in report['requiredIndirectABI'].values())
    assert report['correctedExportDepths']==[] and report['sourceCallEvidenceUnchanged']
    assert all(r['currentVtableLifetimeVerified'] is False and r['opaqueChildStackBalanceAssumed'] is True for r in report['callReviews'])
    assert all(r['implementationVerified'] is False for r in report['externalNormalABIConditions'].values())
def test_export_depths_are_not_stack_anchors(data):
    a=fresh(data);b=fresh(data)
    for c in b['fn']['calls']+b['fn']['indirectCalls']:c['depth']+=4096
    fa,ra=S.FRAME.prove(**a);fb,rb=S.FRAME.prove(**b)
    assert fa['depths']==fb['depths'] and len(rb['correctedExportDepths'])==570
@pytest.mark.parametrize('fault',['missing_call','duplicate_call','instruction','target','receiver','purge','bounded_hash','missing_bounded','unknown_purge','unknown_claim','missing_unknown','interface','native','external_claim','nullable_claim'])
def test_forged_frame_or_call_profile_rejected(data,fault):
    d=fresh(data);r=d['row'];records=r['callContracts']
    if fault=='missing_call':records.pop()
    elif fault=='duplicate_call':records.append(copy.deepcopy(records[0]))
    elif fault=='instruction':records[0]['instructionBytes']='90'
    elif fault=='target':records[0]['target']='0x1'
    elif fault=='receiver':records[0]['receiverValueEquivalenceProved']=not records[0]['receiverValueEquivalenceProved']
    elif fault=='purge':records[0]['purgeBytes']+=4
    elif fault=='bounded_hash':next(iter(r['boundedReturnRegions'].values()))[0]['sha256']='0'*64
    elif fault=='missing_bounded':r['boundedReturnRegions'].pop(next(iter(r['boundedReturnRegions'])))
    elif fault=='unknown_purge':r['requiredIndirectABI']['0xe5342e']['requiredNormalPurge']=4
    elif fault=='unknown_claim':r['requiredIndirectABI']['0xe5342e']['targetProvenanceVerified']=True
    elif fault=='missing_unknown':r['requiredIndirectABI'].pop('0xe5342e')
    elif fault=='interface':r['cacheProof']['interfaceOffset']=8
    elif fault=='native':d['raw']=b'\x90'+d['raw'][1:]
    elif fault=='external_claim':r['externalNormalABIs']['0xbfea1a']['implementationVerified']=True
    else:r['conditionalNonNullWrappers']['0x7e7390']['closedAllPathReturnProof']=True
    with pytest.raises((ValueError,AssertionError)):S.FRAME.prove(**d)

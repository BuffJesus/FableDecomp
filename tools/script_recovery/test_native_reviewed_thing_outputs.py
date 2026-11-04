import copy
import json
from pathlib import Path
import pytest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_reviewed_thing_outputs import restore
R=Path(__file__).resolve().parents[2]
W=R/'tools/script_recovery/fixtures/trader_owned_outputs'
@pytest.fixture(scope='module')
def evidence():
    export=json.loads((R/'refs/script_recovery/trader_escort/translation_unit_typed.json').read_text(encoding='utf-8-sig'))
    fn=next(f for f in export['functions']if int(f['address'],16)==0xe06050)
    return fn,json.loads((R/'refs/script_recovery/trader_escort/units/Q_TraderEscort.json').read_text(encoding='utf-8-sig'))['reviewedThingOutputScopes']['0x00e06050'],(W/'lowering_input.lua').read_text(),RData()
def test_cleanup_boundaries(evidence):
    fn,row,source,data=evidence
    output,todo,calls=restore(source,['retained'],['prior'],fn,row,data,root=R)
    assert output==(W/'lowering_output.lua').read_text()
    assert todo==['retained'] and calls==['prior','TrackThingOutput','RetireThingOutput']
    assert output.count('resources:TrackThingOutput(pCVar8)')==2
    assert output.count('resources:RetireThingOutput(surpriseNearestOwner)')==2
    assert output.index('resources:RetireThingOutput(surpriseActorOwner)')<output.index('::LAB_00e06364::')
    assert 'TrackThingOutput(quest:GetHero' not in output
def test_unreviewed_no_reads():
    class Never:
        def bytes_at(self,*args):raise AssertionError('native read')
    assert restore('source',['todo'],['call'],{},None,Never())==('source',['todo'],['call'])
@pytest.mark.parametrize('change',['extent','export','native','anchor','outside_anchor','dependency','sidecar','intermediate','output','actor','nearest','comment','target','decrement'])
def test_changed_evidence_rejected(evidence,change):
    fn,row,source,data=evidence
    fn,row=copy.deepcopy(fn),copy.deepcopy(row)
    if change=='extent':fn['bodyEndExclusive']='0xe06440'
    elif change=='export':fn['decompile']+='\n'
    elif change=='native':row['nativeSha256']='0'*64
    elif change=='anchor':row['anchors'][next(iter(row['anchors']))]='00'*12
    elif change=='outside_anchor':row['anchors']['0x4aa840']='00'
    elif change=='dependency':next(iter(row['dependencies'].values()))['sha256']='0'*64
    elif change=='sidecar':next(iter(row['sidecar'].values()))['sha256']='0'*64
    elif change=='intermediate':source+='\n'
    elif change=='output':row['recoveredSha256']='0'*64
    else:
        index={'actor':1,'nearest':2,'comment':3,'target':4,'decrement':5}[change]
        row['operations'][index]['before']+='CHANGED'
    with pytest.raises(ValueError,match='reviewed Thing outputs'):
        restore(source,[],[],fn,row,data,root=R)

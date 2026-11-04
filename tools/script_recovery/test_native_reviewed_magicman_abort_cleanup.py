import copy,hashlib,json,sys
from pathlib import Path
import pytest
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R))
from tools.script_recovery.native_reviewed_cleanup_operands import restore
from tools.script_recovery.convert_quest_unit import UnitConverter
SOURCE=(W/'testdata/magicman_abort_cleanup.lua').read_text();ROW=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedCleanupOperands']['0x00e40e80']
@pytest.fixture(scope='module')
def evidence():
    c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json')
    return c.native('0x00e40e80'),c.rdata
def test_exact_native_cleanup_and_no_review_noop(evidence):
    fn,rd=evidence;edit=ROW['replacements'][0]
    assert edit['count']==1 and SOURCE.count(edit['old'])==1
    expected=SOURCE
    for replacement in ROW['replacements']:
        assert replacement['count']==1 and expected.count(replacement['old'])==1
        expected=expected.replace(replacement['old'],replacement['new'],1)
    assert restore(SOURCE,fn,ROW,rd)==expected
    assert restore(SOURCE,fn,None,rd)==SOURCE
    for operation in ['resources:DestroyMovie(xStack_340)','resources:ReleaseResource(xStack_364)']:
        delta=sum(edit['count']*(edit['new'].count(operation)-edit['old'].count(operation)) for edit in ROW['replacements'])
        assert expected.count(operation)==SOURCE.count(operation)+delta
    with pytest.raises(ValueError,match='source phase'):restore(expected,fn,ROW,rd)
@pytest.mark.parametrize('field',['nativeSha256','sourceSha256','luaSha256'])
def test_changed_inputs_rejected(evidence,field):
    fn,rd=evidence;row=copy.deepcopy(ROW);row[field]='0'*64
    with pytest.raises(ValueError):restore(SOURCE,fn,row,rd)
@pytest.mark.parametrize('mutation',['witness','begin','end','count','actor','hero','jump'])
def test_ambiguous_or_changed_cleanup_rejected(evidence,mutation):
    fn,rd=evidence;row=copy.deepcopy(ROW);source=SOURCE
    if mutation=='witness':row['nativeWitnesses'][0]['sha256']='0'*64
    elif mutation=='begin':row['spanBegin']='MISSING_BEGIN'
    elif mutation=='end':row['spanEnd']='MISSING_END'
    elif mutation=='count':row['replacements'][0]['count']=2
    else:
        old=row['replacements'][0]['old'];new=old.replace('DeregisterTimer(me)','DeregisterTimer(otherActor)') if mutation=='actor' else old.replace('DeregisterTimer(iVar18)','DeregisterTimer(otherHero)') if mutation=='hero' else old.replace('LAB_00e446ea','LAB_00e4468c')
        source=source.replace(old,new,1);row['luaSha256']=hashlib.sha256(source.encode()).hexdigest()
    with pytest.raises(ValueError):restore(source,fn,row,rd)

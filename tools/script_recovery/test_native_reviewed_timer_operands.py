import copy,hashlib,json,sys
from pathlib import Path
import pytest
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R))
from tools.script_recovery.native_reviewed_cleanup_operands import restore
from tools.script_recovery.convert_quest_unit import UnitConverter
SOURCE=(W/'testdata/madame_timer_operands.lua').read_text();ROW=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedTimerOperands']['0x00e3bb70']
@pytest.fixture(scope='module')
def evidence():
    c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json')
    return c.native('0x00e3bb70'),c.rdata
def test_exact_native_backed_restoration(evidence):
    fn,rd=evidence
    expected=SOURCE.replace('quest:GetTimer(4)','quest:GetTimer(i_stk_1a8)',1).replace('quest:SetTimer(4, fVar19)','quest:SetTimer(i_stk_1a8, 4)',1)
    assert restore(SOURCE,fn,ROW,rd)==expected
    assert restore(SOURCE,fn,None,rd)==SOURCE
    with pytest.raises(ValueError,match='source phase'):restore(expected,fn,ROW,rd)
@pytest.mark.parametrize('field',['nativeSha256','sourceSha256','luaSha256'])
def test_changed_hash_rejected(evidence,field):
    fn,rd=evidence;row=copy.deepcopy(ROW);row[field]='0'*64
    with pytest.raises(ValueError):restore(SOURCE,fn,row,rd)
@pytest.mark.parametrize('mutation',['witness','begin','end','count','query','reset'])
def test_ambiguous_review_rejected(evidence,mutation):
    fn,rd=evidence;row=copy.deepcopy(ROW);source=SOURCE
    if mutation=='witness':row['nativeWitnesses'][0]['sha256']='0'*64
    elif mutation=='begin':row['spanBegin']='MISSING_BEGIN'
    elif mutation=='end':row['spanEnd']='MISSING_END'
    elif mutation=='count':row['replacements'][0]['count']=2
    elif mutation=='query':source=source.replace('quest:GetTimer(4)','quest:GetTimer(otherTimer)',1)
    else:source=source.replace('quest:SetTimer(4, fVar19)','quest:SetTimer(4, otherDuration)',1)
    row['luaSha256']=hashlib.sha256(source.encode()).hexdigest()
    with pytest.raises(ValueError):restore(source,fn,row,rd)

import copy,hashlib,json
from pathlib import Path
import pytest
from tools.script_recovery.native_reviewed_cleanup_operands import restore
R=Path(__file__).resolve().parents[2]
SOURCE=(R/'tools/script_recovery/testdata/madame_cleanup_operands.lua').read_text()
ROW=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedCleanupOperands']['0x00e3bb70']
class Reader:
    def bytes_at(self,a,n):return b'x'*n
def inputs():
    row=copy.deepcopy(ROW)
    row['nativeSha256']=hashlib.sha256(b'x'*16).hexdigest()
    row['sourceSha256']=hashlib.sha256(b'fixture').hexdigest()
    for w in row['nativeWitnesses']:w['sha256']=hashlib.sha256(b'x'*w['size']).hexdigest()
    return dict(address='0x1000',bodyEndExclusive='0x1010',decompile='fixture'),row
def test_only_bounded_operands_change():
    fn,row=inputs();out=restore(SOURCE,fn,row,Reader())
    a=SOURCE.index(row['spanBegin']);b=SOURCE.index(row['spanEnd'])
    assert out[:a]==SOURCE[:a]
    assert out[out.index(row['spanEnd']):]==SOURCE[b:]
    span=out[a:out.index(row['spanEnd'])]
    assert span.count('resources:DestroyMovie(xStack_dc)')==2
    assert 'quest:DeregisterTimer(i_stk_1a8)' in span
    assert 'resources:ReleaseResource(madameResource)' in span
    assert 'pCVar10 = xStack_dc\n                    goto LAB_00e3e2ea' in span
def test_unreviewed_unchanged():
    assert restore(SOURCE,{},None,None)==SOURCE
@pytest.mark.parametrize('field',['nativeSha256','sourceSha256','luaSha256'])
def test_changed_hash_rejected(field):
    fn,row=inputs();row[field]='bad'
    with pytest.raises(ValueError):restore(SOURCE,fn,row,Reader())
def test_changed_witness_rejected():
    fn,row=inputs();row['nativeWitnesses'][0]['sha256']='bad'
    with pytest.raises(ValueError):restore(SOURCE,fn,row,Reader())
@pytest.mark.parametrize('mutation',['label','count','operand','selector'])
def test_ambiguous_source_rejected_even_if_phase_resealed(mutation):
    fn,row=inputs();s=SOURCE
    if mutation=='label':s=s.replace(row['spanBegin'],'::OTHER::')
    if mutation=='count':row['replacements'][0]['count']=1
    if mutation=='operand':s=s.replace('resources:DestroyMovie(xStack_cc)','resources:DestroyMovie(other)',1)
    if mutation=='selector':s=s.replace('                    goto LAB_00e3e2ea','                    goto OTHER',1)
    row['luaSha256']=hashlib.sha256(s.encode()).hexdigest()
    with pytest.raises(ValueError):restore(s,fn,row,Reader())


def test_original_native_evidence_and_fixture():
    from tools.script_recovery.convert_quest_unit import UnitConverter
    converter=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json')
    fn=converter.native('0x00e3bb70')
    changed=restore(SOURCE,fn,ROW,converter.rdata)
    assert changed!=SOURCE
    with pytest.raises(ValueError,match='source phase'):
        restore(changed,fn,ROW,converter.rdata)

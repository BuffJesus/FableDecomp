import copy,hashlib,json
from pathlib import Path
import pytest
from tools.script_recovery.native_reviewed_loop_termination import restore

ROOT=Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def retail():
    from tools.script_recovery.convert_quest_unit import UnitConverter
    c=UnitConverter(ROOT/'refs/script_recovery/bandit_camp/translation_unit_typed.json')
    fn=c.native('0x00d0fe80')
    config=json.loads((ROOT/'refs/script_recovery/bandit_camp/units/Q_BanditCampBossBattle.json').read_text())
    row=config['reviewedLoopHeadTermination']['0x00d0fe80']
    return fn,row,c.rdata


def test_retail_guard_moves_once_before_first_element_and_keeps_body(retail):
    fn,row,data=retail;source=row['sourceLoop']
    result=restore(source,fn,row,data)
    guard=row['sourceGuard'];loop=row['sourceLoop'];header='    while true do\n'
    expected=header+guard+loop[len(header):].removesuffix(guard+'    end')+'    end'
    assert result.replace('\r\n','\n')==source.replace('\r\n','\n').replace(loop,expected)
    assert result.count('quest:IsActiveThreadTerminating')==source.count('quest:IsActiveThreadTerminating')


@pytest.mark.parametrize('ending',['\n','\r\n'])
def test_source_line_endings_are_preserved(retail,ending):
    fn,row,data=retail;source=row['sourceLoop'].replace('\n',ending)
    result=restore(source,fn,row,data)
    assert result.startswith(('    while true do\n'+row['sourceGuard']).replace('\n',ending))
    assert result.count(ending)==source.count(ending)


def test_unreviewed_function_is_unchanged(retail):
    fn,row,data=retail
    assert restore('anything',fn,None,data)=='anything'


@pytest.mark.parametrize('mutation', ['body_hash','source_hash','empty_branch','backedge','poll','cleanup','owner',
                                     'guard','duplicate','source_body','missing_anchor','loop_shape'])
def test_changed_evidence_or_source_is_rejected(retail,mutation):
    fn,row,data=retail;fn=copy.deepcopy(fn);row=copy.deepcopy(row);source=row['sourceLoop']
    if mutation=='body_hash':row['nativeSha256']='0'*64
    elif mutation=='source_hash':fn['decompile']+='\n'
    elif mutation=='empty_branch':row['emptyBranch']='0x00d108e7'
    elif mutation=='backedge':row['backedge']='0x00d10895'
    elif mutation=='poll':row['pollSite']='0x00d108b2'
    elif mutation=='cleanup':row['cleanupTarget']='0x00d108eb'
    elif mutation=='owner':row['ownerRegister']='ebp'
    elif mutation=='guard':row['sourceGuard']=row['sourceGuard'].replace('LAB_00d108f6','LAB_00d108eb')
    elif mutation=='duplicate':source+='\n'+source
    elif mutation=='source_body':source=source.replace('uVar9 + 1','uVar9 + 2')
    elif mutation=='missing_anchor':row['nativeInstructions'].pop('0x00d108a2')
    elif mutation=='loop_shape':row['sourceLoop']=row['sourceLoop'].replace('while true do','while ready do')
    with pytest.raises(ValueError):restore(source,fn,row,data)


@pytest.mark.parametrize('site,replacement', [
    (0x00d10899, bytes.fromhex('e8a170faff')), # different direct callee
    (0x00d1089e, bytes.fromhex('84db')),       # tests BL instead of AL
    (0x00d108a0, bytes.fromhex('7454')),       # opposite exit polarity
    (0x00d108e7, bytes.fromhex('73ae')),       # opposite count backedge
    (0x00d10897, bytes.fromhex('8bc7')),       # wrong receiver register
])
def test_semantic_native_guards_survive_rehashed_witnesses(retail,site,replacement):
    fn,row,data=retail;row=copy.deepcopy(row)
    start=int(fn['address'],16);raw=bytearray(data.bytes_at(start,int(fn['bodyEndExclusive'],16)-start))
    raw[site-start:site-start+len(replacement)]=replacement;raw=bytes(raw)
    row['nativeSha256']=hashlib.sha256(raw).hexdigest();row['nativeInstructions'][f'0x{site:08x}']=replacement.hex()
    class Changed:
        def bytes_at(self,address,size):
            if address==start and size==len(raw):return raw
            return data.bytes_at(address,size)
    with pytest.raises(ValueError):restore(row['sourceLoop'],fn,row,Changed())

from copy import deepcopy
import json
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_movie_phi_cleanup import fold
from tools.script_recovery.native_reviewed_movie_members import restore

ROOT=Path(__file__).resolve().parents[2]
SOURCE='''CPhysicsMeshInfo *joined;
xStack_30 = &PTR_ctor_01260ef4;
xStack_50[0] = &PTR_ctor_01260ef4;
if (first) {
 Release(&xStack_28);
 xStack_30 = &PTR_base_0126008c;
 joined = (CPhysicsMeshInfo *)&xStack_30;
}
else {
 Release(&xStack_48);
 xStack_50[0] = &PTR_base_0126008c;
 joined = (CPhysicsMeshInfo *)xStack_50;
}
BaseDtor(joined);
'''
LABELS={'Release':0x6e7ab0,'BaseDtor':0x99a430}

@pytest.mark.parametrize('newline',['\n','\r\n'])
def test_each_branch_destroys_its_own_movie(newline):
    result=fold(SOURCE.replace('\n',newline),LABELS)
    assert result.count('RESOURCE_DestroyMovie(xStack_30);')==1
    assert result.count('RESOURCE_DestroyMovie(xStack_50);')==1
    assert 'BaseDtor(joined)' not in result and 'Release(' not in result
    assert 'if (first)' in result and 'else' in result
    assert fold(result,LABELS)==result

@pytest.mark.parametrize('old,new',[
    ('01260ef4','0127094c'),
    ('&xStack_28','&xStack_24'),
    ('BaseDtor(joined);','Use(joined);\nBaseDtor(joined);'),
    ('BaseDtor(joined);','BaseDtor(other);'),
    ('CPhysicsMeshInfo *joined;','int joined;'),
    ('joined = (CPhysicsMeshInfo *)&xStack_30;','joined = missing;'),
])
def test_unproven_join_remains_unmodified(old,new):
    source=SOURCE.replace(old,new)
    assert fold(source,LABELS)==source

@pytest.mark.parametrize('key',['Release','BaseDtor'])
def test_untrusted_helper_name_does_not_establish_identity(key):
    labels=dict(LABELS);labels[key]=0x123456
    assert fold(SOURCE,labels)==SOURCE

@pytest.fixture(scope='module')
def evidence():
    c=UnitConverter(ROOT/'refs/script_recovery/bandit_camp/translation_unit_typed.json')
    fn=c.native('0x00d09010')
    row=json.loads((ROOT/'refs/script_recovery/bandit_camp/units/Q_BanditCampBossBattle.json').read_text())['reviewedMovieMembers']['0x00d09010']
    release=next(x['currentName'] for x in fn['calls'] if int(x['target'],16)==0x6e7ab0)
    base=next(x['currentName'] for x in fn['calls'] if int(x['target'],16)==0x99a430)
    lines=['CPhysicsMeshInfo *joined;']
    for pair in row['sharedBaseDestructors']:
        obj=pair['object'];lines.append(f'{obj} = &PTR_ctor_01260ef4;')
        for site in pair['releaseSites']:
            lines += [f'{release}(&{pair["member"]});',f'{obj} = &PTR_base_0126008c;',f'joined = (CPhysicsMeshInfo *)&{obj};']
    lines += [f'{base}(joined);']*len(row['baseDestructorSites'])
    return '\n'.join(lines)+'\n',fn,row,c.rdata

def test_native_review_covers_all_six_releases(evidence):
    source,fn,row,data=evidence
    result=restore(source,fn,row,data)
    assert result.count('RESOURCE_DestroyMovie(')==6
    assert restore(source,fn,None,data)==source

@pytest.mark.parametrize('kind',['native','source','frame','slot','member','start','release','base','source_use','missing_release'])
def test_changed_reviewed_lifetime_is_rejected(evidence,kind):
    source,fn,row,data=evidence;row=deepcopy(row);pair=row['sharedBaseDestructors'][0]
    if kind=='native':row['nativeSha256']='0'*64
    elif kind=='source':row['sourceSha256']='0'*64
    elif kind=='frame':pair['frameBytes']+=4
    elif kind=='slot':pair['entrySlot']+=4
    elif kind=='member':pair['member']='xStack_8c'
    elif kind=='start':pair['startSites'][0]='0x00D098FC'
    elif kind=='release':pair['releaseSites'][0]='0x00D09A47'
    elif kind=='base':row['baseDestructorSites'][0]='0x00D098FC'
    elif kind=='source_use':source+='use(joined);\n'
    elif kind=='missing_release':source=source.replace('(&xStack_90);','(&unknown);',1)
    with pytest.raises(ValueError,match='reviewed movie members'):restore(source,fn,row,data)

import copy,hashlib,json,sys
from pathlib import Path
import pytest
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R))
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_cleanup_operands import restore
SOURCE=(W/'testdata/magicman_abort_cleanup.lua').read_text()
assert hashlib.sha256(SOURCE.encode()).hexdigest()=='86db0c247958215b521f45fdba2ba188d88e221924438c7ff869333c387af61b'
ROW=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedCleanupOperands']['0x00e40e80']
PRIOR=copy.deepcopy(ROW)
PRIOR['replacements']=PRIOR['replacements'][:3]
PRIOR['spanEnd']=next(line for line in SOURCE.splitlines() if line.strip()=='::LAB_00e4203f::')
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');return c.native('0x00e40e80'),c.rdata
def test_exact_shared_movie_operand_preserves_all_prior_cleanup(evidence):
 fn,rd=evidence;edit=ROW['replacements'][3]
 assert ROW['replacements'][:3]==PRIOR['replacements'] and len(ROW['replacements'])>=4
 baseline=restore(SOURCE,fn,PRIOR,rd)
 assert baseline.count(edit['old'])==1 and edit['count']==1
 expected=baseline
 for extension in ROW['replacements'][3:]:
  assert extension['count']==1 and expected.count(extension['old'])==1
  expected=expected.replace(extension['old'],extension['new'],1)
 assert restore(SOURCE,fn,ROW,rd)==expected and restore(SOURCE,fn,None,rd)==SOURCE
 assert expected.count('resources:DestroyMovie(xStack_340)')==baseline.count('resources:DestroyMovie(xStack_340)')+sum(e['new'].count('resources:DestroyMovie(xStack_340)')-e['old'].count('resources:DestroyMovie(xStack_340)') for e in ROW['replacements'][3:])
 assert expected.count('resources:DestroyMovie(xStack_330)')==baseline.count('resources:DestroyMovie(xStack_330)')-1
 assert expected.count('resources:ReleaseResource(xStack_364)')==baseline.count('resources:ReleaseResource(xStack_364)')
 assert expected.count('quest:DeregisterTimer(')==baseline.count('quest:DeregisterTimer(')
 with pytest.raises(ValueError,match='source phase'):restore(expected,fn,ROW,rd)
@pytest.mark.parametrize('field',['nativeSha256','sourceSha256','luaSha256'])
def test_changed_source_or_native_rejected(evidence,field):
 fn,rd=evidence;row=copy.deepcopy(ROW);row[field]='0'*64
 with pytest.raises(ValueError):restore(SOURCE,fn,row,rd)
@pytest.mark.parametrize('mutation',['witness','begin','end','count','label','movie','duplicate'])
def test_ambiguous_movie_tail_rejected(evidence,mutation):
 fn,rd=evidence;row=copy.deepcopy(ROW);source=SOURCE;old=row['replacements'][3]['old']
 if mutation=='witness':row['nativeWitnesses'][-1]['sha256']='0'*64
 elif mutation=='begin':row['spanBegin']='MISSING_BEGIN'
 elif mutation=='end':row['spanEnd']='MISSING_END'
 elif mutation=='count':row['replacements'][3]['count']=2
 else:
  new=old.replace('LAB_00e44615','LAB_00e44811') if mutation=='label' else old.replace('xStack_330','xStack_310') if mutation=='movie' else old+'\n'+old
  assert new!=old;source=source.replace(old,new,1);row['luaSha256']=hashlib.sha256(source.encode()).hexdigest()
 with pytest.raises(ValueError):restore(source,fn,row,rd)

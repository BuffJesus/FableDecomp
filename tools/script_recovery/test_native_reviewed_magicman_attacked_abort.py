import copy,hashlib,json,sys
from pathlib import Path
import pytest
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R))
from tools.script_recovery.native_reviewed_cleanup_operands import restore
from tools.script_recovery.convert_quest_unit import UnitConverter
SOURCE=(W/'testdata/magicman_abort_cleanup.lua').read_text()
assert hashlib.sha256(SOURCE.encode()).hexdigest()=='86db0c247958215b521f45fdba2ba188d88e221924438c7ff869333c387af61b'
ROW=json.loads((R/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())['reviewedCleanupOperands']['0x00e40e80']
PRIOR=copy.deepcopy(ROW);PRIOR['replacements']=PRIOR['replacements'][:1]
PRIOR['spanEnd']=next(line for line in SOURCE.splitlines() if line.strip()=='::LAB_00e41b75::')
@pytest.fixture(scope='module')
def evidence():
 c=UnitConverter(R/'refs/script_recovery/bordello/translation_unit_typed.json');return c.native('0x00e40e80'),c.rdata
def test_exact_three_blocks_preserves_prior_and_no_review(evidence):
 fn,rd=evidence;expected=SOURCE
 for edit in ROW['replacements']:
  assert edit['count']==1 and expected.count(edit['old'])==1
  expected=expected.replace(edit['old'],edit['new'],1)
 assert restore(SOURCE,fn,ROW,rd)==expected
 assert restore(SOURCE,fn,None,rd)==SOURCE
 assert ROW['replacements'][0]==PRIOR['replacements'][0]
 baseline=restore(SOURCE,fn,PRIOR,rd)
 assert baseline==SOURCE.replace(PRIOR['replacements'][0]['old'],PRIOR['replacements'][0]['new'],1)
 assert expected.count(PRIOR['replacements'][0]['new'])==baseline.count(PRIOR['replacements'][0]['new'])
 assert expected.count('resources:ReleaseResource(xStack_364)')==baseline.count('resources:ReleaseResource(xStack_364)')+2
 assert expected.count('resources:DestroyMovie(xStack_2f8)')==baseline.count('resources:DestroyMovie(xStack_2f8)')
 with pytest.raises(ValueError,match='source phase'):restore(expected,fn,ROW,rd)
@pytest.mark.parametrize('field',['nativeSha256','sourceSha256','luaSha256'])
def test_changed_inputs_rejected(evidence,field):
 fn,rd=evidence;row=copy.deepcopy(ROW);row[field]='0'*64
 with pytest.raises(ValueError):restore(SOURCE,fn,row,rd)
@pytest.mark.parametrize('block',[0,1,2])
@pytest.mark.parametrize('mutation',['witness','begin','end','count','actor','tail','duplicate'])
def test_ambiguous_or_changed_block_rejected(evidence,block,mutation):
 fn,rd=evidence;row=copy.deepcopy(ROW);source=SOURCE;old=row['replacements'][block]['old']
 if mutation=='witness':row['nativeWitnesses'][-1]['sha256']='0'*64
 elif mutation=='begin':row['spanBegin']='MISSING_BEGIN'
 elif mutation=='end':row['spanEnd']='MISSING_END'
 elif mutation=='count':row['replacements'][block]['count']=2
 else:
  if mutation=='actor':new=old.replace('DeregisterTimer(me)','DeregisterTimer(otherActor)')
  elif mutation=='tail':lines=old.splitlines();new='\n'.join(lines[:-1]+['CHANGED_'+lines[-1]])
  else:new=old+'\n'+old
  assert new!=old;source=source.replace(old,new,1);row['luaSha256']=hashlib.sha256(source.encode()).hexdigest()
 with pytest.raises(ValueError):restore(source,fn,row,rd)

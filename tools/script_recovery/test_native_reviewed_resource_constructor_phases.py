import copy,hashlib,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_resource_constructors import restore
@pytest.fixture(scope='module')
def guard():
 c=UnitConverter(ROOT/'refs/script_recovery/arena/translation_unit_typed.json');fn=c.native('0x00f1b9c0');row=json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())['reviewedInlinedResourceConstructors']['0x00f1b9c0']
 source='ppuVar20 = RESOURCE_NewResource();\nRESOURCE_PrepareResource(xStack_12c);\nb = RESOURCE_TryAcquire(xStack_12c, me, 4);\nCPhysicsMeshInfo::~CPhysicsMeshInfo((CPhysicsMeshInfo *)&xStack_12c);\nCPhysicsMeshInfo::~CPhysicsMeshInfo((CPhysicsMeshInfo *)&xStack_12c);\nRESOURCE_ReleaseResource(ppuVar20);\n'
 row['loweredSha256']=hashlib.sha256(source.encode()).hexdigest();return source,fn,row,c.rdata
@pytest.mark.parametrize('newline',['\n','\r\n'])
def test_resource_storage_and_both_frame_cleanup_phases(guard,newline):
 source,fn,row,rdata=guard;result=restore(source.replace('\n',newline),fn,row,rdata)
 assert result.startswith('xStack_12c = RESOURCE_NewResource();\nppuVar20 = xStack_12c;')
 assert result.count('RESOURCE_ReleaseResource(xStack_12c);')==2
 assert 'RESOURCE_ReleaseResource(ppuVar20);'in result
def test_no_review_no_change(guard):
 source,fn,_,rdata=guard;assert restore(source,fn,None,rdata)==source
@pytest.mark.parametrize('kind',['native','export','lowered','lateFrame','stackSave','nonDominatingSave','stackPop','frameSaveMismatch','lateLea','lateVtable','lateZeros','lateZeroDefs','lateCall','missingLate','duplicateLate','lateSource','sourceMissing','sourceExtra','sourceAlias','lateMetadataDepth','lateMetadataReceiver'])
def test_frame_phase_or_native_source_drift_rejected(guard,kind):
 source,original_fn,original_row,rdata=guard;fn=copy.deepcopy(original_fn);row=copy.deepcopy(original_row);late=row['additionalBaseDestructors'][0]
 if kind in('native','export','lowered'):row[{'native':'nativeSha256','export':'sourceSha256','lowered':'loweredSha256'}[kind]]='0'*64
 elif kind=='lateFrame':late['frameBytes']+=4
 elif kind=='stackSave':late['stackSave']='0xf1b9c8';row['nativeInstructions']['0xf1b9c8']='57'
 elif kind=='nonDominatingSave':late['stackSave']='0xf1c937';row['nativeInstructions']['0xf1c937']='55'
 elif kind=='stackPop':late['stackPop']='0xf1c9a9';row['nativeInstructions']['0xf1c9a9']='5f'
 elif kind=='frameSaveMismatch':late['stackSave']='0xf1b9d5'
 elif kind=='lateLea':late['lea']=row['earlyDestructor']['lea']
 elif kind=='lateVtable':late['vtable']=row['earlyDestructor']['vtable']
 elif kind=='lateZeros':late['zeros'].reverse()
 elif kind=='lateZeroDefs':late['zeroDefinitions']['0xf1c993']=[]
 elif kind=='lateCall':late['call']=row['earlyDestructor']['call']
 elif kind=='missingLate':row['additionalBaseDestructors']=[]
 elif kind=='duplicateLate':row['additionalBaseDestructors']*=2
 elif kind=='lateSource':late['sourceCall']='wrong();'
 elif kind.startswith('lateMetadata'):
  call=next(c for c in fn['calls']if int(c['site'],16)==int(late['call'],16))
  if kind=='lateMetadataDepth':call['depth']-=4
  else:call['ecxStack']+=4
 else:
  call=row['earlyDestructor']['sourceCall']
  if kind=='sourceMissing':source=source.replace(call,'',1)
  elif kind=='sourceExtra':source+=call+'\n'
  else:source=source.replace('ppuVar20 = RESOURCE_NewResource();','xStack_12c = RESOURCE_NewResource();')
  row['loweredSha256']=hashlib.sha256(source.encode()).hexdigest()
 with pytest.raises(ValueError,match='does not dominate'if kind=='nonDominatingSave'else None):restore(source,fn,row,rdata)

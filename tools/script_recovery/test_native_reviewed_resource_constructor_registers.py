import copy,hashlib,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_resource_constructors import restore
@pytest.fixture(scope='module')
def guard():
 c=UnitConverter(ROOT/'refs/script_recovery/arena/translation_unit_typed.json');fn=c.native('0x00f19bb0');row=json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())['reviewedInlinedResourceConstructors']['0x00f19bb0']
 source='ppuVar18 = RESOURCE_NewResource();\nRESOURCE_PrepareResource(xStack_c8);\nb = RESOURCE_TryAcquire(xStack_c8, me, 4);\nCPhysicsMeshInfo::~CPhysicsMeshInfo((CPhysicsMeshInfo *)&xStack_c8);\nRESOURCE_ReleaseResource(ppuVar18);\nRESOURCE_ReleaseResource(ppuVar18);\n'
 row['loweredSha256']=hashlib.sha256(source.encode()).hexdigest()
 return source,fn,row,c.rdata
@pytest.mark.parametrize('newline',['\n','\r\n'])
def test_stack_storage_and_both_exit_destructors(guard,newline):
 source,fn,row,rdata=guard;result=restore(source.replace('\n',newline),fn,row,rdata)
 assert result.startswith('xStack_c8 = RESOURCE_NewResource();\nppuVar18 = xStack_c8;')
 assert result.count('RESOURCE_ReleaseResource(xStack_c8);')==2
 assert 'RESOURCE_DestroyMovie('not in result
 assert 'RESOURCE_ReleaseResource(ppuVar18);'in result
def test_legacy_address_order_spelling(guard):
 # rows reviewed before callOrder pairing named the F1A793 print as DestroyMovie on the slot
 source,fn,row,rdata=guard;row=copy.deepcopy(row);part=row['resourceDestructorRewrites'][0]
 part['sourceCall']='RESOURCE_DestroyMovie(xStack_c8);';part.pop('sourceOccurrence');part.pop('sourceCallCount')
 source=source.replace('RESOURCE_ReleaseResource(ppuVar18);\n','RESOURCE_DestroyMovie(xStack_c8);\n',1);row['loweredSha256']=hashlib.sha256(source.encode()).hexdigest()
 result=restore(source,fn,row,rdata);assert result.count('RESOURCE_ReleaseResource(xStack_c8);')==2 and 'RESOURCE_DestroyMovie(' not in result
def test_no_review_no_recovery(guard):
 source,fn,_,rdata=guard;assert restore(source,fn,None,rdata)==source
@pytest.mark.parametrize('kind',['native','export','lowered','frame','slot','depth','pushes','missingPush','zeroRegister','zeroDefs','zeroWriter','missingZeroProof','resourceSites','resourceSite','resourceLea','resourceSource','missingResourceHelper','resourceHelper','resourceFields','missingRewrite','duplicateRewrite','sourceAmbiguous'])
def test_native_or_source_drift_rejected(guard,kind):
 source,fn,original,rdata=guard;row=copy.deepcopy(original)
 if kind in('native','export','lowered'):row[{'native':'nativeSha256','export':'sourceSha256','lowered':'loweredSha256'}[kind]]='0'*64
 elif kind=='frame':row['frameBytes']+=4
 elif kind=='slot':row['entrySlot']+=4
 elif kind=='depth':row['constructor']['vtableDepth']-=4
 elif kind=='pushes':row['constructor']['stackPushes'].reverse()
 elif kind=='missingPush':del row['nativeInstructions']['0xf19bcb']
 elif kind=='zeroRegister':row['earlyDestructor']['zeroRegisterName']='edi'
 elif kind=='zeroDefs':row['earlyDestructor']['zeroDefinitions']['0xf19dd7']=['0xf19bbc']
 elif kind=='zeroWriter':row['nativeInstructions']['0xf19c7e']='9090'
 elif kind=='missingZeroProof':del row['earlyDestructor']['zeroDefinitions']
 elif kind=='resourceSites':row['resourceDestructorSites'].reverse()
 elif kind=='resourceSite':row['resourceDestructorRewrites'][0]['call']='0xf1a7f6'
 elif kind=='resourceLea':row['resourceDestructorRewrites'][0]['lea']='0xf19bbe'
 elif kind=='resourceSource':row['resourceDestructorRewrites'][0]['sourceCall']='RESOURCE_DestroyMovie(ppuVar18);'
 elif kind=='missingResourceHelper':del row['helperBytes']['0x7e74d0']
 elif kind in('resourceHelper','resourceFields'):row['helperBytes']['0x7e74d0']='00'*0x42
 elif kind=='missingRewrite':row['resourceDestructorRewrites']=[]
 elif kind=='duplicateRewrite':row['resourceDestructorRewrites']*=2
 else:
  source+='RESOURCE_ReleaseResource(ppuVar18);\n';row['loweredSha256']=hashlib.sha256(source.encode()).hexdigest()
 with pytest.raises(ValueError):restore(source,fn,row,rdata)

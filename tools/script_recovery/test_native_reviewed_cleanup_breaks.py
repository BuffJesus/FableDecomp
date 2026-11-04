import copy,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from unittest.mock import patch
from tools.script_recovery.native_reviewed_cleanup_breaks import restore
@pytest.fixture(scope='module')
def guard(tmp_path_factory):
 unit=json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())
 c=UnitConverter(ROOT/'refs/script_recovery/arena/translation_unit_typed.json');captured=[]
 class Captured(Exception):pass
 def capture(source,fn,row,rdata):
  if row is not None:
   captured.append((source,fn,row,rdata));raise Captured()
  return source
 with patch('tools.script_recovery.native_reviewed_cleanup_breaks.restore',capture),patch.object(c,'constructor_defaults',return_value={}):
  with pytest.raises(Captured):c.convert(unit,tmp_path_factory.mktemp('cleanup_input'))
 return captured[0]
@pytest.mark.parametrize('newline',['\n','\r\n'])
def test_cleanup_tail_returns_through_resource_destructor(guard,newline):
 SOURCE,FN,ROW,RDATA=guard
 result=restore(SOURCE.replace('\n',newline),FN,ROW,RDATA)
 assert 'RESOURCE_DestroyMovie(this_00);\n              goto LAB_00f1a7f6;'in result
def test_unreviewed_source_unchanged(guard):
 SOURCE,FN,_,RDATA=guard;assert restore(SOURCE,FN,None,RDATA)==SOURCE
@pytest.mark.parametrize('kind',['nativeSha256','sourceSha256','loweredSha256','movieSite','releaseSite','tailEnd','frameBytes','resourceSlot','movieAlias','instruction','helper','missingInstruction','missingHelper','metadataDepth','metadataTarget','metadataReceiver','sourceBreak','sourceAlias','sourceDuplicate','sourceExit'])
def test_native_or_source_drift_rejected(kind,guard):
 SOURCE,FN,ROW,RDATA=guard
 row=copy.deepcopy(ROW);fn=copy.deepcopy(FN);source=SOURCE
 if kind.endswith('Sha256'):row[kind]='0'*64
 elif kind in('movieSite','releaseSite','tailEnd','frameBytes','resourceSlot'):row[kind]+=4
 elif kind=='movieAlias':row[kind]='wrong'
 elif kind=='instruction':row['nativeInstructions']['0xf1a7f2']='90909090'
 elif kind=='helper':row['helperBytes']['0x7e74d0']='00'
 elif kind=='missingInstruction':del row['nativeInstructions']['0xf1a7f2']
 elif kind=='missingHelper':del row['helperBytes']['0x6e7b80']
 elif kind.startswith('metadata'):
  call=next(c for c in fn['calls']if int(c['site'],16)==row['releaseSite'])
  if kind=='metadataDepth':call['depth']+=4
  elif kind=='metadataReceiver':call['ecxStack']+=4
  else:call['target']='0x006E7B80'
 else:
  import hashlib
  if kind=='sourceBreak':source=source.replace('RESOURCE_DestroyMovie(this_00);\n              break;','RESOURCE_DestroyMovie(this_00);\n              return;')
  elif kind=='sourceAlias':source=source.replace('RESOURCE_DestroyMovie(this_00);','RESOURCE_DestroyMovie(xStack_74);')
  elif kind=='sourceDuplicate':source+='\nLAB_00f1a7ed:\nRESOURCE_DestroyMovie(this_00);\nbreak;\n'
  else:source=source.replace('LAB_00f1a7f6:','LAB_00f1a7ff:')
  row['loweredSha256']=hashlib.sha256(source.encode()).hexdigest()
 with pytest.raises(ValueError):restore(source,fn,row,RDATA)

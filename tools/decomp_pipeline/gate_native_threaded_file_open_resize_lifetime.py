"""Owned rooted write opens composed with actual file construction and virtual deletion."""
import hashlib,json,re,struct,subprocess,sys
from datetime import datetime,timezone
from pathlib import Path
import pefile
import check_one as V
from native_exact_roots import import_names
from wide_string_own_copy_variant_oracle import compare_variant as compare_own_variant
from wide_string_append_variant_oracle import compare_variant as compare_append_variant
from base_class_delete_variant_oracle import compare_variant as compare_base_variant
from threaded_file_open_write_oracle import R,SOURCE,ENTRY,ROOTS,ADDRESS
from gate_game_component_abi import Coff
SIZE=143;SYMBOL=ROOTS[0]["symbol"]
def audit_body(obj):
 c=Coff(obj);sec,off=c.symbols[SYMBOL];body,rels=c.sections[sec];assert off==0 and len(body)==143 and len(rels)==8
 original=read_virtual_data(EXE.read_bytes(),ENTRY,SIZE);patched=bytearray(body);audit=[]
 for site,(name,kind) in sorted(rels.items()):
  assert kind in (6,20) and struct.unpack_from("<i",body,site)[0]==0
  target=ADDRESS[name];operand=struct.unpack_from("<I",original,site)[0]
  assert ((ENTRY+site+4+operand)&0xffffffff if kind==20 else operand)==target
  struct.pack_into("<I",patched,site,(target-(ENTRY+site+4 if kind==20 else 0))&0xffffffff);audit.append(dict(site=site,symbol=name,kind=kind,target=hex(target)))
 assert bytes(patched)==original
 return bytes(patched),dict(grade="MATCH",bytes=143,relocations=audit)
from pe_oracle import EXE,read_virtual_data
FIXTURE=R/'rebuild/tests/integration/NativeThreadedFileOpenResizeLifetime_test.cpp'
SOURCES=[FIXTURE,SOURCE,*[R/'rebuild/src/compiled/00/99'/name for name in (
 'CWideString_Constructor_0099aed0.cpp','CWideString_LiteralConstructor_0099b6b0.cpp',
 'CWideString_CopyConstructor_0099b720.cpp','CWideString_CopyAssignment_0099b7d0.cpp',
 'CWideString_UnassignString_0099b4d0.cpp','CWideString_AllocStringData_0099b3c0.cpp',
 'CWideString_Destructor_0099b510.cpp','CCharString_operatorcharconstptr_0099af00.cpp',
 'STL_Mthrowlengtherror_0099c550.cpp')],R/'rebuild/src/compiled/01/3b/WideStringInstanceCount_013bca20.cpp']
SOURCES.extend([R/'rebuild/src/compiled/00/99'/name for name in (
 'global_ConvertPathToFullPath_00999110.cpp','CAFile_ExpandPath_00998c90.cpp',
 'CAFile_GetCurrentPath_00997f80.cpp','NFile_CalcCurrentPath_009974a0.cpp')])
SOURCES.append(R/'rebuild/src/compiled/01/3b/CAFileCurrentPath_013bca10.cpp')
SOURCES.extend([R/'rebuild/src/compiled/00/99'/name for name in (
 'CCharString_operatorcharconstptr2_0099af10.cpp','CWideString_GetLength_0099b220.cpp',
 'CWideString_AddWideStrings_0099be70.cpp','CWideString_AddWideStringAndLiteral_0099bf30.cpp')])
SOURCES.append(R/'rebuild/src/compiled/00/40/Global_MemCmpUnsigned16_00403c60.cpp')
SOURCES.extend([R/'rebuild/src/compiled'/name for name in (
 '00/99/CWideString_AppendCharacter_0099b9a0.cpp','00/99/STLWideString_Reserve_0099c990.cpp',
 '00/51/STLWideString_UninitializedFillN_005123a0.cpp')])
SOURCES.extend([R/'rebuild/src/compiled/00/98/CThreadedFile_Close_0098ec40.cpp',
 R/'rebuild/src/compiled/00/98/CThreadedFile_Constructor_0098dfd0.cpp',
 R/'rebuild/src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp',
 R/'rebuild/src/compiled/00/98/CThreadedFile_ResizeFile_0098e090.cpp'])

def audit_lifetime(objects):
 from threaded_file_lifetime_oracle import ADDRESS,CTORSYM,DTORSYM,DELETESYM
 roots=[];raw=EXE.read_bytes()
 for index,symbol,entry,size,export in ((25,DELETESYM,0x991450,88,'FileDeleting'),(25,DTORSYM,0x98f090,67,'FileDestructor'),(26,CTORSYM,0x98dfd0,41,'FileConstructor')):
  c=Coff(objects[index]);sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert off==0 and len(body)==size
  patched=bytearray(body);original=read_virtual_data(raw,entry,size);audit=[]
  for site,(name,kind) in sorted(rels.items()):
   assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0;target=ADDRESS[name]
   operand=struct.unpack_from('<I',original,site)[0];assert ((entry+site+4+operand)&0xffffffff if kind==20 else operand)==target
   struct.pack_into('<I',patched,site,(target-(entry+site+4 if kind==20 else 0))&0xffffffff);audit.append(dict(site=site,symbol=name,kind=kind,target=hex(target)))
  assert bytes(patched)==original
  roots.append(dict(index=index,symbol=symbol,address=hex(entry),bytes=size,grade='MATCH',export=export,body=body.hex(),relocations=audit))
 return roots




def audit_bases(objects):
 specs=[('??0CBaseClass@@QAE@XZ',0x99a2d0,9,'BaseConstructor'),
  ('??1CBaseClass@@UAE@XZ',0x99a2e0,7,'BaseDestructor'),
  ('??0CBaseClassNonCopyable@@QAE@XZ',0x99a2f0,9,'NoncopyConstructor'),
  ('??1CBaseClassNonCopyable@@UAE@XZ',0x99a300,7,'NoncopyDestructor'),
  ('??_GCBaseClass@@UAEPAXI@Z',0x4371f0,31,'BaseDeleting'),
  ('??_GCBaseClassNonCopyable@@UAEPAXI@Z',0x99a470,31,'NoncopyDeleting')]
 addresses={'??_7CBaseClass@@6B@':0x1231710,'??_7CBaseClassNonCopyable@@6B@':0x129a7c4,'??3@YAXPAX@Z':0xbfe9bc}
 c=Coff(objects[27]);raw=EXE.read_bytes();roots=[]
 for symbol,entry,size,export in specs:
  sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert off==0 and len(body)==size
  cold=export=='BaseDeleting';original=read_virtual_data(raw,entry,30 if cold else size);patched=bytearray(body);audit=[]
  for site,(name,kind) in sorted(rels.items()):
   assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0;target=addresses[name]
   if not cold:
    operand=struct.unpack_from('<I',original,site)[0];assert ((entry+site+4+operand)&0xffffffff if kind==20 else operand)==target
   struct.pack_into('<I',patched,site,(target-(entry+site+4 if kind==20 else 0))&0xffffffff);audit.append(dict(site=site,symbol=name,kind=kind,target=hex(target)))
  if not cold:assert bytes(patched)==original
  roots.append(dict(index=27,symbol=symbol,address=hex(entry),bytes=size,grade='FUNCTIONAL_COLD_VARIANT' if cold else 'MATCH',export=export,body=body.hex(),relocations=audit))
 assert read_virtual_data(raw,0x1231710,4)==struct.pack('<I',0x4371f0)
 assert read_virtual_data(raw,0x129a7c4,4)==struct.pack('<I',0x99a470)
 return roots

def audit_helpers(objects):
 range_symbol=next(n for n in Coff(objects[7]).symbols if n.startswith('??$_M_range_initialize@PBG'))
 roots=[(2,'??0CWideString@@QAE@XZ',0x99aed0,15,'WideDefault'),
  (3,'??0CWideString@@QAE@PBG@Z',0x99b6b0,104,'WideLiteral'),
  (4,'??0CWideString@@QAE@ABV0@@Z',0x99b720,66,'WideCopy'),
  (5,'??4CWideString@@QAEAAV0@ABV0@@Z',0x99b7d0,47,'WideAssign'),
  (6,'?UnassignString@CWideString@@AAEXXZ',0x99b4d0,62,'WideUnassign'),
  (7,'?AllocStringData@CWideString@@AAEPAVCWideStringData@@PBGJ@Z',0x99b3c0,121,'WideAllocate'),
  (8,'??1CWideString@@QAE@XZ',0x99b510,68,'WideDestroy'),
  (9,'??BCWideString@@QBEPBGXZ',0x99af00,15,'WidePointer'),
  (7,range_symbol,0x99c670,98,'WideRange')]
 addresses={symbol:entry for _,symbol,entry,_,_ in roots}
 addresses.update({'?g_CWideStringInstanceCount_013BCA20@@3HA':0x13bca20,'?g_FableEmptyWideString_0129A8E0@@3PAGA':0x129a8e0,
  '?_M_throw_length_error@?$_String_base@GV?$allocator@G@_STL@@@_STL@@QBEXXZ':0x99c550,
  '_free':0xbfea14,'??3@YAXPAX@Z':0xbfe9bc,'??2@YAPAXI@Z':0xbfea1a,'_malloc':0xbfea0e,'_memmove':0xbfeae6})
 raw=EXE.read_bytes();imports=import_names(raw);reports=[]
 for index,symbol,entry,size,export in roots:
  c=Coff(objects[index]);sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert off==0 and len(body)==size
  original=read_virtual_data(raw,entry,size);patched=bytearray(body);audit=[]
  for site,(name,kind) in sorted(rels.items()):
   assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0;target=addresses[name]
   original_operand=struct.unpack_from('<I',original,site)[0]
   assert ((entry+site+4+original_operand)&0xffffffff if kind==20 else original_operand)==target
   if name in ('_free','??3@YAXPAX@Z','??2@YAPAXI@Z','_malloc','_memmove'):
    thunk=read_virtual_data(raw,target,6);assert thunk[:2]==b'\xff\x25'
    assert imports[struct.unpack_from('<I',thunk,2)[0]]==(name if name.startswith('?') else name[1:])
   struct.pack_into('<I',patched,site,(target-(entry+site+4 if kind==20 else 0))&0xffffffff);audit.append(dict(site=site,symbol=name,kind=kind,target=hex(target)))
  assert bytes(patched)==original and read_virtual_data(raw,entry+size,1)==b'\xcc'
  reports.append(dict(index=index,symbol=symbol,address=hex(entry),bytes=size,grade='MATCH',export=export,body=body.hex(),relocations=audit))
 counter=Coff(objects[11]);sec,off=counter.symbols['?g_CWideStringInstanceCount_013BCA20@@3HA'];body,rels=counter.sections[sec];meta=counter.section_metadata[sec]
 assert off==0 and body==b'' and not rels and meta['raw_size']==4 and meta['raw_pointer']==0 and meta['characteristics']&0x80
 assert read_virtual_data(raw,0x13bca20,4)==b'\0'*4 and read_virtual_data(raw,0x129a8e0,2)==b'\0\0'
 return reports

def audit_paths(objects,base_helpers):
 append_symbol=next(n for n in Coff(objects[20]).symbols if n.startswith('??$append@PBG'))
 specs=[(12,'?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z',0x999110,41,'FullPath'),
  (13,'?ExpandPath@CAFile@@SI?AVCWideString@@ABV2@0@Z',0x998c90,474,'ExpandPath'),
  (14,'?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ',0x997f80,132,'CurrentPath'),
  (15,'?CalcCurrentPath@NFile@@YI?AVCWideString@@XZ',0x9974a0,73,'CalcCurrentPath'),
  (17,'?GetAsCharArray@CWideString@@QBEPBGXZ',0x99af10,15,'WideArray'),
  (18,'?GetLength@CWideString@@QBEJXZ',0x99b220,17,'WideLength'),
  (19,'??H@YI?AVCWideString@@ABV0@0@Z',0x99be70,177,'WidePlusWide'),
  (20,'??H@YI?AVCWideString@@ABV0@PBG@Z',0x99bf30,177,'WidePlusLiteral'),
  (20,'??YCWideString@@QAEABV0@PBG@Z',0x99b940,95,'WideAppendLiteral'),
  (19,'??YCWideString@@QAEABV0@ABV0@@Z',0x99b8d0,104,'WideAppendWide'),
  (20,append_symbol,0x99c7e0,306,'WideAppendRange'),
  (21,'?MemCmp_Unsigned16@@YAHPBG0K@Z',0x403c60,75,'CompareWide'),
  (19,'?MakeOwnCopyOfData@CWideString@@AAEXXZ',0x99b560,124,'OwnCopyVariant'),
  (22,'??YCWideString@@QAEABV0@G@Z',0x99b9a0,205,'WideCharacterAppend'),
  (23,'?reserve@?$basic_string@GU?$char_traits@G@_STL@@V?$allocator@G@2@@_STL@@QAEXI@Z',0x99c990,152,'WideReserve'),
  (24,'??$uninitialized_fill_n@G@_STL@@YIPAGPAGIABG@Z',0x5123a0,32,'WideFill')]
 addresses={h['symbol']:int(h['address'],16) for h in base_helpers}
 addresses.update({symbol:entry for _,symbol,entry,_,_ in specs})
 addresses.update({'?g_CWideStringInstanceCount_013BCA20@@3HA':0x13bca20,'?g_FableEmptyWideString_0129A8E0@@3PAGA':0x129a8e0,
  '?g_CAFileCurrentPath_013BCA10@@3VCWideString@@A':0x13bca10,
  '?_M_throw_length_error@?$_String_base@GV?$allocator@G@_STL@@@_STL@@QBEXXZ':0x99c550,
  '_free':0xbfea14,'??3@YAXPAX@Z':0xbfe9bc,'??2@YAPAXI@Z':0xbfea1a,'_malloc':0xbfea0e,'_memmove':0xbfeae6,
  '_wcscmp':0xbfe94c,'__wgetcwd':0xbfed40,'___security_cookie':0x139c8a8,'@__security_check_cookie@4':0xbfe9f9,
  '??_C@_13FPGAJAPJ@?$AA?2?$AA?$AA@':0x122de78,'??_C@_13EBCNDICG@?$AA?3?$AA?$AA@':0x122de70,
  '??_C@_11LOCGONAA@?$AA?$AA@':0x122d70c,
  '??YCWideString@@QAEABV0@G@Z':0x99b9a0,'?GetSubString@CWideString@@QBE?AV1@KK@Z':0x99bdb0,
  '?GetAbsoluteFilePath@CAFile@@SI?AVCWideString@@ABV2@0@Z':0x998c30})
 raw=EXE.read_bytes();imports=import_names(raw);reports=[]
 assert read_virtual_data(raw,0x139c8a8,4)==bytes.fromhex('4ee640bb')
 check=read_virtual_data(raw,0xbfe9f9,14)
 assert check[:9]==bytes.fromhex('3b0da8c839017501c3') and check[9]==0xe9
 for index,symbol,entry,size,export in specs:
  c=Coff(objects[index]);sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert off==0 and len(body)==size
  near=export in ('OwnCopyVariant','WideAppendWide','WideCharacterAppend');original=read_virtual_data(raw,entry,128 if export=='OwnCopyVariant' else 203 if export=='WideCharacterAppend' else size);patched=bytearray(body);audit=[]
  for site,(name,kind) in sorted(rels.items()):
   assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0;target=addresses[name]
   if not near:
    operand=struct.unpack_from('<I',original,site)[0];assert ((entry+site+4+operand)&0xffffffff if kind==20 else operand)==target,(export,site,name,hex(target),hex(operand))
   if name in ('_free','??3@YAXPAX@Z','??2@YAPAXI@Z','_malloc','_memmove','_wcscmp','__wgetcwd'):
    thunk=read_virtual_data(raw,target,6);assert thunk[:2]==b'\xff\x25'
    assert imports[struct.unpack_from('<I',thunk,2)[0]]==(name if name.startswith('?') else name[1:])
   if name.startswith('??_C@'):
    literal_sec,literal_off=c.symbols[name];literal=c.sections[literal_sec][0][literal_off:]
    end=next(i+2 for i in range(0,len(literal),2) if literal[i:i+2]==b'\0\0');assert read_virtual_data(raw,target,end)==literal[:end]
   struct.pack_into('<I',patched,site,(target-(entry+site+4 if kind==20 else 0))&0xffffffff);audit.append(dict(site=site,symbol=name,kind=kind,target=hex(target)))
  if not near:
   assert bytes(patched)==original,export
   if export!='WideFill':assert read_virtual_data(raw,entry+size,1)==b'\xcc'
   else:assert original[-3:]==b'\xc2\x04\x00' and read_virtual_data(raw,0x5123c0,3)==b'\x8b\x49\x0c'
  elif export=='WideCharacterAppend':
   from near_match import compare,retail_relocations
   char_grade=compare(body,original,tuple(rels),retail_relocations('0099b9a0',203)).to_dict();assert char_grade['near_match'] and char_grade['opcode_similarity']>=.90
  reports.append(dict(index=index,symbol=symbol,address=hex(entry),bytes=size,grade='NEAR_MATCH' if near else 'MATCH',export=export,body=body.hex(),relocations=audit))
 cache=Coff(objects[16]);sec,off=cache.symbols['?g_CAFileCurrentPath_013BCA10@@3VCWideString@@A'];body,rels=cache.sections[sec];meta=cache.section_metadata[sec]
 assert off==0 and body==b'' and not rels and meta['raw_size']==4 and meta['raw_pointer']==0 and meta['characteristics']&0x80
 assert read_virtual_data(raw,0x13bca10,4)==b'\0'*4
 return reports
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 assert sys.platform=='win32'
 build=R/'rebuild/build/native-file-open-resize-lifetime/00/98/0098e000'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 include=build/'include';include.mkdir();sources=build/'sources';sources.mkdir()
 inputs={Path(__file__),R/'tools/decomp_pipeline/threaded_file_resize_oracle.py',R/'tools/decomp_pipeline/base_class_delete_variant_oracle.py',R/'tools/decomp_pipeline/threaded_file_lifetime_oracle.py',EXE,R/'tools/decomp_pipeline/wide_string_own_copy_variant_oracle.py',R/'tools/decomp_pipeline/wide_string_append_variant_oracle.py',R/'tools/decomp_pipeline/threaded_file_open_write_oracle.py',R/'tools/decomp_pipeline/check_one.py',R/'tools/decomp_pipeline/near_match.py',R/'tools/decomp_pipeline/native_exact_roots.py',R/'tools/decomp_pipeline/pe_oracle.py',R/'tools/decomp_pipeline/gate_game_component_abi.py'}
 visited=set()
 def closure(p):
  p=p.resolve()
  if p in visited:return
  visited.add(p);inputs.add(p)
  for n in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()))
 for p in SOURCES:closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
 inputs.update(V.VC/'bin'/n for n in ('cl.exe','link.exe','c1xx.dll','c2.dll'))
 inputs.update(V.VC/'lib'/n for n in ('libcmt.lib','oldnames.lib','kernel32.lib'))
 assert all(p.is_file() for p in inputs)
 pins={str(p):sha(p) for p in sorted(inputs)}
 def save(n,data):
  p=build/n;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('input-pins.json',pins);(build/'gate-source.py').write_bytes(Path(__file__).read_bytes())
 for p in visited:
  target=include/p.relative_to(R/'rebuild/include') if p.is_relative_to(R/'rebuild/include') else sources/p.relative_to(R)
  target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=';'.join(map(str,(V.VC/'include',include,sources)))
 flags=['/nologo','/c','/O2','/MT','/W3','/showIncludes','/I'+str(include),'/I'+str(sources)]
 def compile(name,source,extra=()):
  obj=build/(name+'.obj');cmd=[*flags,*extra,'/Fo'+str(obj),str(source)];r=V.cl(cmd,env,build)
  save(name+'.compile.json',dict(command=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr
  shown=[]
  for line in r.stdout.splitlines():
   if 'including file:' in line.lower():
    p=Path(line.split(':',2)[-1].strip()).resolve();assert p.is_relative_to(V.VC/'include') or p.is_relative_to(build),p;shown.append(str(p))
  if re.search(r'^\s*#include',source.read_text(encoding='utf-8'),re.M):assert shown
  return obj
 objects=[compile('provider%d'%i,sources/p.relative_to(R),('/GS',) if i==15 else ()) for i,p in enumerate(SOURCES)]
 # The operator+ TU already defines the exact same strong allocator. Compile
 # its standalone owner for independent parity, then link only one whole TU.
 allocator='?AllocStringData@CWideString@@AAEPAVCWideStringData@@PBGJ@Z'
 allocator_records=[]
 for index in (7,20):
  c=Coff(objects[index]);sec,off=c.symbols[allocator];assert off==0;allocator_records.append(c.sections[sec])
 assert allocator_records[0]==allocator_records[1]
 code,grade=audit_body(objects[1]);oracle=build/'oracle.bin';oracle.write_bytes(read_virtual_data(EXE.read_bytes(),ENTRY,SIZE))
 char_oracle=build/'character.bin';char_oracle.write_bytes(read_virtual_data(EXE.read_bytes(),0x99b9a0,203))
 helpers=audit_helpers(objects);save('helper-audit.json',helpers)
 path_helpers=audit_paths(objects,helpers);save('path-helper-audit.json',path_helpers)
 own_copy=compare_own_variant(objects[19]);assert own_copy['cases']==112;save('own-copy-variant.json',own_copy)
 append_variant=compare_append_variant(objects[19]);assert append_variant['cases']==576;save('append-variant.json',append_variant)
 helpers+=path_helpers
 lifetime_helpers=audit_lifetime(objects);save('lifetime-helper-audit.json',lifetime_helpers);helpers+=lifetime_helpers
 base_helpers=audit_bases(objects);save('base-helper-audit.json',base_helpers);helpers+=base_helpers
 base_variant=compare_base_variant(objects[27]);assert base_variant['cases']==256;save('base-delete-variant.json',base_variant)
 from threaded_file_resize_oracle import audit_body as audit_resize,SYMBOL as RESIZE_SYMBOL
 _,resize_grade=audit_resize(objects[28]);save('resize-audit.json',resize_grade)
 c=Coff(objects[28]);sec,off=c.symbols[RESIZE_SYMBOL];resize_body,resize_rels=c.sections[sec];assert off==0
 helpers.append(dict(index=28,symbol=RESIZE_SYMBOL,address='0x98e090',bytes=len(resize_body),grade=resize_grade['acceptance_grade'],export='FileResize',body=resize_body.hex(),relocations=resize_grade['relocations']))
 resize_oracle=build/'resize.bin';resize_oracle.write_bytes(read_virtual_data(EXE.read_bytes(),0x98e090,58))
 ctor_oracle=build/'constructor.bin';ctor_oracle.write_bytes(read_virtual_data(EXE.read_bytes(),0x98dfd0,41))
 delete_oracle=build/'deleting.bin';delete_oracle.write_bytes(read_virtual_data(EXE.read_bytes(),0x991450,88))
 exports=['/export:WriteOpen='+SYMBOL,*['/export:'+h['export']+'='+h['symbol'] for h in helpers],'/export:FileVtable=??_7CThreadedFile@@6B@,DATA','/export:BaseVtable=??_7CBaseClass@@6B@,DATA','/export:NoncopyVtable=??_7CBaseClassNonCopyable@@6B@,DATA']
 def link(name,objs,with_exports=False):
  objs=[p for p in objs if p!=objects[7]]
  exe=build/(name+'.exe');cmd=[str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),*map(str,objs),'kernel32.lib',*(exports if with_exports else [])]
  r=subprocess.run(cmd,env=env,cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.link.json',dict(command=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return exe,r
 exe,linked=link('probe',objects,True);assert linked.returncode==0,linked.stdout+linked.stderr
 with pefile.PE(str(exe)) as pe:
  assert pe.FILE_HEADER.Machine==0x14c and pe.OPTIONAL_HEADER.Magic==0x10b
  imports={x.name.decode() for e in pe.DIRECTORY_ENTRY_IMPORT for x in e.imports if x.name}
  assert {'GetFileSize','CreateFileW','CloseHandle','SetFilePointer','SetEndOfFile','GetDiskFreeSpaceW'}<=imports
  locations={x.name.decode():x.address for x in pe.DIRECTORY_ENTRY_EXPORT.symbols if x.name}
  parent=Coff(objects[1]);sec,_=parent.symbols[SYMBOL];body,rels=parent.sections[sec]
  checks=[dict(export='WriteOpen',body=body.hex(),bytes=len(body),relocations=[dict(site=site) for site in rels]),*helpers]
  for item in checks:
   selected=bytearray(pe.get_data(locations[item['export']],item['bytes']));expected=bytearray.fromhex(item['body'])
   for rel in item['relocations']:selected[rel['site']:rel['site']+4]=b'\0'*4
   assert selected==expected,('Linked provider changed',item['export'])
  assert struct.unpack('<I',pe.get_data(locations['FileVtable'],4))[0]==pe.OPTIONAL_HEADER.ImageBase+locations['FileDeleting']
  for table,body_name in (('BaseVtable','BaseDeleting'),('NoncopyVtable','NoncopyDeleting')):
   assert struct.unpack('<I',pe.get_data(locations[table],4))[0]==pe.OPTIONAL_HEADER.ImageBase+locations[body_name]
 def run(name,executable):
  data=build/(name+'.owned-file.bin');assert not data.exists();data.write_bytes(b'owned open_write fixture')
  r=subprocess.run([str(executable),str(oracle),str(data),str(char_oracle),str(ctor_oracle),str(delete_oracle),str(resize_oracle)],cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr,owned_file_exists=data.exists(),owned_file_sha256=sha(data) if data.exists() else None,owned_file_size=data.stat().st_size if data.exists() else None));return r
 positive=run('positive',exe);assert positive.returncode==0 and 'cases=720 opens=360 failures=360 virtual_deletes=144 heap_deletes=432 manager_calls=360 path_forms=3 cache_instances=1 char_cases=128 reserve_checks=6 fill_cases=16' in positive.stdout,positive.stdout+positive.stderr
 omissions=[]
 for index in range(1,len(objects)):
  _,omission=link('omitted%d'%index,[p for i,p in enumerate(objects) if i!=index])
  omissions.append(dict(index=index,source=str(SOURCES[index]),required=bool(omission.returncode),undefined=re.findall(r'unresolved external symbol (.*)',omission.stdout+omission.stderr)))
 noop=sources/'noop.cpp';noop.write_text('#include "fable_threaded_file.h"\nbool CThreadedFile::OpenForWrite(const CWideString&,bool) {return false;}\nbool CThreadedFile::Open(const CWideString&,bool) {return false;}\n')
 control=compile('noop',noop);control_exe,linked=link('control',[objects[0],control,*objects[2:]]);assert linked.returncode==0
 negative=run('control',control_exe);assert negative.returncode==80 and 'NATIVE_OPEN_RESIZE_LIFETIME_FAILURE' in negative.stderr
 identity=sources/'identity-path.cpp';identity.write_text('#include "fable_filesystem.h"\nCWideString __fastcall CAFile::ConvertPathToFullPath(const CWideString& path) {return path;}\n')
 identity_obj=compile('identity-path',identity)
 identity_exe,linked=link('identity-path-control',[identity_obj if i==12 else p for i,p in enumerate(objects)]);assert linked.returncode==0,linked.stdout+linked.stderr
 identity_run=run('identity-path-control',identity_exe)
 assert identity_run.returncode==80 and 'NATIVE_OPEN_RESIZE_LIFETIME_FAILURE' in identity_run.stderr and 'wcscmp(resolved' in identity_run.stderr
 noop_character=sources/'noop-character.cpp';noop_character.write_text('#include "fable_string.h"\nconst CWideString& CWideString::operator+=(wchar_t) {return *this;}\n')
 noop_character_obj=compile('noop-character',noop_character)
 noop_character_exe,linked=link('character-control',[noop_character_obj if i==22 else p for i,p in enumerate(objects)]);assert linked.returncode==0,linked.stdout+linked.stderr
 character_run=run('character-control',noop_character_exe)
 assert character_run.returncode==80 and 'NATIVE_OPEN_RESIZE_LIFETIME_FAILURE' in character_run.stderr and 'value.GetLength()' in character_run.stderr
 noop_lifetime=sources/'noop-lifetime.cpp';noop_lifetime.write_text('#include "fable_threaded_file.h"\nvoid CThreadedFile::Close() {}\nCThreadedFile::~CThreadedFile() {}\n')
 noop_lifetime_obj=compile('noop-lifetime',noop_lifetime)
 lifetime_exe,linked=link('lifetime-control',[noop_lifetime_obj if i==25 else p for i,p in enumerate(objects)]);assert linked.returncode==0,linked.stdout+linked.stderr
 lifetime_run=run('lifetime-control',lifetime_exe)
 assert lifetime_run.returncode==80 and 'NATIVE_OPEN_RESIZE_LIFETIME_FAILURE' in lifetime_run.stderr
 damaged_base=sources/'damaged-base.cpp';base_text=(sources/SOURCES[27].relative_to(R)).read_text()
 old='inline CBaseClass::~CBaseClass()\n{\n}';assert base_text.count(old)==1
 damaged_base.write_text(base_text.replace(old,'inline CBaseClass::~CBaseClass()\n{\n    *reinterpret_cast<unsigned long*>(this)=0x13579bdf;\n}'))
 damaged_base_obj=compile('damaged-base',damaged_base)
 base_exe,linked=link('base-control',[damaged_base_obj if i==27 else p for i,p in enumerate(objects)]);assert linked.returncode==0,linked.stdout+linked.stderr
 base_run=run('base-control',base_exe)
 assert base_run.returncode==80 and 'NATIVE_OPEN_RESIZE_LIFETIME_FAILURE' in base_run.stderr
 noop_resize=sources/'noop-resize.cpp';noop_resize.write_text('#include "fable_threaded_file.h"\nbool CThreadedFile::ResizeFile(unsigned long) {return false;}\n')
 noop_resize_obj=compile('noop-resize',noop_resize)
 resize_exe,linked=link('resize-control',[noop_resize_obj if i==28 else p for i,p in enumerate(objects)]);assert linked.returncode==0,linked.stdout+linked.stderr
 resize_run=run('resize-control',resize_exe)
 assert resize_run.returncode==80 and 'NATIVE_OPEN_RESIZE_LIFETIME_FAILURE' in resize_run.stderr
 assert sum(o['required'] for o in omissions)==24
 assert all(sha(Path(p))==v for p,v in pins.items()),'Input drift'
 counts=re.search(r'data_allocations=(\d+) data_releases=(\d+)',positive.stdout);assert counts and counts[1]==counts[2]
 assert len(helpers)==35 and sum(h['grade']=='MATCH' for h in helpers)==30+(resize_grade['acceptance_grade']=='MATCH') and sum(h['grade']=='NEAR_MATCH' for h in helpers)==4-(resize_grade['acceptance_grade']=='MATCH')
 resize_details=re.search(r'NATIVE_OPEN_RESIZE_DETAIL calls=(\d+) unaligned_failures=(\d+) invalid_true=(\d+) sector_bytes=(\d+)',positive.stdout);assert resize_details and int(resize_details[1])==4320 and int(resize_details[3])==180
 report=dict(resize_calls=4320,unaligned_failures=int(resize_details[2]),invalid_handle_true=180,sector_bytes=int(resize_details[4]),resize_grade=resize_grade,status='PASS',build=str(build),cases=720,opens=360,failures=360,virtual_deletes=144,heap_deletes=432,manager_calls=360,path_forms=3,character_cases=128,reserve_checks=6,fill_cases=16,cache_instances=1,data_allocations=int(counts[1]),data_releases=int(counts[2]),root=grade,helpers=helpers,variant_evidence=dict(own_copy=own_copy,append_wide=append_variant,base_delete=base_variant),linked_bodies_verified=36,actual_counter_zero_bss=True,actual_cache_zero_bss=True,link_omissions=omissions,required_source_omissions=sum(o['required'] for o in omissions),runtime_controls=6,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},linked_object_sha256={str(p):sha(p) for i,p in enumerate(objects) if i!=7},validation_only_sources=[str(SOURCES[7])],executable_sha256=sha(exe),oracle_sha256=sha(oracle),character_oracle_sha256=sha(char_oracle),actual_imports=sorted(imports),boundaries=['Original/rebuilt write-open calls actual ConvertPathToFullPath, ExpandPath, GetCurrentPath and CalcCurrentPath; actual process directory read, typed cache construction, wide-string and CRT ownership.','Absolute, plain relative and rooted input paths; dot/parent and narrow read-open branches are explicit cold traps, length-error provider linked cold. Owned existing/absent/missing-parent/empty paths, whole96B context, payload, size and key guards.',('%d helper bodies and parent exact; one cold base deleting COMDAT is independently functional-only (256 cases), with no new root acceptance; two reused operator+ COMDATs and character append pass near. Independent variant cases112/576 plus original/rebuilt native character cases128, six actual reserve checks and sixteen direct fill cases. All36 linked bodies checked; composition adds no engine root credit.'%sum(h['grade']=='MATCH' for h in helpers)),'Standalone allocator object is compiled for parity only; identical allocator definition from operator+ TU is linked, with complete code/relocation equality. CRT adapters track data blocks and call real malloc/free.',('Original58B/rebuilt%dB ResizeFile executes six times per owned file, including failed opens. Cross-mode outcomes compare return, length, cursor and last error; full context and final retained payload checked. No-buffer/write-through/overlapped handle flags originate from real OpenForWrite.'%len(resize_body)),'One actual process-lifetime cache instance remains empty at main exit, its data released; source/name aliases and temporary counters balance each case. Actual file constructor/deleting/ordinary destructor linked and checked, filename ownership retained through opens and released by destruction. Actual base/noncopyable lifetime and all three vtable targets verified; four additional base bodies linked cold. Manager stream cleanup controlled; no production manager, game, exception-unwind or all-path coverage claim.'])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-file-open-resize-lifetime.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_FILE_OPEN_RESIZE_LIFETIME_PASS paths=720 opens=360 failures=360 virtual=144 heap=432 chars=128 reserve=6 fill=16 bodies=36 resizes=4320 omissions=%d controls=6'%sum(x['required'] for x in omissions));print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()

"""Owned write opens with recovered wide-string ownership and native counter."""
import hashlib,json,re,struct,subprocess,sys
from datetime import datetime,timezone
from pathlib import Path
import pefile
import check_one as V
from native_exact_roots import import_names
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
FIXTURE=R/'rebuild/tests/integration/NativeThreadedFileOpenWriteStrings_test.cpp'
SOURCES=[FIXTURE,SOURCE,*[R/'rebuild/src/compiled/00/99'/name for name in (
 'CWideString_Constructor_0099aed0.cpp','CWideString_LiteralConstructor_0099b6b0.cpp',
 'CWideString_CopyConstructor_0099b720.cpp','CWideString_CopyAssignment_0099b7d0.cpp',
 'CWideString_UnassignString_0099b4d0.cpp','CWideString_AllocStringData_0099b3c0.cpp',
 'CWideString_Destructor_0099b510.cpp','CCharString_operatorcharconstptr_0099af00.cpp',
 'STL_Mthrowlengtherror_0099c550.cpp')],R/'rebuild/src/compiled/01/3b/WideStringInstanceCount_013bca20.cpp']

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
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 assert sys.platform=='win32'
 build=R/'rebuild/build/native-file-open-write-strings/00/98/0098e000'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 include=build/'include';include.mkdir();sources=build/'sources';sources.mkdir()
 inputs={Path(__file__),EXE,R/'tools/decomp_pipeline/threaded_file_open_write_oracle.py',R/'tools/decomp_pipeline/check_one.py',R/'tools/decomp_pipeline/near_match.py',R/'tools/decomp_pipeline/native_exact_roots.py',R/'tools/decomp_pipeline/pe_oracle.py',R/'tools/decomp_pipeline/gate_game_component_abi.py'}
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
 def compile(name,source):
  obj=build/(name+'.obj');cmd=[*flags,'/Fo'+str(obj),str(source)];r=V.cl(cmd,env,build)
  save(name+'.compile.json',dict(command=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr
  shown=[]
  for line in r.stdout.splitlines():
   if 'including file:' in line.lower():
    p=Path(line.split(':',2)[-1].strip()).resolve();assert p.is_relative_to(V.VC/'include') or p.is_relative_to(build),p;shown.append(str(p))
  assert shown;return obj
 objects=[compile('provider%d'%i,sources/p.relative_to(R)) for i,p in enumerate(SOURCES)]
 code,grade=audit_body(objects[1]);oracle=build/'oracle.bin';oracle.write_bytes(read_virtual_data(EXE.read_bytes(),ENTRY,SIZE))
 helpers=audit_helpers(objects);save('helper-audit.json',helpers)
 exports=['/export:WriteOpen='+SYMBOL,*['/export:'+h['export']+'='+h['symbol'] for h in helpers]]
 def link(name,objs,with_exports=False):
  exe=build/(name+'.exe');cmd=[str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),*map(str,objs),'kernel32.lib',*(exports if with_exports else [])]
  r=subprocess.run(cmd,env=env,cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.link.json',dict(command=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return exe,r
 exe,linked=link('probe',objects,True);assert linked.returncode==0,linked.stdout+linked.stderr
 with pefile.PE(str(exe)) as pe:
  assert pe.FILE_HEADER.Machine==0x14c and pe.OPTIONAL_HEADER.Magic==0x10b
  imports={x.name.decode() for e in pe.DIRECTORY_ENTRY_IMPORT for x in e.imports if x.name}
  assert {'GetFileSize','CreateFileW','CloseHandle'}<=imports
  locations={x.name.decode():x.address for x in pe.DIRECTORY_ENTRY_EXPORT.symbols if x.name}
  parent=Coff(objects[1]);sec,_=parent.symbols[SYMBOL];body,rels=parent.sections[sec]
  checks=[dict(export='WriteOpen',body=body.hex(),bytes=len(body),relocations=[dict(site=site) for site in rels]),*helpers]
  for item in checks:
   selected=bytearray(pe.get_data(locations[item['export']],item['bytes']));expected=bytearray.fromhex(item['body'])
   for rel in item['relocations']:selected[rel['site']:rel['site']+4]=b'\0'*4
   assert selected==expected,('Linked provider changed',item['export'])
 def run(name,executable):
  data=build/(name+'.owned-file.bin');assert not data.exists();data.write_bytes(b'owned open_write fixture')
  r=subprocess.run([str(executable),str(oracle),str(data)],cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr,owned_file_exists=data.exists(),owned_file_sha256=sha(data) if data.exists() else None,owned_file_size=data.stat().st_size if data.exists() else None));return r
 positive=run('positive',exe);assert positive.returncode==0 and 'cases=96 opens=48 failures=48 data_allocations=104 data_releases=104' in positive.stdout,positive.stdout+positive.stderr
 omissions=[]
 for index in range(1,len(objects)):
  _,omission=link('omitted%d'%index,[p for i,p in enumerate(objects) if i!=index]);assert omission.returncode,('Unexpected redundant source',index)
  omissions.append(dict(index=index,source=str(SOURCES[index]),undefined=re.findall(r'unresolved external symbol (.*)',omission.stdout+omission.stderr)))
 noop=sources/'noop.cpp';noop.write_text('#include "fable_threaded_file.h"\nbool CThreadedFile::OpenForWrite(const CWideString&,bool) {return false;}\nbool CThreadedFile::Open(const CWideString&,bool) {return false;}\n')
 control=compile('noop',noop);control_exe,linked=link('control',[objects[0],control,*objects[2:]]);assert linked.returncode==0
 negative=run('control',control_exe);assert negative.returncode==80 and 'NATIVE_OPEN_WRITE_STRINGS_FAILURE' in negative.stderr
 assert all(sha(Path(p))==v for p,v in pins.items()),'Input drift'
 report=dict(status='PASS',build=str(build),cases=96,opens=48,failures=48,data_allocations=104,data_releases=104,root=grade,helpers=helpers,linked_bodies_verified=10,actual_counter_zero_bss=True,link_omissions=omissions,runtime_controls=1,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(exe),oracle_sha256=sha(oracle),actual_imports=sorted(imports),boundaries=['Privately loaded original143B body, eight checked dependency operands; compiled143B exact body from frozen ordinary C++ and headers.','Owned file only; existing/absent/missing-parent/empty paths, overwrite/preserve modes, initial flags, full context/filename/padding/size/payload and sort-key progression checks.','Nine existing string roots exact after named relocations, actual counter zero BSS, and ten linked bodies verified. CRT new/delete adapters track data blocks and call real malloc/free; string buffer allocation/release uses real CRT.','Path conversion shares the supplied absolute string; read-open-only helpers are cold traps, length-error provider linked cold. No file constructor, manager, game or exception-unwind claim.'])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-file-open-write-strings.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_FILE_OPEN_WRITE_STRINGS_PASS cases=96 opens=48 failures=48 allocations=104 releases=104 omissions=%d controls=1'%len(omissions));print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()

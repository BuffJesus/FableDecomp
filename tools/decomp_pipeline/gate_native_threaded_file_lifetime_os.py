"""Linked virtual file lifetime with real owned Windows handles and heap release."""
import hashlib,json,re,struct,subprocess,sys
from datetime import datetime,timezone
from pathlib import Path
import pefile
import check_one as V
from gate_game_component_abi import Coff
from threaded_file_lifetime_oracle import R,SOURCE,CTOR,CTORSYM,DTORSYM,DELETESYM,ADDRESS
from pe_oracle import EXE,read_virtual_data
FIXTURE=R/'rebuild/tests/integration/NativeThreadedFileLifetime_test.cpp'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 assert sys.platform=='win32'
 build=R/'rebuild/build/native-file-lifetime/00/99/00991450'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 include=build/'include';include.mkdir();sources=build/'sources';sources.mkdir()
 inputs={Path(__file__),EXE,R/'tools/decomp_pipeline/threaded_file_lifetime_oracle.py',R/'tools/decomp_pipeline/check_one.py',R/'tools/decomp_pipeline/gate_game_component_abi.py',R/'tools/decomp_pipeline/pe_oracle.py'};visited=set()
 def closure(p):
  p=p.resolve()
  if p in visited:return
  visited.add(p);inputs.add(p)
  for n in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()))
 for p in (FIXTURE,SOURCE,CTOR):closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
 inputs.update(V.VC/'bin'/n for n in ('cl.exe','link.exe','c1xx.dll','c2.dll'))
 inputs.update(V.VC/'lib'/n for n in ('libcmt.lib','oldnames.lib','kernel32.lib'))
 assert all(p.is_file() for p in inputs);pins={str(p):sha(p) for p in sorted(inputs)}
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
 # The real destructor TU must precede the constructor's equivalent out-of-line
 # deleting COMDAT; verify the selected linked body, not just provider objects.
 objects=[compile('fixture',sources/FIXTURE.relative_to(R)),compile('lifetime',sources/SOURCE.relative_to(R)),compile('constructor',sources/CTOR.relative_to(R))]
 roots=[];raw=EXE.read_bytes()
 for index,symbol,entry,size,export in ((1,DELETESYM,0x991450,88,'FileDeleting'),(1,DTORSYM,0x98f090,67,'FileDestructor'),(2,CTORSYM,0x98dfd0,41,'FileConstructor')):
  c=Coff(objects[index]);sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert off==0 and len(body)==size
  patched=bytearray(body);original=read_virtual_data(raw,entry,size);audit=[]
  for site,(name,kind) in sorted(rels.items()):
   assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0;target=ADDRESS[name]
   operand=struct.unpack_from('<I',original,site)[0];assert ((entry+site+4+operand)&0xffffffff if kind==20 else operand)==target
   struct.pack_into('<I',patched,site,(target-(entry+site+4 if kind==20 else 0))&0xffffffff);audit.append(dict(site=site,symbol=name,kind=kind,target=hex(target)))
  assert bytes(patched)==original
  roots.append(dict(symbol=symbol,entry=hex(entry),bytes=size,export=export,body=body.hex(),relocations=audit))
 exports=['/export:FileDeleting='+DELETESYM,'/export:FileDestructor='+DTORSYM,'/export:FileConstructor='+CTORSYM,'/export:FileVtable=??_7CThreadedFile@@6B@,DATA']
 def link(name,objs,with_exports=False):
  exe=build/(name+'.exe');cmd=[str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),*map(str,objs),'kernel32.lib',*(exports if with_exports else [])]
  r=subprocess.run(cmd,env=env,cwd=build,capture_output=True,text=True,timeout=30);save(name+'.link.json',dict(command=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return exe,r
 exe,linked=link('probe',objects,True);assert linked.returncode==0,linked.stdout+linked.stderr
 with pefile.PE(str(exe)) as pe:
  assert pe.FILE_HEADER.Machine==0x14c and pe.OPTIONAL_HEADER.Magic==0x10b
  locations={x.name.decode():x.address for x in pe.DIRECTORY_ENTRY_EXPORT.symbols if x.name}
  for root in roots:
   selected=bytearray(pe.get_data(locations[root['export']],root['bytes']));expected=bytearray.fromhex(root['body'])
   for rel in root['relocations']:selected[rel['site']:rel['site']+4]=b'\0'*4
   assert selected==expected,('Wrong linked COMDAT selected',root['export'])
  assert struct.unpack('<I',pe.get_data(locations['FileVtable'],4))[0]==pe.OPTIONAL_HEADER.ImageBase+locations['FileDeleting']
  imports={x.name.decode() for e in pe.DIRECTORY_ENTRY_IMPORT for x in e.imports if x.name};assert {'CloseHandle','GetHandleInformation','CreateFileA','VirtualAlloc'}<=imports
 ctor_oracle=build/'constructor.bin';ctor_oracle.write_bytes(read_virtual_data(raw,0x98dfd0,41))
 delete_oracle=build/'deleting.bin';delete_oracle.write_bytes(read_virtual_data(raw,0x991450,88))
 data=build/'owned-file.bin';data.write_bytes(bytes((i*37+23)&255 for i in range(1234)));data_hash=sha(data)
 def run(name,executable):
  r=subprocess.run([str(executable),str(ctor_oracle),str(delete_oracle),str(data)],cwd=build,capture_output=True,text=True,timeout=30);save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return r
 positive=run('positive',exe);assert positive.returncode==0 and 'cases=52 direct=48 virtual_deletes=4' in positive.stdout,positive.stdout+positive.stderr
 for index,symbol in ((1,DTORSYM),(2,CTORSYM)):
  _,missing=link('omitted%d'%index,[p for i,p in enumerate(objects) if i!=index]);assert missing.returncode and symbol in missing.stdout+missing.stderr
 noop=sources/'noop.cpp';noop.write_text('#include "fable_threaded_file.h"\nvoid CThreadedFile::Close() {}\nCThreadedFile::~CThreadedFile() {}\n')
 control=compile('noop',noop);control_exe,linked=link('control',[objects[0],control,objects[2]]);assert linked.returncode==0,linked.stdout+linked.stderr
 negative=run('control',control_exe);assert negative.returncode==80 and 'NATIVE_FILE_LIFETIME_FAILURE' in negative.stderr
 assert sha(data)==data_hash and all(sha(Path(p))==v for p,v in pins.items()),'Input drift'
 report=dict(status='PASS',build=str(build),cases=52,direct=48,virtual_deletes=4,roots=roots,linked_body_selection='EXACT_MASKED',linked_vtable_scalar_target=True,link_omissions=2,runtime_controls=1,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(exe),owned_file_sha256=data_hash,actual_imports=sorted(imports),boundaries=['Private original ctor/deleting blobs and freshly compiled native lifetime; real owned handles and CRT malloc/free through a checked deletion adapter.','Actual linked vtable dispatch, original flag semantics, handle replacement/reload, full96B guards/cleanup trace and real GetHandleInformation closure checks.','Manager CloseAllStreams and wide/base lifetime controlled; raw manager storage alias only; no game, production manager, real string allocation or exception unwind.'])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-file-lifetime-os.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_FILE_LIFETIME_OS_PASS cases=52 direct=48 virtual_deletes=4 omissions=2 controls=1');print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()

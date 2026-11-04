"""Owned Windows file probe: original retail and rebuilt OpenForWrite instructions."""
import hashlib,json,re,struct,subprocess,sys
from datetime import datetime,timezone
from pathlib import Path
import pefile
import check_one as V
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
FIXTURE=R/'rebuild/tests/integration/NativeThreadedFileOpenWrite_test.cpp'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 assert sys.platform=='win32'
 build=R/'rebuild/build/native-file-open-write/00/98/0098e000'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 include=build/'include';include.mkdir();sources=build/'sources';sources.mkdir()
 inputs={Path(__file__),EXE,R/'tools/decomp_pipeline/threaded_file_open_write_oracle.py',R/'tools/decomp_pipeline/check_one.py',R/'tools/decomp_pipeline/near_match.py',R/'tools/decomp_pipeline/native_exact_roots.py',R/'tools/decomp_pipeline/pe_oracle.py',R/'tools/decomp_pipeline/gate_game_component_abi.py'}
 visited=set()
 def closure(p):
  p=p.resolve()
  if p in visited:return
  visited.add(p);inputs.add(p)
  for n in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()))
 for p in (FIXTURE,SOURCE):closure(p)
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
 objects=[compile('fixture',sources/FIXTURE.relative_to(R)),compile('root',sources/SOURCE.relative_to(R))]
 code,grade=audit_body(objects[1]);oracle=build/'oracle.bin';oracle.write_bytes(read_virtual_data(EXE.read_bytes(),ENTRY,SIZE))
 def link(name,objs):
  exe=build/(name+'.exe');cmd=[str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),*map(str,objs),'kernel32.lib']
  r=subprocess.run(cmd,env=env,cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.link.json',dict(command=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return exe,r
 exe,linked=link('probe',objects);assert linked.returncode==0,linked.stdout+linked.stderr
 with pefile.PE(str(exe)) as pe:
  assert pe.FILE_HEADER.Machine==0x14c and pe.OPTIONAL_HEADER.Magic==0x10b
  imports={x.name.decode() for e in pe.DIRECTORY_ENTRY_IMPORT for x in e.imports if x.name}
  assert {'GetFileSize','CreateFileW','CloseHandle'}<=imports
 def run(name,executable):
  data=build/(name+'.owned-file.bin');assert not data.exists();data.write_bytes(b'owned open_write fixture')
  r=subprocess.run([str(executable),str(oracle),str(data)],cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr,owned_file_exists=data.exists(),owned_file_sha256=sha(data) if data.exists() else None,owned_file_size=data.stat().st_size if data.exists() else None));return r
 positive=run('positive',exe);assert positive.returncode==0 and 'cases=24 opens=16 failures=8 original_rebuilt=PASS' in positive.stdout,positive.stdout+positive.stderr
 _,omission=link('root-omitted',objects[:1]);assert omission.returncode and SYMBOL in omission.stdout+omission.stderr
 noop=sources/'noop.cpp';noop.write_text('#include "fable_threaded_file.h"\nbool CThreadedFile::OpenForWrite(const CWideString&,bool) {return false;}\nbool CThreadedFile::Open(const CWideString&,bool) {return false;}\n')
 control=compile('noop',noop);control_exe,linked=link('control',[objects[0],control]);assert linked.returncode==0
 negative=run('control',control_exe);assert negative.returncode==80 and 'NATIVE_OPEN_WRITE_FAILURE' in negative.stderr
 assert all(sha(Path(p))==v for p,v in pins.items()),'Input drift'
 report=dict(status='PASS',build=str(build),cases=24,opens=16,failures=8,root=grade,link_omissions=1,runtime_controls=1,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(exe),oracle_sha256=sha(oracle),actual_imports=sorted(imports),boundaries=['Privately loaded original143B body, eight checked dependency operands; compiled143B exact body from frozen ordinary C++ and headers.','Owned file only; existing/absent/missing-parent paths, overwrite/preserve modes, initial flags, full context/filename/padding/size/payload and sort-key progression checks.','Path/string helpers are the established controlled recorders; real CreateFileW/GetFileSize/CloseHandle execute; no engine file constructor, manager, game or save executes.'])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-file-open-write-io.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_FILE_OPEN_WRITE_IO_PASS cases=24 opens=16 failures=8 omissions=1 controls=1');print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()

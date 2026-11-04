"""Typed bank ChangeLoadingPriority, original bytes and candidate-required ABI proof."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,importlib.util,itertools,json,re,shutil,struct,subprocess
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--scratch',type=Path);args=ap.parse_args()
 source=(args.scratch if args.scratch else R/'rebuild/src/compiled/00/9d')/'CBankFileAsyncData_ChangeLoadingPriority_009d5630.cpp'
 include=args.scratch/'include' if args.scratch else R/'rebuild/include'
 out=R/'rebuild/build/native-bank-priority/00/9d/009d5630'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');out.mkdir(parents=True)
 test=R/'rebuild/tests/integration/NativeBankPriority_test.py'
 host=R/'rebuild/tests/00/9d/CBankFileAsyncData_ChangeLoadingPriority_009d5630_test.cpp'
 spec=importlib.util.spec_from_file_location('priority_test',test);testmod=importlib.util.module_from_spec(spec);spec.loader.exec_module(testmod)
 ROOT,FILE=testmod.ROOT,testmod.FILE
 provider=R/'rebuild/src/compiled/00/98/CThreadedFile_SetObjectPriority_0098f0e0.cpp'
 inputs={Path(__file__),test,EXE};visited=set()
 def closure(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(),re.M):
   q=next(q for q in (p.parent/name,include/name) if q.is_file());closure(q)
 closure(source);closure(provider);closure(host)
 symbol='?ChangeLoadingPriority@CBankFileAsyncData@@QAEXW4EThreadedFilePriority@@@Z'
 for name in ('egor_pdb_names.tsv','fablewin_pdb_names.tsv'):
  p=R/'ghidra_out'/name;assert symbol in p.read_text(encoding='utf-8');inputs.add(p)
 inputs.update(R/'tools/decomp_pipeline'/n for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py'))
 for folder in ('bin','include','lib'):inputs.update(p for p in (V.VC/folder).rglob('*') if p.is_file())
 sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 pins={str(p):sha(p) for p in sorted(inputs)}
 def save(name,value):(out/name).write_text(json.dumps(value,indent=2)+'\n')
 save('input-pins.json',pins);inc=out/'include';inc.mkdir()
 for p in visited:
  if p.is_relative_to(include):
   q=inc/p.relative_to(include);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 (out/test.name).write_bytes(test.read_bytes());(out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
 fs='?SetObjectPriority@CThreadedFile@@QAEXKW4EThreadedFilePriority@@@Z'
 addresses={fs:FILE,'?SetObjectPriority@CThreadedFileManager@@QAEXKW4EThreadedFilePriority@@@Z':testmod.MANAGER,'?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A':testmod.GLOBAL}
 codes={};objects={};relocations={}
 for src,sym,address,size in ((source,symbol,ROOT,30),(provider,fs,FILE,10)):
  snap=out/src.name;snap.write_bytes(src.read_bytes());obj=out/(src.stem+'.obj')
  r=V.cl(['/nologo','/c','/O2','/MT','/W3','/I'+str(inc),'/Fo'+str(obj),str(snap)],env,out)
  save(src.stem+'-compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr
  c=Coff(obj);section,offset=c.symbols[sym];assert offset==0
  body,rels=c.sections[section];assert len(body)==size;b=bytearray(body)
  for site,(name,kind) in rels.items():
   assert kind in (6,20);value=addresses[name]+struct.unpack_from('<i',b,site)[0]-(address+site+4 if kind==20 else 0)
   struct.pack_into('<I',b,site,value&0xffffffff)
  codes[address]=bytes(b);objects[str(obj)]=sha(obj);relocations[sym]=rels
 raw=EXE.read_bytes();original={ROOT:read_virtual_data(raw,ROOT,30),FILE:read_virtual_data(raw,FILE,10)};assert codes==original
 hostcopy=out/host.name;hostcopy.write_bytes(host.read_bytes());hostobj=out/'host.obj'
 r=V.cl(['/nologo','/c','/O2','/MT','/W3','/I'+str(inc),'/Fo'+str(hostobj),str(hostcopy)],env,out)
 save('host-compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr
 rootobj=out/(source.stem+'.obj');exe=out/'host.exe'
 for name,providers in [('host',[hostobj,rootobj]),('omit-root-host',[hostobj])]:
  r=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(out/(name+'.exe')),*map(str,providers),'kernel32.lib'],env=env,cwd=out,capture_output=True,text=True,timeout=30)
  save(name+'-link.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr))
  assert (r.returncode==0)==(name=='host'),r.stdout+r.stderr
 r=subprocess.run([str(exe)],cwd=out,capture_output=True,text=True,timeout=10)
 save('host-run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and 'NATIVE_BANK_PRIORITY_HOST_PASS cases=864' in r.stdout
 objects[str(hostobj)]=sha(hostobj)
 cases=0
 for case in itertools.product((0,1,7,255),(0,1,0x7fffffff,0x80000000,0xfffffffe,0xffffffff),(0,1,2,3,4,5,6,7,0xffffffff),range(4)):
  assert testmod.execute(codes,*case)==testmod.execute(original,*case);cases+=1
 mutants={'omit-root':{FILE:codes[FILE]},'omit-file':{ROOT:codes[ROOT]}}
 for name,at,value in [('wrong-uid',2,0x1c),('wrong-bank',15,0x24),('wrong-file',18,0x14),('skip-call',5,0xeb)]:
  b=bytearray(codes[ROOT]);b[at]=value;mutants[name]=dict(codes);mutants[name][ROOT]=bytes(b)
 controls={}
 for name,bodies in mutants.items():
  try:testmod.execute(bodies,7,0x80000000,6,2)
  except Exception as error:controls[name]=type(error).__name__
  else:raise AssertionError('undetected '+name)
 assert all(sha(Path(p))==h for p,h in pins.items());assert all(sha(Path(p))==h for p,h in objects.items())
 report=dict(status='PASS_SCRATCH' if args.scratch else 'PASS',build=str(out),cases=cases,root_bytes=30,file_bytes=10,controls=controls,input_sha256=pins,object_sha256=objects,relocations=relocations,boundaries=['Original and rebuilt native member plus actual file wrapper execute; manager priority implementation is controlled.','UID0 uses an unmapped bank pointer and must return without dereferencing it. Complete arena/global preservation, receiver/arguments, stack cleanup and callee-saved registers checked.','No worker integration or new reconstruction root credit; replaces existing accepted padded-struct source.'])
 report.update(host_cases=864,host_executable_sha256=sha(exe),host_root_omission=True)
 save('receipt.json',report)
 if not args.scratch:(R/'rebuild/compile-gate/native-bank-priority.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_BANK_PRIORITY_PASS cases=%d controls=%d root_bytes=30 file_bytes=10'%(cases,len(controls)));print(out/'receipt.json')
if __name__=='__main__':main()

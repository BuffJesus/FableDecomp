import sys,struct,subprocess,json,itertools
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import build_installer_lifetime as build
import oracle_file_installer_destructor as oracle
import check_one
w=build.w;obj=build.obj;fixture=w/'fixture.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(fixture),str(ROOT/'rebuild/tests/integration/FileInstallerDestructor_test.cpp')],build.env,w);assert r.returncode==0,r.stdout+r.stderr
blob=obj.read_bytes()
for old,new in [('__imp__SetEvent@4','_SignalSlot'),('__imp__Sleep@4','_SleepSlot'),('__imp__CloseHandle@4','_CloseSlot'),('__imp__DeleteCriticalSection@4','_DeleteSlot'),('_free','_Fnod')]:
 needle=old.encode()+b'\0';value=new.encode()+b'\0';assert blob.count(needle)==1 and len(value)<=len(needle);blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
obj.write_bytes(blob);kinds=['lockctor','clear','signal','release','sleep','lock','close','lockdtor','deletecs','alias','completed','active','counted','range','free']
rows=list(itertools.product([0,1,3],range(2),range(4),range(3)));data=bytearray(struct.pack('<I',len(rows)))
for args in rows:
 events,after=oracle.model(args);data.extend(struct.pack('<5I',*args,len(events)));data.extend(oracle.initial(*args));data.extend(after)
 for kind,values,state in events:data.extend(struct.pack('<3I',kinds.index(kind)+1,*(list(values)+[0]*(2-len(values)))));data.extend(state)
binary=w/'retail.bin';binary.write_bytes(data);exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(fixture)],env=build.env,capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(cmd+[str(obj),str(fixture)],env=build.env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'INSTALLER_DESTRUCTOR_NATIVE_OK cases=72'in r.stdout,(r.returncode,r.stdout,r.stderr)
(ROOT/'rebuild/compile-gate/installer-destructor-native.json').write_text(json.dumps(dict(cases=72,omission_checks=1,full_region_bytes=oracle.WIDTH,boundary='Native compiled C++ destructor compared at every boundary and final state against independent retail-validated model; normalized compiler vtables. Dependencies and releases recorded, memory retained; no real Windows/worker/heap lifecycle claim.'),indent=2)+'\n');print(r.stdout.strip())

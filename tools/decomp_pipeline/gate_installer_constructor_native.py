import sys,json,ast,subprocess,struct,argparse
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import build_installer_lifetime as build
import check_one,oracle_file_installer_constructor as oracle
ap=argparse.ArgumentParser();ap.add_argument('--failure',action='store_true');args=ap.parse_args()
if args.failure:import oracle_file_installer_constructor_failure as oracle
from variable_allocator_parity import check
source=ROOT/'rebuild/src/compiled/00/9c/CFileInstaller_Constructor_009c5890.cpp';obj=build.w/'constructor.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],build.env,build.w);assert r.returncode==0,r.stdout+r.stderr
tree=ast.parse((ROOT/'tools/decomp_pipeline/gate_file_installer_constructor.py').read_text());fn=next(n for n in tree.body if isinstance(n,ast.FunctionDef)and n.name=='main')
assignment=next(n for n in fn.body if isinstance(n,ast.Assign)and any(isinstance(t,ast.Name)and t.id=='targets'for t in n.targets))
targets=ast.literal_eval(assignment.value);targets.pop('?g_FileInstallerVTable_0129B43C@@3EA');targets['??_7CFileInstaller@@6B@']=0x129b43c
report=check(obj,'??0CFileInstaller@@QAE@XZ',0x9c5890,389,18,targets)
(ROOT/'rebuild/compile-gate/installer-polymorphic-constructor-parity.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
fixtureSource=build.w/'fixture.cpp';f=(ROOT/'rebuild/tests/integration'/('FileInstallerConstructorFailure_test.cpp'if args.failure else 'FileInstallerConstructor_test.cpp')).read_text()
f=f.replace('unsigned char g_FileInstallerVTable_0129B43C;','static void* observedVtable;\nCFileInstaller::~CFileInstaller() {}')
f=f.replace('if(*(unsigned long*)actual!=(unsigned long)&g_FileInstallerVTable_0129B43C)exit(10);','if(!observedVtable)observedVtable=*(void**)actual;if(*(void**)actual!=observedVtable||!observedVtable)exit(10);')
fixtureSource.write_text(f);fixture=build.w/'fixture.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(fixture),str(fixtureSource)],build.env,build.w);assert r.returncode==0,r.stdout+r.stderr
blob=obj.read_bytes()
for old,new in [('__imp__InitializeCriticalSection@4','_InitSlot'),('__imp__CreateEventW@16','_EventSlot'),('__beginthread','_ThreadRecording')]:
 needle=old.encode()+b'\0';value=new.encode()+b'\0';assert blob.count(needle)==1
 if len(value)>len(needle):
  raw=bytearray(blob);ptr,count=struct.unpack_from('<II',raw,8);start=ptr+count*18;pos=raw.index(needle,start);delta=len(value)-len(needle);offset=pos-start
  for i in range(count):
   at=ptr+i*18
   if raw[at:at+4]==b'\0'*4:
    oldOffset=struct.unpack_from('<I',raw,at+4)[0]
    if oldOffset>offset:struct.pack_into('<I',raw,at+4,oldOffset+delta)
  struct.pack_into('<I',raw,start,struct.unpack_from('<I',raw,start)[0]+delta);blob=bytes(raw[:pos])+value+bytes(raw[pos+len(needle):])
 else:blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
obj.write_bytes(blob);rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
for row in rows:
 data.extend(struct.pack('<3I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
 for *values,state in row['events']:data.extend(struct.pack('<4I',*values));data.extend(state)
binary=build.w/'retail.bin';binary.write_bytes(data);exe=build.w/'ctor.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(fixture)],env=build.env,capture_output=True,text=True);assert r.returncode!=0
r=subprocess.run(cmd+[str(obj),str(fixture)],env=build.env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);marker='FILE_INSTALLER_CTOR_FAILURE_NATIVE_OK cases=5'if args.failure else 'FILE_INSTALLER_CTOR_NATIVE_OK cases=24';assert r.returncode==0 and marker in r.stdout,(r.returncode,r.stdout,r.stderr)
report=dict(cases=len(rows),omission_checks=1,boundary='Production polymorphic installer constructor with compiler-generated vtable; recorded OS/containers/thread launches and fixture destructor. Full32KiB snapshots versus retail oracle; vtable canonicalized and stable across cases, identity corroborated by relocation parity.')
if args.failure:report['failure']='Five null sentinel allocation points; native SEH write fault at zero and matching complete partial memory/call states.'
(ROOT/'rebuild/compile-gate'/('polymorphic-constructor-failure.json'if args.failure else 'polymorphic-constructor-behavior.json')).write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip())

import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-managed-texture-destructor-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a6/CManagedTextureData_Destructor_00a6b5b0.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/managed_texture_data_destructor_candidate2_20261009.cpp'
obj=w/'destructor.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
proof=check(obj,'??1CManagedTextureData@@UAE@XZ',0xa6b5b0,107,5,{'??3@YAXPAX@Z':0xbfe9bc,'??1CResource@@UAE@XZ':0x9fc550,'??_7CManagedTextureData@@6B@':0x129dc24})
body=read_virtual_data(EXE.read_bytes(),0xa6b5b0,107);fixups=[];cs=Cs(CS_ARCH_X86,CS_MODE_32);external={0xbfe9bc:0,0x9fc550:1}
for ins in cs.disasm(body,0xa6b5b0):
 if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
  target=int(ins.op_str,16);assert target in external;fixups.append((ins.address-0xa6b5b0+1,external[target],20))
assert len(fixups)==4
(w/'managed_texture_destructor_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases={'@ManagedDestructor@8':'??1CManagedTextureData@@UAE@XZ','_ManagedVTable':'??_7CManagedTextureData@@6B@','??1CResource@@UAE@XZ':'@BaseDestructorBoundary@8','?DeleteCIVCountedPointeeBase@CIVCountedPointeeBase@@UBEXXZ':'@UnusedBoundary@8'}
(w/'managed_texture_destructor_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedTextureDataDestructor_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=16384 ' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_managed_texture_data.h','fable_resource_list.h','fable_counted_pointer.h','rebuild_abi.h')]
report=dict(cases=16384,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual complete managed-data destructor113B vs107B original,98.8235percent near match. One extra derived-vptr store is redundant for correctly constructed managed-data owners; gate requires original correct runtime vptr and does not claim arbitrary invalid-vptr equivalence. Raw remaining76B fields/refcount wrap/null-live-shared controls/two placements-fills. Unconditional C++delete of stored volume-header pointer includingNULL, then volume-handle release,texture-handle release,base destructor tailcall. Stored deleters and strong delete/base destructor are functional boundaries; callbacks swap liveInfo and mutate owner/data/padding; full16KiB snapshots at every boundary and completion. Base cleanup, emitted deleting wrappers/vtable/helperdata not additional recovery.')
(ROOT/'rebuild/compile-gate/native-managed-texture-data-destructor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_TEXTURE_DATA_DESTRUCTOR_GATE_PASS')

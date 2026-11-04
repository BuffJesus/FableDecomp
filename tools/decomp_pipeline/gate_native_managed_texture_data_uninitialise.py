import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-managed-texture-uninitialise-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a6/CManagedTextureData_Uninitialise_00a69da0.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/managed_texture_data_uninitialise_candidate1_20261009.cpp'
obj=w/'destructor.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
proof=check(obj,'?Uninitialise@CManagedTextureData@@QAEXXZ',0xa69da0,65,2,{'??3@YAXPAX@Z':0xbfe9bc,'?Uninitialise@CTexture@@QAEXXZ':0x9f9f70})
body=read_virtual_data(EXE.read_bytes(),0xa69da0,65);fixups=[];cs=Cs(CS_ARCH_X86,CS_MODE_32);external={0xbfe9bc:0,0x9f9f70:1}
for ins in cs.disasm(body,0xa69da0):
 if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
  target=int(ins.op_str,16);assert target in external;fixups.append((ins.address-0xa69da0+1,external[target],20))
assert len(fixups)==2
(w/'managed_texture_uninitialise_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases={'@ManagedUninitialise@8':'?Uninitialise@CManagedTextureData@@QAEXXZ','?Uninitialise@CTexture@@QAEXXZ':'@TextureShutdownBoundary@8'}
(w/'managed_texture_uninitialise_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedTextureDataUninitialise_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=1536 ' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_managed_texture_data.h','fable_resource_list.h','fable_counted_pointer.h','rebuild_abi.h')]
report=dict(cases=1536,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Managed-data shutdown64B/65B original96.1538percent near firstcandidate. Original drops ordinary sharedcontrol count withunsigned wrap, reloadsInfo afterstoreddeleter, deletesliveInfo,clearsInfo/Data/D3DHeader/backingData/AllocSize, then tailcalls texture shutdown withNULL receiver. Functional storeddeleter/delete/texture shutdown boundaries; gate explicitly requiresNULL receiver and doesnot claim usable full realtexture shutdown, whose existing PC body dereferencesreceiver.1536cases/raw76Bfields/NULL-two-controls/refcounts endpoints/callbackInfo swap/field mutation/two placements-fills/full16KiB callback-final snapshots,one requiredsource/omission. No safeNULL check invented andno realheap/GPU shutdown claim.')
(ROOT/'rebuild/compile-gate/native-managed-texture-data-uninitialise.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_TEXTURE_DATA_UNINITIALISE_GATE_PASS')

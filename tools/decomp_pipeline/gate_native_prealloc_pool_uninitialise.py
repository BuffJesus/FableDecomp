import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-pool-uninitialise-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a6/CPreallocTexturePool_Uninitialise_00a6a4d0.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/prealloc_pool_uninitialise_candidate1_20261009.cpp'
obj=w/'destructor.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
targets={'??3@YAXPAX@Z':0xbfe9bc,'?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,'?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z':0x9fc020}
c=Coff(obj);sec,off=c.symbols['?Uninitialise@CPreallocTexturePool@@QAEXXZ'];vector=[sym for sym,kind in c.sections[sec][1].values() if sym.startswith('?DeallocateMemory@')];assert len(vector)==1;targets[vector[0]]=0xa6be40
proof=check(obj,'?Uninitialise@CPreallocTexturePool@@QAEXXZ',0xa6a4d0,111,4,targets)
body=read_virtual_data(EXE.read_bytes(),0xa6a4d0,111);fixups=[];cs=Cs(CS_ARCH_X86,CS_MODE_32);external={0x9fc020:0,0xbfe9bc:1,0x9f9f70:2,0xa6be40:3}
for ins in cs.disasm(body,0xa6a4d0):
 if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
  target=int(ins.op_str,16);assert target in external;fixups.append((ins.address-0xa6a4d0+1,external[target],20))
assert len(fixups)==4
(w/'pool_uninitialise_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases={'@PoolUninitialise@8':'?Uninitialise@CPreallocTexturePool@@QAEXXZ','?Uninitialise@CTexture@@QAEXXZ':'@TextureBoundary@8','?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z':'@RemoveBoundary@12',vector[0]:'@VectorBoundary@8'}
(w/'pool_uninitialise_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativePreallocPoolUninitialise_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=3072 ' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CPreallocTexturePool.h','fable_managed_texture_data.h','fable_resource_list.h','fable_counted_pointer.h','rebuild_abi.h')]
report=dict(cases=3072,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Pool shutdown110B/111B near97.8261percent firstcandidate,46/46instructions. Cached selecteddata survives resource-removal callbacks; liveCount andlivevector reloaded nextiteration. Original sharedhandle cleanup inlined,countswrap/reloadInfo afterdeleter,clearsordinaryhandle/header/backing/allocation, NULL-receiver texture shutdown preserved; Countzero before tailvector deallocation, callback Countmutation aftertailvisible.3072cases/full16KiB callback/final snapshots,count0/1/3,shared/distinct controls,rawcounts,8callback modes shrinkCount/replacevector/replaceInfo/fieldmutations,two placements-fills. Resource removal/storeddeleter/delete/textureNULLshutdown/vector deallocation are functionalboundaries; no usable full real shutdown/GPU/realheap claim andno helper coverage. One actualsource/omission; NULL PCquirk preserved withoutguard.')
(ROOT/'rebuild/compile-gate/native-prealloc-pool-uninitialise.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_PREALLOC_POOL_UNINITIALISE_GATE_PASS')

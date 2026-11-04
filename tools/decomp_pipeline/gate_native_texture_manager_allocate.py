import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-texture-manager-allocate-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a6/CTextureManager_Allocate_00a69b70.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/texture_manager_allocate_candidate2_20261009.cpp'
texture='?AllocateTexture@CTextureManager@@QAE_NAAVCManagedTexture@@KKIABVCPixelFormat@@PAVCManagedTextureOwner@@@Z';volume='?AllocateVolumeTexture@CTextureManager@@QAE_NAAVCManagedTexture@@KKKIABVCPixelFormat@@PAVCManagedTextureOwner@@@Z'
size='?CalculateTextureSize@CTextureManager@@SIKKK_NABVCPixelFormat@@@Z';index='?GetPoolIndex@CTextureManager@@AAEJK@Z';pool='?Allocate@CPreallocTexturePool@@QAEPAVCManagedTextureData@@XZ';create='?CreateTexture@CManagedTextureData@@QAE_NKKIABVCPixelFormat@@@Z';create_volume='?CreateVolumeTexture@CManagedTextureData@@QAE_NKKKIABVCPixelFormat@@@Z'
targets={size:0xa698f0,index:0xa69730,pool:0xa69830,create:0xa697a0,create_volume:0xa699d0};external={0xa698f0:0,0xa69730:1,0xa69830:2,0xa697a0:3,0xa699d0:4}
specs=[(texture,0xa69b70,176,0),(volume,0xa69c20,186,256)];obj=w/'allocate.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
proofs=[check(obj,symbol,address,length,4,targets) for symbol,address,length,offset in specs];body=bytearray(512);fixups=[];raw=EXE.read_bytes()
for symbol,address,length,offset in specs:
 b=read_virtual_data(raw,address,length);body[offset:offset+length]=b
 for ins in Cs(CS_ARCH_X86,CS_MODE_32).disasm(b,address):
  if ins.mnemonic=='call' and ins.bytes[0]==0xe8:target=int(ins.op_str,16);assert target in external;fixups.append((offset+ins.address-address+1,external[target],20))
assert len(fixups)==8
(w/'texture_manager_allocate_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases={'@AllocateTextureRoot@32':texture,'@AllocateVolumeRoot@36':volume,size:'@SizeBoundary@16',index:'@IndexBoundary@12',pool:'@PoolBoundary@8',create:'@TextureCreateBoundary@24',create_volume:'@VolumeCreateBoundary@28'}
(w/'texture_manager_allocate_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeTextureManagerAllocate_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=36864 snapshots=' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CTextureManager.h','engine/CPreallocTexturePool.h','fable_managed_texture.h','fable_managed_texture_data.h','fable_iv_counted_pointer.h','fable_resource_list.h','rebuild_abi.h')]
report=dict(cases=36864,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Both complete manager allocation roots acceptednear on candidate2:ordinary170B/176B94.964percent andvolume180B/186B95.1724percent,insdelta4.23/4.05percent. One actual required TU/two canonical methods. Size calculation/index lookup/pool selection/managed creation/old virtualdeleter are functional boundaries inthis root-only gate, their previousseparate recovery not claimed newly composed. Texture forcesmip-size true;volume false thenunsigned32bit depthmultiply; poolindex-1 returnsfalse withoutincrement; poolNULL orcreatefalse incrementsselectedFailedAllocations withwrap/livepostcallbackvalue;success setsOwner beforeoldhandle refcount decrement/zero virtualdelete, clears/storescachedselecteddata andincrements liveRefCount; no same-pointer identityguard. 36864cases/full16KiB call/final snapshots, indices-1/0/3/15/84B stride, two selecteddata/null, oldhandleNULL/same/other,rawcounts0/1/2/ffffffff/newcountpatterns/callbackmanager-format-handle-pointer-owner-counter mutations,ownerNULL/valid/rawffffffff,two placements-fills. Only both roots addnearcoverage; no helpers/data.')
(ROOT/'rebuild/compile-gate/native-texture-manager-allocate.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_TEXTURE_MANAGER_ALLOCATE_GATE_PASS')

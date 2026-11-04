import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-prealloc-texture-pool-allocate-'));env=V.env()
specs=[('00/a6/CPreallocTexturePool_Allocate_00a69830.cpp','?Allocate@CPreallocTexturePool@@QAEPAVCManagedTextureData@@XZ',0xa69830,125,0,3),('00/9f/CResource_SetAsMRU_009fc4a0.cpp','?SetAsMRU@CResource@@QAEXXZ',0x9fc4a0,80,192,0),('00/9f/CTexture_IsBusy_009f9d50.cpp','?IsBusy@CTexture@@QBE_NXZ',0x9f9d50,3,320,0)]
global_symbol='?ReuseBusyTextures@CTextureManager@@0_NA';targets={sym:address for rel,sym,address,size,off,count in specs};targets[global_symbol]=0x13d28e8;offsets={address:off for rel,sym,address,size,off,count in specs};offsets[0x13d28e8]=0x80000000
objects=[];sources=[];coffs=[];proofs=[];body=bytearray(384);fixups=[];cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;raw=EXE.read_bytes()
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/prealloc_texture_pool_allocate_candidate1_20261009.cpp'
 obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 objects.append(obj);sources.append(source);coffs.append(Coff(obj));proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16);assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
assert len(fixups)==3,fixups
source=ROOT/'rebuild/src/compiled/01/3d/CTextureManager_ReuseBusyTextures_013d28e8.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/texture_manager_reuse_busy_candidate_20261009.cpp'
obj=w/'reuse.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
assert coff_data(Coff(obj),global_symbol,1)==read_virtual_data(raw,0x13d28e8,1)==bytes(1);objects.append(obj);sources.append(source)
(w/'prealloc_texture_pool_allocate_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
(w/'prealloc_texture_pool_allocate_aliases.h').write_text('#pragma comment(linker,"/alternatename:@PoolAllocate@8='+specs[0][1]+'")\n#pragma comment(linker,"/alternatename:_ReuseBusyByte='+global_symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativePreallocTexturePoolAllocate_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in objects:
 r=subprocess.run(cmd+[str(o) for o in objects if o!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=122880 snapshots=' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CPreallocTexturePool.h','engine/CTextureManager.h','engine/CManagedTextureOwner.h','fable_managed_texture_data.h','fable_surface_types.h','fable_resource_list.h','fable_carray.h','rebuild_abi.h')]
report=dict(cases=122880,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,data=[dict(address='013d28e8',symbol=global_symbol,bytes=1,grade='DATA_MATCH')],omission_checks=4,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original125B pool Allocate near96.4286percent firstcandidate, actual80B/78B acceptednear SetAsMRU and3B PCfalse texturebusy plus originalprivate staticbool reuse byte. Four mandatoryactual sources/all omissions fail. Independent/native/compiled full16KiB callback/final snapshots, six list shapes(empty sentinel,NULLhead,single,three,detached,broken reciprocal),liveCount0/1/2/3/5,rawbusy-lock-allocation bytes0/1/80/ff,actualMRU intrusivepointer movement,actualPCqueryclearsbusy,temporary Lockedtrue fororiginalowner virtualslot10 thenfalse, returnonlyifAllocated cleared, liveCount reloaded eachiteration. Onlyowner-release callback boundary;eight actions(clear/leave/count shrink/owner replace/lock-busy mutation/head sentinel orNULL/global reuse mutation/release next candidate),two placements-fills. Originalreuse false byte full-size BSS audit, actualglobal value copiedinto snapshots. PCfalseleaf meansbusy-reuse-true path unreachable; no claim of a busy GPU implementation. Only poolfunction andnamedbyte newcoverage; existingMRU/leaf not recounted.')
(ROOT/'rebuild/compile-gate/native-prealloc-texture-pool-allocate.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_PREALLOC_TEXTURE_POOL_ALLOCATE_GATE_PASS')

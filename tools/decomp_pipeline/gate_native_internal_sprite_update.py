import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-internal-sprite-update-'));env=V.env()
specs=[('00/ba/CEngineInternalPrimitive2DSprite_Update_00bad8a0.cpp','internal_sprite_update_candidate_20261009.cpp','?Update@CEngineInternalPrimitive2DSprite@@UAEXABVCEnginePrimitive@@K@Z',0xbad8a0,671,10),('00/9f/CTexture_GetDimensions_009f9db0.cpp',None,'?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z',0x9f9db0,43,0)]
targets={s[2]:s[3] for s in specs};targets.update({'?DELTA_NOTIONAL_ZERO@@3MB':0x129ba3c,'?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A':0x1436e98,'?BeginLoading@CGraphicInfoFrame@@QBEXKK@Z':0x9fe620,'__real@4f800000':0x122dcb4});offsets={0xbad8a0:0,0x9f9db0:768,0x129ba3c:0xffffffff,0x1436e98:0xfffffffe,0x9fe620:0xfffffffd,0x122dcb4:0xfffffffc};body=bytearray(832);raw=EXE.read_bytes();fixups=[];objects=[];sources=[];proofs=[];cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
for rel,scratch,symbol,address,length,n in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if scratch and ('--stage' in sys.argv or not source.exists()):source=ROOT/'work/parity_addtext'/scratch
 sources.append(source);obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);proofs.append(check(obj,symbol,address,length,n,targets));b=read_virtual_data(raw,address,length);off=offsets[address];body[off:off+length]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+length:assert target in offsets;fixups.append((off+ins.address-address+ins.size-4,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((off+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
assert len(fixups)==10
(w/'internal_sprite_update_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
(w/'internal_sprite_update_aliases.h').write_text('#pragma comment(linker,"/alternatename:@InternalSpriteUpdate@16='+specs[0][2]+'")\n#pragma comment(linker,"/alternatename:_BeginLoadBoundary=?BeginLoading@CGraphicInfoFrame@@QBEXKK@Z")\n')
extras=[ROOT/'rebuild/src/compiled/01/29/Fable_DeltaNotionalZero_0129ba3c.cpp',ROOT/'rebuild/src/compiled/01/43/NEngineInternal_GPResourceManager_01436e98.cpp'];sources.extend(extras);fixture=ROOT/'rebuild/tests/integration/NativeInternalSpriteUpdate_test.cpp'
for i,path in enumerate(extras+[fixture]):
 obj=w/f'extra{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=45);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=29184 snapshots=115968' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineInternalPrimitive2DSprite.h','engine/CGraphicDataBank.h','fable_graphic_info.h','fable_graphic_info_frame.h','fable_2d_vector.h','fable_surface_types.h')]
report=dict(cases=29184,snapshots=115968,providers=proofs,omission_checks=4,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete internal sprite update and actual texture dimensions/DELTA/resource global;four mandatory providers. FrameBeginLoading and COMGetLevelDesc callbacks;lookup andvectorthreshold are actual inline code. Independent full64KiB/global snapshots aftercallbacks/final;native/compiled allbyte equality,model clipNaNs classified. Bank/directtexture/nullframe,source aliases own internalfields,callbackinput/frame/texture/global mutations,rawfloat/unsigned dimensions,DELTA-neighbor threshold cases with zero othercomponents,three x87 precisions,two placements/fills.')
(ROOT/'rebuild/compile-gate/native-internal-sprite-update.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_INTERNAL_SPRITE_UPDATE_GATE_PASS')

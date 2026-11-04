import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-graphic-frame-reset-lru-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9f/CGraphicInfoFrame_CFrameData_ResetTexture_009fe440.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/graphic_frame_reset_lru_candidate_20261009.cpp'
symbol='?ResetTexture@CFrameData@CGraphicInfoFrame@@QAEXXZ';targets={'?Release@CManagedTexture@@QAE_NXZ':0xa69ad0,'?UnInitialise@CGraphicFrame@@QAEXXZ':0xa00b90}
obj=w/'loader.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
proof=check(obj,symbol,0x9fe440,29,2,targets);rootBody=read_virtual_data(EXE.read_bytes(),0x9fe440,29);body=bytearray(608);body[:29]=rootBody;body[64:137]=read_virtual_data(EXE.read_bytes(),0xa69ad0,73);body[160:254]=read_virtual_data(EXE.read_bytes(),0xa00b90,94);body[288:357]=read_virtual_data(EXE.read_bytes(),0x9fcba0,69);body[400:476]=read_virtual_data(EXE.read_bytes(),0xa69a80,76);body[512:590]=read_virtual_data(EXE.read_bytes(),0x9fc450,78);cs=Cs(CS_ARCH_X86,CS_MODE_32);fixups=[];addresses=list(targets.values())
for ins in cs.disasm(rootBody,0x9fe440):
 if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
  target=int(ins.op_str,16);assert target in addresses;fixups.append((ins.address-0x9fe440+1,addresses.index(target)))
assert len(fixups)==2
cleanupFixups=[]
for ins in cs.disasm(body[160:254],0xa00b90):
 if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
  target=int(ins.op_str,16);assert target in (0x9fcba0,0xbfe9bc);cleanupFixups.append((ins.address-0xa00b90+1,(0x9fcba0,0xbfe9bc).index(target)))
assert len(cleanupFixups)==3
textureFixups=[]
for ins in cs.disasm(body[400:476],0xa69a80):
 if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
  target=int(ins.op_str,16);assert target in (0xbfe9bc,0x9fc450);textureFixups.append((ins.address-0xa69a80+1,(0xbfe9bc,0x9fc450).index(target)))
assert len(textureFixups)==3
(w/'graphic_frame_reset_lru_retail.h').write_text('struct Fixup{unsigned site,target;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u}'%f for f in fixups)+'};\nstatic const Fixup cleanupFixups[]={'+','.join('{%u,%u}'%f for f in cleanupFixups)+'};\nstatic const Fixup textureFixups[]={'+','.join('{%u,%u}'%f for f in textureFixups)+'};\n')
(w/'graphic_frame_reset_lru_aliases.h').write_text('#pragma comment(linker,"/alternatename:@FrameReset@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeGraphicFrameResetLRU_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
releaseSource=ROOT/'rebuild/src/compiled/00/a6/CManagedTexture_Release_00a69ad0.cpp';ro=w/'release.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(ro),str(releaseSource)],env,w);assert not r.returncode,r.stdout+r.stderr
releaseProof=check(ro,'?Release@CManagedTexture@@QAE_NXZ',0xa69ad0,73,1,{'?ReleaseTexture@CManagedTextureData@@QAEXXZ':0xa69a80});cleanupSource=ROOT/'rebuild/src/compiled/00/a0/CGraphicFrame_UnInitialise_00a00b90.cpp';co=w/'cleanup.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(co),str(cleanupSource)],env,w);assert not r.returncode,r.stdout+r.stderr
cleanupProof=check(co,'?UnInitialise@CGraphicFrame@@QAEXXZ',0xa00b90,94,3,{'?Uninitialise@CGraphicFrameData@@QAEXXZ':0x9fcba0,'??3@YAXPAX@Z':0xbfe9bc});dataSource=ROOT/'rebuild/src/compiled/00/9f/CGraphicFrameData_Uninitialise_009fcba0.cpp';do=w/'data.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(do),str(dataSource)],env,w);assert not r.returncode,r.stdout+r.stderr
dataProof=check(do,'?Uninitialise@CGraphicFrameData@@QAEXXZ',0x9fcba0,69,1,{'??3@YAXPAX@Z':0xbfe9bc});textureSource=ROOT/'rebuild/src/compiled/00/a6/CManagedTextureData_ReleaseTexture_00a69a80.cpp';to=w/'texture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(to),str(textureSource)],env,w);assert not r.returncode,r.stdout+r.stderr
textureProof=check(to,'?ReleaseTexture@CManagedTextureData@@QAEXXZ',0xa69a80,76,3,{'??3@YAXPAX@Z':0xbfe9bc,'?SetAsLRU@CResource@@QAEXXZ':0x9fc450});lruSource=ROOT/'rebuild/src/compiled/00/9f/CResource_SetAsLRU_009fc450.cpp';lo=w/'lru.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(lo),str(lruSource)],env,w);assert not r.returncode,r.stdout+r.stderr
lruProof=check(lo,'?SetAsLRU@CResource@@QAEXXZ',0x9fc450,78,0,{});objects=[obj,ro,co,do,to,lo]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(6):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=49152 snapshots=158592' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','engine/CGraphicDataBank.h','engine/CManagedTextureOwner.h','fable_managed_texture.h','fable_managed_texture_data.h','engine/CGraphicFrame.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_game_text_bank.h','fable_counted_pointer.h','fable_resource_list.h','fable_iv_counted_pointer.h')]
report=dict(cases=49152,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof,releaseProof,cleanupProof,dataProof,textureProof,lruProof],omission_checks=6,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source,releaseSource,cleanupSource,dataSource,textureSource,lruSource]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete reset with actual managed Release and frame UnInitialise,six mandatory providers. virtualDelete,counted-pointee deletion andheap delete boundaries. Independent/native/compiled full16KiB aftercallbacks/final;data/controlblock/owner/mip mutations,reference wrap,cachedframe object,pointerreload,final owner andmip overwrites,two placements/fills.')
(ROOT/'rebuild/compile-gate/native-graphic-frame-reset-lru.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_GRAPHIC_FRAME_RESET_LRU_GATE_PASS')

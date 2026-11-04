import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-graphic-info-unload-frame-'));env=V.env()
specs=[('00/9f/CGraphicInfo_UnloadFrame_009fe8e0.cpp','?UnloadFrame@CGraphicInfo@@QAEXK@Z',0x9fe8e0,277,0,5),('00/9f/CResource_GetUnusedFrames_009fbf70.cpp','?GetUnusedFrames@CResource@@QBEKXZ',0x9fbf70,18,384,0)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'?StopLoading@CFrameData@CGraphicInfoFrame@@QAE_NXZ':0x9fdc20,'?FreeTextureMemory@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z':0x9fe460});offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x9fdc20:0x80000000,0x9fe460:0x80000001})

objects=[];sources=[];proofs=[];coffs=[];body=bytearray(448);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/graphic_info_unload_frame_candidate_20261009.cpp'
 obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 objects.append(obj);sources.append(source);coffs.append(Coff(obj));proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
assert len(fixups)==5
(w/'graphic_info_unload_frame_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@UnloadFrame@12='+specs[0][1]+'")']
(w/'graphic_info_unload_frame_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeGraphicInfoUnloadFrame_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=138240' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','engine/CGraphicDataBank.h','engine/CManagedTextureOwner.h','fable_managed_texture.h','fable_managed_texture_data.h','engine/CGraphicFrame.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_game_text_bank.h','fable_counted_pointer.h','fable_resource_list.h','fable_iv_counted_pointer.h','fable_base_class.h')]
report=dict(cases=138240,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=2,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete277B GraphicInfo::UnloadFrame plusactual18B original GetUnusedFrames;two mandatory sources. StopLoading/FreeTextureMemory/virtualDelete callbacks remain. Independent/native/compiled full16KiB callback/final snapshots;cached frame count andcurrent frame reference,frame-array reload eachiteration,cached owner slot throughdelete,zero/single/multi frame path,firstfailedstop,minunsignedage/threshold/refwrap,callback frame-array/data/count/bank/age mutation,two placements/fills. Controlled count0/1 frame indices0/1/3 have allocated backing slots; no bounds-safetyclaim.')
(ROOT/'rebuild/compile-gate/native-graphic-info-unload-frame.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_GRAPHIC_INFO_UNLOAD_FRAME_GATE_PASS')

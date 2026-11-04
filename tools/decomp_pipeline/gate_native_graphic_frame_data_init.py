import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-graphic-frame-data-init-'));env=V.env()
specs=[('00/9f/CGraphicInfoFrame_CFrameData_Constructor_009fe380.cpp','??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z',0x9fe380,181,0,10),('00/9f/CResourceBank_AddResource_009fc380.cpp','?AddResource@CResourceBank@@QAEXAAVCResource@@@Z',0x9fc380,52,256,0),('00/9f/CResource_SetResourceSize_009fbf20.cpp','?SetResourceSize@CResource@@QAEXK@Z',0x9fbf20,46,384,0),('00/9f/CResourceBank_Lifetime_009fc5f0.cpp','??0CResource@@QAE@XZ',0x9fbec0,33,448,1),('00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??0CBaseClass@@QAE@XZ',0x99a2d0,9,496,1)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs}
targets.update({'??_7CFrameData@CGraphicInfoFrame@@6B@':0x129c89c,'??_7CManagedTexture@@6B@':0x129c880,'??_7CResource@@6B@':0x12354a4,'??_7CBaseClass@@6B@':0x1231710,'?Sizeof@CGraphicFrameData@@QBEKXZ':0x9fcbf0,'?Sizeof@CManagedTexture@@QBEKXZ':0xa69e60})
addresses=[0x9fbec0,0x99a2d0,0x129c89c,0x129c880,0x9fcbf0,0xa69e60,0x9fbf20,0x9fc380]
objects=[];sources=[];proofs=[];coffs=[];body=bytearray(512);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel;obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 objects.append(obj);sources.append(source);coffs.append(Coff(obj));proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert offset==0 and target in addresses;fixups.append((ins.address-address+1,addresses.index(target),20))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in targets.values():
     site=offset+ins.address-address+ins.imm_offset
     if offset==0:assert operand.imm in addresses;fixups.append((site,addresses.index(operand.imm),6))
     else:initFixups.append((site,operand.imm))
assert len(fixups)==10 and initFixups==[(461,0x12354a4),(500,0x1231710)]
(w/'graphic_frame_data_init_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@FrameDataConstructor@12='+specs[0][1]+'")']
for local,symbol in [('FrameVTable','??_7CFrameData@CGraphicInfoFrame@@6B@'),('ManagedVTable','??_7CManagedTexture@@6B@'),('ResourceVTable','??_7CResource@@6B@'),('BaseVTable','??_7CBaseClass@@6B@')]:aliases.append('#pragma comment(linker,"/alternatename:_'+local+'='+symbol+'")')
defined={name for c in coffs for name in c.symbols};undefined={name for c in coffs for data,rels in c.sections.values() for name,kind in rels.values() if name not in defined and name not in ('??3@YAXPAX@Z','__purecall','??_7type_info@@6B@')}
for name in sorted(undefined):aliases.append('#pragma comment(linker,"/alternatename:'+name+'=_AbortBoundary")')
(w/'graphic_frame_data_init_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeGraphicFrameDataInit_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(5):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=24576 snapshots=49152' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','engine/CGraphicDataBank.h','engine/CManagedTextureOwner.h','fable_managed_texture.h','fable_managed_texture_data.h','engine/CGraphicFrame.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_game_text_bank.h','fable_counted_pointer.h','fable_resource_list.h','fable_iv_counted_pointer.h','fable_base_class.h')]
report=dict(cases=24576,snapshots=49152,providers=proofs,omission_checks=5,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual frame-data constructor,resource/base constructors,size accounting andbank registration;five mandatory providers,no behavioral callbacks. Independent/native/compiled full64KiB final snapshots;all256 LoadingMip bytes,raw frame/count/accounting wrap,empty/live tails,two banks/placements/fills. Abort-only aliases cover unexecuted size-query branches andunused virtual/destructor/link dependencies;executing any fails. Real constructors prove frame/loading-texture pointers zero before size queries.')
(ROOT/'rebuild/compile-gate/native-graphic-frame-data-init.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_GRAPHIC_FRAME_DATA_INIT_GATE_PASS')

import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-frame-data-destructor-'));env=V.env()
specs=[('00/9f/CGraphicInfoFrame_CFrameData_Destructor_009fe6c0.cpp','??1CFrameData@CGraphicInfoFrame@@UAE@XZ',0x9fe6c0,182,0,12)]
targets={'??_7CFrameData@CGraphicInfoFrame@@6B@':0x129c89c,'?Release@CManagedTexture@@QAE_NXZ':0xa69ad0,'?UnInitialise@CGraphicFrame@@QAEXXZ':0xa00b90,'??3@YAXPAX@Z':0xbfe9bc,'??1CManagedTexture@@UAE@XZ':0xa69e20,'?Uninitialise@CGraphicFrameData@@QAEXXZ':0x9fcba0,'??1CResource@@UAE@XZ':0x9fc550,'??_7CResource@@6B@':0x12354a4,'?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z':0x9fc020,'??_7CIVCountedPointeeBase@@6B@':0x12354d0}
offsets={0x9fc550:256,0xa69ad0:0x80000000,0xa00b90:0x80000001,0xbfe9bc:0x80000002,0xa69e20:0x80000003,0x9fcba0:0x80000004,0x9fc020:0x80000005,0x129c89c:0x80000006,0x12354a4:0x80000007,0x12354d0:0x80000008}

objects=[];sources=[];proofs=[];coffs=[];body=bytearray(320);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/frame_data_destructor_candidate_20261009.cpp'
 obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 objects.append(obj);sources.append(source);coffs.append(Coff(obj));proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
base=read_virtual_data(raw,0x9fc550,30);body[256:286]=base
for ins in cs.disasm(base,0x9fc550):
 if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
  target=int(ins.op_str,16);assert target in offsets;fixups.append((256+ins.address-0x9fc550+1,offsets[target],20))
 if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
  for operand in ins.operands:
   if operand.type==2 and operand.imm in offsets:fixups.append((256+ins.address-0x9fc550+ins.imm_offset,offsets[operand.imm],6))
assert len(fixups)==13
(w/'frame_data_destructor_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@FrameDataDestructor@8='+specs[0][1]+'")']
for a,b in {'_FrameVTable':'??_7CFrameData@CGraphicInfoFrame@@6B@','_ResourceVTable':'??_7CResource@@6B@','_PointeeVTable':'??_7CIVCountedPointeeBase@@6B@','?Release@CManagedTexture@@QAE_NXZ':'?ManagedRelease@Boundary@@QAE_NXZ','?UnInitialise@CGraphicFrame@@QAEXXZ':'?FrameUninit@Boundary@@QAEXXZ','??1CManagedTexture@@UAE@XZ':'?ManagedDtor@Boundary@@QAEXXZ','?Uninitialise@CGraphicFrameData@@QAEXXZ':'?GraphicUninit@Boundary@@QAEXXZ','?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z':'?RemoveResource@Boundary@@QAEXAAVCResource@@@Z'}.items():aliases.append('#pragma comment(linker,"/alternatename:'+a+'='+b+'")')
undefined={name for coff in coffs for data,rels in coff.sections.values() for name,kind in rels.values() if name not in coff.symbols and name not in targets and name not in ('??3@YAXPAX@Z','__purecall')}
for name in sorted(undefined):aliases.append('#pragma comment(linker,"/alternatename:'+name+'=_AbortBoundary")')
(w/'frame_data_destructor_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeFrameDataDestructor_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(1):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=6912' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','engine/CGraphicDataBank.h','engine/CManagedTextureOwner.h','fable_managed_texture.h','fable_managed_texture_data.h','engine/CGraphicFrame.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_game_text_bank.h','fable_counted_pointer.h','fable_resource_list.h','fable_iv_counted_pointer.h','fable_base_class.h')]
report=dict(cases=6912,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete182B frame-data destructor accepted near with canonical inline GraphicFrame destructor and compiled inline Resource destructor;native executes original30B Resource destructor tail. One mandatory production source. ManagedRelease/FrameUnInitialise/ManagedDestructor/GraphicFrameDataUninitialise/countDelete/heap/ResourceListRemove remain boundaries;abort aliases only unused vtable methods. Independent/native/compiled full16KiB callback/final snapshots including compiler vtable identities without masks;file Info null/live despiteData null,rawrefwrap,mipreset,member destruction order,callback reintroduces frame afterreset/loading/managed dtor,frame/controlblock pointer caching/reloads,resourcevptr/list callback/finalpointee vptr,two placements/fills. New inline Frame destructor regression-tested against existing ctor/sprite loading/full six-source reset/freeTexture gates;no separate inline coverage.')
(ROOT/'rebuild/compile-gate/native-frame-data-destructor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_FRAME_DATA_DESTRUCTOR_GATE_PASS')

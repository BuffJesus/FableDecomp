import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from gate_game_component_abi import Coff
w=Path(tempfile.mkdtemp(prefix='native-graphic-frame-data-ctor-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9f/CGraphicInfoFrame_CFrameData_Constructor_009fe380.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/graphic_frame_data_ctor_candidate_20261009.cpp'
symbol='??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z';targets={'??0CResource@@QAE@XZ':0x9fbec0,'??0CBaseClass@@QAE@XZ':0x99a2d0,'??_7CFrameData@CGraphicInfoFrame@@6B@':0x129c89c,'??_7CManagedTexture@@6B@':0x129c880,'?Sizeof@CGraphicFrameData@@QBEKXZ':0x9fcbf0,'?Sizeof@CManagedTexture@@QBEKXZ':0xa69e60,'?SetResourceSize@CResource@@QAEXK@Z':0x9fbf20,'?AddResource@CResourceBank@@QAEXAAVCResource@@@Z':0x9fc380}
obj=w/'loader.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
proof=check(obj,symbol,0x9fe380,181,10,targets);body=read_virtual_data(EXE.read_bytes(),0x9fe380,181);cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];addresses=list(targets.values())
for ins in cs.disasm(body,0x9fe380):
 if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
  target=int(ins.op_str,16);assert target in addresses;fixups.append((ins.address-0x9fe380+1,addresses.index(target),20))
 if ins.disp_size==4 and ins.disp in addresses:fixups.append((ins.address-0x9fe380+ins.disp_offset,addresses.index(ins.disp),6))
 if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
  for operand in ins.operands:
   if operand.type==2 and operand.imm in addresses:fixups.append((ins.address-0x9fe380+ins.imm_offset,addresses.index(operand.imm),6))
assert len(fixups)==10
(w/'graphic_frame_data_ctor_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@FrameDataConstructor@12='+symbol+'")','#pragma comment(linker,"/alternatename:_FrameVTable=??_7CFrameData@CGraphicInfoFrame@@6B@")','#pragma comment(linker,"/alternatename:_ManagedVTable=??_7CManagedTexture@@6B@")']
boundaries=['@ResourceCtor@8','@BaseCtor@8',None,None,'@GraphicSize@8','@ManagedSize@8','@SetSize@12','@AddResource@12']
for name,target in zip(targets,boundaries):
 if target:aliases.append('#pragma comment(linker,"/alternatename:'+name+'='+target+'")')
c=Coff(obj);undefined={name for body,rels in c.sections.values() for name,kind in rels.values() if name not in c.symbols and name not in targets and name not in ('??3@YAXPAX@Z','__purecall')}
for name in sorted(undefined):aliases.append('#pragma comment(linker,"/alternatename:'+name+'=_AbortBoundary")')
(w/'graphic_frame_data_ctor_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeGraphicFrameDataCtor_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=16000 snapshots=216000' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','engine/CGraphicDataBank.h','engine/CManagedTextureOwner.h','fable_managed_texture.h','fable_managed_texture_data.h','engine/CGraphicFrame.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_game_text_bank.h','fable_counted_pointer.h','fable_resource_list.h','fable_iv_counted_pointer.h')]
report=dict(cases=16000,snapshots=216000,providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete181B constructor; independent/native/compiled full64KiB callback/final snapshots. Resource/base constructors, frame/managed size queries, SetResourceSize and AddResource are boundaries. Unsigned frame age/size wrap, callback field/bank mutation and reload, padding preservation, two placements/fills. Abort-only aliases resolve unused virtual/destructor methods; executing them fails. Typed original layout, compiler vtables and return-this ABI verified.')
(ROOT/'rebuild/compile-gate/native-graphic-frame-data-ctor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_GRAPHIC_FRAME_DATA_CTOR_GATE_PASS')

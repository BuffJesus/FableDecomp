import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-volume-copy-'));env=V.env()
specs=[('00/a6/CVolume_CopyConstructor_00a673e0.cpp','??0CVolume@@QAE@ABV0@@Z',0xa673e0,40,0,2),('00/9f/CVolume_Destructor_009fccf0.cpp','??1CVolume@@UAE@XZ',0x9fccf0,24,128,3),('00/a6/CVolume_Uninitialise_00a67460.cpp','?Uninitialise@CVolume@@QAEX_N@Z',0xa67460,27,192,0),('00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??0CBaseClass@@QAE@XZ',0x99a2d0,9,240,1),('00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??1CBaseClass@@UAE@XZ',0x99a2e0,7,256,1)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'??_7CVolume@@6B@':0x129c840,'??_7CBaseClass@@6B@':0x1231710})
offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x129c840:0x80000000,0x1231710:0x80000001})

objects=[];sources=[];proofs=[];coffs=[];body=bytearray(320);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[];compiled={}
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/volume_copy_candidate_20261009.cpp'
 obj=compiled.get(source)
 if obj is None:
  obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;compiled[source]=obj
  objects.append(obj);sources.append(source);coffs.append(Coff(obj))
 proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
assert len(fixups)==7
(w/'volume_copy_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@VolumeCopy@12='+specs[0][1]+'")','#pragma comment(linker,"/alternatename:_VolumeVTable=??_7CVolume@@6B@")','#pragma comment(linker,"/alternatename:_BaseVTable=??_7CBaseClass@@6B@")']
defined={name for c in coffs for name in c.symbols};undefined={name for c in coffs for data,rels in c.sections.values() for name,kind in rels.values() if name not in defined and name not in ('??2@YAPAXI@Z','??3@YAXPAX@Z','__purecall','??_7type_info@@6B@','?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z')}
for name in sorted(undefined):aliases.append('#pragma comment(linker,"/alternatename:'+name+'=_AbortBoundary")')
(w/'volume_copy_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeCopy_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(4):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=768 snapshots=2560' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_d3d9.h')]
report=dict(cases=768,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=4,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete40B original CVolume copyconstructor plusactual9B BaseCtor,24B VolumeDtor,27B Uninitialise and7B BaseDtor;four mandatorysources. OnlyCOMAddRef callback,unusedvirtualaliasesabort. Independent/native/compiled full16KiB callback/final snapshots andreturnthis identity;null/twopointees/selfsource,source-target/interface/vtable replacements,AddRef ignoredreturn,noextra reloadaftercallback,twoplacements/fills. Original CVolume/BaseClass vtable identities preservedwithout masks;dtorproviders mandatoryfor actualvtable andpreviouslyindependentlyproved.')
(ROOT/'rebuild/compile-gate/native-volume-copy.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VOLUME_COPY_GATE_PASS')

import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-volume-scalar-destructor-'));env=V.env()
specs=[('00/9f/CVolume_Destructor_009fccf0.cpp','??_GCVolume@@UAEPAXI@Z',0x9fcd10,45,0,4),('00/a6/CVolume_Uninitialise_00a67460.cpp','?Uninitialise@CVolume@@QAEX_N@Z',0xa67460,27,128,0),('00/99/CBaseClass_CBaseClass_0099a2d0.cpp','??1CBaseClass@@UAE@XZ',0x99a2e0,7,240,1)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets.update({'??3@YAXPAX@Z':0xbfe9bc,'??_7CVolume@@6B@':0x129c840,'??_7CBaseClass@@6B@':0x1231710})
offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x129c840:0x80000000,0x1231710:0x80000001,0xbfe9bc:0x80000002})

objects=[];sources=[];proofs=[];coffs=[];body=bytearray(256);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/volume_scalar_destructor_candidate_20261009.cpp'
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
volumeSection,volumeOffset=coffs[0].symbols['??_7CVolume@@6B@'];volumeData,volumeRelocs=coffs[0].sections[volumeSection];assert volumeOffset==0 and len(volumeData)==4 and volumeRelocs=={0:('??_ECVolume@@UAEPAXI@Z',6)} and read_virtual_data(raw,0x129c840,4)==bytes.fromhex('10cd9f00')
(w/'volume_scalar_destructor_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases=['#pragma comment(linker,"/alternatename:@VolumeScalarDestructor@12='+specs[0][1]+'")','#pragma comment(linker,"/alternatename:_VolumeVTable=??_7CVolume@@6B@")','#pragma comment(linker,"/alternatename:_BaseVTable=??_7CBaseClass@@6B@")']
defined={name for c in coffs for name in c.symbols};undefined={name for c in coffs for data,rels in c.sections.values() for name,kind in rels.values() if name not in defined and name not in ('??2@YAPAXI@Z','??3@YAXPAX@Z','__purecall','??_7type_info@@6B@','?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z')}
for name in sorted(undefined):aliases.append('#pragma comment(linker,"/alternatename:'+name+'=_AbortBoundary")')
(w/'volume_scalar_destructor_aliases.h').write_text('\n'.join(aliases)+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeScalarDestructor_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=3072 snapshots=13312' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_d3d9.h')]
report=dict(cases=3072,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=3,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete45B original CVolume scalar-deleting destructor generatedby genuineinlinevirtualC++ destructor;originalVolumeVTable129C840 firstentry9FCD10 confirmed,compiler4B vtable hasoneweakvector-delete relocation;runtimeassertion verifiesitsfallback address equalsactualscalar-delete provider. Actual27B Uninitialise/7B BaseDtor,three mandatorysources. OnlyCOMRelease/heapdelete callbacks;unusedGetLevel/Init/Copy virtual/helperaliasesabort. Independent/native/compiled full16KiB snapshots andreturnthis identity;flagsraw32bits onlybit0controlsheapdelete,null/twopointees/rawpayload/slotandvtable replacements,clearPD/BaseVTbeforeheapcallback,heapwritesobjectbeforecachedselfreturn,two placements/fills. No additionalvtable/data/inline-ctor coverage.')
(ROOT/'rebuild/compile-gate/native-volume-scalar-destructor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VOLUME_SCALAR_DESTRUCTOR_GATE_PASS')

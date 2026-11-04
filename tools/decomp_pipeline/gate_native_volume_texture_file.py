import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-volume-texture-file-'));env=V.env()
specs=[('00/a6/CVolumeTexture_FileWrappers_00a694e0.cpp','?SaveToFile@CVolumeTexture@@QAEXAAVCAFile@@@Z',0xa694e0,48,0,3),('00/a6/CVolumeTexture_FileWrappers_00a694e0.cpp','?LoadFromFile@CVolumeTexture@@QAEXAAVCAFile@@@Z',0xa69510,53,64,3)]
targets={'??0CFileDataOutputStream@@QAE@PAVCAFile@@@Z':0x994400,'??1CFileDataOutputStream@@UAE@XZ':0x994430,'?SaveToDataStream@CVolumeTexture@@QAEXAAVCDataOutputStream@@@Z':0xa68ae0,'??0CFileDataInputStream@@QAE@PAVCAFile@@K@Z':0x994700,'??1CFileDataInputStream@@UAE@XZ':0x994780,'?LoadFromDataStream@CVolumeTexture@@QAEXAAVCDataInputStream@@@Z':0xa68de0}
offsets={0x994400:0,0x994430:1,0xa68ae0:2,0x994700:3,0x994780:4,0xa68de0:5}

objects=[];sources=[];proofs=[];body=bytearray(128);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/volume_texture_file_candidate_20261009.cpp'
 obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
 sources.append(source);proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
 
 if not objects:objects.append(obj)
assert len(fixups)==6,fixups
(w/'volume_texture_file_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
(w/'volume_texture_file_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@SaveFile@12':specs[0][1],'@LoadFile@12':specs[1][1],'??0CFileDataOutputStream@@QAE@PAVCAFile@@@Z':'@OutputCtor@12','??1CFileDataOutputStream@@UAE@XZ':'@OutputDtor@8','?SaveToDataStream@CVolumeTexture@@QAEXAAVCDataOutputStream@@@Z':'@SaveStream@12','??0CFileDataInputStream@@QAE@PAVCAFile@@K@Z':'@InputCtor@16','??1CFileDataInputStream@@UAE@XZ':'@InputDtor@8','?LoadFromDataStream@CVolumeTexture@@QAEXAAVCDataInputStream@@@Z':'@LoadStream@12'}.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeTextureFile_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(1):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=768 snapshots=6144' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_extents_i.h','fable_surface_types.h','fable_d3d_format_info.h','fable_d3d9.h','fable_display_geometry.h','fable_volume_box.h','fable_d3dx9_texture.h','fable_data_input_stream.h','fable_data_output_stream.h','fable_file_data_output_stream.h','fable_filesystem.h')]
report=dict(cases=768,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Two original nonconst void wrappers SaveToFile(CAFile&)48B andLoadFromFile(CAFile&)53B, exact firstcandidate; typed originalreference prototypes verifiedPDB. Shared mandatorysource, constructor/serializer/destructor explicit boundaries. Save constructs12B outputstream, serializes, destroys; load constructs36B inputstreamwith16384B buffer, deserializes, destroys; cachedowner andfile, voidreturn. Full16KiB snapshots andcapturedtemporarybytes onlywhilealive, rawfile-referencebits/ownerfields/mutation/two placements-fills, no temporarypointerlogs orafterlife reads. No claimof underlyingserialization recovered')
(ROOT/'rebuild/compile-gate/native-volume-texture-file.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VOLUME_TEXTURE_FILE_GATE_PASS')

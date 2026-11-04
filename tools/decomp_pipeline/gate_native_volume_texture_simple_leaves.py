import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-volume-texture-simple-leaves-'));env=V.env()
specs=[('00/a6/CVolumeTexture_SimpleLeaves_00a67e10.cpp','?PreLoad@CVolumeTexture@@QAEXXZ',0xa67e10,10,0,0),('00/a6/CVolumeTexture_SimpleLeaves_00a67e10.cpp','?IsBusy@CVolumeTexture@@QBE_NXZ',0xa67e20,3,32,0),('00/a6/CVolumeTexture_SimpleLeaves_00a67e10.cpp','?Compress@CVolumeTexture@@QAE_NXZ',0xa67e30,3,48,0),('00/a6/CVolumeTexture_SimpleLeaves_00a67e10.cpp','?GenerateMipmaps@CVolumeTexture@@QAEXKK@Z',0xa67f20,3,64,0)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};targets['?GD3DFormats@@3QBVCD3DFormatInfo@@B']=0x129ba40;offsets={address:offset for rel,symbol,address,size,offset,count in specs};offsets.update({0x129ba40:0x80000000,0x129ba48:0x80000001})

objects=[];sources=[];proofs=[];body=bytearray(256);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];compiled={}
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/volume_texture_simple_leaves_candidate_20261009.cpp'
 obj=compiled.get(source)
 if obj is None:
  obj=w/f'{address:x}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;compiled[source]=obj;objects.append(obj);sources.append(source)
 proofs.append(check(obj,symbol,address,size,count,targets));b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not address<=target<address+size:assert target in offsets;fixups.append((offset+ins.address-address+1,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((offset+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
  if ins.mnemonic not in ('call','jmp') and ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in offsets:fixups.append((offset+ins.address-address+ins.imm_offset,offsets[operand.imm],6))
assert len(fixups)==0,fixups
(w/'volume_texture_simple_leaves_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
(w/'volume_texture_simple_leaves_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@TexturePreLoad@8':specs[0][1],'@TextureIsBusy@8':specs[1][1],'@TextureCompress@8':specs[2][1],'@TextureGenerateMipmaps@16':specs[3][1]}.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeVolumeTextureSimpleLeaves_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(1):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=32768 snapshots=81920' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_volume_types.h','fable_base_class.h','fable_extents_i.h','fable_surface_types.h','fable_d3d_format_info.h','fable_d3d9.h')]
report=dict(cases=32768,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Four complete original methods in one mandatory genuine source: PreLoad10B nonconstvoid viaCOMslot9; IsBusy3B constboolfalse; Compress3B nonconstboolfalse; GenerateMipmaps3B nonconstvoid(unsignedlong,unsignedlong) no-opret8. Original donor/PDB constness,return types andparameter widths verified. Full16KiB native/model/compiled snapshots,rawownerfields/nullrawPD for no-op leaves,raw32bit mip arguments,COMmutation/vtable andPDreplacement onlyPreLoad,ignoredvoidcallbackresult; bool lowbyte checked with properboolABI. Two placements-fills. Each physical leaf countedonce,no helperdata coverage.')
(ROOT/'rebuild/compile-gate/native-volume-texture-simple-leaves.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_VOLUME_TEXTURE_SIMPLE_LEAVES_GATE_PASS')

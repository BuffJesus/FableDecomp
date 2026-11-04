import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-managed-texture-busy-'));env=V.env()
specs=[('00/a6/CManagedTextureData_IsBusy_00a69810.cpp','?IsBusy@CManagedTextureData@@QAE_NXZ',0xa69810,26,0,1),('00/9f/CTexture_IsBusy_009f9d50.cpp','?IsBusy@CTexture@@QBE_NXZ',0x9f9d50,3,64,0)]
targets={symbol:address for rel,symbol,address,size,offset,count in specs};offsets={address:offset for rel,symbol,address,size,offset,count in specs}


objects=[];sources=[];proofs=[];coffs=[];body=bytearray(192);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[];initFixups=[];compiled={}
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext'/('texture_busy_candidate_20261009.cpp' if address==0x9f9d50 else 'managed_texture_busy_candidate_20261009.cpp')
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
assert len(fixups)==1
(w/'managed_texture_busy_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
(w/'managed_texture_busy_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@ManagedBusy@8':specs[0][1],'@TextureBusy@8':specs[1][1]}.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedTextureBusy_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=2048 snapshots=4096' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_managed_texture_data.h','fable_counted_pointer.h','fable_surface_types.h','fable_resource_list.h','fable_d3d9.h','rebuild_abi.h')]
report=dict(cases=2048,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=2,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original CManagedTextureData nonconstboolIsBusy complete26B andCTexture constboolIsBusy3B, both exactcanonicalAPI corrections forpreviouslandedfacade/mislabel. Two mandatoryactualsources, no functionalcall boundaries: liveBusyFlag read, ifnonnull queryactualoriginal PCfalseleaf onrawTexture.Data thenstorefalse, returnfalse; zero flag remainszero, allother76B fields preserved. Actualdirectleaf rawself pointers no-op; 2048cases/4096full16KiB snapshots, eight rawbusybytes/rawpointerwords/ownerfields/two placements-fills. No additional function/helperdata coverage')
(ROOT/'rebuild/compile-gate/native-managed-texture-busy.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_TEXTURE_BUSY_GATE_PASS')

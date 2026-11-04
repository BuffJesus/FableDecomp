import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-managed-texture-data-chain-'));env=V.env()
specs=[('00/a6/CManagedTextureData_Initialise_00a69d60.cpp','?Initialise@CManagedTextureData@@QAEXKK@Z',0xa69d60,61,0,2),('00/56/CCountedPointer_CTexture_Reset_005669c0.cpp','?Reset@?$CCountedPointer@VCTexture@@@@QAEXPAVCTexture@@@Z',0x5669c0,103,128,3),('00/a6/CManagedTextureData_IsBusy_00a69810.cpp','?IsBusy@CManagedTextureData@@QAE_NXZ',0xa69810,26,256,1),('00/9f/CTexture_IsBusy_009f9d50.cpp','?IsBusy@CTexture@@QBE_NXZ',0x9f9d50,3,320,0)]
targets={'?Reset@?$CCountedPointer@VCTexture@@@@QAEXPAVCTexture@@@Z':0x5669c0,'?IsBusy@CTexture@@QBE_NXZ':0x9f9d50,'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,'?DeleteData@?$CCountedPointer@VCTexture@@@@CIXPAX@Z':0x41bd80,'?Uninitialise@CTexture@@QAEXXZ':0x9f9f70};offsets={0x5669c0:128,0x9f9d50:320,0xbfea1a:0x80000000,0xbfe9bc:0x80000001,0x41bd80:0x80000002}

objects=[];sources=[];proofs=[];body=bytearray(384);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/counted_texture_reset_candidate_20261009.cpp'
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
 objects.append(obj)
assert len(fixups)==6,fixups
(w/'managed_texture_data_chain_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
helper_proof=check(objects[1],'?DeleteData@?$CCountedPointer@VCTexture@@@@CIXPAX@Z',0x41bd80,23,2,targets)
(w/'managed_texture_data_chain_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@ManagedDataInitialise@16':specs[0][1],'@ManagedBusy@8':specs[2][1],'@TextureDeleteData@4':'?DeleteData@?$CCountedPointer@VCTexture@@@@CIXPAX@Z','?Uninitialise@CTexture@@QAEXXZ':'_AbortBoundary'}.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedTextureDataChain_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(4):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=55296 snapshots=' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_managed_texture_data.h','fable_resource_list.h','fable_counted_pointer.h','fable_surface_types.h','fable_d3d9.h','fable_base_class.h','rebuild_abi.h')]
report=dict(cases=55296,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,unclaimed_helper_proof=helper_proof,omission_checks=4,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual61B ManagedTextureDataInitialise and103B CountedTextureReset plus26B ManagedBusy/3B TextureBusy four-source composition, full originalbody parity repeated andallfour source omissionsfail. Only original C++ allocators/delete/old storeddeleter boundaries, allocate8B texture/constructorzero thenrealReset sharedcleanup/allocate12B count1/recordedactualspecializeddeleter/liveData reload thenparent storesrawallocationSize/pool andrealbusyquery clearsoriginalPCbusyflag/returnsfalse. Original23B emitteddeleter supportingproof unchanged, notexecuted/countedas newcoverage,unusedTextureUninitaborts. Complete16KiB event/final snapshots, oldnull/twocontrolblocks/refcounts0-1-2-ffffffff,texture/controlallocationfailures/twoheapblocks each/rawargs/callback owner-info-datamutations/two placements-fills. No new function/helperdata coverage')
(ROOT/'rebuild/compile-gate/native-managed-texture-data-chain.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_TEXTURE_DATA_CHAIN_GATE_PASS')

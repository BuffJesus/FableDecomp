import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-counted-volume-texture-reset-'));env=V.env()
specs=[('00/a6/CCountedPointer_CVolumeTexture_Reset_00a6ae60.cpp','?Reset@?$CCountedPointer@VCVolumeTexture@@@@QAEXPAVCVolumeTexture@@@Z',0xa6ae60,103,0,3)]
targets={'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,'?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@CIXPAX@Z':0xa6acb0};offsets={0xbfea1a:0,0xbfe9bc:1,0xa6acb0:2}

objects=[];sources=[];proofs=[];body=bytearray(256);raw=EXE.read_bytes();cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;fixups=[]
for rel,symbol,address,size,offset,count in specs:
 source=ROOT/'rebuild/src/compiled'/rel
 if not source.exists():source=ROOT/'work/parity_addtext/counted_volume_texture_reset_candidate_20261009.cpp'
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
assert len(fixups)==3,fixups
(w/'counted_volume_texture_reset_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
helper_proof=check(objects[0],'?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@CIXPAX@Z',0xa6acb0,11,0,targets)
(w/'counted_volume_texture_reset_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in {'@CountedVolumeTextureReset@12':specs[0][1],'@VolumeTextureDeleteData@4':'?DeleteData@?$CCountedPointer@VCVolumeTexture@@@@CIXPAX@Z'}.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeCountedVolumeTextureReset_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
for omit in range(1):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=36864 snapshots=' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_managed_texture_data.h','fable_resource_list.h','fable_counted_pointer.h','fable_volume_types.h','fable_d3d9.h','fable_base_class.h','rebuild_abi.h')]
report=dict(cases=36864,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,unclaimed_helper_proof=helper_proof,omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete103B original CCountedPointer<CVolumeTexture>::Reset exactfirstcandidate/36instructions/3relocations, one mandatoryactualsource. Old sharedinfo decremented withwrap, countzero invokes storedfastcalldeleter thenreloadsInfo forC++delete; clearsInfo/storesnewData; nonnull allocates12B controlblock,count1,originalspecializeddeleter pointer/liveData reloadedafterallocator. Only original allocator/operatordelete andold deleter functionalboundaries. Complete16KiB event/final snapshots,oldinfo null/twoblocks/rawrefcounts/newdata null-same-owneraliases/allocationfailure/two newblocks/callback owner-info-data-countermutation/two placements-fills. Emitted11B virtual specializeddeleter code matchesoriginal but notexecuted orcountedas additionalcoverage; virtual scalar deleter dispatch is not executed in this Arena gate. StrongC++allocator/delete fixture ownsArena lifetime, no actualCRTfree onArena, no helperdata extra coverage')
(ROOT/'rebuild/compile-gate/native-counted-volume-texture-reset.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_COUNTED_VOLUME_TEXTURE_RESET_GATE_PASS')


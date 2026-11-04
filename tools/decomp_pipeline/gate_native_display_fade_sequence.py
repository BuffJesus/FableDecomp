import hashlib,json,struct,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-display-fade-sequence-'));env=V.env()
specs=[('CDisplayEngine_SetAboutToRun_00434cd0.cpp','?SetAboutToRun@CDisplayEngine@@QAEXXZ',0x434cd0,141,3),('CFadeInFadeOutBase_Update_00434870.cpp','?Update@CFadeInFadeOutBase@@QAEXM@Z',0x434870,95,2),('CDisplayEngine_EndScreenFadeOut_00434c90.cpp','?EndScreenFadeOut@CDisplayEngine@@QAEXXZ',0x434c90,59,1),('CFadeInFadeOutBase_GetFadeInOutAlpha_004348d0.cpp','?GetFadeInOutAlpha@CFadeInFadeOutBase@@QAEMXZ',0x4348d0,79,5)]
targets={s[1]:s[2] for s in specs};targets.update({'?GInitialFadeAllowedToBeActive@@3_NA':0x1375cdc,'?EndProfile@NProfileTimer@@YIXJ@Z':0x9d8250,'__real@38d1b717':0x129ba3c,'__real@3f800000':0x122ded8,'__real@00000000':0x122dedc})
offsets={s[2]:i*256 for i,s in enumerate(specs)};offsets.update({0x1375cdc:0xffffffff,0x9d8250:0xfffffffe,0x129ba3c:1024,0x122ded8:1028,0x122dedc:1032});body=bytearray(1036);struct.pack_into('<fff',body,1024,0.0001,1,0);fixups=[];cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;raw=EXE.read_bytes();objects=[];proofs=[];sources=[]
for i,(file,symbol,address,length,relcount) in enumerate(specs):
 source=ROOT/'rebuild/src/compiled/00/43'/file;sources.append(source);obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);proofs.append(check(obj,symbol,address,length,relcount,targets));b=read_virtual_data(raw,address,length);body[i*256:i*256+length]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16);assert target in offsets;fixups.append((i*256+ins.address-address+ins.size-4,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((i*256+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
# EndScreenFadeOut's A2 uses a moffs immediate rather than a ModRM displacement.
assert (512+9,0xffffffff,6) in fixups
(w/'display_fade_sequence_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
settings=ROOT/'rebuild/src/compiled/01/37/DisplayEngine_FadeSettings_01375cdc.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDisplayFadeSequence_test.cpp';sources.append(settings)
for i,path in enumerate([settings,fixture]):
 obj=w/f'extra{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(5):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=768 snapshots=24576' in r.stdout
report=dict(cases=768,snapshots=24576,providers=proofs,omission_checks=5,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Four actual fade methods plus actual settings global;only profiling is callback boundary.Six step sizes including negative/zero,16frame sequences,two setup attempts,four endfade timings,alllock/global states,profile lock mutation,two positions/fills.Full8KiB+global model snapshots and alpha bits everyframe.')
(ROOT/'rebuild/compile-gate/native-display-fade-sequence.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_DISPLAY_FADE_SEQUENCE_GATE_PASS')

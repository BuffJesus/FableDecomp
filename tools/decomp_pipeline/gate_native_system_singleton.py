"""Genuine singleton BSS, compiler-generated CRT lifetime, and original native lifecycle."""
import hashlib,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_system_lifetime import TARGETS
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-system-singleton-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('01/3c/GSystemManager_013ca618.cpp','00/9a/CSystemManager_Lifetime_009a7440.cpp','00/9a/Global_GFGetSystemManager_009a4ec0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeSystemSingleton_test.cpp'
glob='?GSystemManager@@3VCSystemManager@@A';count='?NoInstances@CSystemManager@@2EA';ctor='??0CSystemManager@@QAE@XZ';dtor='??1CSystemManager@@UAE@XZ';getter='?GFGetSystemManager@@YIPAVCSystemManager@@XZ'
aliases={'_SystemVTable':'??_7CSystemManager@@6B@','@BaseCtor@4':'??0CInitBaseClass@@QAE@XZ','@BaseDtor@4':'??1CInitBaseClass@@UAE@XZ','@WideCtor@4':'??0CWideString@@QAE@XZ','@CharCtor@4':'??0CCharString@@QAE@XZ','@WideDtor@4':'??1CWideString@@QAE@XZ','@CharDtor@4':'??1CCharString@@QAE@XZ'}
(w/'system_singleton_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
targets=dict(TARGETS);targets.update({glob:0x13ca618,ctor:0x9a7440,dtor:0x9a5800,'_$E2':0x122a680,'_atexit':0x4012bc})
raw=EXE.read_bytes();code=bytearray(608);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,n,offset in ((0x1223ff0,22,0),(0x122a680,10,32),(0x9a7440,118,64),(0x9a5800,416,192)):
 body=read_virtual_data(raw,address,n);code[offset:offset+n]=body
 for i in md.disasm(body,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2:
   target=i.operands[0].imm
   if not address<=target<address+n:assert target in targets.values();fixups.append((offset+i.address-address+1,target,1))
  elif i.mnemonic in ('mov','push'):
   for op in i.operands:
    if op.type==2 and op.imm in (0x13ca618,0x122a680,0x129affc):fixups.append((offset+i.address-address+i.imm_offset,op.imm,0))
assert len(fixups)==24
(w/'system_singleton_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
c=Coff(objects[0]);assert coff_data(c,glob,0x180)==read_virtual_data(raw,0x13ca618,0x180)==bytes(0x180);assert coff_data(c,count,1)==read_virtual_data(raw,0x13ca608,1)==b'\0'
sec,off=c.symbols['.CRT$XCU'];body,rel=c.sections[sec];assert off==0 and body==bytes(4) and rel=={0:('_$E1',6)}
proofs=[check(objects[i],s,a,n,num,targets) for i,s,a,n,num in ((0,'_$E1',0x1223ff0,22,4),(0,'_$E2',0x122a680,10,2),(1,ctor,0x9a7440,118,5),(1,dtor,0x9a5800,416,13),(2,getter,0x9a4ec0,6,1))]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=snapshots=0
for scenario in range(5):
 for mutation in range(2):
  r=subprocess.run([str(exe),str(scenario),str(mutation)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'NATIVE_SYSTEM_SINGLETON_PASS' in r.stdout;print(r.stdout.strip(),flush=True);cases+=1;snapshots+=int(r.stdout.strip().split('snapshots=')[1])
headers=[ROOT/'rebuild/include'/p for p in ('fable_system_manager_state.h','fable_gfuninitialise.h','fable_counted_pointer.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,data=[{'address':'013ca618','symbol':glob,'bytes':384},{'address':'013ca608','symbol':count,'bytes':1}],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Compiler-generated CRT initializer/destructor wrappers exactly match original22/10byte bodies;CRT XCU registration audited. Real typed singleton and NoInstances BSS match original zero images. Production constructor runs before main and registers actual teardown;original wrappers and original118/416byte constructor/destructor execute natively with24 audited fixups. Full384byte object snapshots at base/string/ownership/atexit boundaries plus all controlblocks and final state;five ownership patterns,two mutation modes. Getter returns real singleton. Base/string methods,atexit and DeleteFunc/operator delete remain test boundaries;registered native callback identity asserted separately,no object-byte normalization.')
(ROOT/'rebuild/compile-gate/native-system-singleton.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SYSTEM_SINGLETON_GATE_PASS',snapshots,'snapshots')

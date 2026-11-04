"""Compare scenery-type registration with the original implementation."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-detail-register-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b5/CEngineLocalDetailGenerator_RegisterObjectCollectionType_00b59c70.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDetailRegister_test.cpp'
symbol='?RegisterObjectCollectionType@CEngineLocalDetailGenerator@@QAEPBVCLocalDetailObjectCollectionType@@ABV2@@Z'
o=w/'source.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(o)
targets={'?IsEquivalent@CLocalDetailObjectCollectionType@@QBE_NABV1@@Z':0xbe2680,'??0CLocalDetailObjectCollectionType@@QAE@ABV0@@Z':0xbe26c0,'??2@YAPAXI@Z':0xbfea1a}
grow=next(s for s in c.symbols if s.startswith('?_M_insert_overflow@'));targets[grow]=0xb59f60
boundary={0xbe2680:'@Equivalent@12',0xbe26c0:'@Copy@12'}
aliases={'@Register@12':symbol};aliases.update({s:boundary[a] for s,a in targets.items() if a in boundary})
(w/'detail_register_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb59c70,159);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body,0xb59c70):
 if i.mnemonic=='call' and i.operands[0].type==2:
  a=i.operands[0].imm;assert a in targets.values();fixups.append((i.address-0xb59c70+1,a,0))
assert len(fixups)==4
(w/'detail_register_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%(f[0],f[1]) for f in fixups)+'};\n')
proof=check(o,symbol,0xb59c70,159,4,targets)
f=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(f),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(f)],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+[str(f),str(o)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,match,fail,old,spare in itertools.product((0,0x95),range(7),(-1,0,2),(0,1),(0,1,3),(0,2)):
  a=bytearray([fill])*4096
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0xc,base+0x200);put(0x10,base+0x200+old*4);put(0x14,base+0x200+(old+spare)*4)
  for start in (0x200,0x400):
   for j in range(8):put(start+j*4,base+0x600+j*0x20)
  f.write(struct.pack('<3I',mode,match&0xffffffff,fail));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original159byte registration body/four audited fixups. Full4KiB snapshots, two sequential registrations, empty/existing list, equivalent/missing matches, spare capacity/growth, allocation success/failure and callbacks replacing/shrinking vector or mutating candidate/source. Equivalence, copy constructor, allocation and vector growth remain boundaries.')
(ROOT/'rebuild/compile-gate/native-detail-register.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DETAIL_REGISTER_GATE_PASS',snapshots,'snapshots')

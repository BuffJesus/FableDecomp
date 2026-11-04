"""Compare scenery-type registration with the original implementation."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-detail-register-types-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b5/CEngineLocalDetailGenerator_RegisterObjectCollectionType_00b59c70.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDetailRegisterTypes_test.cpp'
copy_source=ROOT/'rebuild/src/compiled/00/be/CLocalDetailObjectCollectionType_CopyCompare_00be2680.cpp'
co=w/'copy.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(co),str(copy_source)],env,w);assert not r.returncode,r.stdout+r.stderr
symbol='?RegisterObjectCollectionType@CEngineLocalDetailGenerator@@QAEPBVCLocalDetailObjectCollectionType@@ABV2@@Z'
o=w/'source.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(o)
targets={'?IsEquivalent@CLocalDetailObjectCollectionType@@QBE_NABV1@@Z':0xbe2680,'??0CLocalDetailObjectCollectionType@@QAE@ABV0@@Z':0xbe26c0,'??2@YAPAXI@Z':0xbfea1a}
grow=next(s for s in c.symbols if s.startswith('?_M_insert_overflow@'));targets[grow]=0xb59f60
targets.update({'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CLocalDetailObjectCollectionType@@6B@':0x12a7d94})
aliases={'@Register@12':symbol,'_TypeTable':'??_7CLocalDetailObjectCollectionType@@6B@','??0CBaseClassNonCopyable@@QAE@XZ':'@BaseCtor@4','??1CBaseClassNonCopyable@@UAE@XZ':'@BaseDtor@4'}
(w/'detail_register_types_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(446);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for a,n,offset in [(0xb59c70,159,0),(0xbe26c0,83,256),(0xbe2680,62,384)]:
 span=read_virtual_data(raw,a,n);body[offset:offset+n]=span
 for i in md.disasm(span,a):
  if i.mnemonic=='call' and i.operands[0].type==2:
   target=i.operands[0].imm;assert target in targets.values();fixups.append((offset+i.address-a+1,target,0))
assert struct.unpack_from('<I',body,272)[0]==0x12a7d94;fixups.append((272,0x12a7d94,1));assert len(fixups)==6
(w/'detail_register_types_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
proof=check(o,symbol,0xb59c70,159,4,targets)
copy_proof=[check(co,s,a,n,r,targets) for s,a,n,r in [('?IsEquivalent@CLocalDetailObjectCollectionType@@QBE_NABV1@@Z',0xbe2680,62,0),('??0CLocalDetailObjectCollectionType@@QAE@ABV0@@Z',0xbe26c0,83,2)]]
c=Coff(co);sec,off=c.symbols['??_7CLocalDetailObjectCollectionType@@6B@'];b,rel=c.sections[sec];assert off==0 and len(b)==4 and rel=={0:('??_ECLocalDetailObjectCollectionType@@UAEPAXI@Z',6)}
f=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(f),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for keep in (o,co):
 r=subprocess.run(cmd+[str(f),str(keep)],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+[str(f),str(o),str(co)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,match,fail,old,spare in itertools.product((0,0x95),range(7),(-1,0,2),(0,1),(0,1,3),(0,2)):
  a=bytearray([fill])*4096
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0xc,base+0x200);put(0x10,base+0x200+old*4);put(0x14,base+0x200+(old+spare)*4)
  for start in (0x200,0x400):
   for j in range(8):put(start+j*4,base+0x600+j*0x60)
  put(0x814,999)
  for j in range(3):put(0x614+j*0x60,999 if j==match else 100+j)
  f.write(struct.pack('<3I',mode,match&0xffffffff,fail));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,provider=proof,copy_providers=copy_proof,copy_sha256=hashlib.sha256(copy_source.read_bytes()).hexdigest(),source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Three original spans/six audited fixups, actual registration/copy/comparison and emitted vtable. Full4KiB snapshots, two sequential registrations, empty/existing list, equivalent/missing matches, spare capacity/growth, allocation success/failure and callbacks replacing/shrinking vector or mutating candidate/source. Only base constructor, allocation and vector growth remain boundaries. Allocation failure ends repeated sequence to avoid later dereference of an inserted null entry.')
(ROOT/'rebuild/compile-gate/native-detail-register-types.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DETAIL_REGISTER_TYPES_GATE_PASS',snapshots,'snapshots')

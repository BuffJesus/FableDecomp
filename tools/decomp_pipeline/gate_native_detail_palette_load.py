"""Compare actual scenery-type palette loading with the original implementation."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-detail-palette-load-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/bd/CLocalDetailCacheMap_PaletteLoad_00bdedd0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDetailPaletteLoad_test.cpp'
symbol='?Load@CObjectTypeCollectionPalette@CLocalDetailCacheMap@@QAEXAAVCDataInputStream@@@Z'
o=w/'source.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(o)
targets={'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,'_memmove':0xbfeae6,'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??_7CLocalDetailObjectCollectionType@@6B@':0x12a7d94,'?Load@CLocalDetailObjectCollectionType@@QAEXAAVCDataInputStream@@_N@Z':0xbe27b0,'?GPLocalDetailGenerator@NEngineInternal@@3PAVCEngineLocalDetailGenerator@@A':0x1436e88,'?RegisterObjectCollectionType@CEngineLocalDetailGenerator@@QAEPBVCLocalDetailObjectCollectionType@@ABV2@@Z':0xb59c70}
grow=next(s for s in c.symbols if s.startswith('?_M_insert_overflow@'));targets[grow]=0xbdfc00
boundary={0x993ca0:'@Overflow@16',0x99a2f0:'@BaseCtor@4',0x99a300:'@BaseDtor@4',0xbe27b0:'@TypeLoad@16',0xb59c70:'@Register@12'}
aliases={'@Load@12':symbol,'_TypeTable':'??_7CLocalDetailObjectCollectionType@@6B@'};aliases.update({s:boundary[a] for s,a in targets.items() if a in boundary})
(w/'detail_palette_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xbdedd0,356);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body,0xbdedd0):
 if i.mnemonic=='call' and i.operands[0].type==2:
  a=i.operands[0].imm;assert a in targets.values();fixups.append((i.address-0xbdedd0+1,a,0))
for off,a in [(248,0x12a7d94),(271,0x1436e88)]:assert struct.unpack_from('<I',body,off)[0]==a;fixups.append((off,a,1))
assert len(fixups)==10
(w/'detail_palette_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
proof=check(o,symbol,0xbdedd0,356,10,targets)
# Audit the emitted one-slot virtual table and its real scalar destructor target.
sec,off=c.symbols['??_7CLocalDetailObjectCollectionType@@6B@'];table,rel=c.sections[sec]
assert off==0 and len(table)==4 and rel=={0:('??_ECLocalDetailObjectCollectionType@@UAEPAXI@Z',6)}
scalar=struct.unpack('<I',read_virtual_data(raw,0x12a7d94,4))[0]
scalar_proof=check(o,'??_GCLocalDetailObjectCollectionType@@UAEPAXI@Z',scalar,30,2,dict(targets,**{'??3@YAXPAX@Z':0xbfe9bc}))
f=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(f),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(f)],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+[str(f),str(o)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,retail,count,remaining,position,old,capacity in itertools.product((0,0x95),range(7),(0,1,0x80,0xff),(-1,0,1,3),(0,1,4,5),(0,0xffffffff),(0,2),(2,4)):
  a=bytearray([fill])*4096
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0,base+0x500);put(4,base+0x500+old*4);put(8,base+0x500+capacity*4)
  put(0x204,position);put(0x208,128);put(0x20c,base+0x300);put(0x214,remaining);a[0x300]=retail;put(0x301,count)
  f.write(struct.pack('<I',mode));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,scalar=scalar_proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original356byte loader/ten audited fixups; actual one-slot vtable checked and native vptr redirected to compiled table. Full4KiB snapshots; flags, negative/zero/positive counts, existing entries, spare capacity/growth, stream split/wrap, callback generator/vector changes and null registration. Object Load/Register, base lifecycle, vector growth and stream overflow remain boundaries. Base ctor boundary initializes otherwise uninitialized object fields before Load boundary inspects them. Real scalar destructor audited but not executed here.')
(ROOT/'rebuild/compile-gate/native-detail-palette-load.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DETAIL_PALETTE_LOAD_GATE_PASS',snapshots,'snapshots')

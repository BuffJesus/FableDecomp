"""Native world-map opening, ownership and callback traversal comparisons."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-texture-palette-load-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/bf/CEngineTexturePalette_Load_00bf9290.cpp';fixture=ROOT/'rebuild/tests/integration/NativeTexturePaletteLoad_test.cpp'
symbol='?Load@CEngineTexturePalette@@QAEXAAVCDataInputStream@@@Z'
targets={'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,'?resize@?$vector@UCPaletteEntry@CEngineTexturePalette@@V?$allocator@UCPaletteEntry@CEngineTexturePalette@@@_STL@@@_STL@@QAEXI@Z':0xbf97a0,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,'??1CCharString@@QAE@XZ':0x99eae0,'?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z':0x9ccdc0}
boundary={0x993ca0:'@Overflow@16',0xbf97a0:'@Resize@12',0x99efb0:'@Assign@12',0x99eae0:'@Destroy@4',0x9ccdc0:'@Find@12'}
aliases={'@Load@12':symbol};aliases.update({s:boundary[a] for s,a in targets.items()})
(w/'palette_load_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xbf9290,362);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body,0xbf9290):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm in boundary;fixups.append((i.address-0xbf9290+1,i.operands[0].imm))
assert len(fixups)==9
(w/'palette_load_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xbf9290,362,9,targets)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,retail,count,remaining,position,found in itertools.product((0,0x95),range(7),(0,1,0x80,0xff),(1,2,4),(0,1,4,5),(0,0xffffffff),(0,1,0x80000000,0xffffffff)):
  a=bytearray([fill])*2048
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0,base+0x400);put(4,base+0x500);put(8,base+0x508);put(12,base+0x520);put(0x10,0x778899aa);a[0x14]=0x95
  put(0x204,position);put(0x208,128);put(0x20c,base+0x300);put(0x214,remaining);a[0x300]=retail;put(0x301,count)
  f.write(struct.pack('<2I',mode,found));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%2060==0;snapshots=size//2060
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original362byte body/nine audited fixups. Full2KiB snapshots;retail flag bytes0/1/80/ff,one/two/four entries,split/full/wrapping stream reads,found/missing/high-bit indices,callbacks swap vector or bank/default state. Stream strings/overflow,vector resize,string assignment/destruction and bank lookup are boundaries.')
(ROOT/'rebuild/compile-gate/native-texture-palette-load.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_TEXTURE_PALETTE_LOAD_GATE_PASS',snapshots,'snapshots')

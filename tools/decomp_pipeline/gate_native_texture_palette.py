"""Texture palette construction and real non-POD vector growth against retail."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-texture-palette-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/bf/CEngineTexturePalette_Constructor_00bf90a0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeTexturePalette_test.cpp'
ctor='??0CEngineTexturePalette@@QAE@PBVCGraphicDataBank@@_NK@Z';grow='?_M_insert_overflow@?$vector@UCPaletteEntry@CEngineTexturePalette@@V?$allocator@UCPaletteEntry@CEngineTexturePalette@@@_STL@@@_STL@@QAEXPAUCPaletteEntry@CEngineTexturePalette@@ABU34@ABU__false_type@2@I_N@Z'
aliases={'@Construct@20':ctor,'@Grow@28':grow,'@DefaultText@4':'??0CCharString@@QAE@XZ','@CopyText@12':'??0CCharString@@QAE@ABV0@@Z','@DestroyText@4':'??1CCharString@@QAE@XZ'}
(w/'texture_palette_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
spans=[(0xbf90a0,171,0),(0xbf94d0,249,192),(0xbf8f30,54,448),(0xbf8f70,50,512)]
raw=EXE.read_bytes();code=bytearray(562);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;allowed={a for a,_,_ in spans}|{0x99e4b0,0x99ec30,0x99eae0,0xbfea0e,0xbfea14}
for a,n,offset in spans:
 body=read_virtual_data(raw,a,n);code[offset:offset+n]=body
 for i in md.disasm(body,a):
  if i.mnemonic=='call':
   assert i.operands[0].type==2 and i.operands[0].imm in allowed;fixups.append((offset+i.address-a+1,i.operands[0].imm))
assert len(fixups)==14
(w/'texture_palette_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate((source,fixture,source)):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o)]+(['/Dmalloc=PaletteAllocate','/Dfree=PaletteFree'] if i==2 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
c=Coff(objects[0]);copy=next(s for s in c.symbols if s.startswith('??$__uninitialized_copy'));fill=next(s for s in c.symbols if s.startswith('??$__uninitialized_fill_n'))
targets={'??0CCharString@@QAE@XZ':0x99e4b0,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??1CCharString@@QAE@XZ':0x99eae0,'_malloc':0xbfea0e,'_free':0xbfea14,grow:0xbf94d0,copy:0xbf8f30,fill:0xbf8f70}
proofs=[check(objects[0],s,a,n,num,targets) for s,a,n,num in [(ctor,0xbf90a0,171,5),(grow,0xbf94d0,249,7),(copy,0xbf8f30,54,1),(fill,0xbf8f70,50,1)]]
d=Coff(objects[2])
for symbol in (ctor,grow,copy,fill):
 sec,off=c.symbols[symbol];sec1,off1=d.symbols[symbol];body,rel=c.sections[sec];body1,rel1=d.sections[sec1];assert off==off1 and body==body1 and {p:({'_free':'_PaletteFree','_malloc':'_PaletteAllocate'}.get(t,t),k) for p,(t,k) in rel.items()}==rel1
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects[1:])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for poison,mode,bank,retail,default in itertools.product((0,0x95),range(6),(0,base+0xc00),(0,1,0x80,0xff),(0,1,0xffffffff)):
  f.write(struct.pack('<6I',mode,0,bank,retail,default,0));f.write(bytes([poison])*4096);cases+=1
 for poison,mode,n,count,atend in itertools.product((0,0x95),range(6),(0,1,3),(0,1,3),(0,1)):
  for pos in range(n+1):
   if atend and pos!=n:continue
   a=bytearray([poison])*4096
   struct.pack_into('<3I',a,4,base+0x200 if n else 0,base+0x200+n*8 if n else 0,base+0x218 if n else 0)
   for j in range(n):struct.pack_into('<2I',a,0x200+j*8,0x100+j,0x1000+j)
   struct.pack_into('<2I',a,0x500,0x2222,0x3333)
   f.write(struct.pack('<6I',mode,1,0x200+pos*8 if n else 0,count,atend,0));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'palette mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineTexturePalette.h','fable_stlport_vector_impl.h','fable_stlport_uninitialized.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original171/249/54/50byte constructor,growth,copy,fill spans with14audited fixups. Actual template helpers compose. Full4KiB snapshots;constructor retail bytes0/1/80/ff,banknull/live,default0/1/ffffffff,zero/poison fills. Reentrant default-string constructor supplies spare/full vector storage. Standalone grow with0/1/3 old elements,all insertion positions,fill0/1/3,append/nonappend;copy/destruction/allocation callbacks mutate state. String lifecycle and malloc/free remain boundaries;renamed allocation symbols audited against unchanged bodies/relocations for all four providers.')
(ROOT/'rebuild/compile-gate/native-texture-palette.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_TEXTURE_PALETTE_GATE_PASS',snapshots,'snapshots')

"""Map constructor against original native execution."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-add-map-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineWorldMap_AddMap_00b42d10.cpp','00/b4/CEngineMap_Constructor_00b429e0.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeAddMap_test.cpp'
add='?AddMap@CEngineWorldMap@@QAEXJJ@Z';ctor='??0CEngineMap@@QAE@PBVCIDrawMap@@JJJJJVCCharString@@_N@Z'
targets={'?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A':0x1436ea8,'?GPLocalDetailGenerator@NEngineInternal@@3PAVCEngineLocalDetailGenerator@@A':0x1436e88,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??1CCharString@@QAE@XZ':0x99eae0,'?OnMapVisibilityChanged@CEngineLandscapeMap@@QAEX_N@Z':0xbdc3c0,'?OnMapVisibilityChanged@CLocalDetailCacheMap@@QAEX_N@Z':0xbde160,'??0CEngineMap@@QAE@PBVCIDrawMap@@JJJJJVCCharString@@_N@Z':0xb429e0,'_malloc':0xbfea0e,'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfeb1c,'?CreateMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z':0xb59800,'?CreateMap@CEngineLandscapeRenderer@@QAEXPAVCEngineMap@@J@Z':0xb67140,'?DeleteMap@CEngineWorldMap@@QAEXJ@Z':0xb42b10}
targets.update({'??0CCharString@@QAE@XZ':0x99e4b0,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,'?resize@?$vector@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@_STL@@@_STL@@QAEXIABQAVCEngineMap@@@Z':0xb44170})
aliases={'@Add@16':add,'@DefaultText@4':'??0CCharString@@QAE@XZ','@CopyText@12':'??0CCharString@@QAE@ABV0@@Z','@AssignText@12':'??4CCharString@@QAEAAV0@ABV0@@Z','@DestroyText@4':'??1CCharString@@QAE@XZ','@DeleteOne@12':'?DeleteMap@CEngineWorldMap@@QAEXJ@Z','@LandscapeCreate@16':'?CreateMap@CEngineLandscapeRenderer@@QAEXPAVCEngineMap@@J@Z','@DetailCreate@12':'?CreateMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z','?OnMapVisibilityChanged@CEngineLandscapeMap@@QAEX_N@Z':'@LandscapeVisible@12','?OnMapVisibilityChanged@CLocalDetailCacheMap@@QAEX_N@Z':'@DetailVisible@12'}
(w/'add_map_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
code=bytearray(684);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;raw=EXE.read_bytes()
for address,size,offset in [(0xb42d10,349,0),(0xb429e0,172,512)]:
 body=read_virtual_data(raw,address,size);code[offset:offset+size]=body
 for i in md.disasm(body,address):
  if i.mnemonic=='call' and i.operands[0].type==2:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+1,i.operands[0].imm,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp in (0x1436ea8,0x1436e88):fixups.append((offset+i.address-address+i.disp_offset,op.mem.disp,0))
assert len(fixups)==18
(w/'add_map_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]+sources[:2]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o)]+(['/Dmalloc=MapAllocate'] if i>=4 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],add,0xb42d10,349,14,targets),check(objects[1],ctor,0xb429e0,172,4,targets)];assert all(p['grade']=='MATCH' for p in proofs)
for idx,symbol in enumerate((add,ctor)):
 c,d=Coff(objects[idx]),Coff(objects[idx+4]);sec,off=c.symbols[symbol];sec1,off1=d.symbols[symbol];body,rel=c.sections[sec];body1,rel1=d.sections[sec1];assert off==off1 and body==body1 and {p:('_MapAllocate' if t=='_malloc' else t,k) for p,(t,k) in rel.items()}==rel1
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib'];linked=[objects[i] for i in (3,4,5,2)]
for omit in (1,2,3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(linked) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,linked)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,id,existing,flags,size,children in itertools.product((0,0x95),range(8),(0,3),(0,1,2),(0,1,2,0xffffffff),(0,4),range(4)):
  a=bytearray([fill])*8192
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0xc,base+0x500 if fill else 0);put(0x20,base+0x700);put(0x24,base+0x700+size*4);put(0x28,base+0x710);put(0x2c,base+0x900);put(0x19c,base+0x620);put(0x620,base+0x600)
  for j in range(4):put(0x700+j*4,base+0xb00 if j==id and existing and size else 0)
  for h in (0x900,0x920):put(h,base+h);put(h+4,base+h)
  for m in (0xb00,0x1100):put(m,0);put(m+4,0);put(m+0x18,0x1234 if existing==1 or m==0x1100 else 0x4321);put(m+0x58,base+0x900);a[m+0x64]=0
  a[0x1800:0x1820]=bytes(32);a[0x1800]=1;put(0x1804,base+0x1c00);put(0x1808,0x99887766);put(0x180c,65535);put(0x1810,0xffffffff);put(0x1814,0x1234);put(0x1818,33);put(0x181c,65536)
  f.write(struct.pack('<4I',mode,id,flags,children));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'constructor native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%8212==0;snapshots=size//8212
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineMap.h','engine/CEngineStaticMapFileBlockManager.h','fable_2d_vector.h','fable_stlport_tree.h','fable_engine_map_list.h','engine/CEngineWorldMap.h','engine/CEngineLandscapeRenderer.h','engine/CEngineLocalDetailGenerator.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original349/172bytes with18audited fixups,actual map constructor and renderer globals. Full8KiB snapshots plus callback arguments/singletons. Two AddMap calls;id0/3,empty/full vector,missing/matching/mismatching UID,flags0/1/2/ffffffff,child masks. Callbacks replace vector storage/map slot/list sentinel or clear detail child;null new boundary is repaired by renderer callback to exercise constructor skip. Malloc-renamed objects body/relocations audited. DrawMapInfo,vector resize,DeleteMap,strings,allocation,renderer creation and visibility remain boundaries.')
(ROOT/'rebuild/compile-gate/native-add-map.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ADD_MAP_GATE_PASS',snapshots,'snapshots')

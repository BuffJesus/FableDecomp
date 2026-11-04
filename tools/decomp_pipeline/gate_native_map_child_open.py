"""Canonical landscape/detail opening, ownership and stream ABI versus retail."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-map-child-open-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/bd'/p for p in ('CEngineLandscapeMap_OpenStaticMap_00bdd0e0.cpp','CLocalDetailCacheMap_OpenStaticMap_00bdf010.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeMapChildOpen_test.cpp'
land='?OpenStaticMap@CEngineLandscapeMap@@QAEXPAVCEngineStaticMapFileBlockManager@@AAVCDataInputStream@@@Z';detail='?OpenStaticMap@CLocalDetailCacheMap@@QAEXPAVCEngineStaticMapFileBlockManager@@AAVCDataInputStream@@@Z'
targets={'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,'??3@YAXPAX@Z':0xbfe9bc,'_malloc':0xbfea0e,'_free':0xbfea14,'?Load@CEngineTexturePalette@@QAEXAAVCDataInputStream@@@Z':0xbf9290,'?LoadHeader@CQuadTreeElement@CLocalDetailCacheMap@@QAEXAAVCDataInputStream@@@Z':0xbde290,'?Load@CObjectTypeCollectionPalette@CLocalDetailCacheMap@@QAEXAAVCDataInputStream@@@Z':0xbdedd0,'___security_cookie':0x139c8a8,'@__security_check_cookie@4':0xbfe9f9}
boundary={0x993ca0:'@Overflow@16',0xbf9290:'@Palette@12',0xbde290:'@Quad@12',0xbdedd0:'@Types@12'}
aliases={'@Landscape@16':land,'@Detail@16':detail,'_Delete':'??3@YAXPAX@Z'};aliases.update({s:boundary[a] for s,a in targets.items() if a in boundary})
(w/'map_child_open_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(506);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for address,length,offset in [(0xbdd0e0,292,0),(0xbdf010,186,320)]:
 span=read_virtual_data(raw,address,length);body[offset:offset+length]=span
 for i in md.disasm(span,address):
  if i.mnemonic=='call' and i.operands[0].type==2:
   a=i.operands[0].imm;assert a in targets.values();fixups.append((offset+i.address-address+1,a,1))
  else:
   for op in i.operands:
    if op.type==3 and op.mem.disp==0x139c8a8:fixups.append((offset+i.address-address+i.disp_offset,op.mem.disp,0))
assert len(fixups)==11
(w/'map_child_open_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture,sources[0]]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/GS','/MT','/I'+str(w),'/Fo'+str(o)]+(['/Dmalloc=Allocate','/Dfree=Free'] if n==3 else [])+[str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=[check(objects[0],land,0xbdd0e0,292,6,targets),check(objects[1],detail,0xbdf010,186,5,targets)]
c=Coff(objects[0]);d=Coff(objects[3]);sec,off=c.symbols[land];sec1,off1=d.symbols[land];b,rel=c.sections[sec];b1,rel1=d.sections[sec1];assert off==off1 and b==b1 and {p:({'_malloc':'_Allocate','_free':'_Free'}.get(t,t),k) for p,(t,k) in rel.items()}==rel1
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib'];linked=[objects[2],objects[1],objects[3]]
for omit in (1,2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(linked) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,linked)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,op,pos,remaining,size,owner,count in itertools.product((0,0x95),range(6),range(3),(0,0x7fffffff,0xfffffffc),(-1,0,12,20),(0,1,7,16),(0,1,2),(1,2)):
  a=bytearray([fill])*4096
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0x18,base+0x700);put(0x1c,0 if owner==0 else base+0x600 if owner==1 else base+0x620);put(0x200,base+0x720);put(0x204,base+0x620)
  put(0x304,pos);put(0x308,1024);put(0x30c,base+0x500);put(0x314,remaining)
  for j,v in enumerate((17,size,33,44,55)):put(0x24+j*4,v);put(0x500+j*4,v)
  put(0x600,count);put(0x620,count);put(0x640,2)
  f.write(struct.pack('<2I',mode,op));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proof,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Two original spans/11audited fixups. Native and compiled detail both use actual CRT security-cookie check. Full4KiB snapshots through individual and sequential child opening. Complete/split/invalid/wrapped stream positions,null/equal/distinct ownership,callback replacement,zero/unaligned scratch sizes with verified zeroing. Overflow,virtual seek,palette/quad header loading,allocation and deletion remain boundaries;renamed allocation body/relocations audited.')
(ROOT/'rebuild/compile-gate/native-map-child-open.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MAP_CHILD_OPEN_GATE_PASS',snapshots,'snapshots')

"""Native world-map opening, ownership and callback traversal comparisons."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-world-map-open-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b4/CEngineWorldMap_OpenStaticMaps_00b42750.cpp';fixture=ROOT/'rebuild/tests/integration/NativeWorldMapOpen_test.cpp'
symbol='?OpenStaticMaps@CEngineWorldMap@@AAEXW4STATIC_MAP_OPEN_STATE@CEngineMap@@@Z'
targets={'?OpenStaticMapForUse@CEngineWaterRenderer@@QAEXPAVCStaticMapBankFile@@VCCharString@@@Z':0xb6d4d0,'?CommonStaticMapHeaderEntryName@@3VCCharString@@A':0x1436ecc,'??1CCharString@@QAE@XZ':0x99eae0,'?OpenDevelopmentStaticMap@CEngineMap@@QAEXAAVCStaticMapBankFile@@W4STATIC_MAP_OPEN_STATE@1@@Z':0xb42530,'?OpenStaticMapsUsingCommonRetailHeader@CEngineWorldMap@@AAEXJ@Z':0xb420f0,'?SeaStaticMapBankFileName@@3VCCharString@@A':0x1436ec4,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??_C@_0CG@DFDLEMHP@SetStaticMapFileForUse?3?5OpenStat@':0x12a1f84,'?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z':0x9e9f40,'?GPWaterRenderer@NEngineInternal@@3PAVCEngineWaterRenderer@@A':0x1436e54,'?SetLoadingMemoryPool@CBankFileAsync@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z':0xb3e820,'?AutoBuildWorldMapGrid@CEngineWorldMap@@AAEXXZ':0xb40070,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A':0x1436e98,'?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z':0x9ccdc0}
boundary={0xb40070:'@Grid@4',0x99ebf0:'@Text@16',0x99eae0:'@Destroy@4',0x99ec30:'@Copy@12',0x9e9f40:'@Progress@16',0xb42530:'@MapOpen@16',0xb3e820:'@Pool@16',0xb6d4d0:'@WaterOpen@16',0x9ccdc0:'@Find@12',0xb420f0:'@Common@12',0x1436e98:'_Resources',0x1436e54:'_Water',0x1436ec4:'_SeaName',0x1436ecc:'_CommonName'}
aliases={'@OpenMaps@12':symbol};aliases.update({s:boundary[a] for s,a in targets.items() if a in boundary})
(w/'world_open_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb42750,385);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for i in md.disasm(body,0xb42750):
 if i.mnemonic=='call' and i.operands[0].type==2:
  a=i.operands[0].imm;assert a in boundary;fixups.append((i.address-0xb42750+1,a,1))
 else:
  for op in i.operands:
   if op.type==2 and op.imm in targets.values():fixups.append((i.address-0xb42750+i.imm_offset,op.imm,0))
   elif op.type==3 and op.mem.disp in targets.values():fixups.append((i.address-0xb42750+i.disp_offset,op.mem.disp,0))
assert len(fixups)==20
(w/'world_open_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb42750,385,20,targets)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,state,old,opened,index,mask,info in itertools.product((0,0x95),range(8),(0,1,2,0xffffffff),(0,1),(0,1),(0,1,0xffffffff),(0,5,7),(0,1)):
  a=bytearray([fill])*4096
  def put(off,v):struct.pack_into('<I',a,off,v&0xffffffff)
  put(0x20,base+0x400);put(0x24,base+0x40c);put(0x28,base+0x40c);put(0x1a8,old)
  for j in range(3):put(0x400+j*4,base+0x600+j*0x80 if mask&(1<<j) else 0);put(0x500+j*4,base+0x900+j*0x80)
  put(0xbe0,base+0xc00);put(0xbe4,base+0xd00 if info else 0);put(0xd00,1);put(0xd10,0xffffffff)
  f.write(struct.pack('<4I',mode,state,opened,index));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original385byte body/20audited fixups. Full4KiB snapshots,all states,failed/successful bank open,signed header IDs,sparse maps,ownership increment and callback list replacement/shrink. Grid,bank virtual open,pool transfer,strings,progress,water,map open and common-header reader are explicit boundaries.')
(ROOT/'rebuild/compile-gate/native-world-map-open.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WORLD_MAP_OPEN_GATE_PASS',snapshots,'snapshots')

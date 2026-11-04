"""World water loading: full-state native comparison at explicit bank/stream boundaries."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-world-water-load-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b4/CEngineWorldMap_LoadWaterData_00b41fa0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeWorldWaterLoad_test.cpp'
symbol='?LoadWaterData@CEngineWorldMap@@AAEXXZ'
targets={'?WaterStaticMapBankFileName@@3VCCharString@@A': 21196488, '?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z': 10276288, '??0?$vector@DV?$allocator@D@_STL@@@_STL@@QAE@I@Z': 4266256, '?ReadInfo@CBankFileEntryUpdateData@@QBEXKPAXK@Z': 10272400, '??0CMemoryDataInputStream@@QAE@PBXK@Z': 10045744, '?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z': 10311888, '?GetEntryDataSize@CBankFile@@QBEKK@Z': 10273344, '??0?$vector@EV?$allocator@E@_STL@@@_STL@@QAE@I@Z': 5328416, '?ReadDataNonAsync@CBankFileAsyncEntry@@QAEXKPAXK@Z': 10310320, '?GPWaterRenderer@NEngineInternal@@3PAVCEngineWaterRenderer@@A': 21196372, '?LoadNonPatchData@CEngineWaterRenderer@@QAEXAAVCMemoryDataInputStream@@0@Z': 11983600, '??1CMemoryDataInputStream@@UAE@XZ': 10042128, '??3@YAXPAX@Z': 12577212, '_WaterFree': 12577300}
aliases={'@LoadWater@4': '?LoadWaterData@CEngineWorldMap@@AAEXXZ', '?WaterStaticMapBankFileName@@3VCCharString@@A': '_WaterName', '?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z': '@Find@12', '??0?$vector@DV?$allocator@D@_STL@@@_STL@@QAE@I@Z': '@Chars@12', '?ReadInfo@CBankFileEntryUpdateData@@QBEXKPAXK@Z': '@ReadInfo@20', '??0CMemoryDataInputStream@@QAE@PBXK@Z': '@Stream@16', '?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z': '@Async@16', '?GetEntryDataSize@CBankFile@@QBEKK@Z': '@DataSize@12', '??0?$vector@EV?$allocator@E@_STL@@@_STL@@QAE@I@Z': '@Bytes@12', '?ReadDataNonAsync@CBankFileAsyncEntry@@QAEXKPAXK@Z': '@ReadData@20', '?GPWaterRenderer@NEngineInternal@@3PAVCEngineWaterRenderer@@A': '_WaterRenderer', '?LoadNonPatchData@CEngineWaterRenderer@@QAEXAAVCMemoryDataInputStream@@0@Z': '@Load@16', '??1CMemoryDataInputStream@@UAE@XZ': '@DestroyStream@4'}
(w/'world_water_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xb41fa0,331);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body,0xb41fa0):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm in targets.values();fixups.append((i.address-0xb41fa0+1,i.operands[0].imm,0))
 elif i.mnemonic=='push' and i.operands[0].type==2 and i.operands[0].imm==0x1436ec8:
  fixups.append((i.address-0xb41fa0+1,0x1436ec8,1))
 elif any(o.type==3 and o.mem.disp==0x1436e54 for o in i.operands):
  fixups.append((i.address-0xb41fa0+2,0x1436e54,1))
assert len(fixups)==17
(w/'world_water_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT']+(['/Dfree=WaterFree'] if n==0 else [])+['/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb41fa0,331,17,targets)
original=w/'unaliased.obj'
r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(original),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
original_targets=dict(targets);original_targets['_free']=original_targets.pop('_WaterFree')
assert check(original,symbol,0xb41fa0,331,17,original_targets)==proof
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,index,infoSize,dataSize,refs,hasInfo in itertools.product((0,0x95),range(7),(0,1,2),(0,4,32),(0,4,32),(1,2),(0,1)):
  a=bytearray([fill])*4096;struct.pack_into('<I',a,0x60,base+0x400)
  for i in range(3):struct.pack_into('<I',a,0x400+i*4,base+0x500)
  struct.pack_into('<I',a,0x504,infoSize);struct.pack_into('<I',a,0x544,infoSize)
  struct.pack_into('<3I',a,0x604,base+0x34,3,0);struct.pack_into('<3I',a,0x800,refs,hasInfo,base+0x600)
  f.write(struct.pack('<3I',mode,index,dataSize));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual331byte parent/17audited fixups. Full4KiB snapshots and normalized callback arguments for stack-local vectors/streams. Missing/present bank entries, empty/nonempty info/data, refcounts/null ownership, callback mutations to metadata, bank owner, renderer and cleanup. Bank lookup/reads, vector constructors, stream lifecycle, LoadNonPatchData, allocation release are explicit boundaries. Only this TU aliases free; CRT free remains intact.')
(ROOT/'rebuild/compile-gate/native-world-water-load.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WORLD_WATER_LOAD_GATE_PASS',snapshots,'snapshots')

"""Static map selection against original execution and callback mutations."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-map-file-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b4/CEngineWorldMap_SetStaticMapFileForUse_00b428e0.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp','00/bd/CEngineLandscapeMemoryManager_EnablePoolAllocation_00bda070.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeMapFileSelection_test.cpp'
select='?SetStaticMapFileForUse@CEngineWorldMap@@QAEXABVCWideString@@@Z'
targets={'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??1CCharString@@QAE@XZ':0x99eae0,'?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z':0x9e9f40,'?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ':0xb40000,'?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A':0x1436ea8,'?EnablePoolAllocation@CEngineLandscapeMemoryManager@@QAEX_N@Z':0xbda070,'??4CWideString@@QAEAAV0@ABV0@@Z':0x99b7d0,'?OpenStaticMaps@CEngineWorldMap@@AAEXW4STATIC_MAP_OPEN_STATE@CEngineMap@@@Z':0xb42750,'?LoadWaterData@CEngineWorldMap@@AAEXXZ':0xb41fa0}
aliases={'@SelectMapFile@12':select,'@ConstructText@16':'??0CCharString@@QAE@PBDJ@Z','@DestroyText@4':'??1CCharString@@QAE@XZ','@CloseMaps@4':'?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ','@DisposePool@4':'?FreeDisposalList@CEngineLandscapeMemoryManager@@QAEXXZ','@AssignWide@12':'??4CWideString@@QAEAAV0@ABV0@@Z','@OpenMaps@12':'?OpenStaticMaps@CEngineWorldMap@@AAEXW4STATIC_MAP_OPEN_STATE@CEngineMap@@@Z','@LoadWater@4':'?LoadWaterData@CEngineWorldMap@@AAEXXZ'}
(w/'map_file_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(282);code[:245]=read_virtual_data(raw,0xb428e0,245);code[256:]=read_virtual_data(raw,0xbda070,26);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
string_addresses=(0x12a202c,0x12a1ffc,0x12a1fd4,0x12a1fac)
for i in md.disasm(code[:245],0xb428e0):
 if i.mnemonic=='call' and i.operands[0].type==2:
  target=i.operands[0].imm;assert target in targets.values();fixups.append((i.address-0xb428e0+1,target,1))
 else:
  for op in i.operands:
   if op.type==3 and op.mem.disp==0x1436ea8:fixups.append((i.address-0xb428e0+i.disp_offset,op.mem.disp,0))
   elif op.type==2 and op.imm in string_addresses:fixups.append((i.address-0xb428e0+i.imm_offset,op.imm,0))
assert len(fixups)==22
assert code[269]==0xe8 and 0xbda070+18+struct.unpack_from('<i',code,270)[0]==0xbd9ce0
fixups.append((270,0xbd9ce0,1))
(w/'map_file_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
c=Coff(objects[0])
for symbol,(sec,offset) in c.symbols.items():
 if symbol.startswith('??_C@'):
  value=c.sections[sec][0][offset:];matches=[a for a in string_addresses if read_virtual_data(raw,a,len(value))==value];assert len(matches)==1;targets[symbol]=matches[0]
proofs=[check(objects[0],select,0xb428e0,245,22,targets),check(objects[2],'?EnablePoolAllocation@CEngineLandscapeMemoryManager@@QAEX_N@Z',0xbda070,26,1,{'?FreeDisposalList@CEngineLandscapeMemoryManager@@QAEXXZ':0xbd9ce0})];assert proofs[0]['grade']=='MATCH'
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
base=0x24000000;cases=0
with (w/'input.bin').open('wb') as f:
 for fill,mode,status,path,renderer,pool in itertools.product((0,0x95),range(8),(0,1,3,0xffffffff),(0x30,0x400),(0x800,0x1000),(0,1,0x80,0xff)):
  a=bytearray([fill])*8192
  for off,val in ((0x1b0,status),(0x30,0x1111),(0x400,0x2222),(0x404,0x3333),(0xeb0,base+0x1800),(0x16b0,base+0x1900)):struct.pack_into('<I',a,off,val)
  a[0x1839]=a[0x1939]=pool
  f.write(struct.pack('<3I',mode,path,renderer));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
snapshots=(w/'compiled.bin').stat().st_size//8204;assert snapshots==cases*37-cases//4
headers=[ROOT/'rebuild/include'/p for p in ('engine/CEngineWorldMap.h','engine/CEngineMap.h','engine/CEngineLandscapeMemoryManager.h','engine/CEngineLandscapeRenderer.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original245/26bytes with23audited fixups and exact four text literals versus compiled provider with real renderer global. Two selections per case;full8KiB snapshots plus callback arguments and renderer singleton. Alias/separate input name;callbacks mutate name,renderer and build status. Text lifecycle,progress,CloseStaticMapFile,pool enable,wide assignment,OpenStaticMaps,LoadWaterData remain recording boundaries.')
(ROOT/'rebuild/compile-gate/native-map-file-selection.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MAP_FILE_SELECTION_GATE_PASS',snapshots,'snapshots')

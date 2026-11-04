"""Engine morph forwarding through actual graphic-bank scene counters."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-morph-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_MorphForwarders_00b239c0.cpp','01/43/NEngineInternal_GPResourceManager_01436e98.cpp','01/43/NEngineInternal_GPSurfaceCompositionManager_01436eac.cpp','00/9f/CGraphicDataBank_SceneDelay_009fd0c0.cpp','00/9f/CResourceList_IncrementFrameCounter_009fc160.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeEngineMorph_test.cpp'
add='?AddCustomMorphTexture@CEngine@@UBEKAAVCTexture@@ABVCCharString@@@Z';update='?UpdateTextureMorphCache@CEngine@@UAEXABV?$CArray@VCEngineTextureMorph@@@@_NMPBVCIEngineLongSet@@@Z'
resource='?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A';surface='?GPSurfaceCompositionManager@NEngineInternal@@3PAVCEngineSurfaceCompositionManager@@A';scene='?ChangeScene@CGraphicDataBank@@QAEXXZ';increment='?IncrementFrameCounter@CResourceList@@QAEXK@Z';custom='?AddCustomMorphTexture@CEngineSurfaceCompositionManager@@QAEKAAVCTexture@@ABVCCharString@@@Z';disk='?UpdateDiskCache@CEngineSurfaceCompositionManager@@QAEXABV?$CArray@VCEngineTextureMorph@@@@_NMPBVCIEngineLongSet@@@Z'
targets={resource:0x1436e98,surface:0x1436eac,scene:0x9fd0c0,increment:0x9fc160,custom:0xb2c4e0,disk:0xb2d9f0}
aliases={'@Add@16':add,'@Update@24':update,'_ResourceGlobal':resource,'_SurfaceGlobal':surface,custom:'@Custom@16',disk:'@Disk@24'}
(w/'engine_morph_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(111);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
specs=[(0,add,0xb23ce0,11,2,0),(0,update,0xb239c0,27,4,16),(3,scene,0x9fd0c0,41,2,48),(4,increment,0x9fc160,15,0,96)]
for n,sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp in (0x1436e98,0x1436eac):fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==8
(w/'engine_morph_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,address,length,count,targets) for n,sym,address,length,count,offset in specs]
for n,(sym,address) in enumerate(((resource,0x1436e98),(surface,0x1436eac)),1):assert coff_data(Coff(objects[n]),sym,4)==read_virtual_data(raw,address,4)==bytes(4)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(5):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for slot,force,bits,delay,counter,mode,alias in itertools.product(range(2),(0,1,255),(0,0x80000000,0x3f800000,0x7f800000,0x7fc12345,0x7f812345),(0,1,0x80000000,0xffffffff),(0,1,0x80000000,0xffffffff),range(4),range(3)):
  result=counter^bits;f.write(struct.pack('<8I',slot,force,bits,delay,counter,mode,alias,result));cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as f,(w/'compiled.bin').open('rb') as g:
 while True:
  a=f.read(1<<20);b=g.read(1<<20);assert a==b,w
  if not a:break
size=(w/'compiled.bin').stat().st_size;assert size%16392==0;snapshots=size//16392
report=dict(cases=cases,snapshots=snapshots,omission_checks=5,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual engine Update/Add morph forwarding,graphic-bank ChangeScene/resource-list increments andtwo globals. Full16KiB+globals independent model acrossUpdate/Add/Update sequence. Modular counters/delay,numericreturnbits,rawbool/floatbits includingNaNs,inputaliases,null/live usedset,callbackresource/surface replacements andbank-delay/input mutation. Descriptor CArray and CIEngineLongSet are existing opaque ABI references,not layout claims. Only surface-composition AddCustomMorphTexture/UpdateDiskCache remaincallbacks.')
(ROOT/'rebuild/compile-gate/native-engine-morph.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ENGINE_MORPH_GATE_PASS',snapshots,'snapshots')

"""Engine frame orchestration through actual parameter, pool, capture helpers."""
import hashlib,itertools,json,re,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-engine-frame-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_RenderFrame_00b25950.cpp','00/42/CEngineInterpolationInfo_CEngineInterpolationInfo_0042fa30.cpp','00/a5/CSimpleMemoryPool_FreeAll_00a5c720.cpp','00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeEngineFrame_test.cpp'
frame='?RenderFrame@CEngine@@AAEXABVCEngineInterpolationInfo@@@Z';ctor='??0CEngineInterpolationInfo@@QAE@XZ';pool='?FreeAll@CSimpleMemoryPool@@QAEXXZ';restore='?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ';state='?FableCurrentRenderStateManager_013cb508@@3PAVCRenderStateManager@@A';defaults='?SetDefaultRenderStates@CEngine@@AAEXXZ'
targets={state:0x13cb508,'__real@4f800000':0x122dcb4,defaults:0xb24850,ctor:0x42fa30,pool:0xa5c720,restore:0xa05840}
aliases={'@Frame@12':frame,state:'_StateGlobal',defaults:'@Defaults@8'}
(w/'engine_frame_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
specs=[(frame,0xb25950,365,7,0),(ctor,0x42fa30,158,0,368),(pool,0xa5c720,101,0,528),(restore,0xa05840,121,0,640)]
raw=EXE.read_bytes();body=bytearray(761);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
for sym,address,length,count,offset in specs:
 code=read_virtual_data(raw,address,length);body[offset:offset+length]=code
 for i in md.disasm(code,address):
  if i.mnemonic in ('call','jmp') and i.operands[0].type==2 and not address<=i.operands[0].imm<address+length:
   assert i.operands[0].imm in targets.values();fixups.append((offset+i.address-address+i.imm_offset,i.operands[0].imm,1))
  else:
   for o in i.operands:
    if o.type==3 and o.mem.disp in (0x13cb508,0x122dcb4):fixups.append((offset+i.address-address+i.disp_offset,o.mem.disp,0))
assert len(fixups)==7
(w/'engine_frame_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][3]={'+','.join('{%d,%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,a,l,c,targets) for n,(sym,a,l,c,off) in enumerate(specs)]
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(4):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for count,layers,mask,mode,frame_id,block,rate,bookmark in itertools.product(range(4),range(3),(0,1,7),range(6),(0,0xffffffff),range(2),(1,4,0x80000000),(1,0x80000000)):
  f.write(struct.pack('<8Id',count,layers,mask,mode,frame_id,block,rate,bookmark,1.5+1/rate));cases+=1
r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin')],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
match=re.search(r'NATIVE_ENGINE_FRAME_PASS cases=(\d+) snapshots=(\d+)',r.stdout);assert match and int(match[1])==cases;snapshots=int(match[2])
report=dict(cases=cases,snapshots=snapshots,omission_checks=4,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual RenderFrame, interpolation constructor, FreeAll, RestoreCaptureBlock. Full64KiB and fixture current-manager global at every callback plus final; independent event/state model. Parameters compared through byte124, excluding three uninitialized tail padding bytes and stack pointer identity. 53-bit nearest x87 with exact reciprocal test inputs and independently supplied expected timestamp. Component/layer filtering, mutable filter/end pointer, updated interpolation after defaults, captured parameters, signed counter wrap, empty/single-block pool, capture-mask overflow and manager replacement. Default render states and component/layer virtual operations remain callbacks; no visual frame.')
(ROOT/'rebuild/compile-gate/native-engine-frame.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_ENGINE_FRAME_GATE_PASS',snapshots,'snapshots')

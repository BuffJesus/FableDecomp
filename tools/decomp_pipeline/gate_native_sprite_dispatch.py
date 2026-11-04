import hashlib,json,subprocess,tempfile,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-sprite-dispatch-'));env=V.env()
specs=[('00/ba/CEnginePrimitiveManager2DSprites_Render2DPrimitive_00baced0.cpp','?Render2DPrimitive@CEnginePrimitiveManager2DSprites@@UAEXAAVCEngineInternalPrimitiveBase@@AAVCEngineSubPrimitiveChain@@W4ESubPrimitiveType@@@Z',0xbaced0,47,0),('00/bb/CEnginePrimitiveManager2DSprites_RenderPrimitive_00bb18d0.cpp','?RenderPrimitive@CEnginePrimitiveManager2DSprites@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z',0xbb18d0,5,64),('00/ba/CEnginePrimitiveManagerSprites_RenderPrimitive_00baf9e0.cpp','?RenderPrimitive@CEnginePrimitiveManagerSprites@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z',0xbaf9e0,30,80)]
sources=[ROOT/'rebuild/src/compiled'/spec[0] for spec in specs];fixture=ROOT/'rebuild/tests/integration/NativeSpriteManagerDispatch_test.cpp'
aliases={'@TwoD@20':specs[0][1],'@Noop@20':specs[1][1],'@ThreeD@20':specs[2][1],'?RenderSprite@CEnginePrimitiveManagerSprites@@IAEXAAVCEngineInternalPrimitiveSprite@@@Z':'@SpriteBoundary@12'}
(w/'sprite_dispatch_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(110)
for p,s,a,l,o in specs:body[o:o+l]=read_virtual_data(raw,a,l)
import capstone
fixup=next(ins.address-0xbaf9e0+81 for ins in capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(body[80:],0xbaf9e0) if ins.mnemonic=='call')
(w/'sprite_dispatch_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned spriteFixup='+str(fixup)+';\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
targets={'?RenderSprite@CEnginePrimitiveManagerSprites@@IAEXAAVCEngineInternalPrimitiveSprite@@@Z':0xbade40};proofs=[check(objects[n],s,a,l,int(n==2),targets) for n,(_,s,a,l,_) in enumerate(specs)];assert all(p['grade']=='MATCH' for p in proofs)
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip());assert 'cases=4608 snapshots=13824' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ['fable_engine_primitive.h','fable_engine_primitive_manager.h','engine/CEnginePrimitiveManager2DSprites.h','engine/CEnginePrimitiveManagerSprites.h']]
report=dict(cases=4608,snapshots=13824,providers=proofs,omission_checks=3,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),header_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in headers},boundary='Actual canonical manager virtualslots7/10 through compiled base dispatch,2D clock/sprite slots13/14,original result/primitive enums.4608cases/13824full4KiB snapshots with raw enable bytes,6primitive types,16layers/list types and callback mutations. Concrete RenderSprite/Clock/SpritePrim operations remain boundaries;no drawing.')
(ROOT/'rebuild/compile-gate/native-sprite-dispatch.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_SPRITE_DISPATCH_GATE_PASS')

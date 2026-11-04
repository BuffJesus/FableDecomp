import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-sprite-masks-'));env=V.env()
specs=[('00/ba/CEngineInternalPrimitive2DSprite_GetRenderLayerMask_00baceb0.cpp','?GetRenderLayerMask@CEngineInternalPrimitive2DSprite@@UAEKXZ',0xbaceb0,4,0),('00/ba/CEngineInternalPrimitive2DClockSprite_GetRenderLayerMask_00bacec0.cpp','?GetRenderLayerMask@CEngineInternalPrimitive2DClockSprite@@UAEKXZ',0xbacec0,6,0)]
raw=EXE.read_bytes();body=bytearray(14);body[:4]=read_virtual_data(raw,0xbaceb0,4);body[8:]=read_virtual_data(raw,0xbacec0,6)
(w/'sprite_masks_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
(w/'sprite_masks_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+s[1]+'")' for a,s in zip(('@SpriteMask@8','@ClockMask@8'),specs))+'\n')
sources=[ROOT/'rebuild/src/compiled'/s[0] for s in specs];fixture=ROOT/'rebuild/tests/integration/NativeSpriteMasks_test.cpp';objects=[];proofs=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
    if i<2:proofs.append(check(obj,*specs[i][1:],{}))
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
    r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=2048 snapshots=4096' in r.stdout
headers=[ROOT/'rebuild/include/engine'/f'{c}.h' for c in ('CEngineInternalPrimitive2DSprite','CEngineInternalPrimitive2DClockSprite')]
report=dict(cases=2048,snapshots=4096,providers=proofs,omission_checks=2,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},header_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual inherited sprite classes dispatch GetRenderLayerMask through original virtualslot28.256mask patterns,2locations,2fills,complete4KiB nonmutation snapshots and native/independent return values. No callbacks.')
(ROOT/'rebuild/compile-gate/native-sprite-masks.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_SPRITE_MASKS_GATE_PASS')

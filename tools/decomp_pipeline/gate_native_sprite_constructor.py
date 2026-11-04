import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-sprite-constructor-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/41/CEnginePrimitive2DSprite_Constructor_0041beb0.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/sprite_ctor_candidate_20261009.cpp'
symbol='??0CEnginePrimitive2DSprite@@QAE@_KABVC2DBoxF@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEKABVC2DVector@@5@Z'
obj=w/'ctor.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;proof=check(obj,symbol,0x41beb0,171,0,{})
(w/'sprite_ctor_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0x41beb0,171)))+'};\n');(w/'sprite_ctor_aliases.h').write_text('#pragma comment(linker,"/alternatename:@SpriteCtor@76='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeSpriteConstructor_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=8192 snapshots=16384' in r.stdout
report=dict(cases=8192,snapshots=16384,providers=[proof],omission_checks=1,source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete171B sprite constructor;no callbacks. Independent/native/compiled full16KiB snapshots;rawfloat patterns,allbytecolourvalues,themeignored,basePad/boolpadding preserved,two placements/fills,eight combinations of own-member input aliases.')
(ROOT/'rebuild/compile-gate/native-sprite-constructor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_SPRITE_CONSTRUCTOR_GATE_PASS')

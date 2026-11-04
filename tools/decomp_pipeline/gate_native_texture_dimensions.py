import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-texture-dimensions-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/9f/CTexture_GetDimensions_009f9db0.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/texture_dimensions_candidate_20261009.cpp'
symbol='?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z';obj=w/'dimensions.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;proof=check(obj,symbol,0x9f9db0,43,0,{})
(w/'texture_dimensions_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0x9f9db0,43)))+'};\n');(w/'texture_dimensions_aliases.h').write_text('#pragma comment(linker,"/alternatename:@TextureDimensions@16='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeTextureDimensions_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=196608 snapshots=786432' in r.stdout
report=dict(cases=196608,snapshots=786432,providers=[proof],omission_checks=1,source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete43B texture GetDimensions with real COM slot17 GetLevelDesc callback. Independent/native/compiled full16KiB callback/final snapshots;raw32bit dimensions/levels,success/failureHRESULT stillconsumesfilleddescriptor,wrapper mutation,two placements/fills,output alias wrapper/interface.')
(ROOT/'rebuild/compile-gate/native-texture-dimensions.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_TEXTURE_DIMENSIONS_GATE_PASS')

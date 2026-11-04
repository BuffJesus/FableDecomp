import hashlib,json,subprocess,sys,tempfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-managed-texture-release-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/a6/CManagedTexture_Release_00a69ad0.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/managed_texture_release_candidate_20261009.cpp'
symbol='?Release@CManagedTexture@@QAE_NXZ';obj=w/'clear.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;proof=check(obj,symbol,0xa69ad0,73,1,{'?ReleaseTexture@CManagedTextureData@@QAEXXZ':0xa69a80})
body=read_virtual_data(EXE.read_bytes(),0xa69ad0,73);assert body[30]==0xe8 and 0xa69ad0+35+struct.unpack_from('<i',body,31)[0]==0xa69a80
(w/'managed_texture_release_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0xa69ad0,73)))+'};\nstatic const unsigned fixupSite=31;\n');(w/'managed_texture_release_aliases.h').write_text('#pragma comment(linker,"/alternatename:@ManagedRelease@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedTextureRelease_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=1152 snapshots=2704' in r.stdout
header=ROOT/'rebuild/include/fable_managed_texture.h'
report=dict(cases=1152,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,header,ROOT/'rebuild/include/fable_managed_texture_data.h',ROOT/'rebuild/include/fable_iv_counted_pointer.h',ROOT/'rebuild/include/fable_resource_list.h')},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete73B managed texture Release; actual boolean return semantics. TextureRelease and virtualDelete callbacks. Independent/native/compiled full16KiB callback/final snapshots;null/count0/1/2/3/80000000/FFFFFFFF,callback pointer replacement/clear/recount,unsigned wrap,final zero,two placements/fills. Source omission fails linkage.')
(ROOT/'rebuild/compile-gate/native-managed-texture-release.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_TEXTURE_RELEASE_GATE_PASS')

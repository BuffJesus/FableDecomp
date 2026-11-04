import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-texture-level-count-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/9f/CTexture_CopyFromTexture_009fa4e0.cpp';obj=w/'provider.obj'
r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
symbol='?GetNoLevels@CTexture@@QBEJXZ';proof=check(obj,symbol,0x9f9df0,9,0,{})
(w/'texture_level_count_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0x9f9df0,9)))+'};\n')
unused=['??_ECSurface@@UAEPAXI@Z','?CopyFromSurface@CSurface@@QAEXPBVC2DBoxI@@ABV1@0K@Z','?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z','?Uninitialise@CSurface@@QAEXXZ']
(w/'texture_level_count_aliases.h').write_text('#pragma comment(linker,"/alternatename:@TextureLevels@8='+symbol+'")\n'+''.join('#pragma comment(linker,"/alternatename:'+s+'=_UnusedBoundary")\n' for s in unused))
fixture=ROOT/'rebuild/tests/integration/NativeTextureLevelCount_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=256 snapshots=1024' in r.stdout,r.stdout
headers=[ROOT/'rebuild/include'/p for p in ['fable_surface_types.h','fable_d3d9.h']]
report=dict(cases=256,snapshots=1024,providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual9B out-of-line CTexture::GetNoLevels COMDAT from already-landed CopyFromTexture TU, completeMATCH. One required actualsource/omission,original/model/compiled full16KiB callback/final snapshots,eight raw32bit signed-long returns,four interface/owner callback mutations,two interfaces/placements/fills.OnlyCOMGetLevelCount boundary; unexecuted parent copy/surface imports explicitly abort. No new copy-parent claim. Physicalleaf9F9DF0 identity replaces inherited parentaddress9FA4E0 in index; no new standalone strong provider or duplicate.')
(ROOT/'rebuild/compile-gate/native-texture-level-count.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_TEXTURE_LEVEL_COUNT_GATE_PASS')

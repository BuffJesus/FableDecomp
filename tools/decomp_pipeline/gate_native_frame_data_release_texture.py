import hashlib,json,subprocess,sys,tempfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-frame-data-release-texture-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/9f/CGraphicInfoFrame_CFrameData_ReleaseTexture_009fea00.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/frame_data_release_texture_candidate_20261009.cpp'
symbol='?ReleaseTexture@CFrameData@CGraphicInfoFrame@@UAEXXZ';obj=w/'clear.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;proof=check(obj,symbol,0x9fea00,24,1,{'?UnloadFrame@CGraphicInfo@@QAEXK@Z':0x9fe8e0})
body=read_virtual_data(EXE.read_bytes(),0x9fea00,24);assert body[18]==0xe8 and 0x9fea00+23+struct.unpack_from('<i',body,19)[0]==0x9fe8e0
(w/'frame_data_release_texture_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0x9fea00,24)))+'};\nstatic const unsigned fixupSite=19;\n');(w/'frame_data_release_texture_aliases.h').write_text('#pragma comment(linker,"/alternatename:@ReleaseFrameTexture@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeFrameDataReleaseTexture_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=524288 snapshots=1572864' in r.stdout
header=ROOT/'rebuild/include/fable_graphic_info.h'
report=dict(cases=524288,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,header,ROOT/'rebuild/include/fable_managed_texture_data.h',ROOT/'rebuild/include/fable_iv_counted_pointer.h',ROOT/'rebuild/include/fable_resource_list.h')},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete24B nested virtual frame-data ReleaseTexture;original GraphicInfo::UnloadFrame callback remains. Independent/native/compiled full16KiB callback/final snapshots;null/live FileData,all65536unsignedFrameNum values,callback replacesFileData/owner/info/FrameNum,no further parent writes,two placements/fills. One source omission;null owner/info not claimed safe.')
(ROOT/'rebuild/compile-gate/native-frame-data-release-texture.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_FRAME_DATA_RELEASE_TEXTURE_GATE_PASS')

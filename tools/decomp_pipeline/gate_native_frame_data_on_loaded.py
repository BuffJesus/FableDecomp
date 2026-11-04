import hashlib,json,subprocess,sys,tempfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-frame-data-on-loaded-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/9f/CGraphicInfoFrame_CFrameData_OnResourceLoaded_009ff860.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/frame_data_on_loaded_candidate_20261009.cpp'
symbol='?OnResourceLoaded@CFrameData@CGraphicInfoFrame@@UAEXXZ';obj=w/'clear.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;proof=check(obj,symbol,0x9ff860,65,1,{'?UploadFrame@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z':0x9ff220})
body=read_virtual_data(EXE.read_bytes(),0x9ff860,65);assert body[12]==0xe8 and 0x9ff860+17+struct.unpack_from('<i',body,13)[0]==0x9ff220
(w/'frame_data_on_loaded_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0x9ff860,65)))+'};\nstatic const unsigned fixupSite=13;\n');(w/'frame_data_on_loaded_aliases.h').write_text('#pragma comment(linker,"/alternatename:@OnLoaded@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeFrameDataOnLoaded_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=36864 snapshots=111680' in r.stdout
header=ROOT/'rebuild/include/fable_graphic_info.h'
report=dict(cases=36864,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,header,ROOT/'rebuild/include/fable_managed_texture_data.h',ROOT/'rebuild/include/fable_iv_counted_pointer.h',ROOT/'rebuild/include/fable_resource_list.h')},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete65B nested virtual OnResourceLoaded;UploadFrame(false) andvirtualDelete callbacks. Independent/native/compiled full16KiB callback/final snapshots;empty/live file/frame/owner data,uploadfalse/true,countwrap0/1/2/3/80000000/FFFFFFFF,callback frame/owner/data mutations,cached owner slot afterupload andafterdeletion,FileDataaliasowner slot,two placements/fills. One mandatory source omission.')
(ROOT/'rebuild/compile-gate/native-frame-data-on-loaded.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_FRAME_DATA_ON_LOADED_GATE_PASS')

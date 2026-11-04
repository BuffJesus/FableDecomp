import hashlib,json,subprocess,sys,tempfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-graphic-frame-data-uninitialise-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/9f/CGraphicFrameData_Uninitialise_009fcba0.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/graphic_frame_data_uninitialise_candidate_20261009.cpp'
symbol='?Uninitialise@CGraphicFrameData@@QAEXXZ';obj=w/'clear.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;proof=check(obj,symbol,0x9fcba0,69,1,{'??3@YAXPAX@Z':0xbfe9bc})
body=read_virtual_data(EXE.read_bytes(),0x9fcba0,69);assert body[45]==0xe8 and 0x9fcba0+50+struct.unpack_from('<i',body,46)[0]==0xbfe9bc
(w/'graphic_frame_data_uninitialise_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0x9fcba0,69)))+'};\nstatic const unsigned fixupSite=46;\n');(w/'graphic_frame_data_uninitialise_aliases.h').write_text('#pragma comment(linker,"/alternatename:@DataUninitialise@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeGraphicFrameDataUninitialise_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=110592 snapshots=239616' in r.stdout
header=ROOT/'rebuild/include/fable_graphic_frame_data.h'
report=dict(cases=110592,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,header,ROOT/'rebuild/include/fable_resource_list.h',ROOT/'rebuild/include/fable_counted_pointer.h')},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete69B frame-data Uninitialise and original inline counted-pointer release; counted-pointee/heapdelete callbacks. Independent/native/compiled full16KiB callback/final snapshots;all256flagbytes,referencecountwrap,null/liveInfo,reloaded info after deletion,callback flag/field changes and final pointerclear,two placements/fills,mandatory omission.')
(ROOT/'rebuild/compile-gate/native-graphic-frame-data-uninitialise.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_GRAPHIC_FRAME_DATA_UNINITIALISE_GATE_PASS')

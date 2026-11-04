import hashlib,json,subprocess,sys,tempfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-frame-data-sizeof-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/9f/CGraphicInfoFrame_CFrameData_Sizeof_009fdbe0.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/frame_data_sizeof_candidate_20261009.cpp'
symbol='?Sizeof@CFrameData@CGraphicInfoFrame@@QBEKXZ';obj=w/'size.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
proof=check(obj,symbol,0x9fdbe0,51,2,{'?Sizeof@CGraphicFrameData@@QBEKXZ':0x9fcbf0,'?Sizeof@CManagedTexture@@QBEKXZ':0xa69e60})
body=read_virtual_data(EXE.read_bytes(),0x9fdbe0,51);sites=[12,40]
for site,target in zip(sites,[0x9fcbf0,0xa69e60]):assert body[site-1]==0xe8 and 0x9fdbe0+site+4+struct.unpack_from('<i',body,site)[0]==target
(w/'frame_data_sizeof_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned sites[]={12,40};\n')
(w/'frame_data_sizeof_aliases.h').write_text('#pragma comment(linker,"/alternatename:@FrameDataSizeof@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeFrameDataSizeof_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=4608' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','fable_managed_texture.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_iv_counted_pointer.h')]
report=dict(cases=4608,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete51B nested FrameData::Sizeof; deferred GraphicFrameData::Sizeof and ManagedTexture::Sizeof remain callbacks, no claim of recovering them. Independent/native/compiled full16KiB callback/final snapshots plus unsigned return; null/live frame and loading texture, raw32bit child sizes/wrap, frame callback changes frame/loading pointer before loading decision, managed callback changes fields, two placements/fills. Source omission fails.')
(ROOT/'rebuild/compile-gate/native-frame-data-sizeof.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_FRAME_DATA_SIZEOF_GATE_PASS')

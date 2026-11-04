import hashlib,json,subprocess,sys,tempfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-graphic-info-frame-release-data-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/a0/CGraphicInfoFrame_ReleaseData_00a00680.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/graphic_info_frame_release_data_candidate_20261009.cpp'
symbol='?ReleaseData@CGraphicInfoFrame@@ABEXXZ';obj=w/'size.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
proof=check(obj,symbol,0xa00680,36,0,{})
body=read_virtual_data(EXE.read_bytes(),0xa00680,36)
(w/'graphic_info_frame_release_data_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
(w/'graphic_info_frame_release_data_aliases.h').write_text('#pragma comment(linker,"/alternatename:@FrameReleaseData@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeGraphicInfoFrameReleaseData_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=512 snapshots=1088' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','fable_managed_texture.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_iv_counted_pointer.h')]
report=dict(cases=512,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete36B original private const CGraphicInfoFrame::ReleaseData; donor B5E2D0/PDB38B identity and owner-target call in nested FreeTextureMemory establish shared-body identity despite Ghidra AnimatedBlock label. Independent/native/compiled full16KiB callback/final snapshots;null/live Data,rawrefwrap,virtualDelete changes ownerslot,explicit final clear,cached pointee,two pointee positions/arena placements/fills. OnlyvirtualDelete boundary;source omission fails.')
(ROOT/'rebuild/compile-gate/native-graphic-info-frame-release-data.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_GRAPHIC_INFO_FRAME_RELEASE_DATA_GATE_PASS')

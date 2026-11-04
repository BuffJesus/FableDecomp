import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-resource-bank-frame-end-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/9f/CResourceList_FrameEnd_009fc150.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/resource_bank_frame_end_candidate_20261009.cpp'
symbol='?FrameEnd@CResourceBank@@QAEXXZ';obj=w/'clear.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;proof=check(obj,symbol,0x9fc430,15,0,{})
(w/'resource_bank_frame_end_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,read_virtual_data(EXE.read_bytes(),0x9fc430,15)))+'};\n');(w/'resource_bank_frame_end_aliases.h').write_text('#pragma comment(linker,"/alternatename:@BankFrameEnd@8='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeResourceBankFrameEnd_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=144 snapshots=288' in r.stdout
header=ROOT/'rebuild/include/fable_game_text_bank.h'
report=dict(cases=144,snapshots=288,providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,header,ROOT/'rebuild/include/fable_resource_list.h')},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete15B resource-bank FrameEnd, donor public B6C3A0 identity. Independent/native/compiled full16KiB final snapshots, no callbacks; both current frames increment modulo32 and unload counters clear, other bytes preserved, six raw counter patterns each, two placements/fills, one mandatory source omission.')
(ROOT/'rebuild/compile-gate/native-resource-bank-frame-end.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_RESOURCE_BANK_FRAME_END_GATE_PASS')

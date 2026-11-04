import hashlib,json,subprocess,sys,tempfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-managed-texture-assignment-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/a6/CManagedTexture_Assignment_00a69b20.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/managed_texture_assignment_candidate_20261009.cpp'
symbol='??4CManagedTexture@@QAEABV0@ABV0@@Z';obj=w/'size.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
proof=check(obj,symbol,0xa69b20,72,1,{'?Release@CManagedTexture@@QAE_NXZ':0xa69ad0})
body=read_virtual_data(EXE.read_bytes(),0xa69b20,72);assert body[18]==0xe8 and 0xa69b20+23+struct.unpack_from('<i',body,19)[0]==0xa69ad0
(w/'managed_texture_assignment_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
(w/'managed_texture_assignment_aliases.h').write_text('#pragma comment(linker,"/alternatename:@ManagedAssign@12='+symbol+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedTextureAssignment_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=120);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=73728 snapshots=207360' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','fable_managed_texture.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_iv_counted_pointer.h')]
report=dict(cases=73728,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Exact complete72B original CManagedTexture assignment const-reference return and argument;PDB donor B7D230 corrects Ghidra BlendState identity. Independent/native/compiled full16KiB callback/final snapshots and return-this pointer;null/same/distinct Data,selfassignment,rawrefwrap,Release changes source/target Data,second identity check afterRelease,virtualDelete changes source/target,source reloaded afterdeletion,oldpointee cached,target overwritten/refincrement,two placements/fills. Release andvirtualDelete remain callbacks;one mandatory source omission.')
(ROOT/'rebuild/compile-gate/native-managed-texture-assignment.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_TEXTURE_ASSIGNMENT_GATE_PASS')

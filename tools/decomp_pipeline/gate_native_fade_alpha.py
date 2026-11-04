import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-fade-alpha-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/43/CFadeInFadeOutBase_GetFadeInOutAlpha_004348d0.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/fade_alpha_candidate_20261009.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeFadeAlpha_test.cpp';objects=[];raw=read_virtual_data(EXE.read_bytes(),0x4348d0,79);cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;sites=[]
for ins in cs.disasm(raw,0x4348d0):
 if ins.disp_size==4:
  assert ins.disp in [0x129ba3c,0x122ded8,0x122dedc];sites.append((ins.address-0x4348d0+ins.disp_offset,[0x129ba3c,0x122ded8,0x122dedc].index(ins.disp)))
assert len(sites)==4
(w/'fade_alpha_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,raw))+'};\nstatic const unsigned constantSites[][2]={'+','.join('{%u,%u}'%s for s in sites)+'};\n')
for i,path in enumerate([source,fixture]):
 obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],'?GetFadeInOutAlpha@CFadeInFadeOutBase@@QAEMXZ',0x4348d0,79,5,{'__real@38d1b717':0x129ba3c,'__real@3f800000':0x122ded8,'__real@00000000':0x122dedc})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr[-1000:]);assert 'cases=24576 snapshots=49152' in r.stdout
report=dict(cases=24576,snapshots=49152,providers=[proof],omission_checks=1,source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},header_sha256=hashlib.sha256((ROOT/'rebuild/include/fable_fade_in_out.h').read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete79B native function includes allreturn branches;no callbacks.Full8KiB nonmutation snapshots plus independent double-precision model and bit-identical native/compiled returns;epsilon neighbors,signedzero,infinities,quiet/signalingNaNs,denormals,allflags and three x87 precision settings.')
(ROOT/'rebuild/compile-gate/native-fade-alpha.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_FADE_ALPHA_GATE_PASS')

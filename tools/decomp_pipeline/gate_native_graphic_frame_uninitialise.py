import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-graphic-frame-uninitialise-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a0/CGraphicFrame_UnInitialise_00a00b90.cpp'
if '--stage' in sys.argv or not source.exists():source=ROOT/'work/parity_addtext/graphic_frame_uninitialise_candidate_20261009.cpp'
symbol='?UnInitialise@CGraphicFrame@@QAEXXZ';targets={'?Uninitialise@CGraphicFrameData@@QAEXXZ':0x9fcba0,'??3@YAXPAX@Z':0xbfe9bc}
obj=w/'loader.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
proof=check(obj,symbol,0xa00b90,94,3,targets);body=read_virtual_data(EXE.read_bytes(),0xa00b90,94);cs=Cs(CS_ARCH_X86,CS_MODE_32);fixups=[];addresses=list(targets.values())
for ins in cs.disasm(body,0xa00b90):
 if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
  target=int(ins.op_str,16);assert target in addresses;fixups.append((ins.address-0xa00b90+1,addresses.index(target)))
assert len(fixups)==3
(w/'graphic_frame_uninitialise_retail.h').write_text('struct Fixup{unsigned site,target;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u}'%f for f in fixups)+'};\n')
(w/'graphic_frame_uninitialise_aliases.h').write_text('#pragma comment(linker,"/alternatename:@FrameUninitialise@8='+symbol+'")\n#pragma comment(linker,"/alternatename:?Uninitialise@CGraphicFrameData@@QAEXXZ=@DataUninitialise@8")\n')
fixture=ROOT/'rebuild/tests/integration/NativeGraphicFrameUninitialise_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=124416 snapshots=289872' in r.stdout
headers=[ROOT/'rebuild/include'/p for p in ('fable_graphic_info.h','fable_graphic_info_frame.h','engine/CGraphicDataBank.h','engine/CManagedTextureOwner.h','fable_managed_texture.h','fable_managed_texture_data.h','engine/CGraphicFrame.h','fable_graphic_frame.h','fable_graphic_frame_data.h','fable_game_text_bank.h','fable_counted_pointer.h','fable_resource_list.h','fable_iv_counted_pointer.h')]
report=dict(cases=124416,snapshots=int(r.stdout.split('snapshots=')[1]),providers=[proof],omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Complete94B frame UnInitialise and original inline frame-data/count-pointer destruction. DataUninitialise, counted-pointee deletion and heap delete boundaries. Independent/native/compiled full16KiB snapshots after callbacks and return. Null/live,count wrap,packed initialised bit,callback owner/control-block mutations,reloads before deletion,padding preserved,final owner clear,two placements/fills;mandatory source omission.')
(ROOT/'rebuild/compile-gate/native-graphic-frame-uninitialise.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_GRAPHIC_FRAME_UNINITIALISE_GATE_PASS')

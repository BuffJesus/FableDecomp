"""Landscape visibility teardown, real static discard, and mutating deletion callbacks."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-landscape-visibility-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/bd/CEngineLandscapeMap_OnMapVisibilityChanged_00bdc3c0.cpp'
discard=ROOT/'rebuild/src/compiled/00/9d/CBankFileAsyncData_DiscardAsyncData_009d68a0.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeLandscapeMapVisibility_test.cpp'
symbol='?OnMapVisibilityChanged@CEngineLandscapeMap@@QAEX_N@Z'
static='?DiscardAsyncData@CBankFileAsyncData@@SIXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z'
bank='?DiscardAsyncData@CBankFileAsync@@QAEXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z'
release='?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z';finish='?DeleteUnusedBlocks@CEngineStaticMapFileBlockManager@@QAEXXZ'
targets={static:0x9d68a0,'??_V@YAXPAX@Z':0xbfeb1c,release:0xbe0400,finish:0xbe04f0}
aliases={'@Entry@12':symbol,bank:'@Discard@12',release:'@Release@12',finish:'@Finish@4'}
(w/'landscape_visibility_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
oracle=EXE.read_bytes();body=read_virtual_data(oracle,0xbdc3c0,133);wrapper=read_virtual_data(oracle,0x9d68a0,16)
for fix,target in zip((22,60,89,125),targets.values()):assert body[fix-1]==0xe8 and 0xbdc3c0+fix+4+struct.unpack_from('<i',body,fix)[0]==target
assert wrapper[10]==0xe8 and 0x9d68af+struct.unpack_from('<i',wrapper,11)[0]==0x9d67e0
(w/'landscape_visibility_retail.h').write_text('\n'.join('static const unsigned char '+name+'[]={'+','.join(str(b) for b in data)+'};' for name,data in [('retailCode',body),('discardCode',wrapper)]))
objects=[]
for i,p in enumerate([source,discard,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],symbol,0xbdc3c0,133,4,targets),check(objects[1],static,0x9d68a0,16,1,{bank:0x9d67e0})]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in (0,1):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,visible,dynamic,loading,patch,tree,request,manager in itertools.product((0,0x55,0xaa,0xff),range(8),range(2),range(2),range(2),range(3),range(2),range(2),range(2)):
 arena=bytearray([fill])*4096
 def put(off,value):struct.pack_into('<I',arena,off,value)
 put(4,base+0x800 if tree else 0);put(8,base+0x604 if patch else 0);put(0x600,patch-1 if patch else 0)
 put(0x604,base+0xd00);put(0x800,base+0xd10);put(0x900,base+0xd10)
 put(0x20,base+0xa00 if manager else 0);put(0x30,0xfffffffe)
 put(0x38,base+0x400 if loading else 0);put(0x428,base+0xc00)
 arena[0x5d]=dynamic;arena[0x5e]=request
 inputs.extend(struct.pack('<2I',mode,visible));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'landscape teardown state');assert len(actual)%(4096+12)==0
snapshots=len(actual)//(4096+12)
headers=[ROOT/'rebuild/include'/p for p in ('candidates/landscape_background_lod_compiled.h','engine/CEngineLandscapePatch.h','engine/CLandscapeBackgroundTreeNode.h','engine/CEngineStaticMapFileBlockManager.h','fable_game_text_bank.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source,discard]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original133byte landscape body and16byte static discard execute natively with five audited relocations. Four fills,visibility/dynamic/loading branches,null/zero-cookie/nonempty patch arrays,background tree,request and manager branches. Bank callback changes dynamic flag;array delete changes request/key/tree/manager;release replaces tree;tree deletion clears/replaces manager. Full4KiB snapshots without normalization. Bank owner,virtual deleting destructors,array deallocation,ReleaseRequest and DeleteUnusedBlocks remain recorded boundaries.')
(ROOT/'rebuild/compile-gate/native-landscape-visibility.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_LANDSCAPE_VISIBILITY_GATE_PASS',snapshots,'snapshots')

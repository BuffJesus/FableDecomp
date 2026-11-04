"""Full landscape clear and actual close/static-discard composition."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-landscape-clear-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/bd/CEngineLandscapeMap_Clear_00bdc450.cpp','00/bd/CEngineLandscapeMap_CloseStaticMap_00bdc4f0.cpp','00/9d/CBankFileAsyncData_DiscardAsyncData_009d68a0.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeLandscapeClear_test.cpp'
clear='?Clear@CEngineLandscapeMap@@QAEXXZ';close='?CloseStaticMap@CEngineLandscapeMap@@QAEXXZ';static='?DiscardAsyncData@CBankFileAsyncData@@SIXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z';bank='?DiscardAsyncData@CBankFileAsync@@QAEXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z';release='?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z'
aliases={'@Entry@4':clear,'@Close@4':close,bank:'@Discard@12',release:'@Release@12'}
(w/'landscape_clear_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(241)
for address,n,offset,pairs in ((0xbdc450,149,0,((33,0xbfeb1c),(86,0xbfe9bc),(113,0x9d68a0),(130,0xbe0400))),(0x9d68a0,16,192,((11,0x9d67e0),)),(0xbdc4f0,17,224,((4,0xbdc450),))):
 body=read_virtual_data(raw,address,n);code[offset:offset+n]=body
 for fix,target in pairs:assert body[fix-1]==0xe8 and address+fix+4+struct.unpack_from('<i',body,fix)[0]==target
(w/'landscape_clear_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in code)+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
targets={clear:0xbdc450,static:0x9d68a0,bank:0x9d67e0,release:0xbe0400,'??_V@YAXPAX@Z':0xbfeb1c,'??3@YAXPAX@Z':0xbfe9bc}
proofs=[check(objects[i],s,a,n,count,targets) for i,s,a,n,count in ((0,clear,0xbdc450,149,4),(1,close,0xbdc4f0,17,1),(2,static,0x9d68a0,16,1))]
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,patch,tree,loading,refs,request,kind in itertools.product((0,0x55,0xaa,0xff),range(7),range(3),range(2),range(2),(None,0,1,2,0xffffffff),range(2),range(2)):
 arena=bytearray([fill])*4096
 def put(off,value):struct.pack_into('<I',arena,off,value)
 put(4,base+0x800 if tree else 0);put(8,base+0x604 if patch else 0);put(0x600,patch-1 if patch else 0);put(0x604,0x25000000);put(0x800,0x25000004);put(0x900,0x25000004)
 put(0x18,base+0xa00);put(0x1c,base+0xe00 if refs is not None else 0);put(0xe00,refs or 0);put(0xe08,base+0xa00);put(0xe40,1);put(0xe48,base+0xa40)
 put(0x38,base+0xb00 if loading else 0);put(0xb28,base+0xd00);put(0x20,base+0xc00);put(0x30,0xffffffff);arena[0x5d]=fill&1;arena[0x5e]=request
 inputs.extend(struct.pack('<2I',mode,kind));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'landscape clear/lifetime');assert len(actual)%(4096+12)==0
snapshots=len(actual)//(4096+12)
headers=[ROOT/'rebuild/include'/p for p in ('candidates/landscape_background_lod_compiled.h','engine/CEngineLandscapePatch.h','engine/CLandscapeBackgroundTreeNode.h','fable_game_text_bank.h','fable_counted_pointer.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=3,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Three original bodies execute natively with six audited calls;actual landscape close and static discard compose. Four fills,null/zero-cookie/nonempty patch arrays,null/tree,loading,control null/count0/1/2/ffffffff,request flags. Callbacks replace tree/controlblocks,overwrite ownership fields,replace request manager/key and reassert header/flag. Full4KiB snapshots;virtual deletion,allocation,bank discard and request release remain recorded boundaries,no normalization.')
(ROOT/'rebuild/compile-gate/native-landscape-clear.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_LANDSCAPE_CLEAR_GATE_PASS',snapshots,'snapshots')

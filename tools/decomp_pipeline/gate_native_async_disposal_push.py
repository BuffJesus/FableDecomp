"""Native counted-pointer disposal-list append with allocator mutations."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-async-disposal-push-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9d/STLAsyncDisposalList_push_back_009d7b00.cpp';fixture=ROOT/'rebuild/tests/integration/NativeAsyncDisposalPush_test.cpp';objects=[]
for i,extra in enumerate(([],['/Dmalloc=PushMalloc'])):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj)]+extra+[str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
c=Coff(objects[0]);symbol=next(s for s in c.symbols if s.startswith('?push_back'))
(w/'async_disposal_push_aliases.h').write_text('#pragma comment(linker,"/alternatename:@Entry@12='+symbol+'")\n')
body=read_virtual_data(EXE.read_bytes(),0x9d7b00,59);assert body[6]==0xe8 and 0x9d7b0b+struct.unpack_from('<i',body,7)[0]==0xbfea0e
(w/'async_disposal_push_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b) for b in body)+'};\n')
obj=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],symbol,0x9d7b00,59,1,{'_malloc':0xbfea0e})
renamed=Coff(objects[1]);sec,off=c.symbols[symbol];other,otheroff=renamed.symbols[symbol];body,rel=c.sections[sec];newbody,newrel=renamed.sections[other]
assert off==otheroff and body==newbody and {p:('_PushMalloc' if t=='_malloc' else t,k) for p,(t,k) in rel.items()}==newrel
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[2])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects[1:])),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;base=0x24000000
for fill,mode,list_offset,count,alias,refs,present in itertools.product((0,0x55,0xff),range(3),(0,0x100),(0,1,3),range(2),(None,0,1,2,0xffffffff),range(2)):
 if alias and count==0:continue
 arena=bytearray([fill])*4096
 def put(at,value):struct.pack_into('<I',arena,at,value)
 put(list_offset,base+0x200);nodes=[0x500+32*i for i in range(count)];chain=[0x200]+nodes
 for i,node in enumerate(chain):put(node,base+chain[(i+1)%len(chain)]);put(node+4,base+chain[(i-1)%len(chain)])
 put(0x300,base+0x300);put(0x304,base+0x300);value=0x508 if alias else 0x400
 put(value,base+0xc80 if present else 0);put(value+4,base+0xa00 if refs is not None else 0);put(0xa00,refs or 0);put(0xb00,0xffffffff)
 inputs.extend(struct.pack('<3I',mode,list_offset,value));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
 r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'disposal push');assert len(actual)==cases*4*4108
headers=[ROOT/'rebuild/include'/p for p in ('fable_game_text_bank.h','fable_counted_pointer.h','fable_stlport_list.h')]
report=dict(cases=cases,snapshots=cases*4,omission_checks=1,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original59byte body natively executed with one audited malloc fixup;renamed production body/relocations identical. Two appends,empty/single/three-node lists,two list owners,source aliases existing list value,null/data pointers,Info null/count0/1/2/ffffffff,allocator replaces source pointer or owner sentinel after cached-sentinel load. Full4KiB at allocation and return;malloc remains recorded boundary,no normalized bytes.')
(ROOT/'rebuild/compile-gate/native-async-disposal-push.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ASYNC_DISPOSAL_PUSH_GATE_PASS',cases*4,'snapshots')

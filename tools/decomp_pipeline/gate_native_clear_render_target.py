"""Complete clear parent: audited native body and retail full-state replay."""
import hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
import oracle_native_clear_render_target as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-clear-render-target-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9d/CRenderManager2D_ClearRenderTarget_009d8cf0.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeClearRenderTarget_test.cpp'
ids={name:i+1 for i,name in enumerate(sorted(set(oracle.names.values())))}
(w/'native_clear_event_ids.h').write_text('\n'.join('#define E_%s %d'%(name,value)for name,value in ids.items())+'\n')
objects=[]
for i,path in enumerate((source,fixture)):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/GS','/I'+str(w),'/Fo'+str(obj),str(path)],env,w)
 assert not r.returncode,r.stdout+r.stderr
 objects.append(obj)
proof=check(objects[0],'?ClearRenderTarget@CRenderManager2D@@QAEXABVCRGBColour@@K@Z',oracle.ENTRY,3385,72,oracle.targets)
# Test-only absolute data symbols: no instructions or retail code are linked.
# The native fixture maps these globals at the same addresses as the oracle,
# so all state pointers can be compared without masking pointer fields.
strings=bytearray();symbols=bytearray();globals_={name:value for name,value in oracle.targets.items()if '@@3' in name}
for name,value in globals_.items():
 symbols.extend(struct.pack('<2IIhHBB',0,4+len(strings),value,-1,0,2,0));strings.extend(name.encode()+b'\0')
bindings=w/'globals.obj';bindings.write_bytes(struct.pack('<HHIIIHH',0x14c,0,0,20,len(globals_),0,0)+symbols+struct.pack('<I',4+len(strings))+strings)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/fixed','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1]),str(bindings)],env=env,capture_output=True,text=True);assert r.returncode,'parent provider required'
r=subprocess.run(cmd+list(map(str,objects))+[str(bindings)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
runner=oracle.Runner();input_file=w/'input.bin';expected_file=w/'expected.bin';actual_file=w/'actual.bin'
cases=snapshots=0;digest=hashlib.sha256()
with input_file.open('wb') as inp,expected_file.open('wb') as out:
 inp.write(runner.u.mem_read(0x1390000,0x40000))
 for flags,mutation,arena,shader,layout in oracle.scenarios():
  inp.write(struct.pack('<2I',flags,mutation));inp.write(arena);inp.write(shader);inp.write(layout)
  for name,args,state,shader_state in runner.run(arena,shader,layout,flags,mutation):
   receiver=args[0]if args else 0;values=[v for v in args[1:]if isinstance(v,int)];payload=b''.join(v for v in args[1:]if isinstance(v,bytes))
   assert len(values)<=5 and len(payload)<=140
   header=[ids.get(name,0),receiver,len(values)]+values+[0]*(5-len(values))+[len(payload),0]
   record=struct.pack('<10I',*header)+payload+bytes(140-len(payload))+state+shader_state
   out.write(record);digest.update(record);snapshots+=1
  cases+=1
r=subprocess.run([str(exe),str(input_file),str(actual_file)],env=env,capture_output=True,text=True,timeout=90);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
offset=0
with expected_file.open('rb') as expected,actual_file.open('rb') as actual:
 while True:
  left=expected.read(1024*1024);right=actual.read(1024*1024)
  if left!=right:
   at=next((i for i,(x,y)in enumerate(zip(left,right))if x!=y),min(len(left),len(right)))+offset
   raise AssertionError((w,at,at//25744,at%25744))
  if not left:break
  offset+=len(left)
assert (cases,snapshots)==(4096,34304)
report=dict(providers=[proof],cases=cases,snapshots=snapshots,compared_bytes=offset,trace_sha256=digest.hexdigest(),
            omission_checks=1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),
            boundary='Real native parent, recorded engine dependencies. Both clear modes; flags0..15; shader enable states; pending updates; identity/reset; empty/nonempty presets; dirty states; callback mutations. Full24KiB arena plus988-byte shader object at every call/return. Four unspecified specular vertex words alone are masked. No GPU rendering.')
(ROOT/'rebuild/compile-gate/native-clear-render-target.json').write_text(json.dumps(report,indent=2)+'\n')
for path in (input_file,expected_file,actual_file):path.unlink()
print(r.stdout.strip(),snapshots,'full-state snapshots',flush=True)

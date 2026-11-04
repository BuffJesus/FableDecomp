"""Complete buffered-draw parent, optionally with native state-block dispatch."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
import oracle_native_render2d_list as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
compose=True
w=Path(tempfile.mkdtemp(prefix='native-render2d-list-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9d/CRenderManager2D_Render2DDrawList_009da9f0.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeRender2DList_test.cpp'
ids={name:i+1 for i,name in enumerate(sorted(set(oracle.names.values())))}
(w/'native_render2d_event_ids.h').write_text('\n'.join('#define E_%s %d'%(name,value)for name,value in ids.items())+'\n')
objects=[]
for i,path in enumerate((source,fixture)):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/GS','/I'+str(w),'/Fo'+str(obj),str(path)]+(['/DFABLE_COMPOSE_STATE_BLOCKS']if compose and i==1 else[]),env,w)
 assert not r.returncode,r.stdout+r.stderr
 objects.append(obj)
proof=check(objects[0],'?Render2DDrawList@CRenderManager2D@@QAEX_N@Z',oracle.ENTRY,3344,127,oracle.targets)
for i,path in enumerate(['00/9f/CTexture_Uninitialise_009f9f70.cpp','00/9f/CTexture_Assignment_009fa1c0.cpp','00/9a/CRenderManagerCore_AttachTextureToStage_009a0cf0.cpp','00/9e/CQuickDrawTriInfoVector_Erase_009e15e0.cpp','00/9e/Render2DVertexVector_Erase_009e1440.cpp','00/9f/CTexture_PreallocatedStorage_009fa230.cpp']):
 obj=w/('support%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
if compose:
 for i,path in enumerate(['00/9d/CStateBlockFunctions_Apply_009df060.cpp','01/39/CRenderManager2D_StateBlockLibrary_01396510.cpp']):
  obj=w/('blocks%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
# Test-only absolute data symbols: no instructions or retail code are linked.
# The native fixture maps these globals at the same addresses as the oracle,
# so all state pointers can be compared without masking pointer fields.
strings=bytearray();symbols=bytearray();globals_={name:value for name,value in oracle.targets.items()if '@@3' in name or 'LibraryStateBlockFunctions' in name or 'D3DTextureMipMapModeTable' in name}
if compose:globals_={name:value for name,value in globals_.items()if 'LibraryStateBlockFunctions'not in name}
for name,value in globals_.items():
 symbols.extend(struct.pack('<2IIhHBB',0,4+len(strings),value,-1,0,2,0));strings.extend(name.encode()+b'\0')
bindings=w/'globals.obj';bindings.write_bytes(struct.pack('<HHIIIHH',0x14c,0,0,20,len(globals_),0,0)+symbols+struct.pack('<I',4+len(strings))+strings)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/fixed','/out:'+str(exe),'kernel32.lib']
for omit in [i for i in range(len(objects))if i!=1]:
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit]+[str(bindings)],env=env,capture_output=True,text=True);assert r.returncode,('required provider',omit)
r=subprocess.run(cmd+list(map(str,objects))+[str(bindings)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
runner=oracle.Runner();input_file=w/'input.bin';expected_file=w/'expected.bin';actual_file=w/'actual.bin'
cases=snapshots=full_snapshots=0;digest=hashlib.sha256();trace=[];position=0
with input_file.open('wb') as inp,expected_file.open('wb') as out:
 inp.write(runner.u.mem_read(0x1390000,0x40000))
 for flags,mutation,arena,shader,layout in oracle.scenarios():
  inp.write(struct.pack('<3I',flags,mutation,len(arena)));inp.write(arena);inp.write(shader);inp.write(layout)
  for name,args,state,shader_state in runner.run(arena,shader,layout,flags,mutation):
   receiver=args[0]if args else 0;values=[v for v in args[1:]if isinstance(v,int)];payload=b''.join(v for v in args[1:]if isinstance(v,bytes))
   assert len(values)<=5 and len(payload)<=140
   header=[ids.get(name,0),receiver,len(values)]+values+[0]*(5-len(values))+[len(payload),0]
   record=struct.pack('<10I',*header)+payload+bytes(140-len(payload))+state+shader_state
   trace.append((position,cases,name));position+=len(record);out.write(record);digest.update(record);snapshots+=1;full_snapshots+=bool(state)
  cases+=1
r=subprocess.run([str(exe),str(input_file),str(actual_file)],env=env,capture_output=True,text=True,timeout=90);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
offset=0
with expected_file.open('rb') as expected,actual_file.open('rb') as actual:
 while True:
  left=expected.read(1024*1024);right=actual.read(1024*1024)
  if left!=right:
   at=next((i for i,(x,y)in enumerate(zip(left,right))if x!=y),min(len(left),len(right)))+offset
   import bisect
   item=trace[bisect.bisect_right([t[0]for t in trace],at)-1];raise AssertionError((w,at,item,at-item[0]))
  if not left:break
  offset+=len(left)
assert (cases,snapshots,full_snapshots)==(520,35008,14996)
report=dict(providers=[proof],cases=cases,snapshots=snapshots,full_snapshots=full_snapshots,compared_bytes=offset,trace_sha256=digest.hexdigest(),
            omission_checks=len(objects)-1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),
            boundary='Actual native Render2DDrawList parent, state-block library, texture adoption/accounting/assignment/cleanup/cache binding, info and vertex erasure. Shader, state-realization/restoration, viewport, font virtual methods, string destruction and COM remain boundaries. Small cases compare complete36KiB arena plus shader at every event; large cases compare768KiB at engine/draw/return events, with ordered compact string-destruction events and final full arena. Empty and mixed text/geometry queues, callback mutations, depth booleans and2000/2001/2002/4003-primitive batches. No GPU rendering.')
(ROOT/'rebuild/compile-gate/native-render2d-list.json').write_text(json.dumps(report,indent=2)+'\n')
for path in (input_file,expected_file,actual_file):path.unlink()
print(r.stdout.strip(),full_snapshots,'full-state snapshots',snapshots-full_snapshots,'compact string events',flush=True)

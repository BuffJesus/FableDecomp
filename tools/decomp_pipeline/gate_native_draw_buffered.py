"""Complete buffered-draw parent, optionally with native state-block dispatch."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
import oracle_native_draw_buffered as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--state-blocks',action='store_true');compose=parser.parse_args().state_blocks
if compose:
 for address in (0x1396de8,0x1396f78,0x1396f7c,0x1396f6c,0x1396f68,0x1396ef0,0x1396e7c,0x1396e5c):
  oracle.targets[f'?FableRenderDefaultValue_{address:08x}@@3KA']=address
w=Path(tempfile.mkdtemp(prefix='native-draw-buffered-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/9d/CRenderManager2D_DrawBufferedPolys_009d9c80.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeDrawBuffered_test.cpp'
ids={name:i+1 for i,name in enumerate(sorted(set(oracle.names.values())))}
(w/'native_draw_event_ids.h').write_text('\n'.join('#define E_%s %d'%(name,value)for name,value in ids.items())+'\n')
objects=[]
for i,path in enumerate((source,fixture)):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/GS','/I'+str(w),'/Fo'+str(obj),str(path)]+(['/DFABLE_COMPOSE_STATE_BLOCKS']if compose and i==1 else[]),env,w)
 assert not r.returncode,r.stdout+r.stderr
 objects.append(obj)
proof=check(objects[0],'?DrawBufferedPolys@CRenderManager2D@@QAEXXZ',oracle.ENTRY,3435,93,oracle.targets)
from gate_game_component_abi import Coff
replay=w/'parent-replay.obj'
r=V.cl(['/nologo','/c','/O2','/MT','/GS','/Dqsort=FableDrawTestQsort','/Fo'+str(replay),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
original,renamed=Coff(objects[0]),Coff(replay)
for symbol,(section,offset) in original.symbols.items():
 if symbol not in renamed.symbols or section not in original.sections:continue
 body,rel=original.sections[section]
 if not rel:continue
 other,other_offset=renamed.symbols[symbol];other_body,other_rel=renamed.sections[other]
 assert body==other_body and offset==other_offset
 assert {o:({'_qsort':'_FableDrawTestQsort'}.get(t,t),k)for o,(t,k)in rel.items()}==other_rel
objects[0]=replay
for i,path in enumerate(['00/9f/CTexture_Uninitialise_009f9f70.cpp','00/9f/CTexture_Assignment_009fa1c0.cpp','00/9a/CRenderManagerCore_AttachTextureToStage_009a0cf0.cpp','00/9e/CQuickDrawTriInfoVector_Erase_009e15e0.cpp','00/9d/Render2D_DepthComparators_009d8c30.cpp']):
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
runner=oracle.Runner(compose);input_file=w/'input.bin';expected_file=w/'expected.bin';actual_file=w/'actual.bin'
cases=snapshots=0;digest=hashlib.sha256()
with input_file.open('wb') as inp,expected_file.open('wb') as out:
 inp.write(runner.u.mem_read(0x1390000,0x40000))
 for flags,mutation,arena,shader,layout in oracle.scenarios(compose):
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
   raise AssertionError((w,at,at//38032,at%38032))
  if not left:break
  offset+=len(left)
assert cases==576
assert snapshots==(19136 if compose else 19288)
report=dict(providers=[proof],cases=cases,snapshots=snapshots,compared_bytes=offset,trace_sha256=digest.hexdigest(),
            omission_checks=len(objects)-1,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),
            boundary='Native parent with actual texture assignment/cleanup/cache binding, draw-record erase and compiler inline vertex clearing. Shader, state realization/restoration, window, strings, COM and sorting remain boundaries. Sort responses deterministically reverse complete records to verify parent consumption; no CRT sorting or GPU frame claim. Full36KiB arena and988-byte shader snapshots. Callback mutations change device/vertex buffer pointers, viewport bits, shader flags and layout index.')

if compose:report['boundary']+=' This run additionally executes the actual library data and all five Apply methods; GFGetSystemManager returns the fixed test root. Only valid library slots1..5 are used.'
(ROOT/('rebuild/compile-gate/native-draw-buffered'+('-state-blocks'if compose else'')+'.json')).write_text(json.dumps(report,indent=2)+'\n')
for path in (input_file,expected_file,actual_file):path.unlink()
print(r.stdout.strip(),snapshots,'full-state snapshots',flush=True)

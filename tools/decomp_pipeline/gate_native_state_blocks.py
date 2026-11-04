"""Typed state-block Apply methods and actual initialized virtual library."""
import hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_native_state_blocks as oracle
ROOT=Path(__file__).resolve().parents[2]
SPECS=[['?Apply@CStateBlockFunctionSold@@UAEXXZ', 10350688, 1736], ['?Apply@CStateBlockFunctionAlphaAdditive@@UAEXXZ', 10352432, 1633], ['?Apply@CStateBlockFunctionAlphaMultiply@@UAEXXZ', 10354080, 1633], ['?Apply@CStateBlockFunctionAlphaBoolean@@UAEXXZ', 10355728, 1738], ['?Apply@CStateBlockFunctionAlphaText@@UAEXXZ', 10357472, 1635]]
TARGETS={'?GFGetSystemManager@@YIPAVCSystemManager@@XZ': 10112704, '?FableRenderDefaultValue_01396f6c@@3KA': 20541292, '?FableRenderDefaultValue_01396f68@@3KA': 20541288, '?FableRenderDefaultValue_01396e04@@3KA': 20540932, '?FableRenderDefaultValue_01396ef0@@3KA': 20541168, '?FableRenderDefaultValue_01396eec@@3KA': 20541164, '?FableRenderDefaultValue_01396e7c@@3KA': 20541052, '?FableRenderDefaultValue_01396e70@@3KA': 20541040, '?FableRenderDefaultValue_01396e5c@@3KA': 20541020, '?FableRenderDefaultValue_01396f78@@3KA': 20541304, '?FableRenderDefaultValue_01396f7c@@3KA': 20541308, '?FableRenderDefaultValue_01396de8@@3KA': 20540904, '?FableRenderDefaultValue_01396e58@@3KA': 20541016}
w=Path(tempfile.mkdtemp(prefix='native-state-blocks-'));env=V.env();objects=[]
paths=['rebuild/src/compiled/00/9d/CStateBlockFunctions_Apply_009df060.cpp','rebuild/src/compiled/01/39/CRenderManager2D_StateBlockLibrary_01396510.cpp','rebuild/tests/integration/NativeStateBlocks_test.cpp']
for i,path in enumerate(paths):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
c=Coff(objects[0]);reports=[check(objects[0],s,a,n,len(c.sections[c.symbols[s][0]][1]),TARGETS)for s,a,n in SPECS]
data=Coff(objects[1]);locations=dict(TARGETS);data_specs=[]
for label,name,address,vtable,entry in [('Sold','Sold',0x1396530,0x129b9a8,0x9df060),('Additive','AlphaAdditive',0x1396534,0x129b988,0x9df730),('Multiply','AlphaMultiply',0x1396538,0x129b990,0x9dfda0),('Boolean','AlphaBoolean',0x139653c,0x129b998,0x9e0410),('Text','AlphaText',0x1396540,0x129b9a0,0x9e0ae0)]:
 locations['_'+label]=address;locations['??_7CStateBlockFunction'+name+'@@6B@']=vtable;locations['?Apply@CStateBlockFunction'+name+'@@UAEXXZ']=entry
 data_specs.extend([('_'+label,address,4,1),('??_7CStateBlockFunction'+name+'@@6B@',vtable,4,1)])
data_specs.append(('?LibraryStateBlockFunctions@CRenderManager2D@@2PAPAVCStateBlockFunctionBase@@A',0x1396510,28,5))
raw=EXE.read_bytes();sections=pe_sections(raw);data_proof=[]
for symbol,address,size,count in data_specs:
 section,offset=data.symbols[symbol];body,relocs=data.sections[section];patched=bytearray(body[offset:offset+size]);checked=[]
 for at,(target,kind)in relocs.items():
  if offset<=at<offset+size:
   assert kind==6 and target in locations;struct.pack_into('<I',patched,at-offset,locations[target]+struct.unpack_from('<I',patched,at-offset)[0]);checked.append(target)
 assert len(checked)==count and patched==raw[va_to_off(sections,address):va_to_off(sections,address)+size],symbol
 data_proof.append(dict(symbol=symbol,address=hex(address),bytes=size,relocations=count))
assert not any(s.startswith('_$E')for s in data.symbols),'library must be statically initialized'
strings=bytearray();symbols=bytearray();globals_={name:value for name,value in TARGETS.items()if '@@3' in name}
for name,value in globals_.items():
 symbols.extend(struct.pack('<2IIhHBB',0,4+len(strings),value,-1,0,2,0));strings.extend(name.encode()+b'\0')
bindings=w/'globals.obj';bindings.write_bytes(struct.pack('<HHIIIHH',0x14c,0,0,20,len(globals_),0,0)+symbols+struct.pack('<I',4+len(strings))+strings)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/fixed','/out:'+str(exe),'kernel32.lib']
for omit in (0,1):
 r=subprocess.run(cmd+[str(o)for i,o in enumerate(objects)if i!=omit]+[str(bindings)],env=env,capture_output=True,text=True);assert r.returncode,omit
r=subprocess.run(cmd+list(map(str,objects))+[str(bindings)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
runner=oracle.Runner();inputs=w/'input.bin';expected=w/'expected.bin';actual=w/'actual.bin';cases=snapshots=0
with inputs.open('wb')as inp,expected.open('wb')as out:
 inp.write(runner.u.mem_read(0x1390000,0x10000))
 for mutation,reverse,arena in oracle.scenarios():
  inp.write(struct.pack('<2I',mutation,reverse));inp.write(arena)
  for event in runner.run(arena,mutation,reverse):out.write(event);snapshots+=1
  cases+=1
r=subprocess.run([str(exe),str(inputs),str(actual)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
offset=0
with expected.open('rb')as left,actual.open('rb')as right:
 while True:
  a,b=left.read(1024*1024),right.read(1024*1024)
  if a!=b:
   at=next((i for i,(x,y)in enumerate(zip(a,b))if x!=y),min(len(a),len(b)))+offset;raise AssertionError((w,at,at//(oracle.WIDTH+8),at%(oracle.WIDTH+8)))
  if not a:break
  offset+=len(a)
assert (cases,snapshots)==(480,9600)
report=dict(providers=reports,data=data_proof,cases=cases,snapshots=snapshots,compared_bytes=offset,omission_checks=2,boundary='Actual five virtual Apply methods reached through the native statically initialized library. Complete40KiB arena over two render managers, capture masks0/1/2/high/all, existing captures and dirty queues, repeated calls, forward/reverse order. GFGetSystemManager is the sole callback boundary and may change the selected render manager before return. Library entries, five instance vptrs and five vtable slots independently match relocated retail data.',source_sha256={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest()for p in paths})
(ROOT/'rebuild/compile-gate/native-state-blocks.json').write_text(json.dumps(report,indent=2)+'\n')
for p in (inputs,expected,actual):p.unlink()
print(r.stdout.strip(),snapshots,'full-state snapshots',flush=True)

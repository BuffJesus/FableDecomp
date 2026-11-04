"""Native engine/world-map argument forwarding and singleton dispatch."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from engine_world_map_forwarders_parity import SPECS,GLOBAL,parent,owner
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-world-map-forwarders-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/b2/CEngine_WorldMapForwarders_00b23db0.cpp','01/43/NEngineInternal_GPEngineWorldMap_01436e8c.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeWorldMapForwarders_test.cpp'
targets={GLOBAL:0x1436e8c};targets.update({owner(s):s[5]for s in SPECS});aliases={'_MapGlobal':GLOBAL}
for i,s in enumerate(SPECS):
    n=4 if i==0 else 16 if i in (3,6)else 12;aliases['@Entry%d@%d'%(i,n)]=parent(s);aliases[owner(s)]='@Boundary%d@%d'%(i,n)
(w/'world_map_forwarders_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")'for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(16*9)
for i,s in enumerate(SPECS):
    body=read_virtual_data(raw,s[2],11);assert body[:2]==b'\x8b\x0d' and body[6]==0xe9;assert struct.unpack_from('<I',body,2)[0]==targets[GLOBAL] and s[2]+11+struct.unpack_from('<i',body,7)[0]==s[5];code[16*i:16*i+11]=body
(w/'world_map_forwarders_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(str(b)for b in code)+'};\n')
objects=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],parent(s),s[2],11,2,targets)for s in SPECS]
assert coff_data(Coff(objects[1]),GLOBAL,4)==read_virtual_data(raw,0x1436e8c,4)==bytes(4)
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(2):
    r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
inputs=bytearray();cases=0;values=(0,1,0xffffffff,0x80000000,0x7fffffff,0x12345678,0x24000600,0x24000700)
for fill,kind,mode,owner_offset in itertools.product(range(256),range(9),range(2),(0,0x800)):
    a=values[fill%8];b=values[(fill//8)%8]
    if kind==1:a=0x24000600+(fill%2)*0x100
    if kind==2:a=(0,0x24000600,0x24000000+owner_offset)[fill%3]
    if kind==6:b=fill&1
    if kind==7:a=fill&1
    arena=bytes([fill])*4096;inputs.extend(struct.pack('<5I',kind,mode,owner_offset,a,b));inputs.extend(arena);cases+=1
(w/'input.bin').write_bytes(inputs)
for kind in ('retail','compiled'):
    r=subprocess.run([str(exe),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
actual=(w/'compiled.bin').read_bytes();expected=(w/'retail.bin').read_bytes();assert actual==expected,(w,'world-map forwarding');assert len(actual)==cases*2*(4096+20)
report=dict(cases=cases,snapshots=cases*2,omission_checks=2,providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Nine original11byte bodies natively relocated with audited global and tail-jump operands. Full4KiB arenas,receiver/arguments/global capture,all256fills,two owner targets,signed-long boundaries,null/alias world pointers and both booleans. Callbacks mutate receiver and replace global. World-map owner methods remain recorded boundaries;no object layout beyond dispatch asserted,no normalized bytes.')
(ROOT/'rebuild/compile-gate/native-world-map-forwarders.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_WORLD_MAP_FORWARDERS_GATE_PASS',cases*2,'snapshots')

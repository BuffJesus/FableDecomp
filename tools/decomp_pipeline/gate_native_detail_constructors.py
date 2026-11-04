"""Detail-cache and quadtree constructors with real destruction and audited vtables."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-detail-ctors-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled'/p for p in ('00/bd/CLocalDetailCacheMap_Constructors_00bdefc0.cpp','00/bd/CLocalDetailCacheMap_QuadLifetime_00bddfb0.cpp','00/bd/CLocalDetailCacheMap_CQuadTreeElement_RemoveObjectCollections_00bdda50.cpp','00/be/CEngineStaticMapFileBlockManager_ReleaseRequest_00be0400.cpp','00/be/STLStaticMapBlockTree_find_00be08e0.cpp','01/43/NEngineInternal_MapRenderers_01436ea8.cpp','00/9d/CMovableResource_Verify_009d5100.cpp','00/bd/CLocalDetailCacheMap_QuadRelocation_00bde0c0.cpp','00/bd/CEngineLandscapeMemoryManager_SetVirtualBlockMovableResource_00bd9ab0.cpp','00/bd/CLocalDetailCacheMap_Destructor_00bded60.cpp')];fixture=ROOT/'rebuild/tests/integration/NativeDetailConstructors_test.cpp'
quad='??0CQuadTreeElement@CLocalDetailCacheMap@@QAE@PAV1@JJJJ@Z';detail='??0CLocalDetailCacheMap@@QAE@PAVCEngineMap@@@Z';table='??_7CQuadTreeElement@CLocalDetailCacheMap@@6B@';qd='??1CQuadTreeElement@CLocalDetailCacheMap@@UAE@XZ';dd='??1CLocalDetailCacheMap@@QAE@XZ'
aliases={'@ConstructQuad@28':quad,'@ConstructDetail@12':detail,'@DestroyQuad@4':qd,'@DestroyDetail@4':dd,'_QuadTable':table,'?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z':'@FreeVirtual@12'}
(w/'detail_constructors_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();code=bytearray(194);code[:119]=read_virtual_data(raw,0xbddf30,119);code[128:]=read_virtual_data(raw,0xbdefc0,66)
assert code[8:10]==bytes.fromhex('c700') and struct.unpack_from('<I',code,10)[0]==0x12a7df0
assert code[160]==0xe8 and 0xbdefc0+37+struct.unpack_from('<i',code,161)[0]==0xbddf30
(w/'detail_constructors_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,code))+'};\n')
objects=[]
for i,p in enumerate(sources+[fixture]):
 o=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],detail,0xbdefc0,66,1,{quad:0xbddf30}),check(objects[0],quad,0xbddf30,119,1,{table:0x12a7df0})]
table_targets={'??_ECQuadTreeElement@CLocalDetailCacheMap@@UAEPAXI@Z':0xbdfa10,'?RelocateData@CQuadTreeElement@CLocalDetailCacheMap@@UAEXJPAX0J@Z':0xbde0c0,'?CanRelocateData@CQuadTreeElement@CLocalDetailCacheMap@@UBE_NJ@Z':0xbddd10,'?Verify@CMovableResource@@UBEKXZ':0x9d5100}
for idx in (0,1):
 c=Coff(objects[idx]);sec,off=c.symbols[table];body,rel=c.sections[sec];assert off==0 and body==bytes(16) and set(rel)=={0,4,8,12};patched=bytearray(body)
 for at,(target,kind) in rel.items():assert kind==6;struct.pack_into('<I',patched,at,table_targets[target])
 assert patched==read_virtual_data(raw,0x12a7df0,16)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(len(sources)):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('unexpected omission link',sources[omit])
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for fill,flag,kind,parent,xy,wh in itertools.product((0,0x95),(0,0xf,0x80,0xff),(0,1),(0,0x24000600),((0,0),(-1,65536),(0x7fffffff,-0x80000000)),((0,0),(1,33),(65535,65536),(-1,-2))):
  if not kind and parent:continue
  a=bytearray([fill])*2048;a[0x48]=a[0x54]=flag;struct.pack_into('<2H',a,0x45c,wh[0]&65535,wh[1]&65535)
  f.write(struct.pack('<6I',kind,parent,*(v&0xffffffff for v in xy+wh)));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'constructor mismatch')
  if not x:break
snapshots=(w/'compiled.bin').stat().st_size//2056;assert snapshots==cases*2
headers=[ROOT/'rebuild/include'/p for p in ('engine/CLocalDetailCacheMap.h','engine/CEngineMap.h','fable_bounding_volumes.h')]
report=dict(cases=cases,snapshots=snapshots,omission_checks=len(sources),providers=proofs,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Original66/119byte constructors/twoaudited fixups versus compiled providers. Complete emitted quad vtables from constructor and lifetime providers audited against retail;native vptr targets real compiled table without snapshot normalization. Full2KiB snapshots before/after actual compiled detail/quad destruction. Zero/poison fills,flag0/0f/80/ff,parentnull/live,signed coordinate/extent extremes;upper flag bits preserved. Real ownership cleanup,relocation,verification,renderer singleton dependencies link;FreeVirtual must remain uncalled because initialized objects own no children or allocations. Existing destructor gates independently cover native destruction.')
(ROOT/'rebuild/compile-gate/native-detail-constructors.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DETAIL_CONSTRUCTORS_GATE_PASS',snapshots,'snapshots')

import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-manager-pool-usage-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a6/CTextureManager_PoolUsage_00a69f10.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/texture_manager_pool_usage_candidate1_20261009.cpp'
age=ROOT/'rebuild/src/compiled/00/9f/CResource_GetUnusedFrames_009fbf70.cpp'
pool=ROOT/'rebuild/src/compiled/00/a6/CPreallocTexturePool_Usage_00a69ea0.cpp'
specs=[(source,'?GetPoolTexturesAllocated@CTextureManager@@QAEKK@Z',0xa69f10,53,0,0),(source,'?GetPoolTexturesInUse@CTextureManager@@QAEKKK@Z',0xa69f50,24,64,1),(pool,'?GetTexturesInUse@CPreallocTexturePool@@QAEKK@Z',0xa69ed0,61,128,1),(age,'?GetUnusedFrames@CResource@@QBEKXZ',0x9fbf70,18,224,0)]
objects={};proofs=[];body=bytearray(256);callSites=[]
for p,symbol,address,length,offset,relocs in specs:
 if p not in objects:
  obj=w/(p.stem+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects[p]=obj
 proofs.append(check(objects[p],symbol,address,length,relocs,{'?GetUnusedFrames@CResource@@QBEKXZ':0x9fbf70,'?GetTexturesInUse@CPreallocTexturePool@@QAEKK@Z':0xa69ed0}));b=read_virtual_data(EXE.read_bytes(),address,length);body[offset:offset+length]=b
 for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(b,address):
  if i.mnemonic=='call':assert int(i.op_str,16) in (0xa69ed0,0x9fbf70);callSites.append(offset+i.address-address+1)
assert len(callSites)==2
(w/'manager_pool_usage_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned callSites[]={'+','.join(map(str,callSites))+'};\n')
(w/'manager_pool_usage_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in [('@AllocatedRoot@12',specs[0][1]),('@InUseRoot@16',specs[1][1])])+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeTextureManagerPoolUsage_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];objs=list(objects.values())
for omitted in range(3):
 r=subprocess.run(cmd+[str(o) for j,o in enumerate(objs) if j!=omitted],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objs)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=786432 snapshots=3145728' in r.stdout,r.stdout
headers=[ROOT/'rebuild/include'/p for p in ['engine/CTextureManager.h','engine/CPreallocTexturePool.h','fable_managed_texture_data.h','fable_resource_list.h','fable_iv_counted_pointer.h','fable_carray.h']]
report=dict(cases=786432,snapshots=3145728,providers=proofs,omission_checks=3,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in list(objects)+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Manager pool index wrappers at indices0/1/2/15 with84B stride, actual pool InUse and resource age, three required sources. Two manager roots plus actual18B resource unused-frame provider; original/model/compiled read-only full16KiB comparisons, counts0..5/rawAllocated0,1,80,ff/null and live resource lists/unsigned frame-age subtraction and threshold endpoints/two placements-fills. Empty count preserves invalid array pointer without reading it. All three source omissions fail. Only two manager roots add coverage; existing resource-age helper not recounted.')
(ROOT/'rebuild/compile-gate/native-texture-manager-pool-usage.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_TEXTURE_MANAGER_POOL_USAGE_GATE_PASS')

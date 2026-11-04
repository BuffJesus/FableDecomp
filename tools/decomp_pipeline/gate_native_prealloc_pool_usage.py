import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-prealloc-pool-usage-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a6/CPreallocTexturePool_Usage_00a69ea0.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/prealloc_pool_usage_candidate1_20261009.cpp'
age=ROOT/'rebuild/src/compiled/00/9f/CResource_GetUnusedFrames_009fbf70.cpp'
specs=[(source,'?GetTexturesAllocated@CPreallocTexturePool@@QAEKXZ',0xa69ea0,35,0,0),(source,'?GetTexturesInUse@CPreallocTexturePool@@QAEKK@Z',0xa69ed0,61,64,1),(age,'?GetUnusedFrames@CResource@@QBEKXZ',0x9fbf70,18,128,0)]
objects={};proofs=[];body=bytearray(160);callSite=None
for p,symbol,address,length,offset,relocs in specs:
 if p not in objects:
  obj=w/(p.stem+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects[p]=obj
 proofs.append(check(objects[p],symbol,address,length,relocs,{'?GetUnusedFrames@CResource@@QBEKXZ':0x9fbf70}));b=read_virtual_data(EXE.read_bytes(),address,length);body[offset:offset+length]=b
 for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(b,address):
  if i.mnemonic=='call':assert int(i.op_str,16)==0x9fbf70;callSite=offset+i.address-address+1
assert callSite is not None
(w/'pool_usage_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned callSite='+str(callSite)+';\n')
(w/'pool_usage_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in [('@AllocatedRoot@8',specs[0][1]),('@InUseRoot@12',specs[1][1])])+'\n')
fixture=ROOT/'rebuild/tests/integration/NativePreallocPoolUsage_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];objs=list(objects.values())
for omitted in range(2):
 r=subprocess.run(cmd+[str(o) for j,o in enumerate(objs) if j!=omitted],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objs)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=196608 snapshots=786432' in r.stdout,r.stdout
headers=[ROOT/'rebuild/include'/p for p in ['engine/CPreallocTexturePool.h','fable_managed_texture_data.h','fable_resource_list.h','fable_iv_counted_pointer.h','fable_carray.h']]
report=dict(cases=196608,snapshots=786432,providers=proofs,omission_checks=2,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in list(objects)+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Two pool count roots plus actual18B resource unused-frame provider; original/model/compiled read-only full16KiB comparisons, counts0..5/rawAllocated0,1,80,ff/null and live resource lists/unsigned frame-age subtraction and threshold endpoints/two placements-fills. Empty count preserves invalid array pointer without reading it. Both source omissions fail. Only two pool roots add coverage; existing resource-age helper not recounted.')
(ROOT/'rebuild/compile-gate/native-prealloc-pool-usage.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_PREALLOC_POOL_USAGE_GATE_PASS')

import hashlib,json,subprocess,tempfile,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-subprimitive-walkers-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/b8'/p for p in ('CEngineSubPrimitiveRenderer_RenderList_00b842d0.cpp','CEngineSubPrimitiveRenderer_RenderListTwoPass_00b84320.cpp','CEngineSubPrimitiveRenderer_DeleteList_00b84400.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveWalkers_test.cpp'
one='?RenderList@CEngineSubPrimitiveRenderer@@AAEXABV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@W4ERenderPassType@@@Z'
two='?RenderListTwoPass@CEngineSubPrimitiveRenderer@@AAEXABV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@W4ERenderPassType@@1@Z'
delete='?DeleteList@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@@Z'
aliases={'@One@16':one,'@Two@20':two,'@Delete@12':delete}
(w/'subprimitive_walkers_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(289)
specs=[(one,0xb842d0,69,0),(two,0xb84320,100,80),(delete,0xb84400,97,192)]
for sym,a,l,offset in specs:body[offset:offset+l]=read_virtual_data(raw,a,l)
(w/'subprimitive_walkers_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')

objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,a,l,0,{}) for n,(sym,a,l,offset) in enumerate(specs)]
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip())
assert 'cases=4608 ' in r.stdout
import re
snapshots=int(re.search(r'snapshots=(\d+)',r.stdout)[1])
report=dict(cases=4608,snapshots=snapshots,providers=proofs,omission_checks=3,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Actual one-pass/two-pass list walkers and deletion; independent full8KiB expectations cover0..3entries,1..3linked nodes,prepass flag combinations,pass values,list replacement and truncation,live next-link changes,manager/flag changes,and destruction scribbling after cached-next load. Only manager Draw and virtual deleting destructor boundaries; no rendered frame.')
(ROOT/'rebuild/compile-gate/native-subprimitive-walkers.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_SUBPRIMITIVE_WALKERS_GATE_PASS')

import hashlib,json,subprocess,tempfile,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-subprimitive-lists-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/b8'/p for p in ('CEngineSubPrimitiveRenderer_GetCount_00b842b0.cpp','CEngineSubPrimitiveRenderer_ClearType_00b847b0.cpp')]
fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveLists_test.cpp'
count='?GetCount@CEngineSubPrimitiveRenderer@@QBEKW4ESubPrimitiveType@@@Z'
clear='?ClearType@CEngineSubPrimitiveRenderer@@QAEXW4ESubPrimitiveType@@@Z'
delete='?DeleteList@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@@Z'
aliases={'@GetCount@12':count,'@ClearType@12':clear,delete:'@DeleteList@12'}
(w/'subprimitive_lists_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(52);body[:25]=read_virtual_data(raw,0xb842b0,25);body[32:]=read_virtual_data(raw,0xb847b0,20)
assert body[47]==0xe9
(w/'subprimitive_lists_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixup=48;\n')
objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[n],sym,a,l,c,{delete:0xb84400}) for n,(sym,a,l,c) in enumerate(((count,0xb842b0,25,0),(clear,0xb847b0,20,1)))]
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip())
assert 'cases=4080 snapshots=32640' in r.stdout
report=dict(cases=4080,snapshots=32640,providers=proofs,omission_checks=2,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Actual GetCount and ClearType; independent full4KiB expectations for all15types,0..16elements,null empty array,arbitrary flags,mutating DeleteList callback and count after clearing. Actual deletion is a boundary. No drawing.')
(ROOT/'rebuild/compile-gate/native-subprimitive-lists.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_SUBPRIMITIVE_LISTS_GATE_PASS')

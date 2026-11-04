import hashlib,json,subprocess,tempfile,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-subprimitive-radix-'));env=V.env()
sources=[ROOT/'rebuild/src/compiled/00/b8/CEngineSubPrimitiveRenderer_SortSingleRadix_00b84b20.cpp']
fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveRadix_test.cpp'
root='?SortSingleRadix@CEngineSubPrimitiveRenderer@@AAEXPBVCSortElement@1@PAV21@KK@Z'
(w/'subprimitive_radix_aliases.h').write_text('#pragma comment(linker,"/alternatename:@Radix@24='+root+'")\n')
raw=EXE.read_bytes();body=read_virtual_data(raw,0xb84b20,232)
assert body[11]==0xe8
(w/'subprimitive_radix_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixup=12;\n')

objects=[]
for n,p in enumerate(sources+[fixture]):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proofs=[check(objects[0],root,0xb84b20,232,1,{'__chkstk':0xbfea30})]
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(1):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip())
assert 'cases=1664 snapshots=4992' in r.stdout
report=dict(cases=1664,snapshots=4992,providers=proofs,omission_checks=1,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Actual single radix pass;independent stable insertion reference;1664cases/4992full64KiB snapshots for counts0..1025,8key patterns,3digits,and three-pass composition with low30bit ordering/stability checks. Original payload bits retained;owner/input/neighbour memory verified. Only CRT stack probing;no drawing.')
(ROOT/'rebuild/compile-gate/native-subprimitive-radix.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_SUBPRIMITIVE_RADIX_GATE_PASS')

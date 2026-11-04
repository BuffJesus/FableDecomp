"""Native scenery-type copy and comparison, including serialized padding."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-detail-copy-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/be/CLocalDetailObjectCollectionType_CopyCompare_00be2680.cpp';fixture=ROOT/'rebuild/tests/integration/NativeDetailCopyCompare_test.cpp'
specs=[('?IsEquivalent@CLocalDetailObjectCollectionType@@QBE_NABV1@@Z',0xbe2680,62,0),('??0CLocalDetailObjectCollectionType@@QAE@ABV0@@Z',0xbe26c0,83,2)]
table='??_7CLocalDetailObjectCollectionType@@6B@';targets={'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,table:0x12a7d94}
aliases={'@Equivalent@12':specs[0][0],'@Copy@12':specs[1][0],'_TypeTable':table,'??0CBaseClassNonCopyable@@QAE@XZ':'@BaseCtor@4','??1CBaseClassNonCopyable@@UAE@XZ':'@BaseDtor@4'}
(w/'detail_copy_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
raw=EXE.read_bytes();body=bytearray(147);body[:62]=read_virtual_data(raw,0xbe2680,62);body[64:]=read_virtual_data(raw,0xbe26c0,83)
assert struct.unpack_from('<i',body,70)[0]+0xbe26c0+10==0x99a2f0 and struct.unpack_from('<I',body,80)[0]==0x12a7d94
(w/'detail_copy_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned copyCallFixup=70,copyTableFixup=80;\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=[check(objects[0],s,a,n,r,targets) for s,a,n,r in specs]
c=Coff(objects[0]);sec,off=c.symbols[table];b,rel=c.sections[sec];assert off==0 and len(b)==4 and rel=={0:('??_ECLocalDetailObjectCollectionType@@UAEPAXI@Z',6)}
scalar=struct.unpack('<I',read_virtual_data(raw,0x12a7d94,4))[0];scalar_proof=check(objects[0],'??_GCLocalDetailObjectCollectionType@@UAEPAXI@Z',scalar,30,2,{'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??3@YAXPAX@Z':0xbfe9bc})
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0
with (w/'input.bin').open('wb') as f:
 for fill,mode,different in itertools.product((0,0x95),range(4),range(-1,84)):
  a=bytearray([fill])*2048
  if different>=0:a[0x100+different]^=0x55
  f.write(struct.pack('<I',mode));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
assert (w/'retail.bin').read_bytes()==(w/'compiled.bin').read_bytes()
size=(w/'compiled.bin').stat().st_size;assert size%2056==0;snapshots=size//2056
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,providers=proof,scalar=scalar_proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Two original spans/two fixups; actual one-slot vtable and scalar checked. Every byte of84byte object varied independently, including ignored fields and compared52byte block padding; self comparison; copy then compare; base-constructor callback changes source or destination. Full2KiB snapshots and constructor return pointer. Base ctor remains boundary; scalar deletion not executed.')
(ROOT/'rebuild/compile-gate/native-detail-copy-compare.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_DETAIL_COPY_COMPARE_GATE_PASS',snapshots,'snapshots')

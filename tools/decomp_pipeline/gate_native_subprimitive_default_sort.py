import hashlib,json,subprocess,tempfile,sys,struct
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-subprimitive-default-sort-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b8/SubPrimitiveSort_DefaultSort_00b868c0.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveDefaultSort_test.cpp'
root='??$sort@PAVCSortElement@CEngineSubPrimitiveRenderer@@@_STL@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@0@Z'
# Recover compiler symbols directly; each is checked against a fixed native call target.
objects=[]
o=w/'0.obj';r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
from gate_game_component_abi import Coff
coff=Coff(o);sec,off=coff.symbols[root]
targets={s:0xb867a0 if '__introsort_loop@' in s else 0xb85b30 if '__insertion_sort@' in s else 0xb851d0 for s,k in coff.sections[sec][1].values()}
body=read_virtual_data(EXE.read_bytes(),0xb868c0,120)
import capstone
fixups=[]
for ins in capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(body,0xb868c0):
    if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
        target=ins.address+5+struct.unpack_from('<i',ins.bytes,1)[0]
        assert target in targets.values(),hex(target)
        index={0xb867a0:0,0xb85b30:1,0xb851d0:2}[target]
        fixups.append((ins.address-0xb868c0+1,index))
assert len(fixups)==4,fixups
(w/'subprimitive_default_sort_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate([source,fixture]):
    o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],root,0xb868c0,120,4,targets)
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
print(r.stdout.strip());assert 'cases=384 snapshots=2624' in r.stdout
report=dict(cases=384,snapshots=2624,providers=[proof],omission_checks=1,source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Actual default sort coordinator;384cases/2624full64KiB snapshots,counts0..1025,threshold15..17,depth powers,two array locations,callback mutation. Introsort and insertion helpers remain boundaries; no concrete sorting or drawing claim.')
(ROOT/'rebuild/compile-gate/native-subprimitive-default-sort.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_SUBPRIMITIVE_DEFAULT_SORT_GATE_PASS')


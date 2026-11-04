import hashlib,json,subprocess,tempfile,sys,struct
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-subprimitive-sort-list-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b8/CEngineSubPrimitiveRenderer_SortList_00b84990.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveSortList_test.cpp'
root='?SortList@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@_N@Z'
targets={
'??$__stable_sort@PAVCSortElement@CEngineSubPrimitiveRenderer@@V12@HU?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@_STL@@@_STL@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@00PAHU?$less@VCSortElement@CEngineSubPrimitiveRenderer@@@0@@Z':0xb86710,
'??$sort@PAVCSortElement@CEngineSubPrimitiveRenderer@@@_STL@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@0@Z':0xb868c0,
'?RadixSort@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@@Z':0xb847d0}
aliases={'@SortList@16':root}
for symbol,address in targets.items():
    aliases[symbol]={0xb86710:'@Stable@20',0xb868c0:'@Ordinary@8',0xb847d0:'@Radix@12'}[address]
(w/'subprimitive_sort_list_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xb84990,82)
import capstone
fixups=[]
for ins in capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(body,0xb84990):
    if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
        target=ins.address+5+struct.unpack_from('<i',ins.bytes,1)[0]
        assert target in targets.values(),hex(target)
        index={0xb86710:0,0xb868c0:1,0xb847d0:2}[target]
        fixups.append((ins.address-0xb84990+1,index))
assert len(fixups)==3,fixups
(w/'subprimitive_sort_list_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate([source,fixture]):
    o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],root,0xb84990,82,3,targets)
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
print(r.stdout.strip());assert 'cases=768 snapshots=1536' in r.stdout
report=dict(cases=768,snapshots=1536,providers=[proof],omission_checks=1,source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Actual sorting dispatcher; retail and independent route/state model, 768cases/1536full64KiB snapshots. Empty arrays, stable raw bool bytes, threshold254..257, counts0..1025,two array locations. Stable/ordinary/radix sorting remain explicit callbacks; no drawing.')
(ROOT/'rebuild/compile-gate/native-subprimitive-sort-list.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_SUBPRIMITIVE_SORT_LIST_GATE_PASS')


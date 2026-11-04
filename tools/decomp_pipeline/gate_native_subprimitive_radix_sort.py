import hashlib,json,subprocess,tempfile,sys,struct
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-subprimitive-radix-sort-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b8/CEngineSubPrimitiveRenderer_RadixSort_00b847d0.cpp'
fixture=ROOT/'rebuild/tests/integration/NativeSubPrimitiveRadixSort_test.cpp'
root='?RadixSort@CEngineSubPrimitiveRenderer@@AAEXAAV?$CArray@VCSortElement@CEngineSubPrimitiveRenderer@@@@@Z'
targets={'__chkstk': 12577328, '?erase@?$vector@VCSortElement@CEngineSubPrimitiveRenderer@@V?$allocator@VCSortElement@CEngineSubPrimitiveRenderer@@@_STL@@@_STL@@QAEPAVCSortElement@CEngineSubPrimitiveRenderer@@PAV34@0@Z': 12081888, '?_M_fill_insert@?$vector@VCSortElement@CEngineSubPrimitiveRenderer@@V?$allocator@VCSortElement@CEngineSubPrimitiveRenderer@@@_STL@@@_STL@@QAEXPAVCSortElement@CEngineSubPrimitiveRenderer@@IABV34@@Z': 12083152}
aliases={'@RadixSort@12':root}
for symbol,address in targets.items():
    if address==0xb85ae0: aliases[symbol]='@Erase@16'
    elif address==0xb85fd0: aliases[symbol]='@Fill@20'
(w/'subprimitive_radix_sort_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xb847d0,444)
import capstone
fixups=[]
for ins in capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(body,0xb847d0):
    if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
        target=ins.address+5+struct.unpack_from('<i',ins.bytes,1)[0]
        assert target in targets.values(),hex(target)
        index=0 if target==targets['__chkstk'] else 1 if target==0xb85ae0 else 2
        fixups.append((ins.address-0xb847d0+1,index))
assert len(fixups)==3,fixups
(w/'subprimitive_radix_sort_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate([source,fixture]):
    o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],root,0xb847d0,444,3,targets)
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
print(r.stdout.strip());assert 'cases=768 snapshots=1536' in r.stdout
report=dict(cases=768,snapshots=1536,providers=[proof],omission_checks=1,source_sha256={str(source.relative_to(ROOT)):hashlib.sha256(source.read_bytes()).hexdigest()},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),result=r.stdout.strip(),boundary='Actual three-pass radix sorting compared with original native bytes and independent stable insertion model; full64KiB states, counts0..1025,8patterns,shrink/equal/grow and replacement buffer. Vector erase/fill insertion and CRT stack probe remain explicit boundaries; no drawing.')
(ROOT/'rebuild/compile-gate/native-subprimitive-radix-sort.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_SUBPRIMITIVE_RADIX_SORT_GATE_PASS')


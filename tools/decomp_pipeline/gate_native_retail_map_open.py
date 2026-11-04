"""Native retail per-map opening and inline ownership comparison."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-retail-map-open-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b4/CEngineMap_OpenRetailStaticMap_00b41e50.cpp';fixture=ROOT/'rebuild/tests/integration/NativeRetailMapOpen_test.cpp'
symbol='?OpenRetailStaticMap@CEngineMap@@QAEXAAVCStaticMapBankFile@@AAVCDataInputStream@@AAV?$map@VCCharString@@JU?$less@VCCharString@@@_STL@@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@@Z'
targets={'?CloseStaticMap@CEngineMap@@QAEXXZ': 11792192, '?ReadMapInfoBlock@CEngineMap@@CIXAAUCStaticMapInfoBlock@1@AAVCDataInputStream@@@Z': 11792288, '?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z': 10311888, '??3@YAXPAX@Z': 12577212, '?SetBankFileEntry@CEngineStaticMapFileBlockManager@@QAEXV?$CCountedPointer@VCBankFileAsyncEntry@@@@@Z': 12452768, '?OpenStaticMap@CEngineLandscapeMap@@QAEXPAVCEngineStaticMapFileBlockManager@@AAVCDataInputStream@@@Z': 12439776, '?OpenStaticMap@CLocalDetailCacheMap@@QAEXPAVCEngineStaticMapFileBlockManager@@AAVCDataInputStream@@@Z': 12447760, '?_M_find@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@QAEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z': 4371559}
aliases={'@OpenRetail@20': '?OpenRetailStaticMap@CEngineMap@@QAEXAAVCStaticMapBankFile@@AAVCDataInputStream@@AAV?$map@VCCharString@@JU?$less@VCCharString@@@_STL@@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@@Z', '_Delete': '??3@YAXPAX@Z', '?CloseStaticMap@CEngineMap@@QAEXXZ': '@Close@4', '?ReadMapInfoBlock@CEngineMap@@CIXAAUCStaticMapInfoBlock@1@AAVCDataInputStream@@@Z': '@Parse@8', '?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z': '@Async@16', '?SetBankFileEntry@CEngineStaticMapFileBlockManager@@QAEXV?$CCountedPointer@VCBankFileAsyncEntry@@@@@Z': '@SetEntry@16', '?OpenStaticMap@CEngineLandscapeMap@@QAEXPAVCEngineStaticMapFileBlockManager@@AAVCDataInputStream@@@Z': '@Land@16', '?OpenStaticMap@CLocalDetailCacheMap@@QAEXPAVCEngineStaticMapFileBlockManager@@AAVCDataInputStream@@@Z': '@Detail@16', '?_M_find@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@QAEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z': '@Find@12'}
(w/'retail_map_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xb41e50,325);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body,0xb41e50):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm in targets.values();fixups.append((i.address-0xb41e50+1,i.operands[0].imm))
assert len(fixups)==9
(w/'retail_map_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb41e50,325,9,targets)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,old,new,refs,detail,index in itertools.product((0,0x95),range(7),(0,1),(0,1,2),(1,2),(0,1),(0,3,0xffffffff)):
  a=bytearray([fill])*4096
  struct.pack_into('<2I',a,0,base+0x700,base+0x740 if detail else 0)
  struct.pack_into('<2I',a,0x3c,base+0x600 if old else 0,base+0x840 if old else 0)
  struct.pack_into('<2I',a,0x800,base+0x620,0 if new==0 else base+0x840 if new==1 else base+0x860)
  for off in (0x840,0x860,0x880):struct.pack_into('<I',a,off,refs)
  struct.pack_into('<I',a,0xa14,16);struct.pack_into('<I',a,0xb04,index)
  struct.pack_into('<2I',a,0xb10,128,160);struct.pack_into('<I',a,0xb58,192)
  f.write(struct.pack('<I',mode));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual325byte retail map opener/nine audited fixups with real inline counted ownership. Full4KiB snapshots; old/new/null/shared ownership, release callback mutation, optional detail and child callbacks changing detail presence, map-name position changed during Close, arbitrary bank indices. Tree find,Close,header parser,GetAsyncEntry,virtual positioning and child opens remain callback boundaries. SetEntry boundary consumes its by-value counted pointer through the real shared destructor.')
(ROOT/'rebuild/compile-gate/native-retail-map-open.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_RETAIL_MAP_OPEN_GATE_PASS',snapshots,'snapshots')

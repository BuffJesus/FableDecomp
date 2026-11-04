"""Shared retail map header loading with original code and full-state callback traces."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-common-map-header-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b4/CEngineWorldMap_OpenStaticMapsUsingCommonRetailHeader_00b420f0.cpp';fixture=ROOT/'rebuild/tests/integration/NativeCommonMapHeader_test.cpp'
symbol='?OpenStaticMapsUsingCommonRetailHeader@CEngineWorldMap@@AAEXJ@Z'
targets={'?GetEntryDataSize@CBankFile@@QBEKK@Z': 10273344, '?ReadEntryData@CBankFile@@QAEXKKPAXK@Z': 10273440, '??0CMemoryDataInputStream@@QAE@PBXK@Z': 10045744, '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z': 10042528, '?ReadNullTerminatedString@CMemoryDataInputStream@@UAE?AVCCharString@@XZ': 10050864, '??0CCharString@@QAE@ABV0@@Z': 10087472, '??1CCharString@@QAE@XZ': 10087136, '??1CMemoryDataInputStream@@UAE@XZ': 10042128, '?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@J@_STL@@@_STL@@_N@2@ABU?$pair@$$CBVCCharString@@J@2@@Z': 4396550, '?OpenRetailStaticMap@CEngineMap@@QAEXAAVCStaticMapBankFile@@AAVCDataInputStream@@AAV?$map@VCCharString@@JU?$less@VCCharString@@@_STL@@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@@Z': 11804240, '?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@J@_STL@@@2@@Z': 4391434, '_CommonAlloc': 12577294, '_CommonFree': 12577300}
aliases={'@OpenCommon@12': '?OpenStaticMapsUsingCommonRetailHeader@CEngineWorldMap@@AAEXJ@Z', '?GetEntryDataSize@CBankFile@@QBEKK@Z': '@Size@12', '?ReadEntryData@CBankFile@@QAEXKKPAXK@Z': '@ReadBank@24', '??0CMemoryDataInputStream@@QAE@PBXK@Z': '@Stream@16', '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z': '@Overflow@16', '?ReadNullTerminatedString@CMemoryDataInputStream@@UAE?AVCCharString@@XZ': '@ReadName@12', '??0CCharString@@QAE@ABV0@@Z': '@Copy@12', '??1CCharString@@QAE@XZ': '@Destroy@4', '??1CMemoryDataInputStream@@UAE@XZ': '@StreamDestroy@4', '?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@J@_STL@@@_STL@@_N@2@ABU?$pair@$$CBVCCharString@@J@2@@Z': '@Insert@16', '?OpenRetailStaticMap@CEngineMap@@QAEXAAVCStaticMapBankFile@@AAVCDataInputStream@@AAV?$map@VCCharString@@JU?$less@VCCharString@@@_STL@@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@@Z': '@OpenMap@20', '?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@_STL@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@J@_STL@@@2@@Z': '@Erase@12'}
(w/'common_map_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0xb420f0,662);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body,0xb420f0):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm in targets.values();fixups.append((i.address-0xb420f0+1,i.operands[0].imm,0))
assert len(fixups)==19
(w/'common_map_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%(f[0],f[1]) for f in fixups)+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT']+(['/Dfree=CommonFree','/Dmalloc=CommonAlloc'] if n==0 else [])+['/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb420f0,662,19,targets)
original=w/'unaliased.obj'
r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(original),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
original_targets=dict(targets);original_targets['_free']=original_targets.pop('_CommonFree');original_targets['_malloc']=original_targets.pop('_CommonAlloc')
assert check(original,symbol,0xb420f0,662,19,original_targets)==proof
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,size,count,maps,mask,remaining in itertools.product((0,0x95),range(7),(0,64),(-1,0,1,3),(0,1,3),(0,2,7),(0,64)):
  a=bytearray([fill])*4096;struct.pack_into('<3I',a,0x20,base+0x400,base+0x400+4*maps,base+0x40c)
  for i in range(3):struct.pack_into('<I',a,0x400+i*4,0 if mask&(1<<i) else base+0x700+32*i)
  struct.pack_into('<I',a,0xa00,count&0xffffffff)
  for i in range(3):struct.pack_into('<2I',a,0xa04+8*i,0,16+8*i)
  f.write(struct.pack('<3I',mode,size,remaining));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%4108==0;snapshots=size//4108
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual662byte shared-header parent/19audited fixups. Full4KiB snapshots. Empty/nonempty byte buffers, negative/zero/one/three records, null/non-null maps, mutable vector bounds, tree insert rejection, string-copy ownership and teardown. Bank reads, stream lifecycle/string reads, tree insert/erase and per-map retail opening remain explicit recording boundaries. Only production TU allocator names are aliased; both aliased and original objects pass parity. Stack-local callback pointers normalized, no snapshot masking.')
(ROOT/'rebuild/compile-gate/native-common-map-header.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_COMMON_MAP_HEADER_GATE_PASS',snapshots,'snapshots')

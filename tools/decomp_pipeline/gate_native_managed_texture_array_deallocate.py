import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-managed-array-deallocate-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/a6/CArray_ManagedTextureData_DeallocateMemory_00a6be40.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/managed_texture_array_deallocate_candidate2_20261009.cpp'
obj=w/'deallocate.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(obj)
root=[s for s in c.symbols if s.startswith('?DeallocateMemory@')];copy=[s for s in c.symbols if '__copy@' in s];destroy=[s for s in c.symbols if '__destroy_aux' in s];assert len(root)==len(copy)==len(destroy)==1
specs=[(root[0],0xa6be40,111,0,4),(copy[0],0xa6b080,82,128,0),(destroy[0],0xa6b700,50,256,0)];targets={root[0]:0xa6be40,copy[0]:0xa6b080,destroy[0]:0xa6b700,'_free':0xbfea14};offsets={0xa6b080:128,0xa6b700:256,0xbfea14:0xffffffff};proofs=[];body=bytearray(320);fixups=[]
for symbol,address,length,offset,relocs in specs:
 proofs.append(check(obj,symbol,address,length,relocs,targets));b=read_virtual_data(EXE.read_bytes(),address,length);body[offset:offset+length]=b
 for i in Cs(CS_ARCH_X86,CS_MODE_32).disasm(b,address):
  if i.mnemonic=='call' and i.bytes[0]==0xe8:target=int(i.op_str,16);assert target in offsets;fixups.append((offset+i.address-address+1,offsets[target]))
assert len(fixups)==4
(w/'managed_array_deallocate_retail.h').write_text('struct Fixup{unsigned site,target;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u}'%f for f in fixups)+'};\n')
(w/'managed_array_deallocate_aliases.h').write_text('#pragma comment(linker,"/alternatename:@ArrayDeallocate@8='+root[0]+'")\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedTextureArrayDeallocate_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=9216 snapshots=' in r.stdout,r.stdout
headers=[ROOT/'rebuild/include'/p for p in ['engine/CPreallocTexturePool.h','fable_managed_texture_data.h','fable_stlport_vector.h','fable_stlport_algobase.h','fable_iv_counted_pointer.h','fable_resource_list.h','fable_carray.h']]
report=dict(cases=9216,snapshots=int(r.stdout.split('snapshots=')[1]),providers=proofs,omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Arraystorage release112B/111B near93.9759percent candidate2;actual emitted82B class-elementcopy and50B destructorhelpers bothfullMATCH, uncounte dsupportingproviders. Original/model/compiled full16KiB callback/final snapshots,capacity0/1/4,length0/1/3,NULL storage/null elements/shared-distinctowners/rawrefcounts,unsigned wrap,virtualdeleter mutationsfinish/capacity/current-nextslots/nextrefcount,freecallback fieldmutations,two placements-fills. Capacityzero skipsclear/freeevennonnullstorage;clear viaemptycopy thenactualdestructorloop,Finishcachedstart,swapzerosbeforesecondactualdestructorpass/free. Copyhelper executesemptyrangebranch here; fullnonempty copy behavior not claimed byrootgate. Only owner virtualdeletion/free remainfunctionalboundaries; no GPU/realCRT heapclaim. One actualsource withthree bodyproofs andomission; only root addsnearcoverage.')
(ROOT/'rebuild/compile-gate/native-managed-texture-array-deallocate.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_TEXTURE_ARRAY_DEALLOCATE_GATE_PASS')

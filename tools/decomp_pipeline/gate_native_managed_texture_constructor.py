"""Actual VC7.1 constructor/scalar bodies against original bytes and full-state model."""
import hashlib,json,struct,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-managed-texture-constructor-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/a6/CManagedTextureData_Constructor_00a69980.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/managed_texture_data_constructor_candidate_20261009.cpp'
obj=w/'constructor.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
ctor='??0CManagedTextureData@@QAE@XZ';scalar='??_GCManagedTextureData@@UAEPAXI@Z';table='??_7CManagedTextureData@@6B@'
targets={'??0CResource@@QAE@XZ':0x9fbec0,table:0x129dc24,'??1CManagedTextureData@@UAE@XZ':0xa6b5b0,'??3@YAXPAX@Z':0xbfe9bc}
specs=[(ctor,0xa69980,67,0),(scalar,0xa6b590,30,128)]
proofs=[check(obj,symbol,address,size,2,targets) for symbol,address,size,offset in specs]
c=Coff(obj);raw=EXE.read_bytes();section,start=c.symbols[table];data,relocs=c.sections[section];assert start==0 and len(data)==40
slots=['??_ECManagedTextureData@@UAEPAXI@Z','?DeleteCIVCountedPointeeBase@CIVCountedPointeeBase@@UBEXXZ','?UnloadResource@CResource@@UAE_NXZ','?GetUnloadableMemory@CResource@@UBEKXZ','?IsResourceLoaded@CResource@@UBE_NXZ','?OnResourceLoaded@CResource@@UAEXXZ','?CanAbortResourceLoad@CResource@@UBE_NJ@Z','?OnResourceLoadAborted@CResource@@UAEXXZ','?GetDebugMemoryInUse@CResource@@UBEKXZ','?ResetDebugMemoryInUse@CResource@@UAEXXZ']
addresses=[0xa6b590,0x42d930,0x42d940,0x9fbef0,0x42d950,0x42d960,0x42d970,0x42d980,0x9fbf00,0x9fbf10]
c.table(table,slots);assert read_virtual_data(raw,0x129dc24,40)==struct.pack('<10I',*addresses)
body=bytearray(256);fixups=[];cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
external={0x9fbec0:0,0x129dc24:1,0xa6b5b0:2,0xbfe9bc:3}
for symbol,address,size,offset in specs:
 b=read_virtual_data(raw,address,size);body[offset:offset+size]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16);assert target in external;fixups.append((offset+ins.address-address+1,external[target],20))
  elif ins.imm_size==4:
   for operand in ins.operands:
    if operand.type==2 and operand.imm in external:fixups.append((offset+ins.address-address+ins.imm_offset,external[operand.imm],6))
assert len(fixups)==4
(w/'managed_texture_constructor_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
aliases={'@ManagedConstructor@8':ctor,'@ManagedScalar@12':scalar,'_ManagedVTable':table,'??0CResource@@QAE@XZ':'@BaseBoundary@8','??1CManagedTextureData@@UAE@XZ':'@DestructorBoundary@8','?DeleteCIVCountedPointeeBase@CIVCountedPointeeBase@@UBEXXZ':'@UnusedBoundary@8','?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z':'@UnusedBoundary@8'}
(w/'managed_texture_constructor_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeManagedTextureConstructor_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)]
r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr)
assert 'cases=2048 snapshots=9216' in r.stdout,(w,r.stdout)
headers=[ROOT/'rebuild/include'/p for p in ('fable_managed_texture_data.h','fable_resource_list.h','fable_counted_pointer.h','rebuild_abi.h')]
report=dict(cases=2048,snapshots=9216,providers=proofs,omission_checks=1,vtable_slots=slots,vtable_addresses=addresses,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='One mandatory actual constructor translation unit, complete exact 67B constructor and 30B scalar deleting wrapper. Original CResource constructor and managed-data destructor are functional callback boundaries; strong operator delete records requests without freeing arena memory. Full 16KiB snapshots at each boundary and completion, raw flag patterns, mutation, two placements/fills. Constructor preserves byte 0x4b, wrapper deletes only for bit zero and returns original this. Complete ten-slot original vtable relocation audit plus runtime first-slot scalar identity. Existing accepted scalar identity corrected: no new scalar coverage. Main destructor and base constructor behavior are not claimed recovered by this gate.')
(ROOT/'rebuild/compile-gate/native-managed-texture-constructor.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_MANAGED_TEXTURE_CONSTRUCTOR_GATE_PASS')

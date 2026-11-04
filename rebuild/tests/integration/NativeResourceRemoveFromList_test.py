"""Existing canonical inline wrapper plus actual list removal; no callbacks."""
import hashlib,itertools,json,struct,subprocess,sys,tempfile
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
BUILD=Path(tempfile.mkdtemp(prefix='native-resource-remove-list-'))
ROOT='?RemoveFromResourceList@CResource@@QAEXXZ';CHILD='?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z'
SPECS=[('00/9f/CResourceList_CResourceList_009fc570.cpp',ROOT,0x9fc440,16),('00/9f/CResourceList_RemoveResource_009fc020.cpp',CHILD,0x9fc020,80)]
objects=[];sources=[];proofs=[];native={};compiled={};cold=set();targets={CHILD:0x9fc020}
def word(v):return struct.pack('<I',v&0xffffffff)
for i,(relative,symbol,address,length) in enumerate(SPECS):
 source=R/'rebuild/src/compiled'/relative;obj=BUILD/(str(i)+'.obj')
 q=V.cl(['/nologo','/c','/O2','/Oy','/GS','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(source)],V.env(),BUILD)
 assert not q.returncode,q.stdout+q.stderr
 cf=Coff(obj);section,offset=cf.symbols[symbol];body,rel=cf.sections[section];assert offset==0
 proofs.append(check(obj,symbol,address,length,len(rel),targets));fixed=bytearray(body)
 for at,(target,kind) in rel.items():
  assert kind==20 and target==CHILD
  fixed[at:at+4]=word(targets[target]-(address+at+4))
 original=read_virtual_data(EXE.read_bytes(),address,length);assert bytes(fixed)==original
 native[address]=original;compiled[address]=bytes(fixed)
 for data,fixups in cf.sections.values():
  cold.update(target for target,kind in fixups.values() if target not in cf.symbols and target!=CHILD)
 objects.append(obj);sources.append(source)

# The fixture imports the root alias, never compiles a substitute member body.
test=BUILD/'required.cpp';test_obj=BUILD/'required.obj'
aliases=['#pragma comment(linker,"/alternatename:_RequiredRoot='+ROOT+'")']
aliases+=['#pragma comment(linker,"/alternatename:'+name+'=_UnusedBoundary")' for name in sorted(cold)]
test.write_text('#include <stdlib.h>\nextern "C" void UnusedBoundary(){abort();}\nextern "C" void RequiredRoot();\nint main(){RequiredRoot();return 0;}\n'+'\n'.join(aliases)+'\n')
q=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test_obj),str(test)],V.env(),BUILD);assert not q.returncode,q.stdout+q.stderr
command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(BUILD/'required.exe'),str(test_obj)]
for omit in range(2):
 q=subprocess.run(command+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=V.env(),capture_output=True,text=True)
 assert q.returncode and ('LNK2001' in q.stdout or 'LNK2019' in q.stdout),q.stdout+q.stderr
q=subprocess.run(command+list(map(str,objects)),env=V.env(),capture_output=True,text=True);assert not q.returncode,q.stdout+q.stderr

A=0x2000000;STACK=0x3000000;STOP=0x4000000;SIZE=16384
machines=[]
for code in (native,compiled):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for base,size in [(0x9fc000,4096),(A,SIZE),(STACK,65536),(STOP,4096)]:u.mem_map(base,size)
 for address,body in code.items():u.mem_write(address,body)
 machines.append(u)
def get(memory,p):return struct.unpack_from('<I',memory,p-A)[0]
def put(memory,p,value):memory[p-A:p-A+4]=word(value)
values=(0,1,2,0xffffffff);allocations=(0,1,0x7fffffff,0xffffffff);sizes=(0,1,0x80000000,0xffffffff)
cases=snapshots=0
for count,allocated,size,present,alias,links,place,fill in itertools.product(values,allocations,sizes,range(2),range(2),range(6),range(2),range(2)):
 base=A+(0x1800 if place else 0);listp=base+0x100;head=listp+4;resource=head if alias else base+0x300
 prev=resource if links==4 else head if links==5 else base+0x400
 nextp=resource if links==4 else head if links==5 else base+0x440
 initial=bytearray([0xa9 if fill else 0])*SIZE
 put(initial,resource+8,listp if present else 0);put(initial,resource+12,prev);put(initial,resource+16,0 if links==0 else nextp)
 if links:
  put(initial,prev+16,head if links==2 else resource)
  put(initial,nextp+12,head if links==3 else resource)
 put(initial,listp+0x20,count);put(initial,listp+0x24,allocated);put(initial,resource+0x14,size)
 expected=bytearray(initial)
 if get(expected,resource+8):
  receiver=get(expected,resource+8);put(expected,resource+8,0)
  nextp=get(expected,resource+16)
  if nextp:
   prev=get(expected,resource+12)
   if get(expected,prev+16)==resource and get(expected,nextp+12)==resource:
    put(expected,prev+16,nextp)
    # Aliased nodes require live field reads between the two link writes.
    put(expected,get(expected,resource+16)+12,get(expected,resource+12))
   put(expected,resource+12,0);put(expected,resource+16,0)
  put(expected,receiver+0x20,get(expected,receiver+0x20)-1)
  put(expected,receiver+0x24,get(expected,receiver+0x24)-get(expected,resource+0x14))
 for u in machines:
  u.mem_write(A,bytes(initial));u.mem_write(STACK,bytes(65536));sp=STACK+0x8000;u.mem_write(sp,word(STOP))
  u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,resource);u.reg_write(UC_X86_REG_EFLAGS,2)
  regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
  for i,reg in enumerate(regs):u.reg_write(reg,0x11223300+i)
  u.emu_start(0x9fc440,STOP,count=200)
  assert bytes(u.mem_read(A,SIZE))==bytes(expected),(count,allocated,size,present,alias,links,place,fill)
  assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
  assert [u.reg_read(reg) for reg in regs]==[0x11223300+i for i in range(4)]
  snapshots+=1
 cases+=1
files=sources+[R/'rebuild/include/fable_resource_list.h',R/'rebuild/include/fable_game_text_bank.h',Path(__file__)]
report=dict(cases=cases,snapshots=snapshots,arena_bytes=SIZE,providers=proofs,omission_checks=2,source_sha256={str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},
 boundary='Existing exact16B canonical inline RemoveFromResourceList and exact80B RemoveResource execute together, no behavioral callbacks. Null/live list, head/resource alias, detached/reciprocal/broken/self/head links, count/memory/size wrap, two placements/fills. Cold unused constructor/queue imports abort if called; no real heap/GPU or additional coverage.')
(R/'rebuild/compile-gate/native-resource-remove-from-list.json').write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_RESOURCE_REMOVE_FROM_LIST_GATE_PASS',cases,snapshots)

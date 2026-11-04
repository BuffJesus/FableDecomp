"""Independent bounded priority-tail model; no source/PE/PDB dependencies."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=0x98f0e0;CHILD=0x98eff0;A=0x200000;N=32768;G=0x13bc000;MANAGER=0x13bc870;STOP=0x100100;SP=A+0x7000
NAMED_PRIORITY_VALUES=tuple(range(1,8)) # COUNT7 is named, not scheduler priority.
RAW_PRIORITY_VALUES=(0,*NAMED_PRIORITY_VALUES,0x7fffffff,0x80000000,0xffffffff)
def expected(arena,glob,c):
 a=bytearray(arena);g=bytearray(glob)
 if c['mutation']:
  a[0x180]^=0x51;g[0x9c0]^=0x73
 if c['mutation']==2:
  struct.pack_into('<II',a,SP+4-A,0x01234567,0xfedcba98)
 elif c['mutation']==3:
  struct.pack_into('<I',g,MANAGER-G,0x87654321)
  if A<=c['this']<=A+N-4:struct.pack_into('<I',a,c['this']-A,0x80000000)
 return bytes(a),bytes(g),c['eax'],c['flags_after']
def case_domain():
 for uid,priority,this,mutation,eax,flags_before,flags_after in itertools.product((0,1,0x7fffffff,0x80000000,0xfffffffe,0xffffffff),RAW_PRIORITY_VALUES,(0,A+0x100,0x81234567,0xffffffff),range(4),(0,0xffffffff),(0x202,0x246),(0x202,0xa46)):
  yield dict(uid=uid,priority=priority,this=this,mutation=mutation,eax=eax,flags_before=flags_before,flags_after=flags_after)
def check(body):
 assert len(body)==10
 hits={ROOT:0,CHILD:0};cases=points=named=raw_only=0
 for c in case_domain():
  arena=bytearray((i*17+cases)&255 for i in range(N));glob=bytes((i*31+cases)&255 for i in range(4096));struct.pack_into('<III',arena,SP-A,STOP,c['uid'],c['priority']);arena=bytes(arena);want=expected(arena,glob,c)
  u=Uc(UC_ARCH_X86,UC_MODE_32)
  for address,size in ((A,N),(G,4096),(0x980000,0x20000),(0x100000,4096)):u.mem_map(address,size)
  u.mem_write(A,arena);u.mem_write(G,glob);u.mem_write(ROOT,body);u.mem_write(CHILD,b'\xc2\x08\x00')
  saved={UC_X86_REG_EBX:0x12345678,UC_X86_REG_ESI:0x87654321,UC_X86_REG_EDI:0xfedcba98,UC_X86_REG_EBP:0xabcdef01}
  for reg,v in saved.items():u.reg_write(reg,v)
  u.reg_write(UC_X86_REG_ECX,c['this']);u.reg_write(UC_X86_REG_ESP,SP);u.reg_write(UC_X86_REG_EFLAGS,c['flags_before']);events=[]
  def hook(machine,pc,size,user):
   nonlocal points
   if pc==ROOT:hits[ROOT]+=1
   elif pc==CHILD:
    hits[CHILD]+=1;assert not events;events.append((MANAGER,c['uid'],c['priority']))
    assert machine.reg_read(UC_X86_REG_ECX)==MANAGER and machine.reg_read(UC_X86_REG_ESP)==SP
    assert bytes(machine.mem_read(SP,12))==struct.pack('<III',STOP,c['uid'],c['priority'])
    assert bytes(machine.mem_read(A,N))==arena and bytes(machine.mem_read(G,4096))==glob
    assert machine.reg_read(UC_X86_REG_EFLAGS)==c['flags_before'];points+=1
    machine.mem_write(A,want[0]);machine.mem_write(G,want[1]);machine.reg_write(UC_X86_REG_EAX,c['eax']);machine.reg_write(UC_X86_REG_ECX,0x76543210);machine.reg_write(UC_X86_REG_EDX,0x89abcdef);machine.reg_write(UC_X86_REG_EFLAGS,c['flags_after'])
   elif pc==STOP:machine.emu_stop()
  u.hook_add(UC_HOOK_CODE,hook);u.emu_start(ROOT,STOP,count=32)
  assert events==[(MANAGER,c['uid'],c['priority'])] and u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+12
  assert all(u.reg_read(reg)==v for reg,v in saved.items())
  assert u.reg_read(UC_X86_REG_EAX)==want[2] and u.reg_read(UC_X86_REG_EFLAGS)==want[3]
  assert u.reg_read(UC_X86_REG_ECX)==0x76543210 and u.reg_read(UC_X86_REG_EDX)==0x89abcdef
  assert bytes(u.mem_read(A,N))==want[0] and bytes(u.mem_read(G,4096))==want[1]
  cases+=1;points+=1
  if c['priority'] in NAMED_PRIORITY_VALUES:named+=1
  else:raw_only+=1
 assert cases==8448 and points==16896 and hits=={ROOT:cases,CHILD:cases} and (named,raw_only)==(5376,3072)
 return dict(cases=cases,checkpoints=points,hits={hex(k):v for k,v in hits.items()},named_enum_bit_patterns=named,raw_unnamed_or_out_of_range_machine_patterns=raw_only,typed_caller_operational_enum_domain=list(range(1,7)),count7_sentinel_machine_cases=768)

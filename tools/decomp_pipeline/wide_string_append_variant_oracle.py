"""Bounded contracts for the operator+ TU's near wide-append COMDAT."""
import hashlib,itertools,struct
from pathlib import Path
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY,SYMBOL=0x99b8d0,'??YCWideString@@QAEABV0@ABV0@@Z'
ARENA,DST,SRC,DST_DATA,SRC_DATA,NEW_DST,NEW_SRC=0x2000000,0x2000100,0x2000110,0x2000200,0x2000240,0x2000280,0x20002c0
STACK,STOP,EMPTY,OWN,APPEND=0x3000000,0x4000000,0x129a8e0,0x99b560,0x99c7e0
ADDRESS={'?MakeOwnCopyOfData@CWideString@@AAEXXZ':OWN,'?g_FableEmptyWideString_0129A8E0@@3PAGA':EMPTY,
 '??$append@PBG@?$basic_string@GU?$char_traits@G@_STL@@V?$allocator@G@2@@_STL@@QAEAAV01@PBG0ABUforward_iterator_tag@1@@Z':APPEND}

def execute(code,args,missing_own=False):
 dest_present,source_present,self_alias,mutation,owners,length,seed=args;rhs=DST if self_alias else SRC
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x99b000,0x2000),(0x129a000,0x1000),(ARENA,0x4000),(STACK,0x10000),(STOP,0x1000)):
  u.mem_map(a,n);u.mem_write(a,b'\xcc'*n)
 u.mem_write(ENTRY,code)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(ARENA,bytes((i*37+seed*19)&255 for i in range(0x4000)));u.mem_write(EMPTY,b'\0\0')
 put(DST,DST_DATA if dest_present else 0);put(SRC,SRC_DATA if source_present else 0)
 for i,(data,n,count) in enumerate(((DST_DATA,2,4),(SRC_DATA,length,owners),(NEW_DST,2,1),(NEW_SRC,length,owners))):
  text=ARENA+0x1000+i*0x100;content=b''.join(struct.pack('<H',0x410+i*32+j+seed) for j in range(n))+b'\0\0';u.mem_write(text,content)
  for off,value in ((0,text),(4,text+n*2),(8,text+64),(12,count)):put(data+off,value)
 expected=bytearray(u.mem_read(ARENA,0x4000));trace=[]
 def model(a,value):struct.pack_into('<I',expected,a-ARENA,value&0xffffffff)
 def change(a,value):put(a,value);model(a,value)
 def verify():assert bytes(u.mem_read(ARENA,0x4000))==bytes(expected)
 if not dest_present:
  other=get(rhs)
  if rhs!=DST and other:model(DST,other);model(other+12,get(other+12)+1)
 def hook(uc,a,size,_):
  if a not in (OWN,APPEND) or (a==OWN and missing_own):return
  sp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX)
  if a==OWN:
   assert dest_present and not trace and ecx==DST;verify();trace.append(('own_copy',))
   if mutation:
    change(DST,NEW_DST);change(DST_DATA+12,3)
    if mutation==2 and not self_alias:change(SRC,NEW_SRC)
   pop=0;result=0xbad00000
  else:
   assert dest_present and trace==[('own_copy',)] and ecx==get(DST);verify()
   other=get(rhs);first=get(other) if other else EMPTY;end=first
   while bytes(u.mem_read(end,2))!=b'\0\0':end+=2
   assert (get(sp+4),get(sp+8))==(first,end)
   assert STACK<=get(sp+12)<STACK+0x10000
   data=get(DST);out=get(data+4);payload=bytes(u.mem_read(first,end-first))+b'\0\0'
   assert out+len(payload)<=get(data+8)
   u.mem_write(out,payload);expected[out-ARENA:out-ARENA+len(payload)]=payload;change(data+4,out+len(payload)-2)
   trace.append(('append',data,first,end));pop=12;result=data
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,rhs);saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ECX,DST);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(ENTRY,STOP,count=500)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
 assert u.reg_read(UC_X86_REG_EAX)==DST and all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved))
 verify();assert len(trace)==(2 if dest_present else 0)
 return trace,bytes(expected)

def compare_variant(obj):
 c=Coff(obj);sec,off=c.symbols[SYMBOL];body,rels=c.sections[sec];assert off==0 and len(body)==104
 original=read_virtual_data(EXE.read_bytes(),ENTRY,104)
 grade=compare(body,original,tuple(rels),retail_relocations('0099b8d0',104)).to_dict();assert grade['near_match'] and grade['opcode_similarity']>=.90
 patched=bytearray(body)
 for site,(name,kind) in rels.items():
  assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0
  struct.pack_into('<I',patched,site,(ADDRESS[name]-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
 cases=0
 for args in itertools.product((False,True),(False,True),(False,True),range(3),(0,1,2,0xffffffff),(0,1,3),(0,7)):
  assert execute(bytes(patched),args)==execute(original,args);cases+=1
 for code,missing in ((b'\xc2\x04\x00',False),(bytes(patched),True)):
  detected=False
  try:execute(code,(True,True,False,1,1,1,0),missing)
  except Exception:detected=True
  assert detected
 return dict(status='PASS',cases=cases,grade=grade,acceptance_grade='NEAR_MATCH',bytes=104,original_bytes=104,controls=2,object_sha256=hashlib.sha256(Path(obj).read_bytes()).hexdigest(),
  boundaries=['Copy-on-write and STL append controlled as typed child contracts, including replacement/live source reload, self aliasing, null/empty source and shared-owner wrapping.',
  'Full16KiB independent memory prediction and thiscall ABI. Actual child algorithms and allocation are exercised separately by the own-copy variant and native path composition.'])

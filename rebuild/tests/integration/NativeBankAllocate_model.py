from pathlib import Path
import sys,json,struct,itertools,hashlib
sys.path.insert(0,'tools/decomp_pipeline')
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parent
TARGETS={'?DeallocateMemory@?$CArrayVector@UCRuntimeData@CBankFile@@@@QAEXXZ': 10304912, '?resize@?$vector@UCRuntimeData@CBankFile@@V?$allocator@UCRuntimeData@CBankFile@@@_STL@@@_STL@@QAEXIABUCRuntimeData@CBankFile@@@Z': 10305552, '?DeallocateMemory@?$CArrayVector@VCCharString@@@@QAEXXZ': 10305456, '?resize@?$vector@VCCharString@@V?$allocator@VCCharString@@@_STL@@@_STL@@QAEXI@Z': 4831072, '_memmove': 12577510, '_free': 12577300, '?resize@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXIABK@Z': 4606257, '?resize@?$vector@PAVCBankFileEntryUpdateData@@V?$allocator@PAVCBankFileEntryUpdateData@@@_STL@@@_STL@@QAEXIABQAVCBankFileEntryUpdateData@@@Z': 10305008, '?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@K@_STL@@@2@@Z': 5739573}
TARGETS['_FixtureBankAllocateFree']=0xbfea14
ENTRY=0x9ceae0;A=0x2000000;BANK=A+0x100;STACK=0x3000000;STOP=0x4000000
original=read_virtual_data(EXE.read_bytes(),ENTRY,375)

def execute(code,args):
 size,flags,capacity,maps,mutation=args;u=Uc(UC_ARCH_X86,UC_MODE_32)
 for page in {x&~4095 for x in TARGETS.values()}|{ENTRY&~4095,STOP}:u.mem_map(page,4096)
 for address in TARGETS.values():u.mem_write(address,b'\xc3')
 u.mem_map(A,0x3000);u.mem_map(STACK,0x10000);u.mem_write(ENTRY,code)
 expected=bytearray((i*37+13)&255 for i in range(0x3000));u.mem_write(A,bytes(expected))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v,track=True):
  raw=struct.pack('<I',v&0xffffffff);u.mem_write(a,raw)
  if track:expected[a-A:a-A+4]=raw
 def expect(a,v):struct.pack_into('<I',expected,a-A,v&0xffffffff)
 def triple(a,values):
  for i,v in enumerate(values):put(a+4*i,v)
 put(BANK+4,0x12345678);put(BANK+0x78,flags)
 for offset,base,bit in [(0x14,A+0x800,1),(0x2c,A+0x900,2)]:
  triple(BANK+offset,(base,base+4,base+16) if capacity&bit else (0,0,0))
 for offset,base,bit in [(0xac,A+0xa00,1),(0xc4,A+0xb00,2)]:
  put(BANK+offset,base);put(BANK+offset+4,3 if maps&bit else 0);put(base+4,base+64)
 expected_initial=bytes(expected);expect(BANK+4,size)
 sp=STACK+0x8000;put(sp,STOP,False);put(sp+4,size,False);trace=[];freed=[];map_calls=[]
 def flag_change(mask):put(BANK+0x78,get(BANK+0x78)^mask)
 def hook(uc,address,length,_):
  if address not in TARGETS.values():assert ENTRY<=address<ENTRY+len(code),hex(address);return
  esp=uc.reg_read(UC_X86_REG_ESP);cx=uc.reg_read(UC_X86_REG_ECX);pop=0;ret=0x87654321
  if address==0x9d3d90:
   assert not trace and cx==BANK+0x20 and get(BANK+4)==size;triple(cx,(0,0,0));trace.append('runtime-clear')
   if mutation==1:put(BANK+4,0x55555555)
  elif address==0x9d4010:
   assert trace==['runtime-clear'] and cx==BANK+0x20 and get(esp+4)==size;value=get(esp+8);assert bytes(u.mem_read(value,10))==b'\0'*10
   triple(cx,(A+0x1000,A+0x1018,A+0x1030));pop=8;trace.append('runtime-resize')
   if mutation==2:flag_change(8)
  elif address==0x9d3fb0:
   assert trace[-1]=='runtime-resize' and cx==BANK+8;triple(cx,(0,0,0));trace.append('symbols-clear')
   if mutation==3:flag_change(0x38)
  elif address==0x49b760:
   assert trace[-1]=='symbols-clear' and cx==BANK+8 and get(BANK+0x78)&8 and get(esp+4)==size
   triple(cx,(A+0x1100,A+0x1104,A+0x1120));pop=4;trace.append('symbols-resize')
  elif address==0xbfea14:
   pointer=get(esp+4);offset={A+0x800:0x14,A+0x900:0x2c}[pointer];assert capacity&(1 if offset==0x14 else 2) and pointer not in freed
   assert [get(BANK+offset+4*i) for i in range(3)]==[0,0,0]
   for i in range(3):expect(BANK+offset+4*i,0)
   freed.append(pointer);trace.append('checksum-free' if offset==0x14 else 'update-free')
   if mutation==4 and offset==0x14:flag_change(16)
   if mutation==5 and offset==0x2c:flag_change(32)
  elif address in (0x464931,0x9d3df0):
   offset,flag,base,label=(0x14,16,A+0x1200,'checksum-resize') if address==0x464931 else (0x2c,32,A+0x1300,'update-resize')
   assert cx==BANK+offset and get(BANK+0x78)&flag and get(esp+4)==size and get(get(esp+8))==0
   triple(cx,(base,base+4,base+16));pop=8;trace.append(label)
  elif address==0x579435:
   assert cx in (BANK+0xac,BANK+0xc4);offset=cx-BANK;bit=1 if offset==0xac else 2;assert maps&bit and offset not in map_calls
   header=get(cx);assert get(esp+4)==get(header+4);pop=4;map_calls.append(offset);trace.append('symbol-map' if bit==1 else 'filename-map')
   if mutation==6:header=A+0x1400+bit*64;put(cx,header)
   expect(header+8,header);expect(header+4,0);expect(header+12,header);expect(cx+4,0)
  else:raise AssertionError(('unexpected helper',hex(address)))
  uc.reg_write(UC_X86_REG_EAX,ret);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0xdeadf00d);uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);saved=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
 for n,reg in enumerate(saved):u.reg_write(reg,0x11112200+n)
 u.reg_write(UC_X86_REG_ECX,BANK);u.reg_write(UC_X86_REG_ESP,sp)
 try:u.emu_start(ENTRY,STOP,count=1000)
 except Exception as exc:raise AssertionError((args,trace,hex(u.reg_read(UC_X86_REG_EIP)))) from exc
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and all(u.reg_read(reg)==0x11112200+n for n,reg in enumerate(saved))
 assert bytes(u.mem_read(A,0x3000))==bytes(expected),(args,trace,'state')
 assert len(freed)==bool(capacity&1)+bool(capacity&2) and len(map_calls)==bool(maps&1)+bool(maps&2)
 assert ('symbols-resize' in trace)==bool(get(BANK+0x78)&8)
 assert ('checksum-resize' in trace)==bool(get(BANK+0x78)&16)
 assert ('update-resize' in trace)==bool(get(BANK+0x78)&32)
 return dict(args=args,trace=trace,state_sha256=hashlib.sha256(expected).hexdigest())

def compare(obj):
 c=Coff(obj);s,o=c.symbols['?AllocateBuffers@CBankFile@@AAEXK@Z'];body,rels=c.sections[s];assert not o;patched=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<i',body,site)[0]-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(patched)==original
 results=[]
 for args in itertools.product((0,1,7,0xffffffff),range(0,64,8),range(4),range(4),range(7)):
  a=execute(original,args);z=execute(bytes(patched),args);assert a==z;results.append(a)
 return dict(status='PASS',grade='MATCH',cases=len(results),bytes=len(body),fixups=len(rels),object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),boundary='Original/rebuilt12KiB state and ABI with controlled array/tree helpers, including helper mutations; no allocation failure or exception unwinding claim.')

def oracle_controls():
 controls=[
  ('size-offset',0x9ceaf4,b'\x08',(7,0,0,0,0)),
  ('skip-runtime-release',0x9ceaf5,b'\x90'*5,(7,0,0,0,0)),
  ('symbol-flag',0x9ceb26,b'\x04',(7,8,0,0,0)),
  ('checksum-flag',0x9ceb7c,b'\x08',(7,16,0,0,0)),
  ('update-flag',0x9cebdb,b'\x10',(7,32,0,0,0)),
  ('skip-checksum-free',0x9ceb71,b'\x90'*5,(7,0,1,0,0)),
  ('skip-map-erasure',0x9cec07,b'\x90'*5,(7,0,0,1,0)),
  ('skip-map-left-reset',0x9cec0e,b'\x90'*3,(7,0,0,1,0)),
 ]
 results=[]
 for name,address,replacement,args in controls:
  execute(original,args);changed=bytearray(original);offset=address-ENTRY;changed[offset:offset+len(replacement)]=replacement
  try:execute(bytes(changed),args)
  except AssertionError as exc:results.append(dict(name=name,detected=True,detail=str(exc)))
  else:raise AssertionError(name)
 return results

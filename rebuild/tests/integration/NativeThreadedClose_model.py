"""Independent frozen byte-state/ABI model and immutable machine runners."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ESP,UC_X86_REG_EIP

def run(body,mode):
 ARENA=0x200000;SIZE=0x8000;FILE=ARENA+0x100;SP=ARENA+0x7000;MANAGER=0x13bc870;STOP=0x100100;CLOSE=0x600000
 def put(b,offset,value):struct.pack_into('<I',b,offset,value&0xffffffff)
 def get(b,offset):return struct.unpack_from('<I',b,offset)[0]
 cases=0;snapshots=0;skip=0;called=0
 for initial in (0xffffffff,0,1,0x80000000):
  for changed in (0xffffffff,0,0x12345678,0x80000000):
   for result in (0,1,0xffffffff):
    for flag in (0,1,255):
     arena=bytearray((i*13+cases)&255 for i in range(SIZE));globals_=bytearray((i*29+cases)&255 for i in range(0x178));put(arena,FILE-ARENA+8,initial);arena[FILE-ARENA+0x18:FILE-ARENA+0x1a]=bytes([flag,flag]);put(arena,SP-ARENA,STOP)
     expected=bytearray(arena);eg=bytearray(globals_);phases=[];put(expected,SP-ARENA-4,0xfedcba98)
     if initial!=0xffffffff:
      put(expected,SP-ARENA-8,FILE);put(expected,SP-ARENA-12,0x98ec54);put(expected,FILE-ARENA+8,changed);expected[FILE-ARENA+0x18:FILE-ARENA+0x1a]=b'\xa5\x5a';expected[0x300]^=0x39;eg[0x40]^=0x21;phases.append(('manager',bytes(expected),bytes(eg)))
      put(expected,SP-ARENA-8,changed);put(expected,SP-ARENA-12,0x98ec5e);expected[0x301]^=0x73;eg[0x41]^=0x12;phases.append(('closehandle',bytes(expected),bytes(eg)))
      put(expected,FILE-ARENA+8,0xffffffff);expected[FILE-ARENA+0x18:FILE-ARENA+0x1a]=b'\0\0';called+=1
     else:skip+=1
     u=Uc(UC_ARCH_X86,UC_MODE_32)
     for base,size in ((0x980000,0x10000),(0x1300000,0x100000),(0x143f000,0x1000),(CLOSE,0x1000),(ARENA,SIZE),(0x100000,0x1000)):u.mem_map(base,size)
     u.mem_write(0x98ec40,body);u.mem_write(ARENA,bytes(arena));u.mem_write(MANAGER,bytes(globals_));u.mem_write(0x143fe30,struct.pack('<I',CLOSE));u.mem_write(0x98e7d0,b'\xc2\x04\0');u.mem_write(CLOSE,b'\xc2\x04\0')
     callee={UC_X86_REG_EBX:0x81234567,UC_X86_REG_ESI:0xfedcba98,UC_X86_REG_EDI:0x87654321,UC_X86_REG_EBP:0xaabbccdd}
     for r,v in callee.items():u.reg_write(r,v)
     u.reg_write(UC_X86_REG_ECX,FILE);u.reg_write(UC_X86_REG_ESP,SP);events=[];actualphases=[]
     def hook(machine,address,size,user):
      if address==0x98e7d0:
       stack=machine.reg_read(UC_X86_REG_ESP);assert machine.reg_read(UC_X86_REG_ECX)==MANAGER and struct.unpack('<I',machine.mem_read(stack+4,4))[0]==FILE
       machine.mem_write(FILE+8,struct.pack('<I',changed));machine.mem_write(FILE+0x18,b'\xa5\x5a');machine.mem_write(ARENA+0x300,bytes([arena[0x300]^0x39]));machine.mem_write(MANAGER+0x40,bytes([globals_[0x40]^0x21]));events.append('manager')
      elif address==CLOSE:
       stack=machine.reg_read(UC_X86_REG_ESP);assert struct.unpack('<I',machine.mem_read(stack+4,4))[0]==changed
       machine.mem_write(ARENA+0x301,bytes([arena[0x301]^0x73]));machine.mem_write(MANAGER+0x41,bytes([globals_[0x41]^0x12]));machine.reg_write(UC_X86_REG_EAX,result);events.append('closehandle')
      else:return
      actualphases.append((events[-1],bytes(machine.mem_read(ARENA,SIZE)),bytes(machine.mem_read(MANAGER,0x178))))
     u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x98ec40,STOP,count=100)
     assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==SP+4 and all(u.reg_read(r)==v for r,v in callee.items())
     actual=bytearray(u.mem_read(ARENA,SIZE));assert actual==expected and bytes(u.mem_read(MANAGER,0x178))==eg and actualphases==phases
     assert events==([] if initial==0xffffffff else ['manager','closehandle']);cases+=1;snapshots+=len(phases)+1
 return dict(mode=mode,cases=cases,snapshots=snapshots,skip_cases=skip,called_cases=called,arena_bytes=SIZE,manager_global_bytes=0x178)

"""Original/recreated repeated char setter, allocation/free boundaries controlled."""
from pathlib import Path
import sys,json,struct,itertools
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9a0390;DATA=0x2000000;STACK=0x3000000;STOP=0x4000000;OBJ=DATA+0x100;OLD=DATA+0x1000;NEW=DATA+0x2000
TARGETS={'??_V@YAXPAX@Z':0xbfeb1c,'??_U@YAPAXI@Z':0xbfeb22}
def execute(code,length,ch,old,high):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for address,size in ((0x9a0000,0x1000),(0xbfe000,0x1000),(DATA,0x10000),(STACK,0x10000),(STOP,0x1000)):u.mem_map(address,size)
 u.mem_write(ENTRY,bytes(code));u.mem_write(DATA,bytes([0x59])*0x10000);trace=[];cap=(length+4)&~3
 put=lambda a,v:u.mem_write(a,struct.pack('<I',v&0xffffffff));get=lambda a:struct.unpack('<I',u.mem_read(a,4))[0]
 put(OBJ,OLD if old else 0);put(OBJ+4,77);put(OBJ+8,(high<<31)|112)
 expected=bytearray(u.mem_read(OBJ-16,45));struct.pack_into('<III',expected,16,NEW,length,(high<<31)|cap)
 def hook(uc,address,size,_):
  if address not in TARGETS.values():return
  sp=uc.reg_read(UC_X86_REG_ESP)
  if address==0xbfeb1c:
   assert old and not trace and get(sp+4)==OLD;trace.append('delete');u.mem_write(OLD,bytes([0xd7])*112);rv=0xbad0
  else:
   assert trace==(['delete'] if old else []) and get(sp+4)==cap and get(OBJ)==0
   trace.append('new');rv=NEW
  uc.reg_write(UC_X86_REG_EAX,rv);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,length);put(sp+8,ch|0x7193ab00)
 regs={UC_X86_REG_EBX:0x11223344,UC_X86_REG_ESI:0x22334455,UC_X86_REG_EDI:0x33445566,UC_X86_REG_EBP:0x44556677}
 for reg,value in regs.items():u.reg_write(reg,value)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJ);u.emu_start(ENTRY,STOP,count=2000)
 assert u.reg_read(UC_X86_REG_ESP)==sp+12 and all(u.reg_read(reg)==value for reg,value in regs.items())
 assert bytes(u.mem_read(OBJ-16,45))==bytes(expected)
 assert bytes(u.mem_read(NEW-16,cap+32))==bytes([0x59])*16+bytes([ch])*length+bytes(cap-length)+bytes([0x59])*16
 assert trace==(['delete','new'] if old else ['new'])
 return trace

def run(obj,out):
 c=Coff(obj);sec,off=c.symbols['?SetString@?$CBasicString@D@@IAEXKD@Z'];body,rels=c.sections[sec];fixed=bytearray(body);assert not off
 for site,(name,kind) in rels.items():assert kind==20;struct.pack_into('<I',fixed,site,(TARGETS[name]+struct.unpack_from('<i',body,site)[0]-(ENTRY+site+4))&0xffffffff)
 original=read_virtual_data(EXE.read_bytes(),ENTRY,158);cases=0
 for length,ch,old,high in itertools.product((0,1,2,3,4,5,6,7,8,9,15,16,17,31,32,33,63,64,65,127,128,129),(0,1,127,128,255),(0,1),(0,1)):
  assert execute(original,length,ch,old,high)==execute(fixed,length,ch,old,high);cases+=1
 report=dict(status='PASS',cases=cases,bytes=len(body),boundary='Full object/guard/allocation byte checks plus allocator call size/order, low byte char ABI including poisoned high stack bits, callee registers and stack. New/delete controlled; original and recreated instructions executed.');Path(out).write_text(json.dumps(report,indent=2));print('REPEATED_CHAR_MODEL_PASS',cases)
if __name__=='__main__':run(Path(sys.argv[1]),Path(sys.argv[2]))

"""Execute CCompressorZlib::Compress / Decompress (the 13-byte block header around InternalCompress/InternalDecompress) against retail."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
OBJ=0x2000000;IN=0x2001000;OUT=0x2002000;INFO=0x2003000;STACK=0x3000000;STOP=0x4000000
COMPRESS,DECOMPRESS,ICOMPRESS,IDECOMPRESS=0xa3ac40,0xa3abe0,0xa3aa40,0xa3ab00
def run(codes,entry,args,result,setup=None):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0xa3a000,0x1000),(OBJ,0x4000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():u.mem_write(a,bytes(b))
 for a in (ICOMPRESS,IDECOMPRESS):u.mem_write(a,b'\xc3')
 u.mem_write(IN,bytes(i*3&0xff for i in range(64)));u.mem_write(OUT,b'\xee'*0x100);u.mem_write(INFO,b'\xcc'*16)
 if setup:setup(u)
 trace=[]
 def hook(uc,a,s,user):
  if a not in (ICOMPRESS,IDECOMPRESS):return
  sp=uc.reg_read(UC_X86_REG_ESP);d,n,o,c=[get(sp+4+4*i) for i in range(4)]
  trace.append(('compress' if a==ICOMPRESS else 'decompress',uc.reg_read(UC_X86_REG_ECX),d,n,o,c))
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+20)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 for i,v in enumerate(args):put(sp+4+4*i,v)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(entry,STOP,count=200)
 assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs))
 return trace,bytes(u.mem_read(OUT,32)),bytes(u.mem_read(INFO,12))
def compare(candidate,native):
 cases=0
 for size,result,cap in itertools.product((0,1,40,0x7fffffff),(0,9,50),(13,64,0x100)):
  c=run(candidate,COMPRESS,[IN,size,OUT,cap,INFO],result);assert c==run(native,COMPRESS,[IN,size,OUT,cap,INFO],result),(size,result)
  trace,out,info=c;mag=b'1PMC'
  if size==0:assert not trace and out[:13]==mag+struct.pack('<iiB',0,13,6) and out[13]==0xee and info[:9]==struct.pack('<iiB',0,13,6)
  else:assert trace==[('compress',OBJ,IN,size,OUT+13,(cap-13)&0xffffffff)] and out[:13]==mag+struct.pack('<IIB',size,(result+13)&0xffffffff,6) and info[:9]==struct.pack('<IIB',size,(result+13)&0xffffffff,6)
  cases+=1
 for size,un,co,typ in itertools.product((13,14,60),(0,7,0x12345678),(13,99),(6,0)):
  def setup(u,un=un,co=co,typ=typ):u.mem_write(OUT,b'1PMC'+struct.pack('<iiB',un,co,typ))
  c=run(candidate,DECOMPRESS,[OUT,size,IN,64,INFO],0,setup);assert c==run(native,DECOMPRESS,[OUT,size,IN,64,INFO],0,setup)
  trace,out,info=c;assert info[:9]==struct.pack('<iiB',un,co,typ)
  assert trace==([] if size==13 else [('decompress',OBJ,OUT+13,size-13,IN,64)]);cases+=1
 return cases

"""Execute CMemoryFile's buffer members (Write, Read, seeks, SetLength, Close) against retail."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UcError
from unicorn.x86_const import *
FILE=0x2000000;HEAP=0x2100000;SRC=0x2200000;DST=0x2300000;STACK=0x3000000;STOP=0x4000000
WRITE,READ,SETPOS,MOVEPOS,SETLEN,CLOSE=0x9e95f0,0x9e9220,0x9e90c0,0x9e9140,0x9e9730,0x9e9390
EXC_CTOR=0x99ae50
ORACLE_SET={WRITE,READ,SETPOS,MOVEPOS,SETLEN,CLOSE,EXC_CTOR}
STR_CTOR,STR_PLUS,STR_DTOR,STR_COPY,STR_TEXT=0x99ebf0,0x99f600,0x99eae0,0x99ec30,0x99e4c0
THROW,RESIZE2,RESIZE1,MEMMOVE=0xbfeb84,0x486189,0x4861df,0xbfeae6
LITERALS={0x129a898:'Cannot seek past end of file',0x129a8b8:'Cannot move before beginning of file',0x129a850:'Null dst parameter',0x129a830:'Cannot read past end of file',0x129c394:'Null src parameter',0x129a878:'File is not open for writing'}
class Thrown(Exception):pass
def run(codes,entry,args,state):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(0x486000,0x1000),(0x99a000,0x6000),(0x9e9000,0x1000),(0xbfe000,0x1000),(FILE,0x1000),(HEAP,0x10000),(SRC,0x1000),(DST,0x1000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,b in codes.items():
  if a in ORACLE_SET:u.mem_write(a,bytes(b))
 for a in (STR_CTOR,STR_PLUS,STR_DTOR,STR_COPY,STR_TEXT,THROW,RESIZE2,RESIZE1,MEMMOVE):u.mem_write(a,b'\xc3')
 access,position,length,data=state
 u.mem_write(FILE,b'\x77'*0x24);put(FILE,0x122e0fc);put(FILE+4,access);put(FILE+8,position);put(FILE+0xc,length);put(FILE+0x20,0x55aa55aa);u.mem_write(FILE+0x1c,b'\x01')
 heap=[HEAP+0x100]
 def store(bytes_,cap):
  start=heap[0];heap[0]+=max(cap,1)+0x40;u.mem_write(start,bytes(bytes_));put(FILE+0x10,start if cap else 0);put(FILE+0x14,start+len(bytes_) if cap else 0);put(FILE+0x18,start+cap if cap else 0)
 store(data,len(data))
 u.mem_write(SRC,bytes((0xa0+i)&0xff for i in range(0x400)));u.mem_write(DST,b'\xee'*0x200)
 trace=[];strings={}
 def buffer():s,f=get(FILE+0x10),get(FILE+0x14);return bytes(u.mem_read(s,f-s)) if s else b''
 def hook(uc,a,size,user):
  sp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);edx=uc.reg_read(UC_X86_REG_EDX);ret=get(sp);result=0;pop=0
  if a==STR_CTOR:strings[ecx]='';put(ecx,ecx);pop=8;result=ecx
  elif a==STR_PLUS:strings[ecx]=strings.get(edx,'?')+LITERALS.get(get(sp+4),'?');put(ecx,ecx);pop=4;result=ecx
  elif a==STR_COPY:strings[ecx]=strings.get(get(sp+4),'?');put(ecx,ecx);pop=4;result=ecx
  elif a==STR_TEXT:trace.append(('text',strings.get(ecx,'?')));result=ecx
  elif a==STR_DTOR:pass
  elif a==THROW:raise Thrown(('throw',get(get(sp+4)),get(sp+8)))
  elif a in (RESIZE2,RESIZE1):
   n=get(sp+4);fill=u.mem_read(get(sp+8),1)[0] if a==RESIZE2 else 0;old=buffer();new=(old+bytes([fill])*n)[:n];trace.append(('resize',n,fill));store(new,n);pop=8 if a==RESIZE2 else 4
  elif a==MEMMOVE:d,s,n=get(sp+4),get(sp+8),get(sp+12);uc.mem_write(d,bytes(uc.mem_read(s,n)));result=d
  else:return
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,ret);uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 for i,v in enumerate(args):put(sp+4+4*i,v)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,FILE);u.reg_write(UC_X86_REG_ESP,sp)
 try:
  u.emu_start(entry,STOP,count=200000);outcome='return'
  assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs)),hex(entry)
 except Thrown as e:outcome=e.args[0];u.emu_stop()
 fields=struct.unpack('<IIII',u.mem_read(FILE,16))
 return outcome,trace,fields,u.mem_read(FILE+0x1c,1)[0],get(FILE+0x20),buffer(),bytes(u.mem_read(DST,0x200))
def expect_write(state,data,size):
 access,position,length,buf=state
 if not data:return ('throw',1)
 if size==0:return None
 end=(position+size)&0xffffffff
 if end>=0x80000000:return None
 if access==1:return ('throw',8)
 buf=bytearray(buf)
 if end>length:buf=(buf+bytes(end))[:end] if end>len(buf) else buf[:end];length=end
 for i in range(size):buf[position+i]=(0xa0+i)&0xff
 return ('ok',position+size,length,bytes(buf))
def compare(candidate,native):
 cases=0
 for n,position,size,access,nulldata in itertools.product((0,1,7,32),(0,1,6,31,32,0x7fffff00),(0,1,5,40,0x200),(1,2,3),(0,1)):
  if position>n and position!=0x7fffff00:continue
  if position==0x7fffff00 and size not in (0,0x200):continue
  data=bytes((i*29+n)&0xff for i in range(n));state=(access,position,n,data);args=[0 if nulldata else SRC,size,0]
  c=run(candidate,WRITE,args,state);r=run(native,WRITE,args,state);assert c==r,(n,position,size,access,nulldata)
  want=expect_write(state,not nulldata,size)
  if want is None:assert c[0]=='return' and c[2][2]==position and c[5]==data
  elif want[0]=='throw':assert c[0][0]=='throw' and c[0][1]==want[1] and c[0][2]==0x1369380 and c[1][-1][0]=='text'
  else:assert c[0]=='return' and c[2][2]==want[1] and c[2][3]==want[2] and c[5]==want[3] and c[4]==0x55aa55aa
  cases+=1
  for entry,args in [(READ,[0 if nulldata else DST,size,0]),(SETPOS,[size]),(MOVEPOS,[(size-position)&0xffffffff]),(SETLEN,[size]),(CLOSE,[])]:
   if size>=0x200 and entry==READ:continue
   assert run(candidate,entry,args,state)==run(native,entry,args,state);cases+=1
 return cases

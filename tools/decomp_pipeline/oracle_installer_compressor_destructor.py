"""Retail compressor destructor with independent complete-region predictions."""
import itertools,json,struct
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;WORKSPACE=BASE+0x1000;OTHER=BASE+0x2000
ROOT=Path(__file__).resolve().parents[2]
def put(s,a,v):struct.pack_into('<I',s,a-BASE,v&0xffffffff)
def get(s,a):return struct.unpack_from('<I',s,a-BASE)[0]
def initial(initialized,compressing,workspace,mutation,result):
 s=bytearray(b'\xa5'*WIDTH);s[0x54]=initialized;s[0x55]=compressing
 put(s,BASE+0x10,WORKSPACE if workspace else 0)
 return s
def mutate(s,mode):
 if mode==1:put(s,BASE+0x10,OTHER)
 if mode==2:put(s,BASE+0x10,0)
 if mode==3:
  s[0x54]=1;s[0x55]=1
  for offset in (0x1c,0x20,0x28,0x2c):put(s,BASE+offset,0xabcdef01)
def model(args):
 initialized,compressing,workspace,mode,result=args;s=initial(*args);events=[];put(s,BASE,0x1245974)
 if initialized:
  events.append((1 if compressing else 2,BASE+0x1c,bytes(s)));mutate(s,mode)
  if not compressing:
   for offset in (0x1c,0x28,0x20,0x2c):put(s,BASE+offset,0)
  s[0x54]=0
 area=get(s,BASE+0x10)
 if area:events.append((3,area,bytes(s)))
 events.append((4,BASE,bytes(s)))
 return events,bytes(s)
def main():
 raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
 for _,va,vs,offset,size in pe_sections(raw):
  if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
 uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;events=[];index=0;args=();snapshots=0
 def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
 def hook(machine,address,size,user):
  nonlocal index,snapshots
  if address not in (0xc00ae0,0xc038d0,0xbfea14,0x99a2e0):return
  kind={0xc00ae0:1,0xc038d0:2,0xbfea14:3,0x99a2e0:4}[address];sp=machine.reg_read(UC_X86_REG_ESP)
  arg=machine.reg_read(UC_X86_REG_ECX)if kind==4 else read(sp+4)
  s=bytes(machine.mem_read(BASE,WIDTH));assert index<len(events) and (kind,arg,s)==events[index],(args,index,kind,'boundary/state mismatch')
  index+=1;snapshots+=1
  if kind in (1,2):
   after=bytearray(s);mutate(after,args[3]);machine.mem_write(BASE,bytes(after));machine.reg_write(UC_X86_REG_EAX,args[4])
  machine.reg_write(UC_X86_REG_EIP,read(sp));machine.reg_write(UC_X86_REG_ESP,sp+4)
 uc.hook_add(UC_HOOK_CODE,hook);cases=0
 for args in itertools.product((0,1),(0,1),(0,1),range(4),(0,0xffffffff)):
  events,after=model(args);index=0;uc.mem_write(BASE,bytes(initial(*args)));uc.mem_write(stack,struct.pack('<I',stop));uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE)
  uc.emu_start(0xa3acc0,stop,count=10000)
  assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4 and index==len(events) and bytes(uc.mem_read(BASE,WIDTH))==after,args
  snapshots+=1;cases+=1
 report=dict(cases=cases,snapshots=snapshots,full_region_bytes=WIDTH,boundary='retail instructions versus independent model; stream-end/free/base-destructor boundaries recorded, no actual zlib or workspace reclamation; callbacks replace/clear workspace or mutate stream/flags, ignored stream-end result')
 (ROOT/'work/file_installer_constructor/compressor-destructor-retail.json').write_text(json.dumps(report,indent=2)+'\n')
 print('INSTALLER_COMPRESSOR_DTOR_RETAIL_OK cases='+str(cases)+' snapshots='+str(snapshots))
if __name__=='__main__':main()

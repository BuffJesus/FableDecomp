"""Execute actual display event, package copier and circular reader instructions."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
def cases():
    dims=[0,1,255,256,0x8000,0xffff,1280,1920]
    return [(w,h,d) for w in dims for h in dims for d in [0,1,16,24,32,255]]+[(1280,720,d) for d in range(256)]
def execute(codes,width,height,depth,length,pattern):
    obj,event,component,stack,stop=0x2000040,0x2100040,0x12345678+pattern*0x1000,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in {a&~0xfff for a in codes}|{0x416000,0xbfe000,0x139c000,0x2000000,0x2100000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,data in codes.items():u.mem_write(a,bytes(data))
    initial=bytearray(bytes([pattern*71])*0x218);struct.pack_into('<I',initial,0x48,component);u.mem_write(obj-0x40,bytes(initial))
    data=bytearray(bytes([pattern*71])*40);struct.pack_into('<HHB',data,5,width,height,depth);data[37]=length;u.mem_write(event,bytes(data));u.mem_write(0x139c8a8,struct.pack('<I',0x1234abcd));u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,obj)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    reads=[];trace=[]
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
        if pc==0x9f18f0:
            index=len(reads);assert index<3;size,start=[(2,0),(2,2),(1,4)][index];assert get(sp+4)==size and get(this+32)==length-start and get(this+36)==start and get(this+40)==0
            assert bytes(uc.mem_read(this,length))==bytes(data[5:5+length]);reads.append((size,start));return
        elif pc==0x41641f:
            assert reads==[(2,0),(2,2),(1,4)];trace.append((this,get(sp+4),get(sp+8),get(sp+12)&255));pop=12
        elif pc==0xbfe9f9:assert this==0x1234abcd;pop=0
        else:return
        uc.reg_write(UC_X86_REG_EAX,0xabababab);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x85caf0,stop,count=1000)
    assert trace==[(component,width,height,depth)] and bytes(u.mem_read(obj-0x40,len(initial)))==initial and bytes(u.mem_read(event,len(data)))==data
    assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved))
    return trace,reads

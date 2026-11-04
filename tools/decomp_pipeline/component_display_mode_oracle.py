"""Record original component display switching, including manager replacement."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from world_local_display_oracle import cases as event_cases
def cases():return event_cases()+[(0xffffffff,0x80000000,0),(0x80000000,0xffffffff,255),(0x12345678,0x87654321,32),(0x10000,0x10001,16)]
def execute(codes,width,height,depth,status,rebind,composed):
    component,world,event,engine,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x3008000,0x4000000;managers=[0x2400040,0x2500040,0x2600040]
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in {a&~0xfff for a in codes}|{0x415000,0x434000,0x9bf000,0x9f2000,0x9e3000,0x13b8000,0x139c000,0xbfe000,stop}|{a&~0xfff for a in [component,world,event,engine]+managers}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,data in codes.items():u.mem_write(a,bytes(data))
    for a in [0x434a10,0x9bff40,0x9f2f90,0x9f2f60,0x9e3820,0x415bf0,0x416365,0xbfe9f9]:u.mem_write(a,b'\xc3')
    regions={a&~0xfff:bytearray(b'\xa9'*0x400) for a in [component,world,event,engine]+managers};struct.pack_into('<I',regions[0x2000000],0x68,engine);struct.pack_into('<I',regions[0x2100000],0x48,component)
    data=regions[0x2200000];struct.pack_into('<HHB',data,0x45,width&0xffff,height&0xffff,depth);data[0x65]=5
    for base,data in regions.items():u.mem_write(base,bytes(data))
    u.mem_write(0x13b8390,struct.pack('<I',managers[0]));u.mem_write(0x139c8a8,struct.pack('<I',0x1234abcd));u.mem_write(stack,struct.pack('<II',stop,event) if composed else struct.pack('<4I',stop,width,height,depth));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world if composed else component)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[];cached=managers[1] if rebind&1 else managers[0];latest=managers[2] if rebind&2 else cached
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        if pc not in [0x434a10,0x9bff40,0x9f2f90,0x9f2f60,0x9e3820,0x415bf0,0x416365,0xbfe9f9]:return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);result=0xabababab
        if pc==0x434a10:trace.append(('pre',this));pop=0
        elif pc==0x9bff40:
            dimensions=get(sp+4);w,h=struct.unpack('<II',uc.mem_read(dimensions,8));trace.append(('set',this,w,h,get(sp+8),get(sp+12),get(sp+16)));result=status;pop=16
            u.mem_write(0x13b8390,struct.pack('<I',cached))
        elif pc==0x9f2f90:
            trace.append(('dimensions',this));out=get(sp+4);u.mem_write(out,struct.pack('<II',0x81234567,4444));result=out;pop=4;u.mem_write(0x13b8390,struct.pack('<I',latest))
        elif pc==0x9f2f60:trace.append(('format',this));out=get(sp+4);u.mem_write(out,struct.pack('<I',31));result=out;pop=4
        elif pc==0x9e3820:assert get(this)==31;trace.append(('depth',31));result=32;pop=0
        elif pc==0x415bf0:trace.append(('height',this));result=0x92345678;pop=0
        elif pc==0x416365:trace.append(('post',this,get(sp+4),get(sp+8),get(sp+12)));pop=12
        else:assert this==0x1234abcd;pop=0
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook)
    try:u.emu_start(0x85caf0 if composed else 0x41641f,stop,count=1000)
    except Exception as exc:raise AssertionError((hex(u.reg_read(UC_X86_REG_EIP)),trace,composed,hex(u.reg_read(UC_X86_REG_ESP)))) from exc
    assert trace==[('pre',engine),('set',managers[0],width&0xffff if composed else width,height&0xffff if composed else height,depth,0,0),('dimensions',cached+0x2c),('format',latest+0x2c),('depth',31),('height',cached+0x2c),('post',component,0x81234567,0x92345678,32)],trace
    assert get(0x13b8390)==latest and all(bytes(u.mem_read(base,len(data)))==data for base,data in regions.items())
    assert u.reg_read(UC_X86_REG_ESP)==stack+(8 if composed else 16) and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved))
    return trace

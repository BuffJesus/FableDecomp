"""Retail IME reset/default/font replay with complete receiver/global snapshots."""
import itertools
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EAX, UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_FPCW
from gate_game_component_abi import DEFAULT_RETAIL, retail_bytes

BASE=0x24000000
GLOBAL=0x13b7cd8

def collect():
    raw=DEFAULT_RETAIL.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32)
    uc.mem_map(0x400000,0x1000000);uc.mem_map(BASE,65536)
    for address,length in [(0x404940,10),(0x404950,17),(0x404a90,147),(0x4051c0,90)]:
        uc.mem_write(address,retail_bytes(raw,address,length))
    stack,stop=BASE+0x8000,BASE+0xf000
    inputs,expected=bytearray(),bytearray();mutation=current=events=0
    def word(address,value):uc.mem_write(address,struct.pack('<I',value))
    def read(address):return struct.unpack('<I',uc.mem_read(address,4))[0]
    def event(kind,value=0):
        nonlocal events
        expected.extend(struct.pack('<II',kind,value));expected.extend(uc.mem_read(BASE,4096));expected.extend(uc.mem_read(GLOBAL,84));events+=1
    def hook(uc,address,size,user):
        if address not in (0xcb2c60,0xcb5130,0xcb5180,0xbfe9bc):return
        sp=uc.reg_read(UC_X86_REG_ESP);extra=0
        if address==0xcb2c60:
            event(1,uc.reg_read(UC_X86_REG_ECX)&255)
            if mutation&1:word(current+4,BASE+0x740)
        elif address==0xcb5130:
            event(2,uc.reg_read(UC_X86_REG_ECX)&255)
            if mutation&2:word(current+0x1c,BASE+0x780)
        elif address==0xcb5180:
            event(3,uc.reg_read(UC_X86_REG_ECX));event(4,uc.reg_read(UC_X86_REG_EDX)&255)
            if mutation&4:
                uc.mem_write(current,b'\1');uc.mem_write(current+8,b'\1');uc.mem_write(current+0x50,bytes([uc.mem_read(current+0x50,1)[0]^1]))
            uc.reg_write(UC_X86_REG_EAX,mutation&1)
        else:
            event(5,read(sp+4))
            if mutation&4:word(current+0x4c,BASE+0x7c0)
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+extra)
    uc.hook_add(UC_HOOK_CODE,hook)
    def run(address,receiver,args=()):
        uc.mem_write(stack,struct.pack('<'+'I'*(len(args)+1),stop,*args))
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,receiver)
        uc.emu_start(address,stop,count=1000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4*(1+len(args))
    cases=0
    ints=[0,1,0xffffffff,0x7fffffff,0x80000000,0x1000001,0xfeffffff,123456789]
    specs=[(mode,mut,0,init,font,variant)for mode,mut,init,font,variant in itertools.product(range(3),range(8),range(2),range(2),range(2))]
    specs += [(3,0,rounding,height,x,y)for rounding,height,x,y in itertools.product(range(4),[0,0xffffffff],ints,ints)]
    for mode,mutation,rounding,p3,p4,p5 in specs:
        arena=bytearray((i*17+cases)%256 for i in range(4096));singleton=bytearray((i*7+cases)%256 for i in range(84))
        if mode!=3:
            for obj,offset in [(arena,0x100),(singleton,0)]:
                obj[offset]=p5;obj[offset+8]=p3;obj[offset+0x50]=p5
                struct.pack_into('<I',obj,offset+4,BASE+0x700)
                struct.pack_into('<I',obj,offset+0x1c,BASE+0x720 if p4 else 0)
        inputs.extend(struct.pack('<6I',mode,mutation,rounding,p3,p4,p5));inputs.extend(arena);inputs.extend(singleton)
        uc.mem_write(BASE,bytes(arena));uc.mem_write(GLOBAL,bytes(singleton))
        current=GLOBAL if mode==2 else BASE+0x100
        uc.reg_write(UC_X86_REG_FPCW,0x37f|(rounding<<10))
        if mode==0:run(0x404a90,0)
        elif mode==3:
            run(0x404940,BASE+0x200,(p3,));run(0x404950,BASE+0x200,(p4,p5))
        else:run(0x4051c0,current)
        event(6);cases+=1
    return inputs,expected,dict(cases=cases,snapshots=events,bytes_per_snapshot=4180)

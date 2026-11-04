"""Retail pool allocation and real block lifetimes; only heap APIs recorded."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x6000
def collect():
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000);uc.mem_map(BASE,0x10000)
    raw=EXE.read_bytes()
    for _,va,_,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    inputs,expected=bytearray(),bytearray();snapshots=news=aligned=mutation=0
    stack,stop=BASE+0xe000,BASE+0xf000
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def event(kind,a=0,b=0):
        nonlocal snapshots
        expected.extend(struct.pack('<3I',kind,a,b));expected.extend(uc.mem_read(BASE,WIDTH));snapshots+=1
    def hook(uc,a,n,user):
        nonlocal news,aligned
        if a not in (0xbfea1a,0xbfe9bc,0x9f2be0,0x9f2b20):return
        sp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);edx=uc.reg_read(UC_X86_REG_EDX)
        if a==0xbfea1a:
            assert read(sp+4)==24 and news<16
            event(1,24);result=BASE+0x800+news*0x20;news+=1
        elif a==0xbfe9bc:event(4,read(sp+4));result=0
        elif a==0x9f2be0:
            assert ecx<=0x500 and edx==16 and aligned<16
            event(2,ecx,edx);result=BASE+0x1000+aligned*0x500;aligned+=1
            if mutation==1:word(BASE+8,read(BASE+8)+16);word(BASE+12,64)
            elif mutation==2:word(BASE+0x14,0)
        else:
            event(3,ecx);result=0
            if mutation==1:word(BASE+8,read(BASE+8)^0x80)
            elif mutation==2:word(BASE+0x14,0)
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
    uc.hook_add(UC_HOOK_CODE,hook)
    patterns=[(0,1,15,16),(16,16,16,16),(1,65,17,128),(128,64,32,16),(17,33,65,129),(256,257,64,0)]
    scenarios=[(0,capacity,growth,pattern,mutation)for capacity,growth,pattern,mutation in itertools.product((0,16,64,128),(0,32,128),range(len(patterns)),range(3))]
    scenarios += [(1,size,owns,0,mutation)for size,owns,mutation in itertools.product((0,16,128,1024),range(2),range(3))]
    for cases,(kind,capacity,growth,pattern,mutation)in enumerate(scenarios):
        arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        put(4,0);put(8,capacity);put(12,growth);put(0x14,0)
        inputs.extend(struct.pack('<5I',kind,capacity,growth,pattern,mutation));inputs.extend(arena)
        uc.mem_write(BASE,bytes(arena));news=aligned=0
        def call(a,receiver=BASE,args=()):
            uc.mem_write(stack,struct.pack('<'+'I'*(len(args)+1),stop,*args));uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,receiver)
            uc.emu_start(a,stop,count=100000);assert uc.reg_read(UC_X86_REG_ESP)==stack+4+len(args)*4
            return uc.reg_read(UC_X86_REG_EAX)
        if kind==0:
            for size in patterns[pattern]:
                event(10,call(0xa5c910,args=(size,)));event(11,call(0xa5c5e0))
            call(0xa5c720);event(12);event(11,call(0xa5c5e0))
            event(10,call(0xa5c910,args=(32,)))
            for repeat in range(2):call(0xa5c720);event(12)
        else:
            news=1
            for mode in range(3):
                call(0xa5c670,BASE+0x800,(capacity,));event(13)
                uc.mem_write(BASE+0x80c,bytes((growth,)))
                if mode==0:call(0xa5c6e0,BASE+0x800);event(14)
                else:call(0xa5c9b0,BASE+0x800,(mode-1,));event(15)
    return inputs,expected,dict(cases=len(scenarios),snapshots=snapshots)

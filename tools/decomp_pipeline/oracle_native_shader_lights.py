"""Native-composition oracle for AllocateLights and its pool/vector providers."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0xc000
def collect():
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000);uc.mem_map(BASE,0x10000)
    raw=EXE.read_bytes()
    for _,va,_,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    inputs,expected=bytearray(),bytearray();snapshots=news=aligned=mallocs=old=count=0
    stack,stop=BASE+0xe000,BASE+0xf000
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def event(kind,a=0,b=0):
        nonlocal snapshots
        memory=bytearray(uc.mem_read(BASE,WIDTH));lights=struct.unpack_from('<I',memory,0x164)[0]
        if lights and lights!=BASE+0x1000:
            for i in range(old,count):
                offset=lights-BASE+i*56
                memory[offset:offset+48]=bytes(48);memory[offset+54:offset+56]=bytes(2)
        expected.extend(struct.pack('<3I',kind,a,b));expected.extend(memory);snapshots+=1
    def hook(uc,a,n,user):
        nonlocal news,aligned,mallocs
        if a not in (0xbfea1a,0xbfe9bc,0x9f2be0,0x9f2b20,0xbfea0e,0xbfea14):return
        sp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);edx=uc.reg_read(UC_X86_REG_EDX)
        if a==0xbfea1a:
            assert read(sp+4)==24 and news<8
            event(1,24);result=BASE+0x800+news*0x20;news+=1
        elif a==0xbfe9bc:event(4,read(sp+4));result=0
        elif a==0x9f2be0:
            assert ecx<=0x1000 and edx==16 and aligned<4
            event(2,ecx,edx);result=BASE+0x6000+aligned*0x1000;aligned+=1
        elif a==0x9f2b20:event(3,ecx);result=0
        elif a==0xbfea0e:
            size=read(sp+4);assert size<=0x1000 and mallocs<3
            event(5,size);result=BASE+0x2000+mallocs*0x1000;mallocs+=1
        else:event(6,read(sp+4));result=0
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
    uc.hook_add(UC_HOOK_CODE,hook);cases=0
    for old,count,size,variant in itertools.product((0,1,4,8),(0,1,3,4,5,8,9,16),(0,1,2,4,0x10000000,0x40000000,0xffffffff),range(3)):
        arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        put(0x164,BASE+0x1000);put(0x168,BASE+0x1c00);put(0x16c,old)
        put(0x170,old);put(0x174,(0,2,4)[variant])
        put(0x17c,0);put(0x180,(0,64,2048)[variant]);put(0x184,(64,128,1024)[variant]);put(0x18c,0)
        put(0x1a0,BASE+0x1800);put(0x1a4,BASE+0x1800+(0,2,8)[variant]*16);put(0x1a8,BASE+0x1800+(0,4,8)[variant]*16)
        inputs.extend(struct.pack('<3I',old,count,size));inputs.extend(arena);uc.mem_write(BASE,bytes(arena));news=aligned=mallocs=0
        uc.mem_write(stack,struct.pack('<III',stop,count,size));uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE)
        uc.emu_start(0x98b420,stop,count=100000);assert uc.reg_read(UC_X86_REG_ESP)==stack+12
        event(10);cases+=1
    return inputs,expected,dict(cases=cases,snapshots=snapshots)

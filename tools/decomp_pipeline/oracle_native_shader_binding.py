"""Retail vertex-shader binding, cache checks and COM callback mutations."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x6000

def collect():
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000);uc.mem_map(BASE,0x10000)
    raw=EXE.read_bytes()
    for _,va,_,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    inputs,expected=bytearray(),bytearray();snapshots=calls=mutation=0
    stack,stop=BASE+0xe000,BASE+0xf000
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def event(kind,receiver=0,value=0):
        nonlocal snapshots
        expected.extend(struct.pack('<3I',kind,receiver,value));expected.extend(uc.mem_read(BASE,WIDTH));snapshots+=1
    def hook(uc,a,n,user):
        nonlocal calls
        if a not in (BASE+0xc000,BASE+0xc100):return
        sp=uc.reg_read(UC_X86_REG_ESP);receiver=read(sp+4)
        assert receiver in (BASE+0x900,BASE+0x940)
        event(1 if a==BASE+0xc000 else 2,receiver,read(sp+8));calls+=1
        if mutation:
            word(BASE+0x1d4,BASE+(0x940 if calls&1 else 0x900))
            word(BASE+0x1dc,0xabcdef00+calls)
            word(BASE+0x1d8,0x12340000+calls)
            word(BASE+0x704,BASE+0x780)
            word(BASE+0x3908,calls&1)
            uc.mem_write(BASE+0x4cfd,bytes((calls&1,)))
        uc.reg_write(UC_X86_REG_EAX,0x80004005 if mutation else 0)
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+12)
    uc.hook_add(UC_HOOK_CODE,hook)
    word(BASE+0x8000+92*4,BASE+0xc000);word(BASE+0x8000+87*4,BASE+0xc100)
    cases=0
    for fixed,software,fixedsoftware,current,cache,force,mutation in itertools.product(range(2),range(2),range(2),(0,1,0xffffffff),range(4),range(2),range(2)):
        arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        put(0x1d0,BASE+0x1000);put(0x1d4,BASE+0x900)
        put(0x900,BASE+0x8000);put(0x940,BASE+0x8000)
        put(0x704,BASE+0x740);put(0x744,0x12345678);put(0x748,0 if cache==3 else 0x87654321)
        put(0x784,0x76543210);put(0x788,0xfedcba98)
        put(0x1d8,0x11111111 if cache==1 else 0 if cache==3 else 0x87654321)
        put(0x1dc,0x22222222 if cache==2 else 0x12345678)
        put(0x3908,current);arena[0x393:0x396]=bytes((software,fixedsoftware,fixed))
        inputs.extend(struct.pack('<II',mutation,force));inputs.extend(arena)
        uc.mem_write(BASE,bytes(arena));calls=0
        for repeat in range(2):
            uc.mem_write(stack,struct.pack('<III',stop,BASE+0x700,force))
            uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE)
            uc.emu_start(0x988020,stop,count=10000)
            assert uc.reg_read(UC_X86_REG_ESP)==stack+12
            event(3)
        cases+=1
    return inputs,expected,dict(cases=cases,snapshots=snapshots)

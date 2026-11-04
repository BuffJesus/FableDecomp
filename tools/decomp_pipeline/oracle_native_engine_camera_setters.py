"""Retail camera/window forwarding, with recorded Process and setter boundaries."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections
BASE=0x24000000
GLOBAL=0x1436ea0
SIZE=2048

def collect(composed):
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000);u.mem_map(BASE,0x10000)
    raw=EXE.read_bytes()
    for _,va,_,off,n in pe_sections(raw):
        if n:u.mem_write(0x400000+va,raw[off:off+n])
    inputs=bytearray();expected=bytearray();cases=snapshots=0
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def event(kind,self=0,arg=0):
        nonlocal snapshots
        expected.extend(struct.pack('<4I',kind,self,arg,get(GLOBAL)))
        expected.extend(u.mem_read(BASE,SIZE));snapshots+=1
    def hook(u,a,n,_):
        if a==0xb314e0:kind=2
        elif not composed and a in (0xb2fbf0,0xb2fc00):kind=1 if a==0xb2fbf0 else 3
        else:return
        sp=u.reg_read(UC_X86_REG_ESP);self=u.reg_read(UC_X86_REG_ECX);arg=get(sp+4)
        event(kind,self,arg)
        if kind==2:
            assert arg==1
            if mode&2:put(BASE+0x14,0xabcdef01);put(self+0x10,0x76543210);put(GLOBAL,BASE+0x500)
        else:
            put(self+(12 if kind==1 else 16),arg)
            if mode&1:put(GLOBAL,BASE+0x500);put(BASE+0x14,0x12345678)
        u.reg_write(UC_X86_REG_EAX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0xcafebabe);u.reg_write(UC_X86_REG_ECX,0x87654321)
        u.reg_write(UC_X86_REG_ESP,sp+8);u.reg_write(UC_X86_REG_EIP,get(sp))
    u.hook_add(UC_HOOK_CODE,hook)
    def call(address,self,arg):
        sp=BASE+0xe000;stop=BASE+0xf000
        u.mem_write(sp,struct.pack('<2I',stop,arg));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,self)
        saved=[(UC_X86_REG_EBX,0x11223344),(UC_X86_REG_ESI,0x55667788),(UC_X86_REG_EDI,0xaabbccdd),(UC_X86_REG_EBP,0x87654321)]
        for reg,value in saved:u.reg_write(reg,value)
        u.emu_start(address,stop,count=200)
        assert u.reg_read(UC_X86_REG_ESP)==sp+8
        assert all(u.reg_read(reg)==value for reg,value in saved)
    for fill,mode,owner,arg in itertools.product(range(256),range(4),(BASE+0x200,BASE),(0,BASE+0x700,0xffffffff)):
        arena=bytes([fill])*SIZE;u.mem_write(BASE,arena);put(GLOBAL,owner)
        inputs.extend(struct.pack('<3I',mode,owner,arg));inputs.extend(arena)
        call(0xb23b50,BASE,arg);event(4)
        call(0xb23b80,BASE,arg);event(5)
        if composed:
            call(0xb2fbf0,BASE+0x500,arg);event(6)
            call(0xb2fc00,BASE+0x500,arg);event(7)
        cases+=1
    return inputs,expected,dict(cases=cases,snapshots=snapshots)

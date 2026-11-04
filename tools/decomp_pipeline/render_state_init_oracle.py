"""Native/candidate initialization with bounded state arrays and callee mutations."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0xa07900
GLOBAL=0x13cb508
SETUP=0xa04630
DEFAULT=0xa059e0
FORCE=0xa044e0
OBJECT=0x2000000
STATES=OBJECT+0x4000
STACK=0x3000000
STOP=0x2200000

def execute(code,count,duplicate,mutation):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in [(0xa04000,0x4000),(0x13cb000,4096),(OBJECT,0x8000),(STACK,0x10000),(STOP,4096)]:u.mem_map(a,n)
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    u.mem_write(ENTRY,bytes(code));u.mem_write(OBJECT,b'\xa5'*0x3a3c);u.mem_write(STATES,b'\xb7'*(256*20))
    for i in range(256):put(OBJECT+0x2408+4*i,STATES+(0 if duplicate else i)*20)
    put(OBJECT+0x2810,count);events=[];returned=False
    def hook(u,a,size,data):
        nonlocal returned
        if a==STOP:returned=True;u.emu_stop();return
        if a not in [SETUP,DEFAULT,FORCE]:return
        assert u.reg_read(UC_X86_REG_ECX)==OBJECT
        events.append((a,bytes(u.mem_read(OBJECT,0x3a3c)),bytes(u.mem_read(STATES,256*20)),get(GLOBAL)))
        if a==SETUP:
            if mutation&1:put(OBJECT+0x2810,7)
            if mutation&2:put(GLOBAL,0)
        elif a==DEFAULT:
            if mutation&4:put(OBJECT+0x2814,0x20)
        elif mutation&8:put(OBJECT+0x280c,13)
        sp=u.reg_read(UC_X86_REG_ESP);u.reg_write(UC_X86_REG_EIP,get(sp));u.reg_write(UC_X86_REG_ESP,sp+4)
        u.reg_write(UC_X86_REG_EAX,0xdead0000);u.reg_write(UC_X86_REG_EDX,0xdead0002)
        # Retail DEFAULT has no calls and preserves ECX (sole write: lea ecx,[ecx]).
        if a!=DEFAULT:u.reg_write(UC_X86_REG_ECX,0xdead0001)
    u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,0x12340000)
    u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJECT)
    preserved=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
    for i,r in enumerate(preserved):u.reg_write(r,0xabcd0000+i)
    u.emu_start(ENTRY,STOP+1,count=10000)
    assert returned and u.reg_read(UC_X86_REG_ESP)==sp+8
    for i,r in enumerate(preserved):assert u.reg_read(r)==0xabcd0000+i
    assert [e[0] for e in events]==[SETUP,DEFAULT,FORCE]
    return events,bytes(u.mem_read(OBJECT,0x3a3c)),bytes(u.mem_read(STATES,256*20)),get(GLOBAL)

"""Compare compiled inline assignment with the original donor specialization."""
import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
HEAP=0x5000000;DEST=HEAP;SOURCE=HEAP+4;OBJS=(0,HEAP+0x100,HEAP+0x200,HEAP+0x300)
STACK=0x6000000;STOP=0x7000000;DELETE=HEAP+0x1000;VTABLE=HEAP+0x2000
GET=0x17a6395;RELEASE=0x180937d;LOCK=0x179f199;DONOR=0x1da50d0;CANDIDATE=0x2000000

def execute(code,address,counts,dest,source,mode,donor):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    pages={address&~0xfff,GET&~0xfff,RELEASE&~0xfff,LOCK&~0xfff}
    for p in pages:u.mem_map(p,0x1000)
    for p,n in [(HEAP,0x4000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(p,n)
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    u.mem_write(address,bytes(code));u.mem_write(HEAP,b'\x5a'*0x4000)
    put(DEST,OBJS[dest]);put(SOURCE,OBJS[source]);put(VTABLE+4,DELETE)
    for obj,count in zip(OBJS[1:],counts):put(obj,VTABLE);put(obj+4,count)
    for a in (GET,RELEASE,LOCK,DELETE):u.mem_write(a,b'\xc3')
    trace=[]
    def deletion(obj):
        trace.append((obj,get(obj+4),get(DEST),get(SOURCE)))
        if mode==1:put(SOURCE,0)
        if mode==2:put(SOURCE,OBJS[3])
        if mode==3:put(DEST,OBJS[3])
    def hook(uc,a,n,user):
        if a not in (GET,RELEASE,LOCK,DELETE):return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);eax=0xbad0bad0
        if a==GET:eax=get(this)
        elif a==LOCK:put(this+4,get(this+4)+1)
        elif a==DELETE:deletion(this)
        else:
            obj=get(this)
            if obj:
                put(obj+4,get(obj+4)-1)
                if get(obj+4)==0:deletion(obj)
                put(this,0)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
        uc.reg_write(UC_X86_REG_EAX,eax);uc.reg_write(UC_X86_REG_ECX,0xbad1bad1);uc.reg_write(UC_X86_REG_EDX,0xbad2bad2)
    u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
    if donor:put(sp+4,SOURCE)
    u.reg_write(UC_X86_REG_ECX,DEST);u.reg_write(UC_X86_REG_EDX,SOURCE);u.reg_write(UC_X86_REG_ESP,sp)
    regs=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for i,r in enumerate(regs):u.reg_write(r,0x123400+i)
    u.emu_start(address,STOP,count=1000)
    assert u.reg_read(UC_X86_REG_ESP)==sp+4+(4 if donor else 0)
    assert all(u.reg_read(r)==0x123400+i for i,r in enumerate(regs))
    assert u.reg_read(UC_X86_REG_EAX)==DEST
    for obj in OBJS[1:]:assert bytes(u.mem_read(obj+8,0x14))==b'\x5a'*0x14
    return get(DEST),get(SOURCE),tuple(get(obj+4) for obj in OBJS[1:]),trace

def compare(candidate,donor):
    cases=0
    for counts in itertools.product((0,1,2,0xffffffff),repeat=3):
        for dest,source,mode in itertools.product(range(4),range(4),range(4)):
            assert execute(candidate,CANDIDATE,counts,dest,source,mode,False)==execute(donor,DONOR,counts,dest,source,mode,True),(counts,dest,source,mode)
            cases+=1
    return cases

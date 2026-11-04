"""Compare the actual x86 providers against retail, then check independent stream expectations."""
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *
OBJ=0x2000000; SOURCE=OBJ+0x100; DATA=OBJ+0x1000; VTABLE=OBJ+0x2000
STACK=0x3000000; STOP=0x4000000; WRITE=0x99a300; BASE=0x99a2f0

def run(code,address,start,length,capacity,cursor):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,size in [(address&~0xfff,0x2000),(0x99a000,0x1000),(OBJ,0x4000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,size)
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    u.mem_write(address,bytes(code));u.mem_write(OBJ,b'\xa5'*0x40)
    u.mem_write(SOURCE,b'\x5a'*0x40)
    put(SOURCE+8,start);put(SOURCE+12,start+length);put(SOURCE+16,start+capacity);put(SOURCE+20,cursor)
    before=bytes(u.mem_read(SOURCE,0x40));put(OBJ,VTABLE);put(VTABLE+16,WRITE)
    u.mem_write(BASE,b'\xc3');u.mem_write(WRITE,b'\xc3');trace=[]
    def hook(uc,a,s,user):
        if a not in (BASE,WRITE):return
        sp=uc.reg_read(UC_X86_REG_ESP);self=uc.reg_read(UC_X86_REG_ECX)
        if a==BASE:trace.append(('base',self));pop=4
        else:trace.append(('write',self,get(sp+4),get(sp+8)));pop=12
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+pop)
        # Volatile registers may be destroyed by the called boundary.
        for reg in (UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX):uc.reg_write(reg,0xbad0bad0)
    u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,SOURCE)
    regs=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
    u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(address,STOP,count=1000)
    assert u.reg_read(UC_X86_REG_ESP)==sp+8
    assert all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs))
    assert bytes(u.mem_read(SOURCE,0x40))==before
    return trace,tuple(get(OBJ+4*i) for i in range(8)),u.reg_read(UC_X86_REG_EAX)

def compare_streams(code):
    cases=0
    for address in (0x995ee0,0x995eb0):
        pair=code['%08x'%address]
        for start in (0,DATA,DATA+3):
            for length in (0,1,2,15,64,0x7fffffff,0x80000000,0xffffffff):
                for extra in (0,19):
                    for cursor in (0,7,0xffffffff):
                        result=run(pair[0],address,start,length,length+extra,cursor)
                        assert result==run(pair[1],address,start,length,length+extra,cursor),(hex(address),start,length,cursor)
                        trace,fields,eax=result;ptr=start if length else SOURCE+8
                        if address==0x995ee0:
                            assert trace==[('base',OBJ)] and eax==OBJ
                            assert fields==(0x123675c,0,length,ptr,0,length,ptr,0xa5a5a5a5),fields
                        else:
                            assert trace==[('write',OBJ,ptr,length)],trace
                            assert fields==(VTABLE,)+(0xa5a5a5a5,)*7
                        cases+=1
    return cases

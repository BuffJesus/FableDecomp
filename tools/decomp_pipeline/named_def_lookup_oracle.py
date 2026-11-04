import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
MANAGER=0x2000000;TABLE=MANAGER+0x1040;ALTERNATE=MANAGER+0x2040;INFO=MANAGER+0x3000;DEFS=(0,MANAGER+0x3100,MANAGER+0x3200);RESULT=MANAGER+0x3400
NAME=MANAGER+0x3500;LOOKUP=0x9ad410;LOAD=0x9ad6e0;STACK=0x3000000;STOP=0x4000000
def execute(code,address,index,selected,count,mode):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(0x9ad000,0x1000);u.mem_map(MANAGER,0x4000);u.mem_map(STACK,0x10000);u.mem_map(STOP,0x1000)
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    u.mem_write(address,bytes(code));u.mem_write(LOAD,b'\xc3');u.mem_write(LOOKUP,b'\xc3');u.mem_write(MANAGER,b'\xa5'*0x4000)
    put(MANAGER+0xa4,TABLE)
    for i,p in enumerate(DEFS):put(INFO+i*0x20+12,p)
    for p in DEFS[1:]:put(p+4,count)
    old=(selected+1)%3
    for i in range(-4,8):put(TABLE+i*4,INFO+(selected if mode==0 else old)*0x20);put(ALTERNATE+i*4,INFO+selected*0x20)
    before=bytes(u.mem_read(MANAGER,0x4000));expected=bytearray(before)
    if index>0 and mode==1:struct.pack_into('<I',expected,0xa4,ALTERNATE)
    if index>0 and mode==2:struct.pack_into('<I',expected,TABLE-MANAGER+index*4,INFO+selected*0x20)
    struct.pack_into('<I',expected,RESULT-MANAGER,DEFS[selected] if index>0 else 0)
    if index>0 and selected:struct.pack_into('<I',expected,DEFS[selected]-MANAGER+4,(count+1)&0xffffffff)
    trace=[]
    def hook(uc,a,n,user):
        if a==LOOKUP:
            sp=uc.reg_read(UC_X86_REG_ESP);assert uc.reg_read(UC_X86_REG_ECX)==MANAGER and get(sp+4)==NAME
            trace.append(('name',MANAGER,NAME))
            uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+8);uc.reg_write(UC_X86_REG_EAX,index&0xffffffff)
            uc.reg_write(UC_X86_REG_ECX,0xbad0bad0);uc.reg_write(UC_X86_REG_EDX,0xbad0bad0)
            return
        if a!=LOAD:return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);arg=get(sp+4);trace.append(('load',this,arg))
        assert this==MANAGER and arg==index&0xffffffff
        if mode==1:put(MANAGER+0xa4,ALTERNATE)
        if mode==2:put(TABLE+index*4,INFO+selected*0x20)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
        for r in (UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX):uc.reg_write(r,0xbad0bad0)
    u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,RESULT);put(sp+8,NAME)
    u.reg_write(UC_X86_REG_ECX,MANAGER);u.reg_write(UC_X86_REG_ESP,sp)
    regs=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for i,r in enumerate(regs):u.reg_write(r,0x123400+i)
    u.emu_start(address,STOP,count=500)
    assert u.reg_read(UC_X86_REG_ESP)==sp+12 and u.reg_read(UC_X86_REG_EAX)==RESULT
    assert all(u.reg_read(r)==0x123400+i for i,r in enumerate(regs))
    assert len(trace)==(2 if index>0 else 1)
    assert bytes(u.mem_read(MANAGER,0x4000))==bytes(expected)
    return get(RESULT),tuple(get(p+4) for p in DEFS[1:]),trace
def compare(candidate,native):
    cases=0
    for address in candidate:
        for index,selected,count,mode in itertools.product((-1,0,1,3,7,-2147483648),range(3),(0,1,2,0xffffffff),range(3)):
            assert execute(candidate[address],address,index,selected,count,mode)==execute(native[address],address,index,selected,count,mode)
            cases+=1
    return cases

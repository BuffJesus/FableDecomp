"""Retail simple-pool free-list allocation, reset and accounting boundaries."""
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
    inputs,expected=bytearray(),bytearray();snapshots=created=mutation=0
    stack,stop=BASE+0xe000,BASE+0xf000
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def event(kind,a=0,b=0):
        nonlocal snapshots
        expected.extend(struct.pack('<3I',kind,a,b));expected.extend(uc.mem_read(BASE,WIDTH));snapshots+=1
    def hook(uc,a,n,user):
        nonlocal created
        if a not in (0xa5c890,BASE+0xc000):return
        sp=uc.reg_read(UC_X86_REG_ESP);receiver=uc.reg_read(UC_X86_REG_ECX);value=read(sp+4)
        if a==0xa5c890:
            assert receiver==BASE and value<=0x800 and created<3
            event(1,value);area=BASE+0x3000+created*0x900;block=BASE+0x900+created*0x20;created+=1
            word(area,value);word(area+4,read(BASE+0x14));word(area+8,97);word(BASE+0x14,area)
            word(block,BASE+0x8000);word(block+4,1);word(block+8,area)
            uc.mem_write(block+12,b'\1');word(block+16,value+16);word(block+20,read(BASE+4));word(BASE+4,block)
            if mutation:word(BASE+8,0x180);word(BASE+12,0x240)
        else:
            assert BASE+0x800<=receiver<BASE+0xa00 and value==1
            event(2,receiver,value)
            if mutation:word(receiver+20,0);word(BASE+4,BASE+0x900);word(BASE+0x14,BASE+0x1800)
        uc.reg_write(UC_X86_REG_EAX,receiver);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
    uc.hook_add(UC_HOOK_CODE,hook);word(BASE+0x8000,BASE+0xc000)
    scenarios=[]
    for areas,request,existing,mutation in itertools.product(range(4),(0,1,15,16,17,32,48,64,65,128,257,0xfffffff1,0xffffffff),range(2),range(2)):
        scenarios.append((0,areas,request,existing,mutation))
    for blocks,owns,mutation in itertools.product(range(5),range(2),range(2)):scenarios.append((1,blocks,owns,0,mutation))
    for blocks,overflow in itertools.product(range(5),range(2)):scenarios.append((2,blocks,overflow,0,0))
    for value in (0,1,15,16,17,1023,1024,0x7fffffff,0xfffffff0,0xfffffff1,0xffffffff):scenarios.append((3,0,value,0,0))
    for cases,(kind,count,value,existing,mutation)in enumerate(scenarios):
        arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        blocks=count if kind in (1,2)else existing
        put(4,BASE+0x800 if blocks else 0);put(8,0x80);put(12,0x100)
        put(0x14,BASE+0x1000 if kind==0 and count else BASE+0x1800 if kind==1 else 0)
        for i in range(blocks):
            o=0x800+i*0x20;put(o,BASE+0x8000);put(o+4,1);put(o+8,BASE+0x2000+i*0x100)
            arena[o+12]=value if kind==1 else 1;put(o+16,0xfffffff0 if kind==2 and value else 0x80+i*16)
            put(o+20,BASE+o+0x20 if i+1<blocks else 0)
        for i in range(count if kind==0 else 0):
            o=0x1000+i*0x200;put(o,(16,64,128)[i]);put(o+4,BASE+o+0x200 if i+1<count else 0);put(o+8,97)
        inputs.extend(struct.pack('<3I',kind,value,mutation));inputs.extend(arena);uc.mem_write(BASE,bytes(arena));created=0
        def call(a,args=()):
            uc.mem_write(stack,struct.pack('<'+'I'*(len(args)+1),stop,*args));uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE)
            uc.emu_start(a,stop,count=100000);assert uc.reg_read(UC_X86_REG_ESP)==stack+4+len(args)*4
            return uc.reg_read(UC_X86_REG_EAX)
        if kind==0:
            for request in (value,16):event(10,call(0xa5c910,(request,)))
        elif kind==1:
            for repeat in range(2):call(0xa5c720);event(11)
        elif kind==3:call(0xa5c5c0,(value,));event(12)
        event(13,call(0xa5c5e0))
    return inputs,expected,dict(cases=len(scenarios),snapshots=snapshots)

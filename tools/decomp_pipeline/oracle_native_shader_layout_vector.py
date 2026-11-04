"""Retail shader layout registration and complete pointer-vector growth paths."""
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
    inputs,expected=bytearray(),bytearray();snapshots=allocations=0
    stack,stop=BASE+0xe000,BASE+0xf000
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def event(kind,value=0):
        nonlocal snapshots
        expected.extend(struct.pack('<II',kind,value));expected.extend(uc.mem_read(BASE,WIDTH));snapshots+=1
    def hook(uc,a,n,user):
        nonlocal allocations
        if a not in (0xbfea0e,0xbfea14,0xbfeae6):return
        sp=uc.reg_read(UC_X86_REG_ESP)
        if a==0xbfea0e:
            size=read(sp+4);assert size<=0x1000
            event(1,size);result=BASE+0x2000+allocations*0x1000;allocations+=1;assert allocations<=4
        elif a==0xbfea14:
            event(2,read(sp+4));result=0
        else:
            result=read(sp+4);size=read(sp+12)
            if size:uc.mem_write(result,bytes(uc.mem_read(read(sp+8),size)))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
    uc.hook_add(UC_HOOK_CODE,hook)
    scenarios=[]
    for size,spare in itertools.product((0,1,4,8),(0,4,12)):
        for count in (0,1,3,9):scenarios.append((0,size,spare,count,0,0,0));scenarios.append((1,size,spare,count,0,0,0))
        for pos,count,alias in itertools.product(sorted(set((0,size//2,size))),(0,1,2,5),(0,1)if size else(0,)):
            scenarios.append((2,size,spare,count,pos,alias,0))
        for pos,count,alias in itertools.product(sorted(set((0,size//2,size))),(1,3),(0,1)if size else(0,)):
            scenarios.append((3,size,spare,count,pos,alias,0))
            if pos==size:scenarios.append((3,size,spare,count,pos,alias,1))
    for cases,(kind,size,spare,count,pos,alias,at_end) in enumerate(scenarios):
        arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        start=BASE+0x1000 if size+spare else 0
        put(0xc,start);put(0x10,start+size*4);put(0x14,start+(size+spare)*4)
        put(0x800,0xfedcba98);put(4,0x12345678);put(8,0x654321)
        for i in range(size):put(0x1000+i*4,0x12340000+i*17)
        value_address=BASE+0x1000+(size//2)*4 if alias else BASE+0x800
        params=(kind,count,pos,value_address,at_end)
        inputs.extend(struct.pack('<5I',*params));inputs.extend(arena)
        uc.mem_write(BASE,bytes(arena));allocations=0
        def call(address,receiver,args):
            uc.mem_write(stack,struct.pack('<'+'I'*(len(args)+1),stop,*args))
            uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,receiver)
            uc.emu_start(address,stop,count=100000)
            assert uc.reg_read(UC_X86_REG_ESP)==stack+4+4*len(args)
        if kind==0:
            for index in (count,count+2):
                call(0x98b230,BASE,(index,read(BASE+0x800)));event(10)
                call(0x98b280,BASE,());event(11)
        elif kind==1:
            call(0x98d870,BASE+0xc,(count,value_address));event(12)
        elif kind==2:
            call(0x98d000,BASE+0xc,(read(BASE+0xc)+pos*4,count,value_address));event(13)
        else:
            call(0x98cde0,BASE+0xc,(read(BASE+0xc)+pos*4,value_address,BASE+0x804,count,at_end));event(14)
    return inputs,expected,dict(cases=len(scenarios),snapshots=snapshots)

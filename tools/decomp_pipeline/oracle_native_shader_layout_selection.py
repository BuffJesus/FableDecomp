"""Retail layout selection; light allocation, vector clear/resize are recorded."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x6000

def collect(native_vector=False):
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000);uc.mem_map(BASE,0x10000)
    raw=EXE.read_bytes()
    for _,va,_,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    inputs,expected=bytearray(),bytearray();snapshots=mutation=0
    stack,stop=BASE+0xe000,BASE+0xf000
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def event(kind,a=0,b=0,c=0,value=bytes(16)):
        nonlocal snapshots
        expected.extend(struct.pack('<4I',kind,a,b,c));expected.extend(value)
        expected.extend(uc.mem_read(BASE,WIDTH));snapshots+=1
    def hook(uc,a,n,user):
        if a not in ((0x98b420,0xa5c300,0xbfea0e,0xbfea14) if native_vector else (0x98b420,0xa5c300,0x98da90)):return
        sp=uc.reg_read(UC_X86_REG_ESP);receiver=uc.reg_read(UC_X86_REG_ECX)
        if a==0x98b420:
            count,size=read(sp+4),read(sp+8);assert receiver==BASE and count<=8 and size<=2
            event(1,count,size)
            word(BASE+0x164,BASE+0x3000);word(BASE+0x168,BASE+0x4000)
            word(BASE+0x16c,count+(1 if mutation==1 and count<4 else 0))
            word(BASE+0x170,8);word(BASE+0x174,4)
            if mutation==2:word(BASE+4,BASE+0x1600);word(BASE+0x3d8,read(BASE+0x3d8)^0x40000040)
            cleanup=8
        elif a==0xa5c300:
            target,count,value=read(sp+4),read(sp+8),bytes(uc.mem_read(read(sp+12),16))
            assert BASE+0x4000<=target<BASE+0x4800 and count<=32
            event(2,target,count,0,value)
            if count:uc.mem_write(target,value*count)
            if mutation==1:word(BASE+4,BASE+0x1600);word(BASE+0x3d8,read(BASE+0x3d8)^0x80)
            if mutation==2:word(BASE+0x198,read(BASE+0x194))
            cleanup=0
        elif native_vector:
            if a==0xbfea0e:
                size=read(sp+4);assert size<=0x800
                event(4,size);result=BASE+0x5000
            else:event(5,read(sp+4));result=0
            uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
            return
        else:
            count=read(sp+4);value=bytes(uc.mem_read(read(sp+8),16));assert receiver==BASE+0x194 and count<=8
            event(3,count,0,0,value)
            first,last=read(receiver),read(receiver+4);old=(last-first)//16
            copied=min(old,count)
            if copied:uc.mem_write(BASE+0x5000,bytes(uc.mem_read(first,copied*16)))
            if count>copied:uc.mem_write(BASE+0x5000+copied*16,value*(count-copied))
            word(receiver,BASE+0x5000);word(receiver+4,BASE+0x5000+count*16);word(receiver+8,BASE+0x5080)
            if mutation:word(BASE+4,BASE+0x1600);word(BASE+0x3d8,read(BASE+0x3d8)^0x40000000)
            cleanup=8
        uc.reg_write(UC_X86_REG_EAX,receiver);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
    uc.hook_add(UC_HOOK_CODE,hook);cases=0
    for index,current,difference,variant,mutation in itertools.product((-1,0,1),range(3),range(15),range(4),range(3)):
        arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        for layout in (0x1000,0x1300,0x1600):
            for off in (0x28,0x34,0x40,0x50,0x5c,0x74,0x80,0x8c):
                put(layout+off+4,off//4);put(layout+off+8,2)
            put(layout+0x4c,(0,2,2,0x80000000)[variant]);put(layout+0x58,4)
        offsets=(None,0x90,0x94,0x84,0x2c,0x38,0x44,0x4c,0x54,0x60,0x78)
        if difference==11:
            for off in (0x90,0x84,0x2c,0x38,0x44,0x54,0x60,0x78):put(0x1300+off,100+off)
            put(0x1364,4);put(0x1358,6)
        elif difference==12:put(0x1364,(0,1,4,6)[variant])
        elif difference==13:put(0x1358,6)
        elif difference==14:put(0x1358,0)
        elif difference:
            off=offsets[difference]
            put(0x1300+off,1 if off==0x4c else 0 if off==0x94 else 100+off)
        put(0x1660,0xfffffffe);put(0x1678,0xfffffffd)
        put(4,(0,BASE+0x1000,BASE+0x1300)[current]);put(8,7)
        put(0xc,BASE+0x800);put(0x10,BASE+0x808);put(0x14,BASE+0x808)
        put(0x800,BASE+0x1300);put(0x804,BASE+0x1300)
        put(0x164,BASE+0x3000);put(0x168,BASE+0x4000);put(0x16c,2)
        put(0x170,8 if variant==2 else 0);put(0x174,4 if variant==2 else 0)
        put(0x194,BASE+0x2800);put(0x198,BASE+0x2800+(0,2,6)[difference%3]*16);put(0x19c,BASE+0x2880)
        if native_vector:put(0x19c,BASE+0x2800+max((0,2,6)[difference%3],(0,2,6,8)[variant])*16)
        put(0x3d8,(0,0xffffffff,0x124802,0x7ffffff0)[variant])
        inputs.extend(struct.pack('<II',index&0xffffffff,mutation));inputs.extend(arena)
        uc.mem_write(BASE,bytes(arena));uc.mem_write(stack,struct.pack('<II',stop,index&0xffffffff))
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE)
        uc.emu_start(0x98b5e0,stop,count=100000)
        assert uc.reg_read(UC_X86_REG_ESP)==stack+8
        event(9);cases+=1
    return inputs,expected,dict(cases=cases,snapshots=snapshots)

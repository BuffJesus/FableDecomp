"""Retail nested capture restoration composed with state realization and UP draw."""
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
    inputs,expected=bytearray(),bytearray();snapshots=cases=calls=mutation=0
    stack,stop=BASE+0xe000,BASE+0xf000
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def event(kind,args=()):
        nonlocal snapshots
        expected.extend(struct.pack('<10I',kind,*args,*([0]*(9-len(args)))))
        expected.extend(uc.mem_read(BASE,WIDTH));snapshots+=1
    hooks={BASE+0xc000:(1,3),BASE+0xc100:(2,4),BASE+0xc200:(3,4),BASE+0xc300:(4,9)}
    def hook(uc,a,n,user):
        nonlocal calls
        if a not in hooks:return
        kind,count=hooks[a];sp=uc.reg_read(UC_X86_REG_ESP);args=tuple(read(sp+4+4*i)for i in range(count))
        event(kind,args);calls+=1
        if mutation and kind!=4:
            index=args[1 if kind==1 else 2]-100
            assert 0<=index<8,index
            word(BASE+0x5000+index*0x14+4,0x76540000+calls)
            word(BASE+4,BASE+(0x5840 if calls&1 else 0x5800))
            word(BASE+0x3cf0,BASE+(0x5840 if calls&1 else 0x5800))
        uc.reg_write(UC_X86_REG_EAX,0x80004005 if mutation else 0)
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+4*count)
    uc.hook_add(UC_HOOK_CODE,hook)
    for slot,hookaddr in [(57,0xc000),(67,0xc100),(69,0xc200),(84,0xc300)]:word(BASE+0x8000+slot*4,BASE+hookaddr)
    for count,dirty,equal,depth,mutation in itertools.product(range(9),range(3),range(2),(1,4,30),range(2)):
        arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        put(4,BASE+0x5800);put(0x3cf0,BASE+0x5800)
        put(0x5800,BASE+0x8000);put(0x5840,BASE+0x8000)
        put(0x2814,1<<depth);put(0x2808,2*(count+1));put(0x280c,0)
        arena[0x5411]=4
        for level in range(2):
            start=level*(count+1);put(8+start*8,BASE+0x5400)
            for i in range(count):
                put(8+(start+i+1)*8,BASE+0x5000+i*0x14)
                put(12+(start+i+1)*8,(0x1000 if level==0 else 0x2000)+i)
        pending=[]
        for i in range(8):
            o=0x5000+i*0x14
            put(o,(0x2000+i)if equal else 0x3000+i);put(o+4,0x4000+i)
            put(o+8,((1<<depth)|(1<<(depth-1)))if i<count else 0)
            put(o+12,100+i);arena[o+16]=int(dirty==2 or(dirty==1 and i%2==0))
            arena[o+17]=i%4;arena[o+18]=i
            if arena[o+16]:pending.append(BASE+o)
        put(0x280c,len(pending))
        for i,p in enumerate(pending):put(0x2008+4*i,p)
        inputs.extend(struct.pack('<I',mutation));inputs.extend(arena);uc.mem_write(BASE,bytes(arena));calls=0
        for repeat in range(2):
            for address,args in [(0xa05840,()),(0xa0af40,(BASE+0x5600,12+repeat*4,0 if count==0 else count+2,BASE+0x5700,count))]:
                uc.mem_write(stack,struct.pack('<'+'I'*(len(args)+1),stop,*args))
                uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE)
                uc.emu_start(address,stop,count=10000)
                assert uc.reg_read(UC_X86_REG_ESP)==stack+4+4*len(args)
                event(5 if address==0xa05840 else 6)
        cases+=1
    return inputs,expected,dict(cases=cases,snapshots=snapshots)

"""Execute retail AddText; record its string, queue-copy and growth boundaries."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_FPCW
from pe_oracle import EXE,pe_sections
BASE=0x24000000; WIDTH=0x6000

def collect():
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000);uc.mem_map(BASE,0x20000)
    raw=EXE.read_bytes()
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    inputs,expected=bytearray(),bytearray();events=calls=mutation=0
    stack,stop=BASE+0x18000,BASE+0x1f000
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def event(kind,value=0):
        nonlocal events
        expected.extend(struct.pack('<II',kind,value));expected.extend(uc.mem_read(BASE,WIDTH));events+=1
    def hook(uc,a,n,user):
        nonlocal calls
        if a not in (0x99aed0,0x99b7d0,0x99b510,0x9f9f70,0x9a4ec0,0x99b720,0xbfea0e,0xbfea14,BASE+0x1e000):return
        sp=uc.reg_read(UC_X86_REG_ESP);self=uc.reg_read(UC_X86_REG_ECX);cleanup=0
        if a==0x99aed0:word(self,0);event(1)
        elif a==0x99b7d0:
            assert read(sp+4)==BASE+0x840;word(self,read(BASE+0x840));event(2)
            if mutation==2:word(BASE+0x848,0xabcdefff);word(BASE+0x850,0x43800000)
            cleanup=4
        elif a==0x99b510:event(3)
        elif a==0x9f9f70:
            assert read(self)==0 and read(self+4)==0;event(4)
        elif a==0x9a4ec0:
            event(5);calls+=1
            if mutation==1:
                word(BASE+0x804,0x42000000+calls*0x10000);word(BASE+0x808,0x3f000000+calls*0x10000)
                word(BASE+0x294,640+calls*79);word(BASE+0x298,480+calls*53)
            uc.reg_write(UC_X86_REG_EAX,BASE+0x700)
        elif a==0x99b720:
            word(self,read(read(sp+4)));event(6,self);cleanup=4
            if mutation==2:word(BASE+0x828,0x43480000)
        elif a==BASE+0x1e000:
            assert read(sp+4)==BASE+0xa00;event(9,BASE+0xa00);word(BASE+0xa04,read(BASE+0xa04)+1)
            if mutation==1:word(BASE+0x834,0xfedcba98);word(BASE+0x838,BASE+0x980)
            cleanup=4
        elif a==0xbfea0e:
            event(7,read(sp+4));uc.reg_write(UC_X86_REG_EAX,BASE+0x5300)
        else:event(10,read(sp+4))
        if a not in (0x9a4ec0,0xbfea0e):uc.reg_write(UC_X86_REG_EAX,self)
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
    uc.hook_add(UC_HOOK_CODE,hook);cases=0
    for offscreen,queue,mutation,rounding,variant in itertools.product(range(2),range(3),range(3),range(4),range(4)):
        arena=bytearray((i*17+cases)%256 for i in range(WIDTH))
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        put(0x4a3c,BASE+0x100);arena[0x2d8]=offscreen
        struct.pack_into('<4f',arena,0x304,-1.5,2.25,1364.5,767.25)
        put(0x760,BASE+0x100);put(0x294,[640,1365,0,0xffffffff][variant]);put(0x298,[480,767,0xffffffff,0][variant])
        struct.pack_into('<3f',arena,0x800,-32.125+variant,63.75-variant,0.125)
        put(0x840,BASE+0x880);put(0x848,0x12345678);put(0x850,0x3f200001)
        put(0x4e94,BASE+0x5200);put(0x4e98,0 if queue==2 else BASE+0x5200)
        put(0x4e9c,BASE+(0x5200 if queue==1 else 0x52f0))
        inputs.extend(struct.pack('<4I',mutation,rounding,variant,0));inputs.extend(arena);uc.mem_write(BASE,bytes(arena))
        uc.mem_write(0x13961e8,struct.pack('<2d',128.125,256.25));calls=0
        uc.mem_write(stack,struct.pack('<7I',stop,BASE+0x800,0x3f200001,BASE+0x840,BASE+0x900,BASE+0x848,0xfedcba98))
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE+0x1000);uc.reg_write(UC_X86_REG_FPCW,0x37f|(rounding<<10))
        uc.emu_start(0x9db700,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+28
        assert calls==(0 if offscreen else 2)
        event(8);cases+=1
    word(BASE+0x16004,BASE+0x1e000)
    for mode,texture,mutation,rounding,variant in itertools.product([1,2],range(2),range(3),range(4),range(8)):
        arena=bytearray((i*23+cases)%256 for i in range(WIDTH))
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        put(0xa00,BASE+0x16000);put(0xa04,variant+1)
        put(0x814,BASE+0x880)
        at=0x818 if mode==1 else 0x800
        put(at,BASE+0xa00 if texture else 0);put(at+4,((variant&15)<<28)|(variant*7919))
        inputs.extend(struct.pack('<4I',mutation,rounding,variant,mode));inputs.extend(arena);uc.mem_write(BASE,bytes(arena));calls=0
        uc.mem_write(stack,struct.pack('<2I',stop,BASE+0x800));uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE+0x900)
        uc.reg_write(UC_X86_REG_FPCW,0x37f|(rounding<<10));uc.emu_start(0x9de890 if mode==1 else 0x9f9d00,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
        event(8);cases+=1
    for size,positionKind,fillLength,atEnd,alias,mutation in itertools.product([0,1,3],range(2),[0,1,3],range(2),range(2),range(3)):
        position=size if positionKind else 0
        if atEnd and position!=size:continue
        arena=bytearray((i*23+cases)%256 for i in range(WIDTH))
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        old=BASE+0x5200 if size else 0
        put(0x4e94,old);put(0x4e98,old+size*60);put(0x4e9c,old+size*60)
        for at in [0x800]+[0x5200+i*60 for i in range(size)]:
            put(at+0x14,BASE+0x880);put(at+0x18,0);put(at+0x1c,0)
        put(0x860,position);put(0x864,fillLength);put(0x868,atEnd);put(0x86c,old if alias and size else BASE+0x800)
        inputs.extend(struct.pack('<4I',mutation,0,0,3));inputs.extend(arena);uc.mem_write(BASE,bytes(arena));calls=0
        uc.mem_write(stack,struct.pack('<6I',stop,old+position*60,read(BASE+0x86c),BASE+0x870,fillLength,atEnd))
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE+0x4e94);uc.reg_write(UC_X86_REG_FPCW,0x37f)
        uc.emu_start(0x9e1750,stop,count=100000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+24
        event(8);cases+=1
    return inputs,expected,dict(cases=cases,snapshots=events,bytes_per_snapshot=WIDTH)

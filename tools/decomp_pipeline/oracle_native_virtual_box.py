import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX,UC_X86_REG_FPCW
from pe_oracle import EXE,pe_sections
BASE=0x24000000;WIDTH=0x5000
def collect():
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000);raw=EXE.read_bytes()
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(BASE,0x20000);stop=BASE+0x1f000;stack=BASE+0x18000
    inputs=bytearray();expected=bytearray();calls=mutation=sourceOffset=0;events=0
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def event(k,arg=0):
        nonlocal events
        expected.extend(struct.pack('<4I',k,arg,calls,0));expected.extend(uc.mem_read(BASE,WIDTH));events+=1
    def hook(uc,a,n,user):
        nonlocal calls
        if a not in (0x9a4ec0,0x9bf490):return
        sp=uc.reg_read(UC_X86_REG_ESP);cleanup=4
        if a==0x9a4ec0:
            event(1);calls+=1
            if mutation:
                word(BASE+0x160,BASE+(0x600 if calls&1 else 0x300));word(BASE+sourceOffset+(calls%4)*4,0x42800000+calls*0x10000)
            uc.reg_write(UC_X86_REG_EAX,BASE+0x100)
        else:
            uc.mem_write(BASE+0x900,bytes(uc.mem_read(read(sp+4),16)));event(2,uc.reg_read(UC_X86_REG_ECX));cleanup=8
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
    uc.hook_add(UC_HOOK_CODE,hook);uc.mem_write(0x13961e8,struct.pack('<2d',128,256));cases=0
    for mode in range(4):
        for variant,delta,mutation,rounding in itertools.product(range(8),[64]if mode==1 else [-8,-4,0,4,8,64],range(2),range(4)):
            arena=bytearray(WIDTH);sourceOffset=0x800;target=sourceOffset+delta
            def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
            put(0x160,BASE+0x300);put(0x494,256+variant*128);put(0x498,512+variant*256);put(0x794,512);put(0x798,1024);put(0x4a3c,BASE+0x600)
            struct.pack_into('<4f',arena,sourceOffset,-32.25+variant,16.125-variant,64.5+variant*2,96.75-variant)
            struct.pack_into('<2d',arena,0x4800,128.125+variant/7.,256.25+variant/9.)
            uc.mem_write(0x13961e8,bytes(arena[0x4800:0x4810]))
            inputs.extend(struct.pack('<5I',mode,mutation,rounding,sourceOffset,target));inputs.extend(arena)
            uc.mem_write(BASE,bytes(arena));calls=0;word(stack,stop);word(stack+4,BASE+sourceOffset)
            uc.reg_write(UC_X86_REG_FPCW,0x37f|(rounding<<10));uc.reg_write(UC_X86_REG_ESP,stack)
            uc.reg_write(UC_X86_REG_ECX,BASE+0x1000 if mode==1 else BASE+sourceOffset);uc.reg_write(UC_X86_REG_EDX,BASE+target)
            uc.emu_start({0:0x9e1e70,1:0xa0b560,2:0x9e21d0,3:0x9e2020}[mode],stop,count=10000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and calls==(2 if mode==2 else 4)
            event(3);cases+=1
    return inputs,expected,dict(cases=cases,snapshots=events,bytes_per_snapshot=WIDTH)

"""Execute retail worker bodies with recorded synchronization/I/O boundaries."""
import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x24000000

def collect():
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000);raw=EXE.read_bytes()
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(BASE,0x20000);stop=BASE+0x1f000;stack=BASE+0x18000
    inputs=bytearray();expected=bytearray();depth=cycles=0;mode=mutation=limit=0;events=0
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def event(k,a=0,b=0):
        nonlocal events
        expected.extend(struct.pack('<4I',k,a,b,depth));expected.extend(uc.mem_read(BASE,4096));events+=1
    def cycle():
        nonlocal cycles
        cycles+=1
        if cycles>=limit:uc.mem_write(BASE+0x7a,b'\1')
    def hook(uc,a,n,user):
        nonlocal depth
        if a not in (0xa62920,0xa62960,0xa62980,0xa629a0,BASE+0x1d000,BASE+0x1d100,0x9c3c20,0x9c4120,0x9c3fb0,0x9c53a0,0x9c4f20):return
        sp=uc.reg_read(UC_X86_REG_ESP);owner=uc.reg_read(UC_X86_REG_ECX);cleanup=4
        if a==0xa62920:
            assert read(sp+4)==BASE+4 and read(sp+8)==0
            event(1,read(sp+4),read(sp+8));word(owner,BASE+4);uc.mem_write(owner+4,b'\1');depth+=1;cleanup=12
        elif a in (0xa62960,0xa62980,0xa629a0):
            locked=uc.mem_read(owner+4,1)[0];event({0xa62960:2,0xa62980:3,0xa629a0:4}[a],locked)
            if a==0xa62980:
                if not locked:depth+=1;uc.mem_write(owner+4,b'\1')
            elif locked:depth-=1;uc.mem_write(owner+4,b'\0')
            if a==0xa62960 and mode==1 and mutation and not uc.mem_read(BASE+0x7a,1)[0]:word(BASE+0x54,BASE+0x500);word(BASE+0x30,0)
        elif a==BASE+0x1d000:
            handle,timeout,alertable=[read(sp+4+i*4)for i in range(3)]
            assert alertable==1 and depth==0
            event(5,handle,timeout)
            if mode==1 or timeout==100:cycle()
            elif handle==read(BASE+0x90):word(BASE+0x7c,0)
            else:
                word(BASE+0x300,BASE+0x340);word(BASE+0x58,0);word(BASE+0x5c,0);word(BASE+0x74,BASE+0x700)
            uc.reg_write(UC_X86_REG_EAX,0xc0);cleanup=16
        elif a==BASE+0x1d100:
            assert depth==0;event(6,read(sp+4));uc.reg_write(UC_X86_REG_EAX,1);cleanup=8
        else:
            assert owner==BASE
            if a==0x9c3c20:
                assert depth==0;event(7,read(sp+4));cycle();cleanup=8
            elif a==0x9c4120:
                assert depth==2;event(8,read(sp+4));cleanup=8
                if mutation:word(BASE+0x74,0x12345678)
            elif a==0x9c3fb0:assert depth==1;event(9,read(sp+4));cleanup=8
            elif a==0x9c53a0:assert depth==1;event(10)
            else:
                assert depth==1;event(11)
                if mutation:
                    uc.mem_write(BASE+0x60,bytes([not uc.mem_read(BASE+0x60,1)[0]]));word(BASE+0x34,cycles);word(BASE+0x68,3)
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
    uc.hook_add(UC_HOOK_CODE,hook);word(0x143fe08,BASE+0x1d000);word(0x143fde0,BASE+0x1d100)
    cases=0
    for mode,mask,mutation,limit in itertools.product(range(2),range(128),range(2),(1,3)):
        arena=bytearray(4096)
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        for o,v in [(0x38,BASE+0x300),(0x3c,BASE+0x320),(0x300,BASE+(0x300 if mask&1 else 0x340)),(0x320,BASE+(0x320 if mask&2 else 0x340)),
                    (0x54,0 if mask&4 else BASE+0x400),(0x30,[-1,0,1,2][(mask>>3)&3]),(0x7c,1 if mask&8 else 0),
                    (0x58,1 if mask&16 else 0),(0x5c,1 if mask&32 else 0),(0x34,1 if mask&16 else 0),(0x68,2),
                    (0x50,BASE+0x400 if mask&32 else 0),(0x84,0x1111),(0x88,0x2222),(0x8c,0x3333),(0x90,0x4444)]:put(o,v)
        for active in (0x400,0x500):
            put(active+0x58,mask&1);put(active+0x90,BASE+0x600);put(active+0x94,BASE+0x620)
        put(0x600,BASE+(0x600 if mask&2 else 0x640));put(0x620,BASE+(0x620 if mask&2 else 0x640))
        arena[0x60]=bool(mask&64);arena[0x7a]=mask==127
        inputs.extend(struct.pack('<3I',mode,mutation,limit));inputs.extend(arena)
        uc.mem_write(BASE,bytes(arena));depth=cycles=0;word(stack,stop)
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE)
        uc.emu_start(0x9c4230 if mode else 0x9c5690,stop,count=50000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and depth==0,(mode,mask,mutation,limit,depth)
        event(12);cases+=1
    return inputs,expected,dict(cases=cases,snapshots=events,arena_bytes=4096,boundary='recorded lock, wait, signal and processing calls; genuine worker loops and HasStreamInput')

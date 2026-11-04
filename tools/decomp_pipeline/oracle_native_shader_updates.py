"""Record retail shader binding/constant uploads and their complete arena state."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_FPCW
from pe_oracle import EXE,pe_sections
BASE=0x24000000
WIDTH=0x3000

def collect():
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000);uc.mem_map(BASE,0x10000)
    raw=EXE.read_bytes()
    for _,va,_,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    inputs,expected=bytearray(),bytearray();snapshots=calls=mutation=0
    stack,stop=BASE+0xe000,BASE+0xf000
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def event(kind,receiver=0,start=0,count=0,payload=b''):
        nonlocal snapshots
        expected.extend(struct.pack('<4I',kind,receiver,start,count));expected.extend(payload.ljust(128,b'\0'))
        expected.extend(uc.mem_read(BASE,WIDTH));snapshots+=1
    def hook(uc,a,n,user):
        nonlocal calls
        if a not in (BASE+0xc000,BASE+0xc100,BASE+0xc200):return
        sp=uc.reg_read(UC_X86_REG_ESP);receiver=read(sp+4)
        assert receiver in (BASE+0x800,BASE+0x840)
        if a in (BASE+0xc000,BASE+0xc200):
            count=read(sp+16);assert 0<count<=8
            event(1 if a==BASE+0xc000 else 3,receiver,read(sp+8),count,bytes(uc.mem_read(read(sp+12),count*16)));cleanup=16
        else:
            event(2,receiver,read(sp+8));cleanup=8
        calls+=1
        if mutation:
            word(BASE+0x3d8,0xa5a50000|calls*0x407)
            word(BASE+4,BASE+(0x1200 if calls&1 else 0x1000))
            word(BASE+0x1d4,BASE+(0x840 if calls&1 else 0x800))
            word(BASE+0x1e4,BASE+0x2040)
            word(BASE+0x194,BASE+0x2500)
            word(BASE+0x3cc,BASE+0x2700)
            word(BASE+0x1ac,0x3f000001+calls)
            word(BASE+0x1bc,0xbf000001+calls)
            uc.mem_write(BASE+0x1e8,b'\0')
            word(BASE+0x1f0,0xffc12345)
        uc.reg_write(UC_X86_REG_EAX,0x80004005 if mutation else 0)
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
    uc.hook_add(UC_HOOK_CODE,hook)
    word(BASE+0x8000+94*4,BASE+0xc000);word(BASE+0x8000+107*4,BASE+0xc100)
    word(BASE+0x8000+109*4,BASE+0xc200)
    cases=0
    for variant,count,mutation,rounding in itertools.product(range(4),(0,1,4,8),range(2),range(4)):
        arena=bytearray((i*17+cases*13)&255 for i in range(WIDTH))
        def put(o,v):struct.pack_into('<I',arena,o,v&0xffffffff)
        put(4,BASE+0x1000);put(0x1d4,BASE+0x800)
        put(0x800,BASE+0x8000);put(0x840,BASE+0x8000)
        put(0x1e4,0 if variant==0 else BASE+0x2000)
        put(0x2004,0x12345678);put(0x2044,0xabcdef01)
        put(0x200c,count);put(0x2010,BASE+0x2100)
        put(0x204c,count);put(0x2050,BASE+0x2140)
        put(0x3cc,BASE+0x2600)
        for j in range(8):
            put(0x2100+j*4,(j+variant)%8);put(0x2140+j*4,(7-j+variant)%8)
        put(0x194,BASE+0x2400);put(0x3d8,(0,1,0x20407,0xffffffff)[variant])
        arena[0x1e8]=(0,1,255,0)[variant]
        for offset in (0x1000,0x1200):
            put(offset+0x2c,(-1,0,95,255)[variant]);put(offset+0x30,count)
            put(offset+0x38,(-4,1,12,255)[variant]);put(offset+0x3c,count if offset==0x1000 else 1)
            put(offset+0x60,(-8,2,48,255)[variant]);put(offset+0x64,count if offset==0x1000 else 4)
        patterns=(0,0x80000000,0x3f800001,0xbf800001,0x7f800000,0xff800000,0x7fc12345,0x00800001)
        for j in range(8):put(0x1ac+j*4,patterns[(j+variant)%len(patterns)])
        params=(mutation,rounding,count,variant)
        inputs.extend(struct.pack('<4I',*params));inputs.extend(arena)
        uc.mem_write(BASE,bytes(arena));uc.reg_write(UC_X86_REG_FPCW,0x27f|(rounding<<10));calls=0
        for op in (6,5,0,1,2,3,4,5,6,1,2,3,4,5,6):
            address=(0x98b930,0x988a20,0x989760,0x9897c0,0x98a760,0x989bf0,0x988290)[op]
            uc.mem_write(stack,struct.pack('<4I',stop,(-1,0,95,255)[variant]&0xffffffff,BASE+0x2800,count))
            uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE)
            uc.emu_start(address,stop,count=10000)
            assert uc.reg_read(UC_X86_REG_ESP)==stack+(16 if op==0 else 4)
            event(10+op)
        cases+=1
    return inputs,expected,dict(cases=cases,snapshots=snapshots)

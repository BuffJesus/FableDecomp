"""Retail main allocation dispatch with native size-query/metadata composition."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
from oracle_main_allocation_size import BASE,WIDTH,TABLE,RANGE,VTABLE,QUERY
ALLOC=0x200d300;LOOKUP=BASE+0x4000;POINTER=BASE+0x6000
def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[];args=();depth=queries=0
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def snapshot():
        state=bytearray(uc.mem_read(BASE,WIDTH));state[0x2000:0x2000+840]=uc.mem_read(0x1396868,840);state[0x3000:0x3008]=uc.mem_read(0x13caaa4,8);return bytes(state)
    def hook(uc,address,size,user):
        nonlocal depth,queries
        if address not in [0x200d000,QUERY,0x200d200,ALLOC]:return
        sp=uc.reg_read(UC_X86_REG_ESP);kind={0x200d000:1,QUERY:2,0x200d200:3,ALLOC:4}[address]
        events.append((kind,uc.reg_read(UC_X86_REG_ECX)if kind in [2,4]else 0,read(sp+4)if kind in [2,4]else 0,depth,snapshot()))
        if kind==4:
            if args[5]:
                uc.mem_write(LOOKUP+args[8]*8+4,b'\x02');word(BASE+0x460+args[1]*4,BASE+0x5000);word(BASE+0x424+args[1]*4,999);word(BASE+0x40c,7);uc.mem_write(BASE+0x414,b'\x07');word(0x1396868+2*28+24,BASE+0x1400)
                word(0x13caaa4,0 if args[4]else POINTER);word(0x13caaa8,0 if args[4]else POINTER+8)
            uc.reg_write(UC_X86_REG_EAX,0 if args[3]else POINTER)
        elif kind==2:
            if args[5]:
                uc.mem_write(BASE+0x414,bytes([255 if queries==0 else 3]));row=2 if args[4]else 0;word(0x1396868+row*28+24,BASE+0x1500)
            uc.reg_write(UC_X86_REG_EAX,0 if queries==0 else 256);queries+=1
        else:
            assert read(sp+4)==0x13caa70;depth+=1 if kind==1 else -1
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
    uc.hook_add(UC_HOOK_CODE,hook);word(0x143fe0c,0x200d000);word(0x143fe10,0x200d200)
    for size,category,maximum,null,special,mutation,debug in itertools.product([0xffffffff,0,1,12,13,44,45,255,0x7fffffff,0x80000000],[0,9],[0,32,64],[0,1],[0,1],[0,1],[0,1]):
        requested=1 if size==0 or size>=0x80000000 else size;total=(requested+20)&0xffffffff;index=min(total,maximum+1);args=(size,category,maximum,null,special,mutation,debug,total,index)
        initial=bytearray(b'\xa5'*WIDTH)
        def put(state,a,v):struct.pack_into('<I',state,a-BASE,v&0xffffffff)
        put(initial,BASE+0x410,30);put(initial,BASE+0x40c,0xffffffff);initial[0x414]=255;initial[0x6000-1]=0
        put(initial,BASE+0x460+category*4,LOOKUP);put(initial,BASE+0x424+category*4,maximum)
        put(initial,RANGE,POINTER if special else 0);put(initial,RANGE+4,POINTER+8 if special else 0);put(initial,VTABLE,ALLOC);put(initial,VTABLE+12,QUERY)
        for row in range(30):put(initial,TABLE+row*28+24,BASE+0x1000)
        for context in [BASE+0x1000,BASE+0x1400,BASE+0x1500]:put(initial,context,VTABLE)
        for entry in range(maximum+2):
            context=BASE+0x1000+entry*16;put(initial,LOOKUP+entry*8,context);put(initial,context,VTABLE);initial[0x4000+entry*8+4]=1
        for tracking in [3,255]:put(initial,BASE+tracking*4,0xfffffff0)
        expected=bytearray(initial);predicted=[(4,BASE+0x1000+index*16,total,0,bytes(expected))]
        if mutation:
            expected[0x4000+index*8+4]=2;put(expected,BASE+0x460+category*4,BASE+0x5000);put(expected,BASE+0x424+category*4,999);put(expected,BASE+0x40c,7);expected[0x414]=7;put(expected,TABLE+2*28+24,BASE+0x1400)
            put(expected,RANGE,0 if special else POINTER);put(expected,RANGE+4,0 if special else POINTER+8)
        inside=bool(special)!=bool(mutation)
        if not null:
            if not inside:expected[0x6000-1]=2 if mutation else 1
            put(expected,BASE+0x40c,8 if mutation else 0);row=0 if inside else 2 if mutation else 1
            for query in range(2):
                context=BASE+0x1500 if mutation and query else BASE+0x1400 if mutation and row==2 else BASE+0x1000
                predicted.append((1,0,0,0,bytes(expected)));predicted.append((2,context,POINTER,1,bytes(expected)))
                if mutation:expected[0x414]=255 if query==0 else 3;put(expected,TABLE+row*28+24,BASE+0x1500)
                predicted.append((3,0,0,1,bytes(expected)))
            tracking=3 if mutation else 255
            for address,value in [(POINTER,0xffffffff),(POINTER+240,0xffffffff),(POINTER+244,tracking),(POINTER+248,requested),(BASE+tracking*4,0xfffffff0+requested)]:put(expected,address,value)
        depth=queries=0;events=[];uc.mem_write(BASE,bytes(initial));uc.mem_write(0x1396868,bytes(initial[0x2000:0x2000+840]));uc.mem_write(0x13caaa4,bytes(initial[0x3000:0x3008]))
        uc.mem_write(stack,struct.pack('<4I',stop,size,category,debug));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0x9f2a40,stop,count=10000)
        result=0 if null else POINTER+4
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+16 and uc.reg_read(UC_X86_REG_EAX)==result and depth==0
        assert events==predicted and snapshot()==expected,args
        rows.append(dict(args=(*args,result,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:]))
    return rows

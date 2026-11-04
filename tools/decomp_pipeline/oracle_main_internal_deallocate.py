"""Retail main free path seeded by independently verified allocation metadata."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
from oracle_main_allocation_size import BASE,WIDTH,TABLE,RANGE,VTABLE,QUERY
from oracle_main_internal_allocate import collect as allocations,POINTER
ALT=VTABLE+0x100;FREE=0x200d400;FREE_ALT=0x200d500
def collect():
    seeds=[row for row in allocations()if row['args'][1]==0 and row['args'][2]==32 and row['args'][3]==0 and row['args'][6]==0]
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];args=();events=[];depth=queries=0
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def snapshot():
        state=bytearray(uc.mem_read(BASE,WIDTH));state[0x2000:0x2000+840]=uc.mem_read(0x1396868,840);state[0x3000:0x3008]=uc.mem_read(0x13caaa4,8);return bytes(state)
    def hook(uc,address,size,user):
        nonlocal depth,queries
        if address not in [0x200d000,QUERY,0x200d200,FREE,FREE_ALT]:return
        kind={0x200d000:1,QUERY:2,0x200d200:3,FREE:5,FREE_ALT:6}[address];sp=uc.reg_read(UC_X86_REG_ESP)
        events.append((kind,uc.reg_read(UC_X86_REG_ECX)if kind in [2,5,6]else 0,read(sp+4)if kind in [2,5,6]else 0,depth,snapshot()))
        if kind==2:
            if args[1]==1 and queries==0:word(0x1396868+args[3]*28+24,BASE+0x1400);uc.mem_write(BASE+0x414,b'\x63')
            if args[1]==1 and queries==2:word(args[4],ALT)
            uc.reg_write(UC_X86_REG_EAX,[0,256,args[2]][queries]);queries+=1
        elif kind in [5,6]:
            if args[1]==1:word(BASE+0x40c,0);uc.mem_write(BASE+0x414,b'\x4d')
        else:assert read(sp+4)==0x13caa70;depth+=1 if kind==1 else -1
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
    uc.hook_add(UC_HOOK_CODE,hook);word(0x143fe0c,0x200d000);word(0x143fe10,0x200d200)
    for seed,(mode,fill)in itertools.product(seeds,itertools.product([0,1,2],[0,1,4,7,64,256])):
        initial=bytearray(seed['after'])
        def get(state,a):return struct.unpack_from('<I',state,a-BASE)[0]
        def put(state,a,v):struct.pack_into('<I',state,a-BASE,v&0xffffffff)
        inside=bool(seed['args'][4])!=bool(seed['args'][5]);row=0 if inside else 2 if seed['args'][5]else 1;cached=get(initial,TABLE+row*28+24)
        put(initial,VTABLE+4,FREE);put(initial,ALT+4,FREE_ALT);put(initial,ALT+12,QUERY)
        if mode==2:put(initial,RANGE,0);put(initial,RANGE+4,0);initial[0x6000-1]=255
        args=(POINTER+4,mode,fill,row,cached);expected=bytearray(initial);predicted=[]
        if mode!=2:
            for index in range(2):
                context=BASE+0x1400 if mode==1 and index else cached
                predicted.append((1,0,0,0,bytes(expected)));predicted.append((2,context,POINTER,1,bytes(expected)))
                if mode==1 and index==0:put(expected,TABLE+row*28+24,BASE+0x1400);expected[0x414]=99
                predicted.append((3,0,0,1,bytes(expected)))
            tracked=get(expected,POINTER+244);requested=get(expected,POINTER+248);put(expected,BASE+tracked*4,get(expected,BASE+tracked*4)-requested)
            predicted.append((2,cached,POINTER,0,bytes(expected)))
            if mode==1:put(expected,cached,ALT)
            expected[0x6000:0x6000+fill]=b'\xcd'*fill
            predicted.append((6 if mode==1 else 5,cached,POINTER,0,bytes(expected)))
            if mode==1:put(expected,BASE+0x40c,0);expected[0x414]=77
            put(expected,BASE+0x40c,get(expected,BASE+0x40c)-1)
        events=[];depth=queries=0;uc.mem_write(BASE,bytes(initial));uc.mem_write(0x1396868,bytes(initial[0x2000:0x2000+840]));uc.mem_write(0x13caaa4,bytes(initial[0x3000:0x3008]))
        uc.mem_write(stack,struct.pack('<2I',stop,POINTER+4));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0x9f2920,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8 and depth==0 and queries==(0 if mode==2 else 3)
        assert events==predicted and snapshot()==expected,args
        rows.append(dict(args=(*args,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:]))
    return rows

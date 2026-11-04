"""Retail guard/tracking initialization composed with the real main size query."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
from oracle_main_allocation_size import BASE,WIDTH,TABLE,RANGE,VTABLE,QUERY
def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[];args=();depth=0;queries=0
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def snapshot():
        state=bytearray(uc.mem_read(BASE,WIDTH));state[0x2000:0x2000+840]=uc.mem_read(0x1396868,840);state[0x3000:0x3008]=uc.mem_read(0x13caaa4,8);return bytes(state)
    def hook(uc,address,size,user):
        nonlocal depth,queries
        if address not in [0x200d000,QUERY,0x200d200]:return
        sp=uc.reg_read(UC_X86_REG_ESP);kind={0x200d000:1,QUERY:2,0x200d200:3}[address]
        events.append((kind,uc.reg_read(UC_X86_REG_ECX)if kind==2 else 0,read(sp+4)if kind==2 else 0,depth,snapshot()))
        if kind==2:
            if args[5]:
                uc.mem_write(BASE+0x414,bytes([255 if queries==0 else 7]));word(0x1396880,BASE+0x1400)
            uc.reg_write(UC_X86_REG_EAX,args[2]if queries==0 else args[3]);queries+=1
        else:
            assert read(sp+4)==0x13caa70;depth+=1 if kind==1 else -1
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
    uc.hook_add(UC_HOOK_CODE,hook);word(0x143fe0c,0x200d000);word(0x143fe10,0x200d200)
    pointer=BASE+0x6000
    for first,second,requested,debug,mutation,tracking in itertools.product([0,32,0xffffffff],[20,24,64,256],[0,1,7,0x7fffffff,0x80000000,0xffffffff],[0,1],[0,1],[0,255]):
        args=(pointer,debug,first,second,requested,mutation,tracking);initial=bytearray(bytes([0xa5 if debug else 0x5a])*WIDTH)
        def put(state,a,v):struct.pack_into('<I',state,a-BASE,v&0xffffffff)
        put(initial,BASE+0x410,1);initial[0x414]=tracking;initial[0x6000-5]=0;put(initial,RANGE,0);put(initial,RANGE+4,0);put(initial,VTABLE+12,QUERY)
        put(initial,TABLE+24,BASE+0x1000);put(initial,BASE+0x1000,VTABLE);put(initial,BASE+0x1400,VTABLE)
        for index in [0,7,255]:put(initial,BASE+index*4,0xfffffff0)
        expected=bytearray(initial);predicted=[]
        for index in range(2):
            predicted.append((1,0,0,0,bytes(expected)));context=BASE+0x1400 if mutation and index else BASE+0x1000;predicted.append((2,context,pointer-4,1,bytes(expected)))
            if mutation:expected[0x414]=255 if index==0 else 7;put(expected,TABLE+24,BASE+0x1400)
            predicted.append((3,0,0,1,bytes(expected)))
        final_tracking=7 if mutation else tracking;metadata=pointer+second-20
        for address,value in [(metadata,0xffffffff),(metadata+4,final_tracking),(metadata+8,requested),(pointer-4,0xffffffff),(BASE+final_tracking*4,0xfffffff0+requested)]:put(expected,address,value)
        uc.mem_write(BASE,bytes(initial));uc.mem_write(0x1396868,bytes(initial[0x2000:0x2000+840]));uc.mem_write(0x13caaa4,bytes(8));depth=queries=0;events=[]
        uc.mem_write(stack,struct.pack('<4I',stop,pointer,debug,requested));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0x9f28d0,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+16 and depth==0 and queries==2
        assert events==predicted and snapshot()==expected,args
        rows.append(dict(args=(*args,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:]))
    return rows

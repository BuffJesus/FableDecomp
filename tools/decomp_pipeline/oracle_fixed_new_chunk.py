"""Retail fixed heap acquisition, checked against independent state predictions."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
from oracle_fixed_chunk_lists import BASE,WIDTH

def collect():
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000);raw=EXE.read_bytes()
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[];args=();heap_calls=0
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def mutate():
        for offset,value in [(0xc14,16),(0xc18,4),(0xc1c,0x2007900),(0xc20,0x2007900),(0xc2c,64),(0xc30,28),(0xc34,3),(0xc38,999),(0xc48,0xfffffffd)]:word(BASE+offset,value)
        word(0x13caa90,0x2007b00);word(0x143fd3c,0x200d400)
    def hook(uc,address,size,user):
        nonlocal heap_calls
        if address not in [0x200d000,0x200d100,0x200d200,0x200d400,0x9f1f00]:return
        assert address!=0x200d400,'GetProcessHeap import must be cached across both calls'
        sp=uc.reg_read(UC_X86_REG_ESP);kind={0x200d000:1,0x200d100:2,0x200d200:3,0x9f1f00:4}[address]
        values=[0,0,0]if kind==1 else[uc.reg_read(UC_X86_REG_ECX),0,0]if kind==4 else[read(sp+4),read(sp+8),read(sp+12)]
        events.append((kind,*values,bytes(uc.mem_read(BASE,WIDTH))))
        if kind==1:
            heap_calls+=1
            if args[5] and heap_calls==1:mutate()
            result=args[6];cleanup=4
        elif kind==2:result=args[3];cleanup=16
        elif kind==3:result=args[4];cleanup=16
        else:result=0;cleanup=4
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
    uc.hook_add(UC_HOOK_CODE,hook)
    internal=itertools.product([0],[0,1,8,0xfffffff0,0xfffffff7,0xfffffff8,0xffffffff],[4,8,16,32,0,0xffffffff],[0,0x2006003],[0,64,0xffffffff],[0,1],[0,0x12345678])
    new=itertools.product([1],[256],[4,8,16],[0,0x2006003],[0,512,0xffffffff],[0,1],[0,0x12345678])
    for mode,size,alignment,allocation,heap_size,mutation,heap in itertools.chain(internal,new):
        args=(mode,size,alignment,allocation,heap_size,mutation,heap);initial=bytearray(b'\xa5'*WIDTH)
        def put(state,a,v):struct.pack_into('<I',state,a-BASE,v&0xffffffff)
        settings=[(0xc14,alignment),(0xc18,{4:2,8:3,16:4}.get(alignment,0)),(0xc1c,0),(0xc20,0),(0xc2c,32),(0xc30,28 if alignment==16 else 24),(0xc34,7),(0xc38,size),(0xc48,2)]
        for off,value in settings:put(initial,BASE+off,value)
        output=0x2007c00;put(initial,output,0xdeadbeef);expected=bytearray(initial)
        total=size+((alignment-1)&0xffffffff&~7);overflow=total>0xffffffff;active=not overflow
        if active and mutation:
            for off,value in [(0xc14,16),(0xc18,4),(0xc1c,0x2007900),(0xc20,0x2007900),(0xc2c,64),(0xc30,28),(0xc34,3),(0xc38,999),(0xc48,0xfffffffd)]:put(expected,BASE+off,value)
        result=heap_size if active and allocation else 0
        if mode==0:
            if active:put(expected,output,allocation)
        elif result:
            align=16 if mutation else alignment;chunk=(allocation+align-1)//align*align;stride=64 if mutation else 32;count=3 if mutation else 7;header=28 if align==16 else 24
            put(expected,chunk,allocation)
            for off in [4,8,12,16,20]:put(expected,chunk+off,0)
            for i in range(count):
                node=chunk+header+i*stride;expected[node-BASE:node-BASE+4]=bytes([i,(i-1)&255 if i else 255,255,0])
                if i:expected[node-BASE-stride+2]=i
            put(expected,chunk+16,chunk+header+(count-1)*stride)
            if mutation:put(expected,chunk+8,0x2007900);put(expected,0x200790c,chunk)
            else:put(expected,BASE+0xc20,chunk)
            put(expected,BASE+0xc1c,chunk);put(expected,BASE+0xc48,0xfffffffe if mutation else 3);result=chunk
        uc.mem_write(BASE,bytes(initial));events=[];heap_calls=0
        word(0x143fd3c,0x200d000);word(0x143fe88,0x200d100);word(0x143fe84,0x200d200);word(0x13caa90,0x2007800)
        uc.mem_write(stack,struct.pack('<3I',stop,size,output));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
        uc.emu_start(0xa65f40 if mode==0 else 0xa663d0,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+(12 if mode==0 else 4)
        assert uc.reg_read(UC_X86_REG_EAX)==result,(args,hex(uc.reg_read(UC_X86_REG_EAX)),hex(result))
        assert bytes(uc.mem_read(BASE,WIDTH))==expected,args
        kinds=([1,2]+([1,3]if allocation else[]))if active else[]
        if mode==1 and not result:kinds+=[4]
        assert [e[0]for e in events]==kinds
        for kind,a,b,c,_ in events:
            if kind==2:assert (a,b,c)==(heap,1,total)
            if kind==3:assert (a,b,c)==(heap,1,allocation)
            if kind==4:assert a==(0x2007b00 if active and mutation else 0x2007800)
        rows.append(dict(args=(*args,result,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:]))
    return rows

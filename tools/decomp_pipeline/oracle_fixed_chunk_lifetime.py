"""Retail fixed chunk initialization/release with independent state predictions."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
from oracle_fixed_chunk_lists import collect as list_cases,BASE,WIDTH
def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[];args=()
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def hook(uc,address,size,user):
        if address not in [0x200d000,0x200d100]:return
        sp=uc.reg_read(UC_X86_REG_ESP)
        if address==0x200d000:
            events.append((1,0,0,0,bytes(uc.mem_read(BASE,WIDTH))))
            if args[2]:word(args[1],args[1]-16);word(BASE+0xc24,args[1]);word(BASE+0xc48,0xfffffffd)
            uc.reg_write(UC_X86_REG_EAX,args[3]);n=1
        else:
            values=[read(sp+4),read(sp+8),read(sp+12)];assert values==[args[3],1,args[1]-8]
            events.append((2,*values,bytes(uc.mem_read(BASE,WIDTH))));uc.reg_write(UC_X86_REG_EAX,args[4]);n=4
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+n*4)
    word(0x143fd3c,0x200d000);word(0x143fe8c,0x200d100);uc.hook_add(UC_HOOK_CODE,hook)
    def invoke(mode,param,initial,expected,extra=(0,0,0),result=0):
        nonlocal args,events
        args=(mode,param,*extra,result);uc.mem_write(BASE,bytes(initial));events=[]
        uc.mem_write(stack,struct.pack('<2I',stop,param));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
        uc.emu_start(0xa661d0 if mode==0 else 0xa66160,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
        if mode==0:assert uc.reg_read(UC_X86_REG_EAX)==result
        assert bytes(uc.mem_read(BASE,WIDTH))==expected
        rows.append(dict(args=(*args,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:]))
    for alignment,count,stride,misalign,pattern in itertools.product([4,8,16],[0,1,3,7,254,255,256,257],[16,64],[0,1,3],[0x5a,0xa5]):
        shift={4:2,8:3,16:4}[alignment];memory=0x2006000+misalign;chunk=(memory+alignment-1)//alignment*alignment;header=(24+4+alignment-1)//alignment*alignment-4
        initial=bytearray(bytes([pattern])*WIDTH)
        def put(state,a,v):struct.pack_into('<I',state,a-BASE,v&0xffffffff)
        put(initial,BASE+0xc14,alignment);put(initial,BASE+0xc18,shift);put(initial,BASE+0xc2c,stride);put(initial,BASE+0xc30,header);put(initial,BASE+0xc34,count)
        expected=bytearray(initial);put(expected,chunk,memory)
        for off in [4,8,12,16,20]:put(expected,chunk+off,0)
        for i in range(count):
            node=chunk+header+i*stride
            expected[node-BASE:node-BASE+4]=bytes([i&255,(i-1)&255 if i else 255,255,0])
            if i:expected[node-BASE-stride+2]=i&255
        put(expected,chunk+16,chunk+header+(count-1)*stride if count else 0)
        invoke(0,memory,initial,expected,result=chunk)
    for seed in list_cases():
        if seed['args'][0]!=0:continue
        chunk=seed['args'][1]
        for current,mutation,heap,result in itertools.product([0,chunk,0x2007900],[0,1],[0,0x12345678],[0,1]):
            initial=bytearray(seed['initial']);expected=bytearray(seed['after'])
            for state in [initial,expected]:struct.pack_into('<I',state,chunk-BASE,chunk-8);struct.pack_into('<I',state,0xc24,current)
            if current==chunk:struct.pack_into('<I',expected,0xc24,0)
            if mutation:struct.pack_into('<I',expected,chunk-BASE,chunk-16);struct.pack_into('<I',expected,0xc24,chunk);struct.pack_into('<I',expected,0xc48,0xfffffffd)
            invoke(1,chunk,initial,expected,(mutation,heap,result))
            assert len(events)==2
    return rows

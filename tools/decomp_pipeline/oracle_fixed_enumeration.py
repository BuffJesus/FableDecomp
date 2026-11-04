"""Retail fixed-area ID enumeration with real/recorded virtual count boundary."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;TABLE=0x200b000;OUTPUT=0x200b800
def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];calls=0;boundary=0
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def hook(uc,address,size,user):
        nonlocal calls
        if address not in [0xa65e50,0x200d000]:return
        calls+=1;assert uc.reg_read(UC_X86_REG_ECX)==BASE
        if boundary:
            if boundary==2:word(BASE+0xc1c,read(read(BASE+0xc1c)+8)if read(BASE+0xc1c)else 0);word(BASE+0xc2c,64)
            sp=uc.reg_read(UC_X86_REG_ESP);uc.reg_write(UC_X86_REG_EAX,0xffffffff);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
    uc.hook_add(UC_HOOK_CODE,hook)
    for chunks,layout,reverse,boundary,pattern in itertools.product([0,1,3,7],[0,1,2],[0,1],[0,1,2],[0x5a,0xa5]):
        for mode in [0,1]:
            stride=64 if boundary==2 and mode==0 else 32
            initial=bytearray(bytes([pattern])*WIDTH)
            def put(a,v):struct.pack_into('<I',initial,a-BASE,v&0xffffffff)
            put(BASE,TABLE);put(TABLE+0x24,0xa65e50 if boundary==0 else 0x200d000);put(BASE+0xc2c,32)
            chain=[0x2006000+i*0x100 for i in range(chunks)]
            if reverse:chain.reverse()
            put(BASE+0xc1c,chain[0]if chain else 0);ordered=[];countResult=0
            for i,chunk in enumerate(chain):
                put(chunk+8,chain[i+1]if i+1<chunks else 0);put(chunk+4,0xfffffffe+i);countResult=(countResult+0xfffffffe+i)&0xffffffff
                ids=[]if layout==0 else[254]if layout==1 else[1,7,3]
                nodes=[0x2008000+i*0x400+(id_*stride if layout==2 else 0)for id_ in ids]
                put(chunk+0x14,nodes[0]if nodes else 0)
                for j,(id_,node)in enumerate(zip(ids,nodes)):
                    initial[node-BASE:node-BASE+4]=bytes([id_,ids[j+1]if j+1<len(ids)else 255,ids[j-1]if j else 255,37])
                ordered.append(nodes)
            expectedNodes=[n for group in (ordered[1:]if boundary==2 and mode==0 else ordered)for n in group]
            pointers=[n+4 for n in expectedNodes];total=len(pointers)
            requests=sorted(set([0,1,total,total+1,0xffffffff]))if mode==0 else sorted(set([0,*range(total),total,total+1,0x7fffffff,0x80000000,0xffffffff]))
            for arg in requests:
                expected=bytearray(initial)
                if boundary==2 and mode==0:struct.pack_into('<I',expected,0xc1c,chain[1]if chunks>1 else 0);struct.pack_into('<I',expected,0xc2c,64)
                result=total if mode==0 else pointers[arg]if arg<total else 0
                if mode==0:
                    for i,pointer in enumerate(pointers):struct.pack_into('<I',expected,OUTPUT-BASE+i*4,pointer)
                uc.mem_write(BASE,bytes(initial));calls=0
                args=[OUTPUT,arg]if mode==0 else[arg];uc.mem_write(stack,struct.pack('<'+str(len(args)+1)+'I',stop,*args));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
                uc.emu_start(0xa660a0 if mode==0 else 0xa66100,stop,count=3000)
                assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4*(len(args)+1)
                assert uc.reg_read(UC_X86_REG_EAX)==result and calls==(1 if mode==0 else 0)
                assert bytes(uc.mem_read(BASE,WIDTH))==expected
                rows.append(dict(args=(mode,boundary,arg,result,countResult if boundary==0 else 0xffffffff,calls),initial=bytes(initial),after=bytes(expected)))
    return rows

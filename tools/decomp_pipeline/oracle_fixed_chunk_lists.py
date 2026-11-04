"""Retail fixed allocator chunk-list mutations, independently predicted."""
import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000
def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[]
    for mode,count,reverse,pattern in itertools.product([0,1,2],[0,1,3,7],[0,1],[0x5a,0xa5]):
        chain=[0x2006000+i*0x100 for i in range(count)]
        if reverse:chain.reverse()
        for selected in range(count)if mode==0 else[-1]:
            initial=bytearray(bytes([pattern])*WIDTH)
            def put(state,a,v):struct.pack_into('<I',state,a-BASE,v)
            put(initial,BASE+0xc1c,chain[0]if chain else 0);put(initial,BASE+0xc20,chain[-1]if chain else 0)
            for i,node in enumerate(chain):put(initial,node+8,chain[i+1]if i+1<count else 0);put(initial,node+12,chain[i-1]if i else 0)
            node=chain[selected]if mode==0 else 0x2007800;expected=bytearray(initial)
            if mode==0:
                prev=chain[selected-1]if selected else 0;next_=chain[selected+1]if selected+1<count else 0
                if next_:put(expected,next_+12,prev)
                if prev:put(expected,prev+8,next_)
                if not prev:put(expected,BASE+0xc1c,next_)
                if not next_:put(expected,BASE+0xc20,prev)
                resultChain=chain[:selected]+chain[selected+1:]
            elif mode==1:
                put(expected,node+8,chain[0]if chain else 0);put(expected,node+12,0)
                if chain:put(expected,chain[0]+12,node)
                else:put(expected,BASE+0xc20,node)
                put(expected,BASE+0xc1c,node);resultChain=[node]+chain
            else:
                put(expected,node+12,chain[-1]if chain else 0);put(expected,node+8,0)
                if chain:put(expected,chain[-1]+8,node)
                else:put(expected,BASE+0xc1c,node)
                put(expected,BASE+0xc20,node);resultChain=chain+[node]
            uc.mem_write(BASE,bytes(initial));uc.mem_write(stack,struct.pack('<2I',stop,node));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
            uc.emu_start([0xa65e70,0xa65ec0,0xa65f00][mode],stop,count=1000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
            assert bytes(uc.mem_read(BASE,WIDTH))==expected
            p=struct.unpack_from('<I',expected,0xc1c)[0];actual=[];prev=0
            while p:
                assert p not in actual and struct.unpack_from('<I',expected,p-BASE+12)[0]==prev
                actual.append(p);prev=p;p=struct.unpack_from('<I',expected,p-BASE+8)[0]
            assert actual==resultChain and struct.unpack_from('<I',expected,0xc20)[0]==prev
            rows.append(dict(args=(mode,node),initial=bytes(initial),after=bytes(expected)))
    return rows

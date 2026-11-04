"""Retail allocation dispatch with real best-fit query and recorded splitting boundary."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
from oracle_variable_tree_insertion import collect as trees,BASE,WIDTH,OWNER

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;events=[];rows=[];returned=0
    def hook(uc,address,size,user):
        if address!=0xa65480:return
        sp=uc.reg_read(UC_X86_REG_ESP);ret,area,size=struct.unpack('<3I',bytes(uc.mem_read(sp,12)))
        events.append((uc.reg_read(UC_X86_REG_ECX),area,size))
        uc.reg_write(UC_X86_REG_EAX,returned);uc.reg_write(UC_X86_REG_ESP,sp+12);uc.reg_write(UC_X86_REG_EIP,ret)
    uc.hook_add(UC_HOOK_CODE,hook)
    empty=bytearray(b'\x5a'*WIDTH);struct.pack_into('<I',empty,0xc34,0)
    states=[bytes(empty)]+[bytes.fromhex(t['snapshots'][-1])for t in trees()]
    for state in states:
      for alignment in [4,8,16,32]:
       for request in [0,1,7,31,32,33,128,1024,0x7fffffff,0x80000000,0xfffffffe,0xffffffff]:
        for returned in [0,0x200b800]:
            initial=bytearray(state);shift={4:2,8:3,16:4,32:5}[alignment]
            struct.pack_into('<2I',initial,0xc14,alignment,shift)
            uc.mem_write(BASE,bytes(initial));events.clear()
            uc.mem_write(stack,struct.pack('<2I',stop,request));uc.reg_write(UC_X86_REG_ECX,OWNER);uc.reg_write(UC_X86_REG_ESP,stack)
            uc.emu_start(0xa656e0,stop,count=20000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
            def word(a):return struct.unpack_from('<I',initial,a-BASE)[0]
            pending=[word(OWNER+0xc34)];nodes=[]
            while pending:
                a=pending.pop()
                if not a:continue
                nodes.append((a,struct.unpack_from('<i',initial,a-BASE+0x18)[0]));pending.extend([word(a+4),word(a+0x10)])
            aligned=((request+alignment-1)&0xffffffff)>>shift<<shift
            signed=aligned if aligned<0x80000000 else aligned-0x100000000
            eligible=[size for a,size in nodes if size>=signed]
            result=uc.reg_read(UC_X86_REG_EAX)
            if eligible:
                assert len(events)==1;chunk,area,size=events[0]
                assert (area,min(eligible))in nodes and size==aligned and chunk==word(area+0x14)and result==returned
            else:assert not events and result==0
            assert bytes(uc.mem_read(BASE,WIDTH))==initial
            rows.append(dict(initial=bytes(initial),request=request,returned=returned,result=result,event=events[0]if events else(0,0,0),calls=len(events)))
    return rows

if __name__=='__main__':print('VARIABLE_ALLOCATE_EXISTING_RETAIL cases=%d'%len(collect()))

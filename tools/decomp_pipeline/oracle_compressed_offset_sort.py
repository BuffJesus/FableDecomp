"""Execute the retail offset-sort chain without mocked callees."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections

def keys_for(count,mode):
    return [i if mode==0 else count-i if mode==1 else (i*7+3)%5 if mode==2 else [0,0x80000000,0xffffffff,7,0x7fffffff,0xfffffffe][i%6] for i in range(count)]

def collect():
    raw=EXE.read_bytes();sections=pe_sections(raw)
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in sections:
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);first=0x2002000;stack=0x2010000;stop=0x200f000;rows=[]
    def call(address,begin,end,args):
        uc.mem_write(stack,struct.pack('<'+'I'*(1+len(args)),stop,*args))
        uc.reg_write(UC_X86_REG_ECX,begin);uc.reg_write(UC_X86_REG_EDX,end);uc.reg_write(UC_X86_REG_ESP,stack)
        uc.emu_start(address,stop,count=1000000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4+4*len(args),(hex(address),uc.reg_read(UC_X86_REG_ESP))
    for count in [0,1,2,16,17,32,64]:
        for mode in range(4):
            keys=keys_for(count,mode)
            for route in range(3):
                middle=count//2 if count>1 else count
                source=b''.join(struct.pack('<II',key,0x80000000+i)for i,key in enumerate(keys))
                guard=struct.pack('<II',0xdeadbeef,0x12345678)
                uc.mem_write(first-8,guard+source+guard);last=first+count*8
                if route==0:call(0x9b85a0,first,last,[0])
                elif route==1:
                    call(0x9b7fa0,first,last,[0,0,0])
                    split=first+min(count,16)*8
                    call(0x9b56f0,first,split,[0])
                    if count>16:call(0x9b2db0,split,last,[0,0])
                else:call(0x9b6be0,first,first+middle*8,[last,0,0])
                result=list(struct.iter_unpack('<II',bytes(uc.mem_read(first,count*8)))) if count else []
                assert bytes(uc.mem_read(first-8,8))==guard and bytes(uc.mem_read(last,8))==guard
                assert sorted(result)==sorted(zip(keys,[0x80000000+i for i in range(count)]))
                prefix=middle if route==2 else count
                assert [key for key,_ in result[:prefix]]==sorted(keys)[:prefix]
                rows.append(dict(count=count,mode=mode,route=route,middle=middle,result=result))
    return rows

if __name__=='__main__':print('RETAIL_COMPRESSED_OFFSET_SORT_PASS cases='+str(len(collect())))

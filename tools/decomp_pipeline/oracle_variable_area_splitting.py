"""Retail split/free-list state, with tree insertion/removal recorded as boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;CHUNK=0x2009000;AREA=0x200a000

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;events=[];rows=[]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def hook(uc,address,size,user):
        if address not in [0xa64ef0,0xa64e70]:return
        sp=uc.reg_read(UC_X86_REG_ESP);count=2 if address==0xa64ef0 else 3
        values=struct.unpack('<'+str(count)+'I',bytes(uc.mem_read(sp,4*count)))
        assert uc.reg_read(UC_X86_REG_ECX)==BASE
        events.append((address,values[1:],bytes(uc.mem_read(BASE,WIDTH))))
        uc.reg_write(UC_X86_REG_ESP,sp+count*4);uc.reg_write(UC_X86_REG_EIP,values[0])
    uc.hook_add(UC_HOOK_CODE,hook)
    for mode in [0,1,2]:
      for align in [4,8,16,32]:
       for pattern in [0x5a,0xa5]:
        for position in [0,1,2]:
         for allocated in [0,1]:
          for request in ([-0x80000000,-1,0,1,95,96,97,128,0x7fffffff]if mode==0 else[0]):
            uc.mem_write(BASE,bytes([pattern])*WIDTH);word(0x13d28d4,align);word(0x13d28d8,{4:2,8:3,16:4,32:5}[align])
            nodes=[(0x200b000,64),(AREA,128),(0x200b080,256)]
            if position==0:nodes=nodes[1:]
            elif position==2:nodes=nodes[:2]
            else:pass
            if mode==2:nodes=[]
            for i,(a,size)in enumerate(nodes):
                word(a,nodes[i+1][0]if i+1<len(nodes)else 0);word(a+8,nodes[i-1][0]if i else 0)
                word(a+0x14,CHUNK);word(a+0x18,size)
            word(CHUNK+0x18,BASE);word(CHUNK+4,len(nodes));word(CHUNK+8,allocated)
            word(CHUNK+0xc,sum(size for a,size in nodes));word(CHUNK+0x28,nodes[0][0]if nodes else 0)
            word(CHUNK+0x2c,nodes[-1][0]if nodes else 0);word(CHUNK+0x10,nodes[-1][1]if nodes else 0)
            word(CHUNK+0x14,nodes[0][1]if nodes else 0);word(CHUNK+0x30,0x200b100 if allocated else 0)
            word(BASE+0xc34,AREA)
            if allocated:word(0x200b100,0);word(0x200b108,0)
            initial=bytes(uc.mem_read(BASE,WIDTH));events.clear()
            entry=[0xa65480,0xa65060,0xa64d20][mode];params=[AREA,request]if mode==0 else[AREA]
            uc.mem_write(stack,struct.pack('<'+str(len(params)+1)+'I',stop,*(p&0xffffffff for p in params)))
            uc.reg_write(UC_X86_REG_ECX,CHUNK);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(entry,stop,count=10000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4*(len(params)+1)
            result=uc.reg_read(UC_X86_REG_EAX)if mode==0 else 0
            after=bytes(uc.mem_read(BASE,WIDTH))
            if mode==0:
                total=(request+32)&0xffffffff;overflow=total>=0x80000000
                if overflow:assert result==0 and after==initial and not events
                else:
                    split=128>total;expected=AREA+32+128-total if split else AREA
                    assert result==expected and read(CHUNK+0x30)==result and read(CHUNK+8)==allocated+1
                    assert read(CHUNK+4)==len(nodes)-(not split)
                    assert read(CHUNK+0xc)==sum(s for a,s in nodes)-(total if split else 128)
                    assert len(events)==1+split and read(result+0x18)==(request&0xffffffff if split else 128)
                    assert after[result-BASE+0x1d]==initial[result-BASE+0x1d]
            elif mode==1:
                assert len(events)==1 and read(CHUNK+4)==len(nodes)-1 and read(CHUNK+0xc)==sum(s for a,s in nodes)-128
                assert read(AREA)==0 and read(AREA+8)==0
            else:
                assert not events and read(CHUNK+0x30)==AREA and read(CHUNK+8)==allocated+1
                assert read(AREA)==(0x200b100 if allocated else 0)and read(AREA+8)==0
            rows.append(dict(mode=mode,align=align,initial=initial,after=after,request=request,result=result,events=list(events)))
    return rows

if __name__=='__main__':print('VARIABLE_AREA_SPLITTING_RETAIL cases=%d'%len(collect()))

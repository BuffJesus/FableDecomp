"""Retail virtual deallocation/chunk destruction with real lists and recorded tree/OS."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;CHUNK=0x2009000;AREA=0x200a100;PREV=0x200b000;NEXT=0x200b100

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;events=[];rows=[];status_calls=0;status_pointer=0;flip=0;free_result=0
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def hook(uc,address,size,user):
        nonlocal status_calls,status_pointer
        if address in [0xa64ef0,0xa64e70]:
            sp=uc.reg_read(UC_X86_REG_ESP);n=2 if address==0xa64ef0 else 3;args=struct.unpack('<'+str(n)+'I',bytes(uc.mem_read(sp,n*4)))
            assert uc.reg_read(UC_X86_REG_ECX)==BASE
            events.append((1 if n==2 else 2,BASE,*((args[1:]+(0,))[:2]),bytes(uc.mem_read(BASE,WIDTH))))
            uc.reg_write(UC_X86_REG_ESP,sp+n*4);uc.reg_write(UC_X86_REG_EIP,args[0]);return
        if address not in [0x200d000,0x200d100]:return
        sp=uc.reg_read(UC_X86_REG_ESP);ret=read(sp)
        if address==0x200d000:
            pointer=read(sp+4);status_calls+=1
            if status_calls==1:status_pointer=pointer
            else:assert pointer==status_pointer
            events.append((3,0,0,0,bytes(uc.mem_read(BASE,WIDTH))))
            uc.mem_write(pointer,struct.pack('<8I',32,17,1000,900,800,700,600,500))
            if status_calls==1 and flip:uc.mem_write(CHUNK+0x1d,bytes([bytes(uc.mem_read(CHUNK+0x1d,1))[0]^1]))
            n=2
        else:
            args=[read(sp+4),read(sp+8),read(sp+12)];assert args==[CHUNK,0,0x8000]
            events.append((4,*args,bytes(uc.mem_read(BASE,WIDTH))));uc.reg_write(UC_X86_REG_EAX,free_result);n=4
        uc.reg_write(UC_X86_REG_ESP,sp+n*4);uc.reg_write(UC_X86_REG_EIP,ret)
    word(0x143fe20,0x200d000);word(0x143fd40,0x200d100);uc.hook_add(UC_HOOK_CODE,hook)
    cases=list(itertools.product([0],[4],range(4),[0,1,3],[0],[0,1],[0,1],[0,1],[0,1],[0x5a,0xa5]))
    cases+=list(itertools.product([1],[4,8,16,32],range(4),[0,2],[1,2],[0,1],[0,1],[0,1],[0,1],[0x5a,0xa5]))
    for mode,alignment,position,free_count,allocated,permanent,demand,flip,free_result,pattern in cases:
        uc.mem_write(BASE,bytes([pattern])*WIDTH);shift={4:2,8:3,16:4,32:5}[alignment]
        word(BASE+0xc14,alignment);word(BASE+0xc18,shift);word(BASE+0xc34,0)
        word(0x13d28d4,64 if mode else alignment);word(0x13d28d8,6 if mode else shift)
        previous=PREV if position in [2,3]else 0;following=NEXT if position in [1,2]else 0
        word(BASE+0xc1c,previous if previous else CHUNK);word(CHUNK+0x24,previous);word(CHUNK+0x20,following)
        word(PREV+0x20,CHUNK);word(PREV+0x24,0);word(NEXT+0x24,CHUNK);word(NEXT+0x20,0)
        uc.mem_write(CHUNK+0x1c,bytes([permanent,demand]));word(CHUNK+0x18,BASE)
        free_nodes=([(0x200a000+i*0x100,64+i*32)for i in range(free_count)]if mode==0 else[(AREA-96,64),(AREA+160,96)]if free_count else[])
        for i,(a,size)in enumerate(free_nodes):
            word(a,free_nodes[i+1][0]if i+1<len(free_nodes)else 0);word(a+8,free_nodes[i-1][0]if i else 0);word(a+0x18,size);word(a+0x14,CHUNK)
        word(CHUNK+4,free_count);word(CHUNK+8,allocated);word(CHUNK+0xc,sum(size for a,size in free_nodes))
        word(CHUNK+0x28,free_nodes[0][0]if free_nodes else 0);word(CHUNK+0x2c,free_nodes[-1][0]if free_nodes else 0)
        word(CHUNK+0x10,free_nodes[-1][1]if free_nodes else 0);word(CHUNK+0x14,free_nodes[0][1]if free_nodes else 0)
        word(CHUNK+0x30,AREA if mode else 0)
        if mode:
            word(AREA,0x200a500 if allocated==2 else 0);word(AREA+8,0);word(AREA+0x18,128);word(AREA+0x14,CHUNK)
            if allocated==2:word(0x200a500,0);word(0x200a508,AREA);word(0x200a514,CHUNK);word(0x200a518,64)
        initial=bytes(uc.mem_read(BASE,WIDTH));events.clear();status_calls=0;status_pointer=0
        entry=0xa65390 if mode==0 else 0xa65a50;argument=CHUNK if mode==0 else AREA+32
        uc.mem_write(stack,struct.pack('<2I',stop,argument));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(entry,stop,count=30000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
        destroyed=mode==0 or allocated==1 and not permanent
        assert status_calls==(2 if destroyed else 0)
        released=destroyed and not(demand^flip);assert sum(e[0]==4 for e in events)==released
        if destroyed:
            assert read(BASE+0xc1c)==(previous if previous else following)
            if previous:assert read(PREV+0x20)==following
            if following:assert read(NEXT+0x24)==previous
            assert [e[0]for e in events if e[0]in [3,4]]==([3,4,3]if released else[3,3])
        else:assert read(BASE+0xc1c)==(previous if previous else CHUNK)
        assert read(CHUNK+0x20)==following and read(CHUNK+0x24)==previous
        if mode:
            assert read(0x13d28d4)==alignment and read(0x13d28d8)==shift
            assert read(CHUNK+8)==allocated-1 and read(CHUNK+4)==1
            assert read(CHUNK+0xc)==128+(224 if free_count else 0)
            assert read(CHUNK+0x28)==(AREA-96 if free_count else AREA)
            assert read(CHUNK+0x30)==(0x200a500 if allocated==2 else 0)
        else:assert [e[2]for e in events if e[0]==1]==[a for a,size in free_nodes]
        rows.append(dict(args=[mode,alignment,argument,flip,free_result],initial=initial,after=bytes(uc.mem_read(BASE,WIDTH)),events=list(events),globals=[read(0x13d28d4),read(0x13d28d8)]))
    return rows

if __name__=='__main__':print('VARIABLE_DEALLOCATION_RETAIL cases=%d'%len(collect()))

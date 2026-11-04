"""Retail freeing/consolidation with exact list state and recorded tree boundaries."""
import random,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;CHUNK=0x2009000;AREAS=0x200a000

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
        args=struct.unpack('<'+str(count)+'I',bytes(uc.mem_read(sp,count*4)))
        assert uc.reg_read(UC_X86_REG_ECX)==BASE
        events.append((address,(args[1:]+(0,))[:2],bytes(uc.mem_read(BASE,WIDTH))))
        uc.reg_write(UC_X86_REG_ESP,sp+count*4);uc.reg_write(UC_X86_REG_EIP,args[0])
    uc.hook_add(UC_HOOK_CODE,hook)
    sizes=[64,32,128,32,96];addresses=[AREAS]
    for size in sizes[:-1]:addresses.append(addresses[-1]+32+size)
    target=addresses[2]
    def chain(head):
        nodes=[];prev=0;a=read(head)
        while a:
            assert a not in nodes and read(a+8)==prev and read(a+0x14)==CHUNK
            nodes.append(a);prev=a;a=read(a)
        return nodes
    for mode in [0,1,2]:
     for alignment in [4,8,16,32]:
      for pattern in [0x5a,0xa5]:
       for mask in range(32):
        if bool(mask&4)!=(mode==1):continue
        for order_mode in [0,1,2]:
            uc.mem_write(BASE,bytes([pattern])*WIDTH);word(0x13d28d4,alignment);word(0x13d28d8,{4:2,8:3,16:4,32:5}[alignment]);word(BASE+0xc34,0)
            order=list(range(5))
            if order_mode==1:order.reverse()
            elif order_mode==2:random.Random(73).shuffle(order)
            free=sorted([i for i in order if mask&(1<<i)],key=lambda i:sizes[i]);allocated=[i for i in order if not mask&(1<<i)]
            for i,a in enumerate(addresses):word(a+0x14,CHUNK);word(a+0x18,sizes[i])
            for indices in [free,allocated]:
                for n,i in enumerate(indices):
                    word(addresses[i],addresses[indices[n+1]]if n+1<len(indices)else 0);word(addresses[i]+8,addresses[indices[n-1]]if n else 0)
            word(CHUNK+0x18,BASE);word(CHUNK+4,len(free));word(CHUNK+8,len(allocated));word(CHUNK+0xc,sum(sizes[i]for i in free))
            word(CHUNK+0x28,addresses[free[0]]if free else 0);word(CHUNK+0x2c,addresses[free[-1]]if free else 0);word(CHUNK+0x30,addresses[allocated[0]]if allocated else 0)
            word(CHUNK+0x10,sizes[free[-1]]if free else 0);word(CHUNK+0x14,sizes[free[0]]if free else 0)
            initial=bytes(uc.mem_read(BASE,WIDTH));events.clear();entry=[0xa658e0,0xa65590,0xa64ce0][mode]
            uc.mem_write(stack,struct.pack('<2I',stop,target));uc.reg_write(UC_X86_REG_ECX,CHUNK);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(entry,stop,count=30000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
            after=bytes(uc.mem_read(BASE,WIDTH));new_free=chain(CHUNK+0x28);new_allocated=chain(CHUNK+0x30)
            assert new_allocated==[addresses[i]for i in allocated if mode==1 or i!=2]
            if mode==2:
                assert new_free==[addresses[i]for i in free]and not events and read(target)==0 and read(target+8)==0
            else:
                active_mask=mask|4;first=last=2
                while first>0 and active_mask&(1<<(first-1)):first-=1
                while last<4 and active_mask&(1<<(last+1)):last+=1
                merged_size=sum(sizes[first:last+1])+32*(last-first)
                expected={addresses[i]:sizes[i]for i in free if not first<=i<=last};expected[addresses[first]]=merged_size
                assert {a:read(a+0x18)for a in new_free}==expected
                assert after[addresses[first]-BASE+0x1d]==initial[addresses[first]-BASE+0x1d]
                assert len(events)==(mode==0)+3*(last-first)
            assert read(CHUNK+4)==len(new_free)and read(CHUNK+8)==len(new_allocated)
            free_sizes=[read(a+0x18)for a in new_free];assert free_sizes==sorted(free_sizes)
            assert read(CHUNK+0xc)==sum(free_sizes)and read(CHUNK+0x2c)==(new_free[-1]if new_free else 0)
            assert read(CHUNK+0x10)==(free_sizes[-1]if free_sizes else 0)and read(CHUNK+0x14)==(free_sizes[0]if free_sizes else 0)
            rows.append(dict(mode=mode,alignment=alignment,area=target,initial=initial,after=after,events=list(events)))
    return rows

if __name__=='__main__':print('VARIABLE_CHUNK_FREE_RETAIL cases=%d'%len(collect()))

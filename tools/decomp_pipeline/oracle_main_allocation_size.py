"""Retail main size query with independent ID/lock/callback state predictions."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;TABLE=BASE+0x2000;RANGE=BASE+0x3000;VTABLE=BASE+0x1800;QUERY=0x200d100
def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[];args=();depth=0
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def snapshot():
        state=bytearray(uc.mem_read(BASE,WIDTH));state[0x2000:0x2000+840]=uc.mem_read(0x1396868,840);state[0x3000:0x3008]=uc.mem_read(0x13caaa4,8);return bytes(state)
    def mutate(which):
        if which==1:
            word(BASE+0x410,1);uc.mem_write(BASE+0x6000-5,b'\0');word(0x13caaa4,args[0]);word(0x13caaa8,args[0]+1);word(0x1396880,BASE+0x1400)
        else:
            word(BASE+0x410,0);uc.mem_write(BASE+0x6000-5,b'\xff');word(0x13caaa4,0xffffffff);word(0x13caaa8,0xffffffff);word(0x1396880,0)
    def hook(uc,address,size,user):
        nonlocal depth
        if address not in [0x200d000,QUERY,0x200d200]:return
        sp=uc.reg_read(UC_X86_REG_ESP);kind={0x200d000:1,QUERY:2,0x200d200:3}[address]
        if kind==2:
            events.append((kind,uc.reg_read(UC_X86_REG_ECX),read(sp+4),depth,snapshot()))
            if args[3]==2:mutate(2)
            uc.reg_write(UC_X86_REG_EAX,args[2])
        else:
            assert read(sp+4)==0x13caa70;events.append((kind,0,0,depth,snapshot()))
            if kind==1:
                depth+=1
                if args[3]==1:mutate(1)
            else:depth-=1
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
    uc.hook_add(UC_HOOK_CODE,hook);word(0x143fe0c,0x200d000);word(0x143fe10,0x200d200)
    modes=['null','header','range0','range1','range29','range255','below','end','reversed']
    for mode,header_id,count,result,mutation,initial_depth in itertools.product(modes,[0,29,30,255],[0,1,30],[0,0xffffffff],[0,1,2],[0,1]):
        pointer=0 if mode=='null'else BASE+0x6000
        start=end=0;resolved=header_id
        if mode.startswith('range'):
            resolved=int(mode[5:]);pointer=0xff00a000;start=pointer-(resolved<<24);end=pointer+1
        elif mode=='below':start=pointer+1;end=pointer+8
        elif mode=='end':start=pointer-8;end=pointer
        elif mode=='reversed':start=pointer+8;end=pointer-8
        initial=bytearray(b'\xa5'*WIDTH)
        def put(state,a,v):struct.pack_into('<I',state,a-BASE,v&0xffffffff)
        put(initial,BASE+0x410,count);initial[0x6000-5]=header_id;put(initial,RANGE,start);put(initial,RANGE+4,end);put(initial,VTABLE+12,QUERY)
        for index in range(30):put(initial,TABLE+index*28+24,BASE+0x1000+index*16);put(initial,BASE+0x1000+index*16,VTABLE)
        put(initial,BASE+0x1400,VTABLE);expected=bytearray(initial);predicted=[(1,0,0,initial_depth,bytes(expected))];held=initial_depth+1
        if mutation==1:
            put(expected,BASE+0x410,1);expected[0x6000-5]=0;put(expected,RANGE,pointer);put(expected,RANGE+4,pointer+1);put(expected,TABLE+24,BASE+0x1400);resolved=0;count_after=1
        else:count_after=count
        valid=pointer!=0 and resolved<count_after
        if valid:
            context=BASE+0x1400 if mutation==1 else BASE+0x1000+resolved*16
            predicted.append((2,context,pointer-4,held,bytes(expected)))
            if mutation==2:
                put(expected,BASE+0x410,0);expected[0x6000-5]=255;put(expected,RANGE,0xffffffff);put(expected,RANGE+4,0xffffffff);put(expected,TABLE+24,0)
            predicted.append((3,0,0,held,bytes(expected)));held-=1
        args=(pointer,initial_depth,result,mutation);depth=initial_depth;events=[];uc.mem_write(BASE,bytes(initial));uc.mem_write(0x1396868,bytes(initial[0x2000:0x2000+840]));uc.mem_write(0x13caaa4,bytes(initial[0x3000:0x3008]))
        uc.mem_write(stack,struct.pack('<2I',stop,pointer));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0x9f2390,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
        assert uc.reg_read(UC_X86_REG_EAX)==(result if valid else 0)and depth==held
        assert events==predicted and snapshot()==expected,(mode,header_id,count,result,mutation,initial_depth)
        rows.append(dict(args=(*args,result if valid else 0,held,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:],mode=mode,valid=valid))
    return rows

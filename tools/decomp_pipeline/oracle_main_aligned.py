"""Retail aligned API with independent pointer/owner/callback ordering model."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;OWNER_A=BASE;OWNER_B=BASE+0x1000;GLOBAL=BASE+0x3000;ALIGNED=BASE+0x7000
def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[];args=();depth=0
    hooks={0x200d000:1,0x200d100:2,0x9f2870:4,0x9f2a40:5,0x9f2920:6,0x9f2310:7,0x200d200:8}
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def snapshot():
        state=bytearray(uc.mem_read(BASE,WIDTH));state[0x3000:0x3004]=uc.mem_read(0x13caa90,4);state[0x3004:0x3005]=uc.mem_read(0x13caa94,1);struct.pack_into('<I',state,0x3008,depth);return bytes(state)
    def hook(uc,address,size,user):
        nonlocal depth
        if address not in hooks:return
        kind=hooks[address];sp=uc.reg_read(UC_X86_REG_ESP);owner=uc.reg_read(UC_X86_REG_ECX);mode,parameter,alignment,original,_,_,_,mutation,*_=args
        values=(owner,read(sp+4),0)if kind in [5,6]else(owner,0,0)if kind==7 else(read(sp+4),read(sp+8),read(sp+12))if kind==8 else(0,0,0)
        events.append((kind,*values,snapshot()));cleanup=4
        if kind==1:
            assert read(sp+4)==0x13caa70;depth+=1;cleanup=8
            if mutation==1:
                word(0x13caa90,OWNER_B)
                if mode:word(parameter-4,original+16)
            elif mutation==2 and mode:word(parameter-4,original+16)
        elif kind==2:
            assert read(sp+4)==0x13caa70;cleanup=8
            if mode==0:
                target=((original+alignment+3)&0xffffffff)&~((alignment-1)&0xffffffff)
                if mutation==1:word(target-4,0x12345678)
                elif mutation==2:word(0x13caa90,0)
            depth-=1
        elif kind==4:word(0x13caa90,OWNER_B if mutation==2 else OWNER_A)
        elif kind==5:
            assert read(sp+8)==0 and read(sp+12)==0;cleanup=16;word(owner+0x40c,read(owner+0x40c)+1);uc.reg_write(UC_X86_REG_EAX,original)
            if mutation==2:word(0x13caa90,OWNER_A)
        elif kind==6:
            cleanup=8;word(owner+0x40c,read(owner+0x40c)-1)
            if mutation==2:word(0x13caa90,OWNER_B);word(OWNER_B+0x40c,0);uc.mem_write(0x13caa94,b'\1')
        elif kind==7:
            if mutation==2:word(0x13caa90,OWNER_A)
        elif kind==8:cleanup=16;uc.reg_write(UC_X86_REG_EAX,args[8])
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
    uc.hook_add(UC_HOOK_CODE,hook);word(0x143fe0c,0x200d000);word(0x143fe10,0x200d100);word(0x143fd40,0x200d200)
    allocation=((0,size,alignment,BASE+0x6000+offset,singleton,0,0,mutation,1)for size,alignment,offset,singleton,mutation in itertools.product([0,1,235,236,0x7fffffff,0xffffffff],[1,2,4,8,16,32,64,256,4096],[0,3],[0,OWNER_A],[0,1,2]))
    release=((1,pointer,0,original,singleton,count,flag,mutation,result)for pointer,original,singleton,count,flag,mutation,result in itertools.product([0,ALIGNED],[0,BASE+0x5004],[0,OWNER_A],[0,1,3],[0,1],[0,1,2],[0,1]))
    for base_args in itertools.chain(allocation,release):
        mode,parameter,alignment,original,singleton,count,flag,mutation,free_result=base_args;args=base_args;initial=bytearray(b'\xa5'*WIDTH)
        def put(state,a,v):struct.pack_into('<I',state,a-BASE,v&0xffffffff)
        put(initial,GLOBAL,singleton);initial[0x3004]=flag;put(initial,GLOBAL+8,0)
        for owner in [OWNER_A,OWNER_B]:put(initial,owner+0x40c,count)
        if mode and parameter:put(initial,parameter-4,original)
        expected=bytearray(initial);predicted=[];held=0;active=singleton
        def event(kind,a=0,b=0,c=0):put(expected,GLOBAL+8,held);predicted.append((kind,a,b,c,bytes(expected)))
        if mode==0:
            if not active:event(4);active=OWNER_B if mutation==2 else OWNER_A;put(expected,GLOBAL,active)
            event(1);held+=1
            if mutation==1:active=OWNER_B;put(expected,GLOBAL,active)
            event(5,active,(parameter+alignment+3)&0xffffffff,0);put(expected,active+0x40c,count+1)
            if mutation==2:active=OWNER_A;put(expected,GLOBAL,active)
            event(2)
            result=((original+alignment+3)&0xffffffff)&~((alignment-1)&0xffffffff)
            if mutation==1:put(expected,result-4,0x12345678)
            elif mutation==2:active=0;put(expected,GLOBAL,0)
            held-=1;put(expected,result-4,original)
        else:
            result=0
            if parameter:
                event(1);held+=1
                if mutation==1:active=OWNER_B;put(expected,GLOBAL,active);put(expected,parameter-4,original+16)
                elif mutation==2:put(expected,parameter-4,original+16)
                if original and active:
                    event(6,active,original);put(expected,active+0x40c,count-1)
                    if mutation==2:active=OWNER_B;put(expected,GLOBAL,active);put(expected,OWNER_B+0x40c,0);expected[0x3004]=1
                    current_count=struct.unpack_from('<I',expected,active-BASE+0x40c)[0]
                    if expected[0x3004]and current_count==0:
                        cached=active;event(7,cached)
                        if mutation==2:active=OWNER_A;put(expected,GLOBAL,active)
                        event(8,cached,0,0x8000);active=0;put(expected,GLOBAL,0)
                event(2);held-=1
        put(expected,GLOBAL+8,held);events=[];depth=0;uc.mem_write(BASE,bytes(initial));word(0x13caa90,singleton);uc.mem_write(0x13caa94,bytes([flag]));word(stack,stop)
        uc.reg_write(UC_X86_REG_ECX,parameter);uc.reg_write(UC_X86_REG_EDX,alignment);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0x9f2be0 if mode==0 else 0x9f2b20,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4 and depth==0
        if mode==0:assert uc.reg_read(UC_X86_REG_EAX)==result
        assert events==predicted and snapshot()==expected,base_args
        rows.append(dict(args=(*base_args,result,len(events)),initial=bytes(initial),after=bytes(expected),events=events[:]))
    return rows

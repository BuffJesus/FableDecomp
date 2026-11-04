"""Replay actual font selection/measurement and x87 conversion with recorded bank calls."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_FPCW
from pe_oracle import EXE,pe_sections
BASE=0x24000000
def collect():
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000);uc.mem_map(BASE,65536);raw=EXE.read_bytes()
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    stack,stop,extent,release=BASE+0x8000,BASE+0xf000,BASE+0xf100,BASE+0xf200
    inputs,expected=bytearray(),bytearray();mutation=refs=events=calls=0
    def word(a,v):uc.mem_write(a,struct.pack('<I',v))
    def read(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def string(a):return bytes(uc.mem_read(a,32)).split(b'\0',1)[0]
    def event(kind,value=0):
        nonlocal events
        expected.extend(struct.pack('<II',kind,value));expected.extend(uc.mem_read(BASE,4096));events+=1
    def hook(uc,a,n,user):
        nonlocal calls
        if a not in (0x99e4b0,0x99eae0,0x99efe0,0x99f100,0x9a4ec0,0x9e2c80,extent,release,0xbfe9bc,0x99b6b0,0x99b720,0x99b510,0x9db700):return
        sp=uc.reg_read(UC_X86_REG_ESP);self=uc.reg_read(UC_X86_REG_ECX);cleanup=0
        if a==0x99e4b0:word(self,0);event(1);uc.reg_write(UC_X86_REG_EAX,self)
        elif a==0x99eae0:event(9)
        elif a==0x99efe0:
            text=string(read(sp+4));assert text==b'ENG_ARIAL_';event(2)
            uc.mem_write(BASE+0x600,text+b'\0');word(self,BASE+0x600)
            if mutation==1:word(BASE+0x110,28)
            uc.reg_write(UC_X86_REG_EAX,self);cleanup=4
        elif a==0x99f100:
            text=string(read(sp+4));event(3,int(text));uc.mem_write(BASE+0x600,string(BASE+0x600)+text+b'\0')
            if mutation==2:word(BASE+0x110,7)
            uc.reg_write(UC_X86_REG_EAX,self);cleanup=4
        elif a==0x9a4ec0:
            event(4);calls+=1
            if mutation==3:
                word(BASE+0x108,0x42800000+calls*0x10000)
                word(BASE+0x10c,0x42000000+calls*0x10000)
                word(BASE+0x908,BASE+(0xc00 if calls&1 else 0xb00))
                uc.mem_write(0x13750b0,bytes([(calls*37)&255]));uc.mem_write(BASE+0x850,bytes([(calls*37)&255]))
            uc.reg_write(UC_X86_REG_EAX,BASE+0x700)
        elif a==0x9e2c80:
            assert read(read(sp+8))==BASE+0x600;event(5,self);target=read(sp+4)
            word(target,BASE+0x300);word(target+4,BASE+0x400 if refs else 0)
            uc.reg_write(UC_X86_REG_EAX,target);cleanup=8
        elif a==extent:
            assert read(sp+8)==BASE+0x680 and read(sp+12)==0;event(6,self);target=read(sp+4)
            uc.mem_write(target,bytes(uc.mem_read(BASE+0x620,8)));uc.reg_write(UC_X86_REG_EAX,target);cleanup=12
        elif a==0x99b6b0:
            assert read(sp+4)==BASE+0x680;word(self,BASE+0x680);event(12);cleanup=4;uc.reg_write(UC_X86_REG_EAX,self)
        elif a==0x99b720:
            assert read(read(sp+4))==BASE+0x680;word(self,BASE+0x680);event(13);cleanup=4;uc.reg_write(UC_X86_REG_EAX,self)
        elif a==0x99b510:
            assert read(self)==BASE+0x680;event(14)
        elif a==0x9db700:
            uc.mem_write(BASE+0x800,bytes(uc.mem_read(read(sp+4),12)))
            word(BASE+0x80c,read(sp+8));word(BASE+0x810,read(read(sp+20)))
            word(BASE+0x814,read(sp+16));word(BASE+0x818,read(read(sp+12)));word(BASE+0x81c,read(sp+24))
            event(15,self);cleanup=24;uc.reg_write(UC_X86_REG_EAX,1)
        elif a==release:event(7,self)
        else:event(8,read(sp+4))
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
    uc.hook_add(UC_HOOK_CODE,hook);word(BASE+0x3004,extent);cases=0
    heights=[0,15,16,18,24,28,0x80000001,0xffffffff]
    specs=[(0,h,m,r,0,0x204,0)for h,m,r in itertools.product(heights,range(3),range(4))]
    specs += [(1,h,m,r,refs,dest,variant)for h,m,r,refs,dest,variant in itertools.product(heights,range(3),range(4),range(3),[0x200,0x204],range(2))]
    specs += [(2,h,m,r,refs,0x204,v) for h,m,r,refs,v in itertools.product([15,16,24,0xffffffff],range(4),range(4),range(3),range(5))]
    for mode,height,mutation,rounding,refs,dest,variant in specs:
        arena=bytearray((i*19+cases)%256 for i in range(4096))
        def put(o,v):struct.pack_into('<I',arena,o,v)
        put(0x110,height);put(0x300,BASE+0x3000);put(0x400,refs);put(0x404,release);put(0x408,BASE+0x300);put(0x784,BASE+0x500)
        struct.pack_into('<2f',arena,0x620,*([1.25,2.5]if not variant else [-1.5,1000000000.0]))
        if mode==2:
            put(0x104,[0xff606060,0xff616060,0xff606160,0xff606061,0x007f80ff][variant])
            struct.pack_into('<2f',arena,0x108,-32.125,63.75)
            put(0x760,BASE+0x900);put(0x908,BASE+0xb00);put(0xa94,1365);put(0xa98,767)
            arena[0x850]=[0,128,255][refs]
        uc.mem_write(0x13750b0,bytes([arena[0x850]]));uc.mem_write(0x13961e8,struct.pack('<2d',128.125,256.25));calls=0
        inputs.extend(struct.pack('<4I',mode,mutation,rounding,dest));inputs.extend(arena);uc.mem_write(BASE,bytes(arena))
        args=(BASE+0x680,)if mode==2 else((BASE+0x680,BASE+0x200,BASE+dest)if mode else(BASE+0x180,BASE+0x200))
        uc.mem_write(stack,struct.pack('<'+'I'*(1+len(args)),stop,*args));uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE+0x100)
        uc.reg_write(UC_X86_REG_FPCW,0x37f|(rounding<<10));uc.emu_start({0:0x404970,1:0x404f60,2:0x405020}[mode],stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4*(len(args)+1)
        event(10);cases+=1
    return inputs,expected,dict(cases=cases,snapshots=events,bytes_per_snapshot=4096)

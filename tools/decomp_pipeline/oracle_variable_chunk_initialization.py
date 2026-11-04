"""Execute retail chunk initialization/list insertion with a recorded tree boundary."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000
WIDTH=0x8000
OWNER=BASE
CHUNK=0x2009000
OLD_CHUNK=0x200a000
AREAS=0x200b000
CASES=[(mode,alignment,size,count,pattern)
       for mode in [0,1,2]
       for alignment in ([4,8,16,32]if mode!=2 else[4])
       for size in ([64,4096,-1]if mode==0 else[0,31,4096,-1]if mode==1 else[-9,-8,0,32,128,129])
       for count in ([0,1]if mode==0 else[0]if mode==1 else[0,1,4])
       for pattern in [0x5a,0xa5]]

def initialize(write,args):
    mode,alignment,size,count,pattern=args
    write(BASE,bytes([pattern])*WIDTH)
    def word(address,value):write(address,struct.pack('<I',value&0xffffffff))
    word(0x13d28d4,alignment);word(0x13d28d8,{4:2,8:3,16:4,32:5}[alignment])
    word(OWNER+0xc1c,OLD_CHUNK if count and mode==0 else 0)
    word(OWNER+0xc34,AREAS+0xc0)
    if mode==2:
        word(CHUNK+0x18,OWNER);word(CHUNK+4,count);word(CHUNK+0xc,sum([-8,0,32,128][:count]))
        word(CHUNK+0x28,AREAS if count else 0);word(CHUNK+0x2c,AREAS+(count-1)*0x40 if count else 0)
        word(CHUNK+0x10,[-8,0,32,128][count-1]if count else 0);word(CHUNK+0x14,-8 if count else 0)
        for i in range(count):
            address=AREAS+i*0x40
            word(address,AREAS+(i+1)*0x40 if i+1<count else 0)
            word(address+8,AREAS+(i-1)*0x40 if i else 0)
            word(address+0x18,[-8,0,32,128][i]);word(address+0x14,CHUNK)
        word(CHUNK+0x80+0x18,size);word(CHUNK+0x80+0x14,CHUNK)

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[]
    def hook(uc,address,size,user):
        if address!=0xa64e70:return
        esp=uc.reg_read(UC_X86_REG_ESP);ret,root,area=struct.unpack('<3I',bytes(uc.mem_read(esp,12)))
        assert uc.reg_read(UC_X86_REG_ECX)==OWNER
        events.append(dict(owner=OWNER,root=root,area=area,before=bytes(uc.mem_read(BASE,WIDTH)).hex()))
        uc.reg_write(UC_X86_REG_ESP,esp+12);uc.reg_write(UC_X86_REG_EIP,ret)
    uc.hook_add(UC_HOOK_CODE,hook)
    for args in CASES:
        initialize(uc.mem_write,args);events.clear();mode,alignment,size,count,pattern=args
        if mode==0:entry=0xa65830;this=OWNER;parameters=[CHUNK,size]
        elif mode==1:entry=0xa65410;this=CHUNK;parameters=[CHUNK+0x80,size,OWNER]
        else:entry=0xa650f0;this=CHUNK;parameters=[CHUNK+0x80]
        uc.mem_write(stack,struct.pack('<'+str(len(parameters)+1)+'I',stop,*(v&0xffffffff for v in parameters)))
        uc.reg_write(UC_X86_REG_ECX,this);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(entry,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4*(len(parameters)+1)
        assert len(events)==1
        if mode==0:assert uc.reg_read(UC_X86_REG_EAX)==CHUNK
        rows.append(dict(args=args,after=bytes(uc.mem_read(BASE,WIDTH)).hex(),event=events[0]))
    return rows

if __name__=='__main__':print('VARIABLE_CHUNK_RETAIL cases='+str(len(collect())))

"""Execute retail initialization; record unrecovered initial chunk boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
CASES=[(minimum,initial,size,alignment,flags,internal)for alignment in [4,8,16,0,32]
       for minimum,initial,size,flags,internal in [(4096,0,0,0,0),(0,0x23450000,0,7,9),(-1,0,1024,0xffffffff,0x80000000),(65536,0x23450000,8192,3,5)]]

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);owner=0x2004000;chunk=0x2009000;stack=0x2010000;stop=0x200f000;rows=[];events=[]
    def hook(uc,address,size,user):
        if address not in [0xa65720,0xa65830]:return
        esp=uc.reg_read(UC_X86_REG_ESP);count=1 if address==0xa65720 else 2;words=struct.unpack('<'+str(count+1)+'I',bytes(uc.mem_read(esp,4*(count+1))))
        assert uc.reg_read(UC_X86_REG_ECX)==owner
        events.append(dict(kind=count,args=list(words[1:]),globals=bytes(uc.mem_read(0x13d28d4,8)).hex(),owner=bytes(uc.mem_read(owner,0xc38)).hex()))
        uc.reg_write(UC_X86_REG_EAX,chunk);uc.reg_write(UC_X86_REG_ESP,esp+4*(count+1));uc.reg_write(UC_X86_REG_EIP,words[0])
    uc.hook_add(UC_HOOK_CODE,hook)
    for args in CASES:
        for pattern in [0x5a,0xa5]:
            events.clear();uc.mem_write(owner-4,b'\x17'*4+bytes([pattern])*0xc38+b'\x31'*4);uc.mem_write(chunk,bytes([pattern])*0x34);uc.mem_write(0x13d28d4,b'\x77'*8)
            uc.mem_write(stack,struct.pack('<8I',stop,0x12345678,*(v&0xffffffff for v in args)));uc.reg_write(UC_X86_REG_ECX,owner);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0xa65930,stop,count=10000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+32 and uc.reg_read(UC_X86_REG_EAX)==owner
            assert bytes(uc.mem_read(owner-4,4))==b'\x17'*4 and bytes(uc.mem_read(owner+0xc38,4))==b'\x31'*4
            assert all(event['owner']==bytes(uc.mem_read(owner,0xc38)).hex() and event['globals']==bytes(uc.mem_read(0x13d28d4,8)).hex()for event in events)
            rows.append(dict(args=args,pattern=pattern,bytes=bytes(uc.mem_read(owner,0xc38)).hex(),chunk=bytes(uc.mem_read(chunk,0x34)).hex(),globals=bytes(uc.mem_read(0x13d28d4,8)).hex(),events=list(events)))
    return rows

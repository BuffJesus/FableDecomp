"""Retail execution check with string conversion and ASCII CRT boundaries."""
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from pe_oracle import EXE, pe_sections
CASES=[('enum E','enum',4,True),('preenum enumX enum_ enum2 enum E','enum',30,True),('enumX','enum',0,False),('#definition_template T X','#definition',0,False),('x #definition C N','#definition',13,True),('"quoted" enum E','enum',13,True)]
def main():
    raw=EXE.read_bytes();sections=pe_sections(raw)
    for source,needle,expected,success in CASES:
        u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
        for _,va,vs,off,size in sections:
            if size:u.mem_write(va+0x400000,raw[off:off+size])
        u.mem_map(0x2000000,0x20000)
        obj=0x2001000;string=0x2002000;arg=0x2003000;data=0x2003100;text=0x2003200;stack=0x2010000;stop=0x200f000
        u.mem_write(obj,bytes(40));u.mem_write(obj+4,struct.pack('<I',string));u.mem_write(string,source.encode()+bytes(1));u.mem_write(arg,struct.pack('<I',data));u.mem_write(data,struct.pack('<II',text,len(needle)));u.mem_write(text,needle.encode()+bytes(1));u.mem_write(stack,struct.pack('<II',stop,arg));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,obj)
        def ret(value):
            sp=u.reg_read(UC_X86_REG_ESP);dest=struct.unpack('<I',u.mem_read(sp,4))[0];u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_EIP,dest)
        def cstr(ptr):
            b=bytearray()
            while True:
                c=u.mem_read(ptr+len(b),1)[0]
                if not c:return bytes(b)
                b.append(c)
        def hook(uc,addr,size,user):
            if addr==stop:uc.emu_stop()
            elif addr==0x99e4c0:
                p=struct.unpack('<I',uc.mem_read(uc.reg_read(UC_X86_REG_ECX),4))[0];ret(struct.unpack('<I',uc.mem_read(p,4))[0])
            elif addr==0xbfeb3a:
                sp=uc.reg_read(UC_X86_REG_ESP);a,b=struct.unpack('<II',uc.mem_read(sp+4,8));i=cstr(a).find(cstr(b));ret(0 if i<0 else a+i)
            elif addr in (0xbfedb8,0xbfeda6):
                sp=uc.reg_read(UC_X86_REG_ESP);v=struct.unpack('<I',uc.mem_read(sp+4,4))[0];ret(int(chr(v).isalpha() if addr==0xbfedb8 else chr(v).isdigit()))
        u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x9b9c00,stop,count=100000)
        pos=struct.unpack('<I',u.mem_read(obj+0x20,4))[0];result=bool(u.reg_read(UC_X86_REG_EAX)&255)
        assert (pos,result)==(expected,success),(source,pos,result)
    print('RETAIL_WHOLE_STRING_ORACLE_PASS cases=6')
if __name__=='__main__':main()

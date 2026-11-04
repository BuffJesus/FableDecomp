"""Execute retail numeric-item converters with Windows x87 precision."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_FPCW
from pe_oracle import EXE,pe_sections
CASES={0x9b9700:['','0','123','-123','12-3','2147483647','2147483648','4294967296','-2147483648'],0x9b9770:['','0','1.25','-1.25','0.1','123456.789','1.2.3','-0.0','16777217.0','0.000000000000001']}
def collect():
    raw=EXE.read_bytes();sections=pe_sections(raw);rows=[]
    for address,cases in CASES.items():
        for text in cases:
            u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
            for _,va,vs,off,size in sections:
                if size:u.mem_write(va+0x400000,raw[off:off+size])
            u.mem_map(0x2000000,0x20000);obj=0x2001000;data=0x2002000;stack=0x2010000;stop=0x200f000
            u.mem_write(obj,bytes(12));u.mem_write(data,text.encode()+bytes(1));u.mem_write(stack,struct.pack('<III',stop,data,len(text)));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,obj);u.reg_write(UC_X86_REG_FPCW,0x027f)
            u.hook_add(UC_HOOK_CODE,lambda uc,va,size,user:uc.emu_stop() if va==stop else None);u.emu_start(address,stop,count=100000)
            kind,bits=struct.unpack('<II',u.mem_read(obj,8));rows.append(dict(address=address,text=text,kind=kind,bits=bits))
    return rows
if __name__=='__main__':
    import json
    print(json.dumps(collect(),indent=2))

"""Retail allocated-data and metadata-based aligned-header totals."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000
def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[]
    for mode,address in enumerate([0xa64db0,0xa64e20,0xa65210]):
     for alignment,shift in [(4,2),(8,3),(16,4),(32,5),(64,6),(128,7)]:
      for chunks in [0,1,3,7]:
       for areas in [0,1,3,7]:
        for sizes in [[0,32,64,128],[0x7fffffff,0x80000000,0xffffffff,1],[1,7,31,33]]:
         for metadata in [0,1,2]:
          for pattern in [0x5a,0xa5]:
           initial=bytearray(bytes([pattern])*WIDTH);struct.pack_into('<3I',initial,0xc14,alignment,shift,0x2009000 if chunks else 0)
           data=0;headers=0;controlled=0;area_header=(32+alignment-1)//alignment*alignment;chunk_header=(52+alignment-1)//alignment*alignment
           for i in range(chunks):
            a=0x2009000+i*0x80;first=0x2006000+i*0x400;count=areas if metadata==0 else(areas+2 if metadata==1 else 0x80000001)
            struct.pack_into('<I',initial,a-BASE,sizes[i%len(sizes)]);controlled+=sizes[i%len(sizes)]+area_header+chunk_header
            struct.pack_into('<I',initial,a-BASE+8,count);struct.pack_into('<I',initial,a-BASE+0x20,a+0x80 if i+1<chunks else 0)
            struct.pack_into('<I',initial,a-BASE+0x30,first if areas else 0);headers+=count*area_header+chunk_header
            for j in range(areas):
                b=first+j*0x40;value=sizes[(i+j)%len(sizes)];data+=value
                struct.pack_into('<I',initial,b-BASE,b+0x40 if j+1<areas else 0);struct.pack_into('<I',initial,b-BASE+0x18,value)
           expected=(controlled if mode==2 else headers+(data if mode==0 else 0))&0xffffffff
           uc.mem_write(BASE,bytes(initial));uc.mem_write(0x13d28d4,struct.pack('<2I',256,8))
           uc.mem_write(stack,struct.pack('<I',stop));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
           uc.emu_start(address,stop,count=4000)
           result=uc.reg_read(UC_X86_REG_EAX);globals_=struct.unpack('<2I',bytes(uc.mem_read(0x13d28d4,8)))
           assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4
           assert result==expected and globals_==(alignment,shift)
           assert bytes(uc.mem_read(BASE,WIDTH))==initial
           rows.append(dict(args=(mode,result,*globals_),initial=bytes(initial)))
    return rows

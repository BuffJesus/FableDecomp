"""Retail allocator enumeration, checked against independently built lists."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;OUTPUT=0x200b800

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[]
    for alignment,shift in [(4,2),(8,3),(16,4),(32,5),(64,6),(128,7)]:
     for chunks in [0,1,3,7]:
      for layout in [0,1,2]:
       for reverse in [False,True]:
        for pattern in [0x5a,0xa5]:
         initial=bytearray(bytes([pattern])*WIDTH)
         def word(address,value):struct.pack_into('<I',initial,address-BASE,value&0xffffffff)
         word(BASE+0xc14,alignment);word(BASE+0xc18,shift)
         chunk_addresses=[0x2006000+i*0x100 for i in range(chunks)]
         if reverse:chunk_addresses.reverse()
         word(BASE+0xc1c,chunk_addresses[0]if chunks else 0)
         ordered=[];serial=0
         for i,chunk in enumerate(chunk_addresses):
            word(chunk+0x20,chunk_addresses[i+1]if i+1<chunks else 0)
            size=0 if layout==0 else 1 if layout==1 else i%4
            nodes=[0x2008000+(serial+j)*0x100 for j in range(size)];serial+=size
            if reverse:nodes.reverse()
            word(chunk+0x30,nodes[0]if nodes else 0)
            word(chunk+8,0xffffffff-i) # Deliberately unrelated cached count.
            for j,node in enumerate(nodes):
                word(node,nodes[j+1]if j+1<size else 0);word(node+0x14,chunk)
                word(node+0x18,[0,0x80000000,0xffffffff][j%3])
            ordered.extend(nodes)
         header=(32+alignment-1)//alignment*alignment
         pointers=[node+header for node in ordered]
         total=len(pointers)
         requests=[(0,max_)for max_ in sorted(set([0,1,max(0,total-1),total,total+1,0xffffffff]))]
         requests.extend((1,index&0xffffffff)for index in sorted(set([-0x80000000,-1,*range(total),total,total+1,0x7fffffff])))
         for mode,arg in requests:
            expected=bytearray(initial)
            if mode==0:
                result=total
                for j,pointer in enumerate(pointers):struct.pack_into('<I',expected,OUTPUT-BASE+j*4,pointer)
                args=[OUTPUT,arg];entry=0xa65260
            else:
                result=pointers[arg]if arg<total else 0;args=[arg];entry=0xa652d0
            uc.mem_write(BASE,bytes(initial));uc.mem_write(0x13d28d4,struct.pack('<2I',256,8))
            uc.mem_write(stack,struct.pack('<'+str(len(args)+1)+'I',stop,*args))
            uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
            uc.emu_start(entry,stop,count=3000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4*(len(args)+1)
            assert uc.reg_read(UC_X86_REG_EAX)==result
            assert bytes(uc.mem_read(BASE,WIDTH))==expected
            assert struct.unpack('<2I',bytes(uc.mem_read(0x13d28d4,8)))==(alignment,shift)
            rows.append(dict(args=(mode,arg,result,alignment,shift),initial=bytes(initial),after=bytes(expected)))
    return rows

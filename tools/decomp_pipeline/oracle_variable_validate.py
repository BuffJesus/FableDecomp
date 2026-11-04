"""Retail validation traversal with real shipping no-op and recorded boundary."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;MAIN=0x2007800;ALTERNATE=0x2007900

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[];mode=0
    def word(a,v):uc.mem_write(a,struct.pack('<I',v))
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def hook(uc,address,size,user):
        if address!=0x9f1f40:return
        sp=uc.reg_read(UC_X86_REG_ESP)
        events.append((uc.reg_read(UC_X86_REG_ECX),read(sp+4),read(0x13d28d4),read(0x13d28d8),read(0x13caa90),bytes(uc.mem_read(BASE,WIDTH))))
        if mode==2 and len(events)==1:
            word(BASE+0xc14,128);word(BASE+0xc18,7)
            word(0x13d28d4,64);word(0x13d28d8,6);word(0x13caa90,ALTERNATE)
        if mode:
            uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
    uc.hook_add(UC_HOOK_CODE,hook)
    for mode in [0,1,2]:
     for alignment,shift in [(4,2),(8,3),(16,4),(32,5),(64,6),(128,7)]:
      for chunks in [0,1,3,7]:
       for layout in [0,1,2]:
        for reverse in [False,True]:
         for pattern in [0x5a,0xa5]:
            initial=bytearray(bytes([pattern])*WIDTH)
            def put(a,v):struct.pack_into('<I',initial,a-BASE,v)
            put(BASE+0xc14,alignment);put(BASE+0xc18,shift)
            chain=[0x2006000+i*0x100 for i in range(chunks)]
            if reverse:chain.reverse()
            put(BASE+0xc1c,chain[0]if chain else 0);ordered=[];serial=0
            for i,chunk in enumerate(chain):
                put(chunk+0x20,chain[i+1]if i+1<chunks else 0)
                n=0 if layout==0 else 1 if layout==1 else i%4
                nodes=[0x2008000+(serial+j)*0x100 for j in range(n)];serial+=n
                if reverse:nodes.reverse()
                put(chunk+0x30,nodes[0]if nodes else 0);put(chunk+8,0xffffffff-i)
                for j,node in enumerate(nodes):put(node,nodes[j+1]if j+1<n else 0)
                ordered.extend(nodes)
            uc.mem_write(BASE,bytes(initial));word(0x13d28d4,256);word(0x13d28d8,8);word(0x13caa90,MAIN)
            uc.mem_write(stack,struct.pack('<I',stop));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack);events=[]
            uc.emu_start(0xa65330,stop,count=3000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4
            mutated=mode==2 and bool(ordered);expected=bytearray(initial)
            if mutated:struct.pack_into('<2I',expected,0xc14,128,7)
            assert bytes(uc.mem_read(BASE,WIDTH))==expected and len(events)==len(ordered)
            for i,(this,data,a,s,singleton,state)in enumerate(events):
                changed=mode==2 and i>0;actual_alignment=64 if changed else alignment
                assert (this,data,a,s,singleton)==(ALTERNATE if changed else MAIN,ordered[i]+(32+actual_alignment-1)//actual_alignment*actual_alignment,actual_alignment,6 if changed else shift,ALTERNATE if changed else MAIN)
                assert state==(bytes(expected)if changed else bytes(initial))
            final=(64,6,ALTERNATE)if mutated else(alignment,shift,MAIN)
            assert (read(0x13d28d4),read(0x13d28d8),read(0x13caa90))==final
            rows.append(dict(args=(mode,len(events)if mode else 0,*final),initial=bytes(initial),after=bytes(expected),events=events if mode else []))
    return rows

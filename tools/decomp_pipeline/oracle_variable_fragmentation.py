"""Retail virtual fragmentation; recorded counts plus genuine native-count mode."""
import struct,math
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP,UC_X86_REG_FPCW,UC_X86_REG_FPSW,UC_X86_REG_FPTAG
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;TABLE=0x200b000;ALTERNATE=0x200b100
FREE=0x200d000;ALLOC=0x200d100;NEW_ALLOC=0x200d200
def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;slot=0x200e000;rows=[];events=[]
    uc.mem_write(stop,b'\xd9\x1d'+struct.pack('<I',slot)) # Oracle-only float32 return materialization.
    def hook(uc,address,size,user):
        if address not in (FREE,ALLOC,NEW_ALLOC):return
        kind={FREE:1,ALLOC:2,NEW_ALLOC:3}[address]
        assert uc.reg_read(UC_X86_REG_ECX)==BASE
        events.append((kind,*struct.unpack('<2I',bytes(uc.mem_read(0x13d28d4,8))),bytes(uc.mem_read(BASE,WIDTH))))
        if mode:
            uc.reg_write(UC_X86_REG_EIP,0xa64530 if kind==1 else 0xa64560);return
        sp=uc.reg_read(UC_X86_REG_ESP);ret=struct.unpack('<I',bytes(uc.mem_read(sp,4)))[0]
        if kind==1 and mutation:
            uc.mem_write(BASE,struct.pack('<I',ALTERNATE));uc.mem_write(BASE+0xc14,struct.pack('<2I',64,6));uc.mem_write(0x13d28d4,struct.pack('<2I',64,6))
        uc.reg_write(UC_X86_REG_EAX,free if kind==1 else allocated);uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
    uc.hook_add(UC_HOOK_CODE,hook)
    configs=[]
    for free in [0,1,2,3,7,0x7fffffff,0x80000000,0x80000001,0xfffffffe,0xffffffff]:
     for allocated in [0,1,2,3,7,0x7fffffff,0x80000000,0xffffffff]:
      for mutation in [0,1]:configs.append((0,free,allocated,mutation,[]))
    for values in [[0,0,0],[1,2,3],[0xffffffff,1,1],[0x80000000,0x7fffffff,2]]:
     for length in [0,1,3]:configs.append((1,sum(values[:length])&0xffffffff,0,0,values[:length]))
    for mode,free,allocated,mutation,values in configs:
     for alignment,shift in [(4,2),(128,7)]:
      for precision in [0,1]:
       for pattern in [0x5a,0xa5]:
        initial=bytearray(bytes([pattern])*WIDTH)
        def word(a,v):struct.pack_into('<I',initial,a-BASE,v)
        word(BASE,TABLE);struct.pack_into('<2I',initial,0xc14,alignment,shift);word(BASE+0xc1c,0x2009000 if values else 0)
        for table,callback in [(TABLE,ALLOC),(ALTERNATE,NEW_ALLOC)]:word(table+0x20,FREE);word(table+0x24,callback)
        for i,value in enumerate(values):
            chunk=0x2009000+i*0x80;word(chunk+4,value);word(chunk+8,17+i);word(chunk+0x20,chunk+0x80 if i+1<len(values)else 0)
        uc.mem_write(BASE,bytes(initial));uc.mem_write(0x13d28d4,struct.pack('<2I',256,8));events.clear()
        uc.reg_write(UC_X86_REG_FPCW,0x37f if precision else 0x27f);uc.reg_write(UC_X86_REG_FPSW,0);uc.reg_write(UC_X86_REG_FPTAG,0xffff)
        uc.mem_write(stack,struct.pack('<I',stop));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
        uc.emu_start(0xa64590,stop,count=5000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4
        uc.emu_start(stop,stop+6,count=2);bits=struct.unpack('<I',bytes(uc.mem_read(slot,4)))[0]
        expected=0.0 if free<=1 else(math.inf if allocated==0 else(free-1)/allocated)
        assert bits==struct.unpack('<I',struct.pack('<f',expected))[0],(free,allocated,precision,hex(bits),expected)
        assert [e[0]for e in events]==[1,3 if mutation else 2]
        after=bytes(uc.mem_read(BASE,WIDTH));globals_=struct.unpack('<2I',bytes(uc.mem_read(0x13d28d4,8)))
        wanted=bytearray(initial)
        if mutation:struct.pack_into('<I',wanted,0,ALTERNATE);struct.pack_into('<2I',wanted,0xc14,64,6)
        assert after==wanted and globals_==((64,6)if mutation else(alignment,shift))
        rows.append(dict(args=(mode,free,allocated,mutation,precision,bits,*globals_,len(events)),initial=bytes(initial),after=after,events=list(events)))
    return rows

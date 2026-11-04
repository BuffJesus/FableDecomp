"""Retail virtual Allocate; real best-fit/retry dispatcher, recorded split/NewChunk."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
from oracle_variable_tree_insertion import collect as trees,BASE,WIDTH,OWNER
ALIGN=0x13d28d4;SHIFT=0x13d28d8;RETURNED=0x200b800

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;events=[];rows=[]
    def hook(uc,address,size,user):
        if address not in (0xa65480,0xa65720):return
        sp=uc.reg_read(UC_X86_REG_ESP);ret,arg=struct.unpack('<2I',bytes(uc.mem_read(sp,8)))
        assert bytes(uc.mem_read(BASE,WIDTH))==initial
        globals_=struct.unpack('<2I',bytes(uc.mem_read(ALIGN,8)))
        if address==0xa65720:
            events.append((2,uc.reg_read(UC_X86_REG_ECX),arg,0,*globals_));value=0x200b000 if new_success else 0;pop=8
            if change_header:uc.mem_write(ALIGN,struct.pack('<2I',64,6))
        else:
            amount=struct.unpack('<I',bytes(uc.mem_read(sp+8,4)))[0]
            events.append((1,uc.reg_read(UC_X86_REG_ECX),arg,amount,*globals_))
            value=RETURNED if (retry_result if any(e[0]==2 for e in events)else first_result)else 0;pop=12
        uc.reg_write(UC_X86_REG_EAX,value);uc.reg_write(UC_X86_REG_ESP,sp+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    uc.hook_add(UC_HOOK_CODE,hook)
    empty=bytearray(b'\x5a'*WIDTH);struct.pack_into('<I',empty,0xc34,0)
    built=trees();states=[bytes(empty),bytes.fromhex(built[0]['snapshots'][-1]),bytes.fromhex(built[-1]['snapshots'][-1])]
    for state in states:
      for alignment in [4,8,16,32]:
       for request in [0,1,7,31,32,33,128,1024,0x7fffffff,0x80000000,0xfffffffe,0xffffffff]:
        for first_result in [0,1]:
         for new_success in [0,1]:
          for retry_result in [0,1]:
           for change_header in [0,1]:
            initial=bytearray(state);shift={4:2,8:3,16:4,32:5}[alignment];struct.pack_into('<2I',initial,0xc14,alignment,shift);initial=bytes(initial)
            uc.mem_write(BASE,initial);uc.mem_write(ALIGN,struct.pack('<2I',128,7));events.clear()
            uc.mem_write(stack,struct.pack('<2I',stop,request));uc.reg_write(UC_X86_REG_ECX,OWNER);uc.reg_write(UC_X86_REG_ESP,stack)
            uc.emu_start(0xa659c0,stop,count=20000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
            def word(a):return struct.unpack_from('<I',initial,a-BASE)[0]
            pending=[word(OWNER+0xc34)];nodes=[]
            while pending:
                a=pending.pop()
                if a:nodes.append((a,struct.unpack_from('<i',initial,a-BASE+0x18)[0]));pending.extend([word(a+4),word(a+0x10)])
            aligned=((request+alignment-1)&0xffffffff)>>shift<<shift;signed=aligned if aligned<0x80000000 else aligned-0x100000000
            eligible=[size for a,size in nodes if size>=signed];hit=bool(eligible);needs_new=not(hit and first_result)
            assert [e[0]for e in events]==(([1]if hit else[])+([2]+([1]if new_success and hit else[])if needs_new else[]))
            for kind,owner,arg,size,ga,gs in events:
                if kind==2:assert owner==OWNER and arg==request
                else:assert (arg,min(eligible))in nodes and size==aligned and owner==word(arg+0x14)
            final_globals=struct.unpack('<2I',bytes(uc.mem_read(ALIGN,8)))
            assert final_globals==((64,6)if needs_new and change_header else(alignment,shift))
            success=hit and(first_result or(new_success and retry_result));expected=RETURNED+(64 if needs_new and change_header else 32)if success else 0
            result=uc.reg_read(UC_X86_REG_EAX);assert result==expected
            assert bytes(uc.mem_read(BASE,WIDTH))==initial
            rows.append(dict(args=(request,first_result,new_success,retry_result,change_header,result,*final_globals),initial=initial,events=list(events)))
    return rows

if __name__=='__main__':print('VARIABLE_VIRTUAL_ALLOCATE_RETAIL cases=%d'%len(collect()))

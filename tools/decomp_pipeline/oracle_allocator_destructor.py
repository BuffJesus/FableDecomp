"""Run retail allocator destruction through recorded CRT/Win32 boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);owner=0x2001000;stack=0x2010000;stop=0x200f000;release=0x200e000;destroy=release+16;state={};rows=[]
    def read(address):return struct.unpack('<I',bytes(uc.mem_read(address,4)))[0]
    def write(address,value):uc.mem_write(address,struct.pack('<I',value))
    write(0x143fd40,release);write(0x143fde4,destroy)
    def hook(machine,address,size,user):
        if address not in (0xbfea14,release,destroy):return
        esp=machine.reg_read(UC_X86_REG_ESP);pointer=read(esp+4)
        if address==0xbfea14:state['order'].append(pointer);cleanup=4
        elif address==release:
            assert read(esp+8)==0 and read(esp+12)==0x8000
            state['order'].append(0x100000+pointer);cleanup=16
            if len(state['order'])==1+(1 if state['audit'] else 0)and state['mode']in(4,5):write(owner+0x410,3 if state['mode']==4 else 1)
            machine.reg_write(UC_X86_REG_EAX,0 if state['mode']==7 else 1)
        else:assert pointer==0x13caa70;state['order'].append(0xdead);cleanup=8
        machine.reg_write(UC_X86_REG_ESP,esp+cleanup);machine.reg_write(UC_X86_REG_EIP,read(esp))
    uc.hook_add(UC_HOOK_CODE,hook)
    for mode in range(8):
        count=[0,3,30,1,1,3,0,3][mode];audit=mode in(1,2,4,5,7)
        before=bytearray(b'\x77'*0x49c);struct.pack_into('<I',before,0x404,0x1100 if audit else 0);struct.pack_into('<I',before,0x410,count)
        for i in range(15):struct.pack_into('<I',before,0x460+i*4,0x3000+i*4 if mode==2 or mode in(1,3,6,7)and i in(0,7,14)else 0)
        uc.mem_write(owner,bytes(before))
        for i in range(30):write(0x1396880+i*28,0 if mode==3 else 0x2000+i*4)
        state.update(mode=mode,audit=audit,order=[]);write(stack,stop);uc.reg_write(UC_X86_REG_ECX,owner);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0x9f2310,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4
        final_count=read(owner+0x410);struct.pack_into('<I',before,0x410,final_count);assert bytes(uc.mem_read(owner,0x49c))==bytes(before)
        rows.append(dict(mode=mode,order=state['order'][:],final_count=final_count))
    return rows

if __name__=='__main__':print(collect())

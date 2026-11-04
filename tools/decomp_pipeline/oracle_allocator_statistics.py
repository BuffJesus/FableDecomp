"""Execute retail allocator aggregation with recording virtual allocators."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000)
    owner=0x2001000;stack=0x2010000;stop=0x200f000;enter=0x200e000;leave=enter+16
    uc.mem_write(0x143fe0c,struct.pack('<II',enter,leave));rows=[];state={}
    def read(address):return struct.unpack('<I',bytes(uc.mem_read(address,4)))[0]
    def write(address,value):uc.mem_write(address,struct.pack('<I',value))
    def hook(machine,address,size,user):
        if address in (enter,leave):
            esp=machine.reg_read(UC_X86_REG_ESP);assert read(esp+4)==0x13caa70
            state['held']+=1 if address==enter else -1
            machine.reg_write(UC_X86_REG_ESP,esp+8);machine.reg_write(UC_X86_REG_EIP,read(esp));return
        if 0x200d000<=address<0x200d020:
            assert address==0x200d000+state['slot']*4
            index=(machine.reg_read(UC_X86_REG_ECX)-0x2003000)//16
            assert 0<=index<30 and state['held']==state['initial']+1
            state['order'].append(index)
            if index==0 and state['mode']in(4,5):write(owner+0x410,3 if state['mode']==4 else 1)
            value=((0xfffffff0+index*17)&0xffffffff)if state['mode']==6 else (index+1)*(19 if state['slot']==0 else 7)
            esp=machine.reg_read(UC_X86_REG_ESP);machine.reg_write(UC_X86_REG_EAX,value);machine.reg_write(UC_X86_REG_ESP,esp+4);machine.reg_write(UC_X86_REG_EIP,read(esp))
    uc.hook_add(UC_HOOK_CODE,hook)
    for i in range(30):
        obj=0x2003000+i*16;table=0x2004000+i*64;write(obj,table);write(table+28,0x200d000);write(table+36,0x200d004);write(0x1396880+i*28,obj)
    for mode in range(9):
        for slot,address in enumerate((0x9f2450,0x9f2550)):
            count=[0,1,3,15,1,3,3,3,30][mode];initial=1 if mode==7 else 0
            state.update(mode=mode,slot=slot,held=initial,initial=initial,order=[]);write(owner+0x410,count);write(stack,stop)
            uc.reg_write(UC_X86_REG_ECX,owner);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(address,stop,count=10000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4 and state['held']==initial
            rows.append(dict(mode=mode,slot=slot,result=uc.reg_read(UC_X86_REG_EAX),order=state['order'][:],final_count=read(owner+0x410)))
    return rows

if __name__=='__main__':print(collect())

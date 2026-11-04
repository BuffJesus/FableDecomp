"""Execute retail creation and shutdown through the native destructor."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
SPECS=[(1,0,0,0),(0,0,0,0),(0,0,1,0),(0,0,17,0),(1,1,0,1),(0,1,0,1),(0,0,0,0),(0,0,0,0),(0,1,0,0),(0,0,2,1)]

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);owner=0x2004000;stack=0x2010000;stop=0x200f000;apis=[0x200e000+i*16 for i in range(5)];state={};rows=[]
    def read(address):return struct.unpack('<I',bytes(uc.mem_read(address,4)))[0]
    def write(address,value):uc.mem_write(address,struct.pack('<I',value))
    for slot,target in zip([0x143fd44,0x143fd3c,0x143fe20,0x143fde4,0x143fd40],apis):write(slot,target)
    def hook(machine,address,size,user):
        if address not in apis and address!=0x9f20b0:return
        esp=machine.reg_read(UC_X86_REG_ESP)
        if address==apis[0]:
            assert [read(esp+i*4)for i in range(1,5)]==[0,0x49c,0x3000,4];state['order'].append(1);machine.reg_write(UC_X86_REG_EAX,0 if state['fail']else owner);cleanup=20
        elif address==apis[1]:state['order'].append(2);machine.reg_write(UC_X86_REG_EAX,0x12340000);cleanup=4
        elif address==0x9f20b0:
            assert machine.reg_read(UC_X86_REG_ECX)==owner and read(esp+4)==0x12340000;state['order'].append(3)
            uc.mem_write(owner,bytes(0x49c));write(owner+0x408,0x12340000);write(owner+0x40c,state['count']);machine.reg_write(UC_X86_REG_EAX,owner);cleanup=8
        elif address==apis[2]:
            assert read(0x13caa90)==(0 if state['fail']else owner);state['order'].append(4);buffer=read(esp+4)
            uc.mem_write(buffer,struct.pack('<8I',32,51,16 if state['mode']==3 else 1024,32 if state['mode']==3 else 512,0,0,0,0));cleanup=8
        elif address==apis[3]:assert read(esp+4)==0x13caa70;state['order'].append(5);cleanup=8
        else:
            assert [read(esp+i*4)for i in range(1,4)]==[owner,0,0x8000];assert read(0x13caa90)==owner;state['order'].append(6);machine.reg_write(UC_X86_REG_EAX,0 if state['mode']==6 else 1);cleanup=16
        machine.reg_write(UC_X86_REG_ESP,esp+cleanup);machine.reg_write(UC_X86_REG_EIP,read(esp))
    uc.hook_add(UC_HOOK_CODE,hook)
    for mode,(fail,flag,count,old)in enumerate(SPECS):
        state.update(mode=mode,fail=fail,count=count,order=[]);write(0x13caa90,owner if old else 0);uc.mem_write(0x13caa94,bytes([flag]));write(0x13caa88,1234);write(0x13caab0,5678)
        uc.mem_write(owner,bytes(0x49c));write(owner+0x40c,count)
        def call(address):
            write(stack,stop);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(address,stop,count=10000);assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4
        if mode<7:call(0x9f2870)
        created=read(0x13caa90)!=0;baseline=[read(0x13caa88),read(0x13caab0)];create_flag=bytes(uc.mem_read(0x13caa94,1))[0]
        call(0x9f2d20);first_owner=read(0x13caa90)!=0;first_flag=bytes(uc.mem_read(0x13caa94,1))[0]
        if mode==9:write(owner+0x40c,0);call(0x9f2d20)
        rows.append(dict(mode=mode,created=int(created),baseline=baseline,create_flag=create_flag,first_owner=int(first_owner),first_flag=first_flag,final_owner=int(read(0x13caa90)!=0),final_flag=bytes(uc.mem_read(0x13caa94,1))[0],order=state['order'][:]))
    return rows

if __name__=='__main__':print(collect())

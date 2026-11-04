"""Retail CIME::Render replay with UI and cursor rendering boundaries recorded."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EAX,UC_X86_REG_ESP,UC_X86_REG_EIP
from gate_game_component_abi import DEFAULT_RETAIL,retail_bytes
BASE=0x24000000
def collect():
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1000000);uc.mem_map(BASE,65536)
    uc.mem_write(0x404c00,retail_bytes(DEFAULT_RETAIL.read_bytes(),0x404c00,69))
    inputs,expected=bytearray(),bytearray();mutation=events=0
    stack,stop,callback=BASE+0x8000,BASE+0xf000,BASE+0xf100
    def word(address,value):uc.mem_write(address,struct.pack('<I',value))
    def read(address):return struct.unpack('<I',uc.mem_read(address,4))[0]
    def event(kind,value=0):
        nonlocal events
        expected.extend(struct.pack('<II',kind,value));expected.extend(uc.mem_read(BASE,4096));expected.extend(uc.mem_read(0x13750b0,1));events+=1
    def hook(uc,address,size,user):
        if address not in (callback,0xcb38e0,0x41e5f2,0x41a980):return
        sp=uc.reg_read(UC_X86_REG_ESP)
        if address==callback:
            event(1,uc.reg_read(UC_X86_REG_ECX))
            if mutation&1:word(BASE+0x14c,0)
            uc.reg_write(UC_X86_REG_EAX,BASE+(0x420 if mutation&1 else 0x400))
        elif address==0xcb38e0:
            event(2,(uc.reg_read(UC_X86_REG_ECX)&255)|((uc.reg_read(UC_X86_REG_EDX)&255)<<8))
            if mutation&2:word(BASE+0x5b8,0 if read(BASE+0x5b8) else BASE+0x780)
        elif address==0x41e5f2:
            event(3);uc.reg_write(UC_X86_REG_EAX,BASE+(0x900 if mutation&4 else 0x500))
        else:event(4,uc.reg_read(UC_X86_REG_ECX))
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
    uc.hook_add(UC_HOOK_CODE,hook);word(BASE+0x306c,callback);cases=0
    for initialized,edit,cursor,alpha,mutation in itertools.product(range(2),range(2),range(2),[0,1,255],range(8)):
        arena=bytearray((i*11+cases)%256 for i in range(4096))
        def put(offset,value):struct.pack_into('<I',arena,offset,value)
        arena[0x108]=initialized;put(0x14c,BASE+0x300 if edit else 0);put(0x300,BASE+0x3000)
        arena[0x403]=alpha;arena[0x423]=alpha^255
        put(0x5b8,BASE+0x700 if cursor else 0);put(0x9b8,0 if cursor else BASE+0x7c0)
        initial_alpha=(cases*7)&255
        inputs.extend(struct.pack('<II',mutation,initial_alpha));inputs.extend(arena)
        uc.mem_write(BASE,bytes(arena));uc.mem_write(0x13750b0,bytes([initial_alpha]));word(stack,stop)
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,BASE+0x100)
        uc.emu_start(0x404c00,stop,count=1000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4
        event(5);cases+=1
    return inputs,expected,dict(cases=cases,snapshots=events,bytes_per_snapshot=4097)

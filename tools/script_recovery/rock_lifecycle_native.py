"""Execute original Init or root constructor on deliberately dirty storage."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_lifecycle import recover

def execute(constructor=False, sentinel=0xa5, empty=False):
    data=RData();_,w=recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(0x200000,0x20000)
    pages=set()
    for span in w['spans']:
        address=span['address'];page=address&~4095
        if page not in pages:u.mem_map(page,4096);pages.add(page)
        u.mem_write(address,data.bytes_at(address,span['size']))
    for page in (0xbfe000,0x99e000):
        if page not in pages:u.mem_map(page,4096)
    actor=0x201000;game=0x202000;table=0x203000;api=0x204000;end=0x205000;stack=0x21ef00
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    u.mem_write(actor,bytes([sentinel])*0x58)
    write(game,table);write(actor+4,game);write(actor+12,0 if empty else 0x207000)
    write(table+0x774,api);write(table+0x578,api+16);write(stack,end)
    for pc in (api,api+16,0xbfea1a,0xbfea0e,0x99ebf0,0x99eae0):u.mem_write(pc,b'\xc3')
    u.reg_write(UC_X86_REG_ECX,0x209000 if constructor else actor)
    u.reg_write(UC_X86_REG_EDX,game);u.reg_write(UC_X86_REG_ESP,stack)
    events=[];live=set()
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0xbfea1a:
            assert read(esp+4)==0x58;result=actor;pop=0
        elif pc==0xbfea0e:
            assert read(esp+4)==16;result=0x208000;pop=0
        elif pc==api:
            assert this==game and read(esp+4)==actor+8
            events.append(('persistent',read(esp+8)));pop=8
        elif pc==0x99ebf0:
            assert read(esp+4)==w['iconAddress'] and read(esp+8)==0xffffffff
            live.add(this);events.append(('string.new',w['icon']));pop=8
        elif pc==api+16:
            assert this==game and read(esp+4)==actor+8 and read(esp+8) in live
            events.append(('marker',w['icon']));pop=8
        elif pc==0x99eae0:
            live.remove(this);events.append(('string.destroy',));pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start(0xec5360 if constructor else 0xec4540,end,count=500)
    assert u.reg_read(UC_X86_REG_EIP)==end and not live
    return bytes(u.mem_read(actor,0x58)) if constructor else events

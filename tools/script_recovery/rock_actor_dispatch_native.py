"""Original retained-argument dispatch and destructor, independent of callback body."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_actor_phases import recover


def execute(empty=False,has_info=True):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x200000,0x20000)
    thread=0x201000;parent=0x202000;actor=0x203000;info=0x204000;callback=0x205000;end=0x206000;stack=0x21f000
    for page in (0xec5000,0x4aa000,0xcdd000):u.mem_map(page,4096)
    u.mem_write(0xec52f0,data.bytes_at(0xec52f0,50));u.mem_write(0xec5330,data.bytes_at(0xec5330,40))
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(thread+0x34,callback);write(thread+0x38,parent);write(thread+0x3c,0x1238c8c)
    write(thread+0x40,0 if empty else actor);write(thread+0x44,info if has_info else 0);write(info,2)
    events=[]
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);pop=None
        if pc==callback:
            assert uc.reg_read(UC_X86_REG_ECX)==parent
            assert read(esp+4)==0x1238c8c and read(esp+8)==(0 if empty else actor) and read(esp+12)==(info if has_info else 0)
            assert not has_info or read(info)==3
            events.append(('call.by.value',empty,has_info))
            if has_info:write(info,read(info)-1)
            pop=12
        elif pc==0x4aa840:
            assert uc.reg_read(UC_X86_REG_ECX)==thread+0x3c
            if has_info:write(info,read(info)-1)
            events.append(('stored.argument.destroy',));pop=0
        elif pc==0xcdd4c0:events.append(('thread.base.destroy',));pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook)
    write(stack,end);u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,thread)
    u.emu_start(0xec52f0,end,count=100)
    assert u.reg_read(UC_X86_REG_EIP)==end and u.mem_read(thread+5,1)==b'\1'
    assert not has_info or read(info)==2
    events.append(('dispatch.complete',))
    write(stack,end);write(stack+4,0);u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,thread)
    u.emu_start(0xec5330,end,count=100)
    assert u.reg_read(UC_X86_REG_EIP)==end and (not has_info or read(info)==1)
    return events

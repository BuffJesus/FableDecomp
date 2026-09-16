"""Original final targeting/wait/destruction instructions with raw pointer doubles."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX,UC_X86_REG_ESI
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_final_targeting import recover


def execute(targets=(1,2),stop=1):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x200000,0x20000)
    actor=0x201000;game=0x202000;table=0x203000;hero=0x204000;look=0x204010;enemy=0x204020;frame=0x204030;end=0x205000;stack=0x21ef00
    for page in (0xec4000,0xec5000,0xf35000,0x7e7000):u.mem_map(page,4096)
    u.mem_write(0xec4fa9,data.bytes_at(0xec4fa9,91))
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(actor+4,game);write(game,table);write(stack+108,end)
    for offset,callback in ((0x118,hero),(0x7c8,look),(0x70c,enemy),(0x1c,frame)):write(table+offset,callback)
    u.reg_write(UC_X86_REG_ESI,actor);u.reg_write(UC_X86_REG_ESP,stack);events=[];state={'heroes':0,'queries':0}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==hero:
            assert this==game;result=targets[state['heroes']];state['heroes']+=1;events.append(('hero',result));pop=0
        elif pc in (look,enemy):
            assert this==game and read(esp+4)==actor+8;events.append(('look' if pc==look else 'enemy',read(esp+8)));pop=8
        elif pc==frame:events.append(('frame',));pop=0
        elif pc==0xf35b30:
            assert this==actor;state['queries']+=1;result=state['queries']>=stop;events.append(('term',bool(result)));pop=0
        elif pc==0x7e74d0:
            assert this==stack+0x3c;events.append(('resource.destroy',));pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xec4fa9,end,count=500)
    assert u.reg_read(UC_X86_REG_EIP)==end;return events

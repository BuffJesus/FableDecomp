"""Original Sparrow instructions with acquisition and scheduler engine doubles."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_sparrow_recovery import recover


def execute(failures=0,stop=100,awake_frame=1,populated=True):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(0x200000,0x20000)
    actor=0x201000;parent=0x202000;game=0x203000;table=0x204000;frame=0x205000;acquire=0x205010;end=0x206000;stack=0x21f000
    for page in (0xec4000,0x4ab000,0xf35000,0xe24000,0x99a000,0xcd2000,0x7e7000):u.mem_map(page,4096)
    u.mem_write(0xec46b0,data.bytes_at(0xec46b0,259))
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(actor+4,game);write(actor+0x14,parent);write(game,table);write(table+0x1c,frame);write(table+0x20,acquire);write(stack,end)
    u.reg_write(UC_X86_REG_ECX,actor);u.reg_write(UC_X86_REG_ESP,stack)
    events=[];state={'frames':0,'queries':0,'attempts':0,'resource':None,'held':False}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x4abe90:assert read(esp+4)==actor+8;pop=4
        elif pc==0xf35b10:assert this==actor;events.append(('condition',));pop=4
        elif pc==0xe241b0:pop=0
        elif pc==frame:
            state['frames']+=1;u.mem_write(parent+0x4c,bytes([state['frames']>=awake_frame]));events.append(('frame',));pop=0
        elif pc==0xf35b30:
            state['queries']+=1;result=state['queries']>=stop;events.append(('term',bool(result)));pop=0
        elif pc==0x99a380:
            state['resource']=this;events.append(('new',));pop=0
        elif pc==0xcd23b9:assert this==state['resource'];events.append(('prepare',));pop=0
        elif pc==acquire:
            assert read(esp+4)==actor+8 and read(esp+8)==state['resource'] and read(esp+12)==4
            result=state['attempts']>=failures;state['attempts']+=1
            events.append(('acquire',4,bool(result),state['held']))
            state['held']=bool(result or populated);write(state['resource']+8,int(state['held']));pop=12
        elif pc==0x7e74d0:
            assert this==state['resource'];events.append(('destroy',state['held']));state['held']=False;pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xec46b0,end,count=5000)
    assert u.reg_read(UC_X86_REG_EIP)==end and not state['held']
    return events

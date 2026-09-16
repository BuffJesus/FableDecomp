"""Original home phase instructions; scoped getter and movement operand doubles."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EAX
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def execute(moves=1,busy=0,cancel=999,home=(7.0,-3.0,11.0)):
    data=RData();_,w=recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbb000,0xdbc000,0x7e7000,0xcbe000,0x99a000,0xf35000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;snapshot=0x204000;frame=0x204010;end=0x205000
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(owner+8,table);write(game,table);write(table+0x1c,snapshot)
    write(stack+0x14c,end)
    for pc in (snapshot,frame,0x7e7490,0xcbe45c,0x99a2e0,0x7e72f0,0x7e7450,0xf35b30,0x7e74d0,0x99a430):u.mem_write(pc,b'\xc3')
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_EBP,owner);u.reg_write(UC_X86_REG_ESI,owner+8);u.reg_write(UC_X86_REG_EDI,0)
    events=[];state={'moves':0,'busy':0,'queries':0,'success':False}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0xdbb53c:state['success']=True;uc.emu_stop();return
        if pc==snapshot:
            assert this==owner+8 and read(esp+4)==stack+0x64
            u.mem_write(stack+0x64,struct.pack('<fff',*home));events.append(('home',home));pop=4
            write(table+0x1c,frame)
        elif pc==frame:events.append(('frame',));pop=0
        elif pc==0x7e7490:
            assert this==stack+16;result=read(esp+4);assert result==stack+0x58
            write(result+8,0);events.append(('thing.new',));pop=4
        elif pc==0xcbe45c:
            assert this==stack+0x58 and uc.reg_read(UC_X86_REG_EDX)==stack+0x64 and read(esp+4)==0x40000000
            result=state['moves']<moves;events.append(('outside',bool(result)));pop=4
        elif pc==0x99a2e0:assert this==stack+0x58;events.append(('thing.destroy',));pop=0
        elif pc==0x7e72f0:
            assert this==stack+16 and read(esp+4)==stack+0x64
            args=[read(esp+i) for i in (8,12,16,20)];assert args==[0,0,0,1]
            events.append(('move',home,0.0,0,False,True));state['moves']+=1;state['busy']=busy;pop=20
        elif pc==0x7e7450:
            assert this==stack+16;result=state['busy']>0;state['busy']-=1;events.append(('busy',bool(result)));pop=0
        elif pc==0xf35b30:
            state['queries']+=1;result=state['queries']>=cancel;events.append(('term',bool(result)));pop=0
        elif pc in (0x7e74d0,0x99a430):events.append(('resource.destroy',));pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbb3fa,end,count=2000)
    assert state['success'] or u.reg_read(UC_X86_REG_EIP)==end
    return state['success'],events

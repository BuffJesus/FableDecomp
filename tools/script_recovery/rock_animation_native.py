"""Run original animation/helper/task instructions to continuation or unwind."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EBP
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_animation import recover


def execute(flags=(1,0),busy_polls=0,stop=100,empty=False):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x200000,0x20000)
    actor=0x201000;game=0x202000;table=0x203000;expert=0x204000;expert_table=0x205000
    frame=0x206000;play=0x206010;task=0x206020;end=0x207000;stack=0x21ef00;resource=stack+0x3c
    for page in (0xec4000,0xec5000,0x7e7000,0x99e000,0xf35000,0xcd2000,0x1375000):u.mem_map(page,4096)
    for address,size in [(0xec4eee,278),(0x7e73d0,15),(0x7e7450,16)]:u.mem_write(address,data.bytes_at(address,size))
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(actor+4,game);write(game,table);write(table+0x1c,frame);write(expert,expert_table);write(expert_table+0x48,play);write(expert_table+0x68,task)
    write(resource+8,0 if empty else expert);write(stack+108,end);u.mem_write(0x1375748,bytes([flags[0]]))
    u.reg_write(UC_X86_REG_ESI,actor);u.reg_write(UC_X86_REG_EBP,0);u.reg_write(UC_X86_REG_ESP,stack)
    events=[];strings={};state={'queries':0,'polls':0,'control':not empty,'open':True}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x99ebf0:
            name=data.string_at(read(esp+4));strings[this]=name;events.append(('string.new',name));pop=8
        elif pc in (0xec4f01,0xec4f37):
            assert len(strings)==1;events.append(('flag',int(uc.mem_read(0x1375748,1)[0])))
        elif pc==play:
            assert this==expert;name=strings[read(esp+4)];arguments=tuple(read(esp+8+4*i) for i in range(7))
            events.append(('play',name,arguments));pop=32
        elif pc==0x99eae0:
            name=strings.pop(this);events.append(('string.destroy',name))
            if name=='SPECIAL_BOAST':u.mem_write(0x1375748,bytes([flags[1]]))
            pop=0
        elif pc==task:
            assert not strings and this==expert;result=state['polls']<busy_polls;state['polls']+=1;events.append(('task',bool(result)));pop=0
        elif pc==0x7e7457:events.append(('task',False))
        elif pc==frame:events.append(('frame',));pop=0
        elif pc==0xf35b30:
            state['queries']+=1;result=state['queries']>=stop;events.append(('term',bool(result)));pop=0
        elif pc==0xcd23b9:assert this==resource;result=state['control'];events.append(('prepare',bool(result)));pop=0
        elif pc==0xcd2770:
            assert this==resource;state['control']=False;write(resource+8,0);events.append(('release.control',));pop=0
        elif pc==0x7e74d0:
            assert this==resource;events.append(('destroy',state['control']));state['control']=False;state['open']=False;pop=0
        elif pc==0xec4fa9:events.append(('continuation',));uc.emu_stop()
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xec4eee,end,count=2000)
    assert u.reg_read(UC_X86_REG_EIP) in (end,0xec4fa9) and not strings
    return events,state['control'],state['open']

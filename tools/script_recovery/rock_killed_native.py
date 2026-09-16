"""Run original by-value killed helper with retained argument/condition doubles."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_killed import recover


def execute(killed_poll=1,stop=100,empty=False,info=True,healthbar=0x12345678):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x200000,0x20000)
    owner=0x201000;game=0x202000;table=0x203000;thing=0x204000;thingtable=0x205000;refs=0x206000
    frame=0x207000;poll=0x207010;remove=0x207020;display=0x207030;end=0x208000;stack=0x21f000
    for page in (0xec5000,0x4ab000,0x4aa000,0xcb7000,0xe24000,0x99e000,0x99a000):u.mem_map(page,4096)
    u.mem_write(0xec5010,data.bytes_at(0xec5010,293))
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+0x40,game);write(owner+0x50,healthbar);write(game,table);write(table+0x1c,frame);write(table+0x548,remove);write(table+0x504,display)
    write(thing,thingtable);write(thingtable+0x48,poll);write(refs,2)
    write(stack,end);write(stack+4,0x1238c8c);write(stack+8,0 if empty else thing);write(stack+12,refs if info else 0)
    u.reg_write(UC_X86_REG_ECX,owner);u.reg_write(UC_X86_REG_ESP,stack)
    events=[];strings={};state={'queries':0,'polls':0}
    def release(pointer):
        reference=read(pointer+8)
        if reference:write(reference,read(reference)-1)
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x4abe90:
            source=read(esp+4);uc.mem_write(this,bytes(uc.mem_read(source,12)))
            if read(this+8):write(refs,read(refs)+1)
            pop=4
        elif pc==0xcb7920:
            assert this==owner
            if info:write(refs,read(refs)+1)
            events.append(('condition',));pop=4
        elif pc==0xe241b0:release(this+4);pop=0
        elif pc==frame:assert not strings;events.append(('frame',));pop=0
        elif pc==0xcb7940:
            assert not strings;state['queries']+=1;result=state['queries']>=stop;events.append(('term',bool(result)));pop=0
        elif pc==0x99ebf0:
            assert data.bytes_at(read(esp+4),1)==b'\0' and read(esp+8)==0xffffffff
            strings[this]='';events.append(('filter.new',));pop=8
        elif pc==poll:
            assert this==thing and read(esp+4) in strings
            state['polls']+=1;result=state['polls']>=killed_poll;events.append(('killed','',bool(result)));pop=4
        elif pc==0x99eae0:strings.pop(this);events.append(('filter.destroy',));pop=0
        elif pc==remove:
            assert not strings;events.append(('remove',struct.unpack('<i',uc.mem_read(esp+4,4))[0]));pop=4
        elif pc==display:events.append(('display',bool(read(esp+4))));pop=4
        elif pc==0x4aa840:release(this);events.append(('argument.destroy',));pop=0
        elif pc==0x99a2e0:events.append(('argument.destroy',));pop=0 # first-cancel inline release already executed
        elif pc==0xec5120:events.append(('MissionOver',True))
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xec5010,end,count=6000)
    assert u.reg_read(UC_X86_REG_EIP)==end and not strings
    assert not info or read(refs)==2 # thread-owned copy + registered condition clone remain
    return events,bool(u.mem_read(owner+0x4a,1)[0])

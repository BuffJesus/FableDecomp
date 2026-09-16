"""Original item phase plus real CDefString conversion/lookup instructions."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_rewards import recover


def execute(added=False,cancel=False,first=0,second=16,replace_definition=False):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x200000,0x20000)
    actor=0x201000;parent=0x202000;game=0x203000;table=0x204000;definition=0x205000;replacement=0x206000
    pool=0x207000;add=0x208000;end=0x209000;stack=0x21ef00
    for page in (0xec4000,0xec5000,0xf35000,0x415000,0x9d4000,0x995000,0x99e000,0x143e000,0x7e7000):u.mem_map(page,4096)
    u.mem_write(0xec4b81,data.bytes_at(0xec4b81,1155));u.mem_write(0x415d70,data.bytes_at(0x415d70,25));u.mem_write(0x9d49b0,data.bytes_at(0x9d49b0,59))
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(actor+0x14,parent);write(actor+4,game);write(game,table);write(table+0x924,add);write(0x143e90c,definition)
    write(definition+0x730,first);write(definition+0x734,second);write(replacement+0x734,32)
    u.mem_write(pool+4,b'REWARD_A\0');u.mem_write(pool+20,b'REWARD_B\0');u.mem_write(pool+36,b'REWARD_CHANGED\0')
    u.mem_write(parent+0x55,bytes([added]));write(stack+108,end)
    u.reg_write(UC_X86_REG_ESI,actor);u.reg_write(UC_X86_REG_EDI,actor+8);u.reg_write(UC_X86_REG_EBP,0);u.reg_write(UC_X86_REG_ESP,stack)
    events=[];strings={};calls=0
    def hook(uc,pc,size,user):
        nonlocal calls
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0xf35b30:events.append(('term',cancel));result=cancel;pop=0
        elif pc==0x415d70:events.append(('definition',this-(replacement if this>=replacement else definition)))
        elif pc==0x995e70:assert this==0x13ca834;result=pool;pop=0
        elif pc==0x99e4b0:strings[this]='';events.append(('string.new',''));result=this;pop=0
        elif pc==0x99ebf0:
            address=read(esp+4);raw=bytearray()
            while u.mem_read(address+len(raw),1)!=b'\0':raw+=u.mem_read(address+len(raw),1)
            name=raw.decode();strings[this]=name;events.append(('string.new',name));result=this;pop=8
        elif pc==add:
            assert this==game and read(esp+4)==actor+8 and len(strings)==1
            events.append(('add',strings[read(esp+8)]));calls+=1
            if calls==1 and replace_definition:write(0x143e90c,replacement)
            pop=8
        elif pc==0x99eae0:events.append(('string.destroy',strings.pop(this)));pop=0
        elif pc==0xec4bfd:assert not strings;events.append(('state',True))
        elif pc==0x7e74d0:events.append(('self.destroy',));pop=0
        elif pc==0xec4c01:events.append(('continuation',));uc.emu_stop()
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xec4b81,end,count=500)
    assert u.reg_read(UC_X86_REG_EIP) in (end,0xec4c01) and not strings
    return events,bool(u.mem_read(parent+0x55,1)[0])

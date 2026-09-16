"""Original hit-helper execution, including original x87 compare and BGRA bytes."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_hit import recover


def execute(hits=(False,True),abilities=(False,),stop=100,empty=False,health=1.0,bar_id=0x12345678):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x200000,0x20000)
    owner=0x201000;game=0x202000;table=0x203000;thing=0x204000;thingtable=0x205000;refs=0x206000
    frame=0x207000;hit=0x207010;ability=0x207020;gethealth=0x207030;add=0x207040;display=0x207050
    end=0x208000;health_value=0x209000;stack=0x21f000
    for page in (0xec5000,0xcb7000,0x99e000,0x99a000,0x4aa000,0x129b000):u.mem_map(page,4096)
    u.mem_write(0xec5140,data.bytes_at(0xec5140,421));u.mem_write(0x129ba3c,data.bytes_at(0x129ba3c,4))
    u.mem_write(health_value,struct.pack('<f',health));u.mem_write(gethealth,b'\xd9\x05'+struct.pack('<I',health_value)+b'\xc2\x04\x00')
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+0x40,game);write(owner+0x50,77);write(game,table)
    for offset,fn in [(0x1c,frame),(0x420,gethealth),(0x514,add),(0x504,display)]:write(table+offset,fn)
    write(thing,thingtable);write(thingtable+0x54,hit);write(thingtable+0xac,ability);write(refs,2)
    write(stack,end);write(stack+4,0x1238c8c);write(stack+8,0 if empty else thing);write(stack+12,refs)
    u.reg_write(UC_X86_REG_ECX,owner);u.reg_write(UC_X86_REG_ESP,stack)
    events=[];strings={};state={'queries':0,'hits':0,'abilities':0}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x99ebf0:
            name=data.string_at(read(esp+4));strings[this]=name;events.append(('string.new',name));pop=8
        elif pc==0x99eae0:events.append(('string.destroy',strings.pop(this)));pop=0
        elif pc in (hit,ability):
            key,values=('hits',hits) if pc==hit else ('abilities',abilities)
            assert this==thing and strings[read(esp+4)]=='SCRIPT_NAME_HERO'
            result=values[min(state[key],len(values)-1)];state[key]+=1
            events.append((key,bool(result)));pop=4
        elif pc==frame:assert not strings;events.append(('frame',));pop=0
        elif pc==0xcb7940:
            assert not strings;state['queries']+=1;result=state['queries']>=stop;events.append(('term',bool(result)));pop=0
        elif pc==gethealth:
            assert read(esp+4)==stack+4 and not strings;events.append(('health',)) # execute fld/ret engine double
        elif pc==add:
            assert read(esp+4)==stack+4
            colour=bytes(uc.mem_read(read(esp+8),4));texture=strings[read(esp+12)]
            scale=struct.unpack('<f',uc.mem_read(esp+16,4))[0]
            events.append(('bar',colour.hex(),texture,scale));result=bar_id;pop=16
        elif pc==0xec52bf:
            assert len(strings)==1;events.append(('state',bar_id))
        elif pc==display:assert not strings;events.append(('display',bool(read(esp+4))));pop=4
        elif pc==0x4aa840:
            assert not strings and this==stack+4;write(refs,read(refs)-1);events.append(('argument.destroy',));pop=0
        elif pc==0x99a2e0:events.append(('argument.destroy',));pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xec5140,end,count=10000)
    assert u.reg_read(UC_X86_REG_EIP)==end and not strings and read(refs)==1
    return events,struct.unpack('<i',u.mem_read(owner+0x50,4))[0]

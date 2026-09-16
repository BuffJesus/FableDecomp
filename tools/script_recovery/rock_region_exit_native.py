"""Execute original region-exit helper, tracing native CString scopes too."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_region_exit import recover


def execute(loaded_frames=0,stop=100):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(0x200000,0x20000)
    owner=0x201000;game=0x202000;table=0x203000;frame=0x204000;region=0x204010;generators=0x204020;end=0x205000;stack=0x21f000
    for page in (0xec4000,0x99e000,0xcb7000):u.mem_map(page,4096)
    u.mem_write(0xec4130,data.bytes_at(0xec4130,176))
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+0x40,game);write(game,table);write(table+0x1c,frame);write(table+0x30,region);write(table+0x8b8,generators);write(stack,end)
    u.reg_write(UC_X86_REG_ECX,owner);u.reg_write(UC_X86_REG_ESP,stack)
    events=[];state={'frames':0,'queries':0};strings={}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x99ebf0:
            assert not strings and read(esp+8)==0xffffffff
            strings[this]=data.string_at(read(esp+4));events.append(('string.new',strings[this]));pop=8
        elif pc==0x99eae0:
            events.append(('string.destroy',strings.pop(this)));pop=0
        elif pc==frame:
            assert not strings;state['frames']+=1;events.append(('frame',));pop=0
        elif pc==region:
            assert this==game;name=strings[read(esp+4)];result=state['frames']<loaded_frames
            events.append(('region',name,bool(result)));pop=4
        elif pc==generators:
            assert this==game;events.append(('generators',strings[read(esp+4)],bool(read(esp+8))));pop=8
        elif pc==0xcb7940:
            assert not strings and this==owner;state['queries']+=1;result=state['queries']>=stop
            events.append(('term',bool(result)));pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xec4130,end,count=5000)
    assert u.reg_read(UC_X86_REG_EIP)==end and not strings
    return events

"""Execute native Main tail; binding allocation and timer spawn are boundaries."""
import hashlib
import struct
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.readable_new_oakvale_main_bindings import NATIVE_SHA
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP,UC_X86_REG_ESI,UC_X86_REG_EBP


def execute(attack_over=False,terminate=False):
    d=RData();body=d.bytes_at(0xdabac0,2018)
    if hashlib.sha256(body).hexdigest()!=NATIVE_SHA:raise ValueError('Oakvale Main native changed')
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdab000,0xdac000,0xdbd000,0xcb7000,0xcb8000,0x99e000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(0xdabac0,body)
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    owner=0x201000;game=0x202000;table=0x203000;stack=0x21e000
    put(owner+0x40,game);put(game,table);u.mem_write(owner+0x50,bytes([attack_over]))
    put(table+0x100,0x204000);put(table+0x460,0x204010);events=[];texts={}
    for pc in (0xcb8930,0xcb7940,0x204000,0x204010,0x99ebf0,0x99eae0,0xdbde40):u.mem_write(pc,b'\xc3')
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;value=0
        if pc==0xdac158:events.append(('attack.over',attack_over))
        if pc in (0xdac198,0xdac219):
            events.append(('objective',) if pc==0xdac198 else ('thread','StartBarrelTimer'))
            uc.reg_write(UC_X86_REG_EIP,0xdac219 if pc==0xdac198 else 0xdac293);return
        if pc==0xcb8930:assert this==owner;events.append(('bindings.base.finalize',));pop=0
        elif pc==0x204000:assert this==game;events.append(('bindings.game.finalize',));pop=0
        elif pc==0xcb7940:assert this==owner;events.append(('term',terminate));value=terminate;pop=0
        elif pc==0x99ebf0:
            assert get(sp+8)==0xffffffff;texts[this]=d.string_at(get(sp+4));value=this;pop=8
        elif pc==0x99eae0:assert this in texts;texts.pop(this);pop=0
        elif pc==0x204010:
            assert this==game and get(sp+8)==0;events.append(('deactivate',texts[get(sp+4)]));pop=8
        elif pc==0xdbde40:assert this==owner;events.append(('mission',));pop=0
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(value));uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_ECX,owner);u.reg_write(UC_X86_REG_EBP,0)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdac146,0xdac29a,count=1000)
    assert not texts and u.reg_read(UC_X86_REG_ESP)==stack
    return events

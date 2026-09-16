"""Execute each original CString/raw-byte/action/dtor sequence and real forwarder."""
import struct
from tools.script_recovery.wife_animation_recovery import prove
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP,UC_X86_REG_ESI

def execute(kind=0,raw=1,populated=True):
    d=RData();prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb3000,0x99e000,0x7e7000,0x1375000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(0xdb3622,d.bytes_at(0xdb3622,129));u.mem_write(0x7e73d0,d.bytes_at(0x7e73d0,15))
    for pc in (0x99ebf0,0x99eae0,0x204000):u.mem_write(pc,b'\xc3')
    stack=0x21e000;owner=0x201000;parent=0x202000;expert=0x203000;table=0x203100
    def write(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+0x14,parent);write(stack+24,expert if populated else 0);write(expert,table);write(table+0x48,0x204000);u.mem_write(0x1375748,b'\x55')
    offsets=(0x2c,0x64);keys=('ST_ARGUING_POINT_AWAY','ST_ARGUING_POINT_AT');events=[];state={'key':False}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None
        if pc==0x99ebf0:
            assert this==stack+offsets[kind] and d.string_at(read(sp+4))==keys[kind] and read(sp+8)==0xffffffff
            state['key']=True;events.append(('key.new',keys[kind]));u.mem_write(0x1375748,bytes([raw]));pop=8
        elif pc in (0xdb3636,0xdb367b):assert state['key'];events.append(('raw',raw))
        elif pc==0x204000:
            assert this==expert and state['key'] and read(sp+4)==stack+offsets[kind]
            flags=[read(sp+i) for i in (8,12,16,20,24,28,32)];assert flags==[0,0,0,1,raw,0,0]
            events.append(('animation',keys[kind],*flags));pop=32
        elif pc==0x99eae0:assert this==stack+offsets[kind] and state['key'];state['key']=False;events.append(('key.destroy',keys[kind]));pop=0
        if pop is not None:uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EAX,parent);u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start((0xdb3622,0xdb3667)[kind],0xdb36a3,count=1000)
    assert not state['key'] and u.reg_read(UC_X86_REG_ESP)==stack;return events

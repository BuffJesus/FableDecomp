"""Execute each original getter/health/x87/destructor sequence, including far join."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EBP,UC_X86_REG_EAX,UC_X86_REG_EBX
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def execute(site,value,empty=False):
    data=RData();_,w=recover(data);row=w['healthSites'][site]
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbb000,0xdbc000,0x7e7000,0x4aa000,0x122d000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    u.mem_write(w['thresholdAddress'],bytes.fromhex(w['thresholdHex']))
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;api=0x204000;number=0x205000
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(game,table);write(table+0x420,api)
    u.mem_write(number,struct.pack('<f',value));u.mem_write(api,b'\xd9\x05'+struct.pack('<I',number)+b'\xc2\x04\x00')
    u.mem_write(0x7e7490,b'\xc3');u.mem_write(0x4aa840,b'\xc3')
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_EBP,owner)
    events=[];state={}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x7e7490:
            assert this==stack+16;state['output']=read(esp+4);write(state['output']+4,0 if empty else 0x206000)
            events.append(('thing.new',empty));pop=4;result=state['output']
        elif pc==api:
            assert this==game and read(esp+4)==state['output'];events.append(('health',empty))
        elif pc==0x4aa840:
            assert this==state['output'];events.append(('thing.destroy',));pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(row['start'],row['end'],count=100)
    assert u.reg_read(UC_X86_REG_EIP)==row['end']
    return bool(u.reg_read(UC_X86_REG_EBX)&255),events

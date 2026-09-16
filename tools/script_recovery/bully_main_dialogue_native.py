"""Original five main-dialogue branches, stopping at common movie cleanup."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def execute(done=False,attacked=False,said=False,hits=0,health=1.0,busy=0,cancel=999):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbb000,0xdbc000,0x7e7000,0x4aa000,0xf35000,0x122d000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(0xdbb000,data.bytes_at(0xdbb000,4096));u.mem_write(0xdbc000,data.bytes_at(0xdbc000,4096));u.mem_write(0x122dedc,data.bytes_at(0x122dedc,4))
    stack=0x21e000;owner=0x201000;parent=0x202000;game=0x203000;table=0x204000;healthapi=0x205000;number=0x205100
    def write(at,v):u.mem_write(at,struct.pack('<I',v))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(owner+20,parent);write(game,table);write(owner+0x20,hits)
    u.mem_write(owner+0x24,bytes([done,said]));u.mem_write(parent+0x6f,bytes([attacked]))
    write(table+0x420,healthapi);write(table+0x118,0x206000);write(table+0x1c,0x206010)
    u.mem_write(number,struct.pack('<f',health));u.mem_write(healthapi,b'\xd9\x05'+struct.pack('<I',number)+b'\xc2\x04\x00')
    for pc in (0x7e7490,0x4aa840,0x7e7390,0x7e7450,0xf35b30,0x206000,0x206010):u.mem_write(pc,b'\xc3')
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_EBP,owner)
    trace=[];state={'queries':0,'busy':busy,'output':None,'result':None}
    def hook(uc,pc,size,user):
        if pc in (0xdbc289,0xdbc1c3,0xdbc851):state['result']=pc==0xdbc289;uc.emu_stop();return
        if pc==0xdbbf49:trace.append(('state','DoneIntro',True))
        if pc==0xdbc03b:trace.append(('state','SaidPieceAboutAttackingVictim',True))
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x7e7490:assert this==stack+16;state['output']=read(sp+4);trace.append(('thing.new',));result=state['output'];pop=4
        elif pc==healthapi:assert read(sp+4)==state['output'];trace.append(('health',))
        elif pc==0x4aa840:assert this==state['output'];trace.append(('thing.destroy',));state['output']=None;pop=0
        elif pc==0xf35b30:state['queries']+=1;result=state['queries']>=cancel;trace.append(('term',bool(result)));pop=0
        elif pc==0x206000:trace.append(('hero',));result=0x207000;pop=0
        elif pc==0x206010:trace.append(('frame',));pop=0
        elif pc==0x7e7390:
            assert this==stack+16 and (read(sp+4),read(sp+12),read(sp+16),read(sp+20),read(sp+24))==(0x207000,0,0,1,0)
            trace.append(('speak',data.string_at(read(sp+8))));pop=24
        elif pc==0x7e7450:assert this==stack+16;result=state['busy']>0;state['busy']-=1;trace.append(('busy',bool(result)));pop=0
        if pop is not None:
            ret=read(sp);uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbbe8a,0xdbcfff,count=10000)
    assert state['result'] is not None and state['output'] is None
    return state['result'],trace

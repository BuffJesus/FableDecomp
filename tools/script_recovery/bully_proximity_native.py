"""Execute original proximity gates, formatted-text lifetime and animation calls."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def execute(timer=0,spoken=False,roll=0,modulus=3,near=True,hits=0,index=10,cancel=999,raw=255):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    targets=(0xbfeb16,0x99e4b0,0x99f1f0,0x99ebf0,0x99eae0,0xcbe2ff,0xf35b30,0x7e73d0)
    for page in {pc&~4095 for pc in targets}|{0xdbc000,0x143e000,0x13ac000,0x1375000}:u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(0xdbc000,data.bytes_at(0xdbc000,4096))
    stack=0x21e000;owner=0x201000;parent=0x202000;game=0x203000;table=0x204000;me=0x205000;hero=0x205100
    def write(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    def signed(v):return v-0x100000000 if v>=0x80000000 else v
    write(owner+4,game);write(owner+20,parent);write(game,table);write(0x143e8f8,game);write(parent+0x104,77)
    write(owner+0x20,hits);write(owner+0x28,index);u.mem_write(owner+0x26,bytes([spoken]))
    write(0x13ac860,modulus);u.mem_write(0x13ac85c,struct.pack('<f',7.25));u.mem_write(0x1375748,bytes([raw]))
    api={0x206000:'face',0x206010:'timer',0x206020:'settimer',0x206030:'hero',0x206040:'conversation',0x206050:'person',0x206060:'line'}
    for offset,pc in zip((0x76c,0x168,0x164,0x118,0x5b0,0x5b4,0x5b8),api):write(table+offset,pc)
    for pc in (*targets,*api):u.mem_write(pc,b'\xc3')
    for reg,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EBP,owner),(UC_X86_REG_EDI,me)):u.reg_write(reg,value)
    trace=[];strings={};state={'queries':0,'result':None}
    def hook(uc,pc,size,user):
        if pc in (0xdbc76f,0xdbcce2):state['result']=pc==0xdbc76f;uc.emu_stop();return
        if pc==0xdbc61b:trace.append(('state','SpokenOnFirstProximity',True))
        if pc==0xdbc63b:trace.append(('state','VictimShake',True))
        if pc==0xdbc6a6:trace.append(('state','IntimidateSpeechLoop',signed(read(owner+0x28))))
        if pc==0xdbc6b7:trace.append(('state','IntimidateSpeechLoop',10))
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x99e4b0:assert this==stack+84;strings[this]='';trace.append(('text.new',));pop=0
        elif pc==0x99f1f0:
            out=read(sp+4);assert out==stack+84 and strings[out]==''
            text=data.string_at(read(sp+8))%signed(read(sp+12));strings[out]=text;trace.append(('text.format',signed(read(sp+12))));pop=0
        elif pc==0x99ebf0:strings[this]=data.string_at(read(sp+4));trace.append(('animation.string.new',strings[this]));pop=8
        elif pc==0x99eae0:
            trace.append(('text.destroy',) if this==stack+84 else ('animation.string.destroy',strings[this]));strings.pop(this);pop=0
        elif pc==0xbfeb16:trace.append(('rand',roll));result=roll;pop=0
        elif pc==0xf35b30:
            state['queries']+=1;result=state['queries']>=cancel;trace.append(('term',bool(result)));pop=0
        elif pc==0xcbe2ff:
            assert this==me and uc.reg_read(UC_X86_REG_EDX)==hero and struct.unpack('<f',uc.mem_read(sp+4,4))[0]==7.25
            trace.append(('distance',7.25,near));result=near;pop=4
        elif pc==0x7e73d0:
            assert this==stack+16 and tuple(read(sp+4+i*4) for i in range(1,8))==(0,0,0,1,raw,0,0)
            assert stack+84 in strings;trace.append(('animation',strings[read(sp+4)],raw));pop=32
        elif pc in api:
            assert this==game;name=api[pc]
            if name=='face':assert (read(sp+4),read(sp+8),read(sp+12))==(me,stack+44,1);trace.append(('face',));pop=12
            elif name=='timer':assert read(sp+4)==77;trace.append(('timer',timer));result=timer;pop=4
            elif name=='settimer':assert (read(sp+4),read(sp+8))==(77,3);trace.append(('settimer',3));pop=8
            elif name=='hero':trace.append(('hero',));result=hero;pop=0
            elif name=='conversation':assert (read(sp+4),read(sp+8),read(sp+12))==(me,0,0);trace.append(('conversation',));result=73;pop=12
            elif name=='person':assert (read(sp+4),read(sp+8))==(73,stack+44);trace.append(('person',));pop=8
            else:
                assert (read(sp+4),read(sp+12),read(sp+16),read(sp+20))==(73,0,me,stack+44)
                trace.append(('line',strings[read(sp+8)]));pop=20
        if pop is not None:
            ret=read(sp);uc.reg_write(UC_X86_REG_EAX,int(result)&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbc588,0xdbcfff,count=10000)
    assert state['result'] is not None and not strings
    return state['result'],trace

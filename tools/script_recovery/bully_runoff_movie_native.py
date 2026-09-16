"""Execute original runoff map/movie instructions, mocking only engine calls."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from tools.script_recovery.bully_runoff_movie import evidence
from tools.script_recovery.lift_native_lua import RData

def execute(attacked=False,given=False,cancel=999):
    data=RData();evidence(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    targets=(0xcdbf70,0xcdbfb0,0x99ebf0,0x99eae0,0xcd3d2e,0x8abd10,
             0x9ac2d0,0x9ac700,0x99efe0,0x9ac310,0x6e7b60,0x6e7b80,
             0xcbfb7d,0xf35b30,0x7e74d0,0xdb0660)
    for page in {p&~4095 for p in targets}|{0xdbc000}:u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(0xdbc9ee,data.bytes_at(0xdbc9ee,0x2f4))
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;parent=0x204000;me=0x205000
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(owner+20,parent);write(game,table)
    u.mem_write(parent+0x6f,bytes([attacked]));u.mem_write(parent+0x6e,bytes([given]))
    apis={0x206000:'start',0x206010:'pause',0x206020:'camera',0x206030:'clear',0x206040:'remove'}
    for offset,pc in zip((0x5c8,0x5ec,0x5cc,0x5a4,0x1b0),apis):write(table+offset,pc)
    for pc in (*targets,*apis):u.mem_write(pc,b'\xc3')
    for reg,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EBP,owner),(UC_X86_REG_EDI,me)):u.reg_write(reg,value)
    events=[];strings={};state={'query':0,'key':None};cell=0x207000
    def hook(uc,pc,size,user):
        if pc==0xdbcce2:uc.emu_stop();return
        if pc==0xdbcbf2:events.append('state:GivenHeroTeddy')
        if pc==0xdbccbf:events.append('state:BullyRanOff')
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x99ebf0:strings[this]='' if read(sp+4)==0x122d70e else data.string_at(read(sp+4));pop=8
        elif pc==0x99eae0:strings.pop(this);pop=0
        elif pc==0xcdbf70:assert this==stack+100;events.append('new:actors');pop=0
        elif pc==0xcd3d2e:assert this==stack+100;state['key']=strings[read(sp+4)];result=cell;pop=4
        elif pc==0x8abd10:
            assert this==cell;role={60:'hero',116:'victim',16:'bully'}[read(sp+4)-stack]
            events.append('actor:'+state['key']+':'+role);pop=4
        elif pc==0x9ac2d0:assert this==stack+88;events.append('new:strings');pop=0
        elif pc==0x9ac700:assert this==stack+88;state['key']=strings[read(sp+4)];result=cell;pop=4
        elif pc==0x99efe0:assert this==cell;events.append('string:'+state['key']+':'+data.string_at(read(sp+4)));pop=4
        elif pc==0xf35b30:
            state['query']+=1;result=state['query']>=cancel;events.append('term:'+str(bool(result)).lower());pop=0
        elif pc==0x6e7b60:assert this==stack+240;events.append('new:movie');pop=0
        elif pc==0xcbfb7d:
            assert uc.reg_read(UC_X86_REG_EDX)==stack+100
            assert read(sp+4)==0 and read(sp+12)==0 and read(sp+16)==1
            inputs=read(sp+8);assert inputs in (0,stack+88)
            events.append('macro:'+strings[this]+':'+('strings' if inputs else 'null'));pop=16
        elif pc==0x6e7b80:assert this==stack+240;events.append('destroy:movie');pop=0
        elif pc==0x9ac310:assert this==stack+88;events.append('destroy:strings');pop=0
        elif pc==0xcdbfb0:assert this==stack+100;events.append('destroy:actors');pop=0
        elif pc==0x7e74d0:events.append('destroy:'+{116:'victim',60:'hero'}[this-stack]);pop=0
        elif pc==0xdb0660:assert this==parent;events.append('good.deed');pop=0
        elif pc in apis:
            assert this==game;name=apis[pc]
            if name=='start':assert strings[read(sp+4)]=='' and read(sp+8)==stack+240;pop=8
            elif name in ('pause','camera'):events.append(name+':'+str(bool(read(sp+4))).lower());pop=4
            elif name=='clear':assert read(sp+4)==stack+44;events.append('clear:victimThing');pop=4
            else:assert (read(sp+4),read(sp+8),read(sp+12))==(me,0,1);events.append('remove:me');pop=12
        if pop is not None:
            ret=read(sp);uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbc9ee,0xdbcce2,count=10000)
    assert u.reg_read(UC_X86_REG_EIP)==0xdbcce2
    assert not strings
    return events

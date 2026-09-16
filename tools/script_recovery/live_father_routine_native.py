"""Original recurring dispatcher; payment/hit interiors are verified boundaries."""
import struct
from tools.script_recovery.live_father_hit_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(talk=True,cancel=12,failures=0,prepare=False,payment=True,hit=True,populated=True):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb8000,0xdb9000,0x99e000,0xf35000,0x6e7000,0xcd2000,0x7e7000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    for address in (0x99ebf0,0x99eae0,0xf35b30,0x6e7b60,0x6e7b80,0xcd23b9,0xcd2770,0x7e74d0):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;done=0x204000
    def write(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    write(owner+4,game);write(game,table);write(owner+8,table);write(stack+0xf4,done)
    apis={0x207000:(0x6c,'talk'),0x207010:(0x5c8,'movie.start'),0x207020:(0x5ec,'pause'),0x207030:(0x20,'acquire'),0x207040:(0x1c,'frame')}
    for address,(slot,name) in apis.items():write(table+slot,address);u.mem_write(address,b'\xc3')
    events=[];texts={};state={'queries':0,'acquires':0,'movie':False,'control':True}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);result=0;pop=None
        if pc==0xdb8c53:
            assert state['movie'];events.append(('payment',payment));uc.reg_write(UC_X86_REG_EIP,0xdb9483 if payment else 0xdb974b);return
        if pc==0xdb9499:
            assert not state['movie'];events.append(('hit',hit));uc.reg_write(UC_X86_REG_EIP,0xdb9720 if hit else 0xdb9781);return
        if pc==0x99ebf0:
            assert read(sp+8)==0xffffffff;address=read(sp+4);texts[this]='' if address==0x122d70e else data.string_at(address);events.append(('text.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',texts.pop(this)));pop=0
        elif pc==0xf35b30:assert this==owner;state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x6e7b60:assert this==stack+40 and not state['movie'];state['movie']=True;events.append(('movie.new',));pop=0
        elif pc==0x6e7b80:assert this==stack+40 and state['movie'];state['movie']=False;events.append(('movie.destroy',));pop=0
        elif pc==0xcd23b9:assert this==stack+16;events.append(('prepare',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==stack+16;events.append(('prepare.release',));pop=0
        elif pc==0x7e74d0:assert this==stack+16 and state['control'];state['control']=False;events.append(('control.destroy',));pop=0
        elif pc in apis:
            name=apis[pc][1]
            if name=='talk':assert this==owner+8 and texts[read(sp+4)]=='SCRIPT_NAME_HERO';result=talk;events.append(('talk',result));pop=4
            elif name=='movie.start':assert this==game and texts[read(sp+4)]=='' and read(sp+8)==stack+40;events.append(('movie.start',));pop=8
            elif name=='pause':assert this==game;events.append(('pause',bool(read(sp+4))));pop=4
            elif name=='acquire':
                assert this==game and [read(sp+i) for i in (4,8)]==[owner+8,stack+16];priority=read(sp+12);assert priority in (3,4)
                result=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',priority,result,populated));pop=12
            elif name=='frame':assert this==game;events.append(('frame',));pop=0
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdb8aee,done,count=10000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not texts and not state['movie'] and not state['control']
    return events

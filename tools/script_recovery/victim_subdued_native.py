"""Original Victim branches and actual pushability argument-consuming callee."""
import struct
from functools import lru_cache
from tools.script_recovery.victim_subdued import recover
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP
@lru_cache(maxsize=1)
def fixture():
    data=RData();return data,recover(data)[1]

def execute(subdued=True,done=False,shake=False,ran_after=0,cancel=999,prepare=False,data_kind='empty',has_info=True):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbc000,0xdbd000,0xf35000,0xcd2000,0x4ab000,0x8a6000,0x99a000,0x40f000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']));u.mem_write(w['calleeAddress'],data.bytes_at(w['calleeAddress'],w['calleeSize']))
    for address in (0xf35b30,0xcd23b9,0xcd2770,0x4abe90):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;parent=0x204000;info=0x205000;proxy=0x206000;proxy_table=0x206100;getter=0x206200;thing=0x207000;end=0x207100;node=0x207200;component=0x208000
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(owner+0x14,parent);write(game,table);write(actor,table)
    write(actor+4,0 if data_kind=='empty' else proxy);write(actor+8,info if has_info else 0);write(info,7)
    write(proxy,proxy_table);write(proxy_table+0x2c,getter);write(thing+0x34,0x400);write(thing+0x48,end);write(node,0xaa);write(node+4,component);u.mem_write(component+0x79,b'\0')
    u.mem_write(parent+0x6c,bytes([int(subdued)]));u.mem_write(parent+0x48,bytes([int(shake)]));u.mem_write(owner+0x1d,bytes([int(done)]))
    apis={0x20a000:(0x7c0,'scared'),0x20a010:(0x844,'movement'),0x20a020:(0x5a4,'clear'),0x20a030:(0x76c,'face'),0x20a040:(0x1c,'frame')}
    for pc,(slot,_) in apis.items():write(table+slot,pc);u.mem_write(pc,b'\xc3')
    write(table+0xd30,0x8a6dd0);events=[];copies=[];state={'queries':0,'frames':0,'complete':False}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (w['phaseEnd'],w['cancel']):state['complete']=pc==w['phaseEnd'];uc.emu_stop();return
        if pc==0xdbce7a:events.append(('get','BullySubdued',subdued))
        if pc==0xdbce94:events.append(('get','DoneThanks',done))
        if pc==0xdbceb2:events.append(('set','DoneThanks',True))
        if pc==0xdbcf47:events.append(('get','VictimShake',shake))
        if pc==0xdbcf60:events.append(('set','VictimShake',False))
        if pc in (0xdbced9,0xdbcefa):
            value=state['frames']>=ran_after;u.mem_write(parent+0x6d,bytes([int(value)]));events.append(('get','BullyRanOff',value))
        if pc==0xf35b30:assert this==owner;state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0xcd23b9:assert this==stack+16;events.append(('prepare',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==stack+16;events.append(('prepare.release',));pop=0
        elif pc==0x4abe90:
            assert read(sp+4)==actor;u.mem_write(this,struct.pack('<III',0x1238c8c,read(actor+4),read(actor+8)))
            if has_info:write(info,read(info)+1)
            copies.append(this);result=this;events.append(('copy.new',));pop=4
        elif pc==0x8a6dd0:
            assert this==game and sp+4==copies[-1] and read(sp+16)==1
            if has_info:assert read(info)==8
            events.append(('pushable',True));return
        elif pc==0x99a2e0:assert this==copies.pop() and read(this+4)==0 and read(this+8)==0;events.append(('copy.destroy',));pop=0
        elif pc==getter:assert this==proxy;result=thing if data_kind=='valid' else 0;pop=0
        elif pc==0x40f020:assert this==thing+0x44;result=node;pop=4
        elif pc in apis:
            name=apis[pc][1];assert this==game
            if name=='frame':state['frames']+=1;events.append(('frame',));pop=0
            elif name=='face':assert [read(sp+i) for i in (4,8,12)]==[actor,stack+32,0];events.append(('face','victim','retainedBully',False));pop=12
            elif name=='clear':assert read(sp+4)==actor;events.append(('clear',));pop=4
            else:assert read(sp+4)==actor and read(sp+8)==(0 if name=='scared' else 1);events.append((name,name=='movement'));pop=8
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EDI,actor);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(w['phaseStart'],0x20f000,count=5000)
    assert not copies and read(info)==7
    return state['complete'],events,bool(u.mem_read(owner+0x1d,1)[0]),bool(u.mem_read(parent+0x48,1)[0])

"""Execute original talk/health/movie/UI instructions through all cleanup joins."""
import struct
from tools.script_recovery.victim_subdued_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(talk=True,subdued=False,attacked=False,displayed=False,xbox=False,health=1.0,busy=0,clicked_after=0,failures=0,cancel=999,prepare=False,populated=True):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbc000,0xdbd000,0xf35000,0x99e000,0x6e7000,0xcd2000,0x7e7000,0x4aa000,0x122d000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    for address in (0xf35b30,0x99ebf0,0x99eae0,0x6e7b60,0x6e7b80,0xcd23b9,0xcd2770,0x7e7490,0x4aa840,0x7e7390,0x7e7450):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;parent=0x204000;hero=0x205000;number=0x206000
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(owner+0x14,parent);write(game,table);write(actor,table);u.mem_write(parent+0x6c,bytes([int(subdued)]));u.mem_write(parent+0x6f,bytes([int(attacked)]));u.mem_write(owner+0x1c,bytes([int(displayed)]));u.mem_write(number,struct.pack('<f',health))
    slots={0x6c:('talk',1),0x7c0:('scared',2),0x118:('hero',0),0x76c:('face',3),0x5c8:('movie.start',2),0x5ec:('pause',1),0x20:('acquire',3),0x1c:('frame',0),0x420:('health',1),0x18:('xbox',0),0x1cc:('info',1),0xa0:('clicked',0)}
    apis={0x207000+i*16:(slot,*spec) for i,(slot,spec) in enumerate(slots.items())}
    for address,(slot,name,count) in apis.items():
        write(table+slot,address);u.mem_write(address,b'\xc3')
        if name=='health':u.mem_write(address,b'\xd9\x05'+struct.pack('<I',number)+b'\xc2\x04\x00')
    events=[];texts={};state={'queries':0,'acquires':0,'clicks':0,'busy':0,'movie':None,'thing':None,'complete':False}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (0xdbd60f,0xdbde18):state['complete']=pc==0xdbd60f;uc.emu_stop();return
        if pc==0xdbcfb7:events.append(('get','BullySubdued',subdued))
        if pc in (0xdbd085,0xdbd31d):events.append(('get','HeroAttackedVictim',attacked))
        if pc==0xdbd4ed:events.append(('get','DisplayedGameInfo',displayed))
        if pc==0xdbd60b:events.append(('set','DisplayedGameInfo',True))
        if pc==0xf35b30:assert this==owner;state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x99ebf0:
            assert read(sp+8)==0xffffffff;address=read(sp+4);texts[this]='' if address==0x122d70e else data.string_at(address);events.append(('text.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',texts.pop(this)));pop=0
        elif pc==0x6e7b60:assert this==stack+(144 if subdued else 160);state['movie']=this;events.append(('movie.new',));pop=0
        elif pc==0x6e7b80:assert this==state['movie'];state['movie']=None;events.append(('movie.destroy',));pop=0
        elif pc==0xcd23b9:assert this==stack+16 and state['movie'];events.append(('prepare',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==stack+16;events.append(('prepare.release',));pop=0
        elif pc==0x7e7490:
            assert this==stack+16;output=read(sp+4);expected=(280 if attacked else 244) if subdued else (220 if attacked else 232)
            assert output==stack+expected and not state['thing'];state['thing']=output;result=output;events.append(('thing.new',populated));pop=4
        elif pc==0x4aa840:assert this==state['thing'];state['thing']=None;events.append(('thing.destroy',));pop=0
        elif pc==0x7e7390:assert this==stack+16 and [read(sp+i) for i in (4,12,16,20,24)]==[hero,0,0,1,0];events.append(('speak',data.string_at(read(sp+8))));state['busy']=busy;pop=24
        elif pc==0x7e7450:assert this==stack+16;result=state['busy']>0;state['busy']-=1;events.append(('busy',result));pop=0
        elif pc in apis:
            slot,name,count=apis[pc]
            if name=='talk':assert this==actor and texts[read(sp+4)]=='SCRIPT_NAME_HERO';result=talk;events.append(('talk',talk))
            else:
                assert this==game
                if name=='scared':assert read(sp+4)==actor;events.append(('scared',bool(read(sp+8))))
                elif name=='hero':result=hero;events.append(('hero',))
                elif name=='face':assert read(sp+4)==actor and read(sp+12)==0;target=read(sp+8);assert target in (hero,stack+32);events.append(('face','hero' if target==hero else 'bully',False))
                elif name=='movie.start':assert texts[read(sp+4)]=='' and read(sp+8)==state['movie'];events.append(('movie.start',))
                elif name=='pause':events.append(('pause',bool(read(sp+4))))
                elif name=='acquire':assert [read(sp+i) for i in (4,8,12)]==[actor,stack+16,4];result=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',result,populated))
                elif name=='health':assert read(sp+4)==state['thing'];events.append(('health',));return
                elif name=='xbox':result=xbox;events.append(('xbox',xbox))
                elif name=='info':events.append(('info',texts[read(sp+4)]))
                elif name=='clicked':result=state['clicks']>=clicked_after;state['clicks']+=1;events.append(('clicked',result))
                else:events.append((name,))
            pop=count*4
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EDI,actor);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbcf76,0x20f000,count=7000)
    assert not texts and state['movie'] is None and state['thing'] is None
    return state['complete'],events,bool(u.mem_read(owner+0x1c,1)[0])

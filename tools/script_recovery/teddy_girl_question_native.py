"""Run original question phase with x87, CString scopes and signed answers."""
import struct
from tools.script_recovery.teddy_girl_health_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_EDI,UC_X86_REG_ESI,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(done=False,hit=False,health=1.0,busy=0,answers=(-1,1),cancel=999,*,talk=False,found=False,ruined=False,presented=None,failures=0,prepare=False,hit_phase=False):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdaf000,0xdb0000,0x7e7000,0x4aa000,0x99e000,0xf35000,0x122d000):u.mem_map(page,4096)
    for page in (0xcd2000,0x6e7000):u.mem_map(page,4096)
    u.mem_map(0xdae000,4096);u.mem_write(0xdaea70,b'\xc3')
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    u.mem_write(w['thresholdAddress'],bytes.fromhex(w['thresholdHex']))
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;healthapi=0x204000;heroapi=0x204020;frameapi=0x204040;questionapi=0x204060;answerapi=0x204080;number=0x205000
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(game,table);write(owner+0x14,0x208000)
    u.mem_write(owner+0x1d,bytes([int(found)]));u.mem_write(0x208091,bytes([int(ruined)]))
    write(table+0x5a4,0x2040a0)
    for slot,address in ((0x20,0x2040b0),(0x5c8,0x2040c0),(0x5ec,0x2040d0),(0x95c,0x2040e0)):write(table+slot,address)
    for offset,pc in ((0x420,healthapi),(0x118,heroapi),(0x1c,frameapi),(0x1c8,questionapi),(0x9c,answerapi)):write(table+offset,pc)
    u.mem_write(owner+0x1c,bytes([int(done)]));u.mem_write(owner+0x1f,bytes([int(hit)]))
    u.mem_write(number,struct.pack('<f',health));u.mem_write(healthapi,b'\xd9\x05'+struct.pack('<I',number)+b'\xc2\x04\x00')
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_EBP,owner);u.reg_write(UC_X86_REG_EDI,game);u.reg_write(UC_X86_REG_ESI,game if talk else 0)
    u.reg_write(UC_X86_REG_EBX,owner+8)
    if hit_phase:u.reg_write(UC_X86_REG_EDI,owner+8)
    events=[];live={};state={'queries':0,'busy':0,'answers':0,'success':False}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if (presented or hit_phase) and pc in (0xdafa9b,0xdb05da,0xdb042b):
            state['success']=pc in (0xdafa9b,0xdb042b);uc.emu_stop();return
        if hit_phase and pc==0xdb02b3:events.append(('set','HeroHitMe',True))
        if presented and pc==0xdaf75c:events.append(('get','FoundTeddy'))
        if presented and pc==0xdaf8f4:events.append(('set','DoneIntro',True))
        if pc in (0xdaf60b,0xdaf6d3,0xdaf38a,0xdb04ea,0xdb04fe,0xdb0513):
            state['success']=pc in (0xdaf60b,0xdaf6d3);uc.emu_stop();return
        if talk and pc in (0xdb0179,0xdb0590,0xdb05a2):
            state['success']=pc==0xdb0179;uc.emu_stop();return
        if talk and pc in (0xdafd91,0xdafd9c,0xdafe64,0xdaffec):
            events.append(('get',{0xdafd91:'FoundTeddy',0xdafd9c:'HeroHitMe',0xdafe64:'DoneIntro',0xdaffec:'TeddyRuined'}[pc]))
        if talk and pc in (0xdafe5b,0xdaff23):events.append(('set','DoneIntro',True))
        if pc==0xdaf2bf:events.append(('get','DoneIntro'))
        if pc==0xdaf2ca:events.append(('get','HeroHitMe'))
        if pc==0xdaf457:events.append(('set','DoneIntro',True))
        if pc==0x7e7490:
            assert this==stack+20;state['output']=read(esp+4);events.append(('thing.new',));result=state['output'];pop=4
        elif pc==healthapi:assert read(esp+4)==state['output'];events.append(('health',))
        elif pc==0x4aa840:assert this==state['output'];events.append(('thing.destroy',));pop=0
        elif pc==heroapi:events.append(('hero',));result=0x207000;pop=0
        elif pc==0x7e7390:
            assert this==stack+20 and read(esp+4)==0x207000
            assert [read(esp+i) for i in (12,16,20,24)]==[0,0,1,0]
            events.append(('speak',data.string_at(read(esp+8))));state['busy']=busy;pop=24
        elif pc==0x7e7450:
            assert this==stack+20;result=state['busy']>0;state['busy']-=1;events.append(('busy',bool(result)));pop=0
        elif pc==frameapi:events.append(('frame',));pop=0
        elif pc==0xf35b30:
            state['queries']+=1;result=state['queries']>=cancel;events.append(('term',bool(result)));pop=0
        elif pc==0x99ebf0:
            assert read(esp+8)==0xffffffff
            address=read(esp+4);live[this]='' if address==0x122d70e else data.string_at(address)
            events.append(('text.new',live[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',live.pop(this)));pop=0
        elif pc==questionapi:
            args=[live[read(esp+i)] for i in (4,8,12,16)]
            assert args==['TEXT_QST_048_GIVE_TEDDY_TO_GIRL','TEXT_OBJECT_HERO_ANSWER_YES','TEXT_OBJECT_HERO_ANSWER_NO',''] and read(esp+20)==1,(args,read(esp+20))
            events.append(('question',));pop=20
        elif pc==answerapi:
            result=answers[min(state['answers'],len(answers)-1)];state['answers']+=1;events.append(('answer',result));pop=0
        elif pc==0xdb0600:assert this==owner;events.append(('given',));pop=0
        elif pc==0x2040a0:assert read(esp+4)==owner+8;events.append(('clear',));pop=4
        elif pc==0xcd23b9:assert this==stack+20;events.append(('prepare.test',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==stack+20;events.append(('prepare.release',));pop=0
        elif pc==0x2040b0:
            assert [read(esp+i) for i in (4,8,12)]==[owner+8,stack+20,4]
            attempts=state.get('acquires',0);state['acquires']=attempts+1;result=attempts>=failures;events.append(('acquire',result));pop=12
        elif pc==0x6e7b60:
            assert this==stack+(184 if hit_phase else (168 if presented=='teddy' else 200));state['movie']=this;events.append(('movie.new',));pop=0
        elif pc==0x2040c0:
            assert live[read(esp+4)]=='' and read(esp+8)==state['movie'];events.append(('movie.start',));pop=8
        elif pc==0x2040d0:events.append(('pause',bool(read(esp+4))));pop=4
        elif pc==0x6e7b80:assert this==state.pop('movie');events.append(('movie.destroy',));pop=0
        elif pc==0x2040e0:
            events.append(('ally',*['girl' if read(esp+i)==owner+8 else 'hero' for i in (4,8)]));pop=8
        elif pc==0xdaea70:assert this==0x208000 and read(esp+4)==2;events.append(('badDeed',2));pop=4
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    start=0xdb0273 if hit_phase else ({'other':0xdaf74d,'teddy':0xdaf87a}[presented] if presented else (0xdafd91 if talk else 0xdaf2bf))
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(start,0x20f000,count=5000)
    assert not live
    return state['success'],events

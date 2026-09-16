"""Original payment dialogue instructions, including reloads and wraparound."""
import struct
from tools.script_recovery.live_father_hit_native import fixture
from tools.script_recovery.live_father_payment import SITES
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def signed(value):return (int(value)+0x80000000)%0x100000000-0x80000000

def execute(good=1,bad=0,paid=0,gold=3,chocolate=False,health=1.0,busy=0,cancel=999,mutation=None,active_quest='NewOakValeIntro'):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb8000,0xdb9000,0x99e000,0xf35000,0x7e7000,0x4aa000,0x122d000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']));u.mem_write(w['thresholdAddress'],bytes.fromhex(w['thresholdHex']))
    for address in (0x99ebf0,0x99eae0,0xf35b30,0x7e7490,0x4aa840,0x7e7390,0x7e7450):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;parent=0x204000;hero=0x205000;number=0x206000
    def write(a,v):u.mem_write(a,struct.pack('<I',int(v)&0xffffffff))
    def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    write(owner+4,game);write(owner+0x14,parent);write(game,table);write(parent+0x54,good);write(parent+0x58,bad);write(owner+0x1c,paid)
    apis={0x207000:(0x118,'hero'),0x207010:(0x420,'health'),0x207020:(0x1c,'frame'),0x207030:(0x1f8,'giveGold'),0x207040:(0x1fc,'gold'),0x207050:(0x2e0,'chocolate'),0x207060:(0x5a4,'clear'),0x207070:(0xa3c,'quest'),0x207080:(0x4a0,'objective')}
    for address,(slot,name) in apis.items():write(table+slot,address);u.mem_write(address,b'\xc3')
    u.mem_write(number,struct.pack('<f',health));u.mem_write(0x207010,b'\xd9\x05'+struct.pack('<I',number)+b'\xc2\x04\x00')
    events=[];texts={};state={'queries':0,'busy':0,'thing':None,'complete':False};sites=dict(SITES)
    reads={0xdb8c56:('GoodDeedsPerformed',parent+0x54),0xdb8c61:('BadDeedsPerformed',parent+0x58),0xdb8d39:('PenniesGiven',owner+0x1c),0xdb8d51:('PenniesGiven',owner+0x1c),0xdb8d57:('GoodDeedsPerformed',parent+0x54),0xdb8d70:('BadDeedsPerformed',parent+0x58),0xdb9214:('BadDeedsPerformed',parent+0x58)}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (0xdb9483,0xdb8d23,0xdb974b):state['complete']=pc==0xdb9483;uc.emu_stop();return
        if pc in reads:
            name,address=reads[pc];events.append(('get',name,signed(read(address))))
        if pc==0xdb8d5e:events.append(('set','PenniesGiven',signed(this)))
        if pc==0xf35b30:
            assert this==owner;state['queries']+=1
            if mutation and state['queries']==mutation[0]:
                _,g,b,p=mutation;write(parent+0x54,g);write(parent+0x58,b);write(owner+0x1c,p);events.append(('mutate',signed(g),signed(b),signed(p)))
            result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x7e7490:
            output=read(sp+4);assert this==stack+16 and output==stack+sites[read(sp)-5] and state['thing'] is None;state['thing']=output;events.append(('thing.new',));result=output;pop=4
        elif pc==0x4aa840:assert this==state['thing'];state['thing']=None;events.append(('thing.destroy',));pop=0
        elif pc==0x7e7390:
            assert this==stack+16 and [read(sp+i) for i in (4,12,16,20,24)]==[hero,0,0,1,0];events.append(('speak',data.string_at(read(sp+8))));state['busy']=busy;pop=24
        elif pc==0x7e7450:assert this==stack+16;result=state['busy']>0;state['busy']-=1;events.append(('busy',result));pop=0
        elif pc==0x99ebf0:
            assert read(sp+8)==0xffffffff;address=read(sp+4);texts[this]='' if address==0x122d70e else data.string_at(address);events.append(('text.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',texts.pop(this)));pop=0
        elif pc in apis:
            assert this==game;name=apis[pc][1]
            if name=='hero':events.append(('hero',));result=hero;pop=0
            elif name=='health':assert read(sp+4)==state['thing'];events.append(('health',))
            elif name=='frame':events.append(('frame',));pop=0
            elif name=='giveGold':events.append(('giveGold',signed(read(sp+4)),signed(read(owner+0x1c))));pop=4
            elif name=='gold':result=gold;events.append(('gold',signed(gold)));pop=0
            elif name=='chocolate':assert texts[read(sp+4)]=='OBJECT_CHOCOLATE_BOX_UNGIVEABLE' and read(sp+8)==hero;result=chocolate;events.append(('chocolate',chocolate));pop=8
            elif name=='clear':assert read(sp+4)==owner+8;events.append(('clear',));pop=4
            elif name=='quest':
                output=read(sp+4);assert output==stack+144;texts[output]=active_quest;events.append(('quest.new',active_quest));result=output;pop=4
            elif name=='objective':
                args=[texts[read(sp+i)] for i in (4,8,12,16)];assert args==[active_quest,'TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01','',''];events.append(('objective',*args));pop=16
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EBP,game);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdb8c53,0x20f000,count=8000)
    assert not texts and state['thing'] is None
    return state['complete'],events,signed(read(owner+0x1c))

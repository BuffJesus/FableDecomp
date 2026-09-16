"""Original optional talk branch, including CString/fresh-Hero ordering."""
import struct
from tools.script_recovery.guard_health_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(talked=True,bad=0,good=1,conversation=-7,busy=0,cancel=999):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdac000,0xdad000,0x99e000,0xcd2000,0xf35000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;parent=0x206000;hero=0x207000;talk=0x208000
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(owner+0x14,parent);write(actor,table);write(game,table);write(table+0x6c,talk)
    write(parent+0x58,bad);write(parent+0x54,good)
    apis={0x204000+i*16:offset for i,offset in enumerate((0x118,0x76c,0x5b0,0x5b4,0x5b8,0x5c0,0x1c))}
    for address,offset in apis.items():write(table+offset,address)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner)
    events=[];state={'queries':0,'busy':busy,'result':None};live={}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (0xdada6c,0xdaddcb,0xdade37,0xdade3b):state['result']=pc==0xdada6c;uc.emu_stop();return
        if pc==0xdad921:events.append(('get','BadDeedsPerformed'))
        if pc==0xdad928:events.append(('get','GoodDeedsPerformed'))
        if pc==0x99ebf0:
            assert read(esp+8)==0xffffffff;key=data.string_at(read(esp+4));live[this]=key;events.append(('text.new',key));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',live.pop(this)));pop=0
        elif pc==talk:
            assert this==actor and live[read(esp+4)]=='SCRIPT_NAME_HERO';events.append(('talk',talked));result=talked;pop=4
        elif pc==0xf35b30:
            state['queries']+=1;result=state['queries']>=cancel;events.append(('term',bool(result)));pop=0
        elif pc in (0xcd23b9,0xcd2770):
            assert this==stack+16
            if pc==0xcd23b9:events.append(('prepare',))
            pop=0
        elif pc in apis:
            kind=apis[pc];assert this==game
            if kind==0x118:events.append(('hero',));result=hero;pop=0
            elif kind==0x76c:
                assert [read(esp+i) for i in (4,8,12)]==[actor,hero,0];events.append(('face',False));pop=12
            elif kind==0x5b0:
                assert [read(esp+i) for i in (4,8,12)]==[actor,0,0];events.append(('conversation',conversation));result=conversation;pop=12
            elif kind==0x5b4:
                assert [read(esp+i) for i in (4,8)]==[conversation&0xffffffff,hero];events.append(('person',conversation));pop=8
            elif kind==0x5b8:
                assert read(esp+4)==conversation&0xffffffff and read(esp+12)==0 and read(esp+16)==actor and read(esp+20)==hero
                events.append(('line',conversation,live[read(esp+8)]));pop=20
            elif kind==0x5c0:
                assert read(esp+4)==conversation&0xffffffff;result=state['busy']>0;state['busy']-=1;events.append(('busy',conversation,bool(result)));pop=4
            elif kind==0x1c:events.append(('frame',));pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdad894,0xdade4b,count=2000)
    assert state['result'] is not None and not live
    return state['result'],events

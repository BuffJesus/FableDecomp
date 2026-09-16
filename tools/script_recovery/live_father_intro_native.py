"""Original intro movie/map/resource orchestration and native state writes."""
import struct
from tools.script_recovery.live_father_hit_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_EDX,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(xbox=False,clicked_after=0,cancel=999,failures=0,populated=True,counter=-1):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb8000,0xdb9000,0x99e000,0xf35000,0x6e7000,0xcdb000,0xcd3000,0x8ab000,0xcbf000,0x7e7000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    for address in (0x99ebf0,0x99eae0,0xf35b30,0x6e7b60,0x6e7b80,0xcdbf70,0xcdbfb0,0xcd3d2e,0x8abd10,0xcbfb7d,0x7e72a0,0x7e74d0):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;done=0x204000;parent=0x205000;hero=0x206000;entry=0x206100
    def write(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    write(owner+4,game);write(owner+0x14,parent);write(game,table);write(owner+8,table);write(stack+0xf4,done)
    slots={0x118:('hero',0),0x20:('acquire',3),0x1c:('frame',0),0x5c8:('movie.start',2),0x5ec:('pause',1),0x5cc:('camera',1),0xa68:('mute',1),0x5e0:('seconds',1),0x684:('reset',1),0x680:('default',0),0x18:('xbox',0),0x1cc:('info',1),0xa0:('clicked',0),0x51c:('counter',3),0x504:('displayQuest',1)}
    apis={0x207000+i*16:(slot,*spec) for i,(slot,spec) in enumerate(slots.items())}
    for address,(slot,name,count) in apis.items():write(table+slot,address);u.mem_write(address,b'\xc3')
    events=[];texts={};state={'queries':0,'acquires':0,'clicks':0,'hero':False,'map':False,'movie':False,'complete':False,'mapKey':None}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);result=0;pop=None
        if pc==0xdb8aee:state['complete']=True;uc.emu_stop();return
        if pc==0xdb8921:events.append(('setBool','DadFinishedIntro',True))
        if pc==0xdb8aac:events.append(('setInt','GUIGoodDeedCounter',counter));assert texts[stack+32]=='HUD_DEED_GOOD_ICON'
        if pc==0x99ebf0:
            assert read(sp+8)==0xffffffff;address=read(sp+4);texts[this]='' if address==0x122d70e else data.string_at(address);events.append(('text.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',texts.pop(this)));pop=0
        elif pc==0xf35b30:assert this==owner;state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x7e72a0:assert this==stack+40;state['hero']=True;events.append(('hero.new',));pop=0
        elif pc==0x7e74d0:
            assert this in (stack+16,stack+40)
            if this==stack+40:assert state['hero'];state['hero']=False;events.append(('hero.destroy',))
            else:events.append(('control.destroy',))
            pop=0
        elif pc==0xcdbf70:assert this==stack+60;state['map']=True;events.append(('map.new',));pop=0
        elif pc==0xcdbfb0:assert this==stack+60 and state['map'];state['map']=False;events.append(('map.destroy',));pop=0
        elif pc==0xcd3d2e:
            assert this==stack+60;state['mapKey']=texts[read(sp+4)];result=entry;pop=4
        elif pc==0x8abd10:
            assert this==entry;source=read(sp+4);assert source in (stack+16,stack+40)
            events.append(('map.set',state['mapKey'],'hero' if source==stack+40 else 'control'));result=entry;pop=4
        elif pc==0x6e7b60:assert this==stack+128 and not state['movie'];state['movie']=True;events.append(('movie.new',));pop=0
        elif pc==0x6e7b80:assert this==stack+128 and state['movie'];state['movie']=False;events.append(('movie.destroy',));pop=0
        elif pc==0xcbfb7d:
            assert texts[this]=='CS_OAKVALE_INTRO_FATHER' and uc.reg_read(UC_X86_REG_EDX)==stack+60 and [read(sp+i) for i in (4,8,12,16)]==[0,0,0,1]
            events.append(('macro',));pop=16
        elif pc in apis:
            slot,name,count=apis[pc];assert this==game
            if name=='hero':result=hero;events.append(('hero',))
            elif name=='acquire':
                assert [read(sp+i) for i in (4,8,12)]==[hero,stack+40,4];result=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',result,populated))
            elif name=='movie.start':assert texts[read(sp+4)]=='' and read(sp+8)==stack+128;events.append(('movie.start',))
            elif name in ('pause','camera','mute','displayQuest'):events.append((name,bool(read(sp+4))))
            elif name=='reset':assert read(sp+4)==0;events.append(('reset',0.0))
            elif name=='seconds':assert read(sp+4)==0x3f800000;events.append(('seconds',1.0))
            elif name=='xbox':result=xbox;events.append(('xbox',xbox))
            elif name=='info':events.append(('info',texts[read(sp+4)]))
            elif name=='clicked':result=state['clicks']>=clicked_after;state['clicks']+=1;events.append(('clicked',result))
            elif name=='counter':assert texts[read(sp+4)]=='HUD_DEED_GOOD_ICON' and [read(sp+i) for i in (8,12)]==[0,0x3f800000];result=counter;events.append(('counter',counter))
            else:events.append((name,))
            pop=count*4
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EBX,owner+8);u.reg_write(UC_X86_REG_EBP,0);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdb8798,done,count=10000)
    assert not texts and not state['movie'] and not state['map'] and not state['hero']
    return state['complete'],events

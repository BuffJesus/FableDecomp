"""Original hit predicates/state stores/acquisitions/map/movie/cleanup bytes."""
import struct
from tools.script_recovery.victim_subdued_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EDX,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(answers=(True,False,False),given=False,cancel=999,self_failures=0,hero_failures=0,prepare=False,populated=True,mask=256):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbc000,0xdbd000,0xdae000,0xf35000,0x99e000,0xcd2000,0x7e7000,0xcdb000,0xcd3000,0x8ab000,0x6e7000,0xcbf000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    for address in (0xf35b30,0x99ebf0,0x99eae0,0xdaea70,0xcd23b9,0xcd2770,0x7e72a0,0x7e74d0,0xcdbf70,0xcdbfb0,0xcd3d2e,0x8abd10,0x6e7b60,0x6e7b80,0xcbfb7d):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;parent=0x204000;hero=0x205000;entry=0x206000
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(owner+0x14,parent);write(game,table);write(actor,table);write(stack+48,mask);u.mem_write(parent+0x6e,bytes([int(given)]))
    slots={0x54:('ordinary',1),0xa8:('ability',1),0xa4:('excluded',2),0x118:('hero',0),0x95c:('ally',2),0x20:('acquire',3),0x1c:('frame',0),0x5c8:('movie.start',2),0x5ec:('pause',1),0x5cc:('camera',1),0x5a4:('clear',1),0x76c:('face',3)}
    apis={0x207000+i*16:(slot,*spec) for i,(slot,spec) in enumerate(slots.items())}
    for address,(slot,*_) in apis.items():write(table+slot,address);u.mem_write(address,b'\xc3')
    events=[];texts={};state={'queries':0,'selfAttempts':0,'heroAttempts':0,'hero':False,'map':False,'movie':False,'result':None,'key':None}
    def resource_name(value):assert value in (stack+16,stack+176);return 'self' if value==stack+16 else 'hero'
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (0xdbdc58,0xdbd9d0,0xdbde18):state['result']={0xdbdc58:'continue',0xdbd9d0:'repeat',0xdbde18:'cancel'}[pc];uc.emu_stop();return
        if pc==0xdbd735:events.append(('set','HeroAttackedVictim',True))
        if pc==0xdbd73c:events.append(('get','GivenHeroTeddy',given))
        if pc==0xdbd75d:events.append(('set','GivenHeroTeddy',True))
        if pc==0xf35b30:assert this==owner;state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x99ebf0:
            assert read(sp+8)==0xffffffff;address=read(sp+4);texts[this]='' if address==0x122d70e else data.string_at(address);events.append(('text.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',texts.pop(this)));pop=0
        elif pc==0xdaea70:assert this==parent and read(sp+4)==2;events.append(('badDeed',2));pop=4
        elif pc==0xcd23b9:events.append(('prepare',resource_name(this),prepare));result=prepare;pop=0
        elif pc==0xcd2770:events.append(('prepare.release',resource_name(this)));pop=0
        elif pc==0x7e72a0:assert this==stack+176;state['hero']=True;events.append(('resource.new','hero'));pop=0
        elif pc==0x7e74d0:assert this==stack+176 and state['hero'];state['hero']=False;events.append(('resource.destroy','hero'));pop=0
        elif pc==0xcdbf70:assert this==stack+192;state['map']=True;events.append(('map.new',));pop=0
        elif pc==0xcdbfb0:assert this==stack+192 and state['map'];state['map']=False;events.append(('map.destroy',));pop=0
        elif pc==0xcd3d2e:assert this==stack+192;state['key']=texts[read(sp+4)];result=entry;pop=4
        elif pc==0x8abd10:assert this==entry;source=read(sp+4);events.append(('map.set',state['key'],resource_name(source),populated));result=entry;pop=4
        elif pc==0x6e7b60:assert this==stack+204;state['movie']=True;events.append(('movie.new',));pop=0
        elif pc==0x6e7b80:assert this==stack+204 and state['movie'];state['movie']=False;events.append(('movie.destroy',));pop=0
        elif pc==0xcbfb7d:assert texts[this]=='CS_OAKVALEINTRO_BRATHIT' and uc.reg_read(UC_X86_REG_EDX)==stack+192 and [read(sp+i) for i in (4,8,12,16)]==[0,0,0,1];events.append(('macro',));pop=16
        elif pc in apis:
            slot,name,count=apis[pc]
            if name in ('ordinary','ability','excluded'):
                assert this==actor;index=('ordinary','ability','excluded').index(name);assert texts[read(sp+(8 if index==2 else 4))]=='SCRIPT_NAME_HERO'
                if index==2:assert read(sp+4)==14
                result=answers[index];events.append(('predicate',index,result))
            else:
                assert this==game
                if name=='hero':result=hero;events.append(('hero',))
                elif name=='ally':
                    args=[read(sp+i) for i in (4,8)];assert args in ([actor,hero],[hero,actor]);events.append(('ally',*['victim' if a==actor else 'hero' for a in args]))
                elif name=='acquire':
                    target,resource,priority=[read(sp+i) for i in (4,8,12)];which=resource_name(resource);assert target==(actor if which=='self' else hero) and priority==4
                    key=which+'Attempts';result=state[key]>=(self_failures if which=='self' else hero_failures);state[key]+=1;events.append(('acquire',which,result,populated))
                elif name=='movie.start':assert texts[read(sp+4)]=='' and read(sp+8)==stack+204;events.append(('movie.start',))
                elif name in ('pause','camera'):events.append((name,bool(read(sp+4))))
                elif name=='clear':assert read(sp+4)==actor;events.append(('clear',))
                elif name=='face':assert [read(sp+i) for i in (4,8,12)]==[actor,stack+32,0];events.append(('face','bully',False))
                else:events.append((name,))
            pop=count*4
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EDI,actor);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbd60f,0x20f000,count=7000)
    assert not texts and not state['hero'] and not state['map'] and not state['movie'] and read(stack+48)==mask
    return state['result'],events,bool(u.mem_read(parent+0x6e,1)[0]),bool(u.mem_read(parent+0x6f,1)[0])

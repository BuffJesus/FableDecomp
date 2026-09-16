"""Original native alert/chase/recheck instructions with changing engine inputs."""
import struct
from tools.script_recovery.guard_health_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(bad=1,dealt=0,distances=(True,False,False,True),failures=0,cancel=999,claim_on_frame=False,conversation=-7):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdac000,0xdad000,0x7e7000,0xcbe000,0xcd2000,0x99e000,0xf35000,0x13ac000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;parent=0x206000;hero=0x207000
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    def number(at):return struct.unpack('<f',u.mem_read(at,4))[0]
    write(owner+4,game);write(owner+0x14,parent);write(game,table);write(parent+0x58,bad);write(parent+0x68,dealt)
    apis={0x204000+i*16:offset for i,offset in enumerate((0x118,0x95c,0x20,0x1c,0x76c,0x5b0,0x5b4,0x5b8,0x800))}
    for address,offset in apis.items():write(table+offset,address)
    for i in range(-8,4):u.mem_write(0x13ac844+i*4,struct.pack('<f',7.25+i))
    u.mem_write(0x13ac840,struct.pack('<f',2.5))
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EBX,0)
    events=[];state={'queries':0,'distance':0,'acquires':0,'frames':0,'result':None}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        stops={0xdad894:'skip',0xdad87e:'claimed',0xdadd47:'cancel',0xdade37:'cancel',0xdade3b:'cancel',0xdaca19:'lecture'}
        if pc in stops:state['result']=stops[pc];uc.emu_stop();return
        if pc in (0xdac7a3,0xdac9f8):events.extend([('get','BadDeedsPerformed'),('get','GuardsDealtWithBadDeeds')])
        if pc==0xdac7c3:events.append(('get','BadDeedsPerformed'))
        if pc==0xdac7d3:
            index=uc.reg_read(UC_X86_REG_EAX);index=index if index<0x80000000 else index-0x100000000
            events.append(('alert',index,number((0x13ac844+index*4)&0xffffffff)))
        if pc in (0xdac8a1,0xdac987,0xdac9bd):events.append(('range',number(0x13ac840)))
        if pc==0xf35b30:
            state['queries']+=1;result=state['queries']>=cancel;events.append(('term',bool(result)));pop=0
        elif pc==0xcbe2ff:
            assert this==owner+8 and uc.reg_read(UC_X86_REG_EDX)==hero
            result=distances[min(state['distance'],len(distances)-1)];state['distance']+=1
            events.append(('distance',number(esp+4),bool(result)));pop=4
        elif pc in (0xcd23b9,0xcd2770):
            assert this==stack+16
            if pc==0xcd23b9:events.append(('prepare',))
            pop=0
        elif pc==0x99ebf0:
            assert read(esp+4)==0x12d8650 and read(esp+8)==0xffffffff
            state['text']=this;events.append(('text.new',data.string_at(read(esp+4))));pop=8
        elif pc==0x99eae0:assert this==state['text'];events.append(('text.destroy',));pop=0
        elif pc==0x7e7320:
            assert this==stack+16 and read(esp+4)==hero and read(esp+12)==1
            events.append(('follow',number(esp+8),True));pop=12
        elif pc in apis:
            kind=apis[pc];assert this==game
            if kind==0x118:events.append(('hero',));result=hero;pop=0
            elif kind==0x95c:
                args=[read(esp+4),read(esp+8)];assert sorted(args)==sorted([owner+8,hero])
                events.append(('ally','guard' if args[0]==owner+8 else 'hero'));pop=8
            elif kind==0x20:
                assert [read(esp+i) for i in (4,8,12)]==[owner+8,stack+16,4]
                state['acquires']+=1;result=state['acquires']>failures;events.append(('acquire',bool(result)));pop=12
            elif kind==0x1c:
                state['frames']+=1;events.append(('frame',))
                u.mem_write(0x13ac840,struct.pack('<f',2.5+state['frames']))
                if claim_on_frame:write(parent+0x68,bad)
                pop=0
            elif kind==0x76c:
                assert [read(esp+i) for i in (4,8,12)]==[owner+8,hero,1];events.append(('face',True));pop=12
            elif kind==0x5b0:
                assert [read(esp+i) for i in (4,8,12)]==[owner+8,0,0];events.append(('conversation',conversation));result=conversation;pop=12
            elif kind==0x5b4:
                assert [read(esp+i) for i in (4,8)]==[conversation&0xffffffff,hero];events.append(('person',conversation));pop=8
            elif kind==0x5b8:
                assert [read(esp+i) for i in (4,8,12,16,20)]==[conversation&0xffffffff,state['text'],0,owner+8,hero]
                events.append(('line',conversation));pop=20
            elif kind==0x800:
                assert read(esp+4)==owner+8;events.append(('cutscene',read(esp+8)));pop=8
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdac7a0,0xdade4b,count=10000)
    assert state['result'] is not None
    return state['result'],events

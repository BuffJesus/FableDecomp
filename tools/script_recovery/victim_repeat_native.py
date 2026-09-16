"""Original repeat-hit bytes and real retained Thing IsAlive implementation."""
import struct
from tools.script_recovery.victim_subdued_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(has_data=True,alive=True,health=1.0,busy=0,failures=0,cancel=999,prepare=False,conversation=-1,populated=True):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbc000,0xdbd000,0xf35000,0x99e000,0x6e7000,0xcd2000,0x7e7000,0x4aa000,0x4ab000,0x122d000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']));u.mem_write(0x4ab130,data.bytes_at(0x4ab130,28))
    for address in (0xf35b30,0x99ebf0,0x99eae0,0x6e7b60,0x6e7b80,0xcd23b9,0xcd2770,0x7e7490,0x4aa840,0x7e7390,0x7e7450):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;hero=0x205000;proxy=0x206000;ptab=0x206100;number=0x206300
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(game,table);write(actor,table);write(proxy,ptab);write(ptab+0x12c,0x208000);u.mem_write(0x208000,b'\xc3');u.mem_write(number,struct.pack('<f',health))
    slots={0x5b0:('conversation',3),0x120:('lookup',2),0x5b4:('person',2),0x5b8:('line',5),0x5c8:('movie.start',2),0x5ec:('pause',1),0x20:('acquire',3),0x1c:('frame',0),0x420:('health',1),0x118:('hero',0)}
    apis={0x207000+i*16:(slot,*spec) for i,(slot,spec) in enumerate(slots.items())}
    for address,(slot,name,_) in apis.items():
        write(table+slot,address);u.mem_write(address,b'\xc3')
        if name=='health':u.mem_write(address,b'\xd9\x05'+struct.pack('<I',number)+b'\xc2\x04\x00')
    events=[];texts={};state={'queries':0,'acquires':0,'busy':0,'bully':False,'movie':False,'health':False,'complete':False}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (0xdbdc58,0xdbde18):state['complete']=pc==0xdbdc58;uc.emu_stop();return
        if pc==0xdbda26:events.append(('alive',bool(uc.reg_read(UC_X86_REG_EAX)&255)))
        if pc==0xf35b30:assert this==owner;state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x99ebf0:
            assert read(sp+8)==0xffffffff;address=read(sp+4);texts[this]='' if address==0x122d70e else data.string_at(address);events.append(('text.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',texts.pop(this)));pop=0
        elif pc==0x208000:assert this==proxy;result=alive;pop=0
        elif pc==0x6e7b60:assert this==stack+80;state['movie']=True;events.append(('movie.new',));pop=0
        elif pc==0x6e7b80:assert this==stack+80 and state['movie'];state['movie']=False;events.append(('movie.destroy',));pop=0
        elif pc==0xcd23b9:assert this==stack+16;events.append(('prepare',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==stack+16;events.append(('prepare.release',));pop=0
        elif pc==0x7e7490:assert this==stack+16 and read(sp+4)==stack+256;state['health']=True;result=stack+256;events.append(('healthThing.new',populated));pop=4
        elif pc==0x4aa840:
            if this==stack+56:assert state['bully'];state['bully']=False;events.append(('bully.destroy',))
            else:assert this==stack+256 and state['health'];state['health']=False;events.append(('healthThing.destroy',))
            pop=0
        elif pc==0x7e7390:assert this==stack+16 and [read(sp+i) for i in (4,12,16,20,24)]==[hero,2,0,1,0];events.append(('speak',data.string_at(read(sp+8)),2));state['busy']=busy;pop=24
        elif pc==0x7e7450:assert this==stack+16;result=state['busy']>0;state['busy']-=1;events.append(('busy',result));pop=0
        elif pc in apis:
            slot,name,count=apis[pc];assert this==game
            if name=='conversation':assert [read(sp+i) for i in (4,8,12)]==[actor,0,0];result=conversation;events.append(('conversation',conversation))
            elif name=='lookup':assert read(sp+4)==stack+56 and texts[read(sp+8)]=='NOVI_Bully';write(stack+60,proxy if has_data else 0);state['bully']=True;result=stack+56;events.append(('bully.new',has_data))
            elif name=='person':assert [read(sp+i) for i in (4,8)]==[conversation&0xffffffff,stack+56];events.append(('person',conversation,'bully'))
            elif name=='line':assert [read(sp+i) for i in (4,12,16,20)]==[conversation&0xffffffff,0,actor,stack+56];events.append(('line',conversation,texts[read(sp+8)],'victim','bully'))
            elif name=='movie.start':assert texts[read(sp+4)]=='' and read(sp+8)==stack+80;events.append(('movie.start',))
            elif name=='pause':events.append(('pause',bool(read(sp+4))))
            elif name=='acquire':assert [read(sp+i) for i in (4,8,12)]==[actor,stack+16,4];result=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',result,populated))
            elif name=='health':assert read(sp+4)==stack+256;events.append(('health',));return
            elif name=='hero':result=hero;events.append(('hero',))
            else:events.append((name,))
            pop=count*4
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EDI,actor);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbd9d0,0x20f000,count=7000)
    assert not texts and not state['bully'] and not state['movie'] and not state['health'];return state['complete'],events

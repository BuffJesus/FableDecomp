"""Original departure instructions with injected native predicate results."""
import struct
from tools.script_recovery.teddy_girl_health_native import fixture
from tools.script_recovery.teddy_girl_movement import recover
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(spoke=True,near=True,onscreen=(True,False),far=False,failures=0,prepare=False,cancel=999):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdaf000,0xdb0000,0x99e000,0xcd2000,0xcbe000,0xf35000,0x7e7000,0x4aa000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    for address in (0x99ebf0,0x99eae0,0x99efe0,0xcbe2ff,0xcbe3ea,0xcd23b9,0xcd2770,0xf35b30,0x7e7300,0x4aa840):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;parent=0x204000;master=0x205000;hero=0x206000;position=0x207000;actor=owner+8
    def write(a,v):u.mem_write(a,struct.pack('<I',v))
    def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    write(owner+4,game);write(owner+0x14,parent);write(owner+0x18,master);write(game,table);write(actor,table);u.mem_write(parent+0x70,bytes([spoke]))
    apis={0x208000:(0x118,'hero'),0x208010:(0x5b0,'conversation'),0x208020:(0x5b4,'person'),0x208030:(0x5b8,'line'),0x208040:(0x20,'acquire'),0x208050:(0x1c,'frame'),0x208060:(0x120,'lookup'),0x208070:(0x694,'screen'),0x208080:(0x1b0,'remove'),0x208090:(0x18,'position')}
    for pc,(slot,name) in apis.items():write(table+slot,pc)
    events=[];live={};state={'queries':0,'acquires':0,'screens':0,'success':False,'target':False}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);result=0;pop=None
        if pc in (0xdafc99,0xdb05da):state['success']=pc==0xdafc99;uc.emu_stop();return
        if pc==0xdafa9e:events.append(('get','SpokeAboutFindingTeddy'))
        if pc in (0xcbe2ff,0xcbe3ea):
            from unicorn.x86_const import UC_X86_REG_EDX
            assert this==actor and uc.reg_read(UC_X86_REG_EDX)==(stack+40 if pc==0xcbe2ff else hero)
            assert read(sp+4)==(0x41200000 if pc==0xcbe2ff else 0x41a00000)
            result=near if pc==0xcbe2ff else far;events.append(('near' if pc==0xcbe2ff else 'far',result));pop=4
        elif pc==0xf35b30:state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0xcd23b9:assert this==stack+20;events.append(('prepare.test',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==stack+20;events.append(('prepare.release',));pop=0
        elif pc==0x99ebf0:assert read(sp+8)==0xffffffff;live[this]=data.string_at(read(sp+4));events.append(('text.new',live[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',live.pop(this)));pop=0
        elif pc==0x99efe0:assert this==master+0x54;events.append(('master',data.string_at(read(sp+4))));pop=4
        elif pc==0x7e7300:
            assert this==stack+20 and [read(sp+i) for i in range(4,32,4)]==[stack+156,0x40400000,1,0,0,0,1]
            events.append(('move',));pop=28
        elif pc==0x4aa840:assert this==stack+156 and state['target'];state['target']=False;events.append(('target.destroy',));pop=0
        elif pc in apis:
            name=apis[pc][1]
            if name=='hero':result=hero;events.append(('hero',));pop=0
            elif name=='conversation':assert [read(sp+i) for i in (4,8,12)]==[actor,0,0];result=37;events.append(('conversation',));pop=12
            elif name=='person':assert [read(sp+i) for i in (4,8)]==[37,hero];events.append(('person',));pop=8
            elif name=='line':assert [read(sp+i) for i in (4,12,16,20)]==[37,0,actor,hero];events.append(('line',live[read(sp+8)]));pop=20
            elif name=='acquire':
                assert [read(sp+i) for i in (4,8,12)]==[actor,stack+20,4];result=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',result));pop=12
            elif name=='frame':events.append(('frame',));pop=0
            elif name=='lookup':assert read(sp+4)==stack+156;state['target']=True;events.append(('target.new',live[read(sp+8)]));result=stack+156;pop=8
            elif name=='position':assert this==actor;result=position;events.append(('position',));pop=0
            elif name=='screen':assert read(sp+4)==position;result=onscreen[min(state['screens'],len(onscreen)-1)];state['screens']+=1;events.append(('screen',result));pop=4
            elif name=='remove':assert [read(sp+i) for i in (4,8,12)]==[actor,0,1];events.append(('remove',));pop=12
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_EBP,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdafa9b,0x20f000,count=4000)
    assert not live and not state['target']
    return state['success'],events

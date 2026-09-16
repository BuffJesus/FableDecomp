"""Original complaint bytes; retained caller resources stop before cleanup."""
import struct
from tools.script_recovery.victim_subdued_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP
def execute(complaint=True,health=1.0,busy=0,failures=0,cancel=999,prepare=False,populated=True):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbc000,0xdbd000,0xf35000,0xcd2000,0x7e7000,0x4aa000,0x122d000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    for address in (0xf35b30,0xcd23b9,0xcd2770,0x7e7490,0x4aa840,0x7e7390,0x7e7450):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;parent=0x204000;hero=0x205000;number=0x206000
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(owner+0x14,parent);write(game,table);u.mem_write(parent+150,bytes([int(complaint)]));u.mem_write(number,struct.pack('<f',health))
    apis={0x207000:(0x20,'acquire',3),0x207010:(0x1c,'frame',0),0x207020:(0x118,'hero',0),0x207030:(0x420,'health',1)}
    for address,(slot,name,_) in apis.items():write(table+slot,address);u.mem_write(address,b'\xc3')
    u.mem_write(0x207030,b'\xd9\x05'+struct.pack('<I',number)+b'\xc2\x04\x00');events=[];state={'queries':0,'acquires':0,'busy':0,'thing':False,'complete':False}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (0xdbdd8d,0xdbde18):state['complete']=pc==0xdbdd8d;uc.emu_stop();return
        if pc==0xdbdc5b:events.append(('get',complaint))
        if pc==0xdbdd86:events.append(('set',False))
        if pc==0xf35b30:assert this==owner;state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0xcd23b9:assert this==stack+16;events.append(('prepare',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==stack+16;events.append(('prepare.release',));pop=0
        elif pc==0x7e7490:assert this==stack+16 and read(sp+4)==stack+268;state['thing']=True;events.append(('thing.new',populated));result=stack+268;pop=4
        elif pc==0x4aa840:assert this==stack+268 and state['thing'];state['thing']=False;events.append(('thing.destroy',));pop=0
        elif pc==0x7e7390:assert this==stack+16 and [read(sp+i) for i in (4,12,16,20,24)]==[hero,0,0,1,0] and data.string_at(read(sp+8))=='TEXT_QST_048_VICTIM_EVIL_BROS_10';events.append(('speak',));state['busy']=busy;pop=24
        elif pc==0x7e7450:assert this==stack+16;result=state['busy']>0;state['busy']-=1;events.append(('busy',result));pop=0
        elif pc in apis:
            slot,name,count=apis[pc];assert this==game
            if name=='acquire':assert [read(sp+i) for i in (4,8,12)]==[actor,stack+16,4];result=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',result,populated))
            elif name=='hero':events.append(('hero',));result=hero
            elif name=='health':assert read(sp+4)==stack+268;events.append(('health',));return
            else:events.append((name,))
            pop=count*4
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EDI,actor);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbdc58,0x20f000,count=7000)
    assert not state['thing'];return state['complete'],events,bool(u.mem_read(parent+150,1)[0])

"""Execute original LiveFather hit/mask/movie/acquisition/health instructions."""
import struct
from functools import lru_cache
from tools.script_recovery.live_father_hit import recover
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

@lru_cache(maxsize=1)
def fixture():
    data=RData();return data,recover(data)[1]

def execute(answers=(True,False,False),health=1.0,busy=0,failures=0,prepare=False,cancel=999,populated=True,mask=0):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb8000,0xdb9000,0xdae000,0x99e000,0xf35000,0x6e7000,0xcd2000,0x7e7000,0x4aa000,0x122d000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']));u.mem_write(w['thresholdAddress'],bytes.fromhex(w['thresholdHex']))
    for address in (0x99ebf0,0x99eae0,0xf35b30,0xdaea70,0x6e7b60,0x6e7b80,0xcd23b9,0xcd2770,0x7e7490,0x4aa840,0x7e7390,0x7e7450):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;parent=0x204000;hero=0x205000;number=0x206000
    def write(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    write(owner+4,game);write(owner+0x14,parent);write(game,table);write(owner+8,table);write(stack+56,mask)
    apis={0x207000:(0x54,'ordinary'),0x207010:(0xa8,'ability'),0x207020:(0xa4,'excluded'),0x207030:(0x118,'hero'),0x207040:(0x95c,'ally'),0x207050:(0x5c8,'movie.start'),0x207060:(0x5ec,'pause'),0x207070:(0x20,'acquire'),0x207080:(0x1c,'frame'),0x207090:(0x420,'health')}
    for address,(slot,name) in apis.items():write(table+slot,address);u.mem_write(address,b'\xc3')
    u.mem_write(number,struct.pack('<f',health));u.mem_write(0x207090,b'\xd9\x05'+struct.pack('<I',number)+b'\xc2\x04\x00')
    events=[];texts={};state={'queries':0,'acquires':0,'busy':0,'movie':False,'thing':False,'complete':False}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);result=0;pop=None
        if pc in (w['end'],w['cancel']):state['complete']=pc==w['end'];uc.emu_stop();return
        if pc==0x99ebf0:
            assert read(sp+8)==0xffffffff;address=read(sp+4);texts[this]='' if address==0x122d70e else data.string_at(address);events.append(('text.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',texts.pop(this)));pop=0
        elif pc==0xf35b30:assert this==owner;state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0xdaea70:assert this==parent and read(sp+4)==2;events.append(('badDeed',2));pop=4
        elif pc==0x6e7b60:assert this==stack+100 and not state['movie'];state['movie']=True;events.append(('movie.new',));pop=0
        elif pc==0x6e7b80:assert this==stack+100 and state['movie'];state['movie']=False;events.append(('movie.destroy',));pop=0
        elif pc==0xcd23b9:assert this==stack+16 and state['movie'];events.append(('prepare',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==stack+16;events.append(('prepare.release',));pop=0
        elif pc==0x7e7490:assert this==stack+16 and read(sp+4)==stack+128 and not state['thing'];state['thing']=True;events.append(('thing.new',not populated));result=stack+128;pop=4
        elif pc==0x4aa840:assert this==stack+128 and state['thing'];state['thing']=False;events.append(('thing.destroy',));pop=0
        elif pc==0x7e7390:
            assert this==stack+16 and [read(sp+i) for i in (4,12,16,20,24)]==[hero,0,0,1,0];events.append(('speak',data.string_at(read(sp+8))));state['busy']=busy;pop=24
        elif pc==0x7e7450:assert this==stack+16;result=state['busy']>0;state['busy']-=1;events.append(('busy',result));pop=0
        elif pc in apis:
            name=apis[pc][1]
            if name in ('ordinary','ability','excluded'):
                assert this==owner+8;index=('ordinary','ability','excluded').index(name);key=read(sp+(8 if index==2 else 4));assert texts[key]=='SCRIPT_NAME_HERO'
                if index==2:assert read(sp+4)==14
                result=answers[index];events.append(('predicate',index,result));pop=8 if index==2 else 4
            elif name=='hero':assert this==game;events.append(('hero',));result=hero;pop=0
            elif name=='ally':
                args=[read(sp+i) for i in (4,8)];assert args in ([owner+8,hero],[hero,owner+8]);events.append(('ally',*['father' if value==owner+8 else 'hero' for value in args]));pop=8
            elif name=='movie.start':assert this==game and texts[read(sp+4)]=='' and read(sp+8)==stack+100;events.append(('movie.start',));pop=8
            elif name=='pause':assert this==game;events.append(('pause',bool(read(sp+4))));pop=4
            elif name=='acquire':assert this==game and [read(sp+i) for i in (4,8,12)]==[owner+8,stack+16,4];result=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',result,populated));pop=12
            elif name=='frame':assert this==game;events.append(('frame',));pop=0
            elif name=='health':assert this==game and read(sp+4)==stack+128;events.append(('health',))
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(w['start'],0x20f000,count=5000)
    assert not texts and not state['movie'] and not state['thing'] and read(stack+56)==mask
    return state['complete'],events

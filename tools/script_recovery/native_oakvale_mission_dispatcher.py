"""Original mission control flow, with checked ownership/scheduler boundaries."""
import struct
from tools.script_recovery.native_oakvale_mission_scopes import *


def execute(region_delay=0,attack_initial=False,attack_after=2,cancel=99,post_cancel=False):
    d=RData();prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbd000,0xdbe000,0x99e000,0xcb7000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(ADDRESS,d.bytes_at(ADDRESS,SIZE))
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;finish=0x204000
    put(stack,finish);put(owner+0x40,game);put(game,table)
    frames=0;queries=0;regions=0;texts={};events=[]
    specs=((0x30,'region',4),(0x1c,'frame',0),(0x5d0,'fade',8),(0xae8,'cache',4),
           (0x450,'activate',4),(0xa20,'stop',8),(0xa18,'time',4),(0x8e8,'sleeping',4),(0x554,'money',4))
    apis={0x205000+i*16:(name,pop) for i,(_,name,pop) in enumerate(specs)}
    for i,(slot,_,_) in enumerate(specs):put(table+slot,0x205000+i*16)
    for pc in (*apis,0xcb7940,0x99ebf0,0x99eae0,0xdbe3c0,0xdbeb20):u.mem_write(pc,b'\xc3')
    boundaries={start:(end,(kind,)) for kind,(start,end) in SCOPES.items()}
    boundaries.update({0xdbdf4b:(0xdbdfcc,('thread','WatchBarrels')),0xdbdfcc:(0xdbe044,('thread','WatchForGotGold')),
                       0xdbe044:(0xdbe0b9,('thread','ManageQuestCoreMarkers'))})
    def hook(uc,pc,size,user):
        nonlocal frames,queries,regions
        if pc in boundaries:
            end,event=boundaries[pc];events.append(event);uc.reg_write(UC_X86_REG_EIP,end);return
        if pc in (0xdbded9,0xdbe1f3,0xdbe217):
            value=attack_initial or frames>=attack_after;u.mem_write(owner+0x50,bytes([value]));events.append(('attack.over',value))
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;value=0
        def arg(i):return get(sp+4+i*4)
        if pc==0x99ebf0:assert arg(1)==0xffffffff;texts[this]=d.string_at(arg(0));value=this;pop=8
        elif pc==0x99eae0:assert this in texts;texts.pop(this);pop=0
        elif pc==0xcb7940:
            assert this==owner;queries+=1;value=queries>=cancel;events.append(('term',value));pop=0
        elif pc in (0xdbe3c0,0xdbeb20):
            assert this==owner;events.append(('attack',) if pc==0xdbe3c0 else ('post',post_cancel));pop=0
        elif pc in apis:
            name,pop=apis[pc];assert this==game
            if name=='region':assert texts[arg(0)]=='StartOakVale';value=regions>=region_delay;regions+=1;events.append(('region',value))
            elif name=='frame':frames+=1;events.append(('frame',))
            elif name=='fade':assert arg(1)==0;events.append(('fade',struct.unpack('<f',struct.pack('<I',arg(0)))[0]))
            elif name=='activate':events.append(('activate',texts[arg(0)]))
            elif name=='stop':assert arg(1)==owner+0x4c;put(arg(1),17);events.append(('stop',bool(arg(0))))
            elif name=='time':events.append(('time',struct.unpack('<f',struct.pack('<I',arg(0)))[0]))
            else:events.append((name,arg(0) if name=='cache' else bool(arg(0))))
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(value));uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start(ADDRESS,finish,count=10000)
    assert not texts and u.reg_read(UC_X86_REG_ESP)==stack+4
    return events

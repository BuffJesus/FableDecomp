"""Execute original gold watcher and attack transition, including CString scopes."""
import hashlib
import struct
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

REGIONS=((0xdbe2e0,212,'47f139092e2b64cc78d20c89b6bd6a9e4590759b7b76d14fd51a7d3acce5982b'),
         (0xdbe3c0,285,'e842cceccf47bfb0bc2c829a199b0889340d6fd750ed429578fd489b732e80dc'))


def prove(data=None):
    d=data or RData()
    for address,size,digest in REGIONS:
        if hashlib.sha256(d.bytes_at(address,size)).hexdigest()!=digest:raise ValueError('Oakvale progress native bytes changed')
    for address,literal in ((0x12d9d2c,'TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03'),(0x12d9d54,'TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_06')):
        if d.string_at(address)!=literal:raise ValueError('Oakvale progress native literal changed')
    if d.bytes_at(0x122d70e,1)!=b'\0':raise ValueError('Oakvale empty objective region changed')
    return dict(regions=REGIONS,objectiveRegions=['',''],limits='Complete original callers; engine/CString operations doubled.')


def execute(kind,initial=0,delay=1,cancel=99,alias=0,mutate=False):
    d=RData();prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbe000,0xcb7000,0x99e000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for a,n,_ in REGIONS:u.mem_write(a,d.bytes_at(a,n))
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;other=0x204000;finish=0x205000;alternate=0x206000
    put(stack,finish);put(owner+0x40,game);put(game,table);texts={};events=[];queries=0;gold_calls=0;active=None
    spec=((0x1fc,'gold',0),(0x1c,'frame',0),(0xa3c,'active',4),(0x4a0,'objective',16),
          (0x450,'activate',4),(0x460,'deactivate',8),(0xa18,'time',4),(0xa40,'theme',8))
    apis={0x207000+i*16:(name,pop) for i,(_,name,pop) in enumerate(spec)}
    for i,(slot,_,_) in enumerate(spec):put(table+slot,0x207000+i*16);put(other+slot,0x207000+i*16)
    apis[0x207100]=('objective.changed',16)
    for pc in (*apis,0xcb7940,0x99ebf0,0x99eae0):u.mem_write(pc,b'\xc3')
    def hook(uc,pc,size,user):
        nonlocal queries,gold_calls,active
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;value=0
        def arg(i):return get(sp+4+4*i)
        if pc==0x99ebf0:
            assert arg(1)==0xffffffff;texts[this]='' if d.bytes_at(arg(0),1)==b'\0' else d.string_at(arg(0));events.append(('key.new',texts[this]));value=this;pop=8
        elif pc==0x99eae0:events.append(('key.close',texts.pop(this)));pop=0
        elif pc==0xcb7940:assert this==owner;queries+=1;value=queries>=cancel;events.append(('term',value));pop=0
        elif pc in apis:
            name,pop=apis[pc];assert this==game
            if name=='gold':
                value=initial if gold_calls<delay else 3;gold_calls+=1
                signed=value&0xffffffff;signed=signed if signed<2**31 else signed-2**32;events.append(('gold',signed))
            elif name=='frame':events.append(('frame',))
            elif name=='active':
                active=arg(0);texts[active]='active';value=0 if alias==2 else alternate if alias else active;events.append(('active',))
                if mutate:put(game,other);put(table+0x4a0,0x207100)
            elif name.startswith('objective'):
                assert arg(0)==(0 if alias==2 else alternate if alias else active)
                values=tuple(texts[arg(i)] for i in (1,2,3));assert values[1:]==('','')
                events.append(('objective',*values,alias,name.endswith('.changed')))
            elif name in ('activate','deactivate','theme'):
                if name!='activate':assert arg(1)==0
                events.append((name,texts[arg(0)]))
            else:assert arg(0)==0x41b80000;events.append(('time',23.0))
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(value)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start(0xdbe2e0 if kind=='gold' else 0xdbe3c0,finish,count=5000)
    assert not texts and u.reg_read(UC_X86_REG_ESP)==stack+4
    return events

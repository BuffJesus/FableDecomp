"""Execute complete retail deed helpers with engine APIs as ABI boundaries."""
import hashlib
import json
import struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP


def prove(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('native_deed_witnesses.json').read_text())
    for row in w['functions']:
        if hashlib.sha256(data.bytes_at(int(row['address'],16),row['size'])).hexdigest()!=row['bytesSha256']:
            raise ValueError('Oakvale deed native bytes changed')
    return w


def execute(kind,good=0,bad=0,done=False,gold=0,sweets=False,cancel=99,delay=0,deed=2,amount=0.125):
    d=RData();w=prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdae000,0xdb0000,0xcb7000,0xcbe000,0x99e000,0x143e000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for row in w['functions']:u.mem_write(int(row['address'],16),d.bytes_at(int(row['address'],16),row['size']))
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def signed(value):return value if value<2**31 else value-2**32
    owner=0x201000;game=0x202000;table=0x203000;defs=0x204000;finish=0x205000;stack=0x21f000
    put(stack,finish);put(stack+4,deed);put(owner+0x40,game);put(game,table);put(owner+0x54,good);put(owner+0x58,bad);put(owner+0x5c,-9)
    u.mem_write(owner+0x94,bytes([sweets]));u.mem_write(owner+0xfc+deed,bytes([done]));put(0x143e90c,defs);u.mem_write(defs+0xd64,struct.pack('<f',amount))
    spec=((0x270,'morality',4),(0x1cc,'info',4),(0xa0,'clicked',0),(0x1c,'frame',0),
          (0x1fc,'gold',0),(0xa3c,'active',4),(0x4a0,'objective',16),(0x53c,'counter',12))
    apis={0x206000+i*16:(name,pop) for i,(_,name,pop) in enumerate(spec)}
    for i,(slot,_,_) in enumerate(spec):put(table+slot,0x206000+i*16)
    for pc in (*apis,0x99ebf0,0x99eae0,0xcb7940,0xcbe9ee):u.mem_write(pc,b'\xc3')
    events=[];texts={};queries=0;clicks=0
    def hook(uc,pc,size,user):
        nonlocal queries,clicks
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;value=0
        if pc in (0xdaeb4b,0xdaebe2):events.append(('bad.record',deed))
        if pc==0x99ebf0:
            assert get(sp+8)==0xffffffff;address=get(sp+4);key='' if d.bytes_at(address,1)==b'\0' else d.string_at(address)
            texts[this]=key;events.append(('key.new',key));pop=8
        elif pc==0x99eae0:events.append(('key.close',texts.pop(this)));pop=0
        elif pc==0xcb7940:assert this==owner;queries+=1;value=queries>=cancel;events.append(('term',value));pop=0
        elif pc==0xcbe9ee:events.append(('tutorial',texts[this]));pop=0
        elif pc in apis:
            name,pop=apis[pc];assert this==game
            if name=='morality':events.append(('counts',signed(get(owner+0x54)),signed(get(owner+0x58))));events.append(('morality',bytes(uc.mem_read(sp+4,4)).hex()))
            elif name=='info':events.append(('info',texts[get(sp+4)]))
            elif name=='clicked':value=clicks>=delay;clicks+=1;events.append(('clicked',value))
            elif name=='frame':events.append(('frame',))
            elif name=='gold':value=gold&0xffffffff;events.append(('gold',gold))
            elif name=='active':value=get(sp+4);texts[value]='active';events.append(('active',))
            elif name=='objective':
                args=[texts[get(sp+4+i*4)] for i in range(4)]
                assert args==['active','TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02','',''];events.append(('objective',))
            else:assert signed(get(sp+4))==-9 and signed(get(sp+8))==signed(get(owner+0x54)) and get(sp+12)==0xffffffff;events.append(('counter',signed(get(sp+8))))
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(value)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start(0xdb0660 if kind=='good' else 0xdaea70,finish,count=5000)
    assert not texts and u.reg_read(UC_X86_REG_ESP)==stack+(4 if kind=='good' else 8)
    return events

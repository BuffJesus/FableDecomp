"""Execute original DoMission ownership scopes with engine boundaries doubled."""
import hashlib
import struct
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_EIP, UC_X86_REG_ESI

ADDRESS=0xdbde40
SIZE=1172
SHA='e3785b3e55a0b1f5556c67316632faeec99b891b1bce0743bd1b762153a91542'
SCOPES={'child':(0xdbdf06,0xdbdf4b),'house':(0xdbe15e,0xdbe1f3),
        'killable.off':(0xdbe106,0xdbe123),'killable.on':(0xdbe24d,0xdbe26a),
        'complete':(0xdbe277,0xdbe2a4),'deactivate':(0xdbe2a4,0xdbe2cd)}


def prove(data=None):
    d=data or RData()
    if hashlib.sha256(d.bytes_at(ADDRESS,SIZE)).hexdigest()!=SHA:raise ValueError('DoMission native bytes changed')
    for address,text in ((0x12d9d08,'CREATURE_HERO_CHILD'),(0x12d9ca4,'HerosOldHouse')):
        if d.string_at(address)!=text:raise ValueError('DoMission literal changed')
    return dict(address=ADDRESS,size=SIZE,sha256=SHA,scopes=SCOPES,
        limits='Original caller instructions; CString/Thing destruction and engine operations are checked doubles.')


def execute(kind,alias=0,empty=False,mutate=False):
    d=RData();prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbd000,0xdbe000,0x99e000,0x4aa000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(ADDRESS,d.bytes_at(ADDRESS,SIZE))
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;other=0x204000;hero=0x205000;alternate=0x206000
    put(owner+0x40,game);put(game,table);texts={};events=[];owned=None;active=None
    spec=((0x118,'hero',0),(0x178,'turn',12),(0x120,'lookup',8),(0x6bc,'unlock',8),
          (0x6ac,'doors',4),(0xa3c,'active',4),(0x4d4,'screen',12),(0xae0,'music',12),
          (0x814,'killable',12),(0x480,'complete',16),(0x464,'deactivate',8))
    apis={}
    for index,(slot,name,pop) in enumerate(spec):
        for changed,t in ((False,table),(True,other)):
            pc=0x207000+index*32+16*changed;apis[pc]=(name,pop,changed);put(t+slot,pc);u.mem_write(pc,b'\xc3')
    for pc in (0x99ebf0,0x99eae0,0x4aa840):u.mem_write(pc,b'\xc3')
    def returned(value):return None if alias==2 else alternate if alias else value
    def hook(uc,pc,size,user):
        nonlocal owned,active
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;value=0
        def arg(i):return get(sp+4+4*i)
        if pc==0x99ebf0:
            assert arg(1)==0xffffffff;texts[this]=d.string_at(arg(0));events.append(('key.new',texts[this]));value=this;pop=8
        elif pc==0x99eae0:events.append(('key.close',texts.pop(this)));pop=0
        elif pc==0x4aa840:assert this==owned;events.append(('output.close',));owned=None;pop=0
        elif pc in apis:
            name,pop,changed=apis[pc];assert this==game
            if name=='hero':
                events.append(('hero',empty));value=0 if empty else hero
                if mutate:
                    put(game,other)
                    for slot in (0x178,0x814):put(table+slot,get(other+slot))
            elif name in ('turn','lookup'):
                owned=arg(0);assert owned==stack+(0x24 if name=='turn' else 0x18)
                key=arg(2 if name=='turn' else 1);assert key in texts
                if name=='turn':assert arg(1)==(0 if empty else hero)
                events.append((name,changed));value=returned(owned) or 0
                if mutate and name=='lookup':put(game,other)
            elif name in ('unlock','doors'):
                assert arg(0)==owned and not texts
                if name=='unlock':assert arg(1)==1
                events.append((name,changed))
            elif name=='active':
                active=arg(0);texts[active]='active';events.append(('active',changed));value=returned(active) or 0
                if mutate:
                    put(game,other)
                    for slot in (0x4d4,0x480,0x464):put(table+slot,get(other+slot))
            elif name in ('screen','complete','deactivate'):
                assert arg(0)==(returned(active) or 0) and active in texts
                flags=[arg(i) for i in range(1,pop//4)]
                assert flags==({'screen':[0,1],'complete':[0,0,0],'deactivate':[0]}[name])
                events.append((name,alias,changed))
            elif name=='music':
                assert not texts and owned and [arg(i) for i in range(3)]==[19,0,1];events.append(('music',changed))
            else:
                assert arg(0)==(0 if empty else hero) and arg(1)==int(kind=='killable.on') and arg(2)==0
                events.append(('killable',kind=='killable.on',changed))
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,value);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start(*SCOPES[kind],count=2000)
    assert not texts and owned is None and u.reg_read(UC_X86_REG_ESP)==stack
    return events

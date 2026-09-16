"""Execute original atomic lookup scopes, including real counted destruction."""
import hashlib
import struct
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_EDX, UC_X86_REG_EIP, UC_X86_REG_ESI, UC_X86_REG_EDI

REGIONS=((0xdbeb20,1095,'8dc72f018eb5e9a030d02a244b9b86416b83ae59c0864a3db40e0b5e9a3a13e7'),
         (0x4aa840,66,'5aa58f8e6b624fb6abbc2496c0f63791f6a7ae480b3e86d9e93f148d23fe4afa'),
         (0x99a2e0,7,'b391b1043a81dbc361b75b6ef3b6f5551ecf4643eb44e742e4f605ce2382115a'))


def prove(data=None):
    d=data or RData()
    for address,size,expected in REGIONS:
        if hashlib.sha256(d.bytes_at(address,size)).hexdigest()!=expected:
            raise ValueError('Post-attack atomic native bytes changed')
    for address,key in ((0x12d9e04,'M_PostAttackStart'),(0x12d9df8,'V_OakVale'),(0x12d9de4,'MK_OVI_DADTRIGGER')):
        if d.string_at(address)!=key:raise ValueError('Post-attack atomic literal changed')
    for slot,target in ((0x120,0x8a7d60),(0x118,0x891ca0),(0x760,0x88e540),(0x6e0,0x896060)):
        if int.from_bytes(d.bytes_at(0x1260f0c+slot,4),'little')!=target:
            raise ValueError('Post-attack atomic vtable changed')
    return dict(regions=REGIONS,limits='Original caller and counted destruction; engine APIs, alive/distance and CString internals are checked doubles.')


def execute(kind,alias=False,empty=False,count=1,result=True,mutate=False):
    d=RData();prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbe000,0xdbf000,0x99e000,0x99a000,0x4aa000,0xbfe000,0xcbe000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for a,n,_ in REGIONS:u.mem_write(a,d.bytes_at(a,n))
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;other=0x204000
    info=0x205000;hero=0x206000;returned_alias=0x206100;alive_table=0x206200
    owned=stack+(28 if kind=='near' else 16);texts={};events=[]
    def event(*args):events.append(args)
    apis={0x207000:('lookup',8),0x207010:('hero',0),0x207020:('teleport',12),
          0x207030:('limbo',8),0x207040:('alive',0),0x207050:('object.destroy',0),
          0x207060:('teleport.changed',12),0x207070:('limbo.changed',8)}
    for pc in (*apis,0x99ebf0,0x99eae0,0xbfe9bc,0xcbe2ff):u.mem_write(pc,b'\xc3')
    for t in (table,other):
        for slot,pc in ((0x120,0x207000),(0x118,0x207010),(0x760,0x207020),(0x6e0,0x207030)):put(t+slot,pc)
    put(owner+0x40,game);put(game,table);put(alive_table+0x12c,0x207040);put(returned_alias,alive_table)
    put(info,count);put(info+4,0x207050);put(info+8,hero)
    if kind=='near':put(owned,alive_table)
    scopes={'alive':(0xdbeb30,0xdbebb3),'teleport':(0xdbebf1,0xdbec43),
            'limbo.on':(0xdbec43,0xdbec86),'limbo.off':(0xdbeebc,0xdbeeff),
            'near':(0xdbed3b,0xdbed56)}
    start,end=scopes[kind]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;value=0
        if pc==0x99ebf0:
            assert get(sp+8)==0xffffffff;texts[this]=d.string_at(get(sp+4));event('key.new',texts[this]);pop=8
        elif pc==0x99eae0:event('key.destroy',texts.pop(this));pop=0
        elif pc==0x99a2e0:
            assert this==owned and get(owned+4)==get(owned+8)==0;event('output.destroy');return
        elif pc==0xbfe9bc:assert get(sp+4)==info and get(info)==0;event('info.free');pop=0
        elif pc==0xcbe2ff:
            assert this==(0 if empty else hero) and uc.reg_read(UC_X86_REG_EDX)==owned and get(sp+4)==0x40a00000
            event('near',empty,result);value=result;pop=4
        elif pc in apis:
            name,pop=apis[pc]
            if name=='lookup':
                assert this==game and get(sp+4)==owned and get(sp+8) in texts
                put(owned,alive_table);put(owned+4,0 if empty else hero);put(owned+8,info if count else 0)
                value=returned_alias if alias else owned;event('lookup',alias,empty)
                if mutate:put(game,other);put(table+0x6e0,0x207070)
            elif name=='alive':
                assert this==(returned_alias if alias else owned);event('alive',alias,result);value=result
            elif name=='hero':
                assert this==game;event('hero',empty);value=0 if empty else hero
                if mutate:put(table+0x760,0x207060)
            elif name.startswith('teleport'):
                assert this==game and get(sp+4)==(0 if empty else hero) and get(sp+8)==(returned_alias if alias else owned) and get(sp+12)==0
                event('teleport',alias,empty,name.endswith('.changed'))
            elif name.startswith('limbo'):
                assert this==game and get(sp+4)==(returned_alias if alias else owned) and get(sp+8)==int(kind=='limbo.on')
                event('limbo',alias,kind=='limbo.on',name.endswith('.changed'))
            else:assert this==hero and get(info)==0;event('object.destroy')
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(value));uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EDI,0x1238c8c)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(start,end,count=1000)
    assert u.reg_read(UC_X86_REG_EIP)==end and u.reg_read(UC_X86_REG_ESP)==stack and not texts
    return events

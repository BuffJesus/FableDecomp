"""Execute retail Init/GivenTeddy and the actual pushability argument destructor."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn import UC_HOOK_MEM_WRITE
from unicorn.x86_const import *
from tools.script_recovery.bully_lifecycle import prove
from tools.script_recovery.lift_native_lua import RData

def execute(init=True,data_kind='empty',has_info=True,has_hero=True):
    data=RData();prove(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdae000,0xdbc000,0x8a6000,0x99a000,0x99e000,0x40f000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for address,size in ((0xdaed30,172),(0xdbcd00,88),(0x8a6dd0,218)):u.mem_write(address,data.bytes_at(address,size))
    stack=0x21e000;owner=0x201000;parent=0x202000;game=0x203000;table=0x204000;done=0x205000;info=0x206000;hero=0x207000
    actor=owner+8;proxy=0x209000;proxy_table=0x209100;getter=0x209200;thing=0x20a000;end=0x20b000;node=0x20b100;component=0x20c000
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(stack,done);write(owner+4,game);write(owner+20,parent);write(game,table)
    write(actor,0x1238c8c);write(actor+4,0 if data_kind=='empty' else proxy);write(actor+8,info if has_info else 0);write(info,7)
    write(proxy,proxy_table);write(proxy_table+0x2c,getter);write(thing+0x34,0x400);write(thing+0x48,end);write(node,0xaa);write(node+4,component)
    u.mem_write(component+0x79,b'\x01')
    apis={0x208000:'damage',0x208010:'kill',0x208020:'combo',0x208030:'information',0x208040:'hero',0x208050:'ally',0x208060:'gold',0x208070:'take'}
    for offset,pc in zip((0x810,0x814,0x838,0x5a0,0x118,0x95c,0x1f8,0x1f4),apis):write(table+offset,pc)
    write(table+0xd30,0x8a6dd0)
    for pc in (*apis,0x99a2e0,0x99ebf0,0x99eae0,getter,0x40f020,0xdaea70):u.mem_write(pc,b'\xc3')
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner)
    trace=[];strings={};state={'pushCalls':0,'copyBaseDestructors':0}
    fields={owner+0x24:('entity','DoneIntro',bool),owner+0x20:('entity','HitsTaken',int),owner+0x1c:('entity','InitialHealth',int),
            owner+0x26:('entity','SpokenOnFirstProximity',bool),parent+0x70:('quest','SpokeAboutFindingTeddy',bool),
            owner+0x25:('entity','SaidPieceAboutAttackingVictim',bool),owner+0x28:('entity','IntimidateSpeechLoop',int),parent+0x91:('quest','TeddyRuined',bool)}
    def store_hook(uc,access,address,size,value,user):
        if address in fields:
            scope,name,kind=fields[address];trace.append(('state',scope,name,kind(value)))
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x8a6dd0:
            assert (read(sp+4),read(sp+8),read(sp+12),read(sp+16))==(0x1238c8c,read(actor+4),read(actor+8),0)
            if has_info:assert read(info)==8
            state['pushCalls']+=1;trace.append(('pushable',False));return
        if pc==0x99a2e0:state['copyBaseDestructors']+=1;pop=0
        elif pc==getter:assert this==proxy;result=thing if data_kind=='valid' else 0;pop=0
        elif pc==0x40f020:assert this==thing+0x44 and read(read(sp+4))==0xaa;result=node;pop=4
        elif pc==0x99ebf0:strings[this]=data.string_at(read(sp+4));trace.append(('string.new',strings[this]));pop=8
        elif pc==0x99eae0:trace.append(('string.destroy',strings.pop(this)));pop=0
        elif pc==0xdaea70:assert this==parent and read(sp+4)==3;trace.append(('bad.deed',3));pop=4
        elif pc in apis:
            assert this==game;name=apis[pc]
            if name=='hero':trace.append(('hero',));result=hero if has_hero else 0;pop=0
            elif name=='ally':assert (read(sp+4),read(sp+8))==(actor,hero if has_hero else 0);trace.append(('ally',));pop=8
            elif name=='gold':assert read(sp+4)==1;trace.append(('gold',1));pop=4
            elif name=='take':trace.append(('take',strings[read(sp+4)]));pop=4
            else:
                count={'damage':1,'kill':2,'combo':1,'information':3}[name]
                assert read(sp+4)==actor and all(read(sp+8+n*4)==0 for n in range(count))
                trace.append((name,*([False]*count)));pop=4*(count+1)
        if pop is not None:
            ret=read(sp);uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_MEM_WRITE,store_hook);u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start(0xdaed30 if init else 0xdbcd00,done,count=10000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not strings and read(info)==7
    assert state['pushCalls']==int(init) and state['copyBaseDestructors']==int(init)
    assert bytes(u.mem_read(component+0x79,1))==(b'\x00' if init and data_kind=='valid' else b'\x01')
    return trace

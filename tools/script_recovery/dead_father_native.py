"""Whole original Main, including real empty/nonempty looping-animation forwarder."""
import struct
from functools import lru_cache
from tools.script_recovery.dead_father_candidate import prove
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP
@lru_cache(maxsize=1)
def fixture():
    data=RData();return data,prove(data)
def execute(cancel=8,failures=0,found_after=0,prepare=False,populated=True,markers=(False,True),angle=1.5,flag5=1):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb8000,0xf35000,0x99a000,0x99e000,0xcd2000,0x7e7000,0x4aa000,0x4ab000,0x1375000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for row in w['functions']:
        if row['name'] in ('Main','loopForwarder'):u.mem_write(row['address'],data.bytes_at(row['address'],row['size']))
    for address in (0xf35b10,0xf35b30,0x4abe90,0x4aa840,0x99a380,0xcd23b9,0xcd2770,0x7e74d0,0x99ebf0,0x99eae0):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;done=0x204000;parent=0x205000;expert=0x206000;etab=0x206100;thingtab=0x206200;number=0x206300;base=stack-44
    def write(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(stack,done);write(owner+4,game);write(owner+0x14,parent);write(game,table);write(expert,etab);write(etab+0x50,0x208000);write(thingtab+0x28,0x208010)
    u.mem_write(0x208000,b'\xc3');u.mem_write(number,struct.pack('<f',angle));u.mem_write(0x208010,b'\xd9\x05'+struct.pack('<I',number)+b'\xc3');u.mem_write(0x1375748,bytes([flag5]))
    apis={0x207000:(0x1c,'frame',0),0x207010:(0x20,'acquire',3),0x207020:(0x120,'lookup',2),0x207030:(0x760,'teleport',3),0x207040:(0x768,'face',3),0x207050:(0x580,'remove',1)}
    for pc,(slot,*_) in apis.items():write(table+slot,pc);u.mem_write(pc,b'\xc3')
    events=[];texts={};state={'queries':0,'acquires':0,'polls':0,'lookups':0,'condition':False,'control':False,'marker':False}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (0xdb84b6,0xdb84d6):v=state['polls']>=found_after;state['polls']+=1;u.mem_write(parent+0x51,bytes([int(v)]));events.append(('found',v))
        if pc==0xdb848a:events.append(('rawFlag',flag5))
        if pc==0x4abe90:assert this==base+16 and read(sp+4)==actor;state['condition']=True;pop=4
        elif pc==0xf35b10:assert this==owner and read(sp+4)==base+12 and read(base+12)==0x12c32f0;events.append(('condition.alive',));pop=4
        elif pc==0x4aa840:
            if this==base+16 and state['condition']:state['condition']=False
            else:assert this==base+12 and state['marker'];state['marker']=False;events.append(('marker.destroy',state['lookups']))
            pop=0
        elif pc==0xf35b30:assert this==owner and not state['condition'];state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x99a380:assert this==base+28;state['control']=True;events.append(('control.new',));pop=0
        elif pc==0xcd23b9:assert this==base+28;events.append(('prepare',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==base+28;events.append(('prepare.release',));pop=0
        elif pc==0x7e74d0:assert this==base+28 and state['control'];state['control']=False;events.append(('control.destroy',));pop=0
        elif pc==0x99ebf0:assert read(sp+8)==0xffffffff;texts[this]=data.string_at(read(sp+4));events.append(('text.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',texts.pop(this)));pop=0
        elif pc==0x208010:assert this==base+12 and state['marker'];events.append(('angle',struct.pack('<f',angle).hex()));return
        elif pc==0x208000:
            assert this==expert and texts[read(sp+4)]=='CS_DEAD_DAD' and [read(sp+i) for i in (8,12,16,20,24,28,32,36)]==[0xffffffff,0,1,0,1,flag5,0,0];events.append(('loop',-1,False,True,False,True,flag5,False,False));pop=36
        elif pc in apis:
            slot,name,count=apis[pc];assert this==game
            if name=='frame':assert not state['condition'];events.append(('frame',))
            elif name=='acquire':
                assert [read(sp+i) for i in (4,8,12)]==[actor,base+28,4];result=state['acquires']>=failures;state['acquires']+=1;write(base+36,expert if populated else 0);events.append(('acquire',result,populated))
            elif name=='lookup':
                assert read(sp+4)==base+12 and texts[read(sp+8)]=='MK_OVID_DAD';state['lookups']+=1;state['marker']=True;write(base+12,thingtab);result=base+12;events.append(('lookup',state['lookups'],markers[state['lookups']-1]))
            elif name=='teleport':assert [read(sp+i) for i in (4,8,12)]==[actor,base+12,0];events.append(('teleport',1,False))
            elif name=='face':assert read(sp+4)==actor and read(sp+12)==1;events.append(('face',u.mem_read(sp+8,4).hex(),True))
            else:assert read(sp+4)==actor;events.append(('remove','self'))
            pop=count*4
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdb8300,done,count=7000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not texts and not state['condition'] and not state['control'] and not state['marker'];return events

def execute_init(data_kind='empty',has_info=True):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb8000,0x8a6000,0x99a000,0x99e000,0x40f000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for row in w['functions']:
        if row['name'] in ('Init','pushable'):u.mem_write(row['address'],data.bytes_at(row['address'],row['size']))
    for pc in (0x99ebf0,0x99eae0):u.mem_write(pc,b'\xc3')
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;done=0x204000;info=0x205000;proxy=0x206000;ptab=0x206100;getter=0x206200;thing=0x207000;end=0x207100;node=0x207200;component=0x208000
    def write(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(stack,done);write(owner+4,game);write(game,table);write(actor,table);write(actor+4,0 if data_kind=='empty' else proxy);write(actor+8,info if has_info else 0);write(info,7)
    write(proxy,ptab);write(ptab+0x2c,getter);write(thing+0x34,0x400);write(thing+0x48,end);write(node,0xaa);write(node+4,component);u.mem_write(component+0x79,b'\x7f')
    write(table+0x578,0x20a000);write(table+0xd30,0x8a6dd0);u.mem_write(0x20a000,b'\xc3');events=[];texts={};copies=[]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x99ebf0:assert read(sp+8)==0xffffffff;texts[this]=data.string_at(read(sp+4));events.append(('text.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',texts.pop(this)));pop=0
        elif pc==0x20a000:assert this==game and read(sp+4)==actor and texts[read(sp+8)]=='HUD_ORB_QUEST_CORE';events.append(('marker.add','self'));pop=8
        elif pc==0x8a6dd0:
            assert this==game and [read(sp+i) for i in (4,8,12)]==[0x1238c8c,read(actor+4),read(actor+8)] and read(sp+16)==0 and not texts
            if has_info:assert read(info)==8
            copies.append(sp+4);events.append(('pushable',False));return
        elif pc==0x99a2e0:assert this==copies.pop() and read(this+4)==0 and read(this+8)==0;events.append(('copy.destroy',));pop=0
        elif pc==getter:assert this==proxy;result=thing if data_kind=='valid' else 0;pop=0
        elif pc==0x40f020:assert this==thing+0x44;result=node;pop=4
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdb8290,done,count=1000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not copies and not texts and read(info)==7
    assert u.mem_read(component+0x79,1)==(b'\0' if data_kind=='valid' else b'\x7f');return events

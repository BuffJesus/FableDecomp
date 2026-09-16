"""Whole native marker helper plus real normal/inline counted Thing destruction."""
import struct
from tools.script_recovery.manage_quest_core_markers import prove
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(cancel=99,delays=(0,0,0,0,0),tutorial=True,counts=(1,1,1),populated=(True,True,True),gold=2):
    d=RData();w=prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbe000,0x99e000,0x99a000,0xcb7000,0x4aa000,0xbfe000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for row in w['regions']:u.mem_write(row['address'],d.bytes_at(row['address'],row['size']))
    for pc in (0x99ebf0,0x99eae0,0xcb7940,0xbfe9bc,0x208000):u.mem_write(pc,b'\xc3')
    stack=0x21e000;base=stack-48;owner=0x201000;game=0x202000;table=0x203000;done=0x204000
    names=('father','trader','theresa');offsets=(12,36,24);keys=('NOVI_LiveFather','NOVI_BookTrader','NOVI_Theresa');infos=[0x205000+i*32 for i in range(3)]
    def write(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(stack,done);write(owner+0x40,game);write(game,table)
    for i,info in enumerate(infos):write(info,counts[i]);write(info+4,0x208000);write(info+8,0x206000+i*16)
    spec=((0x120,'lookup',2),(0x578,'add',2),(0x580,'remove',1),(0x1fc,'gold',0),(0x1c,'frame',0),(0x628,'controlled',0),(0x1d8,'tutorial',1),(0xa4,'clicked',0))
    apis={0x207000+i*16:v for i,v in enumerate(spec)}
    for pc,(slot,*_) in apis.items():write(table+slot,pc);u.mem_write(pc,b'\xc3')
    state={'queries':0,'lookup':0,'polls':[0]*5};events=[];texts={};live=set()
    def poll(index):v=state['polls'][index]>=delays[index];state['polls'][index]+=1;return v
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (0xdbe6cc,0xdbe6ef,0xdbe744,0xdbe763):
            index=3 if pc in (0xdbe6cc,0xdbe6ef) else 4;v=poll(index);u.mem_write(owner+0x94+index-3,bytes([int(v)]));events.append((('sweets','chocs')[index-3],v))
        if pc==0x99ebf0:assert this==base+8 and read(sp+8)==0xffffffff;texts[this]=d.string_at(read(sp+4));events.append(('key.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('key.destroy',texts.pop(this)));pop=0
        elif pc==0xcb7940:assert this==owner;state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x208000:
            i=(this-0x206000)//16;assert this==0x206000+i*16 and read(infos[i])==0;events.append(('object.destroy',names[i]));pop=0
        elif pc==0xbfe9bc:
            i=infos.index(read(sp+4));assert read(infos[i])==0;events.append(('info.free',names[i]));pop=0
        elif pc==0x99a2e0:
            i=offsets.index(this-base);assert i in live and read(this+4)==0 and read(this+8)==0;live.remove(i);events.append(('destroy',names[i]));return
        elif pc in apis:
            _,name,count=apis[pc];assert this==game;args=[read(sp+4+i*4) for i in range(count)];pop=count*4
            if name=='lookup':
                i=state['lookup'];state['lookup']+=1;assert args==[base+offsets[i],base+8] and texts[base+8]==keys[i]
                result=args[0];write(result,0x1238c8c);write(result+4,0x209000+i*16 if populated[i] else 0);write(result+8,infos[i] if counts[i] else 0);live.add(i);events.append(('lookup',names[i],populated[i]))
            elif name in ('add','remove'):
                i=offsets.index(args[0]-base);assert i in live
                if name=='add':assert texts[args[1]]=='HUD_ORB_QUEST_CORE'
                events.append((name,names[i]))
            elif name=='gold':result=3 if poll(0) else gold;events.append(('gold',result))
            elif name=='controlled':result=poll(1);events.append(('controlled',result))
            elif name=='clicked':result=poll(2);events.append(('clicked',result))
            elif name=='tutorial':assert args==[19];result=tutorial;events.append(('tutorial',tutorial))
            else:events.append(('frame',))
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbe4e0,done,count=6000)
    assert not texts and not live and u.reg_read(UC_X86_REG_EIP)==done
    assert all(read(info)==max(0,count-1) for info,count in zip(infos,counts));return events

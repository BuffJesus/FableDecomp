"""Run original Init and all four real callee argument-destruction paths."""
import struct
from functools import lru_cache
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP
from tools.script_recovery.guard_init import prove
from tools.script_recovery.lift_native_lua import RData

@lru_cache(maxsize=1)
def fixture():
    data=RData();return data,prove(data)

def execute(data_kind='empty',has_info=True,home=(7.5,-3.25,11.0)):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdac000,0x8a2000,0x8a3000,0x99a000,0x40f000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for row in [w,*w['consumers']]:u.mem_write(row['address'],data.bytes_at(row['address'],row['size']))
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;done=0x204000;info=0x205000;proxy=0x206000;proxy_table=0x206100;getter=0x206200;thing=0x207000;end=0x207100;node=0x207200;component=0x208000;homeapi=0x209000
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(stack,done);write(owner+4,game);write(game,table);write(actor,table)
    write(actor+4,0 if data_kind=='empty' else proxy);write(actor+8,info if has_info else 0);write(info,7)
    write(proxy,proxy_table);write(proxy_table+0x2c,getter);write(thing+0x34,0x400);write(thing+0x48,end);write(node,0xaa);write(node+4,component)
    apis={0x20a000:'damage',0x20a010:'kill',0x20a020:'combo',0x20a030:'sheathe'}
    for slot,pc in zip((0x810,0x814,0x838,0x7e8),apis):write(table+slot,pc)
    for row in w['consumers']:write(table+row['slot'],row['address'])
    write(table+0x1c,homeapi)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner)
    trace=[];copies=[];callee={r['address']:r['name'] for r in w['consumers']}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in callee:
            assert this==game and [read(sp+i) for i in (4,8,12)]==[0x1238c8c,read(actor+4),read(actor+8)]
            if has_info:assert read(info)==8
            copies.append(sp+4);name=callee[pc]
            value=tuple(struct.unpack('<fff',u.mem_read(sp+16,12))) if name=='centre' else (read(sp+16) if name=='stateGroup' else struct.unpack('<f',u.mem_read(sp+16,4))[0])
            trace.append((name,value));return
        if pc==0x99a2e0:
            assert this==copies.pop() and read(this+4)==0 and read(this+8)==0
            trace.append(('copy.destroy',));pop=0
        elif pc==getter:assert this==proxy;result=thing if data_kind=='valid' else 0;pop=0
        elif pc==0x40f020:assert this==thing+0x44;result=node;pop=4
        elif pc==homeapi:
            assert this==actor;u.mem_write(read(sp+4),struct.pack('<fff',*home));trace.append(('home',home));result=0;pop=4
        elif pc in apis:
            assert this==game and read(sp+4)==actor;name=apis[pc];count=2 if name=='kill' else 1
            assert all(read(sp+8+n*4)==0 for n in range(count));trace.append((name,*([False]*count)));pop=(count+1)*4
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(w['address'],done,count=5000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not copies and read(info)==7
    if data_kind=='valid':
        assert tuple(struct.unpack('<fff',u.mem_read(component+0x14,12)))==home
        assert struct.unpack('<ff',u.mem_read(component+0x20,8))==(0.0,6.0) and read(component+0x38)==4
    return trace

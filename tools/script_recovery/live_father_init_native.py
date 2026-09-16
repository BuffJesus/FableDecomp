"""Original Init plus real empty/invalid/valid copied-Thing callee paths."""
import struct
from tools.script_recovery.live_father_init import prove
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(data_kind='empty',has_info=True,paid=17):
    data=RData();w=prove(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdac000,0x8a6000,0x99a000,0x40f000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for row in w['functions']:u.mem_write(row['address'],data.bytes_at(row['address'],row['size']))
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;done=0x204000;info=0x205000;proxy=0x206000;proxy_table=0x206100;getter=0x206200;thing=0x207000;end=0x207100;node=0x207200;component=0x208000
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(stack,done);write(owner+4,game);write(game,table);write(actor,table);write(owner+0x1c,paid)
    write(actor+4,0 if data_kind=='empty' else proxy);write(actor+8,info if has_info else 0);write(info,7)
    write(proxy,proxy_table);write(proxy_table+0x2c,getter);write(thing+0x34,0x400);write(thing+0x48,end);write(node,0xaa);write(node+4,component);u.mem_write(component+0x79,b'\x7f')
    apis={0x20a000:('damage',(0,)),0x20a010:('kill',(0,0)),0x20a020:('combo',(0,)),0x20a030:('information',(0,1,0)),0x20a040:('deeds',(0,))}
    for slot,pc in zip((0x810,0x814,0x838,0x5a0,0x94c),apis):write(table+slot,pc)
    write(table+0xd30,0x8a6dd0);trace=[];copies=[]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0xdac399:trace.append(('set','PenniesGiven',0))
        if pc==0x8a6dd0:
            assert this==game and [read(sp+i) for i in (4,8,12)]==[0x1238c8c,read(actor+4),read(actor+8)] and read(sp+16)==0
            if has_info:assert read(info)==8
            copies.append(sp+4);trace.append(('pushable',False));return
        if pc==0x99a2e0:
            assert this==copies.pop() and read(this+4)==0 and read(this+8)==0;trace.append(('copy.destroy',));pop=0
        elif pc==getter:assert this==proxy;result=thing if data_kind=='valid' else 0;pop=0
        elif pc==0x40f020:assert this==thing+0x44;result=node;pop=4
        elif pc in apis:
            name,args=apis[pc];assert this==game and read(sp+4)==actor and tuple(read(sp+8+i*4) for i in range(len(args)))==args
            assert read(owner+0x1c)==0;trace.append((name,*map(bool,args)));pop=(len(args)+1)*4
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdac390,done,count=1000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not copies and read(info)==7 and read(owner+0x1c)==0
    assert u.mem_read(component+0x79,1)==(b'\0' if data_kind=='valid' else b'\x7f')
    return trace

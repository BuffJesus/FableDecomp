"""Original AffairMan Init and pushability callee destruction, with checked APIs."""
import struct
from tools.script_recovery.affair_man_complete import prove
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(data_kind='empty',has_info=True,initial=(True,True,True,True),badger=2147483647):
    data=RData();iw=prove(data);w={"calleeAddress":0x8a6dd0,"calleeSize":218};u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb0000,0x8a6000,0x99a000,0x40f000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(iw['functions'][0]['address'],data.bytes_at(iw['functions'][0]['address'],iw['functions'][0]['size']));u.mem_write(w['calleeAddress'],data.bytes_at(w['calleeAddress'],w['calleeSize']))
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;done=0x204000;info=0x205000;proxy=0x206000;proxy_table=0x206100;getter=0x206200;thing=0x207000;end=0x207100;node=0x207200;component=0x208000
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(stack,done);write(owner+4,game);write(game,table);write(actor,table);u.mem_write(owner+0x1c,bytes(map(int,initial)));write(owner+0x20,badger)
    write(actor+4,0 if data_kind=='empty' else proxy);write(actor+8,info if has_info else 0);write(info,7)
    write(proxy,proxy_table);write(proxy_table+0x2c,getter);write(thing+0x34,0x400);write(thing+0x48,end);write(node,0xaa);write(node+4,component);u.mem_write(component+0x79,b'\x7f')
    slots={0x810:('damage',(0,)),0x814:('kill',(0,0)),0x838:('combo',(0,)),0x5a0:('information',(0,0,0)),0x844:('movement',(0,))}
    apis={0x20a000+i*16:(slot,*spec) for i,(slot,spec) in enumerate(slots.items())}
    for pc,(slot,*_) in apis.items():write(table+slot,pc);u.mem_write(pc,b'\xc3')
    write(table+0xd30,w['calleeAddress']);trace=[];copies=[]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0xdb095b:trace.append(('set','BadgerIndex',0))
        for address,name in ((0xdb095e,'EncounterOver'),(0xdb0961,'SaidFirstRangedComment'),(0xdb0964,'HeroAgreedToKeepQuiet'),(0xdb0967,'HeroSaidHeWouldReportMan')):
            if pc==address:trace.append(('set',name,False))
        if pc==w['calleeAddress']:
            assert this==game and [read(sp+i) for i in (4,8,12)]==[0x1238c8c,read(actor+4),read(actor+8)] and read(sp+16)==0
            if has_info:assert read(info)==8
            copies.append(sp+4);trace.append(('pushable',False));return
        if pc==0x99a2e0:assert this==copies.pop() and read(this+4)==0 and read(this+8)==0;trace.append(('copy.destroy',));pop=0
        elif pc==getter:assert this==proxy;result=thing if data_kind=='valid' else 0;pop=0
        elif pc==0x40f020:assert this==thing+0x44;result=node;pop=4
        elif pc in apis:
            slot,name,args=apis[pc];assert this==game and read(sp+4)==actor and tuple(read(sp+8+i*4) for i in range(len(args)))==args
            assert bytes(u.mem_read(owner+0x1c,2))==b'\0\0';trace.append((name,*map(bool,args)));pop=(len(args)+1)*4
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(iw['functions'][0]['address'],done,count=1000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not copies and read(info)==7 and bytes(u.mem_read(owner+0x1c,4))==b'\0\0\0\0' and read(owner+0x20)==0
    assert u.mem_read(component+0x79,1)==(b'\0' if data_kind=='valid' else b'\x7f');return trace

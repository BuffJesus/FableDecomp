"""Original predicate instructions expose CString lifetime masks and ordering."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EBP,UC_X86_REG_EDI,UC_X86_REG_EAX
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def execute(kind,answers=(False,False,False),mask=0):
    data=RData();_,w=recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbb000,0xdbc000,0x99e000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;query=0x204000;hero=0x204040
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(actor,table);write(game,table);write(stack+56,mask)
    for offset,index in ((0x6c,0),(0x2e0,1),(0x54,0),(0xa8,1),(0xa4,2)):write(table+offset,query+index*16)
    write(table+0x118,hero)
    for pc in (0x99ebf0,0x99eae0,query,query+16,query+32,hero):u.mem_write(pc,b'\xc3')
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_EBP,owner);u.reg_write(UC_X86_REG_EDI,actor)
    events=[];live={}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x99ebf0:
            text=data.string_at(read(esp+4));live[this]=text;events.append(('string.new',text));pop=8
        elif pc==0x99eae0:events.append(('string.destroy',live.pop(this)));pop=0
        elif pc==hero:events.append(('hero',));result=0x206000;pop=0
        elif query<=pc<=query+32:
            index=(pc-query)//16
            if kind=='talk' and index==1:
                assert this==game and read(esp+8)==0x206000
                selfkey=read(esp+4);pop=8
            else:
                assert this==actor;selfkey=read(esp+8 if kind=='hit' and index==2 else esp+4)
                if kind=='hit' and index==2:assert read(esp+4)==14
                pop=8 if kind=='hit' and index==2 else 4
            assert selfkey in live;result=answers[index];events.append(('query',index,bool(result)))
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    start,end=(0xdbb613,0xdbb6b9) if kind=='talk' else (0xdbc29e,0xdbc385)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(start,end,count=300)
    assert not live and u.reg_read(UC_X86_REG_EIP)==end
    return bool(u.mem_read(stack+39,1)[0]),read(stack+56),events

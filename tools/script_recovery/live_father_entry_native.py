"""Original Main entry; separately compared intro/routine are explicit boundaries."""
import struct
from tools.script_recovery.live_father_hit_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(finished=False,cancel=999,failures=0,prepare=False,intro=True,populated=True):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb8000,0xdb9000,0x99a000,0xf35000,0x4ab000,0x4aa000,0xcd2000,0x7e7000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    for address in (0x4abe90,0xf35b10,0x4aa840,0xf35b30,0x99a380,0xcd23b9,0xcd2770,0x7e74d0):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;done=0x204000;parent=0x205000
    def write(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    write(stack,done);write(owner+4,game);write(owner+0x14,parent);write(game,table);u.mem_write(parent+0x52,bytes([int(finished)]))
    write(table+0x20,0x207000);write(table+0x1c,0x207010);u.mem_write(0x207000,b'\xc3');u.mem_write(0x207010,b'\xc3')
    base=stack-0xf4;events=[];state={'queries':0,'acquires':0,'control':False,'condition':False}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);result=0;pop=None
        if pc==0xdb878d:events.append(('finished',finished))
        if pc==0xdb8798:events.append(('intro',intro));uc.reg_write(UC_X86_REG_EIP,0xdb8aee if intro else 0xdb9781);return
        if pc==0xdb8aee:events.append(('routine',));uc.reg_write(UC_X86_REG_EIP,0xdb9781);return
        if pc==0x4abe90:assert this==base+104 and read(sp+4)==owner+8;state['condition']=True;pop=4
        elif pc==0xf35b10:assert this==owner and read(sp+4)==base+100 and read(base+100)==0x12c2fe8;events.append(('condition',));pop=4
        elif pc==0x4aa840:assert this==base+104 and state['condition'];state['condition']=False;pop=0
        elif pc==0xf35b30:assert this==owner and not state['condition'];state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x99a380:assert this==base+16;state['control']=True;events.append(('control.new',));pop=0
        elif pc==0xcd23b9:assert this==base+16;events.append(('prepare',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==base+16;events.append(('prepare.release',));pop=0
        elif pc==0x7e74d0:assert this==base+16 and state['control'];state['control']=False;events.append(('control.destroy',));pop=0
        elif pc==0x207000:
            assert this==game and [read(sp+i) for i in (4,8,12)]==[owner+8,base+16,4];result=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',result,populated));pop=12
        elif pc==0x207010:assert this==game and not state['condition'];events.append(('frame',));pop=0
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(w['mainAddress'],done,count=10000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not state['condition'] and not state['control'];return events

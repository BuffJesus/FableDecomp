"""Original late Hero/Victim acquisition instructions and cancellation unwinds."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EBP,UC_X86_REG_EAX,UC_X86_REG_EDI
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def execute(failures=0,cancel=999):
    data=RData();_,w=recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbb000,0xdbc000,0x7e7000,0xcd2000,0xf35000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;hero=0x204000;acquire=0x204010;frame=0x204020
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(game,table)
    for offset,pc in ((0x118,hero),(0x20,acquire),(0x1c,frame)):write(table+offset,pc)
    for pc in (hero,acquire,frame,0x7e72a0,0xcd23b9,0xf35b30,0x7e74d0):u.mem_write(pc,b'\xc3')
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_EBP,owner);u.reg_write(UC_X86_REG_EDI,owner+8)
    events=[];state={'self':0,'queries':0,'success':False}
    def role(address):
        assert address==stack+16
        return 'self'
    def hook(uc,pc,size,user):
        if pc==0xdbc8fa:
            state['success']=True;uc.emu_stop();return
        if pc==0xdbcce2:uc.emu_stop();return
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x7e72a0:
            name=role(this);write(this+8,0);events.append(('new',name));pop=0
        elif pc==0xcd23b9:events.append(('prepare',role(this)));pop=0
        elif pc==acquire:
            actor=read(esp+4);output=read(esp+8);name=role(output);assert read(esp+12)==4
            assert actor==owner+8
            # Failure retains populated output and retry does not reconstruct.
            assert read(output+8)==(0 if state[name]==0 else 123)
            write(output+8,123);state[name]+=1
            result=state[name]>failures
            events.append(('acquire',name,bool(result)));pop=12
        elif pc==frame:events.append(('frame',));pop=0
        elif pc==0xf35b30:
            state['queries']+=1;result=state['queries']>=cancel;events.append(('term',bool(result)));pop=0
        elif pc==0x7e74d0:events.append(('destroy',role(this)));pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbc891,0x205000,count=1500)
    assert state['success'] or u.reg_read(UC_X86_REG_EIP)==0xdbcce2
    return state['success'],events

def termination_edge(tail=False,cancel=False):
    data=RData();_,w=recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbb000,0xdbc000,0xf35000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    owner=0x201000;game=0x202000;table=0x203000;frame=0x204000;stack=0x21e000
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(game,table);write(table+0x1c,frame)
    u.mem_write(frame,b'\xc3');u.mem_write(0xf35b30,b'\xc3')
    u.reg_write(UC_X86_REG_EBP,owner);u.reg_write(UC_X86_REG_ESP,stack);events=[]
    def hook(uc,pc,size,user):
        if pc in (0xdbb588,0xdbcce2):uc.emu_stop();return
        if pc in (frame,0xf35b30):
            events.append(('frame',) if pc==frame else ('term',cancel))
            esp=uc.reg_read(UC_X86_REG_ESP)
            uc.reg_write(UC_X86_REG_EAX,int(cancel) if pc==0xf35b30 else 0)
            uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbc76f if tail else 0xdbb579,0x205000,count=50)
    target=u.reg_read(UC_X86_REG_EIP);assert target in (0xdbb588,0xdbcce2)
    return target==0xdbb588,events

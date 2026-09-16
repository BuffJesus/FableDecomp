"""Execute original acquisition windows through continuation or native unwind."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EBP
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_actor_phases import recover


def execute(kind='self',failures=0,stop=100,populated=True):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x200000,0x20000)
    actor=0x201000;game=0x202000;table=0x203000;frame=0x204000;acquire=0x204010;hero=0x204020;end=0x205000;refs=0x206000
    stack=0x21ef00;self_local=stack+0x3c;hero_local=stack+0x4c
    for page in (0xec4000,0xec5000,0x99a000,0xcd2000,0x7e7000,0xf35000):u.mem_map(page,4096)
    u.mem_write(0xec4b01,data.bytes_at(0xec4b01,1283))
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(actor+4,game);write(game,table);write(table+0x1c,frame);write(table+0x20,acquire);write(table+0x118,hero);write(stack+108,end)
    state={'held':{'self':kind=='hero','hero':False},'queries':0,'attempts':0,'heroes':0};events=[]
    if kind=='hero':write(self_local+8,1);write(self_local+12,refs);write(refs,2)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,actor);u.reg_write(UC_X86_REG_EBP,0)
    def local_name(pointer):return 'self' if pointer==self_local else 'hero'
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (0x99a380,0x7e72a0):
            events.append(('new',local_name(this)));write(this+8,0);write(this+12,0);pop=0
        elif pc==0xcd23b9:events.append(('prepare',local_name(this)));pop=0
        elif pc==frame:events.append(('frame',));pop=0
        elif pc==0xf35b30:
            state['queries']+=1;result=state['queries']>=stop;events.append(('term',bool(result)));pop=0
        elif pc==hero:
            state['heroes']+=1;result=0x208000+state['heroes']*16;events.append(('hero',state['heroes']));pop=0
        elif pc==acquire:
            target=read(esp+4);output=read(esp+8);priority=read(esp+12);name=local_name(output)
            assert priority==4 and target==(actor+8 if kind=='self' else 0x208000+state['heroes']*16)
            result=state['attempts']>=failures;state['attempts']+=1
            events.append(('acquire',name,bool(result),state['held'][name]))
            state['held'][name]=bool(result or populated);reference=refs+(16 if name=='hero' else 0)
            write(output+8,int(state['held'][name]));write(output+12,reference if state['held'][name] else 0);write(reference,2);pop=12
        elif pc==0x7e74d0:
            name=local_name(this);events.append(('destroy',name,state['held'][name]));state['held'][name]=False
            if read(this+12):write(read(this+12),read(read(this+12))-1)
            pop=0
        elif pc==0xec4cb5:
            events.append(('destroy','self',state['held']['self']));state['held']['self']=False
        elif pc==0x99a430:pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    boundary=0xec4b81 if kind=='self' else 0xec4cfb
    def boundary_hook(uc,pc,size,user):
        if pc==boundary:events.append(('continuation',));uc.emu_stop()
    u.hook_add(UC_HOOK_CODE,hook);u.hook_add(UC_HOOK_CODE,boundary_hook)
    u.emu_start(0xec4b01 if kind=='self' else 0xec4c0f,end,count=3000)
    assert u.reg_read(UC_X86_REG_EIP) in (end,boundary)
    return events,dict(state['held'])

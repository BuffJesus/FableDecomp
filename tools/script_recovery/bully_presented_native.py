"""Original two-poll classifier including native empty/nonempty CString equality."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EBP,UC_X86_REG_EDI,UC_X86_REG_EAX
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def execute(first,second):
    data=RData();_,w=recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbb000,0xdbc000,0x99e000,0x411000,0x122d000,0x12d8000):
        u.mem_map(page,4096);u.mem_write(page,data.bytes_at(page,4096))
    u.mem_map(0x200000,0x20000)
    stack=0x21e000;owner=0x201000;actor=owner+8;table=0x203000;poll=0x204000;stringdata=0x206000;chars=0x207000
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(actor,table);write(table+0x8c,poll);write(stack+0x50,actor);write(stringdata,chars)
    u.mem_write(poll,b'\xc3');u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_EBP,owner);u.reg_write(UC_X86_REG_EDI,actor)
    state={'count':0,'text':'','result':None};events=[]
    def hook(uc,pc,size,user):
        if pc in (0xdbbb73,0xdbbd83,0xdbba52):
            state['result']={0xdbbb73:'teddy',0xdbbd83:'none',0xdbba52:'other'}[pc];uc.emu_stop();return
        if pc==poll:
            esp=uc.reg_read(UC_X86_REG_ESP)
            assert uc.reg_read(UC_X86_REG_ECX)==actor and read(esp+4)==stack+40
            events.append(('poll.input',state['text']))
            result,text=(first,second)[state['count']];state['count']+=1
            if text is not None:state['text']=text
            write(stack+40,stringdata if state['text'] else 0)
            u.mem_write(chars,state['text'].encode()+b'\0')
            events.append(('poll.output',result,state['text']))
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+8)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbb9e9,0x205000,count=2000)
    assert state['result'] is not None
    return state['result'],events

"""Original husband Main outer dispatcher with explicit inner phase boundaries."""
import hashlib,json,struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_EIP

def prove(data=None):
    d=data or RData();w=json.loads(Path(__file__).with_name('affair_man_complete_dispatcher_witness.json').read_text())
    if hashlib.sha256(d.bytes_at(w['address'],w['size'])).hexdigest()!=w['sha256']:raise ValueError('AffairMan dispatcher bytes changed')
    return w

def execute(cancel=8,failures=0,hit=False,talk=False,busy=False,phases=(True,True,True),prepare=False,populated=True,actors=(True,True)):
    d=RData();w=prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb0000,0xdb1000,0xf35000,0x99a000,0x99e000,0xcd2000,0x7e7000,0x4aa000,0x4ab000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['address'],d.bytes_at(w['address'],w['size']))
    for pc in (0xf35b10,0xf35b30,0x4abe90,0x4aa840,0x99a380,0xcd23b9,0xcd2770,0x7e74d0,0x99ebf0,0x99eae0,0x7e7450):u.mem_write(pc,b'\xc3')
    stack=0x21e000;base=stack-0x150;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;done=0x204000
    def write(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(stack,done);write(owner+4,game);write(game,table)
    apis={0x207000:(0x1c,'frame',0),0x207010:(0x20,'acquire',3),0x207020:(0x120,'lookup',2)}
    for pc,(slot,*_) in apis.items():write(table+slot,pc);u.mem_write(pc,b'\xc3')
    events=[];state={'queries':0,'acquires':0,'condition':False,'control':False,'lookup':0};things=set();texts={}
    phase_pc={0xdb0c31:0,0xdb0e0f:1,0xdb1622:2}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0xdb0b30:events.append(('hit',hit));uc.reg_write(UC_X86_REG_EAX,int(hit));uc.reg_write(UC_X86_REG_EIP,0xdb0c1a);return
        if pc==0xdb0dc7:events.append(('talk',talk));uc.reg_write(UC_X86_REG_EBX,int(talk));uc.reg_write(UC_X86_REG_EIP,0xdb0df8);return
        if pc in phase_pc:
            index=phase_pc[pc];value=phases[index];events.append((('hit.body','talk.body','idle.body')[index],value));uc.reg_write(UC_X86_REG_EIP,0xdb1c71 if value else 0xdb1d7c);return
        if pc==0x4abe90:assert this==base+100 and read(sp+4)==actor;state['condition']=True;pop=4
        elif pc==0xf35b10:assert this==owner and read(sp+4)==base+96 and read(base+96)==0x12c2fe8;events.append(('condition',));pop=4
        elif pc==0x4aa840:
            if this==base+100:assert state['condition'];state['condition']=False
            else:assert this in things;things.remove(this);events.append(('destroy','woman' if this==base+32 else 'wife'))
            pop=0
        elif pc==0xf35b30:assert not state['condition'];state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x99a380:assert this==base+16;state['control']=True;events.append(('control.new',));pop=0
        elif pc==0xcd23b9:assert this==base+16;events.append(('prepare',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==base+16;events.append(('prepare.release',));pop=0
        elif pc==0x7e74d0:assert this==base+16 and state['control'] and not things;state['control']=False;events.append(('control.destroy',));pop=0
        elif pc==0x99ebf0:assert this==base+64 and read(sp+8)==0xffffffff;texts[this]=d.string_at(read(sp+4));events.append(('key.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('key.destroy',texts.pop(this)));pop=0
        elif pc==0x7e7450:assert this==base+16;result=busy;events.append(('task',busy));pop=0
        elif pc in apis:
            _,name,count=apis[pc];assert this==game;args=[read(sp+4+i*4) for i in range(count)];pop=count*4
            if name=='frame':events.append(('frame',))
            elif name=='acquire':assert args==[actor,base+16,4];result=state['acquires']>=failures;state['acquires']+=1;write(base+24,0x208000 if populated else 0);events.append(('acquire',result,populated))
            else:
                index=state['lookup'];state['lookup']+=1;offset=(32,44)[index];key=('NOVI_AffairWoman','NOVI_AffairWife')[index]
                assert args==[base+offset,base+64] and texts[base+64]==key;things.add(base+offset);result=base+offset;events.append(('lookup',key,actors[index]))
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(w['address'],done,count=5000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not texts and not things and not state['condition'] and not state['control'];return events

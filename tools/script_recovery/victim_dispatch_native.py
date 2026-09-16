"""Original entry/outer dispatcher; each phase body is separately byte-compared."""
import struct
from tools.script_recovery.victim_subdued_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP
def execute(plan=(True,True,'continue',True,True),cancel=6,failures=0,prepare=False,populated=True):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbc000,0xdbd000,0xf35000,0x99a000,0x99e000,0xcd2000,0x7e7000,0x4aa000,0x4ab000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    for address in (0xf35b10,0xf35b30,0x4abe90,0x4aa840,0x99a380,0xcd23b9,0xcd2770,0x7e74d0,0x99ebf0,0x99eae0):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;done=0x204000;base=stack-0x124
    def write(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(stack,done);write(owner+4,game);write(game,table);apis={0x207000:(0x1c,'frame'),0x207010:(0x20,'acquire'),0x207020:(0x120,'lookup')}
    for pc,(slot,_) in apis.items():write(table+slot,pc);u.mem_write(pc,b'\xc3')
    events=[];texts={};state={'queries':0,'acquires':0,'condition':False,'control':False,'bully':False}
    phases={0xdbce77:(0,'subdued',0xdbcf76),0xdbcf76:(1,'talk',0xdbd60f),0xdbd60f:(2,'hit',None),0xdbd9d0:(3,'repeat',0xdbdc58),0xdbdc58:(4,'complaint',0xdbdd8d)}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in phases:
            index,name,next_pc=phases[pc];value=plan[index];events.append((name,value))
            if name=='hit':next_pc={'continue':0xdbdc58,'cancel':0xdbde18,'repeat':0xdbd9d0}[value]
            elif not value:next_pc=0xdbde18
            uc.reg_write(UC_X86_REG_EIP,next_pc);return
        if pc==0x4abe90:assert this==base+84 and read(sp+4)==actor;state['condition']=True;pop=4
        elif pc==0xf35b10:assert this==owner and read(sp+4)==base+80 and read(base+80)==0x12c2fe8;events.append(('condition',));pop=4
        elif pc==0x4aa840:
            if this==base+84:assert state['condition'];state['condition']=False
            else:assert this==base+32 and state['bully'];state['bully']=False;events.append(('bully.destroy',))
            pop=0
        elif pc==0xf35b30:assert this==owner and not state['condition'];state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x99a380:assert this==base+16;state['control']=True;events.append(('control.new',));pop=0
        elif pc==0xcd23b9:assert this==base+16;events.append(('prepare',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==base+16;events.append(('prepare.release',));pop=0
        elif pc==0x7e74d0:assert this==base+16 and state['control'];state['control']=False;events.append(('control.destroy',));pop=0
        elif pc==0x99ebf0:assert read(sp+8)==0xffffffff;texts[this]=data.string_at(read(sp+4));events.append(('text.new',texts[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',texts.pop(this)));pop=0
        elif pc in apis:
            name=apis[pc][1];assert this==game
            if name=='frame':assert not state['condition'];events.append(('frame',));pop=0
            elif name=='acquire':assert [read(sp+i) for i in (4,8,12)]==[actor,base+16,4];result=state['acquires']>=failures;state['acquires']+=1;events.append(('acquire',result,populated));pop=12
            else:assert read(sp+4)==base+32 and texts[read(sp+8)]=='NOVI_Bully';state['bully']=True;events.append(('bully.new',populated));result=base+32;pop=8
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(w['mainAddress'],done,count=10000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not texts and not state['condition'] and not state['control'] and not state['bully'];return events

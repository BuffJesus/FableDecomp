"""Original Wife Main entry, outer branches, approach timing and cleanup.

Interaction, idle policy, movement setup, running-line policy and argument body
are explicit phase boundaries. Their inner engine behavior is not simulated here.
"""
import hashlib,json,struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def prove(data=None):
    d=data or RData();w=json.loads(Path(__file__).with_name('wife_dispatcher_witness.json').read_text())
    if hashlib.sha256(d.bytes_at(w['address'],w['size'])).hexdigest()!=w['sha256']:raise ValueError('Wife dispatcher bytes changed')
    return w

def execute(cancel=10,failures=0,going_after=0,near=False,approach=1,interaction=True,argument=True,hero_near=True,prepare=False,populated=True):
    d=RData();w=prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb2000,0xdb3000,0xf35000,0x99a000,0x99e000,0xcd2000,0x7e7000,0x4aa000,0x4ab000,0xcbe000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['address'],d.bytes_at(w['address'],w['size']))
    calls=(0xf35b10,0xf35b30,0x4abe90,0x4aa840,0x99a380,0xcd23b9,0xcd2770,0x7e74d0,0x99a430,0x99ebf0,0x99eae0,0x7e7360,0xcbe2ff)
    for pc in calls:u.mem_write(pc,b'\xc3')
    stack=0x21e000;base=stack-0xb4;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000;done=0x204000;hero=0x205000
    def write(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(stack,done);write(owner+4,game);write(game,table)
    apis={0x207000:(0x1c,'frame',0),0x207010:(0x20,'acquire',3),0x207020:(0x120,'lookup',2),0x207030:(0x844,'movement',2),0x207040:(0x118,'hero',0)}
    for pc,(slot,*_) in apis.items():write(table+slot,pc);u.mem_write(pc,b'\xc3')
    events=[];state={'queries':0,'acquires':0,'going':0,'approach':0,'condition':False,'control':False,'husband':False,'key':False}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0xdb2c0a:
            value=state['going']>=going_after;state['going']+=1;u.mem_write(owner+0x1c,bytes([int(value)]));events.append(('going',value))
        if pc==0xdb2c15:events.append(('interaction',interaction));uc.reg_write(UC_X86_REG_EIP,0xdb32aa if interaction else 0xdb3e16);return
        if pc==0xdb32aa:events.append(('idle',));uc.reg_write(UC_X86_REG_EIP,0xdb3394);return
        if pc==0xdb3454:events.append(('route',near));uc.reg_write(UC_X86_REG_EAX,int(near));uc.reg_write(UC_X86_REG_EIP,0xdb34a7);return
        if pc==0xdb34c7:events.append(('running',));uc.reg_write(UC_X86_REG_EIP,0xdb357b);return
        if pc==0xdb3600:events.append(('argument',argument));uc.reg_write(UC_X86_REG_EIP,0xdb3cab if argument else 0xdb3d90);return
        if pc==0x4abe90:assert this==base+68 and read(sp+4)==actor;state['condition']=True;pop=4
        elif pc==0xf35b10:assert this==owner and read(sp+4)==base+64 and read(base+64)==0x12c2fe8;events.append(('condition',));pop=4
        elif pc==0x4aa840:
            if this==base+68:assert state['condition'];state['condition']=False
            else:assert this==base+48 and state['husband'];state['husband']=False;events.append(('husband.destroy',))
            pop=0
        elif pc==0xf35b30:assert not state['condition'];state['queries']+=1;result=state['queries']>=cancel;events.append(('term',result));pop=0
        elif pc==0x99a380:assert this==base+16;state['control']=True;events.append(('control.new',));pop=0
        elif pc==0xcd23b9:assert this==base+16;events.append(('prepare',prepare));result=prepare;pop=0
        elif pc==0xcd2770:assert this==base+16;events.append(('prepare.release',));pop=0
        elif pc in (0x7e74d0,0x99a430):assert this==base+16 and state['control'];state['control']=False;events.append(('control.destroy',));pop=0
        elif pc==0x99ebf0:assert this==base+44 and d.string_at(read(sp+4))=='NOVI_AffairMan';state['key']=True;events.append(('key.new',));pop=8
        elif pc==0x99eae0:assert this==base+44 and state['key'];state['key']=False;events.append(('key.destroy',));pop=0
        elif pc==0x7e7360:assert this==base+16;events.append(('clear.commands',));pop=0
        elif pc==0xcbe2ff:
            assert this==actor
            if read(sp+4)==0x40400000:state['approach']+=1;result=state['approach']>=approach;events.append(('near.husband',result))
            else:assert read(sp+4)==0x41700000;result=hero_near;events.append(('near.hero',result))
            pop=4
        elif pc in apis:
            _,name,count=apis[pc];assert this==game;args=[read(sp+4+i*4) for i in range(count)];pop=count*4
            if name=='frame':events.append(('frame',))
            elif name=='acquire':
                assert args==[actor,base+16,3];result=state['acquires']>=failures;state['acquires']+=1
                write(base+24,hero if populated else 0);events.append(('acquire',result,populated))
            elif name=='lookup':assert state['key'] and args==[base+48,base+44];state['husband']=True;events.append(('husband.new',populated));result=base+48
            elif name=='hero':events.append(('hero',));result=hero
            else:assert args==[actor,0];events.append(('movement',False))
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(w['address'],done,count=8000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not any(state[k] for k in ('condition','control','husband','key'));return events

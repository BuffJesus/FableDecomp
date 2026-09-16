"""Execute Main's original outer dispatcher, abstracting proven phase interiors.

Hooks retain original predicate-result branches, caller cancellation queries,
loop backedge, prologue/epilogues and both normal/cancel outer cleanup paths.
Dialogue/movement/acquisition interiors are explicit separately tested calls.
"""
import struct
from tools.script_recovery.teddy_girl_health_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_EBX,UC_X86_REG_EDI,UC_X86_REG_ESI,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

BOUNDARIES={
    'condition':(0xdaf08b,0xdaf0c0),
    'talkWithTeddyPredicate':(0xdaf139,0xdaf1df),
    'questionAcquire':(0xdaf1fa,0xdaf25e),
    'questionMovie':(0xdaf25e,0xdafa9b),
    'presentedClassifierAndResponse':(0xdaf6e8,0xdafa9b),
    'departure':(0xdafa9b,0xdafc99),
    'ordinaryTalkPredicate':(0xdafc99,0xdafcc7),
    'ordinaryTalkAcquire':(0xdafcde,0xdafd42),
    'ordinaryTalkMovie':(0xdafd42,0xdb018e),
    'hitPredicate':(0xdb018e,0xdb0267),
    'hitResponse':(0xdb0273,0xdb042b),
}

def execute(schedule=((False,False,False),),stop=None,cancel=999,iterations=2,prepare=False):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdaf000,0xdb0000,0x99a000,0x99e000,0xf35000,0xcd2000,0x4aa000,0x7e7000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    for address in (0x99a380,0x99ebf0,0x99eae0,0x99e4b0,0xf35b30,0xcd23b9,0xcd2770,0x99a2e0,0x99a430,0x4aa840,0x7e74d0):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;done=0x204000;frame=0x205000;lookup=0x205010
    def write(a,v):u.mem_write(a,struct.pack('<I',v))
    def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    write(stack,done);write(owner+4,game);write(game,table);write(table+0x1c,frame);write(table+0x120,lookup)
    events=[];live=[];texts={};state={'frame':0,'query':0,'iteration':-1,'base':None}
    def close(name):assert live.pop()==name;events.append('destroy:'+name)
    def phase(name):events.append(name);return name!=stop
    def jump(address):u.reg_write(UC_X86_REG_EIP,address)
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0xdaf08b:events.append('condition');uc.reg_write(UC_X86_REG_EDI,0);jump(0xdaf0c0);return
        if state['iteration']>=0:question,talk,hit=schedule[state['iteration']%len(schedule)]
        if pc==0xdaf139:
            events.append('talkTeddy');uc.mem_write(state['base']+39,bytes([question]));uc.reg_write(UC_X86_REG_EBX,owner+8);jump(0xdaf1df);return
        if pc in (0xdaf1fa,0xdafcde):jump((0xdaf25e if pc==0xdaf1fa else 0xdafd42) if phase('acquire') else 0xdb05da);return
        if pc in (0xdaf25e,0xdafd42):
            events.append('movie');complete=phase('question' if pc==0xdaf25e else 'talk');events.append('movie.end');jump((0xdafa9b if pc==0xdaf25e else 0xdb018e) if complete else 0xdb05da);return
        if pc==0xdaf6e8:events.append('presentedKind');jump(0xdafa9b if phase('presented') else 0xdb05da);return
        if pc==0xdafa9b:jump(0xdafc99 if phase('departure') else 0xdb05da);return
        if pc==0xdafc99:events.append('talkPredicate');uc.reg_write(UC_X86_REG_EBX,int(talk));uc.reg_write(UC_X86_REG_ESI,owner+8);jump(0xdafcc7);return
        if pc==0xdb018e:events.append('hitPredicate');uc.mem_write(state['base']+39,bytes([hit]));jump(0xdb0267);return
        if pc==0xdb0273:jump(0xdb042b if phase('hit') else 0xdb05da);return
        if pc==frame:state['frame']+=1;events.append('frame');pop=0
        elif pc==0xf35b30:
            assert this==owner;state['query']+=1;result=state['query']>=cancel or state['frame']>iterations;events.append('term:'+str(int(result)));pop=0
        elif pc==0x99a380:
            state['base']=this-20;live.append('control');events.append('new:control');pop=0
        elif pc==0x99ebf0:texts[this]=data.string_at(read(sp+4));assert texts[this]=='NOVI_Bully';pop=8
        elif pc==lookup:
            assert texts[read(sp+8)]=='NOVI_Bully' and read(sp+4)==state['base']+40
            live.append('bully');events.append('new:bully');write(read(sp+4)+4,0);write(read(sp+4)+8,0);result=read(sp+4);pop=8
        elif pc==0x99e4b0:assert this==state['base']+16;state['iteration']+=1;live.append('output');events.append('new:output');pop=0
        elif pc==0x99eae0:
            if this in texts:del texts[this]
            else:assert this==state['base']+16;close('output')
            pop=0
        elif pc in (0x99a2e0,0x4aa840):assert this==state['base']+40;close('bully');pop=0
        elif pc in (0x99a430,0x7e74d0):assert this==state['base']+20;close('control');pop=0
        elif pc==0xcd23b9:assert this==state['base']+20;events.append('prepare:'+str(int(prepare)));result=prepare;pop=0
        elif pc==0xcd2770:assert this==state['base']+20;events.append('prepare.release');pop=0
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(w['mainAddress'],done,count=8000)
    assert u.reg_read(UC_X86_REG_EIP)==done and u.reg_read(UC_X86_REG_ESP)==stack+4 and not live and not texts
    return events

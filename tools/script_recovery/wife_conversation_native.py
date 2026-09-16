"""Original intermittent conversation, including CString around fresh Hero."""
import hashlib,json,struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def prove(data=None):
    d=data or RData();w=json.loads(Path(__file__).with_name('wife_conversation_witness.json').read_text())
    if hashlib.sha256(d.bytes_at(w['address'],w['size'])).hexdigest()!=w['sha256']:raise ValueError('Wife conversation bytes changed')
    if d.string_at(w['literal'])!=w['text']:raise ValueError('Wife conversation key changed')
    for slot,pc in w['slots'].items():
        if int.from_bytes(d.bytes_at(0x1260f0c+int(slot,16),4),'little')!=pc:raise ValueError('Wife conversation API changed')
    return w

def execute(conversation=0,heroes=(False,True)):
    d=RData();w=prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb3000,0x99e000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['address'],d.bytes_at(w['address'],w['size']))
    stack=0x21e000;owner=0x201000;actor=owner+8;game=0x202000;table=0x203000
    def write(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(game,table);u.mem_write(owner+0x1d,b'\1')
    apis={0x204000:(0x5b0,3,'new'),0x204010:(0x118,0,'hero'),0x204020:(0x5b4,2,'person'),0x204030:(0x5b8,5,'line')}
    for pc,(slot,*_) in apis.items():write(table+slot,pc);u.mem_write(pc,b'\xc3')
    for pc in (0x99ebf0,0x99eae0):u.mem_write(pc,b'\xc3')
    state={'heroes':0,'key':False};events=[]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0xdb3338:events.append(('state','ForceFirstTimeSpeak',False))
        if pc==0x99ebf0:
            assert this==stack+44 and read(sp+4)==w['literal'] and read(sp+8)==0xffffffff
            state['key']=True;events.append(('key.new',));pop=8
        elif pc==0x99eae0:assert this==stack+44 and state['key'];state['key']=False;events.append(('key.destroy',));pop=0
        elif pc in apis:
            _,count,name=apis[pc];assert this==game;args=[read(sp+4+i*4) for i in range(count)];pop=count*4
            if name=='new':assert args==[actor,0,0] and bytes(u.mem_read(owner+0x1d,1))==b'\0';result=conversation;events.append(('new',conversation,False,False))
            elif name=='hero':
                index=state['heroes'];state['heroes']+=1;assert state['key']==(index==1)
                result=(0x205000+index*16) if heroes[index] else 0;events.append(('hero',index,heroes[index]))
            elif name=='person':assert args==[conversation&0xffffffff,0x205000 if heroes[0] else 0];events.append(('person',conversation,heroes[0]))
            else:assert state['key'] and args==[conversation&0xffffffff,stack+44,0,actor,0x205010 if heroes[1] else 0];events.append(('line',conversation,heroes[1],False))
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EDI,actor);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(w['address'],w['address']+w['size'],count=200)
    assert not state['key'] and state['heroes']==2;return events

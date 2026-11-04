"""Retail DoConversation, with explicit engine/resource test boundaries."""
import hashlib
import json
from pathlib import Path
import struct
import sys

W=Path(__file__).resolve().parent
R=W.parents[1]
sys.path.insert(0,str(R))
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from tools.script_recovery.lift_native_lua import RData
DATA=RData()

def execute(speaker='boy0',dialogue='TEXT_REACTION',animation='START',loop='NULL',
            book=2,line=1,conversation_frames=2,task_frames=3,reading_at=999,terminate_at=999,initial_term=False,
            outer=False,lines=3,acquire=True):
    fn=json.loads((W/'book_reaction_native_witness.json').read_text())['0x00e569d0']
    witness=json.loads((W/'book_reaction_native_witness.json').read_text())['0x00e569d0']
    start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
    raw=DATA.bytes_at(start,end-start)
    assert hashlib.sha256(raw).hexdigest()==witness['nativeSha256']
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xe56000,0xe57000,0xcb7000,0x99e000,0x411000,0x4aa000,
                 0x122d000,0x1239000,0x12e6000,0x1375000,0x143e000,0xcd2000):
        u.mem_map(page,4096)
        contents=DATA.bytes_at(page,4096)
        if contents:u.mem_write(page,contents)
    u.mem_map(0x200000,0x30000)
    u.mem_write(start,raw)
    if outer:
        worker=json.loads((W/'book_reaction_native_witness.json').read_text())['0x00e566f0']
        begin,finish=int(worker['address'],16),int(worker['bodyEndExclusive'],16)
        code=DATA.bytes_at(begin,finish-begin)
        assert hashlib.sha256(code).hexdigest()==json.loads((W/'book_reaction_native_witness.json').read_text())['0x00e566f0']['nativeSha256']
        u.mem_write(begin,code)
    stack,owner,game,table,vector,done=0x22e000,0x201000,0x202000,0x203000,0x210000,0x208000
    read=lambda at:struct.unpack('<I',u.mem_read(at,4))[0]
    def write(at,value):u.mem_write(at,struct.pack('<I',value&0xffffffff))
    heap=0x214000
    def string(at):
        chars=[]
        while at:
            value=u.mem_read(at,1)[0]
            if not value:break
            chars.append(value);at+=1
        return bytes(chars).decode()
    def get_object(at):
        block=read(at);return string(read(block)) if block else ''
    def put_object(at,text):
        nonlocal heap
        block=heap;heap+=0x100
        write(block,block+16);u.mem_write(block+16,text.encode()+b'\0');write(at,block)
    write(owner+0x40,game);write(game,table);write(owner+0x4c,vector)
    for n,(offset,text) in enumerate(((0x2c,speaker),(0x38,dialogue),(0x44,animation),(0x50,loop))):
        array=0x211000+n*0x100
        write(vector+book*0x5c+offset,array)
        if outer:
            for index in range(lines):
                value=(('boy0','girl0')[index%2] if offset==0x2c else
                       (text+'_'+str(index) if offset in (0x38,0x44) and text!='NULL' else text))
                put_object(array+index*4,value)
        else:put_object(array+line*4,text)
    write(vector+book*0x5c+0x28,lines)
    write(stack,done);write(stack+4,book);write(stack+8,line)
    write(0x143e90c,0x205000)
    for offset,array,key in ((0x4b0,0x212000,'marker_boy'),(0x4bc,0x212100,'marker_girl')):
        write(0x205000+offset,array);put_object(array+book*4,key)
    boy,girl=owner+0x58,owner+0x68
    write(boy,0x204000);write(girl,0x204000)
    u.mem_write(0x1375748,b'\x01')
    events=[];live=set();thing_slots=set();frame=0;reading=reading_at<=0
    u.mem_write(owner+0x48,bytes([reading]));conversation_start=0;conversation_id=6
    task_deadline={'boy':task_frames,'girl':task_frames}
    actor_names={0x209000:'boy0',0x209010:'girl0',0x209020:'marker_boy',0x209030:'marker_girl'}
    def actor(at):return actor_names[read(at+4)]
    apis={0x206000:(0x120,'lookup',2),0x206010:(0x5b0,'new',3),
          0x206020:(0x118,'hero',0),0x206030:(0x5b4,'person',2),
          0x206040:(0x5b8,'line',5),0x206050:(0x5c0,'active',1),
          0x206060:(0x1c,'frame',0),0x206070:(0x5bc,'remove',2),
          0x2060b0:(0x20,'acquire',3),0x2060c0:(0x760,'teleport',3),0x2060d0:(0xd30,'pushable',4)}
    resource_apis={0x206080:(0x48,'animation',8),0x206090:(0x50,'loop',9),0x2060a0:(0x68,'task',0),
                   0x2060e0:(0x58,'clear.actions',0),0x2060f0:(0x28,'clear.commands',0)}
    for pc,(slot,*_) in apis.items():write(table+slot,pc)
    for pc,(slot,*_) in resource_apis.items():write(0x204000+slot,pc)
    labels={boy:'boy',girl:'girl'}
    def hook(uc,pc,size,user):
        nonlocal frame,reading,conversation_start,conversation_id
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x99ebf0:
            assert this not in live and read(sp+8)==0xffffffff
            put_object(this,string(read(sp+4)));live.add(this);result=this;pop=8
        elif pc==0x99ec30:
            assert this not in live;put_object(this,get_object(read(sp+4)));live.add(this);result=this;pop=4
        elif pc==0x99eae0:
            assert this in live;live.remove(this);write(this,0);pop=0
        elif pc==0x99e960:
            result=get_object(this)!=string(read(sp+4));pop=4
        elif pc==0x411570:
            left,right=string(this),string(uc.reg_read(UC_X86_REG_EDX));result=(left>right)-(left<right);pop=0
        elif pc==0x4aa840:
            assert this in thing_slots;thing_slots.remove(this);pop=0
        elif pc==0xcb7940:
            assert this==owner;result=initial_term or frame>=terminate_at;pop=0
        elif pc==0xcd2770:
            assert this in labels;events.append(('prepare',labels[this]));pop=0
        elif pc in apis:
            slot,name,count=apis[pc];assert this==game
            args=[read(sp+4+i*4) for i in range(count)];pop=count*4
            if name=='lookup':
                output,key=args;name=get_object(key)
                assert name in actor_names.values()
                thing_slots.add(output);write(output,0x01238c8c)
                write(output+4,next(p for p,n in actor_names.items() if n==name));write(output+8,0)
                result=output
            elif name=='new':
                assert args[0] in thing_slots and args[1:]==[0,0]
                conversation_id+=1;conversation_start=frame
                events.append(('new',actor(args[0])));result=conversation_id
            elif name=='hero':result=0x209100
            elif name=='person':assert args==[conversation_id,0x209100];events.append(('person',conversation_id,'hero'))
            elif name=='line':
                assert args[0]==conversation_id and args[2]==0 and args[3] in thing_slots and args[4]==0x209100
                events.append(('line',conversation_id,get_object(args[1]),actor(args[3]),'hero',False))
            elif name=='active':
                conversation=struct.unpack('<i',struct.pack('<I',args[0]))[0]
                result=conversation!=-1 and frame-conversation_start<conversation_frames
                events.append(('active',conversation,bool(result)))
            elif name=='frame':
                frame+=1;reading=frame>=reading_at;u.mem_write(owner+0x48,bytes([reading]));events.append(('frame',frame))
            elif name=='remove':
                conversation=struct.unpack('<i',struct.pack('<I',args[0]))[0]
                assert args[1]==0;events.append(('remove',conversation,False))
            elif name=='acquire':
                assert args[1] in labels and args[2]==4
                events.append(('acquire',labels[args[1]],actor(args[0]),4,bool(acquire)));result=acquire
            elif name=='teleport':
                assert args[2]==0;events.append(('teleport',actor(args[0]),actor(args[1]),False))
            elif name=='pushable':
                assert args[0]==0x01238c8c and args[2]==0
                events.append(('pushable',actor_names[args[1]],bool(args[3])))
        elif pc in resource_apis:
            slot,name,count=resource_apis[pc];assert this in labels
            args=[read(sp+4+i*4) for i in range(count)];pop=count*4
            if name=='task':result=frame<task_deadline[labels[this]];events.append(('task',labels[this],bool(result)))
            elif name=='animation':
                task_deadline[labels[this]]=frame+task_frames
                events.append(('animation',labels[this],get_object(args[0]),*map(bool,args[1:])))
            elif name=='loop':
                assert args[1]==0xffffffff
                events.append(('loop',labels[this],get_object(args[0]),-1,*map(bool,args[2:])))
            else:
                events.append((name,labels[this]))
                if name=='clear.actions':task_deadline[labels[this]]=frame
        if pop is not None:
            ret=read(sp);uc.reg_write(UC_X86_REG_EAX,int(result)&0xffffffff)
            uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xe566f0 if outer else start,done,count=30000)
    assert u.reg_read(UC_X86_REG_EIP)==done and not live and not thing_slots
    assert u.reg_read(UC_X86_REG_ESP)==stack+(8 if outer else 12)
    return events

if __name__=='__main__':
    import argparse
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cases',type=Path,required=True)
    parser.add_argument('--out',type=Path,required=True)
    args=parser.parse_args()
    results=[execute(**case) for case in json.loads(args.cases.read_text())]
    args.out.write_text(json.dumps(results)+'\n',encoding='utf-8')
    print('Original native traces:',len(results))

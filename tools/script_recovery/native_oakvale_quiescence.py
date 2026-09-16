"""Execute original quest process termination before Lua VM destruction."""
import hashlib
import json
import struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_EIP


def prove(data=None):
    data = data or RData()
    witness = json.loads(Path(__file__).with_name('native_oakvale_quiescence_witness.json').read_text())
    for row in witness['regions']:
        if hashlib.sha256(data.bytes_at(row['address'],row['size'])).hexdigest() != row['sha256']:
            raise ValueError('Native process termination bytes changed')
    return witness


def execute(threads, entities, resumes, entity_flag=False, thing_valid=False):
    data=RData(); witness=prove(data); u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(0xcb7000,0x5000);u.mem_map(0xa4b000,0x1000);u.mem_map(0x200000,0x20000)
    u.mem_map(0x1238000,0x1000);u.mem_map(0x99a000,0x1000)
    for row in witness['regions'][:2]:u.mem_write(row['address'],data.bytes_at(row['address'],row['size']))
    def put(address,value):u.mem_write(address,struct.pack('<I',value))
    def get(address):return struct.unpack('<I',u.mem_read(address,4))[0]
    owner=0x201000;head=0x202000;begin=0x204000;stack=0x21f000;done=0x200100
    put(owner+4,head);put(owner+0x2c,0x123456);put(owner+8,begin);put(owner+12,begin+entities*16)
    put(head,head+16 if threads else head);put(head+4,head+threads*16 if threads else head)
    processes={};remaining={};events=[];thing_events=[]
    for index in range(threads+entities):
        process=0x206000+index*0x100;processes[process]=index;remaining[process]=resumes
        put(process,0x205000);u.mem_write(process+4,bytes([bool(resumes),0]))
        if index<threads:
            node=head+16*(index+1);put(node,head if index+1==threads else node+16);put(node+8,process)
        else:
            put(begin+(index-threads)*16+8,process)
            put(process+0x3c,int(entity_flag));put(process+0x4c,process+0x80);put(process+0x50,process+0x90)
            put(process+0x90,2)
    put(0x205004,0x200200);put(stack,done)
    put(0x1238c8c+0x12c,0x200210);put(0x1238c8c+0x118,0x200220)
    targets=(0x200200,0x200210,0x200220,0x99a2e0,0xcbb090,0xcbb120,0xcbb1b0)
    for address in targets:u.mem_write(address,b'\xc3')
    def hook(uc,pc,size,user):
        if pc not in targets:return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=0
        if pc==0x200200:
            assert get(owner+0x2c)==this and bytes(uc.mem_read(this+5,1))==b'\x01'
            remaining[this]-=1;events.append(processes[this]);uc.mem_write(this+4,bytes([bool(remaining[this])]))
        elif pc in (0x200210,0x200220):
            process=get(this+4)-0x80
            assert get(owner+0x2c)==process and get(this+8)==process+0x90 and get(process+0x90)==3
            assert remaining[process]==resumes
            thing_events.append(('valid' if pc==0x200210 else 'kill',processes[process]))
            if pc==0x200210:uc.reg_write(UC_X86_REG_EAX,int(thing_valid))
            else:assert get(sp+4)==1;pop=4
        elif pc==0x99a2e0:
            assert get(this+4)==0 and get(this+8)==0
        elif pc==0xcbb090:
            assert this==owner+4 and all(remaining[process]==0 for process,index in processes.items() if index<threads)
            put(head,head);put(head+4,head)
        elif pc==0xcbb120:
            assert get(owner+0x2c)==0x123456;uc.reg_write(UC_X86_REG_EAX,begin);pop=12
        else:
            assert this==begin and all(value==0 for value in remaining.values());pop=4
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ECX,owner);u.reg_write(UC_X86_REG_ESP,stack);u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start(0xcb7f60,done,count=10000)
    assert events==[index for index in range(threads+entities) for _ in range(resumes)]
    assert get(owner+0x2c)==0x123456 and get(owner+12)==begin and get(head)==head
    assert u.reg_read(UC_X86_REG_ESP)==stack+4
    assert thing_events==[(action,index) for index in range(threads,threads+entities) if entity_flag for action in (('valid','kill') if thing_valid else ('valid',))]
    for process,index in processes.items():
        if index>=threads:assert get(process+0x90)==2
    for process in processes:
        assert bytes(u.mem_read(process+4,2))==b'\x00\x01'
    # An engine drain before host destruction leaves an inert second drain.
    previous=list(events);put(stack,done)
    u.reg_write(UC_X86_REG_ECX,owner);u.reg_write(UC_X86_REG_ESP,stack)
    u.emu_start(0xcb7f60,done,count=10000)
    assert events==previous and get(owner+0x2c)==0x123456
    return events

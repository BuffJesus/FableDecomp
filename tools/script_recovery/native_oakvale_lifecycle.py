"""Execute retail scalar destruction, speech vectors and base cleanup ordering."""
import hashlib
import json
import struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EAX,UC_X86_REG_EIP


def prove(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('native_oakvale_lifecycle_witness.json').read_text())
    for row in w['regions']:
        if hashlib.sha256(data.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:
            raise ValueError('Oakvale lifecycle native bytes changed')
    return w


def execute(flags=0,lengths=(0,1,2,0,2,1,0,1),allocated_empty=False,watch=-1,ambient=0):
    d=RData();w=prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbe000,0xdbf000,0xcbd000,0xcbb000,0x99e000,0x99a000,0xbfe000,0x143e000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for row in w['regions']:u.mem_write(row['address'],d.bytes_at(row['address'],row['size']))
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    owner=0x201000;games=(0x202000,0x202100);table=0x203000;stack=0x21f000;done=0x204000
    strings={};buffers={};events=[];timer_calls=0
    put(stack,done);put(stack+4,flags);put(owner+0x104,ambient);put(owner+0x108,watch)
    for game in games:put(game,table)
    put(table+0x160,0x204100);put(0x143e8f8,games[0])
    for index,(offset,length) in enumerate(zip(w['speechOffsets'],lengths)):
        begin=0x205000+index*0x100 if length or allocated_empty else 0
        put(owner+offset,begin);put(owner+offset+4,begin+length*4)
        if begin:buffers[begin]=('speech.free',index)
        for item in range(length):strings[begin+item*4]=('speech.string',index,item)
    # Nonempty native base containers exercise every distinct cleanup branch.
    put(owner+0x30,0x206000);put(owner+0x34,0x206030)
    for i in range(2):strings[0x206010+i*0x18]=('base.string',i)
    for offset,address in ((0x18,0x206100),(8,0x206200)):
        put(owner+offset,address);put(owner+offset+4,address+16)
    put(owner+4,0x206300)
    buffers.update({a:('base.free',offset) for offset,a in ((0x30,0x206000),(0x18,0x206100),(8,0x206200),(4,0x206300))})
    targets=(0x204100,0x99eae0,0xbfea14,0xbfe9bc,0xcbb200,0xcbb1b0,0xcbb090,0x99a300)
    for pc in targets:u.mem_write(pc,b'\xc3')
    def hook(uc,pc,size,user):
        nonlocal timer_calls
        if pc not in targets:return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=0
        if pc==0x204100:
            assert this==games[timer_calls];value=get(sp+4);value=value if value<2**31 else value-2**32
            events.append(('timer',value));timer_calls+=1;put(0x143e8f8,games[1]);pop=4
        elif pc==0x99eae0:events.append(strings.pop(this))
        elif pc==0xbfea14:events.append(buffers.pop(get(sp+4)))
        elif pc==0xbfe9bc:assert get(sp+4)==owner;events.append(('owner.free',))
        elif pc in (0xcbb200,0xcbb1b0):
            offset=0x18 if pc==0xcbb200 else 8
            assert this==get(owner+offset) and uc.reg_read(UC_X86_REG_EDX)==get(owner+offset+4)
            events.append(('base.range',offset));pop=4
        elif pc==0xcbb090:assert this==owner+4;events.append(('base.map',))
        else:assert this==owner;events.append(('base.object',))
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start(0xdbefa0,done,count=3000)
    assert not strings and not buffers and timer_calls==2 and u.reg_read(UC_X86_REG_EAX)==owner and u.reg_read(UC_X86_REG_ESP)==stack+8
    return events

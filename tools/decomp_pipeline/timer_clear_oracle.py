"""Comparative x86 query-list cleanup with mocked STLport and COM boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9f7250
SYSTEM=0x2000000
PENDING=SYSTEM+0x1000
FREE=SYSTEM+0x1100
QUERY=SYSTEM+0x2000
STACK=0x3000000
CALLBACK=0x2200000
STOP=CALLBACK+0x100
SIZE=0x9f7dc0
CONSTRUCT=0x41c780
DECREMENT=0x41c7b0
POP=0x9f7c50

def execute(code,pending_count,free_count,mutation,result):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in [(0x9f7000,4096),(0x41c000,4096),(SYSTEM,0x4000),(STACK,0x10000),(CALLBACK,4096)]:uc.mem_map(a,n)
    def put(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    uc.mem_write(ENTRY,bytes(code));uc.mem_write(SYSTEM,b'\xa5'*0x4000)
    put(SYSTEM+4,FREE);put(SYSTEM+8,PENDING)
    for head in [FREE,PENDING]:put(head,head);put(head+4,head)
    put(QUERY+0x500+8,CALLBACK)
    for i in range(32):put(QUERY+i*16,QUERY+0x500);put(QUERY+i*16+4,i)
    serial=0
    def append(head,query_id):
        nonlocal serial
        node=SYSTEM+0x1200+serial*24;serial+=1
        previous=get(head+4);put(node,head);put(node+4,previous);put(node+8,QUERY+query_id*16)
        put(node+12,query_id);uc.mem_write(node+16,struct.pack('<d',query_id+0.25))
        put(previous,node);put(head+4,node)
    for i in range(pending_count):append(PENDING,i+1)
    for i in range(free_count):append(FREE,i+10)
    original=bytes(uc.mem_read(SYSTEM,20));events=[];changed=False;returned=False
    def hook(uc,a,size,data):
        nonlocal changed,returned
        if a==STOP:returned=True;uc.emu_stop();return
        if a not in [SIZE,CONSTRUCT,DECREMENT,POP,CALLBACK]:return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=0;value=0
        if a==SIZE:
            assert this in [SYSTEM+4,SYSTEM+8];head=get(this);node=get(head);count=0
            while node!=head:node=get(node);count+=1;assert count<16
            value=count;events.append(('size',this,count))
        elif a==CONSTRUCT:
            pop=4;head=get(sp+4);assert head in [PENDING,FREE];put(this,head);value=this;events.append(('construct',head))
        elif a==DECREMENT:
            node=get(get(this)+4);put(this,node);value=this;events.append(('decrement',node))
        elif a==POP:
            assert this in [SYSTEM+4,SYSTEM+8];head=get(this);node=get(head+4);assert node!=head
            previous=get(node+4);following=get(node);put(previous,following);put(following+4,previous)
            uc.mem_write(node,b'\xdd'*24);events.append(('pop',this,node))
        else:
            pop=4;query=get(sp+4);query_id=get(query+4);assert query==QUERY+query_id*16
            events.append(('release',query_id));value=[0,1,0xffffffff][result]
            if not changed:
                if mutation==1 and query_id<10:append(PENDING,30);changed=True
                if mutation==2 and query_id<10:append(FREE,30);changed=True
                if mutation==3 and query_id>=10:append(PENDING,31);changed=True
        ret=get(sp);uc.reg_write(UC_X86_REG_EAX,value);uc.reg_write(UC_X86_REG_ECX,0xdead0001);uc.reg_write(UC_X86_REG_EDX,0xdead0002)
        uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    uc.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
    uc.reg_write(UC_X86_REG_ESP,sp);uc.reg_write(UC_X86_REG_ECX,SYSTEM)
    preserved=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
    for i,r in enumerate(preserved):uc.reg_write(r,0xabcd0000+i)
    uc.emu_start(ENTRY,STOP+1,count=10000)
    assert returned and uc.reg_read(UC_X86_REG_ESP)==sp+4
    for i,r in enumerate(preserved):assert uc.reg_read(r)==0xabcd0000+i
    assert bytes(uc.mem_read(SYSTEM,20))==original
    assert get(FREE)==FREE and get(FREE+4)==FREE
    # A release in the free-list phase can enqueue pending work; retail does
    # not return to its already-completed pending-list phase.
    if mutation==3 and free_count:
        node=get(PENDING);assert get(node+8)==QUERY+31*16 and get(node)==PENDING and get(node+4)==PENDING
    else:assert get(PENDING)==PENDING and get(PENDING+4)==PENDING
    return events,bytes(uc.mem_read(SYSTEM,0x4000))

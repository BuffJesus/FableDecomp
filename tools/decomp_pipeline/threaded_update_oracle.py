"""Time-slice float storage and worker wake contracts against original x86."""
import hashlib
import itertools
import struct
import tempfile
from pathlib import Path
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_FPCW

R=Path(__file__).resolve().parents[2]
BASE,MANAGER,STACK,STOP,API=0x98e000,0x2000100,0x3000000,0x4000000,0x5000000
PATTERNS=(0,0x80000000,0x3f800000,0xbf800000,0x7f7fffff,0xff7fffff,
          1,0x80000001,0x00800000,0x80800000,0x7f800000,0xff800000,
          0x7fc12345,0xffc12345)


def execute(codes,entry,args,seed,event=0,event_result=1,sleep_result=0):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,size in ((BASE,0x1000),(0x143f000,0x1000),(MANAGER-0x100,0x1000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,size)
    for a,b in codes.items():u.mem_write(a,b)
    initial=bytearray((i*19+seed*23)&255 for i in range(0x300))
    struct.pack_into('<I',initial,0x264,event);u.mem_write(MANAGER-0x100,bytes(initial))
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    put(0x143fde0,API);put(0x143fddc,API+16)
    u.mem_write(API,b'\xc3');u.mem_write(API+16,b'\xc3');trace=[]
    def hook(uc,a,size,user):
        if a not in (API,API+16):return
        sp=uc.reg_read(UC_X86_REG_ESP)
        if a==API:
            assert get(sp+4)==event;trace.append(('set_event',event));pop=4;result=event_result
        else:
            assert get(sp+4)==0 and get(sp+8)==1
            trace.append(('sleep_ex',0,1));pop=8;result=sleep_result
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook)
    sp=STACK+0x8000;put(sp,STOP)
    for i,arg in enumerate(args):put(sp+4+4*i,arg)
    saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
    u.reg_write(UC_X86_REG_ECX,MANAGER);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_FPCW,0x37f)
    u.emu_start(entry,STOP,count=200)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4+len(args)*4
    assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
    expected=bytearray(initial)
    if entry==0x98e170:
        device,bits=args
        if bits&0x7f800000==0x7f800000 and bits&0x7fffff:bits|=0x400000
        struct.pack_into('<I',expected,0x270+4*device,bits)
        assert not trace
    else:assert trace==[('set_event',event),('sleep_ex',0,1)]
    state=bytes(u.mem_read(MANAGER-0x100,len(initial)))
    assert state==bytes(expected),(hex(entry),args,[(hex(i),x,y) for i,(x,y) in enumerate(zip(state,expected)) if x!=y])
    return state,tuple(trace)


def compare_update(roots,addresses):
    inputs={Path(__file__),EXE,R/'tools/decomp_pipeline/movable_pool_construct_oracle.py',*(r['source'] for r in roots)}
    visited=set()
    def closure(path):
        if path in visited:return
        visited.add(path);inputs.add(path)
        import re
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q=next((q for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None)
            assert q,name
            closure(q)
    for root in roots:closure(root['source'])
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='threaded-manager-update-') as tmp:built=compile_codes(roots,addresses,Path(tmp))
    raw=EXE.read_bytes();native={a:read_virtual_data(raw,a,len(b)) for a,b in built.items()}
    assert built==native
    cases=0
    for device,bits,seed in itertools.product((0,1),PATTERNS,range(3)):
        assert execute(built,0x98e170,[device,bits],seed)==execute(native,0x98e170,[device,bits],seed);cases+=1
    for event,ev_result,sleep_result,seed in itertools.product((0,0x12345678),(0,1),(0,0xc0,0xffffffff),range(3)):
        params=(0x98e190,[],seed,event,ev_result,sleep_result)
        assert execute(built,*params)==execute(native,*params);cases+=1
    for entry,args in ((0x98e170,[0,0]),(0x98e190,[])):
        detected=False
        try:execute({**built,entry:b'\xcc'},entry,args,0)
        except Exception:detected=True
        assert detected
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
    return dict(status='PASS',cases=cases,setter_cases=84,wake_cases=36,missing_provider_checks=2,input_sha256=pins,
                boundaries=['Two operational devices; x87 masked exceptions; signaling NaN semantics excluded because Unicorn preserves signaling payloads unlike x87 hardware.',
                            'SetEvent/SleepEx controlled, including ignored failure/APC returns; no actual worker thread.'])

"""Worker-exit traversal, resource release and finished publication vs retail."""
import hashlib
import itertools
import struct
import tempfile
from pathlib import Path
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R=Path(__file__).resolve().parents[2]
ENTRY,PROCESS,MANAGER,HEADER,NODES,OBJECTS,OVERLAPS=0x98f2a0,0x98ecd0,0x2000100,0x2001000,0x2001100,0x2002000,0x2003000
STACK,STOP,API=0x3000000,0x4000000,0x5000000
BEGIN=0x98f380
SYMBOL='?ThreadMain@CThreadedFileManager@@AAEXXZ'
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileManager_ThreadMain_0098f2a0.cpp'
ADDRESS={'?ProcessObjects@CThreadedFileManager@@AAEXXZ':PROCESS,
         '__imp__GlobalFree@4':0x143fdf8,'??3@YAXPAX@Z':0xbfe9bc,
         '__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10}


def execute(code,args,bootstrap=None):
    count,shape,present,overlapped,cycles,finished,free_result=args
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,size in ((0x98e000,0x2000),(0xbfe000,0x1000),(0x143f000,0x1000),
                   (0x2000000,0x4000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,size)
    u.mem_write(ENTRY,code);u.mem_write(PROCESS,b'\xc3');u.mem_write(0xbfe9bc,b'\xc3')
    if bootstrap is not None:u.mem_write(BEGIN,bootstrap)
    u.mem_write(0x2000000,bytes((i*17+count*31)&255 for i in range(0x4000)))
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    for i,slot in enumerate((0x143fdf8,0x143fe0c,0x143fe10)):
        put(slot,API+i*16);u.mem_write(API+i*16,b'\xc3')
    for i,slot in enumerate((0x143fdcc,0x143fdd0),3):
        put(slot,API+i*16);u.mem_write(API+i*16,b'\xc3')
    u.mem_write(MANAGER+0x149,bytes([int(not cycles),int(finished)]))
    put(MANAGER+0x13c,HEADER);put(MANAGER+0x140,count)
    objects={};blocks=set()
    for i in range(count):
        obj=OBJECTS+i*64;node=NODES+i*24;overlap=OVERLAPS+i*32
        active=present=='all' or present=='even' and i%2==0
        has_overlap=overlapped=='all' or overlapped=='odd' and i%2==1
        put(node+16,i+1);put(node+20,obj if active else 0)
        if active:
            objects[obj]=i;put(obj+0x20,overlap if has_overlap else 0)
            if has_overlap:blocks.add(overlap)
    def build(indices,parent):
        if not indices:return 0
        split=0 if shape=='right' else len(indices)-1 if shape=='left' else len(indices)//2
        at=indices[split];node=NODES+at*24;put(node,1);put(node+4,parent)
        put(node+8,build(indices[:split],node));put(node+12,build(indices[split+1:],node));return node
    put(HEADER,0);put(HEADER+4,build(list(range(count)),HEADER))
    put(HEADER+8,NODES if count else HEADER);put(HEADER+12,NODES+(count-1)*24 if count else HEADER)
    initial=bytes(u.mem_read(0x2000000,0x4000));expected=bytearray(initial)
    expected[MANAGER+0x149-0x2000000]=1;expected[MANAGER+0x14a-0x2000000]=1
    for obj in objects:expected[obj-0x2000000:obj-0x2000000+64]=b'\xdd'*64
    if not free_result:
        for block in blocks:expected[block-0x2000000:block-0x2000000+20]=b'\xee'*20
    retained=set();trace=[];depth=0;processed=0
    def hook(uc,a,size,user):
        nonlocal depth,processed
        if a not in (PROCESS,0xbfe9bc,API,API+16,API+32,API+48,API+64):return
        sp=uc.reg_read(UC_X86_REG_ESP);pop=0;result=1
        if a==API+48:
            assert bootstrap is not None and not trace and depth==0
            result=0xfffffffe;trace.append(('get_current_thread',))
        elif a==API+64:
            assert get(sp+4)==0xfffffffe and get(sp+8)==1 and depth==0
            trace.append(('set_thread_priority',1));pop=8;result=free_result
        elif a==PROCESS:
            assert uc.reg_read(UC_X86_REG_ECX)==MANAGER and depth==0
            processed+=1;assert processed<=cycles
            if processed==cycles:uc.mem_write(MANAGER+0x149,b'\1')
            trace.append(('process',processed))
        elif a==API:
            pointer=get(sp+4);assert pointer in blocks and depth==0
            blocks.remove(pointer)
            if free_result:retained.add(pointer)
            else:uc.mem_write(pointer,b'\xee'*20)
            trace.append(('global_free',pointer));pop=4;result=free_result
        elif a==0xbfe9bc:
            pointer=get(sp+4);assert pointer in objects and depth==0
            index=objects.pop(pointer)
            assert get(pointer+0x20) not in blocks
            uc.mem_write(pointer,b'\xdd'*64);trace.append(('delete',index))
        else:
            assert get(sp+4)==MANAGER+0x14c
            if a==API+16:
                assert depth==0 and not objects and not blocks;depth=1
            else:assert depth==1 and uc.mem_read(MANAGER+0x14a,1)==b'\1';depth=0
            trace.append(('enter' if depth else 'leave',));pop=4
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook)
    sp=STACK+0x8000;put(sp,STOP)
    if bootstrap is not None:put(sp+4,MANAGER)
    saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
    u.reg_write(UC_X86_REG_ECX,MANAGER);u.reg_write(UC_X86_REG_ESP,sp)
    u.emu_start(BEGIN if bootstrap is not None else ENTRY,STOP,count=5000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
    assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
    assert processed==cycles and not objects and not blocks and depth==0
    assert [event[1] for event in trace if event[0]=='delete']==[i for i in range(count) if present=='all' or present=='even' and i%2==0]
    state=bytes(u.mem_read(0x2000000,0x4000));assert state==bytes(expected)
    return state,tuple(trace),tuple(sorted(retained))


def compare_exit(source=SOURCE,bootstrap_source=None):
    source=source.resolve();inputs={Path(__file__),source,EXE,R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'}
    visited=set()
    def closure(path):
        if path in visited:return
        visited.add(path);inputs.add(path)
        import re
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q=next((q for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None)
            assert q,name
            closure(q)
    closure(source)
    if bootstrap_source is not None:closure(bootstrap_source)
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='threaded-worker-exit-') as tmp:
        roots=[dict(source=source,symbol=SYMBOL,address=ENTRY)];addresses=dict(ADDRESS)
        if bootstrap_source is not None:
            roots.append(dict(source=bootstrap_source,symbol='?BeginThread@CThreadedFileManager@@CAXPAX@Z',address=BEGIN))
            addresses.update({'__imp__GetCurrentThread@0':0x143fdcc,'__imp__SetThreadPriority@8':0x143fdd0,SYMBOL:ENTRY})
        built_codes=compile_codes(roots,addresses,Path(tmp));built=built_codes[ENTRY]
    native=read_virtual_data(EXE.read_bytes(),ENTRY,176);assert built==native
    if bootstrap_source is not None:
        bootstrap=built_codes[BEGIN];original_bootstrap=read_virtual_data(EXE.read_bytes(),BEGIN,24)
        assert bootstrap==original_bootstrap
    cases=0
    for args in itertools.product((0,1,3,8),('balanced','left','right'),('none','even','all'),('none','odd','all'),(0,1,3,8),(False,True),(0,0x87654321)):
        assert execute(built,args)==execute(native,args),args;cases+=1
        if bootstrap_source is not None:
            assert execute(built,args,bootstrap)==execute(native,args,original_bootstrap),args;cases+=1
    missing=False
    try:execute(b'\xcc',(1,'balanced','all','all',1,False,0))
    except Exception:missing=True
    assert missing
    if bootstrap_source is not None:
        missing_bootstrap=False
        try:execute(built,(1,'balanced','all','all',1,False,0),b'\xcc')
        except Exception:missing_bootstrap=True
        assert missing_bootstrap
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
    return dict(status='PASS',cases=cases,missing_provider_detected=missing,
                missing_provider_checks=2 if bootstrap_source is not None else 1,input_sha256=pins,
                boundaries=['ProcessObjects, GlobalFree, delete and Win32 locks controlled; no actual worker scheduling. Failed GlobalFree retains the block while retail still deletes its object.',
                            'When BeginThread is present, GetCurrentThread/SetThreadPriority are controlled; cdecl caller retains its argument, real tail transfer reloads the receiver after volatile-register clobbering.',
                            'Valid ordered tree topologies supplied directly; nodes and UID map intentionally remain for later destruction.',
                            'Actual main loop, inlined iterator walk, conditional releases and locked finished publication execute.'])


if __name__=='__main__':
    import json,sys
    print(json.dumps(compare_exit(Path(sys.argv[1]) if len(sys.argv)>1 else SOURCE),indent=2))

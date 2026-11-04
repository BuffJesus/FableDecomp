"""Original/rebuilt/model proof of manager shutdown with controlled Win32 waits."""
import hashlib
import itertools
import struct
import tempfile
from pathlib import Path
import pefile
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R=Path(__file__).resolve().parents[2]
ENTRY,MANAGER,STACK,STOP,API=0x98e0d0,0x2000100,0x3000000,0x4000000,0x5000000
SYMBOL='?Uninitialise@CThreadedFileManager@@QAEXXZ'
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileManager_Uninitialise_0098e0d0.cpp'
SLOTS={'EnterCriticalSection':0x143fe0c,'LeaveCriticalSection':0x143fe10,
       'SetEvent':0x143fde0,'Sleep':0x143fe1c,'CloseHandle':0x143fe30,'DeleteCriticalSection':0x143fde4}
ADDRESS={'__imp__%s@4'%name:slot for name,slot in SLOTS.items()}


def execute(code,args):
    initialised,finished,event,delay,event_result,close_result,seed=args
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,size in ((0x98e000,0x1000),(0x143f000,0x1000),(MANAGER-0x100,0x1000),
                   (STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,size)
    u.mem_write(ENTRY,code)
    initial=bytearray((i*17+seed*31)&255 for i in range(0x300))
    initial[0x148+0x100]=int(initialised);initial[0x149+0x100]=0
    initial[0x14a+0x100]=int(finished)
    struct.pack_into('<I',initial,0x164+0x100,event)
    u.mem_write(MANAGER-0x100,bytes(initial))
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    callbacks={API+i*16:name for i,name in enumerate(SLOTS)}
    for a,name in callbacks.items():put(SLOTS[name],a);u.mem_write(a,b'\xc3')
    trace=[];depth=0;sleeps=0;deleted=False
    def hook(uc,a,size,user):
        nonlocal depth,sleeps,deleted
        if a not in callbacks:return
        name=callbacks[a];sp=uc.reg_read(UC_X86_REG_ESP);arg=get(sp+4);result=1
        if name=='EnterCriticalSection':
            assert arg==MANAGER+0x14c and depth==0 and not deleted
            assert uc.mem_read(MANAGER+0x148,1)==b'\0'
            depth=1
        elif name=='LeaveCriticalSection':assert arg==MANAGER+0x14c and depth==1;depth=0
        elif name=='SetEvent':
            assert arg==event and depth==1 and uc.mem_read(MANAGER+0x149,1)==b'\1'
            if not delay:uc.mem_write(MANAGER+0x14a,b'\1')
            result=event_result
        elif name=='Sleep':
            assert arg==0 and depth==0 and not deleted
            sleeps+=1
            assert sleeps<=delay
            if sleeps==delay:uc.mem_write(MANAGER+0x14a,b'\1')
        elif name=='CloseHandle':
            assert arg==event and event and depth==1
            assert uc.mem_read(MANAGER+0x14a,1)==b'\1';result=close_result
        else:
            assert name=='DeleteCriticalSection' and arg==MANAGER+0x14c and depth==0
            assert get(MANAGER+0x164)==0;deleted=True
        trace.append((name,arg))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
    u.hook_add(UC_HOOK_CODE,hook)
    expected=bytearray(initial);events=[]
    if initialised:
        expected[0x248]=0;expected[0x249]=1;expected[0x24a]=1
        struct.pack_into('<I',expected,0x264,0)
        section=MANAGER+0x14c
        events=[('EnterCriticalSection',section),('SetEvent',event)]
        for _ in range(delay if not finished else 0):events.extend([('LeaveCriticalSection',section),('Sleep',0),('EnterCriticalSection',section)])
        if event:events.append(('CloseHandle',event))
        events.extend([('LeaveCriticalSection',section),('DeleteCriticalSection',section)])
    snapshots=[]
    for repeat in range(2):
        sp=STACK+0x8000;put(sp,STOP)
        saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
        for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
        u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,MANAGER)
        u.emu_start(ENTRY,STOP,count=3000)
        assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
        assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
        state=bytes(u.mem_read(MANAGER-0x100,len(initial)))
        assert state==bytes(expected) and trace==events and depth==0
        snapshots.append((state,tuple(trace)))
    return snapshots


def compare_shutdown(source=SOURCE):
    source=source.resolve()
    root=dict(source=source,symbol=SYMBOL,address=ENTRY)
    inputs={Path(__file__),source,EXE,R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'}
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
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    raw=EXE.read_bytes();pe=pefile.PE(data=raw)
    imports={i.address:i.name.decode() for d in pe.DIRECTORY_ENTRY_IMPORT for i in d.imports if i.name}
    assert all(imports[slot]==name for name,slot in SLOTS.items())
    with tempfile.TemporaryDirectory(prefix='threaded-manager-shutdown-') as temp:
        candidate=compile_codes([root],ADDRESS,Path(temp))[ENTRY]
    native=read_virtual_data(raw,ENTRY,150)
    assert candidate==native
    cases=checkpoints=0
    for args in itertools.product((False,True),(False,True),(0,0x12345678),(0,1,2,8),(0,1),(0,1),range(3)):
        actual=execute(candidate,args);original=execute(native,args)
        assert actual==original,args;cases+=1;checkpoints+=len(actual)
    missing=False
    try:execute(b'\xcc',(True,False,0x12345678,1,1,1,0))
    except Exception:missing=True
    assert missing
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
    return dict(status='PASS',cases=cases,checkpoints=checkpoints,missing_provider_detected=missing,input_sha256=pins,
                boundaries=['Win32 lock/event/sleep/close/delete controlled; completion flag is changed at event/sleep boundaries.',
                            'Original/rebuilt/independent storage and call-order model agree; no actual worker thread or handle operations.'])


if __name__=='__main__':
    import sys,json
    report=compare_shutdown(Path(sys.argv[1]) if len(sys.argv)>1 else SOURCE)
    print(json.dumps(report,indent=2))

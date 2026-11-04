"""Manager wait with real UID lookup/priority and controlled worker progress."""
import hashlib
import itertools
import struct
import tempfile
from pathlib import Path
import gate_native_threaded_file_manager_remove as manager
from movable_pool_construct_oracle import compile_codes
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R=Path(__file__).resolve().parents[2]
ENTRY,OBJECT,HEADER,NODES,DATA,FILE,SLOT=0x98f1e0,0x2000100,0x2001000,0x2001100,0x2003000,0x2008000,0x2009000
STACK,STOP,API=0x3000000,0x4000000,0x5000000
SYMBOL='?WaitForObjectToFinish@CThreadedFileManager@@QAEXK@Z'
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileManager_WaitForObjectToFinish_0098f1e0.cpp'


def providers(source=SOURCE):
    roots=[dict(source=source,symbol=SYMBOL,address=ENTRY)]
    roots.extend(r for r in manager.ROOTS if r['name'] in ('uid_find','set_object_priority'))
    roots.extend(r for r in manager.NEAR_ROOTS if r['name']=='set_object_priority')
    addresses=dict(manager.ADDRESS)
    addresses.update(manager.NEAR_ADDRESS)
    addresses.update({'?SetObjectPriority@CThreadedFileManager@@QAEXKW4EThreadedFilePriority@@@Z':0x98eff0,
                      '__imp__SetEvent@4':0x143fde0,'__imp__SleepEx@8':0x143fddc})
    return roots,addresses


def execute(codes,args):
    status,present,priority,mode,delay,device,uid,shape=args
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,size in ((0x98e000,0x2000),(0x990000,0x2000),(0x143f000,0x1000),(0x2000000,0xa000),
                   (STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,size)
    u.mem_write(0x98e000,b'\xcc'*0x2000);u.mem_write(0x990000,b'\xcc'*0x2000)
    for a,b in codes.items():u.mem_write(a,b)
    for a in (0x98e770,0x991350):u.mem_write(a,b'\xc3')
    u.mem_write(0x2000000,b'\xa5'*0xa000)
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    for i,slot in enumerate((0x143fe0c,0x143fe10,0x143fde0,0x143fddc)):
        put(slot,API+i*16);u.mem_write(API+i*16,b'\xc3')
    put(OBJECT+0x13c,HEADER);put(OBJECT+0x164,0x12345678);put(FILE+4,device)
    put(DATA+0x14,priority);put(DATA+0x18,uid);put(DATA+0x1c,status);put(DATA+0x28,FILE)
    keys=[0x10,0x20,0x30]+([uid] if present else [])
    keys=sorted(set(keys));put(OBJECT+0x140,len(keys))
    node_for={key:NODES+i*24 for i,key in enumerate(keys)}
    for key,node in node_for.items():put(node+16,key);put(node+20,DATA if key==uid else DATA+0x100+keys.index(key)*0x100)
    def build(selected,parent):
        if not selected:return 0
        at=0 if shape=='right' else len(selected)-1 if shape=='left' else len(selected)//2
        key=selected[at];node=node_for[key];put(node,1);put(node+4,parent)
        put(node+8,build(selected[:at],node));put(node+12,build(selected[at+1:],node));return node
    def reset(selected):
        put(HEADER,0);put(HEADER+4,build(selected,HEADER))
        put(HEADER+8,node_for[selected[0]] if selected else HEADER);put(HEADER+12,node_for[selected[-1]] if selected else HEADER)
        put(OBJECT+0x140,len(selected))
    reset(keys);initial=bytes(u.mem_read(0x2000000,0xa000));trace=[];depth=0;sleeps=0;seen=set()
    moved=bool(present and status in (0,1) and priority!=6)
    def hook(uc,a,size,user):
        nonlocal depth,sleeps
        seen.add(a)
        if a not in (API,API+16,API+32,API+48,0x98e770,0x991350):return
        sp=uc.reg_read(UC_X86_REG_ESP);arg=get(sp+4);result=1;pop=4
        if a in (API,API+16):
            assert arg==OBJECT+0x14c
            depth+=1 if a==API else -1;assert depth in (0,1,2)
            trace.append(('enter' if a==API else 'leave',depth))
        elif a==0x98e770:
            assert moved and uc.reg_read(UC_X86_REG_ECX)==OBJECT and arg==DATA and depth==2
            assert get(DATA+0x14)==priority;trace.append(('remove_pending',priority))
        elif a==0x991350:
            assert moved and uc.reg_read(UC_X86_REG_ECX)==OBJECT+4+(device*7+6)*16
            assert get(arg)==DATA and get(DATA+0x14)==6 and depth==2
            result=SLOT;trace.append(('insert_pending',device,6))
        elif a==API+32:
            assert arg==0x12345678 and depth==0
            trace.append(('event',))
        else:
            assert arg==0 and get(sp+8)==1 and depth==0
            sleeps+=1;assert sleeps<=delay
            trace.append(('sleep_ex',sleeps));pop=8;result=0xc0 if sleeps%2 else 0
            if sleeps==delay:
                if mode=='finish':put(DATA+0x1c,4)
                else:reset([key for key in keys if key!=uid])
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook)
    sp=STACK+0x8000;put(sp,STOP);put(sp+4,uid)
    saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
    u.reg_write(UC_X86_REG_ECX,OBJECT);u.reg_write(UC_X86_REG_ESP,sp)
    u.emu_start(ENTRY,STOP,count=20000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8
    assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved)) and depth==0
    active=present and status!=4
    assert sleeps==(delay if active else 0)
    expected=bytearray(initial)
    if moved:
        struct.pack_into('<I',expected,DATA+0x14-0x2000000,6)
        struct.pack_into('<I',expected,SLOT-0x2000000,DATA)
    if active:
        if mode=='finish':struct.pack_into('<I',expected,DATA+0x1c-0x2000000,4)
        else:
            # The scheduling boundary replaces the active UID topology. Its
            # mutations are independently bounded to map metadata and links.
            for a in [OBJECT+0x140,HEADER,HEADER+4,HEADER+8,HEADER+12]+[node+offset for node in node_for.values() for offset in (0,4,8,12)]:
                expected[a-0x2000000:a-0x2000000+4]=u.mem_read(a,4)
            assert get(OBJECT+0x140)==len(keys)-1
    events=[('enter',1)]
    if active:
        events.append(('enter',2))
        if moved:events.extend([('remove_pending',priority),('insert_pending',device,6)])
        events.append(('leave',1))
        for n in range(1,delay+1):events.extend([('leave',0),('event',),('sleep_ex',n),('enter',1)])
    events.append(('leave',0));assert trace==events
    state=bytes(u.mem_read(0x2000000,0xa000));assert state==bytes(expected)
    return (state,tuple(trace)),seen


def compare_wait(source=SOURCE):
    source=source.resolve();roots,addresses=providers(source)
    inputs={Path(__file__),EXE,R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'};visited=set()
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
    with tempfile.TemporaryDirectory(prefix='threaded-manager-wait-') as tmp:
        codes=compile_codes(roots,addresses,Path(tmp));coff=Coff(Path(tmp)/'provider0.obj')
        sec,_=coff.symbols[SYMBOL];relocations=coff.sections[sec][1]
    raw=EXE.read_bytes();native={a:read_virtual_data(raw,a,181 if a==ENTRY else len(b)) for a,b in codes.items()}
    # The priority root has its own accepted near gate; use that root's declared
    # original size rather than inferring extent from candidate layout.
    priority=next(r for r in roots if r['address']==0x98eff0)
    native[0x98eff0]=read_virtual_data(raw,0x98eff0,priority['size'])
    grade=compare(codes[ENTRY],native[ENTRY],tuple(relocations),retail_relocations('0098f1e0',181)).to_dict();assert grade['near_match']
    cases=0;witnesses={}
    for args in itertools.product(range(5),(False,True),(1,6),('finish','remove'),(1,3),(0,1),(0,1,0xffffffff),('balanced','left','right')):
        try:
            actual,seen=execute(codes,args);original,_=execute(native,args);assert actual==original
        except Exception as error:raise AssertionError(args) from error
        cases+=1
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
    for root in roots:
        assert root['address'] in witnesses
        reduced={a:b for a,b in codes.items() if a!=root['address']};detected=False
        try:execute(reduced,witnesses[root['address']])
        except Exception:detected=True
        assert detected,root['symbol']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
    return dict(status='PASS',cases=cases,providers=len(roots),missing_provider_checks=len(roots),grade=grade,input_sha256=pins,
                boundaries=['Actual UID lookup and priority selection execute; pending removal/insertion, Win32 and worker completion/removal controlled.',
                            'Wait drops outer lock during event/SleepEx and reacquires before lookup. No actual concurrent worker or IO.'])


if __name__=='__main__':
    import json,sys
    print(json.dumps(compare_wait(Path(sys.argv[1]) if len(sys.argv)>1 else SOURCE),indent=2))

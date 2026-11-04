"""Original/rebuilt manager construction and genuine empty-owner teardown."""
import hashlib
import itertools
import struct
import tempfile
from pathlib import Path
import threaded_manager_destroy_oracle as destroy
from movable_pool_construct_oracle import compile_codes
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UC_HOOK_MEM_INVALID
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_FPCW

R=destroy.R
ENTRY,OBJECT,STACK,STOP,API=0x98f3a0,destroy.OBJECT,destroy.STACK,destroy.STOP,destroy.API
SYMBOL='??0CThreadedFileManager@@QAE@XZ'
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileManager_Constructor_0098f3a0.cpp'


def providers(source=SOURCE):
    roots,addresses=destroy.providers()
    roots=[r for r in roots if r['address'] not in (0x98fd40,0x98fd80)]
    roots.append(dict(source=source,symbol=SYMBOL,address=ENTRY))
    roots.append(dict(source=R/'rebuild/src/compiled/00/99/CBaseClass_CBaseClass_0099a2d0.cpp',
                      symbol='??0CBaseClassNonCopyable@@QAE@XZ',address=0x99a2f0))
    addresses.update({'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CBaseClassNonCopyable@@6B@':0x129a7c4,
                      '_malloc':0xbfea0e,'__imp__InitializeCriticalSection@4':0x143fee4,
                      '__imp__CreateEventW@16':0x143fdc8,'__real@0000000000000000':0x122ed70,
                      '?BeginThread@CThreadedFileManager@@CAXPAX@Z':0x98f380,'__beginthread':0xbfed3a})
    return roots,addresses


def execute(codes,args):
    seed,alignment,event,thread_result,delay,fail_at=args
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,size in ((0x98e000,0x2000),(0x99a000,0x1000),(0xbfe000,0x1000),(0x143f000,0x1000),
                   (0x122e000,0x1000),(0x2000000,0x4000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,size)
    u.mem_write(0x98e000,b'\xcc'*0x2000);u.mem_write(0x99a000,b'\xcc'*0x1000)
    for a,b in codes.items():u.mem_write(a,b)
    for a in (0xbfea0e,0xbfea14,0xbfed3a):u.mem_write(a,b'\xc3')
    u.mem_write(0x122ed70,struct.pack('<d',0.0))
    initial=bytes((i*13+seed*31)&255 for i in range(0x4000));u.mem_write(0x2000000,initial)
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    names=('InitializeCriticalSection','CreateEventW','EnterCriticalSection','LeaveCriticalSection','SetEvent','Sleep','CloseHandle','DeleteCriticalSection')
    slots=(0x143fee4,0x143fdc8,0x143fe0c,0x143fe10,0x143fde0,0x143fe1c,0x143fe30,0x143fde4)
    hooks={API+i*16:name for i,name in enumerate(names)}
    for (a,name),slot in zip(hooks.items(),slots):put(slot,a);u.mem_write(a,b'\xc3')
    trace=[];blocks={};allocations=0;sleeps=0;depth=0;seen=set();faults=[]
    critical=bytes((i*17+seed)&255 for i in range(24));deleted=False
    def hook(uc,a,size,user):
        nonlocal allocations,sleeps,depth,deleted
        seen.add(a)
        if a not in hooks and a not in (0xbfea0e,0xbfea14,0xbfed3a):return
        sp=uc.reg_read(UC_X86_REG_ESP);arg=get(sp+4);pop=4;result=1
        if a==0xbfea0e:
            allocations+=1;assert arg==24 and allocations<=15
            result=0 if allocations==fail_at else 0x2001000+(allocations-1)*0x100
            if result:blocks[result]=24
            trace.append(('malloc',result));pop=0
        elif a==0xbfea14:
            assert deleted and depth==0 and arg in blocks
            uc.mem_write(arg,b'\xdd'*blocks.pop(arg));trace.append(('free',arg));pop=0
        elif a==0xbfed3a:
            assert arg==0x98f380 and get(sp+8)==0x3000 and get(sp+12)==OBJECT
            assert allocations==15 and get(OBJECT)==0x129a164
            assert uc.mem_read(OBJECT+0x148,3)==b'\1\0\0' and get(OBJECT+0x168)==1
            trace.append(('beginthread',arg,0x3000,OBJECT));pop=0;result=thread_result
        else:
            name=hooks[a]
            if name=='InitializeCriticalSection':
                assert arg==OBJECT+0x14c and allocations==15
                assert get(OBJECT)==0x129a164;uc.mem_write(arg,critical)
            elif name=='CreateEventW':
                assert all(get(sp+4+i*4)==0 for i in range(4));pop=16;result=event
            elif name=='EnterCriticalSection':assert arg==OBJECT+0x14c and depth==0;depth=1
            elif name=='LeaveCriticalSection':assert arg==OBJECT+0x14c and depth==1;depth=0
            elif name=='SetEvent':
                assert depth==1 and arg==event
                if not delay:uc.mem_write(OBJECT+0x14a,b'\1')
            elif name=='Sleep':
                assert arg==0 and depth==0;sleeps+=1;assert sleeps<=delay
                if sleeps==delay:uc.mem_write(OBJECT+0x14a,b'\1')
            elif name=='CloseHandle':assert arg==event and event and depth==1
            else:assert name=='DeleteCriticalSection' and depth==0 and arg==OBJECT+0x14c;deleted=True
            trace.append((name,arg))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook)
    def invalid(uc,access,address,size,value,user):
        faults.append((address,size));return False
    u.hook_add(UC_HOOK_MEM_INVALID,invalid)
    def call(entry):
        sp=STACK+0x8000+alignment;put(sp,STOP)
        saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
        for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
        u.reg_write(UC_X86_REG_ECX,OBJECT);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_FPCW,0x37f)
        u.emu_start(entry,STOP,count=10000)
        assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
        assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
    try:call(ENTRY)
    except UcError:
        assert fail_at and allocations==fail_at and faults==[(0,1)]
        assert len(blocks)==fail_at-1
        return (bytes(u.mem_read(0x2000000,0x4000)),tuple(trace),tuple(sorted(blocks)),tuple(faults)),seen
    assert not fail_at and u.reg_read(UC_X86_REG_EAX)==OBJECT and allocations==15
    expected=bytearray(initial)
    def expect(a,v):struct.pack_into('<I',expected,a-0x2000000,v)
    expect(OBJECT,0x129a164)
    for index in range(15):
        head=0x2001000+index*0x100;at=OBJECT+4+index*16 if index<14 else OBJECT+0x13c
        expect(at,head);expect(at+4,0)
        if index<14:expect(at+12,head)
        expected[head-0x2000000]=0;expect(head+4,0);expect(head+8,head);expect(head+12,head)
    expected[OBJECT+0x148-0x2000000:OBJECT+0x14b-0x2000000]=b'\1\0\0'
    expected[OBJECT+0x14c-0x2000000:OBJECT+0x164-0x2000000]=critical
    for offset,value in ((0x164,event),(0x168,1),(0x16c,0),(0x138,0),(0x170,0x3e4ccccd),(0x174,0x3e4ccccd)):expect(OBJECT+offset,value)
    for index in range(7):
        expect(OBJECT+0xe4+index*4,0)
        expected[OBJECT+0x100+index*8-0x2000000:OBJECT+0x108+index*8-0x2000000]=b'\0'*8
    state=bytes(u.mem_read(0x2000000,0x4000));assert state==bytes(expected)
    snapshots=[(state,tuple(trace),tuple(sorted(blocks)))]
    call(destroy.ENTRY)
    assert not blocks and deleted and depth==0 and sleeps==delay
    for index in range(15):expected[0x1000+index*0x100:0x1000+index*0x100+24]=b'\xdd'*24
    expect(OBJECT,0x1231710);expect(OBJECT+0x164,0)
    expected[OBJECT+0x148-0x2000000:OBJECT+0x14b-0x2000000]=b'\0\1\1'
    state=bytes(u.mem_read(0x2000000,0x4000));assert state==bytes(expected)
    snapshots.append((state,tuple(trace),tuple(sorted(blocks))))
    return snapshots,seen


def compare_construct(source=SOURCE):
    source=source.resolve();roots,addresses=providers(source)
    inputs={Path(__file__),Path(destroy.__file__),EXE,R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'};visited=set()
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
    with tempfile.TemporaryDirectory(prefix='threaded-manager-construct-') as temp:
        codes=compile_codes(roots,addresses,Path(temp))
        sources=list(dict.fromkeys(r['source'] for r in roots));coff=Coff(Path(temp)/('provider%d.obj'%sources.index(source)))
        sec,_=coff.symbols[SYMBOL];relocations=coff.sections[sec][1]
    raw=EXE.read_bytes();native={a:read_virtual_data(raw,a,350 if a==ENTRY else 214 if a==destroy.ENTRY else len(b)) for a,b in codes.items()}
    grade=compare(codes[ENTRY],native[ENTRY],tuple(relocations),retail_relocations('0098f3a0',350)).to_dict();assert grade['near_match']
    cases=faults=0;witnesses={}
    matrix=list(itertools.product(range(8),(0,4,8,12),(0,0x12345678),(0,1,0xffffffff),(0,1,3),(0,)))
    matrix.extend((seed,align,0,0,0,fail) for seed,align,fail in itertools.product(range(2),(0,4,8,12),range(1,16)))
    for args in matrix:
        try:
            actual,seen=execute(codes,args);original,_=execute(native,args);assert actual==original
        except Exception as error:raise AssertionError(args) from error
        cases+=1;faults+=bool(args[-1])
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
    for root in roots:
        assert root['address'] in witnesses
        reduced={a:b for a,b in codes.items() if a!=root['address']};detected=False
        try:execute(reduced,witnesses[root['address']])
        except Exception:detected=True
        assert detected,root['symbol']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
    return dict(status='PASS',cases=cases,null_allocation_faults=faults,providers=len(roots),missing_provider_checks=len(roots),grade=grade,input_sha256=pins,
                boundaries=['CRT allocation, Win32 and _beginthread controlled; no actual worker starts. All 15 empty map owners constructed and destroyed by actual C++.',
                            'Null malloc cases reproduce original faults and retained prefix allocations; no invented recovery. Worker completion flag changes only at declared event/sleep boundaries.'])


if __name__=='__main__':
    import json,sys
    print(json.dumps(compare_construct(Path(sys.argv[1]) if len(sys.argv)>1 else SOURCE),indent=2))

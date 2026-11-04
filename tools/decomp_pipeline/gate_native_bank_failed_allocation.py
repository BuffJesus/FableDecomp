"""Behavior and near parity for bank selection/restart after allocation failure.

Tree insertion and pool FreeResource are controlled services. The entry point,
ordered-tree traversal, inlined FreeBuffer and critical-section wrappers execute
genuine compiled/retail instructions.
"""
import argparse
import hashlib
import itertools
import json
import struct
import tempfile
from pathlib import Path
import check_one as V
import gate_native_bank_async_data_teardown as lifetime
import gate_native_bank_async_data_buffer as buffer
import gate_native_bank_async_failure_handler as loading
from gate_game_component_abi import Coff
from movable_pool_construct_oracle import compile_codes
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ESP,UC_X86_REG_EIP

R=Path(__file__).resolve().parents[2]
BANK,DATA,HEADER,NODES=0x2000000,0x2002000,0x2003000,0x2004000
STACK,STOP,API=0x3000000,0x4000000,0x5000000
ENTRY=0x9d5bf0
CTOR,DTOR,FREE,INSERT=0xa62920,0xa629a0,0xa62ed0,0x9d7cf0
SYMBOL='?HandleFailedAllocation@CBankFileAsync@@QAE_NXZ'
HANDLER,BANKS,LOADING=0x200a000,0x200a100,0x9d5f10


def execute(codes,n,shape,finished,allowed,cursor_kind,buffer_kind,through_handler=0):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,length in ((0x9d0000,0x10000),(0xa62000,0x1000),(0x143f000,0x1000),
                            (BANK,0x10000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):
        u.mem_map(address,length)
        if address in (0x9d0000,0xa62000):u.mem_write(address,b'\xcc'*length)
    for address,data in codes.items():u.mem_write(address,data)
    def put(address,value):u.mem_write(address,struct.pack('<I',value&0xffffffff))
    def get(address):return struct.unpack('<I',u.mem_read(address,4))[0]
    u.mem_write(BANK,b'\xa5'*0x10000)
    for slot,target in ((0x143fe0c,API),(0x143fe10,API+16),(0x143fd30,API+32)):
        put(slot,target);u.mem_write(target,b'\xc3')
    for address in (FREE,INSERT):u.mem_write(address,b'\xc3')
    for index in range(n):
        data=DATA+index*0x48
        put(data+8,0x12340000+index)
        put(data+12,0x56780000+index)
        put(data+0x1c,0 if buffer_kind==1 else BANK+0x9000+index*0x20)
        put(data+0x20,0xffffffff if buffer_kind==2 else index+17)
        put(data+0x28,BANK)
        u.mem_write(data+0x44,bytes([(finished>>index)&1,0,(allowed>>index)&1,0]))
    def build(first,last,parent):
        if first==last:return 0
        at=(first+last)//2 if shape==0 else first if shape==1 else last-1
        node=NODES+at*20
        put(node,1);put(node+4,parent)
        put(node+8,build(first,at,node));put(node+12,build(at+1,last,node))
        put(node+16,DATA+at*0x48)
        return node
    put(HEADER,0)
    put(HEADER+4,build(0,n,HEADER))
    put(HEADER+8,NODES if n else HEADER)
    put(HEADER+12,NODES+(n-1)*20 if n else HEADER)
    put(BANK+0x128,HEADER);put(BANK+0x12c,n)
    put(BANK+0x134,BANK+0x3800);put(BANK+0x138,0)
    cursor=(0,DATA-1,DATA+(n//2)*0x48,DATA+(n//2)*0x48+1,DATA+n*0x48,0xffffffff)[cursor_kind]
    put(BANK+0x140,cursor)
    if through_handler:
        put(HANDLER+4,BANKS);put(HANDLER+8,BANKS+(4 if through_handler==1 else 0))
        put(HANDLER+12,BANKS+4);put(BANKS,BANK)
    start=next((i for i in range(n) if DATA+i*0x48>cursor),0)
    selected=next(((start+i)%n for i in range(n) if
        finished&(1<<((start+i)%n)) and allowed&(1<<((start+i)%n))),None) if n else None
    if through_handler==2:selected=None
    before=bytes(u.mem_read(BANK,0x10000))
    trace=[];held=[]
    def hook(uc,address,length,user):
        if address not in (API,API+16,API+32,FREE,INSERT):return
        sp=uc.reg_read(UC_X86_REG_ESP)
        if address in (API,API+16,API+32):
            section=get(sp+4)
            assert section in (BANK+0x14c,HANDLER+0x10)
            if address==API:
                assert section not in held
                if section==BANK+0x14c:assert held==([HANDLER+0x10] if through_handler else [])
                else:assert through_handler and not held
                held.append(section);trace.append(('enter' if section==BANK+0x14c else 'handler_enter',))
            elif address==API+16:
                assert held and held.pop()==section
                trace.append(('leave' if section==BANK+0x14c else 'handler_leave',))
            else:raise AssertionError('unexpected try-only lock')
            result,pop=1,4
        elif address==FREE:
            assert selected is not None and buffer_kind==0 and held[-1]==BANK+0x14c
            data=DATA+selected*0x48
            assert uc.reg_read(UC_X86_REG_ECX)==BANK+0x9000+selected*0x20
            assert get(sp+4)==selected+17
            assert u.mem_read(data+0x44,1)==b'\0'
            assert (get(data+8),get(data+12),get(data+0x20))==(0x12340000+selected,0x56780000+selected,selected+17)
            trace.append(('free',selected))
            result,pop=0,4
        else:
            assert selected is not None and held[-1]==BANK+0x14c
            assert uc.reg_read(UC_X86_REG_ECX)==BANK+0x134
            slot,value=get(sp+4),get(get(sp+8))
            assert value==DATA+selected*0x48
            assert u.mem_read(value+0x44,1)==b'\0'
            if buffer_kind==0:assert (get(value+8),get(value+12),get(value+0x20))==(0,0,0xffffffff)
            # Insertion is a controlled service, not an RB-tree reconstruction.
            put(BANK+0x138,1);put(slot,NODES+0x800);put(slot+4,1)
            trace.append(('insert',selected))
            result,pop=slot,8
        uc.reg_write(UC_X86_REG_EAX,result)
        uc.reg_write(UC_X86_REG_ECX,0xdeadbeef)
        uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp))
        uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook)
    sp=STACK+0x8000;put(sp,STOP)
    if through_handler:put(sp+4,0xabcdef01)
    saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
    u.reg_write(UC_X86_REG_EAX,0xdeadbeef)
    u.reg_write(UC_X86_REG_ECX,HANDLER if through_handler else BANK);u.reg_write(UC_X86_REG_ESP,sp)
    u.emu_start(LOADING if through_handler else ENTRY,STOP,count=20000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+(8 if through_handler else 4)
    assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
    assert not held
    assert u.reg_read(UC_X86_REG_EAX)&255==int(selected is not None)
    expected=bytearray(before)
    expected_trace=[('enter',)]
    if selected is not None:
        data=0x2000+selected*0x48
        expected[data+0x44]=0
        if buffer_kind==0:
            for offset,value in ((8,0),(12,0),(0x20,0xffffffff)):struct.pack_into('<I',expected,data+offset,value)
            expected_trace.append(('free',selected))
        struct.pack_into('<I',expected,0x138,1)
        struct.pack_into('<I',expected,0x140,DATA+selected*0x48)
        expected_trace.append(('insert',selected))
    expected_trace.append(('leave',))
    if through_handler==1:expected_trace=[('handler_enter',)]+expected_trace+[('handler_leave',)]
    if through_handler==2:expected_trace=[('handler_enter',),('handler_leave',)]
    actual=bytes(u.mem_read(BANK,0x10000))
    assert actual==bytes(expected)
    assert trace==expected_trace
    return actual,trace


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,default=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_Lifetime_009d5410.cpp')
    parser.add_argument('--output',type=Path,default=R/'rebuild/compile-gate/native-bank-failed-allocation.json')
    args=parser.parse_args()
    roots=[dict(source=args.source.resolve(),symbol=SYMBOL,address=ENTRY)]
    for name,symbol,address in (('Constructor','??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z',CTOR),
                                ('Destructor','??1CCriticalSectionLock@@QAE@XZ',DTOR)):
        roots.append(dict(source=R/'rebuild/src/compiled/00/a6'/('CCriticalSectionLock_%s_%08x.cpp'%(name,address)),symbol=symbol,address=address))
    roots.extend(r for r in loading.ROOTS if r['name']=='handler_failed_allocation')
    addresses=dict(lifetime.ADDRESS,**buffer.ADDRESS)
    addresses.update(loading.ADDRESS)
    addresses.update({'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__TryEnterCriticalSection@4':0x143fd30,'__imp__LeaveCriticalSection@4':0x143fe10})
    inputs={Path(__file__),Path(lifetime.__file__),Path(buffer.__file__),Path(loading.__file__),
            R/'tools/decomp_pipeline/movable_pool_construct_oracle.py',R/'tools/decomp_pipeline/near_match.py',EXE}
    def closure(path):
        if path in inputs:return
        inputs.add(path)
        import re
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q=next((q for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None)
            assert q,name
            closure(q)
    for root in roots:closure(root['source'])
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='bank-failed-allocation-') as tmp:
        candidate=compile_codes(roots,addresses,Path(tmp))
        coff=Coff(Path(tmp)/'provider0.obj')
        section,_=coff.symbols[SYMBOL];relocations=coff.sections[section][1]
    raw=EXE.read_bytes()
    native={a:read_virtual_data(raw,a,314 if a==ENTRY else len(data)) for a,data in candidate.items()}
    score=compare(candidate[ENTRY],native[ENTRY],tuple(relocations),retail_relocations('009d5bf0',314))
    assert score.near_match,score
    for address,data in candidate.items():
        if address!=ENTRY:assert data==native[address],hex(address)
    cases=0;witness=None
    masks=((0,255),(255,0),(255,255),(85,170),(85,255),(128,255),(255,85),(255,128))
    for n,shape,flags,cursor,kind in itertools.product((0,1,2,3,7,8),range(3),masks,range(6),range(3)):
        scenario=(n,shape,*flags,cursor,kind)
        try:assert execute(candidate,*scenario)==execute(native,*scenario)
        except Exception as error:raise AssertionError(scenario) from error
        if n==8 and flags==(255,255) and kind==0:witness=scenario
        cases+=1
    for n,flags,cursor,kind in itertools.product((0,1,3,8),masks,range(6),range(3)):
        scenario=(n,0,*flags,cursor,kind,1)
        assert execute(candidate,*scenario)==execute(native,*scenario)
        cases+=1
    assert execute(candidate,0,0,0,0,0,0,2)==execute(native,0,0,0,0,0,0,2)
    cases+=1
    assert witness
    omissions=[]
    for root in roots:
        reduced={a:data for a,data in candidate.items() if a!=root['address']}
        failed=False
        try:execute(reduced,*witness,1)
        except Exception:failed=True
        assert failed,root['symbol']
        omissions.append(root['symbol'])
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    report=dict(status='PASS',grade='NEAR_MATCH',source=str(args.source.resolve()),cases=cases,
                built_bytes=len(candidate[ENTRY]),retail_bytes=314,similarity=score.to_dict(),
                missing_provider_checks=omissions,input_sha256=pins,
                boundaries=['Pool FreeResource and restart-tree insertion controlled; genuine critical-section wrappers with controlled Win32 calls.',
                            'Ordered-tree traversal tested; no tree-insertion, pool allocator or concurrent-thread claim.'])
    args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_BANK_FAILED_ALLOCATION_PASS cases=%d providers=4 grade=NEAR_MATCH similarity=%.6f'%(cases,score.opcode_similarity))


if __name__=='__main__':main()

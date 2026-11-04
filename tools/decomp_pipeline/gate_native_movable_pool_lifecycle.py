"""Compose real movable allocation/binding, base allocation, area splitting and free.

Single-threaded state or controlled recursive Win32 critical sections; fixed
preallocated handle table. No vector growth, failure-handler or defrag claim.
"""
import hashlib
import json
import struct
import tempfile
from pathlib import Path

import gate_native_pool_areas as areas
import gate_native_pool_critical as critical
import gate_native_pool_allocate as allocation
import gate_native_pool_allocation_policy as policy
import gate_native_movable_pool_allocate as movable
import gate_native_pool_access as access
import gate_native_pool_unlock as unlock
import gate_native_pool_defragment as defragment
import gate_native_pool_handle_growth as growth
import gate_pool_free as free
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE, read_virtual_data
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX,
    UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP,
    UC_X86_REG_ESP, UC_X86_REG_EIP)

R=Path(__file__).resolve().parents[2]
POOL, INFOS, OUT, BUFFER = 0x2000100, 0x2001000, 0x2003000, 0x2010000
STACK, STOP, API = 0x3000000, 0x4000000, 0x5000000
OWNER=0x2003100
DEBUG=R/'debug_build/FableWin.exe'
OWN_CANDIDATE=(0xa04000,0xa04100,0xa04200)
OWN_ORIGINAL=(0x3363260,0x319b0b0,0x3369910)


def ownership_wrappers():
    source=R/'rebuild/tests/integration/ScopedMemoryHandle_wrappers.cpp'
    return [dict(source=source,symbol=symbol,address=address) for symbol,address in zip(
        ('@ConstructScopedHandle@4','@ClearScopedHandle@4','@DestroyScopedHandle@4'),OWN_CANDIDATE)]


def original_ownership():
    raw=DEBUG.read_bytes()
    assert read_virtual_data(raw,0x1805fa2,5)==b'\xe9'+struct.pack('<i',0x319b0b0-0x1805fa2-5)
    assert read_virtual_data(raw,0x17e0a90,5)==b'\xe9'+struct.pack('<i',0x319b470-0x17e0a90-5)
    return {address:read_virtual_data(raw,address,size) for address,size in
            ((0x3363260,33),(0x319b0b0,55),(0x3369910,19),(0x1805fa2,5),(0x17e0a90,5))}


def providers():
    roots=[]
    addresses={}
    for module, names in (
        (areas, {'create_area','find_free_area','update_largest_free_area','handle_from_pointer'}),
        (critical, {'begin_critical','end_critical','resize_area'}),
        (allocation, {'internal_allocate_handle','internal_allocate'}),
        (policy, {'allocate','allocate_scoped'}),
        (movable, {'allocate_resource'}),
        (access, {'get_pointer','get_size'}),
        (unlock, {'unlock'}),
        (defragment, {'internal_get_resource'}),
        (growth, {'handle_growth'})):
        roots.extend(r for r in module.ROOTS if r['name'] in names)
        for name,address in module.ADDRESS.items():
            assert name not in addresses or addresses[name]==address,(name,address)
            addresses[name]=address
    addresses.update(free.EXTERNAL)
    roots.append(dict(source=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_InternalSetResource_00a028b0.cpp',
                      symbol='?InternalSetResource@CHandleBasedMemoryPool@@QAEXJPAX@Z',address=0xa028b0))
    roots.append(dict(source=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Lock_00a026f0.cpp',
                      symbol='?Lock@CHandleBasedMemoryPool@@QAEXJ@Z',address=0xa026f0))
    for spec in free.SPECS:
        address=int(spec['address'],16)
        roots.append(dict(source=R/'rebuild/src/compiled'/spec['address'][:2]/spec['address'][2:4]/spec['file'],
                          symbol=spec['symbol'],address=address))
        addresses[spec['symbol']]=address
    return roots,addresses


class Machine:
    def __init__(self,codes,capacity,multi,flags,fail_at,initial_handles=4):
        self.u=u=Uc(UC_ARCH_X86,UC_MODE_32)
        for address,length in ((0xa01000,0x4000),(0xa62000,0x1000),(0x143f000,0x1000),
                                (0xbfe000,0x1000),(0x2000000,0x40000),(STACK,0x10000),(STOP,0x1000),(API,0x1000),
                                (0x3363000,0x1000),(0x319b000,0x1000),(0x3369000,0x1000),
                                (0x1805000,0x1000),(0x17e0000,0x1000)):
            u.mem_map(address,length)
            if address in (0xa01000,0xa62000):u.mem_write(address,b'\xcc'*length)
        for address,data in codes.items():u.mem_write(address,data)
        self.trace=[]
        self.depth=0
        self.tries=0
        self.fail_at=fail_at
        self.multi=multi
        self.flags=flags
        self.capacity=capacity
        self.snapshots=0
        self.blocks={}
        self.next_block=0x2020000
        self.initial_handles=initial_handles
        u.mem_write(0x2000000,b'\xa5'*0x40000)
        u.mem_write(POOL,b'\0'*0x464)
        self.put(POOL+4,BUFFER)
        self.put(POOL+12,capacity)
        u.mem_write(POOL+0x24,bytes([multi]))
        if initial_handles:
            self.put(POOL+0x438,INFOS)
            self.put(POOL+0x43c,INFOS+initial_handles*4)
            self.put(POOL+0x440,INFOS+initial_handles*4)
            for i in range(initial_handles):self.put(INFOS+i*4,i+1 if i<initial_handles-1 else 0xffffffff)
        else:
            self.put(POOL+0x444,0xffffffff)
        for address in (0xbfea0e,0xbfea14):u.mem_write(address,b'\xc3')
        for slot,target in ((0x143fe0c,API),(0x143fe10,API+16),(0x143fd30,API+32)):
            self.put(slot,target)
            u.mem_write(target,b'\xc3')
        u.hook_add(UC_HOOK_CODE,self.hook)

    def put(self,address,value):self.u.mem_write(address,struct.pack('<I',value&0xffffffff))
    def get(self,address):return struct.unpack('<I',self.u.mem_read(address,4))[0]

    def hook(self,u,address,size,user):
        if address==0x17e0a90:
            # Debug Clear's typed Free(long) thunk routes into the corresponding
            # retail-layout Free provider on the original side of this test.
            # Preserve this, argument and original return address unchanged.
            u.reg_write(UC_X86_REG_EIP,0xa02b30)
            return
        if address in (0xbfea0e,0xbfea14):
            sp=u.reg_read(UC_X86_REG_ESP)
            arg=self.get(sp+4)
            if address==0xbfea0e:
                assert arg in (4,8,16,32)
                result=self.next_block
                self.next_block+=0x1000
                assert self.next_block<=0x2040000
                self.blocks[result]=arg
                self.trace.append(('malloc',arg,result))
            else:
                assert arg in self.blocks,('invalid CRT free',hex(arg))
                u.mem_write(arg,b'\xdd'*self.blocks[arg])
                del self.blocks[arg]
                result=0
                self.trace.append(('free',arg))
            u.reg_write(UC_X86_REG_EAX,result)
            u.reg_write(UC_X86_REG_ECX,0xdeadbeef)
            u.reg_write(UC_X86_REG_EDX,0x87654321)
            u.reg_write(UC_X86_REG_EIP,self.get(sp))
            u.reg_write(UC_X86_REG_ESP,sp+4)  # CRT cdecl; caller pops the argument.
            return
        if address not in (API,API+16,API+32):return
        sp=u.reg_read(UC_X86_REG_ESP)
        assert self.get(sp+4)==POOL+0x44c
        result=1
        if address==API+16:
            self.depth-=1
            assert self.depth>=0
            name='leave'
        elif address==API+32:
            self.tries+=1
            result=int(self.tries!=self.fail_at)
            self.depth+=result
            name='try'
        else:
            self.depth+=1
            name='enter'
        assert self.depth<=2
        self.trace.append((name,result,self.depth))
        u.reg_write(UC_X86_REG_EAX,result)
        u.reg_write(UC_X86_REG_ECX,0xdeadbeef)
        u.reg_write(UC_X86_REG_EDX,0x87654321)
        u.reg_write(UC_X86_REG_EIP,self.get(sp))
        u.reg_write(UC_X86_REG_ESP,sp+8)

    def call(self,address,args,this=POOL):
        u=self.u
        sp=STACK+0x8000
        self.put(sp,STOP)
        for i,arg in enumerate(args):self.put(sp+4+4*i,arg)
        saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
        for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
        u.reg_write(UC_X86_REG_ESP,sp)
        u.reg_write(UC_X86_REG_ECX,this)
        try:
            u.emu_start(address,STOP,count=20000)
        except Exception as error:
            raise AssertionError(('entry',hex(address),'eip',hex(u.reg_read(UC_X86_REG_EIP)))) from error
        assert u.reg_read(UC_X86_REG_EIP)==STOP,hex(address)
        assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args),hex(address)
        assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
        assert self.depth==0
        return u.reg_read(UC_X86_REG_EAX)

    def snapshot(self):
        self.snapshots+=1
        return bytes(self.u.mem_read(0x2000000,0x40000)),tuple(self.trace),tuple(sorted(self.blocks.items()))


def scenario(codes,capacity,sizes,multi,flags,fail_at,initial_handles=4):
    m=Machine(codes,capacity,multi,flags,fail_at,initial_handles)
    snapshots=[]
    assert m.call(0xa01d50,[BUFFER,capacity,0,0])==BUFFER
    m.call(0xa01da0,[BUFFER])
    snapshots.append(m.snapshot())
    live=[]
    def allocate(size,resource):
        m.put(OUT,0x87654321)
        before=m.snapshot()[0]
        result=m.call(0xa62e40,[OUT,resource,size,flags])&255
        assert result in (0,1)
        if result:
            handle=m.get(OUT)
            assert handle<4 and handle not in live
            header=m.get(m.get(POOL+0x438)+handle*4)
            assert BUFFER<=header<BUFFER+capacity
            assert m.get(header)==resource and m.get(header+4)==handle
            assert m.u.mem_read(header+14,2)==b'\x54\0'
            rounded=(size+15)//16*16
            assert m.get(header+8)==rounded
            unchanged=m.snapshot()[0]
            pointer=m.call(0xa026b0,[handle])
            assert pointer==header+32
            assert m.call(0xa026d0,[handle])==rounded
            assert m.call(0xa01d40,[pointer])==handle
            assert m.call(0xa028d0,[handle])==resource
            assert m.snapshot()[0]==unchanged
            snapshots.append(m.snapshot())
            for repeat in range(2):
                m.call(0xa026f0,[handle])
                assert m.u.mem_read(header+15,1)==b'\1' and m.get(POOL+0x18)==1
                snapshots.append(m.snapshot())
            for repeat in range(2):
                m.call(0xa02710,[handle])
                assert m.u.mem_read(header+15,1)==b'\0' and m.get(POOL+0x18)==0
                snapshots.append(m.snapshot())
            if size:
                m.u.mem_write(pointer,bytes([handle+17])*min(size,17))
            live.append(handle)
        else:
            assert m.get(OUT)==0x87654321
            # A failed wrapper must leave pool/areas/handles unchanged.
            assert m.snapshot()[0]==before
        snapshots.append((result,m.snapshot()))
        return m.get(OUT) if result else None
    first=allocate(sizes[0],0x22220000)
    allocate(sizes[1],0)
    if first is not None:
        m.call(0xa02b30,[first]);live.remove(first);snapshots.append(m.snapshot())
    again=allocate(sizes[0],0x33330000)
    if first is not None and again is not None:assert again==first
    for handle in list(reversed(live)):
        m.call(0xa02b30,[handle]);live.remove(handle);snapshots.append(m.snapshot())
    assert m.get(POOL+0x10)==0
    assert m.get(POOL+0x18)==0
    assert m.get(POOL+0x1c)==capacity-32
    assert m.get(BUFFER+8)==capacity-32
    assert m.u.mem_read(BUFFER+14,1)==b'\x4e'
    assert m.get(BUFFER+24)==0 and m.get(BUFFER+28)==0
    if not multi:assert all(event[0] in ('malloc','free') for event in m.trace)
    if initial_handles:
        assert not m.blocks
    elif m.get(POOL+0x438):
        assert m.blocks=={m.get(POOL+0x438):m.get(POOL+0x440)-m.get(POOL+0x438)}
    return snapshots


def scoped_scenario(codes,ownership,capacity,sizes,multi,flags,fail_at):
    m=Machine(codes,capacity,multi,flags,fail_at,0)
    snapshots=[]
    assert m.call(0xa01d50,[BUFFER,capacity,0,0])==BUFFER
    m.call(0xa01da0,[BUFFER])
    assert m.call(ownership[0],[],OWNER)==OWNER
    assert m.get(OWNER)==0 and m.get(OWNER+4)==0xffffffff
    snapshots.append(m.snapshot())
    previous=None
    for size in sizes:
        before=m.snapshot()[0]
        result=m.call(0xa032f0,[OWNER,size,flags])&255
        assert result in (0,1)
        if result:
            assert m.get(OWNER)==POOL
            handle=m.get(OWNER+4)
            assert handle<4 and handle!=previous
            table=m.get(POOL+0x438)
            header=m.get(table+handle*4)
            assert m.u.mem_read(header+14,1)==b'\x54'
            assert m.get(header+8)==(size+15)//16*16
            if previous is not None:assert m.get(POOL+0x444)==previous
            previous=handle
        else:
            assert m.snapshot()[0]==before
        snapshots.append((result,m.snapshot()))
    m.call(ownership[1],[],OWNER)
    assert m.get(OWNER)==0 and m.get(OWNER+4)==0xffffffff
    assert m.get(POOL+0x10)==0 and m.get(POOL+0x1c)==capacity-32
    snapshots.append(m.snapshot())
    unchanged=m.snapshot()
    m.call(ownership[1],[],OWNER)
    assert m.snapshot()==unchanged
    snapshots.append(m.snapshot())
    # Exercise destruction with live ownership as well as repeated empty Clear.
    result=m.call(0xa032f0,[OWNER,sizes[0],flags])&255
    assert result in (0,1)
    if result:assert m.get(OWNER)==POOL and m.get(POOL+0x10)>0
    snapshots.append((result,m.snapshot()))
    m.call(ownership[2],[],OWNER)
    assert m.get(OWNER)==0 and m.get(OWNER+4)==0xffffffff
    assert m.get(POOL+0x10)==0 and m.get(POOL+0x1c)==capacity-32
    snapshots.append(m.snapshot())
    return snapshots


def compare_handle_growth():
    """Bounded append-by-one growth through the genuine allocator caller."""
    roots,addresses=providers()
    inputs={Path(__file__),R/'tools/decomp_pipeline/movable_pool_construct_oracle.py',
            *(root['source'] for root in roots)}
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='pool-handle-growth-') as tmp:
        candidate=compile_codes(roots,addresses,Path(tmp))
    raw=EXE.read_bytes()
    native={address:read_virtual_data(raw,address,len(data)) for address,data in candidate.items()}
    cases=0
    for capacity in (128,512):
        for sizes in ((0,1),(16,17),(480,1),(2048,1)):
            assert scenario(candidate,capacity,sizes,False,0,0,0)==scenario(native,capacity,sizes,False,0,0,0)
            cases+=1
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    return dict(status='PASS',comparative_cases=cases,input_sha256=pins,
                boundary='Real append-by-one growth through InternalAllocateHandle; CRT malloc/free controlled, freed storage poisoned. General fill/range insertion excluded.')


def main():
    roots,addresses=providers()
    wrappers=ownership_wrappers()
    inputs={Path(__file__),R/'tools/decomp_pipeline/movable_pool_construct_oracle.py',EXE,DEBUG}
    for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py'):
        inputs.add(R/'tools/decomp_pipeline'/name)
    for module in (areas,critical,allocation,policy,movable,access,unlock,defragment,growth,free):inputs.add(Path(module.__file__))
    def closure(path):
        if path in inputs:return
        inputs.add(path)
        import re
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q=next((q for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None)
            assert q,name
            closure(q)
    for root in roots+wrappers:closure(root['source'])
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='movable-pool-lifecycle-') as tmp:
        candidate=compile_codes(roots+wrappers,addresses,Path(tmp))
    raw=EXE.read_bytes()
    native={root['address']:read_virtual_data(raw,root['address'],len(candidate[root['address']])) for root in roots}
    native.update(original_ownership())
    for address,data in ((root['address'],candidate[root['address']]) for root in roots):
        if address!=0xa02b30:assert data==native[address],('expected exact provider',hex(address))
    cases=checkpoints=0
    for capacity in (128,256,512,1024):
        for sizes in ((0,1),(15,16),(16,17),(31,32),(64,128),(480,1),(512,1024),(2048,1)):
            for multi in (False,True):
                for flags in (0,1,2,3):
                    for fail_at in ((0,1,2) if multi and flags&1 else (0,)):
                        for initial_handles in (0,4):
                            args=(capacity,sizes,multi,flags,fail_at,initial_handles)
                            try:
                                a=scenario(candidate,*args)
                                b=scenario(native,*args)
                                assert a==b
                            except Exception as error:
                                raise AssertionError(args) from error
                            cases+=1
                            checkpoints+=len(a)
                        args=(capacity,sizes,multi,flags,fail_at)
                        try:
                            a=scoped_scenario(candidate,OWN_CANDIDATE,*args)
                            b=scoped_scenario(native,OWN_ORIGINAL,*args)
                            assert a==b
                        except Exception as error:
                            raise AssertionError(('scoped',args)) from error
                        cases+=1
                        checkpoints+=len(a)
    omissions=[]
    for root in roots:
        reduced={a:data for a,data in candidate.items() if a!=root['address']}
        detected=False
        for args in ((256,(16,17),False,0,0,0),(128,(0,1),True,1,0,4),(128,(480,1),True,3,0,0)):
            try:
                scenario(reduced,*args)
            except Exception:
                detected=True
                break
        if not detected:
            try:scoped_scenario(reduced,OWN_CANDIDATE,256,(16,17),False,0,0)
            except Exception:detected=True
        assert detected,('omitted provider went undetected',root['symbol'])
        omissions.append(root['symbol'])
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    report=dict(status='PASS',cases=cases,checkpoints=checkpoints,providers=len(roots),missing_provider_checks=omissions,input_sha256=pins,
                boundaries=['Controlled recursive Win32 critical sections and CRT malloc/free; no concurrent threads.',
                            'Debug owner lifecycle calls actual debug Clear; its Free(long) thunk routes to the retail-layout Free provider.',
                            'Ownership fixture wrappers are not retail roots. Failure-handler and defragmentation excluded.'])
    (R/'rebuild/compile-gate/native-movable-pool-lifecycle.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_MOVABLE_POOL_LIFECYCLE_PASS cases=%d checkpoints=%d providers=%d omissions=%d'%(cases,checkpoints,len(roots),len(omissions)))


if __name__=='__main__':main()

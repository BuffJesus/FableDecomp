"""Run real partial defragmentation over allocated, fragmented movable pools."""
import hashlib
import itertools
import json
import tempfile
from pathlib import Path
import gate_native_pool_relocation as relocation
import gate_native_pool_allocation_policy as policy
import gate_native_pool_allocate as allocation
import gate_native_pool_areas as areas
import gate_native_pool_critical as critical
import gate_native_pool_stuck_queue as queue
from gate_native_movable_pool_lifecycle import providers as base_providers
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R,POOL,BUFFER,API=relocation.R,relocation.POOL,relocation.BUFFER,relocation.API
CAN=API+0x110


def providers():
    roots,addresses=base_providers()
    roots=[r for r in roots if r['address']!=0xa032f0]
    for module,names in ((policy,{'partial_defragment'}),(allocation,{'grow_free_area'}),
                         (areas,{'find_largest_free_area'}),
                         (critical,{'test_if_area_is_growable','combine_adjacent_areas'}),
                         (relocation.defragment,{'move_area'})):
        roots.extend(r for r in module.ROOTS if r['name'] in names)
        addresses.update(module.ADDRESS)
    roots.extend(queue.ROOTS)
    addresses.update(queue.ADDRESS)
    for address in ('00a62f30','00a62f70'):
        spec=next(s for s in relocation.movable.SPECS if s['address']==address)
        roots.append(dict(source=R/'rebuild/src/compiled/00/a6'/spec['file'],symbol=spec['symbol'],address=int(address,16)))
    addresses.update(relocation.movable.EXTERNAL)
    return roots,addresses


class PartialMachine(relocation.RelocationMachine):
    def __init__(self,codes,capacity,multi):
        super().__init__(codes,capacity,multi)
        self.invalid_during_move=False
        self.allowed=set()
        self.queries=[]
        self.executed=set()
        self.revoke_after_move=False
        self.put(0x129d6a8,0xa62f70)
        self.put(relocation.RESOURCE_VT+8,CAN)
        self.u.mem_write(CAN,b'\xc3')

    def hook(self,u,address,size,user):
        self.executed.add(address)
        if address==CAN:
            sp=u.reg_read(UC_X86_REG_ESP)
            handle=self.get(sp+4)
            assert u.reg_read(UC_X86_REG_ECX)==relocation.RESOURCE
            result=int(handle in self.allowed and not (self.revoke_after_move and self.moved))
            self.queries.append((handle,result))
            self.trace.append(('can_relocate',handle,result))
            u.reg_write(UC_X86_REG_EAX,0xfedcba00|result)
            u.reg_write(UC_X86_REG_ECX,0xdeadbeef)
            u.reg_write(UC_X86_REG_EDX,0x87654321)
            u.reg_write(UC_X86_REG_EIP,self.get(sp))
            u.reg_write(UC_X86_REG_ESP,sp+8)
            return
        if address==relocation.MEMMOVE:
            sp=u.reg_read(UC_X86_REG_ESP)
            to,source,length=(self.get(sp+4*i) for i in (1,2,3))
            if not (BUFFER<=source<=BUFFER+self.capacity-length and BUFFER<=to<=BUFFER+self.capacity-length):
                assert 0x2000000<=source<=0x2040000-length
                assert 0x2000000<=to<=0x2040000-length
                if length:u.mem_write(to,bytes(u.mem_read(source,length)))
                self.trace.append(('vector_memmove',to,source,length))
                u.reg_write(UC_X86_REG_EAX,to)
                u.reg_write(UC_X86_REG_ECX,0xdeadbeef)
                u.reg_write(UC_X86_REG_EDX,0x87654321)
                u.reg_write(UC_X86_REG_EIP,self.get(sp))
                u.reg_write(UC_X86_REG_ESP,sp+4)
                return
        return super().hook(u,address,size,user)


def scenario(codes,capacity,freed,requested,lock_odd,permission,limit,multi):
    m=PartialMachine(codes,capacity,multi)
    m.put(POOL+0x30,limit)
    size=capacity//8-32
    assert m.call(0xa01d50,[BUFFER,capacity,0,0])==BUFFER
    m.call(0xa01da0,[BUFFER])
    snapshots=[]
    live=[]
    for index in range(8):
        assert m.call(0xa62e40,[0x2003000,relocation.RESOURCE,size,0])&255
        handle=m.get(0x2003000)
        pointer=m.call(0xa026b0,[handle])
        data=bytes((i*17+index*31)&255 for i in range(size))
        m.u.mem_write(pointer,data)
        m.expected[handle]=data
        live.append(handle)
        snapshots.append(m.snapshot())
    for index in freed:
        m.call(0xa02b30,[index]);live.remove(index)
        snapshots.append(m.snapshot())
    m.allowed=set(live) if permission=='all' else {h for h in live if
        (permission=='even' and h%2==0) or (permission=='revoke' and h in (1,3))}
    m.revoke_after_move=permission=='revoke'
    locked=[h for h in live if lock_odd and h%2]
    for handle in locked:m.call(0xa026f0,[handle])
    before_largest=m.get(POOL+0x1c)
    result=m.call(0xa02df0,[requested])&255
    assert result in (0,1)
    assert not result or m.get(POOL+0x1c)>=requested
    if before_largest>=requested:assert result
    assert not set(m.moved)&set(locked)
    assert set(m.moved)<=m.allowed
    snapshots.append((result,m.snapshot()))
    for handle in live:
        pointer=m.call(0xa026b0,[handle])
        assert m.call(0xa01d40,[pointer])==handle
        assert m.call(0xa026d0,[handle])==size
        assert m.call(0xa028d0,[handle])==relocation.RESOURCE
        assert bytes(m.u.mem_read(pointer,size))==m.expected[handle]
    for handle in locked:m.call(0xa02710,[handle])
    for handle in reversed(live):
        m.call(0xa02b30,[handle])
        snapshots.append(m.snapshot())
    assert m.get(POOL+0x10)==0 and m.get(POOL+0x18)==0
    assert m.get(POOL+0x1c)==capacity-32
    assert m.blocks=={m.get(POOL+0x438):m.get(POOL+0x440)-m.get(POOL+0x438)}
    return snapshots,m.executed,dict(result=result,moves=len(m.moved),permission_queries=len(m.queries))


def matrix():
    for capacity in (512,1024):
        for rest in itertools.product(((0,2,4,6),(0,7),(2,3,4)),
                                      (capacity//8-31,capacity//2,capacity+16),
                                      (False,True),('all','none','even','revoke'),(0,1,16),(False,True)):
            yield (capacity,)+rest


def compare_stuck_queue():
    """Exercise the actual stuck branch and independent append/alias growth."""
    roots,addresses=providers()
    inputs={Path(__file__),Path(relocation.__file__),*(r['source'] for r in roots)}
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='pool-stuck-queue-behavior-') as tmp:
        candidate=compile_codes(roots,addresses,Path(tmp))
    raw=EXE.read_bytes()
    native={a:read_virtual_data(raw,a,len(data)) for a,data in candidate.items()}
    cases=0
    for capacity,limit,multi in itertools.product((512,1024),(2,16),(False,True)):
        args=(capacity,(0,2,4,6),capacity//2,False,'revoke',limit,multi)
        a,seen,_=scenario(candidate,*args)
        b,_,_=scenario(native,*args)
        assert a==b
        assert {r['address'] for r in queue.ROOTS}<=seen,('stuck queue not exercised',args)
        cases+=1
    for count,alias in itertools.product((1,2,3,4,5,8),(False,True)):
        outcomes=[]
        for codes in (candidate,native):
            m=PartialMachine(codes,512,False)
            vector,value=0x2006000,0x2006100
            m.u.mem_write(vector,b'\0'*12)
            expected=[]
            history=[]
            for index in range(count):
                start=m.get(vector)
                if alias and start:
                    argument=start
                    expected.append(expected[0])
                else:
                    m.put(value,BUFFER+index*64)
                    argument=value
                    expected.append(BUFFER+index*64)
                m.call(0xa038c0,[argument],vector)
                start,finish,end=(m.get(vector+offset) for offset in (0,4,8))
                assert finish-start==4*len(expected)
                assert end-start==4*(1<<(len(expected)-1).bit_length())
                assert [m.get(start+4*i) for i in range(len(expected))]==expected
                assert m.blocks=={start:end-start}
                history.append(m.snapshot())
            outcomes.append(history)
        assert outcomes[0]==outcomes[1]
        cases+=1
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    return dict(status='PASS',comparative_cases=cases,input_sha256=pins,
                boundary='Eight actual stuck-branch cases plus twelve append/alias cases; CRT/Win32/resource callbacks controlled, freed storage poisoned.')


def main():
    roots,addresses=providers()
    inputs={Path(__file__),Path(relocation.__file__),Path(queue.__file__),
            R/'tools/decomp_pipeline/gate_native_movable_pool_lifecycle.py',
            R/'tools/decomp_pipeline/movable_pool_construct_oracle.py',EXE}
    for module in (policy,allocation,areas,critical,relocation.defragment,relocation.movable):inputs.add(Path(module.__file__))
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
    with tempfile.TemporaryDirectory(prefix='pool-partial-defragment-') as tmp:
        candidate=compile_codes(roots,addresses,Path(tmp))
    raw=EXE.read_bytes()
    native={a:read_virtual_data(raw,a,len(data)) for a,data in candidate.items()}
    for a,data in candidate.items():
        if a!=0xa02b30:assert data==native[a],hex(a)
    cases=checkpoints=successes=moves=queries=0
    executed=set()
    witnesses={}
    for args in matrix():
        try:
            actual,seen,stats=scenario(candidate,*args)
            original,_,original_stats=scenario(native,*args)
            assert actual==original and stats==original_stats
        except Exception as error:raise AssertionError(args) from error
        cases+=1;checkpoints+=len(actual);successes+=stats['result'];moves+=stats['moves'];queries+=stats['permission_queries']
        executed.update(seen)
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
        if cases%64==0:print('partial progress cases=%d'%cases,flush=True)
    omissions=[]
    unexercised=[]
    for root in roots:
        if root['address'] not in witnesses:
            unexercised.append(root['symbol']);continue
        reduced={a:data for a,data in candidate.items() if a!=root['address']}
        detected=False
        try:scenario(reduced,*witnesses[root['address']])
        except Exception:detected=True
        assert detected,('omission undetected',root['symbol'])
        omissions.append(root['symbol'])
    assert not unexercised,unexercised
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    report=dict(status='PASS',cases=cases,checkpoints=checkpoints,providers=len(roots),successes=successes,moves=moves,
                permission_queries=queries,missing_provider_checks=omissions,unexercised=unexercised,input_sha256=pins,
                boundaries=['Win32 locks, CRT allocation/memmove and resource callbacks controlled; no concurrency.',
                            'No failure-handler-driven allocation or full-engine startup claim.'])
    (R/'rebuild/compile-gate/native-pool-partial-defragment.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_POOL_PARTIAL_DEFRAGMENT_PASS cases=%d checkpoints=%d providers=%d omissions=%d unexercised=%d moves=%d'%(cases,checkpoints,len(roots),len(omissions),len(unexercised),moves))
    for name in unexercised:print('UNEXERCISED',name)


if __name__=='__main__':main()

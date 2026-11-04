"""Compose allocator failure policy, partial defrag and real reentrant pool Free."""
import hashlib
import itertools
import json
import tempfile
from pathlib import Path
import gate_native_pool_partial_defragment as partial
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R,POOL,BUFFER,API=partial.R,partial.POOL,partial.BUFFER,partial.API
STATE,VTABLE,CALLBACK=0x2007000,0x2007100,0xa04300


class FailureMachine(partial.PartialMachine):
    def __init__(self,codes,multi,queued,threshold):
        super().__init__(codes,512,multi)
        self.events=[]
        self.max_depth=0
        self.u.mem_write(STATE,b'\0'*56)
        self.put(STATE,VTABLE)
        self.put(STATE+4,POOL)
        for i,handle in enumerate(queued):self.put(STATE+8+4*i,handle)
        self.put(STATE+40,len(queued))
        self.put(VTABLE+4,CALLBACK)
        self.put(POOL+0x28,threshold)
        self.put(POOL+0x2c,threshold)
        self.put(POOL+0x30,16)

    def hook(self,u,address,size,user):
        if address==CALLBACK:
            assert u.reg_read(UC_X86_REG_ECX)==STATE
            arg=self.get(u.reg_read(UC_X86_REG_ESP)+4)
            self.events.append((arg,self.get(STATE+44),self.depth))
            self.trace.append(('failure',arg,self.get(STATE+44),self.depth))
        if address in (API,API+16,API+32):
            sp=u.reg_read(UC_X86_REG_ESP)
            assert self.get(sp+4)==POOL+0x44c
            result=1
            if address==API+16:
                self.depth-=1;assert self.depth>=0;name='leave'
            elif address==API+32:
                self.tries+=1;result=int(self.tries!=self.fail_at)
                self.depth+=result;name='try'
            else:self.depth+=1;name='enter'
            assert self.depth<=3
            self.max_depth=max(self.max_depth,self.depth)
            self.trace.append((name,result,self.depth))
            u.reg_write(UC_X86_REG_EAX,result)
            u.reg_write(UC_X86_REG_ECX,0xdeadbeef)
            u.reg_write(UC_X86_REG_EDX,0x87654321)
            u.reg_write(UC_X86_REG_EIP,self.get(sp))
            u.reg_write(UC_X86_REG_ESP,sp+8)
            return
        super().hook(u,address,size,user)


def scenario(codes,queued,requested,threshold,permission,flags,multi,mode='normal'):
    m=FailureMachine(codes,multi,queued,threshold)
    assert m.call(0xa01d50,[BUFFER,512,0,0])==BUFFER
    m.call(0xa01da0,[BUFFER])
    snapshots=[]
    for index in range(8):
        assert m.call(0xa62e40,[0x2003000,partial.relocation.RESOURCE,32,0])&255
        assert m.get(0x2003000)==index
        pointer=m.call(0xa026b0,[index])
        data=bytes((i*17+index*31)&255 for i in range(32))
        m.u.mem_write(pointer,data);m.expected[index]=data
        snapshots.append(m.snapshot())
    m.allowed=set(range(8)) if permission else set()
    m.put(POOL+0x34,STATE)
    if mode=='shutdown':m.u.mem_write(POOL+0x25,b'\1')
    if mode=='outer_refusal':m.fail_at=1;m.tries=0
    if mode=='inner_refusal':m.fail_at=2;m.tries=0
    m.put(0x2003000,0x87654321)
    result=m.call(0xa62e40,[0x2003000,0,requested,flags])&255
    assert result in (0,1)
    consumed=m.get(STATE+44)
    calls=m.get(STATE+48)
    assert consumed<=len(queued) and calls==len(m.events)
    assert all(event[0]==requested for event in m.events)
    if multi:assert all(event[2]==2 for event in m.events)
    if flags&2 or mode!='normal':assert not calls
    if mode!='normal' or flags&2:assert not result
    if calls:assert m.get(STATE+52)==requested
    live=set(range(8))-set(queued[:consumed])
    for handle in live:
        pointer=m.call(0xa026b0,[handle])
        assert m.call(0xa01d40,[pointer])==handle
        assert m.call(0xa028d0,[handle])==partial.relocation.RESOURCE
        assert bytes(m.u.mem_read(pointer,32))==m.expected[handle]
    if result:
        handle=m.get(0x2003000)
        assert handle not in live
        assert m.call(0xa026d0,[handle])==(requested+15)//16*16
        assert m.call(0xa028d0,[handle])==0
        live.add(handle)
    else:assert m.get(0x2003000)==0x87654321
    snapshots.append((result,m.snapshot()))
    for handle in sorted(live,reverse=True):
        m.call(0xa02b30,[handle]);snapshots.append(m.snapshot())
    assert m.get(POOL+0x10)==0 and m.get(POOL+0x18)==0
    assert m.get(POOL+0x1c)==480
    return snapshots,m.executed,dict(result=result,calls=calls,released=consumed,max_depth=m.max_depth)


def matrix():
    for args in itertools.product(((),(0,),(0,1),(0,2,4)),(1,33,96,513),
                                  (0,128),(False,True),(0,1,2,3),(False,True)):
        yield args+('normal',)
    for flags,multi in itertools.product((0,1,2,3),(False,True)):
        yield ((0,1),33,128,True,flags,multi,'shutdown')
        if multi and flags&1:
            for mode in ('outer_refusal','inner_refusal'):
                yield ((0,1),33,128,True,flags,multi,mode)


def main():
    roots,addresses=partial.providers()
    roots=[r for r in roots if r['address'] not in (0xa026f0,0xa02710,0xa03780,0xa038c0)]
    roots.extend(r for r in partial.policy.ROOTS if r['name']=='scoped_critical_dtor')
    fixture=dict(source=R/'rebuild/tests/integration/PoolFailureCallback_fixture.cpp',
                 symbol='@PoolFailureCallback@12',address=CALLBACK)
    inputs={Path(__file__),Path(partial.__file__),Path(partial.relocation.__file__),
            R/'tools/decomp_pipeline/gate_native_movable_pool_lifecycle.py',
            R/'tools/decomp_pipeline/movable_pool_construct_oracle.py',EXE}
    for module in (partial.policy,partial.allocation,partial.areas,partial.critical,partial.queue,partial.relocation.movable):inputs.add(Path(module.__file__))
    def closure(path):
        if path in inputs:return
        inputs.add(path)
        import re
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q=next((q for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None)
            assert q,name
            closure(q)
    for root in roots+[fixture]:closure(root['source'])
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='pool-failure-policy-') as tmp:
        candidate=compile_codes(roots+[fixture],addresses,Path(tmp))
    raw=EXE.read_bytes()
    native={root['address']:read_virtual_data(raw,root['address'],len(candidate[root['address']])) for root in roots}
    native[CALLBACK]=candidate[CALLBACK]
    for root in roots:
        if root['address']!=0xa02b30:assert candidate[root['address']]==native[root['address']],root['symbol']
    cases=checkpoints=calls=released=successes=max_depth=0
    witnesses={}
    for args in matrix():
        try:
            actual,seen,stats=scenario(candidate,*args)
            original,_,original_stats=scenario(native,*args)
            assert actual==original and stats==original_stats
        except Exception as error:raise AssertionError(args) from error
        cases+=1;checkpoints+=len(actual);calls+=stats['calls'];released+=stats['released'];successes+=stats['result'];max_depth=max(max_depth,stats['max_depth'])
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
        if cases%64==0:print('failure policy progress cases=%d'%cases,flush=True)
    omissions=[];unexercised=[]
    for root in roots:
        if root['address'] not in witnesses:unexercised.append(root['symbol']);continue
        reduced={a:data for a,data in candidate.items() if a!=root['address']}
        detected=False
        try:scenario(reduced,*witnesses[root['address']])
        except Exception:detected=True
        assert detected,root['symbol']
        omissions.append(root['symbol'])
    assert not unexercised,unexercised
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    report=dict(status='PASS',cases=cases,checkpoints=checkpoints,providers=len(omissions),missing_provider_checks=omissions,
                unexercised=unexercised,callback_calls=calls,released_handles=released,successes=successes,max_lock_depth=max_depth,input_sha256=pins,
                boundaries=['Failure callback releases queued handles through real Free; callback is fixture code on both sides.',
                            'Win32 locks, CRT allocation/memmove and resource callbacks controlled; no concurrent threads or bank-handler implementation claim.'])
    (R/'rebuild/compile-gate/native-pool-failure-policy.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_POOL_FAILURE_POLICY_PASS cases=%d checkpoints=%d providers=%d unexercised=%d calls=%d released=%d depth=%d'%(cases,checkpoints,len(omissions),len(unexercised),calls,released,max_depth))
    for name in unexercised:print('UNEXERCISED',name)


if __name__=='__main__':main()

"""Resubmit actual eviction restart trees and release their actual owned nodes."""
import hashlib
import itertools
import json
import tempfile
from pathlib import Path
import gate_native_loading_restart_tree as tree
import gate_bank_restart as restart
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R=tree.R
ENTRY,ERASE,ADD,FILE=0x9d5a90,0x9d7430,0x98f350,0x200c000


def providers():
    roots,addresses=tree.providers()
    spec=restart.SPECS[0]
    erase=spec['relocations'][2][1][0]
    for symbol,address,filename in ((spec['symbol'],ENTRY,spec['file']),
            (erase,ERASE,'STLAsyncBankTree_erase_009d7430.cpp')):
        roots.append(dict(source=R/'rebuild/src/compiled/00/9d'/filename,symbol=symbol,address=address))
        addresses[symbol]=address
    addresses['?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z']=ADD
    return roots,addresses


class SubmitMachine(tree.TreeMachine):
    def __init__(self,codes,multi,threshold,registered):
        super().__init__({a:b for a,b in codes.items() if a!=ADD},multi,threshold,registered)
        self.u.mem_map(0x98f000,0x1000);self.u.mem_write(ADD,b'\xc3')
        self.put(tree.loading.BANK+0x110,FILE)
        self.submissions=[];self.descriptors={}
        for index in range(8):
            obj=tree.loading.OBJECTS+index*0x48
            desc=bytes((i*37+index*19)&255 for i in range(24))
            self.u.mem_write(obj+0x2c,desc);self.descriptors[obj]=desc
            self.put(obj+0x18,0xabcdef00+index)

    def hook(self,u,address,size,user):
        if address==ADD:
            sp=u.reg_read(UC_X86_REG_ESP);desc=self.get(sp+4);obj=desc-0x2c
            assert u.reg_read(UC_X86_REG_ECX)==FILE
            assert self.held==[tree.loading.BANK+0x14c]
            assert bytes(u.mem_read(desc,24))==self.descriptors[obj]
            assert obj not in self.submissions
            self.submissions.append(obj)
            uid=0x80000000+len(self.submissions)*17
            self.trace.append(('submit',obj,desc,uid))
            u.reg_write(UC_X86_REG_EAX,uid);u.reg_write(UC_X86_REG_ECX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0x87654321)
            u.reg_write(UC_X86_REG_EIP,self.get(sp));u.reg_write(UC_X86_REG_ESP,sp+8)
            return
        super().hook(u,address,size,user)

    def submit(self):
        expected=sorted(self.restarted);self.restarted=[]
        before=set(self.blocks)
        self.call(ENTRY,[],tree.loading.BANK)
        assert self.submissions==expected
        assert len(before-set(self.blocks))==len(expected)
        for index in range(8):
            obj=tree.loading.OBJECTS+index*0x48
            uid=0x80000000+(expected.index(obj)+1)*17 if obj in expected else 0xabcdef00+index
            assert self.get(obj+0x18)==uid
            assert bytes(self.u.mem_read(obj+0x2c,24))==self.descriptors[obj]
        snapshots=[self.snapshot()]
        # A second empty update must not resubmit or free anything.
        before=(tuple(self.submissions),dict(self.blocks))
        self.call(ENTRY,[],tree.loading.BANK)
        assert (tuple(self.submissions),self.blocks)==before
        snapshots.append(self.snapshot())
        return snapshots


def scenario(codes,args):
    machines=[]
    def factory(*params):
        m=SubmitMachine(*params);machines.append(m);return m
    snapshots,_,stats=tree.loading.scenario(codes,*args,machine_type=factory)
    m=machines[0];snapshots.extend(m.submit())
    return snapshots,m.executed,stats


def main():
    roots,addresses=providers()
    inputs={Path(__file__),Path(tree.__file__),Path(tree.loading.__file__),Path(restart.__file__),EXE,
            R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'}
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
    with tempfile.TemporaryDirectory(prefix='loading-restart-submit-') as tmp:
        candidate=compile_codes(roots,addresses,Path(tmp))
    raw=EXE.read_bytes()
    native={a:read_virtual_data(raw,a,314 if a==tree.loading.bank.ENTRY else 79 if a==tree.PRED else len(b)) for a,b in candidate.items()}
    for a in (ENTRY,ERASE):assert candidate[a]==native[a],hex(a)
    cases=checkpoints=submitted=0;witnesses={}
    # Existing insertion gate exercises the predecessor; this composition need
    # not artificially reorder the bank cursor to exercise that helper again.
    matrix=itertools.product((0,3,85,255),(False,True),(33,96,513),(0,2),(0,128),(False,True),(False,True))
    for args in matrix:
        try:
            actual,seen,stats=scenario(candidate,args);original,_,other=scenario(native,args)
            assert actual==original and stats==other
        except Exception as error:raise AssertionError(args) from error
        cases+=1;checkpoints+=len(actual);submitted+=stats['restarted']
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
        if cases%64==0:print('restart submit progress cases=%d'%cases,flush=True)
    omissions=[];cold=[]
    for root in roots:
        if root['address'] not in witnesses:cold.append(hex(root['address']));continue
        reduced={a:b for a,b in candidate.items() if a!=root['address']};detected=False
        try:scenario(reduced,witnesses[root['address']])
        except Exception:detected=True
        assert detected,root['symbol'];omissions.append(root['symbol'])
    assert not cold,cold
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
    report=dict(status='PASS',cases=cases,checkpoints=checkpoints,submitted=submitted,
                providers=len(roots),missing_provider_checks=omissions,cold_providers=cold,input_sha256=pins,
                boundaries=['Threaded AddObject records descriptors and returns deterministic UIDs; no actual disk submission or concurrency.',
                            'Actual bank restart traversal, UID writes, tree erase and CRT frees execute; original and rebuilt compared.',
                            'Predecessor helper behavior-only; no new recovery or folded-clone grade claim.'])
    (R/'rebuild/compile-gate/native-loading-restart-submit.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_LOADING_RESTART_SUBMIT_PASS cases=%d checkpoints=%d submitted=%d providers=%d omissions=%d cold=%s'%
          (cases,checkpoints,submitted,len(roots),len(omissions),','.join(cold) or '0'))


if __name__=='__main__':main()

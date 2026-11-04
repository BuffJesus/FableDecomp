"""Read-begin/allocation/completion after actual eviction and restart submission."""
import hashlib
import itertools
import json
import tempfile
from pathlib import Path
import gate_native_loading_restart_submit as submit
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

tree=submit.tree
loading=tree.loading
R=submit.R
BEGIN,COMPLETE,SETUP,ALIGN=0x9d5440,0x9d54a0,0x9d52d0,0x9cbb90


def providers():
    roots,addresses=submit.providers()
    roots.append(next(r for r in loading.bank.buffer.ROOTS if r['address']==SETUP))
    for address,method,symbol in (
            (BEGIN,'OnFileReadBegin','?OnFileReadBegin@CBankFileAsyncData@@UAE_NPAPAXW4EThreadedFilePriority@@@Z'),
            (COMPLETE,'OnFileReadCompletion','?OnFileReadCompletion@CBankFileAsyncData@@UAEX_N@Z')):
        roots.append(dict(source=R/'rebuild/src/compiled/00/9d'/('CBankFileAsyncData_%s_%08x.cpp'%(method,address)),symbol=symbol,address=address))
    symbol='?Align@CBankFile@@QBEKK@Z'
    roots.append(dict(source=R/'rebuild/src/compiled/00/9d/CBankFileAsync_GetAsyncEntry_009d58d0.cpp',symbol=symbol,address=ALIGN))
    addresses[symbol]=ALIGN
    return roots,addresses


class ReadMachine(submit.SubmitMachine):
    def __init__(self,codes,*args):
        super().__init__({a:b for a,b in codes.items() if a!=ALIGN},*args)
        self.u.mem_map(0x9cb000,0x1000)
        if ALIGN in codes:self.u.mem_write(ALIGN,codes[ALIGN])
        else:self.u.mem_write(ALIGN,b'\xcc')
        self.u.mem_map(0x13ca000,0x1000);self.put(0x13ca868,0)
        self.put(loading.BANK+0x98,16)
        self.read_tries=0;self.refuse_at=0;self.refusals=0

    def hook(self,u,address,size,user):
        if address==loading.API+32 and self.refuse_at:
            self.read_tries+=1
            if self.read_tries==self.refuse_at:
                sp=u.reg_read(UC_X86_REG_ESP);section=self.get(sp+4)
                assert section in (loading.BANK+0x14c,loading.POOL+0x44c)
                self.refusals+=1;self.trace.append(('refused_read_try',section))
                u.reg_write(UC_X86_REG_EAX,0);u.reg_write(UC_X86_REG_ECX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0x87654321)
                u.reg_write(UC_X86_REG_EIP,self.get(sp));u.reg_write(UC_X86_REG_ESP,sp+8)
                return
        super().hook(u,address,size,user)

    def read_restarted(self,mode):
        snapshots=[];successes=0
        for obj in self.submissions:
            index=(obj-loading.OBJECTS)//0x48
            length,skip=17+index*7,(index%3)*4
            self.put(obj+0x10,length);self.put(obj+0x14,skip)
            output=0x200d100;self.put(output,0x76543210)
            self.refuse_at=mode if mode<3 or self.multi else 0
            self.read_tries=0
            refused=bool(self.refuse_at)
            ok=self.call(BEGIN,[output,index%3],obj)&255
            assert ok==int(not refused)
            snapshots.append(self.snapshot())
            if refused:
                assert self.get(output)==0x76543210
                assert self.get(obj+12)==0 and self.get(obj+0x20)==0xffffffff
                self.refuse_at=0
                assert self.call(BEGIN,[output,index%3],obj)&255
                snapshots.append(self.snapshot())
            pointer=self.get(output);handle=self.get(obj+0x20)
            assert pointer==self.get(obj+12) and self.get(obj+8)==pointer+skip
            assert self.call(0xa026b0,[handle])==pointer
            assert self.call(0xa026d0,[handle])==((length+skip+15)//16)*16
            assert self.call(0xa028d0,[handle])==obj+4
            payload=bytes((i*13+index*23)&255 for i in range(length+skip))
            self.u.mem_write(pointer,payload)
            self.call(COMPLETE,[index%2],obj)
            assert self.get(obj+0x18)==0 and self.u.mem_read(obj+0x44,1)==b'\1'
            assert self.u.mem_read(obj+0x46,1)==b'\1'
            assert bytes(self.u.mem_read(self.get(obj+8),length))==payload[skip:]
            snapshots.append(self.snapshot())
            self.call(0x9d5410,[],obj)
            assert self.get(obj+8)==0 and self.get(obj+12)==0 and self.get(obj+0x20)==0xffffffff
            snapshots.append(self.snapshot());successes+=1
        assert self.get(loading.POOL+0x10)==0 and self.get(loading.POOL+0x18)==0
        assert self.get(loading.POOL+0x1c)==480
        return snapshots,successes,self.refusals


def scenario(codes,args):
    eligible,locked,request,threshold,multi,mode=args
    machines=[]
    def factory(*params):
        m=ReadMachine(*params);machines.append(m);return m
    snapshots,_,stats=loading.scenario(codes,eligible,locked,request,0,threshold,multi,True,machine_type=factory)
    m=machines[0];snapshots.extend(m.submit())
    read,count,refused=m.read_restarted(mode);snapshots.extend(read)
    return snapshots,m.executed,(count,refused,stats)


def main():
    roots,addresses=providers()
    inputs={Path(__file__),Path(submit.__file__),Path(tree.__file__),Path(loading.__file__),EXE,
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
    with tempfile.TemporaryDirectory(prefix='loading-restart-read-') as tmp:
        candidate=compile_codes(roots,addresses,Path(tmp))
    raw=EXE.read_bytes()
    native={a:read_virtual_data(raw,a,314 if a==loading.bank.ENTRY else 79 if a==tree.PRED else len(b)) for a,b in candidate.items()}
    for a in (BEGIN,COMPLETE,SETUP,ALIGN):assert candidate[a]==native[a],hex(a)
    cases=checkpoints=reads=refusals=0;witnesses={}
    matrix=itertools.product((0,3,85,255),(False,True),(33,513),(0,128),(False,True),(0,1,2,3))
    for args in matrix:
        try:
            actual,seen,stats=scenario(candidate,args);original,_,other=scenario(native,args)
            assert actual==original and stats==other
        except Exception as error:raise AssertionError(args) from error
        cases+=1;checkpoints+=len(actual);reads+=stats[0];refusals+=stats[1]
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
        if cases%64==0:print('restart read progress cases=%d'%cases,flush=True)
    omissions=[]
    for root in roots:
        assert root['address'] in witnesses,('unexercised',root['symbol'])
        reduced={a:b for a,b in candidate.items() if a!=root['address']};detected=False
        try:scenario(reduced,witnesses[root['address']])
        except Exception:detected=True
        assert detected,root['symbol'];omissions.append(root['symbol'])
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
    report=dict(status='PASS',cases=cases,checkpoints=checkpoints,reads=reads,refusals=refusals,
                providers=len(roots),missing_provider_checks=omissions,input_sha256=pins,
                boundaries=['Threaded AddObject and disk payload delivery controlled; no disk IO or concurrency.',
                            'Actual read-begin, SetupBuffer pool branch, completion, restart/tree/pool chain and cleanup execute.',
                            'Forced heap fallback is not executed; predecessor helper behavior-only.'])
    (R/'rebuild/compile-gate/native-loading-restart-read.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_LOADING_RESTART_READ_PASS cases=%d checkpoints=%d reads=%d refusals=%d providers=%d omissions=%d'%
          (cases,checkpoints,reads,refusals,len(roots),len(omissions)))


if __name__=='__main__':main()

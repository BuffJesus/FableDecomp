"""Bank wait through the real file wrapper/manager, then actual data completion."""
import hashlib
import itertools
import json
import tempfile
from pathlib import Path
import gate_native_loading_restart_read as read
import threaded_manager_wait_oracle as wait
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R,loading,tree=read.R,read.loading,read.tree
MANAGER,HEADER,NODES,REQUESTS=0x13bc870,0x200e000,0x200e100,0x200f000
EVENT,SLEEP=loading.API+0x200,loading.API+0x210
WAIT=0x9d5b60


def providers():
    roots,addresses=read.providers()
    roots=[r for r in roots if r['address']!=read.BEGIN]
    roots.extend(wait.providers()[0]);addresses.update(wait.providers()[1])
    roots.extend(r for r in loading.bank.buffer.ROOTS if r['address']==WAIT)
    for filename,symbol,address in (
        ('00/98/CThreadedFile_WaitForObjectToFinish_0098f370.cpp','?WaitForObjectToFinish@CThreadedFile@@QAEXK@Z',0x98f370),
        ('00/a6/CCriticalSectionLock_Release_00a62960.cpp','?Release@CCriticalSectionLock@@QAEXXZ',0xa62960)):
        roots.append(dict(source=R/'rebuild/src/compiled'/filename,symbol=symbol,address=address));addresses[symbol]=address
    addresses['?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A']=MANAGER
    addresses[wait.SYMBOL]=wait.ENTRY
    return roots,addresses


class WaitMachine(read.ReadMachine):
    def __init__(self,codes,*args):
        super().__init__({a:b for a,b in codes.items() if a>>16 not in (0x98,0x99)},*args)
        # SubmitMachine already owns the adjacent 98F000 AddObject page.
        self.u.mem_map(0x98e000,0x1000);self.u.mem_write(0x98e000,b'\xcc'*0x2000)
        self.u.mem_write(0x98f350,b'\xc3')
        self.u.mem_map(0x990000,0x2000);self.u.mem_write(0x990000,b'\xcc'*0x2000)
        self.u.mem_map(0x13bc000,0x1000);self.u.mem_write(MANAGER,b'\0'*0x178)
        for a,b in codes.items():
            if a>>16 in (0x98,0x99):self.u.mem_write(a,b)
        for a in (EVENT,SLEEP,0x98e770,0x991350):self.u.mem_write(a,b'\xc3')
        self.put(0x143fde0,EVENT);self.put(0x143fddc,SLEEP)
        self.put(MANAGER+0x13c,HEADER);self.put(MANAGER+0x164,0x12345678)
        self.completions=0;self.sleeps=0;self.active=None;self.delay=0
        self.put(HEADER,0);self.put(HEADER+4,0);self.put(HEADER+8,HEADER);self.put(HEADER+12,HEADER)

    def snapshot(self):
        return super().snapshot(),bytes(self.u.mem_read(MANAGER,0x178))

    def prepare_requests(self,status,device,finished):
        self.put(read.submit.FILE+4,device)
        self.requests={}
        def build(first,last,parent):
            if first==last:return 0
            mid=(first+last)//2;node=NODES+mid*24;obj=self.submissions[mid];request=REQUESTS+mid*64
            uid=self.get(obj+0x18);self.requests[obj]=(uid,request)
            self.put(node,1);self.put(node+4,parent);self.put(node+8,build(first,mid,node));self.put(node+12,build(mid+1,last,node))
            self.put(node+16,uid);self.put(node+20,request)
            self.put(request+0x14,3 if mid%2==0 else 6);self.put(request+0x18,uid)
            self.put(request+0x1c,4 if finished else status);self.put(request+0x28,read.submit.FILE)
            return node
        count=len(self.submissions)
        self.put(HEADER+4,build(0,count,HEADER));self.put(HEADER+8,NODES if count else HEADER)
        self.put(HEADER+12,NODES+(count-1)*24 if count else HEADER);self.put(MANAGER+0x140,count)

    def hook(self,u,address,size,user):
        if address in (loading.API,loading.API+16,loading.API+32):
            sp=u.reg_read(UC_X86_REG_ESP);section=self.get(sp+4)
            if section==MANAGER+0x14c:
                assert address!=loading.API+32
                assert loading.BANK+0x14c not in self.held
                if address==loading.API:self.held.append(section);name='manager_enter'
                else:assert self.held and self.held.pop()==section;name='manager_leave'
                self.depth=len(self.held);assert self.depth<=2
                self.trace.append((name,self.depth));self.max_depth=max(self.max_depth,self.depth)
                u.reg_write(UC_X86_REG_EAX,1);u.reg_write(UC_X86_REG_ECX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0x87654321)
                u.reg_write(UC_X86_REG_EIP,self.get(sp));u.reg_write(UC_X86_REG_ESP,sp+8);return
        if address in (0x98e770,0x991350):
            assert self.active is not None and self.held==[MANAGER+0x14c]*2
            sp=u.reg_read(UC_X86_REG_ESP);obj=self.active;request=self.requests[obj][1]
            if address==0x98e770:
                assert u.reg_read(UC_X86_REG_ECX)==MANAGER and self.get(sp+4)==request
                assert self.get(request+0x14)==3;result=1;name='pending_remove'
            else:
                assert self.get(self.get(sp+4))==request and self.get(request+0x14)==6
                device=self.get(read.submit.FILE+4)
                assert u.reg_read(UC_X86_REG_ECX)==MANAGER+4+(device*7+6)*16
                result=0x200d200+(obj-loading.OBJECTS)//0x48*4;name='pending_insert'
            self.trace.append((name,request))
            u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ECX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0x87654321)
            u.reg_write(UC_X86_REG_EIP,self.get(sp));u.reg_write(UC_X86_REG_ESP,sp+8);return
        if address in (EVENT,SLEEP):
            assert self.active is not None and not self.held
            sp=u.reg_read(UC_X86_REG_ESP);obj=self.active
            if address==EVENT:
                assert self.get(sp+4)==0x12345678;self.trace.append(('manager_event',))
                result=1;pop=4
            else:
                assert self.get(sp+4)==0 and self.get(sp+8)==1
                self.sleeps+=1;self.trace.append(('manager_sleep_ex',self.sleeps))
                if self.sleeps==self.delay:
                    self.put(self.requests[obj][1]+0x1c,4)
                    # Controlled scheduling adapter: replace SleepEx's frame
                    # with a genuine thiscall completion frame. Its ret4 leaves
                    # the same stack as SleepEx's stdcall ret8 would have done.
                    self.put(sp+4,self.get(sp));self.put(sp+8,(obj-loading.OBJECTS)//0x48%2)
                    u.reg_write(UC_X86_REG_ESP,sp+4);u.reg_write(UC_X86_REG_ECX,obj)
                    u.reg_write(UC_X86_REG_EIP,read.COMPLETE);return
                assert self.sleeps<self.delay;result=0xc0;pop=8
            u.reg_write(UC_X86_REG_EAX,result);u.reg_write(UC_X86_REG_ECX,0xdeadbeef);u.reg_write(UC_X86_REG_EDX,0x87654321)
            u.reg_write(UC_X86_REG_EIP,self.get(sp));u.reg_write(UC_X86_REG_ESP,sp+4+pop);return
        if address==read.COMPLETE:
            assert not self.held and u.reg_read(UC_X86_REG_ECX)==self.active
            self.completions+=1;self.trace.append(('actual_completion',self.active))
        super().hook(u,address,size,user)

    def wait_restarted(self,finished,disabled):
        snapshots=[];waited=0
        self.u.mem_write(loading.BANK+0x149,bytes([disabled]))
        for obj in self.submissions:
            index=(obj-loading.OBJECTS)//0x48;length,skip=17+index*7,(index%3)*4
            self.put(obj+0x10,length);self.put(obj+0x14,skip)
            self.u.mem_write(obj+0x44,bytes([finished]))
            if finished:self.put(obj+0x18,0)
            self.active=obj;self.delay=1+(index%2)*2;self.sleeps=0
            old_uid=self.get(obj+0x18);before=self.completions
            assert self.call(WAIT,[],obj)&255
            active=not finished and not disabled
            assert self.completions-before==int(active)
            assert self.sleeps==(self.delay if active else 0)
            if active:
                handle=self.get(obj+0x20);pointer=self.get(obj+12)
                assert pointer and self.get(obj+8)==pointer+skip
                assert self.call(0xa026b0,[handle])==pointer
                assert self.call(0xa026d0,[handle])==((length+skip+15)//16)*16
                assert self.call(0xa028d0,[handle])==obj+4
                assert self.get(obj+0x18)==0 and self.u.mem_read(obj+0x44,1)==b'\1'
                assert self.u.mem_read(obj+0x46,1)==b'\1'
                self.validate_completed_request(obj);waited+=1
            else:
                assert self.get(obj+0x20)==0xffffffff and self.get(obj+12)==0
                assert self.get(obj+0x18)==old_uid
            snapshots.append(self.snapshot())
            self.call(0x9d5410,[],obj);snapshots.append(self.snapshot())
        assert self.get(loading.POOL+0x10)==0 and self.get(loading.POOL+0x18)==0 and self.get(loading.POOL+0x1c)==480
        return snapshots,waited

    def validate_completed_request(self,obj):
        assert self.get(self.requests[obj][1]+0x1c)==4


def scenario(codes,args):
    eligible,requested,multi,finished,disabled,status,device=args;instances=[]
    def factory(*params):
        m=WaitMachine(*params);instances.append(m);return m
    snapshots,_,stats=loading.scenario(codes,eligible,False,requested,0,128,multi,True,machine_type=factory)
    m=instances[0];snapshots.extend(m.submit());m.prepare_requests(status,device,finished)
    extra,waited=m.wait_restarted(finished,disabled);snapshots.extend(extra)
    return snapshots,m.executed,waited


def main():
    roots,addresses=providers();inputs={Path(__file__),Path(read.__file__),Path(wait.__file__),EXE,
        Path(loading.__file__),Path(tree.__file__),Path(read.submit.__file__),R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'}
    visited=set()
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
    with tempfile.TemporaryDirectory(prefix='loading-wait-completion-') as tmp:codes=compile_codes(roots,addresses,Path(tmp))
    raw=EXE.read_bytes();sizes={loading.bank.ENTRY:314,tree.PRED:79,wait.ENTRY:181}
    native={a:read_virtual_data(raw,a,sizes.get(a,len(b))) for a,b in codes.items()}
    for a in (WAIT,0x98f370,0xa62960):assert codes[a]==native[a],hex(a)
    cases=checkpoints=waited=0;witnesses={}
    for args in itertools.product((0,3,85,255),(33,513),(False,True),(False,True),(False,True),(0,2),(0,1)):
        try:
            actual,seen,count=scenario(codes,args);original,_,other=scenario(native,args)
            assert actual==original and count==other
        except Exception as error:raise AssertionError(args) from error
        cases+=1;checkpoints+=len(actual);waited+=count
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
        if cases%64==0:print('loading wait progress cases=%d'%cases,flush=True)
    for root in roots:
        assert root['address'] in witnesses,('unexercised',root['symbol'])
        reduced={a:b for a,b in codes.items() if a!=root['address']};detected=False
        try:scenario(reduced,witnesses[root['address']])
        except Exception:detected=True
        assert detected,root['symbol']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
    report=dict(status='PASS',cases=cases,checkpoints=checkpoints,waited=waited,providers=len(roots),missing_provider_checks=len(roots),input_sha256=pins,
                boundaries=['Worker progress, pending queue edits, Win32, CRT and threaded AddObject controlled; no actual IO/concurrency.',
                            'Actual bank wait/forced pool allocation, restart cleanup, lock release, file wrapper, manager wait/priority/lookup and data completion execute.',
                            'Scheduling adapter changes manager status and runs actual data completion while both locks are released; manager OnFinishObject not executed.'])
    (R/'rebuild/compile-gate/native-loading-wait-completion.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_LOADING_WAIT_COMPLETION_PASS cases=%d checkpoints=%d waited=%d providers=%d omissions=%d'%(cases,checkpoints,waited,len(roots),len(roots)))


if __name__=='__main__':main()

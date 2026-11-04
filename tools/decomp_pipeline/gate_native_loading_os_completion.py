"""Original OS read callback through actual manager/data completion and cleanup."""
import hashlib
import itertools
import json
import re
import sys
import tempfile
from pathlib import Path

import gate_native_loading_completion_cleanup as base
import gate_native_threaded_read_completion as callback
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP

R,wait,loading = base.R,base.wait,base.loading
MANAGER = base.MANAGER
DELIVERED = loading.API+0x240


def providers():
    roots,addresses = base.providers()
    roots.extend(callback.ROOTS)
    addresses.update(callback.ADDRESS)
    return roots,addresses


class OSMachine(base.CleanupMachine):
    def __init__(self,*args):
        super().__init__(*args)
        self.u.mem_write(DELIVERED,b'\xcc')
        self.delivery_return = None
        self.delivered = 0
        self.put(0x13bc864,0)  # No hard-disk error callback installed.
        self.u.mem_write(0x13bc868,b'\0')
        self.put(0x13ca818,0)  # Installer absence is a boundary of this composition.

    def snapshot(self):
        return super().snapshot(),bytes(self.u.mem_read(0x13bc000,0x1000)),bytes(self.u.mem_read(0x13ca000,0x1000))

    def prepare_requests(self,*args):
        super().prepare_requests(*args)
        for index,obj in enumerate(self.submissions):
            request = self.requests[obj][1]
            block = base.OVERLAPPED+index*32
            self.put(request+0x20,block)
            self.put(request+0x24,MANAGER)
            self.u.mem_write(block,bytes((index*19+i*7)&255 for i in range(20)))
            self.put(block+16,request)
            self.overlapped[block] = request
            self.initial_overlapped[block] = bytes(self.u.mem_read(block,20))

    def hook(self,u,address,size,user):
        if address == DELIVERED:
            assert self.delivery_return is not None and not self.held
            request = self.requests[self.active][1]
            assert self.get(request+0x1c) == 3 and self.get(request+0xc) == self.active
            assert self.get(MANAGER+0x138) == 0
            assert self.get(self.active+0x18) == self.requests[self.active][0]
            sp = u.reg_read(UC_X86_REG_ESP)
            self.put(sp-8,self.delivery_return)
            self.put(sp-4,request)
            self.worker_return = self.delivery_return
            self.delivery_return = None
            self.delivered += 1
            self.put(MANAGER+0x138,request)
            self.held.append(MANAGER+0x14c)
            self.depth = 1
            self.trace.append(('worker_lock',request))
            u.reg_write(UC_X86_REG_ESP,sp-8)
            u.reg_write(UC_X86_REG_ECX,MANAGER)
            u.reg_write(UC_X86_REG_EIP,base.base.FINISH)
            return
        if address == wait.SLEEP and self.sleeps+1 == self.delay:
            assert self.active is not None and not self.held and self.delivery_return is None
            request = self.requests[self.active][1]
            sp = u.reg_read(UC_X86_REG_ESP)
            assert self.get(sp+4) == 0 and self.get(sp+8) == 1
            self.sleeps += 1
            self.trace.append(('manager_sleep_ex',self.sleeps))
            self.delivery_return = self.get(sp)
            # stdcall callback ret12 reaches the same SP as SleepEx ret8.
            self.put(sp-4,DELIVERED)
            self.put(sp,self.delivery_error)
            self.put(sp+4,0xffffffff)  # Retail ignores transferred bytes.
            self.put(sp+8,self.get(request+0x20))
            u.reg_write(UC_X86_REG_ESP,sp-4)
            u.reg_write(UC_X86_REG_EIP,callback.ENTRY)
            return
        super().hook(u,address,size,user)


def scenario(codes,args,machine_type=OSMachine):
    *params,error = args
    machines = []
    def create(*arguments):
        m = machine_type(*arguments)
        m.delivery_error = error
        machines.append(m)
        return m
    snapshots,seen,stats = base.scenario(codes,params,machine_type=create)
    m = machines[0]
    assert m.delivered == stats[0] and m.delivery_return is None
    dirty = bool(m.delivered and error and params[6] == 1)
    assert m.u.mem_read(0x13bc868,1) == bytes([dirty])
    return snapshots,seen,stats


def main():
    roots,addresses = providers()
    assert len({r['address'] for r in roots}) == len(roots)
    inputs = {Path(__file__),EXE,callback.DATA_SOURCE}
    for module in tuple(sys.modules.values()):
        filename = getattr(module,'__file__',None)
        if filename and Path(filename).resolve().is_relative_to(R/'tools/decomp_pipeline'):
            inputs.add(Path(filename).resolve())
    visited = set()
    def closure(path):
        if path in visited:
            return
        visited.add(path)
        inputs.add(path)
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q = next((q for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None)
            assert q,name
            closure(q)
    for root in roots:
        closure(root['source'])
    closure(callback.DATA_SOURCE)
    pins = {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    data = callback.audit_error_state()
    with tempfile.TemporaryDirectory(prefix='loading-os-completion-') as tmp:
        codes = compile_codes(roots,addresses,Path(tmp))
    raw = EXE.read_bytes()
    sizes = {loading.bank.ENTRY:314,wait.tree.PRED:79,wait.wait.ENTRY:181}
    native = {a:read_virtual_data(raw,a,sizes.get(a,len(b))) for a,b in codes.items()}
    assert codes[callback.ENTRY] == native[callback.ENTRY]
    cases = checkpoints = waited = cleaned = 0
    witnesses = {}
    for args in itertools.product((0,3,85,255),(33,513),(False,True),(False,True),
                                  (False,True),(0,2),(0,1),('none','mixed','all'),('ok','fail'),(0,5)):
        try:
            actual,seen,stats = scenario(codes,args)
            original,_,other = scenario(native,args)
            assert actual == original and stats == other
        except Exception as error:
            raise AssertionError(args) from error
        cases += 1
        checkpoints += len(actual)
        waited += stats[0]
        cleaned += stats[1]
        for root in roots:
            if root['address'] in seen:
                witnesses.setdefault(root['address'],args)
        if cases%128 == 0:
            print('loading OS completion cases=%d'%cases,flush=True)
    for root in roots:
        assert root['address'] in witnesses,('unexercised',root['symbol'])
        reduced = {a:b for a,b in codes.items() if a != root['address']}
        detected = False
        try:
            scenario(reduced,witnesses[root['address']])
        except Exception:
            detected = True
        assert detected,root['symbol']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest() == h for p,h in pins.items())
    report = dict(status='PASS',cases=cases,checkpoints=checkpoints,waited=waited,cleaned=cleaned,
                  providers=len(roots),missing_provider_checks=len(roots),data=data,input_sha256=pins,
                  boundaries=['Actual OS read-completion callback publishes status3/signals event before actual OnFinishObject/data callback/UID cleanup.',
                              'Installer absent and hard-disk error callback null in this composition; their branches covered by the standalone callback gate.',
                              'All submitted requests own OVERLAPPED blocks; success/failure completion, retained/mixed/nonretained ownership and GlobalFree failures.',
                              'Scheduling/outer worker lock, pending edits, AddObject, Win32/CRT and disk delivery controlled; no real IO/APC/concurrency or new helper credit.'])
    (R/'rebuild/compile-gate/native-loading-os-completion.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_LOADING_OS_COMPLETION_PASS cases=%d checkpoints=%d waited=%d cleaned=%d providers=%d omissions=%d'%
          (cases,checkpoints,waited,cleaned,len(roots),len(roots)))


if __name__ == '__main__':
    main()

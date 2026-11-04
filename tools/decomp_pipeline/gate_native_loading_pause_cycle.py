"""Real installer Pause before read-wait, then actual OS completion and Unpause."""
import hashlib
import itertools
import json
import re
import struct
import sys
import tempfile
from pathlib import Path

import gate_native_loading_installer_completion as base
import gate_native_installer_pause as pause
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R,loading,INSTALLER = base.R,base.loading,base.INSTALLER
BANK_WAIT = base.base.wait.WAIT
PAUSE_WAIT = loading.API+0x260


def providers():
    roots,addresses = base.providers()
    roots.extend(pause.ROOTS)
    addresses.update(pause.ADDRESS)
    return roots,addresses


class CycleMachine(base.InstallerMachine):
    def __init__(self,*args):
        super().__init__(*args)
        self.u.mem_write(PAUSE_WAIT,b'\xc3')
        self.put(0x143fe08,PAUSE_WAIT)
        self.pausing = False
        self.pause_calls = self.pause_events = self.pause_waits = 0
        self.expected_pause_events = self.expected_unpause_events = 0

    def prepare_requests(self,*args):
        super().prepare_requests(*args)
        self.put(INSTALLER+0x8c,0x55667788)
        self.u.mem_write(0x13ca814,bytes([self.dirty]))
        self.installer_initial = bytes(self.u.mem_read(INSTALLER,0xa0))
        self.model_paused = self.initial_paused

    def call(self,address,args,this=loading.POOL):
        if address != BANK_WAIT:
            return super().call(address,args,this)
        assert self.get(INSTALLER+0x7c) == self.model_paused
        after_pause = (self.model_paused+1)&0xffffffff
        waiting = bool(self.running and after_pause <= 1 and not self.dirty)
        self.expected_pause_events += int(waiting)
        self.expected_unpause_events += 2*int(bool(self.running and after_pause == 1))
        self.pausing = True
        try:
            super().call(pause.ENTRY,[],INSTALLER)
        finally:
            self.pausing = False
        self.pause_calls += 1
        assert self.get(INSTALLER+0x7c) == after_pause
        assert self.pause_events == self.pause_waits == self.expected_pause_events
        result = super().call(address,args,this)
        self.model_paused = max(after_pause-1,0)
        assert self.get(INSTALLER+0x7c) == self.model_paused
        assert self.installer_events == self.expected_unpause_events
        return result

    def hook(self,u,address,size,user):
        if address == base.base.wait.EVENT and self.pausing:
            sp = u.reg_read(UC_X86_REG_ESP)
            assert not self.held and self.running and not self.dirty
            assert self.get(INSTALLER+0x7c) <= 1 and self.get(sp+4) == 0x77889900
            self.pause_events += 1
            self.trace.append(('pause_event',0x77889900))
            pop,result = 4,0 if self.multi else 0xffffffff
        elif address == PAUSE_WAIT:
            sp = u.reg_read(UC_X86_REG_ESP)
            assert self.pausing and not self.held
            assert [self.get(sp+i*4) for i in (1,2,3)] == [0x55667788,0xffffffff,1]
            self.pause_waits += 1
            assert self.pause_events == self.pause_waits
            self.trace.append(('pause_wait',0x55667788))
            pop,result = 12,0xc0 if self.delivery_error else 0
        else:
            super().hook(u,address,size,user)
            return
        u.reg_write(UC_X86_REG_EAX,result)
        u.reg_write(UC_X86_REG_ECX,0xdeadbeef)
        u.reg_write(UC_X86_REG_EDX,0x87654321)
        u.reg_write(UC_X86_REG_EIP,self.get(sp))
        u.reg_write(UC_X86_REG_ESP,sp+4+pop)


def scenario(codes,args):
    eligible,requested,multi,device,retention,overlap,error,paused,running,dirty = args
    machines = []
    def create(*params):
        m = CycleMachine(*params)
        m.initial_paused,m.running,m.dirty = paused,running,dirty
        machines.append(m)
        return m
    params = (eligible,requested,multi,False,False,0,device,retention,overlap,error)
    snapshots,seen,stats = base.base.scenario(codes,params,machine_type=create)
    m = machines[0]
    assert m.pause_calls == m.get_calls == m.unpause_calls == m.delivered
    assert m.pause_events == m.pause_waits == m.expected_pause_events
    assert m.installer_events == m.expected_unpause_events
    expected = bytearray(m.installer_initial)
    struct.pack_into('<I',expected,0x7c,m.model_paused)
    assert bytes(m.u.mem_read(INSTALLER,0xa0)) == bytes(expected)
    assert m.u.mem_read(0x13ca814,1) == bytes([dirty])
    return snapshots,seen,(*stats,m.installer_events,m.pause_waits)


def main():
    roots,addresses = providers()
    assert len({r['address'] for r in roots}) == len(roots)
    inputs = {Path(__file__),EXE,pause.DATA_SOURCE,base.base.callback.DATA_SOURCE}
    for module in tuple(sys.modules.values()):
        filename = getattr(module,'__file__',None)
        if filename and Path(filename).resolve().is_relative_to(R/'tools/decomp_pipeline'):
            inputs.add(Path(filename).resolve())
    visited = set()
    def closure(path):
        if path in visited:return
        visited.add(path);inputs.add(path)
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q = next((q for q in (path.parent/name,R/'rebuild/include'/name) if q.is_file()),None)
            assert q,name
            closure(q)
    for root in roots:closure(root['source'])
    for source in (pause.DATA_SOURCE,base.base.callback.DATA_SOURCE):closure(source)
    pins = {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    data = dict(installer=pause.audit_dirty_state(),threaded=base.base.callback.audit_error_state())
    with tempfile.TemporaryDirectory(prefix='loading-pause-cycle-') as tmp:
        codes = compile_codes(roots,addresses,Path(tmp))
    raw = EXE.read_bytes()
    sizes = {loading.bank.ENTRY:314,base.base.wait.tree.PRED:79,base.base.wait.wait.ENTRY:181,
             base.unpause.ENTRY:89,base.GET:69,pause.ENTRY:103}
    native = {a:read_virtual_data(raw,a,sizes.get(a,len(b))) for a,b in codes.items()}
    assert codes[pause.ENTRY] == native[pause.ENTRY]
    cases = checkpoints = waited = cleaned = events = waits = 0
    witnesses = {}
    for args in itertools.product((3,85,255),(33,513),(False,True),(0,1),
                                  ('none','mixed','all'),('ok','fail'),(0,5),(0,1,0xffffffff),(False,True),(0,1)):
        try:
            actual,seen,stats = scenario(codes,args)
            original,_,other = scenario(native,args)
            assert actual == original and stats == other
        except Exception as error:raise AssertionError(args) from error
        cases += 1;checkpoints += len(actual);waited += stats[0];cleaned += stats[1];events += stats[2];waits += stats[3]
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
        if cases%128 == 0:print('loading pause cycle cases=%d'%cases,flush=True)
    for root in roots:
        assert root['address'] in witnesses,('unexercised',root['symbol'])
        detected = False
        try:scenario({a:b for a,b in codes.items() if a != root['address']},witnesses[root['address']])
        except Exception:detected = True
        assert detected,root['symbol']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest() == h for p,h in pins.items())
    report = dict(status='PASS',cases=cases,checkpoints=checkpoints,waited=waited,cleaned=cleaned,
                  installer_events=events,pause_waits=waits,providers=len(roots),missing_provider_checks=len(roots),data=data,input_sha256=pins,
                  boundaries=['Actual Pause/read-completion/Get/Unpause/scoped locks/manager/data/UID cleanup execute; each read has an independently checked pause-count cycle.',
                              'Read initiation explicitly calls Pause at a fixture boundary; native ProcessObjects/call-site integration is not recovered here.',
                              'Pause acknowledgement and alertable waits controlled, including APC return; existing raw installer only, no owned installer workers.',
                              'Scheduling/outer worker lock, pending edits, AddObject, Win32/CRT and disk delivery controlled; no actual IO/APC/concurrency or new helper credit.'])
    (R/'rebuild/compile-gate/native-loading-pause-cycle.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_LOADING_PAUSE_CYCLE_PASS cases=%d checkpoints=%d waited=%d cleaned=%d events=%d pause_waits=%d providers=%d omissions=%d'%
          (cases,checkpoints,waited,cleaned,events,waits,len(roots),len(roots)))


if __name__ == '__main__':main()

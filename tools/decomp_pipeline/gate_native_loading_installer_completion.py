"""Loading OS completion with actual existing-singleton lookup/installer Unpause."""
import hashlib
import itertools
import json
import re
import sys
import tempfile
from pathlib import Path

import gate_native_loading_os_completion as base
import installer_unpause_oracle as unpause
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE,read_virtual_data
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP

R,loading = base.R,base.loading
INSTALLER = 0x200c400
GET = 0x404440


def providers():
    roots,addresses = base.providers()
    roots.extend(unpause.providers()[0][:1])
    roots.append(dict(source=R/'rebuild/src/compiled/00/40/CFileInstallerSingleton_Get_00404440.cpp',
                      symbol='?Get@CFileInstallerSingleton@@SIAAVCFileInstaller@@XZ',address=GET))
    addresses.update(unpause.ADDRESS)
    addresses.update({'?FableFileInstallerAllocate@@YAPAXI@Z':0xbfea1a,'??0CFileInstaller@@QAE@XZ':0x9c5890,
                      '??4CCountedFileInstaller@@QAEXPAVCFileInstaller@@@Z':0x403db0})
    return roots,addresses


class InstallerMachine(base.OSMachine):
    def __init__(self,codes,*args):
        extra = {a:b for a,b in codes.items() if a>>12 in (0x404,0x9c3)}
        super().__init__({a:b for a,b in codes.items() if a not in extra},*args)
        for page in (0x404000,0x9c3000):
            self.u.mem_map(page,0x1000)
            self.u.mem_write(page,b'\xcc'*0x1000)
        for address,body in extra.items():self.u.mem_write(address,body)
        self.u.mem_write(0xbfea1a,b'\xcc')  # Null singleton construction is excluded.
        self.get_calls = self.unpause_calls = self.installer_events = 0

    def prepare_requests(self,*args):
        super().prepare_requests(*args)
        self.put(0x13ca818,INSTALLER)
        self.put(INSTALLER+0x7c,self.initial_paused)
        self.u.mem_write(INSTALLER+0x7b,bytes([self.running]))
        self.put(INSTALLER+0x90,0x33445566)
        self.put(INSTALLER+0x84,0x77889900)
        self.installer_initial = bytes(self.u.mem_read(INSTALLER,0xa0))

    def hook(self,u,address,size,user):
        if address == GET:
            assert not self.held and self.get(0x13ca818) == INSTALLER
            self.get_calls += 1
        if address == unpause.ENTRY:
            assert not self.held and u.reg_read(UC_X86_REG_ECX) == INSTALLER
            self.unpause_calls += 1
        if address in (loading.API,loading.API+16):
            sp = u.reg_read(UC_X86_REG_ESP)
            section = self.get(sp+4)
            if section == INSTALLER+4:
                if address == loading.API:
                    assert not self.held
                    self.held.append(section)
                    name = 'installer_enter'
                else:
                    assert self.held == [section]
                    self.held.pop()
                    name = 'installer_leave'
                self.depth = len(self.held)
                self.trace.append((name,))
                u.reg_write(UC_X86_REG_EAX,1)
                u.reg_write(UC_X86_REG_ECX,0xdeadbeef)
                u.reg_write(UC_X86_REG_EDX,0x87654321)
                u.reg_write(UC_X86_REG_EIP,self.get(sp))
                u.reg_write(UC_X86_REG_ESP,sp+8)
                return
        if address == base.wait.EVENT and self.held == [INSTALLER+4]:
            sp = u.reg_read(UC_X86_REG_ESP)
            event = self.get(sp+4)
            assert self.running and self.get(INSTALLER+0x7c) == 0
            assert event == (0x33445566 if self.installer_events%2 == 0 else 0x77889900)
            self.installer_events += 1
            self.trace.append(('installer_event',event))
            u.reg_write(UC_X86_REG_EAX,0 if self.multi else 0xffffffff)
            u.reg_write(UC_X86_REG_ECX,0xdeadbeef)
            u.reg_write(UC_X86_REG_EDX,0x87654321)
            u.reg_write(UC_X86_REG_EIP,self.get(sp))
            u.reg_write(UC_X86_REG_ESP,sp+8)
            return
        super().hook(u,address,size,user)


def scenario(codes,args):
    eligible,requested,multi,device,retention,overlap,error,paused,running = args
    machines = []
    def create(*params):
        m = InstallerMachine(*params)
        m.initial_paused,m.running = paused,running
        machines.append(m)
        return m
    params = (eligible,requested,multi,False,False,0,device,retention,overlap,error)
    snapshots,seen,stats = base.scenario(codes,params,machine_type=create)
    m = machines[0]
    assert m.get_calls == m.unpause_calls == m.delivered
    expected = bytearray(m.installer_initial)
    remaining = max(paused-m.delivered,0)
    import struct
    struct.pack_into('<I',expected,0x7c,remaining)
    assert bytes(m.u.mem_read(INSTALLER,0xa0)) == bytes(expected)
    assert m.installer_events == 2*int(bool(paused and running and m.delivered >= paused))
    return snapshots,seen,(*stats,m.installer_events)


def main():
    roots,addresses = providers()
    assert len({r['address'] for r in roots}) == len(roots)
    inputs = {Path(__file__),EXE,base.callback.DATA_SOURCE}
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
    closure(base.callback.DATA_SOURCE)
    pins = {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    data = base.callback.audit_error_state()
    with tempfile.TemporaryDirectory(prefix='loading-installer-completion-') as tmp:
        codes = compile_codes(roots,addresses,Path(tmp))
    raw = EXE.read_bytes()
    sizes = {loading.bank.ENTRY:314,base.wait.tree.PRED:79,base.wait.wait.ENTRY:181,unpause.ENTRY:89,GET:69}
    native = {a:read_virtual_data(raw,a,sizes.get(a,len(b))) for a,b in codes.items()}
    assert codes[GET] == native[GET]
    cases = checkpoints = waited = cleaned = events = 0
    witnesses = {}
    for args in itertools.product((0,3,85,255),(33,513),(False,True),(0,1),
                                  ('none','mixed','all'),('ok','fail'),(0,5),(0,1,3),(False,True)):
        try:
            actual,seen,stats = scenario(codes,args)
            original,_,other = scenario(native,args)
            assert actual == original and stats == other
        except Exception as error:raise AssertionError(args) from error
        cases += 1;checkpoints += len(actual);waited += stats[0];cleaned += stats[1];events += stats[2]
        for root in roots:
            if root['address'] in seen:witnesses.setdefault(root['address'],args)
        if cases%128 == 0:print('loading installer completion cases=%d'%cases,flush=True)
    for root in roots:
        assert root['address'] in witnesses,('unexercised',root['symbol'])
        detected = False
        try:scenario({a:b for a,b in codes.items() if a != root['address']},witnesses[root['address']])
        except Exception:detected = True
        assert detected,root['symbol']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest() == h for p,h in pins.items())
    report = dict(status='PASS',cases=cases,checkpoints=checkpoints,waited=waited,cleaned=cleaned,installer_events=events,
                  providers=len(roots),missing_provider_checks=len(roots),data=data,input_sha256=pins,
                  boundaries=['Actual OS callback/existing-singleton Get/Unpause/scoped locks/manager/data completion/UID cleanup execute.',
                              'Existing raw installer storage only; installer construction, allocation, counted reset and worker runtime not executed.',
                              'Worker scheduling/outer lock, pending edits, AddObject, Win32/CRT and disk delivery controlled; no real IO/APC/concurrency or new helper credit.'])
    (R/'rebuild/compile-gate/native-loading-installer-completion.json').write_text(json.dumps(report,indent=2)+'\n')
    print('NATIVE_LOADING_INSTALLER_COMPLETION_PASS cases=%d checkpoints=%d waited=%d cleaned=%d events=%d providers=%d omissions=%d'%
          (cases,checkpoints,waited,cleaned,events,len(roots),len(roots)))


if __name__ == '__main__':main()

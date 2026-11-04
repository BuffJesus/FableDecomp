"""Real manager completion and virtual callback within the loading wait chain."""
import hashlib
import itertools
import json
import re
import tempfile
from pathlib import Path
import gate_native_loading_wait_completion as base
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE, read_virtual_data
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP

R, read, loading = base.R, base.read, base.loading
MANAGER = base.MANAGER
FINISH = 0x98ebc0


def providers():
    roots, addresses = base.providers()
    root = next(r for r in base.wait.manager.ROOTS if r['address'] == FINISH)
    roots.append(root)
    addresses[root['symbol']] = FINISH
    return roots, addresses


class FinishMachine(base.WaitMachine):
    def __init__(self, *args):
        super().__init__(*args)
        self.worker_return = None
        self.finished_requests = 0

    def prepare_requests(self, *args):
        super().prepare_requests(*args)
        assert self.get(0x129b8b4 + 8) == read.COMPLETE
        for obj, (_, request) in self.requests.items():
            self.put(obj, 0x129b8b4)
            self.put(request + 0xc, obj)
            self.u.mem_write(request + 0x10, b'\1')  # Retained completion queue.
            self.put(request + 0x20, 0)

    def hook(self, u, address, size, user):
        if self.worker_return == address:
            assert self.held == [MANAGER + 0x14c]
            request = self.requests[self.active][1]
            assert self.get(request + 0xc) == 0
            assert self.get(request + 0x1c) == 4
            assert self.get(MANAGER + 0x138) == 0
            self.worker_return = None
            self.held.pop()
            self.depth = 0
            self.trace.append(('worker_unlock', request))
            self.finished_requests += 1
        if address == base.SLEEP and self.sleeps + 1 == self.delay:
            assert self.active is not None and not self.held
            sp = u.reg_read(UC_X86_REG_ESP)
            assert self.get(sp + 4) == 0 and self.get(sp + 8) == 1
            self.sleeps += 1
            self.trace.append(('manager_sleep_ex', self.sleeps))
            request = self.requests[self.active][1]
            self.put(MANAGER + 0x138, request)
            # Model only the scheduler's outer worker lock. OnFinishObject
            # itself drops/reacquires it and dispatches the real virtual callback.
            self.held.append(MANAGER + 0x14c)
            self.depth = 1
            self.trace.append(('worker_lock', request))
            self.worker_return = self.get(sp)
            self.put(sp + 4, self.worker_return)
            self.put(sp + 8, request)
            u.reg_write(UC_X86_REG_ESP, sp + 4)
            u.reg_write(UC_X86_REG_ECX, MANAGER)
            u.reg_write(UC_X86_REG_EIP, FINISH)
            return
        super().hook(u, address, size, user)


def scenario(codes, args):
    eligible, requested, multi, finished, disabled, status, device = args
    instances = []
    def factory(*params):
        m = FinishMachine(*params)
        instances.append(m)
        return m
    snapshots, _, _ = loading.scenario(codes, eligible, False, requested, 0, 128,
                                      multi, True, machine_type=factory)
    m = instances[0]
    snapshots.extend(m.submit())
    m.prepare_requests(status, device, finished)
    extra, waited = m.wait_restarted(finished, disabled)
    snapshots.extend(extra)
    assert m.finished_requests == waited and m.worker_return is None
    return snapshots, m.executed, waited


def main():
    roots, addresses = providers()
    inputs = {Path(__file__), Path(base.__file__), Path(base.wait.__file__), EXE,
              Path(read.__file__), Path(loading.__file__), Path(base.tree.__file__),
              Path(read.submit.__file__), R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'}
    visited = set()
    def closure(path):
        if path in visited:
            return
        visited.add(path)
        inputs.add(path)
        for name in re.findall(r'^\s*#include\s*"([^"]+)"', path.read_text(), re.M):
            q = next((q for q in (path.parent/name, R/'rebuild/include'/name) if q.is_file()), None)
            assert q, name
            closure(q)
    for root in roots:
        closure(root['source'])
    pins = {str(p): hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='loading-manager-completion-') as tmp:
        codes = compile_codes(roots, addresses, Path(tmp))
    raw = EXE.read_bytes()
    sizes = {loading.bank.ENTRY: 314, base.tree.PRED: 79, base.wait.ENTRY: 181}
    native = {a: read_virtual_data(raw, a, sizes.get(a, len(b))) for a, b in codes.items()}
    for a in (FINISH, base.WAIT, 0x98f370, 0xa62960):
        assert codes[a] == native[a], hex(a)
    cases = checkpoints = waited = 0
    witnesses = {}
    for args in itertools.product((0, 3, 85, 255), (33, 513), (False, True),
                                  (False, True), (False, True), (0, 2), (0, 1)):
        try:
            actual, seen, count = scenario(codes, args)
            original, _, other = scenario(native, args)
            assert actual == original and count == other
        except Exception as error:
            raise AssertionError(args) from error
        cases += 1
        checkpoints += len(actual)
        waited += count
        for root in roots:
            if root['address'] in seen:
                witnesses.setdefault(root['address'], args)
        if cases % 64 == 0:
            print('loading manager completion cases=%d' % cases, flush=True)
    for root in roots:
        assert root['address'] in witnesses, ('unexercised', root['symbol'])
        reduced = {a: b for a, b in codes.items() if a != root['address']}
        detected = False
        try:
            scenario(reduced, witnesses[root['address']])
        except Exception:
            detected = True
        assert detected, root['symbol']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest() == h for p, h in pins.items())
    report = dict(status='PASS', cases=cases, checkpoints=checkpoints, waited=waited,
                  providers=len(roots), missing_provider_checks=len(roots), input_sha256=pins,
                  boundaries=['Actual OnFinishObject drops/reacquires the manager lock, invokes the real virtual data callback and publishes finished state.',
                              'QueueWhenFinished=true; UID/request storage retained. Nonretained completion cleanup covered separately, not composed here.',
                              'Worker scheduling/outer worker lock, pending queue edits, Win32, CRT and AddObject controlled; no actual IO/concurrency.'])
    (R/'rebuild/compile-gate/native-loading-manager-completion.json').write_text(json.dumps(report, indent=2)+'\n')
    print('NATIVE_LOADING_MANAGER_COMPLETION_PASS cases=%d checkpoints=%d waited=%d providers=%d omissions=%d' %
          (cases, checkpoints, waited, len(roots), len(roots)))


if __name__ == '__main__':
    main()

"""Loading waits with actual completion, UID erasure and request destruction."""
import hashlib
import itertools
import json
import re
import tempfile
from pathlib import Path

import gate_native_loading_manager_completion as base
from movable_pool_construct_oracle import compile_codes
from pe_oracle import EXE, read_virtual_data
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_ESP, UC_X86_REG_EIP

R, wait, loading, read = base.R, base.base, base.loading, base.read
MANAGER, HEADER, NODES, REQUESTS = wait.MANAGER, wait.HEADER, wait.NODES, wait.REQUESTS
GLOBAL_FREE = loading.API + 0x220
OVERLAPPED = 0x200e800
assert NODES+8*24 <= OVERLAPPED and OVERLAPPED+8*32 <= REQUESTS


def providers():
    roots, addresses = base.providers()
    manager = wait.wait.manager
    roots.extend(r for r in manager.ROOTS if r['name'] in
                 ('uid_key_erase', 'uid_equal_range', 'uid_distance', 'uid_range_erase', 'uid_node_erase'))
    # Range erase inlines iterator erase; its standalone wrapper is not executed.
    for symbol, address, filename in (
        ('?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z',
         0x42a34e, '00/42/STLRbTree_RebalanceForErase_0042a34e.cpp'),
        ('?_S_minimum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z',
         0x41c7e0, '00/41/STLRbTree_Extrema_0041c7e0.cpp'),
        ('?_S_maximum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z',
         0x41c800, '00/41/STLRbTree_Extrema_0041c7e0.cpp')):
        roots.append(dict(source=R/'rebuild/src/compiled'/filename, symbol=symbol, address=address))
        addresses[symbol] = address
    for root in roots:
        addresses[root['symbol']] = root['address']
    return roots, addresses


class CleanupMachine(base.FinishMachine):
    def __init__(self, codes, *args):
        extra = {a:b for a,b in codes.items() if a>>12 in (0x41c, 0x42a)}
        super().__init__({a:b for a,b in codes.items() if a not in extra}, *args)
        for page in (0x41c000, 0x42a000):
            self.u.mem_map(page, 0x1000)
            self.u.mem_write(page, b'\xcc'*0x1000)
        for address, body in extra.items():
            self.u.mem_write(address, body)
        self.u.mem_write(GLOBAL_FREE, b'\xc3')
        self.u.mem_write(0xbfe9bc, b'\xc3')
        self.put(0x143fdf8, GLOBAL_FREE)
        self.uid_nodes = {}
        self.live_requests = set()
        self.overlapped = {}
        self.deleted_requests = set()
        self.cleaned = 0

    def prepare_requests(self, status, device, finished):
        super().prepare_requests(status, device, finished)
        count = len(self.submissions)
        max_depth = count.bit_length()-1
        def color(node, depth):
            if not node:
                return
            self.put(node, int(depth == 0 or depth != max_depth))
            color(self.get(node+8), depth+1)
            color(self.get(node+12), depth+1)
        color(self.get(HEADER+4), 0)
        self.initial_overlapped = {}
        for index, obj in enumerate(self.submissions):
            uid, request = self.requests[obj]
            node = NODES+index*24
            self.uid_nodes[node] = uid
            self.live_requests.add(request)
            retained = self.retention == 'all' or self.retention == 'mixed' and index%2 == 0
            self.u.mem_write(request+0x10, bytes([retained]))
            block = OVERLAPPED+index*32 if self.overlap_mode != 'none' and index%2 else 0
            self.put(request+0x20, block)
            if block:
                self.overlapped[block] = request
                self.u.mem_write(block, bytes((index*19+i*7)&255 for i in range(20)))
                self.initial_overlapped[block] = bytes(self.u.mem_read(block,20))
        self.validate_uid_tree()

    def validate_uid_tree(self):
        seen = set()
        def walk(node, parent):
            if not node:
                return [], 1
            assert node in self.uid_nodes and node not in seen
            seen.add(node)
            assert self.get(node+4) == parent
            color = self.get(node)
            assert color in (0, 1)
            left, right = self.get(node+8), self.get(node+12)
            if color == 0:
                assert not left or self.get(left) == 1
                assert not right or self.get(right) == 1
            lo, lb = walk(left, node)
            hi, hb = walk(right, node)
            assert lb == hb
            assert self.get(node+16) == self.uid_nodes[node]
            assert self.get(node+20) in self.live_requests
            return lo+[self.get(node+16)]+hi, lb+color
        root = self.get(HEADER+4)
        keys, _ = walk(root, HEADER)
        assert keys == sorted(self.uid_nodes.values())
        assert seen == set(self.uid_nodes) and self.get(MANAGER+0x140) == len(seen)
        if root:
            assert self.get(root) == 1
            assert self.get(self.get(HEADER+8)+16) == keys[0]
            assert self.get(self.get(HEADER+12)+16) == keys[-1]
        else:
            assert self.get(HEADER+8) == HEADER == self.get(HEADER+12)

    def validate_completed_request(self, obj):
        request = self.requests[obj][1]
        if request in self.deleted_requests:
            assert bytes(self.u.mem_read(request, 0x40)) == b'\xdd'*0x40
        else:
            super().validate_completed_request(obj)
        self.validate_uid_tree()

    def hook(self, u, address, size, user):
        if address == self.worker_return:
            request = self.requests[self.active][1]
            if request in self.deleted_requests:
                assert self.held == [MANAGER+0x14c] and self.get(MANAGER+0x138) == 0
                self.worker_return = None
                self.held.pop()
                self.depth = 0
                self.trace.append(('worker_unlock', request))
                self.finished_requests += 1
                self.cleaned += 1
                self.validate_uid_tree()
        if address in (0xbfea14, 0xbfe9bc, GLOBAL_FREE):
            sp = u.reg_read(UC_X86_REG_ESP)
            pointer = self.get(sp+4)
            if pointer in self.uid_nodes or address in (0xbfe9bc, GLOBAL_FREE):
                assert self.held == [MANAGER+0x14c]
                request = self.requests[self.active][1]
                assert self.get(request+0xc) == 0 and self.get(request+0x1c) == 4
                assert self.get(MANAGER+0x138) == 0
                pop, result = 0, 0
                if address == 0xbfea14:
                    assert self.uid_nodes.pop(pointer) == self.requests[self.active][0]
                    u.mem_write(pointer, b'\xdd'*24)
                    name = 'uid_node_free'
                elif address == GLOBAL_FREE:
                    assert self.overlapped[pointer] == request
                    assert all(uid != self.requests[self.active][0] for uid in self.uid_nodes.values())
                    pop = 4
                    if self.overlap_mode == 'fail':
                        result = pointer
                    else:
                        del self.overlapped[pointer]
                        u.mem_write(pointer, b'\xdd'*20)
                    name = 'overlapped_free'
                else:
                    assert pointer == request and request in self.live_requests
                    assert all(uid != self.requests[self.active][0] for uid in self.uid_nodes.values())
                    block = self.get(request+0x20)
                    assert not block or self.overlap_mode == 'fail' or block not in self.overlapped
                    assert self.get(self.active+0x18) == 0
                    assert u.mem_read(self.active+0x44,1) == b'\1'
                    self.live_requests.remove(request)
                    self.deleted_requests.add(request)
                    u.mem_write(request, b'\xdd'*0x40)
                    name = 'request_delete'
                self.trace.append((name, pointer, result))
                u.reg_write(UC_X86_REG_EAX, result)
                u.reg_write(UC_X86_REG_ECX, 0xdeadbeef)
                u.reg_write(UC_X86_REG_EDX, 0x87654321)
                u.reg_write(UC_X86_REG_EIP, self.get(sp))
                u.reg_write(UC_X86_REG_ESP, sp+4+pop)
                return
        super().hook(u, address, size, user)


def scenario(codes, args, machine_type=CleanupMachine):
    eligible, requested, multi, finished, disabled, status, device, retention, overlap = args
    instances = []
    def factory(*params):
        m = machine_type(*params)
        m.retention, m.overlap_mode = retention, overlap
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
    assert len(m.deleted_requests) == m.cleaned
    assert len(m.uid_nodes) == len(m.requests)-m.cleaned
    for block in m.overlapped:
        assert bytes(m.u.mem_read(block,20)) == m.initial_overlapped[block]
    return snapshots, m.executed, (waited, m.cleaned)


def main():
    roots, addresses = providers()
    assert len({r['address'] for r in roots}) == len(roots)
    inputs = {Path(__file__), EXE, R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'}
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
    # Pin every imported project module, including the inherited harness chain.
    import sys
    for module in tuple(sys.modules.values()):
        filename = getattr(module, '__file__', None)
        if filename and Path(filename).resolve().is_relative_to(R/'tools/decomp_pipeline'):
            inputs.add(Path(filename).resolve())
    for root in roots:
        closure(root['source'])
    pins = {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    with tempfile.TemporaryDirectory(prefix='loading-completion-cleanup-') as tmp:
        codes = compile_codes(roots, addresses, Path(tmp))
    raw = EXE.read_bytes()
    sizes = {loading.bank.ENTRY:314, wait.tree.PRED:79, wait.wait.ENTRY:181}
    native = {a:read_virtual_data(raw, a, sizes.get(a, len(b))) for a,b in codes.items()}
    added = {r['address'] for r in roots}-{r['address'] for r in base.providers()[0]}
    for address in added | {base.FINISH}:
        assert codes[address] == native[address], hex(address)
    cases = checkpoints = waited = cleaned = 0
    witnesses = {}
    for args in itertools.product((0,3,85,255), (33,513), (False,True), (False,True),
                                  (False,True), (0,2), (0,1), ('none','mixed','all'), ('none','ok','fail')):
        try:
            actual, seen, stats = scenario(codes, args)
            original, _, other = scenario(native, args)
            assert actual == original and stats == other
        except Exception as error:
            raise AssertionError(args) from error
        cases += 1
        checkpoints += len(actual)
        waited += stats[0]
        cleaned += stats[1]
        for root in roots:
            if root['address'] in seen:
                witnesses.setdefault(root['address'], args)
        if cases%128 == 0:
            print('loading cleanup cases=%d'%cases, flush=True)
    for root in roots:
        assert root['address'] in witnesses, ('unexercised', root['symbol'])
        reduced = {a:b for a,b in codes.items() if a != root['address']}
        detected = False
        try:
            scenario(reduced, witnesses[root['address']])
        except Exception:
            detected = True
        assert detected, root['symbol']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest() == h for p,h in pins.items())
    report = dict(status='PASS', cases=cases, checkpoints=checkpoints, waited=waited, cleaned=cleaned,
                  providers=len(roots), missing_provider_checks=len(roots), input_sha256=pins,
                  boundaries=['Actual loading/manager wait/completion and UID erase/rebalance execute; retained, mixed and unretained requests.',
                              'Win32/CRT frees are controlled with poisoning, duplicate rejection and faithful GlobalFree failure retention.',
                              'Worker scheduling/outer lock, pending edits, AddObject and disk delivery controlled; no actual IO/concurrency or new recovery credit.'])
    (R/'rebuild/compile-gate/native-loading-completion-cleanup.json').write_text(json.dumps(report, indent=2)+'\n')
    print('NATIVE_LOADING_COMPLETION_CLEANUP_PASS cases=%d checkpoints=%d waited=%d cleaned=%d providers=%d omissions=%d'%
          (cases, checkpoints, waited, cleaned, len(roots), len(roots)))


if __name__ == '__main__':
    main()

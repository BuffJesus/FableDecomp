"""Independent full-storage/ABI checks for the four manager statistics getters."""
import hashlib
import itertools
import re
import struct
import tempfile
from pathlib import Path

import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE, read_virtual_data
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import (UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX,
    UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP,
    UC_X86_REG_ESP, UC_X86_REG_EIP)

R = Path(__file__).resolve().parents[2]
ARENA, LENGTH = 0x2000000, 0x6000
MANAGER, HEADER, NODES, OBJECTS = ARENA+0x100, ARENA+0x1000, ARENA+0x1100, ARENA+0x3000
STACK, STOP, API = 0x3000000, 0x4000000, 0x5000000
INITIAL = {seed:bytes((i*37+seed*19)&255 for i in range(LENGTH)) for seed in (0,7)}


def execute(codes, root, args):
    count, shape, status_mode, size_mode, seed, mutation = args
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, size in ((0x98e000,0x1000), (0x143f000,0x1000),
                          (ARENA,LENGTH), (STACK,0x10000), (STOP,0x1000), (API,0x1000)):
        u.mem_map(address, size)
    u.mem_write(0x98e000, b'\xcc'*0x1000)
    for address, body in codes.items():
        u.mem_write(address, body)
    u.mem_write(ARENA, INITIAL[seed])
    def put(address, value):
        u.mem_write(address, struct.pack('<I',value&0xffffffff))
    def get(address):
        return struct.unpack('<I',u.mem_read(address,4))[0]
    nodes = [NODES+i*24 for i in range(count)]
    statuses, sizes = [], []
    for index, node in enumerate(nodes):
        status = index%5 if status_mode == 5 else (0xffffffff if status_mode == 6 else status_mode)
        size = (index*0x76543211+seed)&0xffffffff if size_mode == 4 else (0,1,0x80000000,0xffffffff)[size_mode]
        statuses.append(status)
        sizes.append(size)
        put(node,1)
        put(node+16,index*17+1)
        put(node+20,OBJECTS+index*0x40)
        put(OBJECTS+index*0x40+0x1c,status)
        put(OBJECTS+index*0x40+8,size)
    def build(indices, parent):
        if not indices:
            return 0
        split = 0 if shape == 'right' else len(indices)-1 if shape == 'left' else len(indices)//2
        index = indices[split]
        node = nodes[index]
        put(node+4,parent)
        put(node+8,build(indices[:split],node))
        put(node+12,build(indices[split+1:],node))
        return node
    put(MANAGER+0x13c,HEADER)
    put(MANAGER+0x140,count)
    put(HEADER,0)
    put(HEADER+4,build(list(range(count)),HEADER))
    put(HEADER+8,nodes[0] if count else HEADER)
    put(HEADER+12,nodes[-1] if count else HEADER)
    expected = bytearray(u.mem_read(ARENA,LENGTH))
    selected_status = list(statuses)
    selected_size = list(sizes)
    if mutation == 'enter':
        for index in range(count):
            selected_status[index] = 0 if statuses[index] == 4 else 4
            selected_size[index] = (sizes[index]+0x13579bdf)&0xffffffff
    final_status, final_size = list(selected_status), list(selected_size)
    if mutation == 'leave':
        final_status = [4 if value != 4 else 0 for value in statuses]
        final_size = [(value^0xa5a5a5a5)&0xffffffff for value in sizes]
    for index in range(count):
        struct.pack_into('<I',expected,OBJECTS-ARENA+index*0x40+0x1c,final_status[index])
        struct.pack_into('<I',expected,OBJECTS-ARENA+index*0x40+8,final_size[index])
    matching = [i for i,status in enumerate(selected_status) if (status == 4) == root['finished']]
    expected_result = sum(selected_size[i] for i in matching) if root['bytes_total'] else len(matching)
    expected_result &= 0xffffffff
    put(0x143fe0c,API)
    put(0x143fe10,API+16)
    trace, seen = [], set()
    depth = 0
    def hook(uc, address, size, user):
        nonlocal depth
        seen.add(address)
        if address not in (API,API+16):
            return
        sp = uc.reg_read(UC_X86_REG_ESP)
        assert get(sp+4) == MANAGER+0x14c
        if address == API:
            assert not trace and depth == 0
            depth = 1
            trace.append('enter')
            states, lengths = selected_status, selected_size
        else:
            assert trace == ['enter'] and depth == 1
            depth = 0
            trace.append('leave')
            states, lengths = final_status, final_size
        for index in range(count):
            put(OBJECTS+index*0x40+0x1c,states[index])
            put(OBJECTS+index*0x40+8,lengths[index])
        uc.reg_write(UC_X86_REG_EAX,0xfedcba98)
        uc.reg_write(UC_X86_REG_ECX,0xdeadbeef)
        uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp))
        uc.reg_write(UC_X86_REG_ESP,sp+8)
    u.hook_add(UC_HOOK_CODE,hook)
    sp = STACK+0x8000
    put(sp,STOP)
    saved = (UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for index, reg in enumerate(saved):
        u.reg_write(reg,0x55660000+index+seed)
    u.reg_write(UC_X86_REG_ECX,MANAGER)
    u.reg_write(UC_X86_REG_ESP,sp)
    u.emu_start(root['address'],STOP,count=10000)
    assert u.reg_read(UC_X86_REG_EIP) == STOP and u.reg_read(UC_X86_REG_ESP) == sp+4
    assert all(u.reg_read(reg) == 0x55660000+index+seed for index,reg in enumerate(saved))
    result = u.reg_read(UC_X86_REG_EAX)
    assert result == expected_result, (result,expected_result)
    assert trace == ['enter','leave'] and depth == 0
    state = bytes(u.mem_read(ARENA,LENGTH))
    assert state == bytes(expected)
    return (result,state,tuple(trace)), seen


def compare_statistics(roots, addresses, include_dir=None):
    include_dirs = ([Path(include_dir)] if include_dir else [])+[R/'rebuild/include']
    inputs = {Path(__file__),EXE,R/'tools/decomp_pipeline/check_one.py',R/'tools/decomp_pipeline/gate_game_component_abi.py',R/'tools/decomp_pipeline/pe_oracle.py'}
    visited = set()
    def closure(path):
        if path in visited:
            return
        visited.add(path)
        inputs.add(path)
        for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(),re.M):
            q = next((q for q in [path.parent/name,*[inc/name for inc in include_dirs]] if q.is_file()),None)
            assert q,name
            closure(q)
    for root in roots:
        closure(root['source'])
    pins = {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
    codes = {}
    with tempfile.TemporaryDirectory(prefix='threaded-statistics-') as tmp:
        for index,root in enumerate(roots):
            obj = Path(tmp)/('provider%d.obj'%index)
            result = V.cl(['/nologo','/c','/O2','/MT',*['/I'+str(p) for p in include_dirs],'/Fo'+str(obj),str(root['source'])],V.env(),Path(tmp))
            assert result.returncode == 0,result.stdout+result.stderr
            c = Coff(obj)
            section,offset = c.symbols[root['symbol']]
            assert offset == 0
            body,relocations = c.sections[section]
            assert len(body) == root['size'] and len(relocations) == 2
            patched = bytearray(body)
            for site,(symbol,kind) in relocations.items():
                assert kind == 6 and struct.unpack_from('<I',body,site)[0] == 0
                struct.pack_into('<I',patched,site,addresses[symbol])
            codes[root['address']] = bytes(patched)
    raw = EXE.read_bytes()
    native = {r['address']:read_virtual_data(raw,r['address'],r['size']) for r in roots}
    assert codes == native
    cases = 0
    for root in roots:
        for args in itertools.product((0,1,3,8,17),('balanced','left','right'),range(7),range(5),(0,7),('none','enter','leave')):
            try:
                actual, seen = execute(codes,root,args)
                original, _ = execute(native,root,args)
                assert actual == original and root['address'] in seen
            except Exception as error:
                raise AssertionError((root['name'],args)) from error
            cases += 1
        print('statistics getter=%s cases=%d'%(root['name'],cases),flush=True)
        detected = False
        try:
            execute({a:b for a,b in codes.items() if a != root['address']},root,(3,'balanced',5,4,0,'none'))
        except Exception:
            detected = True
        assert detected,root['name']
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest() == h for p,h in pins.items())
    return dict(status='PASS',cases=cases,snapshots_per_mode=cases,missing_provider_checks=len(roots),input_sha256=pins,
                boundaries=['Win32 critical sections controlled; no OS lock or concurrent worker execution.',
                            'Empty/1/3/8/17-node balanced and one-sided ordered trees, statuses 0-4 and unknown, unsigned 32-bit sums.',
                            'Independent totals and full-storage/ABI model; entry/exit mutations test locking boundaries and value capture.'])

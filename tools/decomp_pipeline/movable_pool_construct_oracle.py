"""Differential execution of movable-pool construction and resource binding.

The base constructor is a controlled boundary; binding executes the existing
InternalSetResource provider on both sides. Check full pool/handle/header storage,
callee-saved registers, stack cleanup, return value and the base-constructor call.
"""
import hashlib
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
POOL, INFOS, HEADERS, STACK, STOP = 0x2000100, 0x2001000, 0x2002000, 0x3000000, 0x4000000
BASE, CTOR, SET, INTERNAL = 0xa02d20, 0xa62e20, 0xa62eb0, 0xa028b0
VTABLE = 0x129d6a0


def compile_codes(roots, addresses, build):
    """Patch only named COFF relocations using the gate's independently pinned map."""
    codes = {}
    objects = {}
    env = V.env()
    for root in roots:
        source = root['source']
        if source not in objects:
            obj = build / ('provider%d.obj' % len(objects))
            result = V.cl(['/nologo', '/c', '/O2', '/MT', '/W3',
                '/I'+str(R/'rebuild/include'), '/Fo'+str(obj), str(source)], env, build)
            assert result.returncode == 0, result.stdout + result.stderr
            objects[source] = Coff(obj)
        coff = objects[source]
        section, offset = coff.symbols[root['symbol']]
        assert offset == 0
        data, relocations = coff.sections[section]
        patched = bytearray(data)
        for site, (name, kind) in relocations.items():
            assert kind in (6, 20)
            addend = struct.unpack_from('<i', data, site)[0]
            target = addresses[name] + addend
            if kind == 20:
                target -= root['address'] + site + 4
            struct.pack_into('<I', patched, site, target & 0xffffffff)
        codes[root['address']] = bytes(patched)
    return codes


def execute(codes, entry, seed, selected=0, resource=0):
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, size in ((0xa02000, 0x2000), (0xa62000, 0x1000),
                          (0x2000000, 0x4000), (STACK, 0x10000), (STOP, 0x1000)):
        u.mem_map(address, size)
    def put(address, value):
        u.mem_write(address, struct.pack('<I', value & 0xffffffff))
    def get(address):
        return struct.unpack('<I', u.mem_read(address, 4))[0]
    for address, data in codes.items():
        u.mem_write(address, data)
    initial = bytes((i*37 + seed*19) & 255 for i in range(0x4000))
    u.mem_write(0x2000000, initial)
    base_result = bytes((i*13 + seed*41) & 255 for i in range(0x464))
    trace = []
    u.mem_write(BASE, b'\xc3')
    def hook(uc, address, size, user):
        if address != BASE:
            return
        assert uc.reg_read(UC_X86_REG_ECX) == POOL
        trace.append(('base_constructor', POOL))
        uc.mem_write(POOL, base_result)
        uc.reg_write(UC_X86_REG_EAX, POOL)
        uc.reg_write(UC_X86_REG_ECX, 0xdeadbeef)
        uc.reg_write(UC_X86_REG_EDX, 0x87654321)
    u.hook_add(UC_HOOK_CODE, hook)
    args = [] if entry == CTOR else [selected, resource]
    if args:
        put(POOL+0x438, INFOS)
        for i in range(4):
            put(INFOS+4*i, HEADERS+0x20*i)
    before = bytes(u.mem_read(0x2000000, 0x4000))
    sp = STACK + 0x8000
    put(sp, STOP)
    for i, arg in enumerate(args):
        put(sp+4+4*i, arg)
    saved = (UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP)
    for i, reg in enumerate(saved):
        u.reg_write(reg, 0x55660000+i+seed)
    u.reg_write(UC_X86_REG_ESP, sp)
    u.reg_write(UC_X86_REG_ECX, POOL)
    u.emu_start(entry, STOP, count=100)
    assert u.reg_read(UC_X86_REG_EIP) == STOP
    assert u.reg_read(UC_X86_REG_ESP) == sp+4+4*len(args)
    assert all(u.reg_read(reg) == 0x55660000+i+seed for i, reg in enumerate(saved))
    expected = bytearray(before)
    if entry == CTOR:
        expected[0x100:0x564] = base_result
        struct.pack_into('<I', expected, 0x100, VTABLE)
        assert trace == [('base_constructor', POOL)]
        assert u.reg_read(UC_X86_REG_EAX) == POOL
    else:
        struct.pack_into('<I', expected, 0x2000+selected*0x20, resource)
        assert trace == []
    actual = bytes(u.mem_read(0x2000000, 0x4000))
    assert actual == bytes(expected), (hex(entry), seed, selected, resource)
    return actual, trace


def execute_destruct(codes, seed, flags):
    base, entry, delete = 0xa02db0, 0xa62f90, 0xbfe9bc
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, length in ((0xa02000, 0x2000), (0xa62000, 0x1000),
                            (0xbfe000, 0x1000), (0x2000000, 0x4000),
                            (STACK, 0x10000), (STOP, 0x1000)):
        u.mem_map(address, length)
    def put(address, value):
        u.mem_write(address, struct.pack('<I', value & 0xffffffff))
    def get(address):
        return struct.unpack('<I', u.mem_read(address, 4))[0]
    for address, data in codes.items():
        u.mem_write(address, data)
    for address in (base, delete):
        u.mem_write(address, b'\xc3')
    initial = bytes((i*37+seed*19) & 255 for i in range(0x4000))
    u.mem_write(0x2000000, initial)
    after_base = bytes((i*13+seed*41) & 255 for i in range(0x464))
    trace = []
    def hook(uc, address, length, user):
        if address not in (base, delete):
            return
        if address == base:
            assert uc.reg_read(UC_X86_REG_ECX) == POOL
            # An implicit destructor must not reset the derived vptr first.
            assert bytes(uc.mem_read(0x2000000, 0x4000)) == initial
            trace.append(('base_destructor', POOL))
            uc.mem_write(POOL, after_base)
        else:
            assert get(uc.reg_read(UC_X86_REG_ESP)+4) == POOL
            assert bytes(uc.mem_read(POOL, 0x464)) == after_base
            trace.append(('delete', POOL))
        for reg, value in ((UC_X86_REG_EAX, 0xfedcba98),
                           (UC_X86_REG_ECX, 0xdeadbeef), (UC_X86_REG_EDX, 0x87654321)):
            uc.reg_write(reg, value)
    u.hook_add(UC_HOOK_CODE, hook)
    sp = STACK+0x8000
    put(sp, STOP)
    put(sp+4, flags)
    saved = (UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP)
    for i, reg in enumerate(saved):
        u.reg_write(reg, 0x44550000+i+seed)
    u.reg_write(UC_X86_REG_ESP, sp)
    u.reg_write(UC_X86_REG_ECX, POOL)
    u.emu_start(entry, STOP, count=100)
    assert u.reg_read(UC_X86_REG_EIP) == STOP
    assert u.reg_read(UC_X86_REG_ESP) == sp+8
    assert all(u.reg_read(reg) == 0x44550000+i+seed for i, reg in enumerate(saved))
    assert u.reg_read(UC_X86_REG_EAX) == POOL
    assert trace == [('base_destructor', POOL)] + ([('delete', POOL)] if flags & 1 else [])
    expected = bytearray(initial)
    expected[0x100:0x564] = after_base
    actual = bytes(u.mem_read(0x2000000, 0x4000))
    assert actual == bytes(expected)
    return actual, trace


def compare(roots, addresses):
    source = R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_InternalSetResource_00a028b0.cpp'
    helper = dict(source=source, symbol='?InternalSetResource@CHandleBasedMemoryPool@@QAEXJPAX@Z', address=INTERNAL)
    all_roots = roots + [helper]
    pins = {str(p): hashlib.sha256(p.read_bytes()).hexdigest()
            for p in {Path(__file__), source, *(r['source'] for r in roots)}}
    with tempfile.TemporaryDirectory(prefix='movable-pool-construct-behavior-') as tmp:
        candidate = compile_codes(all_roots, addresses, Path(tmp))
    raw = EXE.read_bytes()
    assert struct.unpack('<4I', read_virtual_data(raw, VTABLE, 16)) == (0xa62f90, 0xa62f30, 0xa62f70, 0xa62ec0)
    native = {a: read_virtual_data(raw, a, len(data)) for a, data in candidate.items()}
    cases = 0
    for seed in range(16):
        assert execute(candidate, CTOR, seed) == execute(native, CTOR, seed)
        cases += 1
        for flags in (0, 1, 2, 3, 0x80, 0x81, 0xfffffffe, 0xffffffff):
            assert execute_destruct(candidate, seed, flags) == execute_destruct(native, seed, flags)
            cases += 1
        for selected in range(4):
            for resource in (0, 1, 0x2020000, 0xffffffff):
                assert execute(candidate, SET, seed, selected, resource) == execute(native, SET, seed, selected, resource)
                cases += 1
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest() == digest for p, digest in pins.items())
    return dict(status='PASS', comparative_cases=cases, input_sha256=pins,
                boundary='Base constructor/destructor and delete controlled; InternalSetResource executes genuine compiled and retail providers.')


def execute_allocate(codes, entered, allocated, flags, size, selected, resource):
    begin, end, allocate, entry = 0xa02140, 0xa02170, 0xa03180, 0xa62e40
    out = 0x2003000
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, length in ((0xa02000, 0x2000), (0xa62000, 0x1000),
                            (0x2000000, 0x4000), (STACK, 0x10000), (STOP, 0x1000)):
        u.mem_map(address, length)
    def put(address, value):
        u.mem_write(address, struct.pack('<I', value & 0xffffffff))
    def get(address):
        return struct.unpack('<I', u.mem_read(address, 4))[0]
    for address, data in codes.items():
        u.mem_write(address, data)
    for address in (begin, end, allocate):
        u.mem_write(address, b'\xc3')
    u.mem_write(0x2000000, b'\xa5'*0x4000)
    put(POOL+0x438, INFOS)
    for i in range(4):
        put(INFOS+4*i, HEADERS+0x20*i)
    initial = bytes(u.mem_read(0x2000000, 0x4000))
    trace = []
    def hook(uc, address, length, user):
        if address == INTERNAL:
            sp = uc.reg_read(UC_X86_REG_ESP)
            assert uc.reg_read(UC_X86_REG_ECX) == POOL
            assert (get(sp+4), get(sp+8)) == (selected, resource)
            trace.append(('bind', selected, resource))
            return  # Execute the genuine provider body.
        if address not in (begin, end, allocate):
            return
        sp = uc.reg_read(UC_X86_REG_ESP)
        assert uc.reg_read(UC_X86_REG_ECX) == POOL
        if address == begin:
            assert get(sp+4) == flags & 1
            trace.append(('begin', flags & 1))
            result, pop = int(entered), 4
        elif address == end:
            trace.append(('end',))
            result, pop = 0x12345678, 0
        else:
            assert (get(sp+4), get(sp+8), get(sp+12)) == (out, size, flags)
            trace.append(('allocate', size, flags))
            # A failed allocation may have written the output: it must still not bind.
            put(out, selected)
            result, pop = int(allocated), 12
        # Boolean-returning boundaries only define AL; poison the upper bits.
        uc.reg_write(UC_X86_REG_EAX, result if address == end else 0xfedcba00 | result)
        uc.reg_write(UC_X86_REG_ECX, 0xdeadbeef)
        uc.reg_write(UC_X86_REG_EDX, 0x87654321)
        uc.reg_write(UC_X86_REG_EIP, get(sp))
        uc.reg_write(UC_X86_REG_ESP, sp+4+pop)
    u.hook_add(UC_HOOK_CODE, hook)
    sp = STACK+0x8000
    put(sp, STOP)
    for i, arg in enumerate((out, resource, size, flags)):
        put(sp+4+4*i, arg)
    saved = (UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP)
    for i, reg in enumerate(saved):
        u.reg_write(reg, 0x44550000+i)
    u.reg_write(UC_X86_REG_ESP, sp)
    u.reg_write(UC_X86_REG_ECX, POOL)
    u.emu_start(entry, STOP, count=150)
    assert u.reg_read(UC_X86_REG_EIP) == STOP
    assert u.reg_read(UC_X86_REG_ESP) == sp+20
    assert all(u.reg_read(reg) == 0x44550000+i for i, reg in enumerate(saved))
    expected = bytearray(initial)
    expected_trace = [('begin', flags & 1)]
    if entered:
        struct.pack_into('<I', expected, out-0x2000000, selected)
        expected_trace.append(('allocate', size, flags))
        if allocated:
            struct.pack_into('<I', expected, 0x2000+selected*0x20, resource)
            expected_trace.append(('bind', selected, resource))
        expected_trace.append(('end',))
    actual = bytes(u.mem_read(0x2000000, 0x4000))
    assert actual == bytes(expected)
    assert trace == expected_trace
    assert u.reg_read(UC_X86_REG_EAX) & 255 == int(entered and allocated)
    return actual, trace


def compare_allocate(roots, addresses):
    source = R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_InternalSetResource_00a028b0.cpp'
    helper = dict(source=source, symbol='?InternalSetResource@CHandleBasedMemoryPool@@QAEXJPAX@Z', address=INTERNAL)
    pins = {str(p): hashlib.sha256(p.read_bytes()).hexdigest() for p in
            {Path(__file__), source, *(r['source'] for r in roots)}}
    with tempfile.TemporaryDirectory(prefix='movable-pool-allocate-behavior-') as tmp:
        candidate = compile_codes(roots+[helper], addresses, Path(tmp))
    raw = EXE.read_bytes()
    native = {a: read_virtual_data(raw, a, len(data)) for a, data in candidate.items()}
    cases = 0
    for flags in (0, 1, 2, 3, 0x80, 0x81, 0xfffffffe, 0xffffffff):
        for entered in (False, True):
            if not entered and not flags & 1:
                continue  # Blocking acquisition cannot report failure.
            for allocated in (False, True):
                for size in (0, 1, 15, 16, 0x7fffffff, 0xffffffff):
                    for selected in range(4):
                        for resource in (0, 0x2020000, 0xffffffff):
                            args = (entered, allocated, flags, size, selected, resource)
                            assert execute_allocate(candidate, *args) == execute_allocate(native, *args)
                            cases += 1
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest() == digest for p, digest in pins.items())
    return dict(status='PASS', comparative_cases=cases, input_sha256=pins,
                boundary='BeginCritical, EndCritical and Allocate controlled; InternalSetResource executes genuine compiled and retail providers.')

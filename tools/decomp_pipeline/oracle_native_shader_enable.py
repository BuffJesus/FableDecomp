"""Retail shader enable sequences, executing the real capability/no-op callees."""
import itertools
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_ESP, UC_X86_REG_EIP
from pe_oracle import EXE, pe_sections

BASE = 0x24000000
WIDTH = 0x6000

def collect_enables():
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    uc.mem_map(0x400000, 0x1200000)
    uc.mem_map(BASE, 0x10000)
    raw = EXE.read_bytes()
    for _, va, _, offset, size in pe_sections(raw):
        if size:
            uc.mem_write(0x400000 + va, raw[offset:offset + size])
    inputs, expected = bytearray(), bytearray()
    cases = snapshots = 0
    stack, stop = BASE + 0xe000, BASE + 0xf000
    values = (0, 1, 2, 9, 10, 99, 127, 255)
    for major, minor, initial in itertools.product(values, values, (0, 1, 255)):
        arena = bytearray((i * 17 + cases * 13) & 255 for i in range(WIDTH))
        def put(o, v):
            struct.pack_into('<I', arena, o, v)
        put(0x1d0, BASE + 0x1000)
        put(0x4a3c, BASE + 0x5000)
        put(0x5128, (0xffff0000 if cases & 1 else 0) | major << 8 | minor)
        arena[0x390:0x393] = bytes((initial, initial, initial))
        inputs.extend(struct.pack('<II', 0, 0)); inputs.extend(arena)
        uc.mem_write(BASE, bytes(arena))
        # Repeated enables and interleaved disables test preserved state too.
        for op in (0, 1, 2, 0, 3, 4, 1, 2):
            address = (0x988110, 0x987fe0, 0x9880e0, 0x9bee00, 0xa0aa20)[op]
            receiver = BASE + (0x5000 if op == 3 else 0x1000 if op == 4 else 0)
            uc.mem_write(stack, struct.pack('<II', stop, initial))
            uc.reg_write(UC_X86_REG_ESP, stack)
            uc.reg_write(UC_X86_REG_ECX, receiver)
            uc.emu_start(address, stop, count=10000)
            assert uc.reg_read(UC_X86_REG_ESP) == stack + (8 if op == 4 else 4)
            result = uc.reg_read(UC_X86_REG_EAX) if op == 3 else 0
            expected.extend(struct.pack('<II', op, result))
            expected.extend(uc.mem_read(BASE, WIDTH))
            snapshots += 1
        cases += 1
    return inputs, expected, dict(cases=cases, snapshots=snapshots)

def collect_shutdown():
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    uc.mem_map(0x400000, 0x1200000)
    uc.mem_map(BASE, 0x10000)
    raw = EXE.read_bytes()
    for _, va, _, offset, size in pe_sections(raw):
        if size:
            uc.mem_write(0x400000 + va, raw[offset:offset + size])
    inputs, expected = bytearray(), bytearray()
    cases = snapshots = mutation = 0
    stack, stop = BASE + 0xe000, BASE + 0xf000
    def word(a, v): uc.mem_write(a, struct.pack('<I', v & 0xffffffff))
    def read(a): return struct.unpack('<I', uc.mem_read(a, 4))[0]
    def event(kind, value=0):
        nonlocal snapshots
        expected.extend(struct.pack('<II', kind, value)); expected.extend(uc.mem_read(BASE, WIDTH))
        snapshots += 1
    def hook(uc, a, n, user):
        if a not in (BASE + 0xc000, BASE + 0xc100): return
        sp = uc.reg_read(UC_X86_REG_ESP)
        if a == BASE + 0xc000:
            assert read(sp + 4) == BASE + 0x900 and read(sp + 8) == 0
            event(10)
            if mutation == 1:
                word(BASE + 0x1e4, BASE + 0x840)
                word(BASE + 0x3d8, 0x1234567f)
            elif mutation == 2:
                word(BASE + 0x1e4, 0)
                uc.mem_write(BASE + 0x391, b'\xff')
            elif mutation == 3:
                word(BASE + 0x808, 1)
                word(BASE + 0x3d8, 0xfedcba9b)
            cleanup = 8
            result = 0x80004005 if mutation & 1 else 0
        else:
            receiver = uc.reg_read(UC_X86_REG_ECX)
            assert receiver in (BASE + 0x800, BASE + 0x840) and read(sp + 4) == 1
            event(11, receiver)
            if mutation:
                word(BASE + 0x1e4, BASE + 0x880)
                word(BASE + 0x3d8, 0x10203040)
            cleanup = 4; result = receiver
        uc.reg_write(UC_X86_REG_EIP, read(sp))
        uc.reg_write(UC_X86_REG_ESP, sp + 4 + cleanup)
        uc.reg_write(UC_X86_REG_EAX, result)
    uc.hook_add(UC_HOOK_CODE, hook)
    word(BASE + 0x8000 + 107 * 4, BASE + 0xc000)
    word(BASE + 0x9000, BASE + 0xc100)
    for enabled, count, mutation in itertools.product((0, 1, 255), (None, 0, 1, 2, 0xffffffff), range(4)):
        arena = bytearray((i * 17 + cases * 13) & 255 for i in range(WIDTH))
        def put(o, v): struct.pack_into('<I', arena, o, v)
        put(0x1d4, BASE + 0x900); put(0x900, BASE + 0x8000)
        put(0x800, BASE + 0x9000); put(0x840, BASE + 0x9000)
        put(0x1e4, BASE + 0x800 if count is not None else 0)
        put(0x808, count or 0); put(0x848, 1)
        put(0x3d8, (0, 1, 3, 0xffffffff)[mutation]); arena[0x391] = enabled
        inputs.extend(struct.pack('<II', 1, mutation)); inputs.extend(arena)
        uc.mem_write(BASE, bytes(arena))
        uc.mem_write(stack, struct.pack('<I', stop))
        uc.reg_write(UC_X86_REG_ESP, stack); uc.reg_write(UC_X86_REG_ECX, BASE)
        uc.emu_start(0x988190, stop, count=10000)
        assert uc.reg_read(UC_X86_REG_ESP) == stack + 4
        event(12); cases += 1
    return inputs, expected, dict(cases=cases, snapshots=snapshots)

def collect():
    i, e, r = collect_enables()
    si, se, sr = collect_shutdown()
    r.update(enable_cases=r['cases'], shutdown_cases=sr['cases'],
             cases=r['cases']+sr['cases'], snapshots=r['snapshots']+sr['snapshots'])
    return i + si, e + se, r

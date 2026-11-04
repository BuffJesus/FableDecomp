"""Compare PSC-manager array teardown with two explicit call boundaries."""
import itertools
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

ADDRESS = 0xaeca80
STRING_DTOR = 0x99eae0
ARRAY_FREE = 0xbfea14
MEM = 0x2000000
OBJECT = MEM
ENTRIES = MEM + 0x100
OTHER = MEM + 0x500
STACK = 0x3000000
STOP = 0x4000000

def execute(code, count, has_storage, mutation, initial):
    assert has_storage or count == 0
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    for base, size in ((0xaec000, 0x1000), (0x99e000, 0x1000),
                       (0xbfe000, 0x1000), (MEM, 0x1000),
                       (STACK, 0x10000), (STOP, 0x1000)):
        u.mem_map(base, size)
    u.mem_write(ADDRESS, bytes(code))
    u.mem_write(STRING_DTOR, b'\xc3')
    u.mem_write(ARRAY_FREE, b'\xc3')
    u.mem_write(MEM, bytes([initial]) * 0x1000)

    def put(addr, value):
        u.mem_write(addr, struct.pack('<I', value & 0xffffffff))

    def get(addr):
        return struct.unpack('<I', u.mem_read(addr, 4))[0]

    begin = ENTRIES if has_storage else 0
    put(OBJECT, begin)
    put(OBJECT + 4, begin + 16 * count if begin else 0)
    stack = STACK + 0x8000
    put(stack, STOP)
    u.reg_write(UC_X86_REG_ECX, OBJECT)
    u.reg_write(UC_X86_REG_ESP, stack)
    registers = (UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP)
    for i, reg in enumerate(registers):
        u.reg_write(reg, 0x123400 + i)
    trace = []

    def hook(uc, target, size, user):
        if target not in (STRING_DTOR, ARRAY_FREE):
            return
        sp = uc.reg_read(UC_X86_REG_ESP)
        if target == STRING_DTOR:
            trace.append(('string', uc.reg_read(UC_X86_REG_ECX)))
            if len(trace) == 1:
                if mutation == 1:
                    put(OBJECT, OTHER)
                elif mutation == 2:
                    put(OBJECT + 4, OTHER)
        else:
            trace.append(('free', get(sp + 4)))
        uc.reg_write(UC_X86_REG_EIP, get(sp))
        uc.reg_write(UC_X86_REG_ESP, sp + 4)
        uc.reg_write(UC_X86_REG_EAX, 0xbad0bad0)
        uc.reg_write(UC_X86_REG_ECX, 0xbad1bad1)
        uc.reg_write(UC_X86_REG_EDX, 0xbad2bad2)

    u.hook_add(UC_HOOK_CODE, hook)
    u.emu_start(ADDRESS, STOP, count=300)
    assert u.reg_read(UC_X86_REG_EIP) == STOP
    assert u.reg_read(UC_X86_REG_ESP) == stack + 4
    assert all(u.reg_read(r) == 0x123400 + i for i, r in enumerate(registers))
    expected = []
    for i in range(count):
        expected.extend((('string', ENTRIES + i * 16 + 12),
                         ('string', ENTRIES + i * 16)))
    if has_storage:
        expected.append(('free', OTHER if count and mutation == 1 else ENTRIES))
    assert trace == expected, (count, has_storage, mutation, initial, trace, expected)
    return bytes(u.mem_read(MEM, 0x600)), tuple(trace)

def compare(candidate, native):
    cases = 0
    for count, has_storage, mutation, initial in itertools.product(
            range(5), range(2), range(3), (0, 0xa5, 0xff)):
        if count and not has_storage:
            continue
        assert execute(candidate, count, has_storage, mutation, initial) == (
            execute(native, count, has_storage, mutation, initial))
        cases += 1
    return cases

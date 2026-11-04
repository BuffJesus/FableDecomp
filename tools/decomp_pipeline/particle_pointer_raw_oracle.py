"""Execute both raw counted-pointer constructors with an allocation boundary."""
import itertools
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

ADDRESS = 0x41956c
DELETE_DATA = 0x419086
ALLOC = 0xbfea1a
MEM = 0x2000000
DEST = MEM
INFO = MEM + 0x100
DATA = (0, MEM + 0x500, MEM + 0x540)
STACK = 0x3000000
STOP = 0x4000000

def execute(code, data, succeeds, initial):
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    for base, size in ((0x419000, 0x1000), (0xbfe000, 0x1000),
                       (MEM, 0x1000), (STACK, 0x10000), (STOP, 0x1000)):
        u.mem_map(base, size)
    u.mem_write(ADDRESS, bytes(code))
    u.mem_write(ALLOC, b'\xc3')
    u.mem_write(MEM, bytes([initial]) * 0x1000)

    def put(addr, value):
        u.mem_write(addr, struct.pack('<I', value & 0xffffffff))

    def get(addr):
        return struct.unpack('<I', u.mem_read(addr, 4))[0]

    stack = STACK + 0x8000
    put(stack, STOP)
    put(stack + 4, DATA[data])
    u.reg_write(UC_X86_REG_ECX, DEST)
    u.reg_write(UC_X86_REG_ESP, stack)
    registers = (UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP)
    for i, reg in enumerate(registers):
        u.reg_write(reg, 0x123400 + i)
    calls = []

    def hook(uc, address, size, user):
        if address != ALLOC:
            return
        sp = uc.reg_read(UC_X86_REG_ESP)
        calls.append(get(sp + 4))
        uc.reg_write(UC_X86_REG_EIP, get(sp))
        uc.reg_write(UC_X86_REG_ESP, sp + 4)
        uc.reg_write(UC_X86_REG_EAX, INFO if succeeds else 0)
        uc.reg_write(UC_X86_REG_ECX, 0xbad1bad1)
        uc.reg_write(UC_X86_REG_EDX, 0xbad2bad2)

    u.hook_add(UC_HOOK_CODE, hook)
    u.emu_start(ADDRESS, STOP, count=200)
    assert u.reg_read(UC_X86_REG_EIP) == STOP
    assert u.reg_read(UC_X86_REG_ESP) == stack + 8
    assert u.reg_read(UC_X86_REG_EAX) == DEST
    assert all(u.reg_read(r) == 0x123400 + i for i, r in enumerate(registers))
    assert calls == ([12] if data else [])
    assert get(DEST) == DATA[data]
    assert get(DEST + 4) == (INFO if data and succeeds else 0)
    if data and succeeds:
        assert (get(INFO), get(INFO + 4), get(INFO + 8)) == (1, DELETE_DATA, DATA[data])
    memory = bytes(u.mem_read(MEM, 0x600))
    return memory, tuple(calls)

def compare(candidate, retail):
    cases = 0
    for data, succeeds, initial in itertools.product(range(3), range(2), (0, 0xa5, 0xff)):
        assert execute(candidate, data, succeeds, initial) == execute(retail, data, succeeds, initial)
        cases += 1
    return cases

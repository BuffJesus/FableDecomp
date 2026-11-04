"""Compare the three Reset bodies with modeled allocator and delete boundaries."""
import itertools
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

MEM = 0x2000000
DEST = MEM
OLD_INFO = MEM + 0x100
NEW_INFO = MEM + 0x200
OTHER_INFO = MEM + 0x300
OLD_DATA = MEM + 0x500
NEW_DATA = (0, MEM + 0x540, MEM + 0x580)
CALLBACK = MEM + 0x800
STACK = 0x3000000
STOP = 0x4000000
NEW = 0xbfea1a
FREE = 0xbfe9bc
SPECS = ((0xaeccc0, 0xaecca0), (0xaecac0, 0xaeca50), (0xaecc30, 0xaecbf0))

def execute(code, address, delete_data, old_present, count, data_selector, allocates, mode):
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    for base, size in ((0xaec000, 0x1000), (0xbfe000, 0x1000),
                       (MEM, 0x1000), (STACK, 0x10000), (STOP, 0x1000)):
        u.mem_map(base, size)
    u.mem_write(address, bytes(code))
    u.mem_write(NEW, b'\xc3')
    u.mem_write(FREE, b'\xc3')
    u.mem_write(CALLBACK, b'\xc3')
    u.mem_write(MEM, b'\xa5' * 0x1000)

    def put(addr, value):
        u.mem_write(addr, struct.pack('<I', value & 0xffffffff))

    def get(addr):
        return struct.unpack('<I', u.mem_read(addr, 4))[0]

    put(DEST, OLD_DATA)
    put(DEST + 4, OLD_INFO if old_present else 0)
    put(OLD_INFO, count)
    put(OLD_INFO + 4, CALLBACK)
    put(OLD_INFO + 8, OLD_DATA)
    put(OTHER_INFO, 17)
    put(OTHER_INFO + 4, CALLBACK)
    put(OTHER_INFO + 8, OLD_DATA)
    stack = STACK + 0x8000
    put(stack, STOP)
    put(stack + 4, NEW_DATA[data_selector])
    u.reg_write(UC_X86_REG_ECX, DEST)
    u.reg_write(UC_X86_REG_ESP, stack)
    registers = (UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP)
    for i, reg in enumerate(registers):
        u.reg_write(reg, 0x123400 + i)
    trace = []

    def hook(uc, target, size, user):
        if target not in (CALLBACK, NEW, FREE):
            return
        sp = uc.reg_read(UC_X86_REG_ESP)
        if target == CALLBACK:
            trace.append(('callback', uc.reg_read(UC_X86_REG_ECX)))
            if mode:
                put(DEST, OLD_DATA + 4)
                put(DEST + 4, OTHER_INFO)
        elif target == NEW:
            trace.append(('new', get(sp + 4)))
        else:
            trace.append(('free', get(sp + 4)))
        uc.reg_write(UC_X86_REG_EIP, get(sp))
        uc.reg_write(UC_X86_REG_ESP, sp + 4)
        uc.reg_write(UC_X86_REG_EAX, (NEW_INFO if allocates else 0) if target == NEW else 0xbad0bad0)
        uc.reg_write(UC_X86_REG_ECX, 0xbad1bad1)
        uc.reg_write(UC_X86_REG_EDX, 0xbad2bad2)

    u.hook_add(UC_HOOK_CODE, hook)
    u.emu_start(address, STOP, count=300)
    assert u.reg_read(UC_X86_REG_EIP) == STOP
    assert u.reg_read(UC_X86_REG_ESP) == stack + 8
    assert all(u.reg_read(r) == 0x123400 + i for i, r in enumerate(registers))
    expected = []
    if old_present and count == 1:
        expected.append(('callback', OLD_DATA))
        expected.append(('free', OTHER_INFO if mode else OLD_INFO))
    if data_selector:
        expected.append(('new', 12))
    assert trace == expected, (address, old_present, count, data_selector, allocates, mode, trace, expected)
    assert get(DEST) == NEW_DATA[data_selector]
    assert get(DEST + 4) == (NEW_INFO if data_selector and allocates else 0)
    if old_present:
        assert get(OLD_INFO) == (count - 1) & 0xffffffff
    else:
        assert get(OLD_INFO) == count
    if data_selector and allocates:
        assert (get(NEW_INFO), get(NEW_INFO + 4), get(NEW_INFO + 8)) == (
            1, delete_data, NEW_DATA[data_selector])
    return bytes(u.mem_read(MEM, 0x900)), tuple(trace)

def compare(candidate, native):
    cases = 0
    for address, delete_data in SPECS:
        for old, count, data, allocates, mode in itertools.product(
                range(2), (0, 1, 2, 0xffffffff), range(3), range(2), range(2)):
            assert execute(candidate[address], address, delete_data, old, count, data, allocates, mode) == (
                execute(native[address], address, delete_data, old, count, data, allocates, mode))
            cases += 1
    return cases

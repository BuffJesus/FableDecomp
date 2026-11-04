"""Compare constructor field writes and three call-boundary identities."""
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

ADDRESS = 0xae8bf0
MAP_CTOR = 0x9e5200
STRING_CTOR = 0x99e4b0
MEM = 0x2000000
OBJECT = MEM
STACK = 0x3000000
STOP = 0x4000000

def execute(code, initial):
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    for base, size in ((0xae8000, 0x1000), (0x9e5000, 0x1000),
                       (0x99e000, 0x1000), (MEM, 0x1000),
                       (STACK, 0x10000), (STOP, 0x1000)):
        u.mem_map(base, size)
    u.mem_write(ADDRESS, bytes(code))
    u.mem_write(MAP_CTOR, b'\xc3')
    u.mem_write(STRING_CTOR, b'\xc3')
    u.mem_write(MEM, bytes([initial]) * 0x1000)

    def put(addr, value):
        u.mem_write(addr, struct.pack('<I', value & 0xffffffff))

    def get(addr):
        return struct.unpack('<I', u.mem_read(addr, 4))[0]

    stack = STACK + 0x8000
    put(stack, STOP)
    u.reg_write(UC_X86_REG_ECX, OBJECT)
    u.reg_write(UC_X86_REG_ESP, stack)
    registers = (UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP)
    for i, reg in enumerate(registers):
        u.reg_write(reg, 0x123400 + i)
    trace = []

    def hook(uc, target, size, user):
        if target not in (MAP_CTOR, STRING_CTOR):
            return
        sp = uc.reg_read(UC_X86_REG_ESP)
        receiver = uc.reg_read(UC_X86_REG_ECX)
        trace.append(('map' if target == MAP_CTOR else 'string', receiver))
        if target == STRING_CTOR:
            put(receiver, 0)
        uc.reg_write(UC_X86_REG_EIP, get(sp))
        uc.reg_write(UC_X86_REG_ESP, sp + 4)
        uc.reg_write(UC_X86_REG_EAX, receiver)
        uc.reg_write(UC_X86_REG_ECX, 0xbad1bad1)
        uc.reg_write(UC_X86_REG_EDX, 0xbad2bad2)

    u.hook_add(UC_HOOK_CODE, hook)
    u.emu_start(ADDRESS, STOP, count=100)
    assert u.reg_read(UC_X86_REG_EIP) == STOP
    assert u.reg_read(UC_X86_REG_ESP) == stack + 4
    assert u.reg_read(UC_X86_REG_EAX) == OBJECT
    assert all(u.reg_read(r) == 0x123400 + i for i, r in enumerate(registers))
    assert trace == [('map', OBJECT), ('string', OBJECT + 0x44), ('string', OBJECT + 0x48)]
    for offset in (0x28, 0x2c, 0x30, 0x38, 0x3c, 0x44, 0x48):
        assert get(OBJECT + offset) == 0
    assert u.mem_read(OBJECT + 0x35, 1) == b'\0'
    assert get(OBJECT + 0x40) == 1
    return bytes(u.mem_read(MEM, 0x100)), tuple(trace)

def compare(candidate, native):
    cases = 0
    for initial in (0, 1, 0x5a, 0xa5, 0xff):
        assert execute(candidate, initial) == execute(native, initial)
        cases += 1
    return cases

"""Native string-to-integer semantics survive the readable conversion."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_evidence_lowering import finish_lua
from tools.script_recovery.build_readable_unit import readable_file
from tools.script_recovery.lift_native_lua import RData


@pytest.fixture(scope='module')
def native_parser():
    import struct
    import unicorn as uc
    from unicorn import x86_const as x86
    machine = uc.Uc(uc.UC_ARCH_X86, uc.UC_MODE_32)
    for address in (0x99e000, 0xbfe000, 0x200000, 0x300000, 0x400000):
        machine.mem_map(address, 4096)
    machine.mem_write(0x99e7f0, RData().bytes_at(0x99e7f0, 0x7a))
    machine.mem_write(0xbfeda6, b'\xc3')
    def classify(vm, address, _size, _data):
        if address == 0xbfeda6:
            argument = struct.unpack('<I', vm.mem_read(vm.reg_read(x86.UC_X86_REG_ESP)+4, 4))[0]
            # The external CRT isdigit call, for these ASCII test operands.
            vm.reg_write(x86.UC_X86_REG_EAX, int(48 <= argument <= 57))
    machine.hook_add(uc.UC_HOOK_CODE, classify)
    def run(text):
        data = text.encode('ascii')
        machine.mem_write(0x200000, struct.pack('<I', 0x200020))
        machine.mem_write(0x200020, struct.pack('<II', 0x200100, len(data)))
        machine.mem_write(0x200100, data+b'\0')
        machine.mem_write(0x300f00, struct.pack('<I', 0x400000))
        machine.reg_write(x86.UC_X86_REG_ESP, 0x300f00)
        machine.reg_write(x86.UC_X86_REG_ECX, 0x200000)
        machine.emu_start(0x99e7f0, 0x400000, count=20000)
        assert machine.reg_read(x86.UC_X86_REG_EIP) == 0x400000
        result = machine.reg_read(x86.UC_X86_REG_EAX)
        return result if result < 2**31 else result - 2**32
    return run


@pytest.mark.parametrize('text,expected', [
    ('', 0), ('nothing', 0), ('0', 0), ('15', 15), (' 12 ', 12),
    ('-12', -12), ('1-2', -12), ('a1b2', 12), ('--12', -12),
    ('12.9', 12), ('.9', 0), ('12.-3', 12), ('0x10', 10),
    ('2147483647', 2147483647), ('2147483648', -2147483648),
    ('4294967295', -1), ('4294967296', 0), ('-4294967295', 1),
    ('12345678901234567890', -350287150),
])
@pytest.mark.parametrize('readable', [False, True])
def test_parser_matches_decimal_scan_and_signed_overflow(text, expected, readable, native_parser):
    source = finish_lua('function Parse(value)\n    return GFCharStringToInt(value)\nend\n')
    if readable:
        source = readable_file(source)[0]
    lua = LuaRuntime()
    lua.execute(source)
    assert lua.globals().Parse(text) == expected
    assert native_parser(text) == expected


def test_parser_does_not_shadow_an_existing_local_and_reads_argument_once():
    source = finish_lua('function Parse(nextText)\n    local parseGameInteger = 7\n'
                        '    return GFCharStringToInt(nextText()) + parseGameInteger\nend\n')
    lua = LuaRuntime()
    calls = []
    def text():
        calls.append(True)
        return '3'
    lua.execute(source)
    assert lua.globals().Parse(text) == 10
    assert calls == [True]

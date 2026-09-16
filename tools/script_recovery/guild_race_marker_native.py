"""Execute the complete retail RaceMarker Main with engine calls as test boundaries."""
import struct

from tools.script_recovery.rock_state_native import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn import UC_HOOK_MEM_WRITE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_ESP, UC_X86_REG_EIP
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.guild_race_marker import verify


def execute(near=(False,), cancel=1, initial=False, raw_true=1):
    data = RData()
    verify(data)
    machine = Uc(UC_ARCH_X86, UC_MODE_32)
    for page in (0xD40000, 0xF35000, 0xCBE000):
        machine.mem_map(page, 0x10000 if page == 0xD40000 else 0x1000)
    machine.mem_map(0x200000, 0x20000)
    entity, parent, game, table, hero, stack, stop = 0x201000, 0x202000, 0x203000, 0x204000, 0x205000, 0x21F000, 0x206000
    get_hero, remove_marker, frame = 0x206010, 0x206020, 0x206030
    machine.mem_write(0xD40AA0, data.bytes_at(0xD40AA0, 0x61))

    def put(address, value):
        machine.mem_write(address, struct.pack('<I', value))

    def read(address):
        return struct.unpack('<I', machine.mem_read(address, 4))[0]

    put(entity+4, game)
    put(entity+0x14, parent)
    put(game, table)
    for slot, callback in ((0x118, get_hero), (0x580, remove_marker), (0x1C, frame)):
        put(table+slot, callback)
    machine.mem_write(parent+0x4C, bytes((0xA5, int(initial), 0x5A)))
    put(stack, stop)
    machine.reg_write(UC_X86_REG_ESP, stack)
    machine.reg_write(UC_X86_REG_ECX, entity)
    trace = []
    queries = distances = 0

    def hook(uc, pc, size, user):
        nonlocal queries, distances
        esp = uc.reg_read(UC_X86_REG_ESP)
        result, pop = 0, None
        if pc == 0xF35B30:
            assert uc.reg_read(UC_X86_REG_ECX) == entity
            queries += 1
            ending = queries >= cancel
            trace.append(('term', ending))
            result, pop = raw_true if ending else 0, 0
        elif pc == get_hero:
            assert uc.reg_read(UC_X86_REG_ECX) == game
            trace.append(('hero',))
            result, pop = hero, 0
        elif pc == 0xCBE2FF:
            assert (uc.reg_read(UC_X86_REG_ECX), uc.reg_read(UC_X86_REG_EDX)) == (hero, entity+8)
            assert read(esp+4) == 0x40400000
            close = near[distances % len(near)]
            distances += 1
            trace.append(('distance', close))
            result, pop = raw_true if close else 0, 4
        elif pc == remove_marker:
            assert uc.reg_read(UC_X86_REG_ECX) == game and read(esp+4) == entity+8
            trace.append(('remove',))
            pop = 4
        elif pc == frame:
            assert uc.reg_read(UC_X86_REG_ECX) == game
            trace.append(('frame',))
            pop = 0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX, result)
            uc.reg_write(UC_X86_REG_EIP, read(esp))
            uc.reg_write(UC_X86_REG_ESP, esp+4+pop)

    def write_hook(uc, access, address, size, value, user):
        if address == parent+0x4D:
            assert size == 1 and value == 1
            trace.append(('state', 'ReachedPlatform', True))

    machine.hook_add(UC_HOOK_CODE, hook)
    machine.hook_add(UC_HOOK_MEM_WRITE, write_hook)
    machine.emu_start(0xD40AA0, stop, count=10000)
    assert machine.reg_read(UC_X86_REG_EIP) == stop and machine.reg_read(UC_X86_REG_ESP) == stack+4
    assert machine.mem_read(parent+0x4C, 1) == b'\xA5' and machine.mem_read(parent+0x4E, 1) == b'\x5A'
    return bool(machine.mem_read(parent+0x4D, 1)[0]), trace

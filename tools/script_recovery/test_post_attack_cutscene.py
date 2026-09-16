"""Run the original post-attack movie caller; engine callees are ABI doubles."""
import hashlib
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_ESI, UC_X86_REG_EAX
from tools.script_recovery.lift_native_lua import RData


def native_trace(hero, acquired):
    data = RData()
    raw = data.bytes_at(0xdbeb20, 1095)
    assert hashlib.sha256(raw).hexdigest() == '8dc72f018eb5e9a030d02a244b9b86416b83ae59c0864a3db40e0b5e9a3a13e7'
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    targets = (0x7e72a0, 0xcdbf70, 0x99ebf0, 0xcd3d2e, 0x8abd10,
               0x99eae0, 0x6e7b60, 0xcbfb7d, 0x6e7b80, 0xcdbfb0, 0x7e74d0)
    for page in sorted({a & ~0xfff for a in targets} | {0xdbe000, 0x100000, 0x200000, 0x201000}):
        uc.mem_map(page, 0x1000)
    uc.mem_write(0xdbeb20, raw)
    for a in targets: uc.mem_write(a, b'\xc3')
    thread, game, table, stack = 0x200000, 0x200100, 0x200200, 0x100800
    def put(a, v): uc.mem_write(a, int(v).to_bytes(4, 'little'))
    def get(a): return int.from_bytes(uc.mem_read(a, 4), 'little')
    put(thread + 0x40, game); put(game, table)
    apis = {0x201010: ('hero', 0), 0x201020: ('acquire', 3),
            0x201030: ('movie.start', 2), 0x201040: ('pause', 1), 0x201050: ('camera', 1)}
    for slot, target in zip((0x118, 0x20, 0x5c8, 0x5ec, 0x5cc), apis):
        put(table + slot, target); uc.mem_write(target, b'\xc3')
    events, keys = [], {}
    def hook(machine, address, size, user):
        esp, ecx = machine.reg_read(UC_X86_REG_ESP), machine.reg_read(UC_X86_REG_ECX)
        args = lambda n: [get(esp + 4 + 4*i) for i in range(n)]
        result, count = 0, 0
        if address == 0xdbedad:
            assert bytes(machine.mem_read(thread + 0x51, 1)) == b'\x01'
            events.append(('dad', True))
        if address == 0x7e72a0:
            assert ecx == stack + 40; events.append(('control.new',))
        elif address == 0xcdbf70:
            assert ecx == stack + 16; events.append(('map.new',))
        elif address == 0x99ebf0:
            pointer, length = args(2); assert length == 0xffffffff
            key = '' if data.bytes_at(pointer, 1) == b'\0' else data.string_at(pointer)
            keys[ecx] = key
            events.append(('key.new', key)); count, result = 2, ecx
        elif address == 0xcd3d2e:
            assert ecx == stack + 16 and keys[args(1)[0]] == 'HERO'
            count, result = 1, 0x200b00
        elif address == 0x8abd10:
            assert ecx == 0x200b00 and args(1) == [stack + 40]
            events.append(('actor', 'HERO')); count = 1
        elif address == 0x99eae0:
            events.append(('key.close', keys.pop(ecx)))
        elif address == 0x6e7b60:
            assert ecx == stack + 56; events.append(('movie.new',))
        elif address == 0xcbfb7d:
            assert keys[ecx] == 'CS_OAKVALEINTRO_HESDEADJIM'
            assert machine.reg_read(UC_X86_REG_EDX) == stack + 16
            assert args(4) == [0, 0, 0, 1]
            events.append(('macro', False, True)); count = 4
        elif address in (0x6e7b80, 0xcdbfb0, 0x7e74d0):
            offset, name = {0x6e7b80: (56, 'movie.close'), 0xcdbfb0: (16, 'map.close'), 0x7e74d0: (40, 'control.close')}[address]
            assert ecx == stack + offset; events.append((name,))
        elif address in apis:
            name, count = apis[address]; assert ecx == game
            if name == 'hero': result = hero; events.append(('hero', hero))
            elif name == 'acquire':
                assert args(3) == [hero, stack + 40, 4]
                result = acquired; events.append(('acquire', hero, 4))
            elif name == 'movie.start':
                key, movie = args(2); assert keys[key] == '' and movie == stack + 56
                events.append(('movie.start', ''))
            else: events.append((name, bool(args(1)[0])))
        else: return
        machine.reg_write(UC_X86_REG_EAX, result)
        machine.reg_write(UC_X86_REG_ESP, esp + 4 + count*4)
        machine.reg_write(UC_X86_REG_EIP, get(esp))
    uc.reg_write(UC_X86_REG_ESP, stack); uc.reg_write(UC_X86_REG_ESI, thread)
    uc.hook_add(UC_HOOK_CODE, hook); uc.emu_start(0xdbeda5, 0xdbeebc, count=300)
    assert uc.reg_read(UC_X86_REG_ESP) == stack and not keys
    return events


def lua_trace(hero, acquired, fail_macro=False, fail_close=False, source=None):
    lua = LuaRuntime(); phase = lua.execute(source or Path(__file__).with_name('post_attack_cutscene.lua').read_text())
    events = []; q, r = lua.table(), lua.table()
    q.SetStateBool = lambda _, key, value: events.append(('dad', value))
    q.FixMovieSequenceCamera = lambda _, value: events.append(('camera', value))
    def new(name, id): events.append((name,)); return id
    r.NewResource = lambda _: new('control.new', 40)
    def acquire(_, id, priority):
        assert id == 40; events.extend([('hero', hero), ('acquire', hero, priority)]); return acquired
    r.TryAcquirePostAttackHero = acquire
    r.NewActorMap = lambda _: new('map.new', 16)
    def actor(_, map, key, id):
        assert map == 16 and id == 40
        events.extend([('key.new', key), ('actor', key), ('key.close', key)])
    r.SetActor = actor
    def movie(_, key):
        events.extend([('movie.new',), ('key.new', key), ('movie.start', key), ('key.close', key)]); return 56
    r.StartMovie = movie
    r.Pause = lambda _, value: events.append(('pause', value))
    def macro(_, key, map, setup, skip):
        assert map == 16
        events.extend([('key.new', key), ('macro', setup, skip), ('key.close', key)])
    wrap = lua.eval('function(callback, message) return function(...) callback(...); if message then error(message, 0) end end end')
    r.RunMacro = wrap(macro, 'macro failure' if fail_macro else None)
    def close(name, id, expected):
        assert id == expected; events.append((name,))
    r.DestroyMovie = wrap(lambda _, id: close('movie.close', id, 56), 'close failure' if fail_close else None)
    r.DestroyActorMap = wrap(lambda _, id: close('map.close', id, 16), 'close failure' if fail_close else None)
    r.ReleaseResource = wrap(lambda _, id: close('control.close', id, 40), 'close failure' if fail_close else None)
    error = None
    try: phase(q, r)
    except Exception as exc: error = str(exc)
    return events, error


class PostAttackCutsceneTests(unittest.TestCase):
    def test_original_caller_including_failed_acquisition_and_empty_hero(self):
        for hero, acquired in itertools.product((0, 0x200c00), (0, 1)):
            events, error = lua_trace(hero, acquired)
            self.assertIsNone(error)
            self.assertEqual(events, native_trace(hero, acquired))

    def test_lua_error_cleanup_preserves_original_failure(self):
        for macro, close in ((True, False), (False, True), (True, True)):
            events, error = lua_trace(0, 0, macro, close)
            self.assertIn('macro failure' if macro else 'close failure', error)
            self.assertEqual(events[-5:], [('camera', False), ('pause', False),
                ('movie.close',), ('map.close',), ('control.close',)])


if __name__ == '__main__': unittest.main()

"""Compare original approach instructions with the structured Lua phase."""
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_control import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EAX, UC_X86_REG_EBP, UC_X86_REG_EBX


class Trace:
    def __init__(self, near, task, cancel):
        self.near, self.task, self.cancel = near, task, cancel
        self.events, self.counts = [], {}

    def call(self, name):
        self.events.append(name)
        i = self.counts.get(name, 0)
        self.counts[name] = i + 1
        if name == 'term': return i + 1 == self.cancel
        if name == 'near': return self.near[i % len(self.near)]
        if name == 'task': return self.task[i % len(self.task)]
        if name == 'hero': return 0 if i % 2 == 0 else 0x202800


def native(data, trace):
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    for address, size in ((0xdb9000, 0x3000), (0x99e000, 0x1000), (0x7e7000, 0x1000),
                          (0xcbe000, 0x1000), (0xf35000, 0x1000), (0x100000, 0x10000), (0x200000, 0x4000)):
        uc.mem_map(address, size)
    uc.mem_write(0xdb98e4, data.bytes_at(0xdb98e4, 0xdb99ca - 0xdb98e4))
    def put(a, v): uc.mem_write(a, v.to_bytes(4, 'little'))
    def get(a): return int.from_bytes(uc.mem_read(a, 4), 'little')
    stack, thread, actor, game, table = 0x108000, 0x200000, 0x200008, 0x201000, 0x202000
    put(thread + 4, game); put(game, table)
    put(table + 0x118, 0x203000); put(table + 0x1c, 0x203010)
    calls = {0xf35b30: 'term', 0x99ebf0: 'construct', 0x99eae0: 'destroy',
             0x7e73e0: 'skip', 0x7e7450: 'task', 0xcbe2ff: 'near', 0x203000: 'hero', 0x203010: 'frame'}
    finished = []
    last_hero = [None]
    def hook(machine, address, size, user):
        if address in (0xdb99ca, 0xdbb2e8):
            finished.append(address == 0xdb99ca); machine.emu_stop(); return
        if address not in calls: return
        name = calls[address]
        esp = machine.reg_read(UC_X86_REG_ESP)
        ecx = machine.reg_read(UC_X86_REG_ECX)
        args = lambda n: [get(esp + 4 + 4*i) for i in range(n)]
        count = 0
        if name == 'term': assert ecx == thread
        elif name in ('hero', 'frame'): assert ecx == game
        elif name in ('construct', 'destroy'):
            assert ecx == stack + 188
            if name == 'construct':
                assert args(2) == [0x12d9928, 0xffffffff]; count = 2
        elif name == 'skip':
            assert ecx == stack + 24 and args(7) == [stack + 188, 1, 0, 0, 1, 0, 0]; count = 7
        elif name == 'task': assert ecx == stack + 24
        elif name == 'near':
            assert ecx == actor and machine.reg_read(UC_X86_REG_EDX) == last_hero[0]
            assert args(1) == [0x40a00000]; count = 1
        result = trace.call(name)
        if name == 'hero': last_hero[0] = result
        value = result if name == 'hero' else 0xabcd0000 | int(bool(result))
        machine.reg_write(UC_X86_REG_EAX, value)
        machine.reg_write(UC_X86_REG_ESP, esp + 4 + count*4)
        machine.reg_write(UC_X86_REG_EIP, get(esp))
    for reg, value in ((UC_X86_REG_ESP, stack), (UC_X86_REG_EBP, thread), (UC_X86_REG_EBX, actor)):
        uc.reg_write(reg, value)
    uc.hook_add(UC_HOOK_CODE, hook)
    uc.emu_start(0xdb98e4, 0xdb99ca + 1, count=5000)
    assert len(finished) == 1 and uc.reg_read(UC_X86_REG_ESP) == stack
    return finished[0]


def lua_phase(trace):
    lua = LuaRuntime()
    run = lua.execute(Path(__file__).with_name('theresa_approach_body.lua').read_text())
    quest, resources = lua.table(), lua.table()
    quest.IsActiveThreadTerminating = lambda self: trace.call('term')
    quest.NewScriptFrame = lambda self, me: trace.call('frame')
    def skip(self, control):
        assert control == 24
        for name in ('construct', 'skip', 'destroy'): trace.call(name)
    def near(self, me, distance):
        assert me == 0x200008 and distance == 5.0
        trace.call('hero')
        return trace.call('near')
    resources.PlayTheresaSkip = skip
    resources.IsTheresaNearHero = near
    resources.IsPerformingScriptTask = lambda self, control: trace.call('task')
    return run(quest, 0x200008, resources, 24)


class TheresaApproachTests(unittest.TestCase):
    def test_native_event_order_and_cancellation(self):
        data = RData(); verify(data)
        patterns = ((True,), (False,), (False, True), (True, False, False, True))
        for near, task, cancel in itertools.product(patterns, patterns, range(1, 13)):
            with self.subTest(near=near, task=task, cancel=cancel):
                expected, actual = Trace(near, task, cancel), Trace(near, task, cancel)
                self.assertEqual(lua_phase(actual), native(data, expected))
                self.assertEqual(actual.events, expected.events)

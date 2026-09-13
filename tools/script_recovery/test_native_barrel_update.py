import copy
import struct
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_colours as fixtures
from tools.script_recovery.native_colours import recover_barrel_colours
from tools.script_recovery.native_barrel_update import recover_barrel_update
from tools.script_recovery.lift_native_lua import Lifter, RData, strip_declarations


class BarrelUpdateTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.NativeColourTests().inputs()
        return fn, recover_barrel_colours(fn, source, RData(), manifest)[0], manifest

    def test_actual_timer_then_bar_update_with_float_binding(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_barrel_update(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual(evidence[0]['defaults'], [-1.0, -1.0])
        statements = strip_declarations(recovered)
        start = next(i for i, line in enumerate(statements) if 'native_bar_timer_value = ' in line)
        native = '{\n' + '\n'.join(statements[start:start + 2]) + '\n}'
        lifter = Lifter(manifest, {'0x108': ('WatchTimer', 'Int'), '0x60': ('GUIBarrelCounter', 'Int')},
                        'quest', False, '', RData())
        body = '\n'.join(lifter.lift('Update', native))
        self.assertEqual(lifter.todo, [])
        for value, expected_float in ((-1, -1.0), (0, 0.0), (45, 45.0),
                                      (16777217, 16777216.0), (2147483647, 2147483648.0)):
            lua, events = LuaRuntime(), []
            bar = [17]
            def state(_q, key):
                events.append(('state', key))
                return 42 if key == 'WatchTimer' else bar[0]
            def timer(_q, timer_id):
                events.append(('timer', timer_id))
                bar[0] = 18
                return value
            def update(_q, bar_id, current, maximum, scale):
                # Model the inspected Forge float parameter conversion. Lua
                # itself intentionally carries the exact signed timer integer.
                self.assertEqual(current, value)
                current = struct.unpack('<f', struct.pack('<f', current))[0]
                events.append(('update', bar_id, current, maximum, scale))
            quest = lua.table_from({'GetStateInt': state, 'GetTimer': timer, 'UpdateQuestInfoBar': update})
            lua.execute('return function(quest)\n' + body + '\nend')(quest)
            self.assertEqual(events, [('state', 'WatchTimer'), ('timer', 42), ('state', 'GUIBarrelCounter'),
                                      ('update', 18, expected_float, -1.0, -1.0)])

    def test_changed_body_source_or_float_contract_rejects(self):
        fn, source, manifest = self.inputs()
        self.assertEqual(recover_barrel_update(fn, source, SimpleNamespace(bytes_at=lambda *_: None), manifest)[1][0]['status'], 'rejected')
        self.assertEqual(recover_barrel_update(fn, source.replace('0xbf800000', '0x3f800000'), RData(), manifest)[1][0]['status'], 'rejected')
        changed = copy.deepcopy(manifest)
        changed['UpdateQuestInfoBar']['parameters'][1]['type'] = 'double'
        self.assertEqual(recover_barrel_update(fn, source, RData(), changed)[1][0]['status'], 'rejected')

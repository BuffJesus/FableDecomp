import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_entry_timer as fixtures
from tools.script_recovery.native_loop_timer import recover_loop_timer
from tools.script_recovery.lift_native_lua import Lifter, RData, strip_declarations


class LoopTimerTests(unittest.TestCase):
    def inputs(self):
        fixture = fixtures.EntryTimerTests()
        fn, source, manifest = fixture.inputs()
        source = fixture.recover(fn, source, manifest)[0]
        return fn, source, manifest

    def test_actual_wait_queries_current_timer_after_each_frame(self):
        fn, source, manifest = self.inputs()
        source, evidence = recover_loop_timer(fn, source, RData(), {'0x108': ('WatchTimer', 'Int')})
        self.assertEqual(evidence[0]['status'], 'recovered')
        # Execute the complete native initial wait, including its early return.
        native = '{\n' + '\n'.join(strip_declarations(source)[:9]) + '\nreturn 99;\n}'
        lifter = Lifter(manifest, {'0x108': ('WatchTimer', 'Int')}, 'quest', False, '',
                        RData(), live_termination=True)
        body = '\n'.join(lifter.lift('Wait', native))
        self.assertEqual(lifter.todo, [])
        for values, terminate, expected in (((1,), False, 99), ((0, 0, 1), False, 99), ((0,), True, None)):
            lua, events = LuaRuntime(), []
            frame_count = [0]
            def state(_q, key):
                self.assertEqual(key, 'WatchTimer')
                return 11 + frame_count[0]
            def timer(_q, timer_id):
                events.append(('timer', timer_id))
                return values[frame_count[0]]
            def frame(_q):
                frame_count[0] += 1
                events.append(('frame',))
                return True
            def terminating(_q):
                events.append(('terminating',))
                return terminate
            quest = lua.table_from({'GetStateInt': state, 'GetTimer': timer,
                                    'NewScriptFrame': frame, 'IsActiveThreadTerminating': terminating})
            self.assertEqual(lua.execute('return function(quest)\n' + body + '\nend')(quest), expected)
            expected_events = [('timer', 11)]
            for i in range(frame_count[0]):
                expected_events += [('frame',), ('terminating',)]
                if not terminate:
                    expected_events.append(('timer', 12 + i))
            self.assertEqual(events, expected_events)

    def test_changed_callee_dispatch_source_or_state_rejects(self):
        fn, source, _ = self.inputs()
        data = RData()
        for changed_address in (0x6E7710, 0x6E7410, 0xCB7940, 0x1261074, 0x1260F28, 0xDB4F70):
            def read(address, size):
                raw = data.bytes_at(address, size)
                return bytes([raw[0] ^ 1]) + raw[1:] if address == changed_address else raw
            result, evidence = recover_loop_timer(fn, source, SimpleNamespace(bytes_at=read),
                                                  {'0x108': ('WatchTimer', 'Int')})
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')
        self.assertEqual(recover_loop_timer(fn, source, data, {})[1][0]['status'], 'rejected')
        self.assertEqual(recover_loop_timer(fn, source.replace('iVar3 < 1', 'iVar3 < 2'), data,
                                           {'0x108': ('WatchTimer', 'Int')})[1][0]['status'], 'rejected')

import copy
import unittest
from dataclasses import replace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_book_trader_acquisition as fixtures
from tools.script_recovery.native_book_trader_acquisition import recover_book_trader_acquisition
from tools.script_recovery.native_post_attack_resources import map_book_trader_resources
from tools.script_recovery.native_book_trader_home import recover_book_trader_home
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter, strip_declarations, load_thing_tables, thing_signatures


class BookTraderHomeTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.BookTraderAcquisitionTests().inputs()
        source, evidence = recover_book_trader_acquisition(fn, source, map_book_trader_resources(fn, data))
        self.assertEqual(evidence[0]['status'], 'recovered')
        return fn, source, data, manifest

    def test_distinct_initial_and_loop_snapshots_drive_native_operands(self):
        fn, source, data, manifest = self.inputs()
        recovered, evidence = recover_book_trader_home(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertIn('unresolved', evidence[0]['resourceLifetime'])
        statements = strip_declarations(recovered)
        def statement(needle):
            found = [s for s in statements if needle in s]
            self.assertEqual(len(found), 1, needle)
            return found[0]
        first = statement('native_arg_book_initial_home =')
        initial_distance = statement('me,native_arg_book_initial_home,fVar23')
        snapshot = statement('native_arg_book_home =')
        distance = statement('pCVar8,native_arg_book_home,fVar23')
        move = statement('native_arg_book_home,0.0,0,0,1')
        # Execute the recovered operand slice under the native threshold loop;
        # task scheduling and native temporary cleanup belong to other passes.
        body = '{\n' + '\n'.join([first, 'fVar23 = 0.1;', initial_distance,
            'if (bVar4) {', snapshot, 'pCVar8 = me;', 'while (true) {',
            'fVar23 = 2.0;', distance, 'if (!bVar4) break;', move, '}', '}']) + '\n}'
        lifter = Lifter(manifest, {}, 'quest', True, '', data,
                        thing_sigs=thing_signatures(load_thing_tables(manifest, {})[0]))
        emitted = '\n'.join(lifter.lift('HomeOperands', body))
        self.assertEqual(lifter.todo, [])
        for initially_far in (False, True):
            with self.subTest(initially_far=initially_far):
                lua, events, snapshots = LuaRuntime(), [], []
                initial = (2, -8, 4.5)
                home = (-31, 7.25, 99)
                loop_results = [True, True, False]
                def get_home(me):
                    values = initial if not snapshots else home
                    snapshots.append(lua.table_from(dict(zip(('x', 'y', 'z'), values))))
                    events.append(('home', *values))
                    return snapshots[-1]
                def distance_call(me, pos, threshold):
                    events.append(('distance', pos.x, pos.y, pos.z, threshold))
                    return initially_far if threshold == 0.1 else loop_results.pop(0)
                def move_call(me, pos, *flags):
                    events.append(('move', pos.x, pos.y, pos.z, *flags))
                me = lua.table_from({'GetHomePos': get_home,
                    'IsDistanceFromPositionOver': distance_call, 'MoveToPosition': move_call})
                lua.execute('return function(quest,me)\n' + emitted + '\nend')(lua.table(), me)
                expected = [('home', *initial), ('distance', *initial, 0.1)]
                if initially_far:
                    expected += [('home', *home), ('distance', *home, 2.0),
                        ('move', *home, 0.0, 0, False, True), ('distance', *home, 2.0),
                        ('move', *home, 0.0, 0, False, True), ('distance', *home, 2.0)]
                self.assertEqual(events, expected)

    def test_changed_native_actor_vector_threshold_or_flags_reject(self):
        fn, source, data, manifest = self.inputs()
        mutations = [
            (0xDB404D, {'stack_arguments': (('register', 'eax'), ('stack', 20), ('constant', 3))}),
            (0xDB4078, {'stack_arguments': (('register', 'ebp'), ('stack', 24), ('constant', 3))}),
            (0xDB40A0, {'ecx': ('register', 'eax')}),
            (0xDB40A7, {'stack_arguments': (('constant', 0x40000000),)}),
            (0xDB40D0, {'stack_arguments': (('stack', 172),)}),
            (0xDB40E4, {'ecx': ('stack', 24)}),
            (0xDB40F2, {'ecx': ('register', 'ebp')}),
            (0xDB40F2, {'edx': ('stack', 172)}),
            (0xDB40F2, {'stack_arguments': (('constant', 0x40400000),)}),
            (0xDB417C, {'target': ('constant', 0x7E7300)}),
            (0xDB417C, {'ecx': ('stack', 24)}),
        ]
        original_move = read_call_window(data, 0xDB3FA0, 4042, 0xDB417C, argument_count=5)
        for index in range(5):
            arguments = list(original_move.stack_arguments)
            arguments[index] = ('constant', 99)
            mutations.append((0xDB417C, {'stack_arguments': tuple(arguments)}))
        for site, changes in mutations:
            with self.subTest(site=hex(site), changes=changes):
                def decode(*args, **kwargs):
                    setup = read_call_window(*args, **kwargs)
                    return replace(setup, **changes) if args[3] == site else setup
                with patch('tools.script_recovery.native_book_trader_home.read_call_window', side_effect=decode):
                    result, evidence = recover_book_trader_home(fn, source, data, manifest)
                self.assertEqual(result, source)
                self.assertEqual(evidence[0]['status'], 'rejected')

    def test_changed_source_and_contracts_reject(self):
        fn, source, data, manifest = self.inputs()
        for changed_fn, changed_source in ((dict(fn, decompile=fn['decompile'] + '\n'), source),
                                           (fn, source + '\n')):
            result, evidence = recover_book_trader_home(changed_fn, changed_source, data, manifest)
            self.assertEqual(result, changed_source)
            self.assertEqual(evidence[0]['status'], 'rejected')
        for name in ('GetHomePos', 'MoveToPosition', 'IsDistanceFromPositionOver'):
            altered = copy.deepcopy(manifest)
            altered[name]['returnType'] = 'unknown'
            self.assertEqual(recover_book_trader_home(fn, source, data, altered)[1][0]['status'], 'rejected')
        self.assertEqual(recover_book_trader_home(dict(fn, address='0x0'), source, data, manifest), (source, []))

    def test_changed_native_snapshot_zero_register_and_callees_reject(self):
        fn, source, data, manifest = self.inputs()
        original = data.bytes_at
        for address in (0xDB402C, 0xDB40D0, 0xDB416B, 0xDB41BD, 0x7E7490, 0x7E72F0, 0xCBE45C):
            def changed(start, size):
                raw = original(start, size)
                if raw is not None and start <= address < start + size:
                    raw = bytearray(raw)
                    raw[address - start] ^= 1
                    return bytes(raw)
                return raw
            with self.subTest(address=hex(address)), patch.object(data, 'bytes_at', side_effect=changed):
                result, evidence = recover_book_trader_home(fn, source, data, manifest)
                self.assertEqual(result, source)
                self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()

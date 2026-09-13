import unittest
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_init_scalars as fixtures
from tools.script_recovery.native_init_scalars import recover_init_scalars, recover_init_bad_deeds
from tools.script_recovery.lift_native_lua import Lifter


ARRAYS = {'0xfc': ('WhichBadDeedsPerformed', 'Bool')}


class InitBadDeedTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.InitScalarTests().inputs()
        source, _ = recover_init_scalars(fn, source, data, fixtures.STATE, manifest)
        return fn, source, data, manifest

    def test_packed_clear_resets_the_five_consumer_keys_only(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_init_bad_deeds(fn, source, data, ARRAYS)
        self.assertEqual(evidence[0]['status'], 'recovered')
        lifter = Lifter(manifest, {}, 'quest', False, '', data, state_arrays=ARRAYS)
        body = '\n'.join(lifter.lift('Clear', '{\n' + evidence[0]['new'] + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, writes = LuaRuntime(), []
        state = {'WhichBadDeedsPerformed_' + str(i): True for i in range(6)}
        state['TalkIntermittentTimer'] = 71
        def write(q, key, value):
            writes.append((key, value))
            state[key] = value
        quest = lua.table_from({'SetStateBool': write})
        lua.execute('return function(quest)\n' + body + '\nend')(quest)
        self.assertEqual(writes, [('WhichBadDeedsPerformed_' + str(i), False) for i in range(5)])
        self.assertTrue(state['WhichBadDeedsPerformed_5'])
        self.assertEqual(state['TalkIntermittentTimer'], 71)
        # Use the same indexed load lowering as the helper consumers.
        reader = Lifter(manifest, {}, 'quest', False, '', data, state_arrays=ARRAYS)
        expression = reader.expr('*(undefined1 *)(this + 0xfc + index)')
        quest.GetStateBool = lambda q, key: state[key]
        run = lua.execute('return function(quest,index) return ' + expression + ' end')
        self.assertEqual([run(quest, i) for i in range(5)], [False] * 5)
        self.assertNotIn('*(undefined4 *)(param_1 + 0xfc)', result)

    def test_missing_array_mapping_rejects(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_init_bad_deeds(fn, source, data, {})
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

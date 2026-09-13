import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_speech_vector_init as fixtures
from tools.script_recovery.native_speech_vectors import recover_vectors, recover_init_vector_construction
from tools.script_recovery.native_init_scalars import recover_init_scalars
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


STATE = {'0x104': ('TalkIntermittentTimer', 'Int'), '0x64': ('GUIBullyHealthCounter', 'Int')}


class InitScalarTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.SpeechVectorInitTests().inputs()
        source, _ = recover_init_vector_construction(fn, source, data, recover_vectors(data.bytes_at))
        return fn, source, data, manifest

    def test_timer_id_and_signed_counter_are_preserved_in_order(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_init_scalars(fn, source, data, STATE, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        snippet = '{\n' + '\n'.join(edit['new'] for edit in evidence[0]['edits']) + '\n}'
        lifter = Lifter(manifest, STATE, 'quest', False, '', data)
        body = '\n'.join(lifter.lift('InitScalars', snippet))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        quest = lua.table_from({'GetStateInt': lambda q, key: 57 if key == 'TalkIntermittentTimer' else 9,
            'SetTimer': lambda q, timer, value: events.append(('timer', timer, value)),
            'SetStateInt': lambda q, key, value: events.append(('state', key, value))})
        lua.execute('return function(quest)\n' + body + '\nend')(quest)
        self.assertEqual(events, [('timer', 57, 0), ('state', 'GUIBullyHealthCounter', -999)])
        self.assertNotIn('SetTimer(ppVar4', result)

    def test_changed_zero_argument_or_missing_typed_state_rejects(self):
        fn, source, data, manifest = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, stack_arguments=(setup.stack_arguments[0], ('constant', 1)))
        with patch('tools.script_recovery.native_init_scalars.read_call_window', side_effect=changed):
            result, evidence = recover_init_scalars(fn, source, data, STATE, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')
        result, evidence = recover_init_scalars(fn, source, data, {}, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

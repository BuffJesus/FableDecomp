import tempfile
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_argument_conversation as fixtures
from tools.script_recovery.native_affair_wife_argument_conversation import recover_affair_wife_argument_conversation
from tools.script_recovery.native_affair_wife_text_sequence import recover_affair_wife_text_sequence
from tools.script_recovery.lift_native_lua import Lifter, known_callee_aliases


class AffairWifeTextSequenceTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeArgumentConversationTests().inputs()
        source, _ = recover_affair_wife_argument_conversation(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_sequence_wraps_and_checks_termination_only_for_missing_key(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_text_sequence(fn, source, data)
        self.assertEqual(evidence[0]['status'], 'recovered')
        start = result.index('native_arg_wife_line_counter = native_arg_wife_line_counter + 10;')
        stop = result.index('uVar14 = rand();', start)
        snippet = '{\n' + result[start:stop] + '\nreturn native_arg_wife_line_counter;\nLAB_00db3d90:\nreturn -1;\n}'
        lifter = Lifter(manifest, {}, 'quest', True, '', data, native_gotos=True,
                        live_termination=True, callee_names=known_callee_aliases(fn))
        body = '\n'.join(lifter.lift('Sequence', snippet, parameters={
            'native_arg_wife_line_counter': 'number', 'ppVar15': 'number', 'local_native_cached_husband': 'thing'}))
        # This cleanup label also has an incoming edge from the later reply,
        # outside the extracted wife-line region.
        self.assertEqual(lifter.todo, ['label LAB_00db3d8b'])
        lua, events = LuaRuntime(), []
        run = lua.execute('return function(quest,me,native_arg_wife_line_counter,ppVar15,local_native_cached_husband)\n' + body + '\nend')
        quest = lua.table_from({'IsActiveThreadTerminating': lambda q: events.append(('termination',)) or False,
            'AddLineToConversation': lambda q, ident, key, speaker, listener, flag:
                events.append(('line', ident, key, speaker, listener, flag))})
        counter = 0
        for expected in (10, 20, 30, 40, 10, 20, 30, 40, 10):
            events.clear()
            wrapping = counter == 40
            counter = run(quest, 'wife', counter, 173, 'husband')
            self.assertEqual(counter, expected)
            self.assertEqual(events, ([('termination',)] if wrapping else []) + [
                ('line', 173, 'TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_' + str(expected), 'wife', 'husband', False)])
        events.clear()
        quest['IsActiveThreadTerminating'] = lambda q: events.append(('termination',)) or True
        self.assertEqual(run(quest, 'wife', 40, 173, 'husband'), -1)
        self.assertEqual(events, [('termination',)])

    def test_changed_text_bank_rejects_specialization(self):
        fn, source, data, _ = self.inputs()
        with tempfile.TemporaryDirectory() as directory:
            bank = Path(directory) / 'text.big'
            bank.write_bytes(b'changed text bank')
            result, evidence = recover_affair_wife_text_sequence(fn, source, data, bank)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

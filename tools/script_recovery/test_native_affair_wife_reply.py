import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_text_sequence as fixtures
from tools.script_recovery.native_affair_wife_text_sequence import recover_affair_wife_text_sequence
from tools.script_recovery.native_affair_wife_reply import recover_affair_wife_reply
from tools.script_recovery.lift_native_lua import Lifter


class AffairWifeReplyTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeTextSequenceTests().inputs()
        source, _ = recover_affair_wife_text_sequence(fn, source, data)
        return fn, source, data, manifest

    def test_reply_probability_roles_and_cancellation(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_reply(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertIn(evidence[0]['new'], result)
        snippet = '{\n' + evidence[0]['new'] + '\nreturn 1;\nLAB_00db3d8b:\nreturn -1;\n}'
        lifter = Lifter(manifest, {}, 'quest', True, '', data, native_gotos=True, live_termination=True)
        body = '\n'.join(lifter.lift('Reply', snippet, parameters={
            'ppVar15': 'number', 'local_native_cached_husband': 'thing'}))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        run = lua.execute('return function(quest,me,ppVar15,local_native_cached_husband)\n' + body + '\nend')
        for raw in (-2147483648, -3, -2, -1, 0, 1, 2, 3, 32767):
            for terminating in (False, True):
                events.clear()
                remainder = (abs(raw) % 2) * (-1 if raw < 0 else 1)
                quest = lua.table_from({
                    'RetailRandModulo': lambda q, n: events.append(('random', n)) or remainder,
                    'IsActiveThreadTerminating': lambda q: events.append(('termination',)) or terminating,
                    'AddLineToConversation': lambda q, ident, key, speaker, listener, flag:
                        events.append(('line', ident, key, speaker, listener, flag))})
                expected = [('random', 2)]
                if remainder == 0:
                    expected.append(('termination',))
                    if not terminating:
                        expected.append(('line', 173, 'TEXT_QST_048_AFFAIRMAN_IN_TROUBLE', 'husband', 'wife', False))
                self.assertEqual(run(quest, 'wife', 173, 'husband'), -1 if remainder == 0 and terminating else 1)
                self.assertEqual(events, expected)

    def test_changed_native_reply_rejects(self):
        fn, source, data, manifest = self.inputs()
        class Changed:
            def bytes_at(self, address, size):
                raw = data.bytes_at(address, size)
                if address <= 0xdb3c61 < address + size:
                    raw = bytearray(raw)
                    raw[0xdb3c61-address] ^= 1
                    return bytes(raw)
                return raw
        result, evidence = recover_affair_wife_reply(fn, source, Changed(), manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

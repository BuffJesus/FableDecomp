import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_random as fixtures
from tools.script_recovery.native_random import fold_affair_wife_random
from tools.script_recovery.native_affair_wife_conversation import recover_affair_wife_conversation
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class AffairWifeConversationTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeRandomTests().inputs()
        source, _ = fold_affair_wife_random(source, fn, data.bytes_at)
        return fn, source, data, manifest

    def test_conversation_id_participant_and_line_listener_keep_native_order(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_conversation(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('Conversation', '{\n' + evidence[0]['new'] + '\nreturn uVar11;\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        heroes = iter(('participant hero', 'listener hero'))
        def hero(q):
            value = next(heroes)
            events.append(('hero', value))
            return value
        quest = lua.table_from({'GetHero': hero,
            'AddNewConversation': lambda q, actor, a, b: events.append(('new', actor, a, b)) or 173,
            'AddPersonToConversation': lambda q, ident, person: events.append(('person', ident, person)),
            'AddLineToConversation': lambda q, ident, key, speaker, listener, flag:
                events.append(('line', ident, key, speaker, listener, flag))})
        self.assertEqual(lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'wife'), 173)
        self.assertEqual(events, [('new', 'wife', False, False), ('hero', 'participant hero'),
            ('person', 173, 'participant hero'), ('hero', 'listener hero'),
            ('line', 173, 'TEXT_QST_048_AFFAIR_WIFE_WHERES_HUSBAND', 'wife', 'listener hero', False)])

    def test_changed_line_listener_rejects_whole_conversation(self):
        fn, source, data, manifest = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, stack_arguments=setup.stack_arguments[:-1] + (('register', 'edi'),)) if args[3] == 0xDB3385 else setup
        with patch('tools.script_recovery.native_affair_wife_conversation.read_call_window', side_effect=changed):
            result, evidence = recover_affair_wife_conversation(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

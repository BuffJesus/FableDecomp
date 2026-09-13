import unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_conversation as fixtures
from tools.script_recovery.native_affair_wife_conversation import recover_affair_wife_conversation, recover_affair_wife_running_line
from tools.script_recovery.lift_native_lua import Lifter


class AffairWifeRunningLineTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeConversationTests().inputs()
        source, _ = recover_affair_wife_conversation(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_line_has_explicit_absent_listener_despite_staged_actor(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_running_line(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('RunningLine', '{\nuVar9 = GSI->GetHero();\n' + evidence[0]['new'] + '\nreturn uVar11;\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        quest = lua.table_from({'GetHero': lambda q: 'staged hero',
            'AddNewConversation': lambda q, who, a, b: events.append(('new', who, a, b)) or 91,
            'AddLineToConversation': lambda q, ident, key, who, listener, flag:
                events.append(('line', ident, key, who, listener, flag))})
        self.assertEqual(lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'wife'), 91)
        self.assertEqual(events, [('new', 'wife', False, False),
            ('line', 91, 'TEXT_QST_048_AFFAIR_WIFE_RUNNING_TO_HUBBY', 'wife', None, False)])

    def test_changed_source_rejects(self):
        fn, source, data, manifest = self.inputs()
        source += '\n// changed'
        result, evidence = recover_affair_wife_running_line(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

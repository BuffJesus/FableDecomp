import re
import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_affair_wife_running_line as fixtures
from tools.script_recovery.native_affair_wife_conversation import recover_affair_wife_running_line
from tools.script_recovery.native_affair_wife_argument_conversation import recover_affair_wife_argument_conversation
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class AffairWifeArgumentConversationTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeRunningLineTests().inputs()
        source, _ = recover_affair_wife_running_line(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_saved_id_and_cached_husband_survive_active_conversation(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_argument_conversation(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        init, query, create = [e['new'] for e in evidence[0]['edits']]
        lookup = re.search(r'GSI->GetThingWithScriptName\(&local_native_cached_husband[^;]+;', result)[0]
        block = '\ncVar5 = ' + query + '\nif (cVar5 == \'\\0\') {\n' + create + '\n}\n'
        snippet = '{\n' + init + '\n' + lookup + '\n' + block * 3 + '\nreturn native_arg_wife_argument_id;\n}'
        lifter = Lifter(manifest, {}, 'quest', True, '', data)
        body = '\n'.join(lifter.lift('ArgumentConversation', snippet))
        self.assertEqual(lifter.todo, [])
        lua, events, active, ids = LuaRuntime(), [], iter((False, True, False)), iter((91, 92))
        def new(q, who, a, b):
            ident = next(ids)
            events.append(('new', who, a, b, ident))
            return ident
        quest = lua.table_from({'GetThingWithScriptName': lambda q, key: 'husband',
            'IsConversationActive': lambda q, ident: events.append(('active', ident)) or next(active),
            'AddNewConversation': new,
            'AddPersonToConversation': lambda q, ident, who: events.append(('person', ident, who))})
        self.assertEqual(lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'wife'), 92)
        self.assertEqual(events, [('active', 0), ('new', 'wife', False, False, 91), ('person', 91, 'husband'),
            ('active', 91), ('active', 91), ('new', 'wife', False, False, 92), ('person', 92, 'husband')])

    def test_changed_native_id_slot_rejects(self):
        fn, source, data, manifest = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, stack_arguments=(('memory', ('stack', 128)),)) if args[3] == 0xDB3B49 else setup
        with patch('tools.script_recovery.native_affair_wife_argument_conversation.read_call_window', side_effect=changed):
            result, evidence = recover_affair_wife_argument_conversation(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

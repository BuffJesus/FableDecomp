import re
import unittest
from dataclasses import replace
from unittest.mock import patch
from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_affair_wife_hits as fixtures
from tools.script_recovery.native_book_trader_hits import recover_affair_wife_hits
from tools.script_recovery.native_affair_pause import recover_affair_wife_pause
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import Lifter


class AffairWifePauseTests(unittest.TestCase):
    def inputs(self):
        fn, source, data, manifest = fixtures.AffairWifeHitTests().inputs()
        source, _ = recover_affair_wife_hits(fn, source, data, manifest)
        return fn, source, data, manifest

    def test_pause_operands_ignore_unrelated_staged_values(self):
        fn, source, data, manifest = self.inputs()
        result, evidence = recover_affair_wife_pause(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        calls = re.findall(r'GSI->PauseAllNonScriptedEntities\(([^)]*)\);', result)
        self.assertEqual(len(calls), 17)
        self.assertEqual(sum(x in ('true', '1') for x in calls), 4)
        self.assertTrue(all(x in ('true', 'false', '1', '0') for x in calls))
        self.assertNotIn('0x5ec', result)
        for edit in evidence[0]['edits']:
            call = re.search(r'GSI->PauseAllNonScriptedEntities\([^)]*\);', edit['new'])[0]
            lifter = Lifter(manifest, {}, 'quest', True, '', data)
            body = '\n'.join(lifter.lift('Pause', '{\nuVar9 = GSI->GetHero();\n' + call + '\n}'))
            self.assertEqual(lifter.todo, [])
            lua, events = LuaRuntime(), []
            quest = lua.table_from({'GetHero': lambda q: 'unrelated actor',
                'PauseAllNonScriptedEntities': lambda q, flag: events.append(flag)})
            lua.execute('return function(quest)\n' + body + '\nend')(quest)
            self.assertEqual(events, ['(true)' in call])

    def test_changed_saved_interface_rejects_all_edits(self):
        fn, source, data, manifest = self.inputs()
        def changed(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, ecx=('memory', ('stack', 40))) if args[3] == 0xDB329B else setup
        with patch('tools.script_recovery.native_affair_pause.read_call_window', side_effect=changed):
            result, evidence = recover_affair_wife_pause(fn, source, data, manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')

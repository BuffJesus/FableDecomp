import unittest
from dataclasses import replace
from unittest.mock import patch

from tools.script_recovery import test_native_barrel_position as fixtures
from tools.script_recovery.native_barrel_position import recover_barrel_return_position
from tools.script_recovery.native_affair_pause import recover_barrel_pause
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import RData, Lifter, strip_declarations


class BarrelPauseTests(unittest.TestCase):
    def inputs(self):
        fixture = fixtures.BarrelPositionTests()
        source, evidence, manifest = fixture.return_position_inputs()
        return fixture.function(), recover_barrel_return_position(source, evidence, RData()), manifest

    def test_saved_interface_pause_arguments_are_preserved(self):
        from lupa.lua54 import LuaRuntime
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_barrel_pause(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertNotIn('0x5ec', recovered)
        self.assertEqual(recovered.count('PauseAllNonScriptedEntities(true)'),
                         source.count('PauseAllNonScriptedEntities(true)') + 1)
        self.assertEqual(recovered.count('PauseAllNonScriptedEntities(false)'),
                         source.count('PauseAllNonScriptedEntities(false)') + 4)
        # Exercise each replacement through the real boolean argument lowering.
        for edit in evidence[0]['edits']:
            lifter = Lifter(manifest, {}, 'quest', True, '', RData())
            body = '\n'.join(lifter.lift('Pause', '{\n' + edit['new'] + '\n}'))
            self.assertEqual(lifter.todo, [])
            lua, calls = LuaRuntime(), []
            quest = lua.table_from({'PauseAllNonScriptedEntities': lambda _, value: calls.append(value)})
            lua.execute('return function(quest,me)\n' + body + '\nend')(quest, None)
            self.assertEqual(calls, ['(true)' in edit['new']])

    def test_changed_source_or_native_operands_reject_the_whole_recovery(self):
        fn, source, manifest = self.inputs()
        changed = source + '\n'
        result, evidence = recover_barrel_pause(fn, changed, RData(), manifest)
        self.assertEqual(result, changed)
        self.assertEqual(evidence[0]['status'], 'rejected')
        def decode(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, stack_arguments=(('constant', 1),))
        with patch('tools.script_recovery.native_affair_pause.read_call_window', side_effect=decode):
            result, evidence = recover_barrel_pause(fn, source, RData(), manifest)
        self.assertEqual(result, source)
        self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()

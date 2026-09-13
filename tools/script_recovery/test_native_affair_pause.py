import unittest
from dataclasses import replace
from unittest.mock import patch

from tools.script_recovery import test_native_affair_facing as fixtures
from tools.script_recovery.native_affair_facing import recover_affair_facing
from tools.script_recovery.native_affair_pause import recover_affair_pause
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import RData


class AffairPauseTests(unittest.TestCase):
    def test_five_native_unpause_calls_are_recovered_and_changed_operands_rejected(self):
        fn, source, manifest = fixtures.AffairFacingTests().inputs()
        data = RData()
        source, _ = recover_affair_facing(fn, source, data)
        result, evidence = recover_affair_pause(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual(result.count('GSI->PauseAllNonScriptedEntities(false);') -
                         source.count('GSI->PauseAllNonScriptedEntities(false);'), 5)
        self.assertNotIn(evidence[0]['old'], result)
        for changes in ({'ecx': ('memory', ('stack', 60))}, {'stack_arguments': (('constant', 1),)}):
            def decode(*args, **kwargs):
                return replace(read_call_window(*args, **kwargs), **changes)
            with patch('tools.script_recovery.native_affair_pause.read_call_window', side_effect=decode):
                result, evidence = recover_affair_pause(fn, source, data, manifest)
                self.assertEqual(result, source)
                self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()

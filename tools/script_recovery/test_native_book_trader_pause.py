import json
import unittest
from dataclasses import replace
from unittest.mock import patch

from tools.script_recovery.lift_native_lua import (ROOT, RData, Lifter, load_manifest, load_slots,
    load_thing_tables, annotate, fold_self_wrapper_arguments)
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.native_affair_pause import recover_book_trader_pause
from tools.script_recovery.native_call_setup_ir import read_call_window


class BookTraderPauseTests(unittest.TestCase):
    def inputs(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DB3FA0')
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = annotate(rename_parameters(fn['decompile'], function_parameters(fn['decompile'], member=True)),
                          slots, things, returning, entity=True)
        return fn, fold_self_wrapper_arguments(source)[0], manifest

    def test_cached_interface_scaffolding_removed_without_removing_resource_uses(self):
        fn, source, manifest = self.inputs()
        result, evidence = recover_book_trader_pause(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        for edit in evidence[0]['edits']:
            self.assertNotIn(edit['old'], result)
        self.assertIn('CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)&ppuStack_124);', result)
        self.assertEqual(result.count('GSI->PauseAllNonScriptedEntities(true);') - source.count('GSI->PauseAllNonScriptedEntities(true);'), 1)
        self.assertEqual(result.count('GSI->PauseAllNonScriptedEntities(false);') - source.count('GSI->PauseAllNonScriptedEntities(false);'), 3)
        lifter = Lifter(manifest, {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('Main', source, native_function=fn))
        self.assertNotIn('ppuVar2 = *', body)
        self.assertIn('quest:PauseAllNonScriptedEntities(true)', body)

    def test_changed_native_false_flag_preserves_all_source(self):
        fn, source, manifest = self.inputs()
        def decode(*args, **kwargs):
            setup = read_call_window(*args, **kwargs)
            return replace(setup, stack_arguments=(('constant', 1),))
        with patch('tools.script_recovery.native_affair_pause.read_call_window', side_effect=decode):
            result, evidence = recover_book_trader_pause(fn, source, RData(), manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()

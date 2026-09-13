import json
import unittest

from tools.script_recovery.lift_native_lua import (ROOT, RData, Lifter, annotate,
    load_manifest, load_slots, load_thing_tables, fold_self_wrapper_arguments,
    known_callee_aliases, thing_signatures, load_entity_state, load_entity_parent_state)
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.native_guard_cleanup import recover_guard_cleanup, LIMITATION
from tools.script_recovery.benchmark_lifter import LuaSyntaxChecker


class GuardCleanupTests(unittest.TestCase):
    def inputs(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DAC760')
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = annotate(rename_parameters(fn['decompile'], function_parameters(fn['decompile'], member=True)),
                          slots, things, returning, entity=True)
        return fn, source, manifest, things

    def test_actual_guard_compiles_with_explicit_cleanup_limitation(self):
        fn, source, manifest, things = self.inputs()
        lifter = Lifter(manifest, load_entity_state('NOVI_Guard'), 'quest', True, '', RData(),
                        callee_names=known_callee_aliases(fn), thing_sigs=thing_signatures(things),
                        parent_state=load_entity_parent_state('NOVI_Guard'),
                        live_termination=True, native_gotos=True)
        body = '\n'.join(lifter.lift('Main', source, native_function=fn))
        self.assertEqual(lifter.guard_cleanup_evidence[0]['status'], 'recovered')
        self.assertIn(LIMITATION, lifter.todo)
        self.assertNotIn('*piStack_13c =', body)
        checked = LuaSyntaxChecker().check({'Guard': 'return function(quest, me)\n' + body + '\nend'})
        self.assertTrue(checked['ok'], checked['errors'])

    def test_changed_source_or_bytes_preserves_unresolved_branch(self):
        fn, source, _, _ = self.inputs()
        source, _ = fold_self_wrapper_arguments(source)
        for changed_fn, changed_source, reader in (
                (dict(fn, decompile=fn['decompile'] + '\n'), source, RData()),
                (fn, source + '\n', RData()),
                (fn, source, type('MissingBytes', (), {'bytes_at': lambda *args: None})())):
            with self.subTest(source_changed=changed_source != source):
                result, evidence = recover_guard_cleanup(changed_fn, changed_source, reader)
                self.assertEqual(result, changed_source)
                self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()

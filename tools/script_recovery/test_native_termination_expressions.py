import json
import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_termination_expressions import fold_inline_termination
from tools.script_recovery.lift_native_lua import (
    ROOT, Lifter, RData, annotate, known_callee_aliases, load_manifest,
    load_slots, load_thing_tables, strip_declarations,
)


class InlineTerminationTests(unittest.TestCase):
    def test_ambiguous_call_label_does_not_acquire_a_reviewed_alias(self):
        reviewed = {'currentName': 'Guessed::Name', 'target': '0x00F35B30'}
        self.assertEqual(known_callee_aliases({'calls': [reviewed, reviewed]}),
                         {'Guessed::Name': 'IsActiveThreadTerminating'})
        for target in ('0x12345678', 'invalid', '', '0x00CBE9EE'):
            other = dict(reviewed, target=target)
            for calls in ([reviewed, other], [other, reviewed]):
                self.assertEqual(known_callee_aliases({'calls': calls}), {})

    def test_actual_theresa_condition_preserves_query_timing_and_saved_result(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DB97A0')
        name = 'CCreatureAction_TrollWhackGroundBase::Initialise'
        self.assertEqual({c['target'] for c in fn['calls'] if c['currentName'] == name}, {'0x00F35B30'})
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = annotate(fn['decompile'], slots, things, returning, entity=True)
        header = next(line.strip() for line in strip_declarations(source)
                      if '||' in line and name in line)
        expression = header[len('if ('):header.index(') goto ')]
        lifter = Lifter(manifest, {}, 'quest', True, '', RData(),
                        callee_names=known_callee_aliases(fn), live_termination=True)
        lifted = lifter.expr(expression)
        self.assertEqual(lifter.calls, ['IsActiveThreadTerminating'])
        self.assertTrue(lifter.used_alive)
        for initial_alive, terminating, expected, expected_queries in (
                (False, False, True, 0), (True, False, False, 1), (True, True, True, 1)):
            lua, queries = LuaRuntime(), []
            def query(_quest):
                queries.append('query')
                return terminating
            quest = lua.table_from({'IsActiveThreadTerminating': query})
            run = lua.execute('return function(quest,alive) local result = ' + lifted +
                              '; return result,alive end')
            result, saved_alive = run(quest, initial_alive)
            self.assertEqual(result, expected)
            self.assertEqual(len(queries), expected_queries)
            self.assertEqual(saved_alive, not terminating if expected_queries else initial_alive)

    def test_requires_reviewed_name_receiver_and_immediate_al_test(self):
        aliases = {'Known::Query': 'IsActiveThreadTerminating'}
        original = "(Known::Query((Known *)param_1), extraout_AL_08 == '\\0')"
        recovered, count = fold_inline_termination(original, aliases, 'quest', True)
        self.assertEqual(count, 1)
        self.assertIn('return alive', recovered)
        for source in (original.replace('Known::Query', 'Unknown::Query'),
                       original.replace('param_1', 'another_entity'),
                       original.replace(', extraout', ', side_effect(), extraout'),
                       '"' + original + '"', '/* ' + original + ' */', '// ' + original):
            self.assertEqual(fold_inline_termination(source, aliases, 'quest', True), (source, 0))

    def test_legacy_mode_uses_saved_frame_status(self):
        result, count = fold_inline_termination(
            "(CScriptBase::IsActiveThreadTerminating(this), extraout_AL != '\\0')", {}, 'quest', False)
        self.assertEqual((result, count), ('(not alive)', 1))


if __name__ == '__main__':
    unittest.main()

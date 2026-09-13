import json
import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_conditions import conditional_call_assignment
from tools.script_recovery.lift_native_lua import (
    ROOT, Lifter, RData, annotate, load_manifest, load_slots, load_thing_tables,
    strip_declarations,
)


class ConditionalCallTests(unittest.TestCase):
    def test_parentheses_do_not_change_short_circuit_operands(self):
        for expression in ('(left) && (bVar4 = GSI->GetTimer(7), bVar4 > 0)',
                           'left && (bVar4 = GSI->GetTimer(7), bVar4 > 0)',
                           '((left && (bVar4 = GSI->GetTimer(7), bVar4 > 0)))'):
            parsed = conditional_call_assignment('while (' + expression + ') {')
            self.assertIsNotNone(parsed)
            self.assertEqual(parsed[2:], ('&&', 'bVar4', 'GSI->', 'GetTimer', '7', 'bVar4 > 0'))

    def test_nested_call_and_quoted_delimiters_remain_intact(self):
        parsed = conditional_call_assignment('if (left || (iVar2 = GSI->Unknown(nested(1,2), "x,)&&"), iVar2)) {')
        self.assertEqual(parsed[6], 'nested(1,2), "x,)&&"')
        self.assertEqual(parsed[2], '||')

    def test_ambiguous_or_unbalanced_conditions_are_rejected(self):
        for condition in ('a && b || (x = GSI->GetTimer(7), x)',
                          'a && (x = GSI->GetTimer(7), x, y)',
                          'a && (x = Unknown(7), x)',
                          'a && (x = GSI->GetTimer((7), x)',
                          'a, (x = GSI->GetTimer(7), x)',
                          'a && (x = GSI->GetTimer(7), x)) || ((b)'):
            self.assertIsNone(conditional_call_assignment('if (' + condition + ') {'), condition)

    def test_actual_theresa_animation_wait_rechecks_only_when_not_near(self):
        unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
        fn = next(f for f in unit['functions'] if f['address'] == '0x00DB97A0')
        manifest, slots = load_manifest(), load_slots()
        things, returning = load_thing_tables(manifest, slots)
        source = annotate(fn['decompile'], slots, things, returning, entity=True)
        header = next(line for line in strip_declarations(source)
                      if line.strip().startswith('while ') and '_IsPerformingScriptTask_' in line)
        for initially_near, after_frame, statuses, expected_frames in (
                (True, False, [], 0), (False, False, [True, True, False], 2),
                (False, True, [True], 1), (False, False, [False], 0)):
            with self.subTest(initially_near=initially_near, after_frame=after_frame, statuses=statuses):
                lifter = Lifter(manifest, {}, 'quest', True, '', RData())
                body = '\n'.join(lifter.lift('Main', '{\nbVar4 = native_arg_near;\n' + header +
                    '\nGSI->NewScriptFrame();\nbVar4 = ' + str(after_frame).lower() + ';\n}\n}',
                    parameters={'native_arg_near': 'bool'}))
                lua, events, remaining = LuaRuntime(), [], iter(statuses)
                def query(_me):
                    events.append('query')
                    return next(remaining)
                def frame(_quest, _me):
                    events.append('frame')
                    return True
                quest = lua.table_from({'NewScriptFrame': frame})
                me = lua.table_from({'IsPerformingScriptTask': query})
                lua.execute('return function(quest,me,native_arg_near)\n' + body + '\nend')(quest, me, initially_near)
                self.assertEqual(events.count('query'), len(statuses))
                self.assertEqual(events.count('frame'), expected_frames)
                self.assertEqual(lifter.todo, [])


if __name__ == '__main__':
    unittest.main()

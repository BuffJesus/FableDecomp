"""A CDefString hidden result is a dynamic string, not the next literal argument."""
import unittest

from tools.script_recovery.native_evidence_lowering import lower_global_definition_strings, finish_lua
from tools.script_recovery.test_lift_native_lua import make


class DefinitionStringTests(unittest.TestCase):
    label = 'CDefString::operator_class_CCharString'

    def test_exact_callee_and_global_field_produce_string_result(self):
        source = self.label + '((CDefString *)(DAT_0143e90c + 0xe40),(int)&xStack_44)\n;'
        lowered = lower_global_definition_strings(source, {self.label: 0x415D70})
        self.assertEqual(lowered, 'xStack_44 = ENGINE_GlobalGameDataString(0xe40);')
        lifter = make()
        output = finish_lua('\n'.join(lifter.lift('Main', '{\n' + lowered + '\n}')))
        self.assertIn('ReadGlobalGameDataString(0xe40)', output)
        self.assertIn('string', lifter.kinds.values())

    def test_unproven_label_or_non_global_receiver_stays_visible(self):
        source = self.label + '((CDefString *)(DAT_0143e90c + 0xe40),(int)&result);'
        for labels in ({}, {self.label: 0x415D71}):
            self.assertEqual(lower_global_definition_strings(source, labels), source)
        source = source.replace('DAT_0143e90c', 'unresolved')
        self.assertEqual(lower_global_definition_strings(source, {self.label: 0x415D70}), source)

    def test_reused_output_reads_each_field_at_its_original_site(self):
        source = '\n'.join(self.label + f'((CDefString *)(DAT_0143e90c + {offset}),(int)&result);'
                           for offset in ('0xe3c', '0xe40'))
        output = lower_global_definition_strings(source, {self.label: 0x415D70})
        self.assertEqual(output.splitlines(), ['result = ENGINE_GlobalGameDataString(0xe3c);',
                                              'result = ENGINE_GlobalGameDataString(0xe40);'])

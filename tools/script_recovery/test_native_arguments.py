import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_arguments import split_arguments, strip_template_pointer_casts
from tools.script_recovery.test_lift_native_lua import make


class NativeArgumentTests(unittest.TestCase):
    def test_template_pointer_casts_preserve_literals_and_other_expressions(self):
        self.assertEqual(strip_template_pointer_casts('(pair<A,B> *)0x0'), '0x0')
        self.assertEqual(strip_template_pointer_casts('(std::pair<A,std::pair<B,C>> **)ptr'), 'ptr')
        for source in ('"(pair<A,B> *)0x0"', '(a < b) + 1', '(pair<A,B>)value',
                       '(pair<A,B>> *)ptr'):
            self.assertEqual(strip_template_pointer_casts(source), source)

    def test_template_casted_flag_is_one_operand_with_boolean_semantics(self):
        lifter = make()
        lifter.manifest = {**lifter.manifest, 'SetFlag': {'scope': 'Quest', 'returnType': 'void',
                             'parameters': [{'name': 'flag', 'type': 'bool'}]}}
        body = lifter.lift('Main', '{\nGSI->SetFlag((pair<enum_A,class_B> *)0x0);\n'
                           'GSI->SetFlag((pair<enum_A,class_B> *)0x1);\n}')
        lua = LuaRuntime()
        events = []
        quest = lua.table_from({'SetFlag': lambda _q, value: events.append(value)})
        lua.execute('return function(Quest)\n' + '\n'.join(body) + '\nend')(quest)
        self.assertEqual(events, [False, True])
        self.assertTrue(all(type(value) is bool for value in events))
        self.assertEqual(lifter.todo, [])

    def test_nested_calls_casts_arrays_and_strings_keep_their_commas(self):
        cases = [
            ('', []),
            ('first, fn(a,b), last', ['first', 'fn(a,b)', 'last']),
            ('(pair<enum_A,class_B> *)0x0, 1', ['(pair<enum_A,class_B> *)0x0', '1']),
            ('a < b, c > d', ['a < b', 'c > d']),
            ('table[index(a,b)], {1,2}, tail', ['table[index(a,b)]', '{1,2}', 'tail']),
            ('"a,b", \'x\', "escaped\\\",comma"', ['"a,b"', "'x'", '"escaped\\\",comma"']),
            ('x /* , ) */, y', ['x /* , ) */', 'y']),
        ]
        for source, expected in cases:
            with self.subTest(source=source):
                self.assertEqual(split_arguments(source), expected)

    def test_malformed_arguments_are_rejected(self):
        for source in ('a,,b', 'a,', 'fn(a,b', 'a],b', '"unterminated', 'a /* comment'):
            with self.subTest(source=source), self.assertRaises(ValueError):
                split_arguments(source)

    def test_emitted_call_preserves_nested_numeric_operand_and_string_commas(self):
        lifter = make()
        lifter.manifest = {**lifter.manifest, 'Record': {
            'scope': 'Quest', 'returnType': 'void', 'parameters': [
                {'name': 'message', 'type': 'const std::string&'}, {'name': 'number', 'type': 'int'}]}}
        body = '\n'.join(lifter.lift('Main', '{\nGSI->Record("one,two",math.max(2,3));\n}'))
        lua = LuaRuntime()
        events = []
        quest = lua.table_from({'Record': lambda _q, message, number: events.append((message, number))})
        lua.execute('return function(Quest)\n' + body + '\nend')(quest)
        self.assertEqual(events, [('one,two', 3)])
        self.assertEqual(lifter.todo, [])


if __name__ == '__main__':
    unittest.main()

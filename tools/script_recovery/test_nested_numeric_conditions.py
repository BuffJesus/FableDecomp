"""Nested conditions staged through 0/1 retain truth, type and call order."""
import itertools

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.readable_style import simplify_conditions


def evaluate(source, values):
    lua = LuaRuntime()
    calls = []
    lua.globals().probe = lambda name, value: calls.append(name) or value
    lua.execute('function check(a,b,c)\n' + source + '\nend')
    value = lua.globals().check(*values)
    return value, type(value), calls


@pytest.mark.parametrize('values', list(itertools.product([False, True], repeat=3)) + [(0, '', None)])
@pytest.mark.parametrize('one,zero', [(1, 0), (0, 1)])
@pytest.mark.parametrize('op', ['==', '~='])
def test_nested_expression_preserves_boolean_value_and_effects(values, one, zero, op):
    source = (f'    return ((probe("a(", a) or (probe("b)", b) and not probe("c", c))) '
              f'and {one} or {zero}) {op} 0\n')
    out = ''.join(simplify_conditions([source])[0])
    assert f'and {one} or {zero}' not in out
    assert evaluate(source, values) == evaluate(out, values)


def test_matching_text_inside_strings_is_not_code():
    source = '    return "((a or (b and c)) and 1 or 0) ~= 0"\n'
    assert ''.join(simplify_conditions([source])[0]) == source


@pytest.mark.parametrize('values', [(1, 1, None), (1, 2, None), (False, False, None)])
def test_double_negated_comparison_preserves_precedence(values):
    source = '    return not not (a == b)\n'
    out = ''.join(simplify_conditions([source])[0])
    assert evaluate(source, values) == evaluate(out, values)


@pytest.mark.parametrize('number', ['0.5', '0.0', '0x0', '0 + 1', '0 << 1', '0 | 1'])
def test_comparison_operand_must_be_the_whole_zero_literal(number):
    source = f'    return ((a) and 1 or 0) ~= {number}\n'
    assert ''.join(simplify_conditions([source])[0]) == source

"""Shared flag targets must become actual Boolean logic without losing early exits."""
import itertools
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import fold_boolean_jump_chains
from tools.script_recovery.build_readable_unit import readable_file


SOURCE = '''function Main(a, b, c)
    local flag
    if test("a", a) then goto LAB_failed end
    flag = true
    if test("b", b) then goto LAB_failed end
    if test("c", c) then goto LAB_failed end
    goto FLOW_done
    ::LAB_failed::
    flag = false
    ::FLOW_done::
    return flag
end
'''


def run(source, values):
    lua = LuaRuntime()
    calls = []
    lua.globals().test = lambda label, value: calls.append(label) or value
    lua.execute(source)
    return lua.globals().Main(*values), calls


@pytest.mark.parametrize('values', list(itertools.product([False, True], repeat=3)) + [(None, 0, ''), ('yes', None, False)])
@pytest.mark.parametrize('initial', [True, False])
def test_short_circuit_order_and_boolean_result(values, initial):
    source = SOURCE if initial else SOURCE.replace('flag = true', 'flag = INITIAL').replace('flag = false', 'flag = true').replace('INITIAL', 'false')
    lines, count = fold_boolean_jump_chains(source.splitlines(keepends=True))
    result = ''.join(lines)
    assert count == 4
    assert 'goto ' not in result
    assert run(result, values) == run(source, values)
    assert isinstance(run(result, values)[0], bool)
    styled, _ = readable_file(source)
    assert run(styled, values) == run(source, values)
    assert isinstance(run(styled, values)[0], bool)


@pytest.mark.parametrize('source', [
    SOURCE.replace('    local flag\n', ''),
    SOURCE.replace('test("b", b)', 'test("b", flag)'),
    SOURCE.replace('    local flag\n', '    local flag\n    if a then goto LAB_failed end\n    record("between")\n'),
    SOURCE.replace('    local flag\n', '    local flag\n    if a then goto FLOW_done end\n'),
    SOURCE.replace('    flag = true\n', '    flag = true\n    record(flag)\n'),
    SOURCE.replace('    local flag\n', '    local flag\n    local function observe()\n        return flag\n    end\n'),
])
def test_observable_intermediates_and_outside_edges_are_not_folded(source):
    lines, count = fold_boolean_jump_chains(source.splitlines(keepends=True))
    assert count == 0
    assert ''.join(lines) == source

"""Combining empty shared-exit branches must preserve short-circuit effects."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import fold_shared_exit_tests
from tools.script_recovery.readable_lua import readable_source

SOURCE = '''return function(first, second)
    local trace = {}
    local function a() trace[#trace + 1] = "a"; return first end
    local function b() trace[#trace + 1] = "b"; return second end
    if a() then
        goto done
    else
        if b() then goto done end
    end
    trace[#trace + 1] = "body"
    ::done::
    trace[#trace + 1] = "tail"
    return table.concat(trace, ",")
end
'''


@pytest.mark.parametrize('first', [None, False, True, 0, ''])
@pytest.mark.parametrize('second', [None, False, True, 0, ''])
def test_both_conditions_and_fallthrough_keep_their_effects(first, second):
    lines, count = fold_shared_exit_tests(SOURCE.splitlines(True))
    assert count == 1
    lua = LuaRuntime()
    before, after = lua.execute(SOURCE), lua.execute(''.join(lines))
    assert before(first, second) == after(first, second)


@pytest.mark.parametrize('old,new', [
    ('if b() then goto done end', 'if b() then goto another end'),
    ('        goto done\n', '        record()\n        goto done\n'),
    ('    else\n', '    elseif third then\n'),
    ('        if b() then goto done end', '        if b() then record(); goto done end'),
])
def test_a_nonempty_or_different_branch_is_not_combined(old, new):
    source = SOURCE.replace(old, new)
    lines, count = fold_shared_exit_tests(source.splitlines(True))
    assert count == 0
    assert ''.join(lines) == source


@pytest.mark.parametrize('opening,closing', [('return [[', ']]'), ('--[=[', ']=]')])
def test_branch_shaped_literal_or_comment_is_untouched(opening, closing):
    source = opening + '\n' + SOURCE + closing + '\n'
    lines, count = fold_shared_exit_tests(source.splitlines(True))
    assert count == 0
    assert ''.join(lines) == source


def test_proven_string_pointer_name_avoids_collision_and_preserves_member_keys():
    source = '''function Read(me)
    local dataString = "occupied"
    local pOther = me:GetDataString()
    local result = {pOther = pOther, label = "pOther"}
    return result, dataString
end
'''
    result, maps = readable_source(source)
    assert maps[0]['locals']['pOther']['name'] == 'dataString2'
    assert '{pOther = dataString2, label = "pOther"}' in result
    lua = LuaRuntime()
    lua.execute(result)
    value, occupied = lua.globals().Read(lua.table_from({'GetDataString': lambda _me: 'HEDWIG'}))
    assert value['pOther'] == 'HEDWIG' and value['label'] == 'pOther' and occupied == 'occupied'


@pytest.mark.parametrize('expression', ['me:GetOther()', '"actual other value"'])
def test_unrelated_other_variable_is_not_a_generated_string_pointer(expression):
    source = f'function Read(me)\n    local pOther = {expression}\n    return pOther\nend\n'
    result, maps = readable_source(source)
    assert 'pOther' not in maps[0]['locals']
    assert 'local pOther =' in result

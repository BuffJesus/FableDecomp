"""Discarding a temporary must preserve valid Lua and observable expression work."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import prune_dead_defs
from tools.script_recovery.build_readable_unit import readable_file


def execute(source):
    lua = LuaRuntime()
    lua.execute('''events = {}
function left() table.insert(events, "left"); return 2 end
function right() table.insert(events, "right"); return 3 end
function text() table.insert(events, "text"); return "suffix" end
'''+source)
    lua.globals().Main()
    return list(lua.globals().events.values())


@pytest.mark.parametrize('expression,expected', [
    ('("prefix_" .. text())', ['text']),
    ('left() + right()', ['left', 'right']),
    ('false and text()', []),
    ('true or text()', []),
    ('text()', ['text']),
])
def test_unused_expression_preserves_execution(expression, expected):
    source = f'''function Main()
    local scratchValue
    scratchValue = {expression}
end
'''
    lines, _ = prune_dead_defs(source.splitlines(keepends=True))
    assert execute(''.join(lines)) == execute(source) == expected
    styled, _ = readable_file(source)
    assert execute(styled) == expected


def test_operator_effects_inside_an_unused_call_expression_are_retained():
    source = '''function Main()
    local scratchValue
    scratchValue = makeValue() .. "tail"
end
'''
    lines, _ = prune_dead_defs(source.splitlines(keepends=True))
    lua = LuaRuntime()
    lua.execute('''count = 0
function makeValue() return setmetatable({}, {__concat = function(a,b) count = count + 1; return b end}) end
'''+''.join(lines))
    lua.globals().Main()
    assert lua.globals().count == 1

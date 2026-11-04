"""Lua relational complements must preserve unordered floating-point results."""
import math
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import _negate

@pytest.mark.parametrize('op', ['<','<=','>','>=','==','~='])
@pytest.mark.parametrize('left', [-math.inf,-1.0,-0.0,0.0,1.0,math.inf,math.nan])
@pytest.mark.parametrize('right', [-1.0,0.0,1.0,math.nan])
def test_negation_preserves_lua_comparison(op,left,right):
    lua=LuaRuntime()
    lua.globals().left=left
    lua.globals().right=right
    condition=f'left {op} right'
    expected=lua.execute('return not ('+condition+')')
    actual=lua.execute('return '+_negate(condition))
    assert actual is expected

def test_negation_evaluates_operands_once_in_order():
    lua=LuaRuntime()
    lua.execute("trace={};function first() table.insert(trace,'first');return 0/0 end;function second() table.insert(trace,'second');return 0 end")
    assert lua.execute('return '+_negate('first() <= second()')) is True
    assert list(lua.globals().trace.values())==['first','second']


@pytest.mark.parametrize('op', ['<','<=','>','>='])
def test_negation_preserves_comparison_metamethod_and_operand_order(op):
    lua=LuaRuntime()
    lua.execute("""
        trace={}
        local mt={
            __lt=function(a,b) table.insert(trace,'lt:'..a.name..':'..b.name);return false end,
            __le=function(a,b) table.insert(trace,'le:'..a.name..':'..b.name);return true end
        }
        left=setmetatable({name='left'},mt)
        right=setmetatable({name='right'},mt)
    """)
    condition=f'left {op} right'
    expected=lua.execute('return not ('+condition+')')
    trace=list(lua.globals().trace.values())
    lua.execute('trace={}')
    assert lua.execute('return '+_negate(condition)) is expected
    assert list(lua.globals().trace.values())==trace

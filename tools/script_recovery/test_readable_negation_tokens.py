"""Negate only the top-level equality token; keep operands byte-for-byte."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import _negate

@pytest.mark.parametrize('condition',[
    'lookup("a == b") == nil','lookup("a ~= b") ~= nil',
    "lookup('a == b') == nil",'lookup([[a == b]]) == nil',
    '(left == right) == flag','(left ~= right) ~= flag',
    'left==right','left~=right',
])
@pytest.mark.parametrize('left,right,flag',[(1,1,True),(1,2,False),(False,False,False),(None,None,True),(1,1,None),(1,2,0)])
def test_equality_negation_preserves_lua_value_and_call_arguments(condition,left,right,flag):
    results=[]
    for expression in ('not ('+condition+')',_negate(condition)):
        lua=LuaRuntime();lua.globals().left=left;lua.globals().right=right;lua.globals().flag=flag
        lua.execute('trace={};function lookup(value) table.insert(trace,value);return nil end')
        actual=lua.execute('return '+expression)
        results.append((actual,list(lua.globals().trace.values())))
    assert results[0]==results[1]

def test_nested_comparison_token_remains_unchanged():
    assert _negate('(left == right) == flag')=='(left == right) ~= flag'
    assert _negate('lookup("a == b") == nil')=='lookup("a == b") ~= nil'

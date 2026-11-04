import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import strip_redundant_parens, style_function


@pytest.mark.parametrize('expr', [
    'choice % ((last - first) >> 2)',
    '(choice | mask) + 1',
    '(choice & mask) * 3',
    'choice << (mask | 1)',
    'choice * (mask // 2)',
    '(~choice) ^ 2',
    '(#list) ^ 2',
])
def test_parenthesis_cleanup_preserves_bitwise_and_integer_operator_semantics(expr):
    source = 'return function(choice,mask,first,last,list)\n return ' + expr + '\nend\n'
    lines, _ = strip_redundant_parens(source.splitlines(keepends=True))
    lua = LuaRuntime(); before = lua.execute(source); after = lua.execute(''.join(lines))
    for choice in (0,1,5,32767):
        for mask in (0,1,3,7):
            args = choice,mask,4096,4124,lua.table_from([1,2,3])
            assert after(*args) == before(*args)


def test_complete_style_pass_keeps_modulo_shift_precedence():
    source = ('function Main(quest,choice,first,last)\n'
              '    local index = choice % ((last - first) >> 2)\n'
              '    quest:Use(index)\nend\n')
    # The real style pass can inline the single-use local; grouping must survive.
    out = style_function(source, set())[0]
    lua = LuaRuntime(); lua.execute(out)
    values = []
    q = lua.table_from({'Use': lambda _, value: values.append(value)})
    for choice in (0,1,2,3,32767): lua.globals().Main(q,choice,4096,4108)
    assert values == [choice % 3 for choice in (0,1,2,3,32767)]

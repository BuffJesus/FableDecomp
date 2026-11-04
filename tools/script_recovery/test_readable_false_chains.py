"""Literal-false chain pruning preserves conditions, results and lexical cleanup."""
import pytest
from tools.script_recovery.readable_style import fold_constant_conditions
from tools.script_recovery.test_readable_constant_conditions import execute


def fold(source):
    lines, count = fold_constant_conditions(source.splitlines(True))
    return ''.join(lines), count


@pytest.mark.parametrize('value', [None, False, True, 0, 7, '', 'then'])
@pytest.mark.parametrize('ending', ['else', 'no_else', 'all_false'])
def test_false_chain_keeps_result_arity_truthiness_and_close_order(value, ending):
    suffix = '''    else
        local owned <close> = setmetatable({}, {__close=function() trace[#trace+1]='close' end})
        trace[#trace+1]='else'
    end
''' if ending == 'else' else '    end\n'
    condition = 'false' if ending == 'all_false' else 'value'
    source = '''function Main(value)
    local outer=7
    if false then
        dead()
    elseif false then
        moreDead()
    elseif CONDITION then
        trace[#trace+1]='chosen'
        return outer,nil,9
'''.replace('CONDITION', condition) + suffix + '''    trace[#trace+1]='after'
    return outer
end
'''
    styled, count = fold(source)
    assert count >= 2 and 'dead()' not in styled and 'moreDead()' not in styled
    assert execute(styled, value) == execute(source, value)


@pytest.mark.parametrize('body', [
    '        ::inside::\n        return 1\n',
    '        -- retained diagnostic\n        dead()\n',
    '        dead("retained text")\n',
    '        local text=[=[\nelseif false then\n]=]\n',
])
def test_chain_preserves_alternate_entries_diagnostics_and_opaque_text(body):
    source = 'function Main()\n    if false then\n' + body + '    else\n        return 2\n    end\nend\n'
    assert fold(source) == (source, 0)


@pytest.mark.parametrize('value', ['then', '', None])
def test_promoted_condition_keeps_calls_strings_and_comment_text(value):
    source = '''function Main(value)
    local function predicate(v) trace[#trace+1]='condition'; return v == "then" end
    if false then
        dead()
    elseif predicate(value) then -- preserved condition text
        return 1
    else
        return 2
    end
end
'''
    styled, count = fold(source)
    assert count and 'if predicate(value) then -- preserved condition text' in styled
    assert execute(styled, value) == execute(source, value)


def test_else_keeps_close_cleanup_when_callback_raises():
    source = '''function Main()
    if false then
        dead()
    else
        local owned <close> = setmetatable({}, {__close=function(_, reason) trace[#trace+1]='close:'..reason end})
        error('callback',0)
    end
end
'''
    styled, count = fold(source)
    assert count and execute(styled) == execute(source)


def test_scope_free_else_does_not_add_nested_do_blocks():
    source = '''function Main()
    local value=0
    if false then
        dead()
    else
        do
            do
                value=7
                do goto finish end
                value=8
            end
        end
    end
    ::finish::
    return value
end
'''
    styled, count = fold(source)
    assert count and not any(line.strip() == 'do' for line in styled.splitlines())
    assert 'do goto finish end' not in styled
    assert execute(styled) == execute(source)


@pytest.mark.parametrize('body', [
    '        local value=9\n',
    '        return 9\n',
    '        ::inside::\n        value=9\n',
    '        value=[=[\n  preserved literal indentation\n]=]\n',
    '        function Callback() end\n',
    '        -- retained else diagnostic\n        value=9\n',
])
def test_kept_else_retains_its_scope_or_opaque_text(body):
    source = 'function Main()\n    local value=0\n    if false then\n        dead()\n    else\n' + body + '    end\n    return value\nend\n'
    styled, count = fold(source)
    assert count and '    do\n' in styled and body in styled
    assert execute(styled) == execute(source)

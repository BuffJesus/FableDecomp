"""Readability must preserve exits and lexical bindings, not just erase labels."""
from lupa.lua54 import LuaRuntime
import pytest

from tools.script_recovery.audit_readability import inspect_source
from tools.script_recovery.readable_lua import readable_function
from tools.script_recovery.readable_style import fold_nested_return_jumps, fold_loop_exit_jumps, fold_cleanup_exit_jumps, fold_goto_return


def fold(source):
    lines, count = fold_nested_return_jumps(source.splitlines(keepends=True))
    return ''.join(lines), count


def execute(source, *args):
    lua = LuaRuntime()
    lua.execute('events = {}; function record(x) table.insert(events, x) end\n' + source)
    result = lua.globals().Main(*args)
    return result, list(lua.globals().events.values())


@pytest.mark.parametrize('cancel', [True, False])
@pytest.mark.parametrize('outer', [True, False])
def test_nested_return_preserves_cleanup_and_fallthrough(cancel, outer):
    source = '''function Main(cancel, outer)
    if outer then
        if cancel then goto LAB_exit end
        record("work")
        ::LAB_exit::
        return
    end
    record("outside")
end
'''
    out, count = fold(source)
    assert count == 1
    assert 'goto' not in out
    assert execute(out, cancel, outer) == execute(source, cancel, outer)


def test_non_tail_exit_still_compiles_and_skips_following_statements():
    source = '''function Main()
    goto LAB_exit
    record("unreachable")
    ::LAB_exit::
    return
end
'''
    out, count = fold(source)
    assert count == 1
    assert execute(out) == execute(source)


def test_return_expression_is_not_moved_into_shadowed_scope():
    source = '''function Main()
    local value = 7
    do
        local value = 99
        goto LAB_exit
    end
    ::LAB_exit::
    return value
end
'''
    out, count = fold(source)
    assert count == 0
    assert execute(out)[0] == 7
    lines, count = fold_goto_return(source.splitlines(keepends=True))
    assert count == 0
    assert execute(''.join(lines))[0] == 7


@pytest.mark.parametrize('cancel', [True, False])
def test_implicit_function_exit(cancel):
    source = '''function Main(cancel)
    if cancel then goto LAB_exit end
    record("work")
    ::LAB_exit::
end
'''
    out, count = fold(source)
    assert count == 1
    assert 'LAB_exit' not in out
    assert execute(out, cancel) == execute(source, cancel)


def test_loop_tail_is_not_a_function_exit():
    source = '''function Main()
    local i = 0
    while i < 3 do
        i = i + 1
        goto LAB_again
        ::LAB_again::
    end
    return i
end
'''
    out, count = fold(source)
    assert count == 0
    assert execute(out)[0] == 3


def test_closure_exit_is_not_rewritten_as_parent_exit():
    source = '''function Main()
    local function nested()
        goto LAB_exit
        ::LAB_exit::
        return
    end
    nested()
    record("parent")
end
'''
    out, count = fold(source)
    assert count == 0
    assert execute(out) == execute(source)


def test_audit_separates_diagnostics_from_strings_and_code():
    row = inspect_source('-- TODO: recover LAB_123\nlocal text = "goto LAB_123"\ngoto LAB_123\n::LAB_123::\n')
    assert row['counts'] == {'unresolvedDiagnostics': 1, 'gotos': 1, 'nativeNames': 2}
    assert row['status'] == 'needs-recovery'
    assert inspect_source('local hero = quest:GetHero()')['status'] == 'needs-human-review'


@pytest.mark.parametrize('loop,end', [('while true do', 'end'), ('repeat', 'until false'), ('for i = 1, 3 do', 'end')])
@pytest.mark.parametrize('inline', [True, False])
def test_loop_exit_is_break(loop, end, inline):
    jump = 'if true then goto LAB_exit end' if inline else 'goto LAB_exit'
    source = f'''function Main()
    {loop}
        record("iteration")
        {jump}
    {end}
    ::LAB_exit::
    record("cleanup")
end
'''
    lines, count = fold_loop_exit_jumps(source.splitlines(keepends=True))
    out = ''.join(lines)
    assert count == 1
    assert 'goto' not in out
    assert execute(out) == execute(source)


@pytest.mark.parametrize('cleanup', ['', '    record("cleanup that jump skips")\n'])
def test_outer_loop_jump_and_intervening_cleanup_are_preserved(cleanup):
    source = f'''function Main()
    while true do
        while true do
            goto LAB_exit
        end
    end
{cleanup}    ::LAB_exit::
    record("finished")
end
'''
    lines, count = fold_loop_exit_jumps(source.splitlines(keepends=True))
    assert count == 0
    assert execute(''.join(lines)) == execute(source)


@pytest.mark.parametrize('cancel', [True, False])
def test_cleanup_exit_retains_call_order_and_live_values(cancel):
    source = '''function Main(cancel)
    local movie = 7
    if cancel then goto LAB_cleanup end
    movie = 9
    record("work")
    ::LAB_cleanup::
    record(movie)
    record("release")
end
'''
    lines, count = fold_cleanup_exit_jumps(source.splitlines(keepends=True))
    assert count == 1
    assert execute(''.join(lines), cancel) == execute(source, cancel)


def test_cleanup_is_not_inlined_into_a_shadowing_scope():
    source = '''function Main()
    local movie = 7
    do
        local movie = 99
        goto LAB_cleanup
    end
    ::LAB_cleanup::
    record(movie)
end
'''
    lines, count = fold_cleanup_exit_jumps(source.splitlines(keepends=True))
    assert count == 0
    assert execute(''.join(lines))[1] == [7]


def test_cleanup_global_is_not_rebound_to_loop_local():
    source = '''function Main()
    for movie = 1, 2 do
        goto LAB_cleanup
    end
    ::LAB_cleanup::
    record(movie)
end
'''
    lines, count = fold_cleanup_exit_jumps(source.splitlines(keepends=True))
    assert count == 0


def test_cleanup_global_is_not_rebound_to_local_function():
    source = '''function Main()
    do
        local function record(x)
        end
        goto LAB_cleanup
    end
    ::LAB_cleanup::
    record("global")
end
'''
    lines, count = fold_cleanup_exit_jumps(source.splitlines(keepends=True))
    assert count == 0
    assert execute(''.join(lines))[1] == ['global']


def test_cleanup_comments_cannot_swallow_an_inlined_return():
    source = '''function Main(cancel)
    if cancel then goto LAB_cleanup end
    ::LAB_cleanup::
    record("cleanup") -- explain this call
end
'''
    _, count = fold_cleanup_exit_jumps(source.splitlines(keepends=True))
    assert count == 0


def test_progress_name_is_based_on_the_consumer_and_reversible():
    source = '''function Main(quest)
    local fVar1
    fVar1 = 0
    fVar1 = fVar1 + 0.25
    quest:UpdateMiniGameInfoBar(fVar1)
end
'''
    out, names = readable_function(source)
    assert names['fVar1']['name'] == 'progress'
    assert 'quest:UpdateMiniGameInfoBar(progress)' in out


def test_conflicting_consumers_keep_a_generic_name():
    source = '''function Main(quest)
    local fVar1
    fVar1 = 0
    quest:UpdateMiniGameInfoBar(fVar1)
    quest:DeregisterTimer(fVar1)
end
'''
    _, names = readable_function(source)
    assert names['fVar1']['name'] == 'scratchValue'

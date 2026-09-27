"""Cosmetic suffix removal must not rename a global or an outer binding."""
import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.readable_style import drop_free_suffixes
from tools.script_recovery.build_readable_unit import readable_file


@pytest.mark.parametrize('body', [
    '    result2 = 7\n    return result2\n',
    '    --[[\n    local result2\n    ]]\n    result2 = 7\n    return result2\n',
    '    local text = [[\n    local result2\n    ]]\n    result2 = 7\n    return result2\n',
    '    local result2 = result2 + 1\n    return result2\n',
    '    if flag then\n        local result2 = 3\n        use(result2)\n    end\n    result2 = 7\n',
    '    local result2 = 3\n    if flag then\n        local result2 = 4\n        use(result2)\n    end\n    return result2\n',
    '    use(result2)\n    local result2 = 3\n    return result2\n',
])
def test_unknown_or_shadowed_scope_preserves_every_identifier(body):
    source = 'function F(flag)\n'+body+'end\n'
    out, count = drop_free_suffixes(source.splitlines(keepends=True))
    assert ''.join(out) == source and count == 0


@pytest.mark.parametrize('decl', ['    local result2 = 7\n', '    local result2\n    result2 = 7\n'])
def test_unique_local_still_gets_the_unsuffixed_name(decl):
    source = 'function F()\n'+decl+'    return result2\nend\n'
    out, count = drop_free_suffixes(source.splitlines(keepends=True))
    assert count == 1 and 'result2' not in ''.join(out)
    lua = LuaRuntime()
    lua.execute(''.join(out))
    assert lua.globals().F() == 7


def test_whole_readable_pipeline_preserves_global_write_and_separate_reader():
    source = '''function Write(quest)
    ppVar5 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(ppVar5, 0)
end
function Read()
    return ppVar5
end
'''
    out, _ = readable_file(source)
    lua = LuaRuntime()
    lua.execute(out)
    calls = []
    quest = lua.table_from({'GetActiveQuestName': lambda _: 'legacy_quest',
        'DeactivateQuestLater': lambda _q,name,delay: calls.append((name,delay))})
    lua.globals().Write(quest)
    assert lua.globals().Read() == 'legacy_quest'
    assert calls == [('legacy_quest',0)]
    assert lua.globals().ppVar is None


def test_assigned_function_parameter_is_a_local_binding():
    source = 'function F(count2)\n    count2 = count2 + 1\n    return count2\nend\n'
    out, count = drop_free_suffixes(source.splitlines(keepends=True))
    assert count == 1 and 'count2' not in ''.join(out)
    lua = LuaRuntime()
    lua.execute(''.join(out))
    assert lua.globals().F(3) == 4

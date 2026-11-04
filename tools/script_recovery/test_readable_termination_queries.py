"""An unused scheduler result must not erase its native observation point."""
import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.build_readable_unit import drop_unused_alive, readable_file
from tools.script_recovery.readable_style import normalize_alive, simplify_conditions


def run(source, terminating):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.globals().TERMINATING = terminating
    return lua.execute(source + '''
local calls = {}
local q = {
 IsActiveThreadTerminating = function() calls[#calls+1]='termination';return TERMINATING end,
 Marker = function(_,name) calls[#calls+1]=name end
}
local value = Main(q)
return value,table.concat(calls,',')
''')


@pytest.mark.parametrize('terminating', [False, True])
@pytest.mark.parametrize('pass_name', ['builder', 'normalize', 'conditions', 'pipeline'])
def test_discarded_result_keeps_order_and_count(pass_name, terminating):
    assignment = 'alive = not quest:IsActiveThreadTerminating()'
    if pass_name == 'conditions':
        assignment = 'bVar1 = quest:IsActiveThreadTerminating()'
    source = ('function Main(quest)\n    local alive = true\n    local bVar1\n'
              '    quest:Marker("before")\n    '+assignment+'\n'
              '    quest:Marker("after")\n    return false\nend\n')
    if pass_name == 'builder':
        result, _ = drop_unused_alive(source)
    elif pass_name == 'normalize':
        result = ''.join(normalize_alive(source.splitlines(True))[0])
    elif pass_name == 'conditions':
        result = ''.join(simplify_conditions(source.splitlines(True))[0])
    else:
        result, _ = readable_file(source)
    assert run(result, terminating) == run(source, terminating) == (False, 'before,termination,after')


@pytest.mark.parametrize('terminating', [False, True])
def test_consumed_result_still_controls_branch(terminating):
    source = '''function Main(quest)
    local alive = true
    local bVar1
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then
        quest:Marker("stopping")
        return false
    end
    quest:Marker("running")
    return true
end
'''
    result, _ = readable_file(source)
    assert run(result, terminating) == run(source, terminating)


def test_unrecognized_effect_expression_is_retained():
    source = 'function Main(quest)\n    local alive = true\n    alive = quest:UnknownEffect()\nend\n'
    result, _ = drop_unused_alive(source)
    assert 'alive = quest:UnknownEffect()' in result

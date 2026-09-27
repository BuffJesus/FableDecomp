"""Cleanup optimizations must not propagate conditional closure assignments."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_unit import readable_file


@pytest.mark.parametrize('flag', [False, True])
def test_conditional_capture_assignment_remains_conditional(flag):
    source = '''function Choose(flag)
    local chosen = false
    local function update()
        if flag then
            chosen = true
        end
    end
    update()
    return chosen
end
'''
    readable, _ = readable_file(source)
    lua = LuaRuntime()
    lua.execute(readable)
    assert lua.globals().Choose(flag) is flag


@pytest.mark.parametrize('iterations', [0, 1, 3])
def test_loop_assignment_is_not_propagated_past_a_skipped_loop(iterations):
    source = '''function Choose(iterations)
    local function update(count)
        local chosen = false
        for i = 1, count do
            chosen = true
        end
        return chosen
    end
    return update(iterations)
end
'''
    readable, _ = readable_file(source)
    lua = LuaRuntime()
    lua.execute(readable)
    assert lua.globals().Choose(iterations) is (iterations > 0)

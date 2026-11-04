import pytest
from tools.script_recovery.lua_binding_guard import ambiguous_bindings

@pytest.mark.parametrize('source',[
 'function Main(iVar1)\n    return iVar1\nend\n',
 'function Main()\n    local iVar1 = 3\n    return iVar1\nend\n',
 'function Main()\n    local iVar1 = 3\n    local function read()\n        return iVar1\n    end\n    return read()\nend\n',
 'function Main()\n    local iVar1 = 3\n    do\n        local iVar1 = 4\n    end\n    return iVar1\nend\n',
 'function Main()\n    local iVar1 = 3\n    return {iVar1 = "iVar1", value = iVar1}\nend\n',
 'function Main(n)\n    local items\n    items = (function(n) local t = {}; for i = 1,n do t[i] = 0 end; return t end)(n)\n    return items\nend\n',
 'function Main()\n    repeat\n        local iVar1 = 3\n    until iVar1 > 2\nend\n',
 'function Main(n)\n    local value = 0\n    for i = 1,n do\n        value = value + i\n    end\n    return value\nend\n',
])
def test_supported_scopes_remain_optimizable(source):
    assert not ambiguous_bindings(source)

@pytest.mark.parametrize('source',[
 'function Main()\n    repeat\n        local iVar1 = 3\n    until true\n    return iVar1\nend\n',
 'function Main()\n    for iVar1 = 1,3 do\n    end\n    return iVar1\nend\n',
 'function Main()\n    if true then\n        local iVar1 = 3\n    else\n        return iVar1\n    end\nend\n',
 'function Main()\n    if true then\n        local iVar1 = 3\n    elseif iVar1 then\n        return 4\n    end\nend\n',
])
def test_expired_scopes_do_not_hide_global_lookup(source):
    assert 'iVar1' in ambiguous_bindings(source)

def test_opaque_text_does_not_add_fake_bindings():
    assert not ambiguous_bindings('function Main()\n    -- local iVar1\n    return "local iVar1", [=[local iVar1]=]\nend\n')

def test_callback_initializer_parameters_and_result_have_separate_scopes():
    source='function Main(use)\n    local value = use(function(key)\n        return key\n    end)\n    return value\nend\n'
    assert not ambiguous_bindings(source)

def test_callback_initializer_cannot_capture_its_own_result_binding():
    source='function Main(use)\n    local value = use(function(key)\n        return value\n    end)\n    return value\nend\n'
    assert 'value' in ambiguous_bindings(source)

def test_multiline_table_initializer_cannot_see_its_own_binding():
    source='function Main()\n    local value = {\n        field = value\n    }\n    return value\nend\n'
    assert 'value' in ambiguous_bindings(source)

@pytest.mark.parametrize('statement',['other=1; value=9','other,value=1,9'])
def test_assignment_inside_table_function_is_not_a_table_key(statement):
    source='function Main()\n    local object = { fn = function() '+statement+' end }\n    local value = 3\n    return object, value\nend\n'
    assert 'value' in ambiguous_bindings(source)

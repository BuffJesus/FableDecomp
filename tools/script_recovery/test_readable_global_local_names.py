import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_unit import readable_file
from tools.script_recovery.readable_lua import readable_function

SOURCES=[
'''function Main()
    function iVar1() return 11 end
    local iVar1 = 3
    return _G.iVar1(), iVar1
end
''',
'''function Main()
    local value = iVar1
    local iVar1 = 3
    return value, iVar1
end
''',
'''function Main()
    local iVar1 = iVar1
    return iVar1
end
''',
'''function Main()
    do
        local iVar1 = 3
    end
    return iVar1
end
''',
'''function Main()
    local function read()
        return iVar1
    end
    local iVar1 = 3
    return read(), iVar1
end
''',
]

@pytest.mark.parametrize('source',SOURCES)
@pytest.mark.parametrize('stage',['rename','source','pipeline'])
def test_global_name_outside_local_scope_is_preserved(source,stage):
    from tools.script_recovery.readable_lua import readable_source
    result=readable_function(source)[0] if stage=='rename' else readable_source(source,inline_literals=True)[0] if stage=='source' else readable_file(source)[0]
    def run(code):
        lua=LuaRuntime(unpack_returned_tuples=True)
        lua.execute('iVar1=7');lua.execute(code)
        return lua.eval('Main')()
    assert run(result)==run(source)

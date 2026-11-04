import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_unit import readable_file
from tools.script_recovery.readable_style import prune_dead_defs,split_initialised_locals

@pytest.mark.parametrize('binding',['local _ENV = environment','local _ENV\n    _ENV = environment'])
@pytest.mark.parametrize('tail',['return exposed','exposed = 9\n    return exposed','local function read()\n        return exposed\n    end\n    return read()'])
@pytest.mark.parametrize('stage',['dead','pipeline'])
def test_local_environment_implicit_uses_keep_binding(binding,tail,stage):
    source='function Main(environment)\n    '+binding+'\n    '+tail+'\nend\n'
    if stage=='dead':
        lines,_=split_initialised_locals(source.splitlines(True));result=''.join(prune_dead_defs(lines)[0])
    else:result=readable_file(source)[0]
    def run(code):
        lua=LuaRuntime(unpack_returned_tuples=True)
        lua.execute('exposed=2')
        lua.execute(code)
        return lua.execute('local env={exposed=7};local ok,r=pcall(Main,env);return ok,r,env.exposed,exposed')
    assert run(result)==run(source)

@pytest.mark.parametrize('tail',['return exposed','exposed = 9\n    return exposed'])
def test_environment_reassignment_keeps_implicit_users(tail):
    source='function Main(environment, other)\n    local _ENV = environment\n    _ENV = other\n    '+tail+'\nend\n'
    result=readable_file(source)[0]
    def run(code):
        lua=LuaRuntime(unpack_returned_tuples=True);lua.execute('exposed=2');lua.execute(code)
        return lua.execute('local a={exposed=7};local b={exposed=8};local ok,r=pcall(Main,a,b);return ok,r,a.exposed,b.exposed,exposed')
    assert run(result)==run(source)

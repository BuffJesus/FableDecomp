import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import prune_dead_defs,split_initialised_locals
from tools.script_recovery.build_readable_unit import readable_file

@pytest.mark.parametrize('initial',['0','108','value'])
@pytest.mark.parametrize('operation',['counter + 4','counter & 7','counter.field'])
@pytest.mark.parametrize('stage',['dead','pipeline'])
def test_initial_value_survives_when_retained_self_read_can_execute(initial,operation,stage):
    source='function Main(value)\n    local counter\n    counter = '+initial+'\n    counter = '+operation+'\n    return true\nend\n'
    result=''.join(prune_dead_defs(source.splitlines(True))[0]) if stage=='dead' else readable_file(source)[0]
    def run(text):
        lua=LuaRuntime(unpack_returned_tuples=True)
        return lua.execute(text+'''
local events={};local function effect()events[#events+1]='effect';return 4 end
local value=setmetatable({},{__add=effect,__band=effect,__index=effect})
local ok,result=pcall(Main,value)
return ok,ok and tostring(result) or 'error',table.concat(events,',')
''')
    assert run(source)==run(result)

@pytest.mark.parametrize('stage',['dead','pipeline'])
def test_self_read_chain_retains_its_initialization(stage):
    source='function Main()\n    local counter\n    counter = 108\n    counter = counter + 4\n    counter = counter + 4\n    return true\nend\n'
    result=''.join(prune_dead_defs(source.splitlines(True))[0]) if stage=='dead' else readable_file(source)[0]
    assert LuaRuntime().execute(result+'return Main()') is True

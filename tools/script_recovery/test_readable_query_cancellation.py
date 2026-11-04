import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import prune_dead_termination_checks
from tools.script_recovery.build_readable_unit import readable_file

@pytest.mark.parametrize('method',['GetValue','IsAlive','HasThing','CanRun','MsgHit','GetStateInt'])
@pytest.mark.parametrize('stage',['prune','pipeline'])
@pytest.mark.parametrize('stop',[False,True])
def test_query_name_does_not_prove_termination_is_unchanged(method,stage,stop):
    source='''function Main(quest,actor)
    if quest:IsActiveThreadTerminating() then return end
    actor:__METHOD__()
    if quest:IsActiveThreadTerminating() then return end
    return true
end
'''.replace('__METHOD__',method)
    result=''.join(prune_dead_termination_checks(source.splitlines(True),set())[0]) if stage=='prune' else readable_file(source)[0]
    def run(code):
        lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().STOP=stop
        return lua.execute(code+'''
local stopped=false;local q={};local actor={};local events={}
function q:IsActiveThreadTerminating()events[#events+1]='check';return stopped end
actor.__METHOD__=function()events[#events+1]='query';stopped=STOP end
return Main(q,actor),table.concat(events,',')
'''.replace('__METHOD__',method))
    assert run(result)==run(source)

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import fold_else_exit,inline_single_use
from tools.script_recovery.build_readable_unit import readable_file


def run(source,condition=False,stop=False):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().condition=condition
    lua.globals().stop=stop
    return lua.execute('''trace={};quest={}
function cleanup() trace[#trace+1]='cleanup' end
function quest:IsObjectInThingsPossession() trace[#trace+1]='possession';return condition end
function quest:IsActiveThreadTerminating() trace[#trace+1]='termination';return stop end
'''+source+'''
local t=table.pack(pcall(Main,quest));local out={}
for i=1,t.n do out[#out+1]=tostring(t[i]) end
return table.concat(out,','),table.concat(trace,',')
''')


@pytest.mark.parametrize('exit_line',[
    'if stop then cleanup(); return end',
    'if stop then cleanup(); return "done" end',
    'if stop then return end',
    'cleanup(); return',
    'return "if then end"',
])
@pytest.mark.parametrize('condition',[False,True])
@pytest.mark.parametrize('stop',[False,True])
@pytest.mark.parametrize('pipeline',[False,True])
def test_else_requires_unconditional_exit(exit_line,condition,stop,pipeline):
    source='''function Main(quest)
    if condition then
        trace[#trace+1]='body'
    else
        '''+exit_line+'''
    end
    trace[#trace+1]='tail'
end
'''
    after=readable_file(source)[0] if pipeline else ''.join(fold_else_exit(source.splitlines(True))[0])
    assert run(after,condition,stop)==run(source,condition,stop)


@pytest.mark.parametrize('reverse',[False,True])
@pytest.mark.parametrize('condition',[False,True])
@pytest.mark.parametrize('stop',[False,True])
@pytest.mark.parametrize('pipeline',[False,True])
def test_inlining_cannot_cross_termination_query(reverse,condition,stop,pipeline):
    first,second=('IsActiveThreadTerminating','IsObjectInThingsPossession') if reverse else ('IsObjectInThingsPossession','IsActiveThreadTerminating')
    source=f'''function Main(quest)
    local value, other
    value = quest:{first}()
    other = quest:{second}()
    if value then
        if other then return 'both' end
        return 'first'
    end
    return other
end
'''
    after=readable_file(source)[0] if pipeline else ''.join(inline_single_use(source.splitlines(True))[0])
    assert run(after,condition,stop)==run(source,condition,stop)


@pytest.mark.parametrize('reverse',[False,True])
@pytest.mark.parametrize('condition',[False,True])
@pytest.mark.parametrize('stop',[False,True])
@pytest.mark.parametrize('pipeline',[False,True])
def test_return_prefix_cannot_cross_termination_query(reverse,condition,stop,pipeline):
    first,second=('IsActiveThreadTerminating','IsObjectInThingsPossession') if reverse else ('IsObjectInThingsPossession','IsActiveThreadTerminating')
    source=f'''function Main(quest)
    local value
    value = quest:{first}()
    return quest:{second}(), value
end
'''
    after=readable_file(source)[0] if pipeline else ''.join(inline_single_use(source.splitlines(True))[0])
    assert run(after,condition,stop)==run(source,condition,stop)

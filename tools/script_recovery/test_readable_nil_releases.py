"""Nil writes remain observable through returns, globals and escaped closures."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_unit import readable_file,prune_nil_releases


CASES=[
    'local value=7\n    value = nil\n    return value',
    'local value=7\n    value = nil\n    return value,9',
    'local value=7\n    value = nil\n    return tostring(value)',
    'local value=7\n    value = nil\n    return {value=value}',
    'shared = nil\n    return 9',
    'shared = nil',
    'local value=7\n    callback=function() return value end\n    value = nil',
    'local value=7\n    callback=function() return value end\n    value = nil\n    return 9',
    'local value=7\n    local function observe()\n        trace[#trace+1]=tostring(value)\n    end\n    local owned <close> = setmetatable({}, {__close=observe})\n    value = nil\n    return 9',
    'local value=7\n    local function observe()\n        trace[#trace+1]=tostring(value)\n    end\n    local owned <close> = setmetatable({}, {__close=observe})\n    value = nil',
    'local value=7\n    if enabled then\n        value = nil\n        return value\n    end\n    return value',
    'local value=7\n    do\n        local value=9\n        value = nil\n        return value\n    end',
    'shared = nil\n    return observe()',
    'local value=7\n    callback=function() return value end\n    value = nil\n    return callback()',
]


def execute(source,enabled):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().enabled=enabled
    return lua.execute('''shared=7;trace={}
function observe() trace[#trace+1]=tostring(shared);return shared end
'''+source+'''
local t=table.pack(pcall(Main));local out={tostring(t.n)}
for i=1,t.n do
    local v=t[i]
    local kind=type(v)
    if kind=='table' then v=v.value end
    out[#out+1]=kind..':'..tostring(v)
end
return table.concat(out,'|'),tostring(shared),callback and tostring(callback()) or '<none>',table.concat(trace,',')
''')


@pytest.mark.parametrize('body',CASES)
@pytest.mark.parametrize('enabled',[False,True])
@pytest.mark.parametrize('stage',['direct','unstyled','styled'])
def test_nil_writes(body,enabled,stage):
    source='function Main()\n    '+body+'\nend\n'
    if stage=='direct':
        after,count=prune_nil_releases(source)
        assert after==source and count==0
    else:
        after=readable_file(source,style=stage=='styled')[0]
    assert execute(after,enabled)==execute(source,enabled)


@pytest.mark.parametrize('source',[
    '--[[\nvalue = nil\nreturn value\n]]\n',
    'local text=[=[\nvalue = nil\nend\n]=]\n',
    'function Main()\n    local unused=9\n    unused = nil\n    return 7\nend\n',
])
def test_ambiguous_cleanup_is_preserved(source):
    assert prune_nil_releases(source)==(source,0)

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import inline_single_use
from tools.script_recovery.build_readable_unit import readable_file

@pytest.mark.parametrize('tail',[
 '    second = quest:GetSecond()\n    return value + second\n',
 '    return quest:GetSecond() + value\n',
 '    return quest:GetSecond(), value\n',
 '    second = quest:GetSecond() + 1\n    return value / second\n',
])
@pytest.mark.parametrize('stage',['inline','pipeline'])
@pytest.mark.parametrize('fault',[0,1,2])
def test_named_queries_keep_effects_and_error_order(tail,stage,fault):
    source='function Main(quest)\n    local value, second\n    value = quest:GetFirst() + 1\n'+tail+'end\n'
    after=''.join(inline_single_use(source.splitlines(True))[0]) if stage=='inline' else readable_file(source)[0]
    def run(code):
        lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().fault=fault
        return lua.execute(code+'''
local events={};local count=0;local quest={}
local function note(k) events[#events+1]=k;count=count+1;if fault==count then error('injected',0) end;return count end
function quest:GetFirst() return note('first') end
function quest:GetSecond() return note('second') end
local v=table.pack(pcall(Main,quest));local result={}
for i=1,v.n do result[#result+1]=tostring(v[i]) end
return table.concat(events,','),table.concat(result,',')
''')
    assert run(source)==run(after)

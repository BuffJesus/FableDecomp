import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import inline_single_use
from tools.script_recovery.build_readable_unit import readable_file

TAILS=['object.x, value','object["x"], value','object + object, value','object .. object, value','#object, value','object < other, value','object == other, value','1 // zero, value','consume(object.x, value)','consume(object["x"], value)','consume({key=object.x}, value)','consume(1 // zero, value)','consume((object + object), value)']
EXPRESSIONS=['first()','quest:GetStateInt("x")','source.value']

def run(source,fault):
    lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().fault=fault
    return lua.execute('''local events={};local count=0;local quest={};zero=0
local function note(k)events[#events+1]=k;count=count+1;if count==fault then error('injected',0)end;return 7 end
function first()note('first');return 42 end
function quest:GetStateInt()note('state');return 43 end
function consume(...)note('consume');return select('#',...) end
source=setmetatable({},{__index=function(t,k)note('source');return 44 end})
local meta={__index=function(t,k)return note('index')end,__add=function()return note('add')end,__concat=function()return note('concat')end,__len=function()return note('len')end,__lt=function()note('lt');return true end,__eq=function()note('eq');return true end}
object=setmetatable({},meta);other=setmetatable({},meta)
'''+source+'''
local v=table.pack(pcall(Main,quest,consume));local out={};for i=1,v.n do out[#out+1]=tostring(v[i])end
return table.concat(out,','):gsub('%[string ".-"%]:%d+: ',''),table.concat(events,',')
''')

@pytest.mark.parametrize('tail',TAILS)
@pytest.mark.parametrize('expression',EXPRESSIONS)
@pytest.mark.parametrize('stage',['inline','pipeline'])
@pytest.mark.parametrize('fault',[0,1,2,3])
def test_implicit_prefix_evaluations_preserve_order_results_and_faults(tail,expression,stage,fault):
    source='function Main(quest)\n    local value\n    value = '+expression+'\n    return '+tail+'\nend\n'
    after=''.join(inline_single_use(source.splitlines(True))[0]) if stage=='inline' else readable_file(source)[0]
    assert run(after,fault)==run(source,fault)

@pytest.mark.parametrize('tail',[
 'return value + 1','return -value','return 0.0 < value','return 1.25 >= value',
 'return consume(". [ + /", value)','return consume([[. [ + /]], value)',
 'return consume(--[[. [ + /]] value)','return (value)',
])
def test_pending_operators_decimals_and_opaque_text_remain_eligible(tail):
    source='function Main(quest,consume)\n    local value\n    value = first()\n    '+tail+'\nend\n'
    lines,count=inline_single_use(source.splitlines(True));after=''.join(lines)
    assert count==1
    for fault in (0,1):assert run(after,fault)==run(source,fault)

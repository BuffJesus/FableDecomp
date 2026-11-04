import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import inline_single_use
from tools.script_recovery.build_readable_unit import readable_file

TAILS=[
 'return gate and value', 'return gate or value',
 'return false and value', 'return true or value',
 'return (gate and value) or 19',
 'return consume(gate and value)',
 'return consume({key=gate or value})',
 'if gate then return value end\n    return nil',
 'if gate then consume(value); return end\n    return nil',
 'if gate and value then\n        return 1\n    end\n    return 2',
 'if gate or value then\n        return 1\n    end\n    return 2',
]
EXPRESSIONS=['draw()', 'quest:GetStateInt("x")', 'object.value', '1 // denominator']

def execute(source,gate,fault):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().gate=gate;lua.globals().fault=fault
    return lua.execute('''local events={};local count=0;local quest={};denominator=fault==2 and 0 or 2
local function note(name)events[#events+1]=name;count=count+1;if fault==1 then error('injected',0)end end
function draw()note('draw');return 42 end
function quest:GetStateInt(k)note('state');return false end
object=setmetatable({},{__index=function(t,k)note('index');return 43 end})
function consume(...)note('consume');return select('#',...) end
'''+source+'''
local out=table.pack(pcall(Main,gate,quest,consume));local values={}
for i=1,out.n do values[#values+1]=tostring(out[i])end
-- Error line numbers differ when safe passes remove unrelated lines.
local result=table.concat(values,','):gsub('%[string ".-"%]:%d+: ','')
return result,table.concat(events,',')
''')

@pytest.mark.parametrize('tail',TAILS)
@pytest.mark.parametrize('expression',EXPRESSIONS)
@pytest.mark.parametrize('stage',['inline','pipeline'])
@pytest.mark.parametrize('gate',[False,True])
@pytest.mark.parametrize('fault',[0,1,2])
def test_unconditional_evaluation_is_not_moved_into_conditional_use(tail,expression,stage,gate,fault):
    source='function Main(gate,quest)\n    local value\n    value = '+expression+'\n    '+tail+'\nend\n'
    after=''.join(inline_single_use(source.splitlines(True))[0]) if stage=='inline' else readable_file(source)[0]
    assert execute(after,gate,fault)==execute(source,gate,fault)

@pytest.mark.parametrize('tail',[
 'return value and gate', 'return value or gate', 'return not value',
 'return consume("and or then", value)',
 'return consume([[and or then]], value)',
 'return consume(--[[and or then]] value)',
 'if value then\n        return gate\n    end\n    return nil',
])
def test_unconditional_uses_and_opaque_text_remain_eligible(tail):
    source='function Main(gate,quest,consume)\n    local value\n    value = draw()\n    '+tail+'\nend\n'
    after,count=inline_single_use(source.splitlines(True));after=''.join(after)
    assert count==1
    for gate in (False,True):
        for fault in (0,1):assert execute(after,gate,fault)==execute(source,gate,fault)

@pytest.mark.parametrize('expression',['object.x','first()'])
@pytest.mark.parametrize('middle',['object.y','object["y"]','1 // zero','#object','object + object','object .. object'])
@pytest.mark.parametrize('stage',['inline','pipeline'])
@pytest.mark.parametrize('fault',[0,1,2])
def test_intervening_implicit_effects_keep_original_order(expression,middle,stage,fault):
    source='function Main()\n    local firstValue, secondValue\n    firstValue = '+expression+'\n    secondValue = '+middle+'\n    return firstValue, secondValue\nend\n'
    after=''.join(inline_single_use(source.splitlines(True))[0]) if stage=='inline' else readable_file(source)[0]
    def run(text):
        lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().fault=fault
        return lua.execute('''local events={};local count=0;zero=0
local function note(k)events[#events+1]=k;count=count+1;if count==fault then error('injected',0)end;return 42 end
function first()return note('first')end
object=setmetatable({},{__index=function(t,k)return note(k)end,__len=function()return note('len')end,__add=function()return note('add')end,__concat=function()return note('concat')end})
'''+text+'''
local v=table.pack(pcall(Main));local r={};for i=1,v.n do r[#r+1]=tostring(v[i])end
return table.concat(r,','):gsub('%[string ".-"%]:%d+: ',''),table.concat(events,',')
''')
    assert run(after)==run(source)

@pytest.mark.parametrize('literal',['0','0.0','1','-1','0xff','1e3','1e-3','.5','false','nil','"text"','[[text]]'])
def test_inert_staging_remains_crossable(literal):
    source='function Main()\n    local value, staged\n    value = first()\n    staged = '+literal+'\n    return value, staged\nend\n'
    after,count=inline_single_use(source.splitlines(True));after=''.join(after)
    assert 'value = first()' not in after
    def run(text):return LuaRuntime(unpack_returned_tuples=True).execute('function first()return 42 end\n'+text+'\nreturn Main()')
    assert run(after)==run(source)

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import prune_dead_termination_checks,pure_local_functions
from tools.script_recovery.build_readable_unit import readable_file

EXPRESSIONS=('value.field','value[1]','value + 1','1 + value','-value','#value','value .. "x"','value < 1','1 < value','value <= 1','value == other','value ~= other','value * 2','value // 2','value ^ 2','value & 1','~value')

@pytest.mark.parametrize('expr',EXPRESSIONS)
@pytest.mark.parametrize('stage',['prune','pipeline'])
@pytest.mark.parametrize('stop',[False,True])
@pytest.mark.parametrize('fault',[False,True])
def test_implicit_expression_preserves_cancellation(expr,stage,stop,fault):
    source='''function Main(quest,value,other)
    if quest:IsActiveThreadTerminating() then return end
    local result = EXPR
    if quest:IsActiveThreadTerminating() then return end
    return true
end
'''.replace('EXPR',expr)
    result=''.join(prune_dead_termination_checks(source.splitlines(True),set())[0]) if stage=='prune' else readable_file(source)[0]
    def run(code):
        lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().STOP=stop;lua.globals().FAULT=fault
        return lua.execute(code+'''
local events={};local stopped=false;local q={}
function q:IsActiveThreadTerminating()events[#events+1]='check';return stopped end
local function effect()events[#events+1]='implicit';stopped=STOP;if FAULT then error('injected',0)end;return 1 end
local meta={__index=effect,__add=effect,__unm=effect,__len=effect,__concat=effect,__lt=effect,__le=effect,__eq=effect,__mul=effect,__idiv=effect,__pow=effect,__band=effect,__bnot=effect}
local value,other=setmetatable({},meta),setmetatable({},meta)
local ok,result=pcall(Main,q,value,other)
return ok,result,table.concat(events,',')
''')
    assert run(result)==run(source)

@pytest.mark.parametrize('expr',EXPRESSIONS)
def test_helper_with_implicit_effect_is_not_marked_pure(expr):
    source='function Helper(value,other)\n    return '+expr+'\nend\n'
    assert 'Helper' not in pure_local_functions(source)

@pytest.mark.parametrize('literal',['"value.field + #x"',"'x[1] < y'",'[=[value.__index + x]=]','true','nil','42'])
def test_opaque_literal_remains_pure(literal):
    source='function Helper()\n    local result = '+literal+'\n    return result\nend\n'
    assert 'Helper' in pure_local_functions(source)

@pytest.mark.parametrize('statement',['value.field = 1','value[1] = 1'])
@pytest.mark.parametrize('stage',['prune','pipeline'])
@pytest.mark.parametrize('stop',[False,True])
def test_indexed_write_cannot_hide_cancellation(statement,stage,stop):
    source='function Main(quest,value)\n    if quest:IsActiveThreadTerminating() then return end\n    '+statement+'\n    if quest:IsActiveThreadTerminating() then return end\n    return true\nend\n'
    result=''.join(prune_dead_termination_checks(source.splitlines(True),set())[0]) if stage=='prune' else readable_file(source)[0]
    def run(code):
        lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().STOP=stop
        return lua.execute(code+'''
local stopped=false;local events={};local q={}
function q:IsActiveThreadTerminating()events[#events+1]='check';return stopped end
local value=setmetatable({},{__newindex=function()events[#events+1]='write';stopped=STOP end})
return Main(q,value),table.concat(events,',')
''')
    assert run(source)==run(result)

@pytest.mark.parametrize('expr',['value.field','value + 1','-value','#value'])
def test_discarded_implicit_expression_still_raises(expr):
    source='function Main(quest,value)\n    local result = '+expr+'\n    return true\nend\n'
    result=readable_file(source)[0]
    def run(code):
        lua=LuaRuntime(unpack_returned_tuples=True)
        return lua.execute(code+'\nlocal ok,err=pcall(Main,{},nil);return ok\n')
    assert run(source)==run(result)==False

@pytest.mark.parametrize('expr',['value.field','value + 1','#value'])
@pytest.mark.parametrize('stop',[False,True])
def test_implicit_helper_call_cannot_hide_cancellation(expr,stop):
    source='function Helper(value)\n    return '+expr+'\nend\nfunction Main(quest,value)\n    if quest:IsActiveThreadTerminating() then return end\n    Helper(value)\n    if quest:IsActiveThreadTerminating() then return end\n    return true\nend\n'
    result=readable_file(source)[0]
    def run(code):
        lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().STOP=stop
        return lua.execute(code+'''
local stopped=false;local events={};local q={}
function q:IsActiveThreadTerminating()events[#events+1]='check';return stopped end
local function effect()events[#events+1]='implicit';stopped=STOP;return 1 end
return Main(q,setmetatable({},{__index=effect,__add=effect,__len=effect})),table.concat(events,',')
''')
    assert run(source)==run(result)

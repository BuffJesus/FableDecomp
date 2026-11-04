import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import inline_single_use
from tools.script_recovery.build_readable_unit import readable_file

def transform(source,stage):
    return ''.join(inline_single_use(source.splitlines(True))[0]) if stage=='inline' else readable_file(source)[0]

def execute(source,mutation,fault):
    lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().mutation=mutation;lua.globals().fault=fault
    return lua.execute('''
local events={};local count=0;shared=10
local quest={}
local function note(k)events[#events+1]=k;count=count+1;if count==fault then error('injected',0)end end
function consume(x)note('old-callee');return x end
function quest:Consume(x)note('old-method');return x end
function first()
 note('first')
 if mutation then
  shared=20;consume=function(x)note('new-callee');return x+100 end
  quest.Consume=function(self,x)note('new-method');return x+100 end
 end
 return 7
end
'''+source+'''
local v=table.pack(pcall(Main,quest,consume));local result={}
for i=1,v.n do result[#result+1]=v[i]==quest and 'quest' or tostring(v[i])end
return table.concat(events,','),table.concat(result,',')
''')

@pytest.mark.parametrize('tail',['return shared, value','return shared + value','return consume(value)','return quest:Consume(value)'])
@pytest.mark.parametrize('stage',['inline','pipeline'])
@pytest.mark.parametrize('mutation',[False,True])
@pytest.mark.parametrize('fault',[0,1,2])
def test_binding_and_method_resolution_stays_after_effectful_expression(tail,stage,mutation,fault):
    source='function Main(quest)\n    local value\n    value = first()\n    '+tail+'\nend\n'
    assert execute(transform(source,stage),mutation,fault)==execute(source,mutation,fault)

@pytest.mark.parametrize('stage',['inline','pipeline'])
@pytest.mark.parametrize('fault',[0,1,2])
def test_captured_local_callee_is_not_read_before_mutating_call(stage,fault):
    source='''function Main(quest)
    local value, saved
    saved = consume
    local function change()
        saved = first
        return 4
    end
    value = change()
    return saved(value)
end
'''
    assert execute(transform(source,stage),True,fault)==execute(source,True,fault)

@pytest.mark.parametrize('tail',['return consume(value)','return consume("and : .", value)','return consume([[and : .]], value)','return consume(--[[and : .]] value)','return quest, value'])
def test_parameter_bindings_and_opaque_text_allow_inlining(tail):
    source='function Main(quest,consume)\n    local value\n    value = first()\n    '+tail+'\nend\n'
    after,count=inline_single_use(source.splitlines(True));assert count==1
    for mutation in (False,True):
        for fault in (0,1,2):assert execute(''.join(after),mutation,fault)==execute(source,mutation,fault)

@pytest.mark.parametrize('stage',['inline','pipeline'])
@pytest.mark.parametrize('fault',[0,1,2])
@pytest.mark.parametrize('kind',['global','method'])
def test_lookup_metamethod_keeps_snapshot_and_errors(stage,fault,kind):
    source='function Main(quest)\n    local value\n    value = shared\n    return '+('missing, value' if kind=='global' else 'quest:Consume(value)')+'\nend\n'
    def run(code):
        lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().fault=fault
        return lua.execute('''local events={};local count=0;shared=10
local function lookup(k)
 events[#events+1]=k;count=count+1;if count==fault then error('injected',0)end
 shared=20;return function(self,value)events[#events+1]='consume';return value end
end
local quest=setmetatable({},{__index=function(t,k)return lookup(k)end})
setmetatable(_G,{__index=function(t,k)lookup(k);return 99 end})
'''+code+'''
local v=table.pack(pcall(Main,quest));local values={}
for i=1,v.n do values[#values+1]=tostring(v[i])end
return table.concat(events,','),table.concat(values,',')
''')
    assert run(transform(source,stage))==run(source)

@pytest.mark.parametrize('value',['0','0.0','-1','0xff','1e-3','false','nil','"text"','[[text]]'])
def test_inert_literal_can_pass_method_lookup(value):
    source='function Main(quest)\n    local value\n    value = '+value+'\n    return quest:Consume(value)\nend\n'
    after,count=inline_single_use(source.splitlines(True));assert count==1
    for fault in (0,1):assert execute(''.join(after),False,fault)==execute(source,False,fault)

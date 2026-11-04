import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import prune_dead_defs,inline_single_use
from tools.script_recovery.build_readable_unit import readable_file

BODIES=[
    'local value\n    value = quest:GetRandomThingWithScriptName("unused")\n    return 7',
    'local first, second\n    first = quest:GetRandomThingWithScriptName("first")\n    second = quest:GetRandomThingWithScriptName("second")\n    return second, first',
    'local first, second\n    first = quest:GetRandomThingWithScriptName("first")\n    second = quest:GetRandomThingWithScriptName("second")\n    return first',
    'local observed, value\n    observed = quest:GetStateInt("DrawCount")\n    value = quest:GetRandomThingWithScriptName("selected")\n    return value, observed',
    'local value, observed\n    value = quest:GetRandomThingWithScriptName("selected")\n    observed = quest:GetStateInt("DrawCount")\n    return observed, value',
    'local value\n    value = quest:GetRandomThingWithScriptName("first")\n    return quest:GetRandomThingWithScriptName("second"), value',
    'local observed\n    observed = quest:GetStateInt("DrawCount")\n    return quest:GetRandomThingWithScriptName("selected"), observed',
    'local value\n    value = quest:GetRandomThingWithScriptName("first")\n    value = quest:GetRandomThingWithScriptName("second")\n    return value',
    'local value\n    value = false and quest:GetRandomThingWithScriptName("skipped")\n    return value',
    'local value\n    value = quest:GetRandomThingWithScriptName("selected")\n    if value then\n        value = nil\n    end\n    return value',
]
def transform(source,stage):
    if stage=='sink':
        from tools.script_recovery.readable_style import sink_hoisted_locals
        return ''.join(sink_hoisted_locals(source.splitlines(True))[0])
    if stage=='split':
        from tools.script_recovery.lua_local_versions import split_hoisted_locals
        return split_hoisted_locals(source)[0]
    if stage=='dead':return ''.join(prune_dead_defs(source.splitlines(True))[0])
    if stage=='inline':return ''.join(inline_single_use(source.splitlines(True))[0])
    return readable_file(source)[0]
def execute(source,fault):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().fault=fault
    return lua.execute('''local quest={};local draws=0;local trace={}
function quest:GetRandomThingWithScriptName(name)
 draws=draws+1;trace[#trace+1]=name
 if draws==fault then error('injected',0)end
 return draws
end
function quest:GetStateInt(name)assert(name=='DrawCount');trace[#trace+1]='count';return draws end
'''+source+'''
local result=table.pack(pcall(Main,quest));local out={}
for i=1,result.n do out[#out+1]=tostring(result[i])end
return table.concat(out,','),draws,table.concat(trace,',')
''')
@pytest.mark.parametrize('body',BODIES)
@pytest.mark.parametrize('stage',['dead','inline','pipeline'])
@pytest.mark.parametrize('fault',[0,1,2])
def test_random_draws_preserve_count_order_errors(body,stage,fault):
    source='function Main(quest)\n    '+body+'\nend\n'
    assert execute(transform(source,stage),fault)==execute(source,fault)


@pytest.mark.parametrize('stage',['dead','inline','pipeline'])
@pytest.mark.parametrize('release',[None,'nil','false','{}'])
def test_unused_random_thing_keeps_original_reference_lifetime(stage,release):
    source='''function Main(quest)
    local held
    held = quest:GetRandomThingWithScriptName("held")
    collectgarbage("collect")
    return weak[1] ~= nil
end
'''
    if release is not None:
        source=source.replace('    collectgarbage', '    held = '+release+'\n    collectgarbage')
    def run(text):
        lua=LuaRuntime(unpack_returned_tuples=True)
        return lua.execute('''weak=setmetatable({}, {__mode='v'});local quest={}
function quest:GetRandomThingWithScriptName(name)local object={};weak[1]=object;return object end
'''+text+'\nreturn Main(quest)')
    assert run(source) is (release is None)
    assert run(transform(source,stage)) is (release is None)


@pytest.mark.parametrize('stage',['sink','pipeline'])
@pytest.mark.parametrize('name',['held','r4'])
@pytest.mark.parametrize('header',['if quest:GetStateBool("take") then','for i=1,2 do'])
@pytest.mark.parametrize('release',[False,True])
def test_used_random_result_keeps_enclosing_scope(stage,name,header,release):
    source='function Main(quest)\n    local '+name+'\n    '+header+'\n        '+name+' = quest:GetRandomThingWithScriptName("held")\n        quest:Use('+name+')\n    end\n'
    if release:source+='    '+name+' = nil\n'
    source+='    collectgarbage("collect")\n    return weak[1] ~= nil\nend\n'
    def run(text):
        lua=LuaRuntime(unpack_returned_tuples=True)
        return lua.execute('''weak=setmetatable({}, {__mode='v'});local quest={}
function quest:GetStateBool()return true end
function quest:Use(value)end
function quest:GetRandomThingWithScriptName()local object={};weak[1]=object;return object end
'''+text+'\nreturn Main(quest)')
    assert run(source) is (not release)
    assert run(transform(source,stage)) is (not release)


@pytest.mark.parametrize('stage',['dead','inline','pipeline','split'])
@pytest.mark.parametrize('name',['held','r4'])
@pytest.mark.parametrize('release',[None,'nil','false','{}'])
def test_copied_random_result_preserves_release(stage,name,release):
    source='function Main(quest)\n    local '+name+'\n    '+name+' = quest:GetRandomThingWithScriptName("held")\n    otherSpawnPoint = '+name+'\n'
    if release is not None:
        source+='    '+name+' = '+release+'\n'
    source+='    quest:Observe()\n    return weak[1] ~= nil\nend\n'
    def run(text):
        lua=LuaRuntime(unpack_returned_tuples=True)
        return lua.execute('''weak=setmetatable({}, {__mode='v'});local quest={}
function quest:GetRandomThingWithScriptName()local object={};weak[1]=object;return object end
function quest:Observe()otherSpawnPoint=nil;collectgarbage('collect')end
'''+text+'\nreturn Main(quest)')
    assert run(source) is (release is None)
    assert run(transform(source,stage)) is (release is None)

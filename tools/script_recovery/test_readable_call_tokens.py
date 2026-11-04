import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import _calls,prune_dead_defs,inline_single_use
from tools.script_recovery.build_readable_unit import readable_file

CALLS=[
 'quest:SetStateInt("x", 42)',
 'quest:SetStateInt ("x", 42)',
 'quest : SetStateInt ("x", 42)',
 'quest:SetStateInt\t("x", 42)',
 'quest.SetStateInt(quest, "x", 42)',
 'quest . SetStateInt (quest, "x", 42)',
 'write (42)',
 'write "42"',
 'write [[42]]',
 'write {42}',
 'functions[1] (42)',
 '(write) (42)',
 'factory() (42)',
 'quest : Write "42"',
 'quest:Write {42}',
 'quest:GetWriter "key" (42)',
 'quest:GetWriter {} (42)',
 'quest:GetWriter "key" "42"',
 'quest:GetWriter "key" {42}',
]

def transform(source,stage):
    if stage=='dead':return ''.join(prune_dead_defs(source.splitlines(True))[0])
    if stage=='inline':return ''.join(inline_single_use(source.splitlines(True))[0])
    return readable_file(source)[0]

def run(source,fault):
    lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().fault=fault
    return lua.execute('''local state=0;local events={};local quest={}
function write(v)
 events[#events+1]='write'
 if fault then error('injected',0)end
 if type(v)=='table' then v=v[1]end
 state=tonumber(v);return state
end
functions={write};function factory()events[#events+1]='factory';return write end
function quest:SetStateInt(k,v)assert(k=='x');return write(v)end
function quest:Write(v)return write(v)end
function quest:GetWriter(key)events[#events+1]='get-writer';return write end
'''+source+'''
local ok,value=pcall(Main,quest)
return ok,value,state,table.concat(events,',')
''')

@pytest.mark.parametrize('call',CALLS)
@pytest.mark.parametrize('stage',['dead','inline','pipeline'])
@pytest.mark.parametrize('fault',[False,True])
def test_call_forms_preserve_effects_and_errors(call,stage,fault):
    source='function Main(quest)\n    local unused\n    unused = '+call+'\n    return 7\nend\n'
    assert run(transform(source,stage),fault)==run(source,fault)

@pytest.mark.parametrize('source,expected',[
 ('quest : SetStateInt ("x", 42)',[(':','quest','SetStateInt')]),
 ('object . nested : Query (x)',[(':','object.nested','Query')]),
 ('"quest:Mutate()"',[]),
 ('[[quest:Mutate()]]',[]),
 ('-- quest:Mutate()',[]),
 ('--[[quest:Mutate()]]',[]),
 ('value .. "call()"',[]),
 ('write "payload"',[('',None,'write')]),
 ('functions[1] (42)',[('',None,'<dynamic>')]),
 ('factory() (42)',[('',None,'factory'),('',None,'<dynamic>')]),
 ('quest:GetWriter "key" (42)',[(':','quest','GetWriter'),('',None,'<dynamic>')]),
 ('quest:GetWriter {} (42)',[(':','quest','GetWriter'),('',None,'<dynamic>')]),
 ('function named(x)',[]),
 ('local function named(x)',[]),
 ('function object:method(x)',[]),
 ('if (condition) then',[]),
 ('value = {name="payload"}',[]),
])
def test_scanner_ignores_opaque_text_and_declarations(source,expected):
    assert _calls(source)==expected


@pytest.mark.parametrize('argument',['("held")',' "held"',' [[held]]',' {"held"}'])
@pytest.mark.parametrize('release',[False,True])
def test_random_argument_forms_keep_reference_lifetime(argument,release):
    source='''function Main(quest)
    local r4
    if quest:GetStateBool("take") then
        r4 = quest:GetRandomThingWithScriptName'''+argument+'''
        other = r4
'''
    if release:source+='        r4 = nil\n'
    source+='    end\n    quest:Observe()\n    return weak[1] ~= nil\nend\n'
    def execute(text):
        lua=LuaRuntime(unpack_returned_tuples=True)
        return lua.execute('''local quest={};weak=setmetatable({}, {__mode='v'})
function quest:GetStateBool()return true end
function quest:GetRandomThingWithScriptName()local v={};weak[1]=v;return v end
function quest:Observe()other=nil;collectgarbage('collect')end
'''+text+'\nreturn Main(quest)')
    assert execute(source) is (not release)
    assert execute(transform(source,'pipeline')) is (not release)

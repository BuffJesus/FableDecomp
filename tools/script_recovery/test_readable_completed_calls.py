import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import _calls,inline_single_use
from tools.script_recovery.build_readable_unit import readable_file

CASES=[
 ('quest:GetStateInt("x")','quest:SetStateInt("x", 1), value'),
 ('shared','write(42), value'),
 ('object.value','write(42), value'),
 ('first()','write(42), value'),
 ('first()','write "42", value'),
 ('first()','write {42}, value'),
 ('first()','factory() (value)'),
 ('first()','consumer(write "42", value)'),
 ('first()','consumer({call=write "42"}, value)'),
 ('first()','consumer(value)'),
 ('quest:GetStateInt("x")','quest:Consume(value)'),
 ('first()','({key=write "42"})["key"], value'),
]

def execute(source,fault):
    lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().fault=fault
    return lua.execute('''local quest={};shared=0;object={value=0};local events={};local count=0
local function note(s)events[#events+1]=s;count=count+1;if count==fault then error('injected',0)end end
function quest:GetStateInt()note('get');return shared end
function quest:SetStateInt(k,v)note('set');shared=v;object.value=v;return 0 end
function first()note('first');return 7 end
function write(v)note('write');shared=42;object.value=42;return 0 end
function consumer(...)note('consume');return 99 end
function factory()note('factory');return consumer end
function quest:Consume(v)return consumer(v)end
'''+source+'''
local values=table.pack(pcall(Main,quest));local out={}
for i=1,values.n do out[#out+1]=tostring(values[i])end
return table.concat(out,','),shared,object.value,table.concat(events,',')
''')

@pytest.mark.parametrize('expression,tail',CASES)
@pytest.mark.parametrize('stage',['inline','pipeline'])
@pytest.mark.parametrize('fault',[0,1,2,3])
def test_completed_calls_preserve_value_effect_order_and_errors(expression,tail,stage,fault):
    source='function Main(quest)\n    local value\n    value = '+expression+'\n    return '+tail+'\nend\n'
    if stage=='inline':after=''.join(inline_single_use(source.splitlines(True))[0])
    else:after=readable_file(source)[0]
    assert execute(after,fault)==execute(source,fault)

@pytest.mark.parametrize('prefix,names',[
 ('consumer(',[]),
 ('consumer(write "42", ',['write']),
 ('consumer({key=write "42"}, ',['write']),
 ('factory()(',['factory']),
 ('quest:GetWriter "key"(',['GetWriter']),
 ('write { key=',[]),
 ('write {}',['write']),
 ('a[b](',[]),
 ('outer(inner(3), ',['inner']),
 ('outer(inner(")"), ',['inner']),
 ('outer(function() end, ',[]),
])
def test_completed_call_scanner_excludes_open_outer_call(prefix,names):
    assert [c[2] for c in _calls(prefix,completed_only=True)]==names


ALIAS_BODIES=[
 'local copy\n    copy = shared\n    mutate()\n    return copy',
 'local original, copy\n    copy = original\n    original = 42\n    return copy',
 'local original, copy, i\n    i = 0\n    while i < 2 do\n        original = i\n        if i == 0 then\n            copy = original\n        end\n        i = i + 1\n    end\n    return copy',
 'local original, copy\n    original = 0\n    local function change()\n        original = 42\n    end\n    copy = original\n    change()\n    return copy',
 'local original, copy\n    original = 1\n    copy = original\n    if true then\n        local original = 42\n        consume(original)\n    end\n    return copy',
 'local copy\n    if true then\n        local original = 42\n        copy = original\n    end\n    return copy',
 'local original, copy\n    original = 7\n    copy = original\n    consume(42)\n    return copy',
 'local copy\n    copy = parameter\n    mutate()\n    return copy',
]

@pytest.mark.parametrize('body',ALIAS_BODIES)
@pytest.mark.parametrize('generated',[False,True])
@pytest.mark.parametrize('stage',['copies','pipeline'])
def test_alias_source_scope_dominance_and_mutation(body,generated,stage):
    if generated:body=body.replace('original','r1').replace('copy','r2')
    source='function Main(parameter)\n    '+body+'\nend\n'
    if stage=='copies':
        from tools.script_recovery.readable_style import propagate_copies
        after=''.join(propagate_copies(source.splitlines(True))[0])
    else:after=readable_file(source)[0]
    def run(text):
        lua=LuaRuntime(unpack_returned_tuples=True)
        return lua.execute('''shared=0;local events={}
function mutate()events[#events+1]='mutate';shared=42 end
function consume(v)events[#events+1]='consume:'..tostring(v)end
'''+text+'''
local values=table.pack(pcall(Main,17));local out={}
for i=1,values.n do out[#out+1]=tostring(values[i])end
return table.concat(out,','),table.concat(events,','),shared
''')
    assert run(after)==run(source)


@pytest.mark.parametrize('store',['shared = 42','object.value = 42','object["value"] = 42','mutate()'])
@pytest.mark.parametrize('stage',['inline','pipeline'])
@pytest.mark.parametrize('fault',[0,1,2,3])
def test_evaluation_does_not_cross_observable_store(store,stage,fault):
    # mutate is defined here to use the same logged mutation as write.
    source='function Main(quest)\n    local value\n    value = first()\n    '+store.replace('mutate()','write(42)')+'\n    return value, shared, object.value\nend\n'
    if stage=='inline':after=''.join(inline_single_use(source.splitlines(True))[0])
    else:after=readable_file(source)[0]
    assert execute(after,fault)==execute(source,fault)


@pytest.mark.parametrize('stage',['inline','pipeline'])
def test_captured_local_write_cannot_cross_reader(stage):
    source='''function Main(quest)
    local state, value
    state = 0
    local function read()
        return state
    end
    value = read()
    state = 42
    return value, state
end
'''
    if stage=='inline':after=''.join(inline_single_use(source.splitlines(True))[0])
    else:after=readable_file(source)[0]
    assert execute(after,0)==execute(source,0)


@pytest.mark.parametrize('stage',['copies','inline','pipeline'])
def test_upvalue_snapshot_survives_external_closure_write(stage):
    prefix='''local external = 0
local function mutate()
    external = 42
end
'''
    body='''function Main()
    local copy
    copy = external
    mutate()
    return copy
end
'''
    source=prefix+body
    if stage=='pipeline':after=readable_file(source)[0]
    else:
        from tools.script_recovery.readable_style import propagate_copies
        operation=propagate_copies if stage=='copies' else inline_single_use
        after=prefix+''.join(operation(body.splitlines(True))[0])
    lua=LuaRuntime(unpack_returned_tuples=True)
    assert lua.execute(after+'\nreturn Main()')==0

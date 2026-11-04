import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_unit import readable_file
from tools.script_recovery.readable_style import cosmetics,tidy_blank_lines,strip_redundant_parens

def execute(source):
    lua=LuaRuntime(unpack_returned_tuples=True)
    return lua.execute('''
local function encode(...) local t=table.pack(...);local out={tostring(t.n)};for i=1,t.n do out[#out+1]=type(t[i])..':'..tostring(t[i]) end;return table.concat(out,'|') end
function values() return 7,nil,9 end
function nothing() end
function count(...) return encode(...) end
'''+source+'\nreturn encode(Main(7,nil,9))')

@pytest.mark.parametrize('expression',['values()','...','nothing()',"values 'x'",'values{}','7,8','values(),values()'])
@pytest.mark.parametrize('context',[
    'return value','return count(value)','return count(1,value)',
    'local t={value}\n    return count(table.unpack(t,1,3))',
    'return count(value,1)',
])
def test_inlined_assignment_keeps_one_value(expression,context):
    source='function Main(...)\n    local value = '+expression+'\n    '+context+'\nend\n'
    assert execute(readable_file(source)[0])==execute(source)

@pytest.mark.parametrize('body',[
    'return (values())','return (...)','return count((values()))',
    'return count(1,(values()))','return count((...),1)',
    'local t={(values())}\n    return count(table.unpack(t,1,3))',
    'local a,b=(values())\n    return a,b',
])
def test_explicit_group_retains_arity(body):
    source='function Main(...)\n    '+body+'\nend\n'
    direct=''.join(strip_redundant_parens(source.splitlines(True))[0])
    assert execute(direct)==execute(source)
    assert execute(readable_file(source)[0])==execute(source)

@pytest.mark.parametrize('literal',[
    '"cost + -1"',"'(#items * 12) / 12'",'[=[x + -3]=]',
    '[=[one\n\n\nthree]=]',
    '[=[\n    local result = 7\n    return result\n]=]',
])
def test_cosmetic_text_opaque(literal):
    source='function Main()\n    return '+literal+'\nend\n'
    assert execute(''.join(cosmetics(source.splitlines(True))[0]))==execute(source)
    assert execute(tidy_blank_lines(source))==execute(source)
    assert execute(readable_file(source)[0])==execute(source)

@pytest.mark.parametrize('expression',['3 + -2','3 + -2^2','3 + -2.5','(3 + -2) * 4'])
def test_arithmetic_still_equivalent(expression):
    source='function Main()\n    return '+expression+'\nend\n'
    assert execute(readable_file(source)[0])==execute(source)


@pytest.mark.parametrize('opaque',[
    '[=[one\n\nthree]=]',
    '[=[\nlocal function imagined()\nend\n]=]',
    '"escaped\\\nnewline"',
])
def test_pipeline_preserves_unsupported_multiline_text_explicitly(opaque):
    source='function Main()\n    return '+opaque+'\nend\n'
    after,report=readable_file(source)
    assert after==source and report.get('preservedMultilineText') is True
    assert not report.get('error')
    assert execute(after)==execute(source)


def test_multiline_diagnostic_is_preserved_verbatim():
    source='''--[=[Unresolved diagnostic
    local value = 7
    return value
]=]
function Main()
    return 9
end
'''
    after,report=readable_file(source)
    assert after==source and report.get('preservedMultilineText') is True


@pytest.mark.parametrize('expr',['values()','...'])
def test_local_assignment_before_close_handler_keeps_call_boundary(expr):
    source='''function Main(...)
    local owned <close> = setmetatable({}, {__close=function() events[#events+1]='close' end})
    local value = EXPR
    return value
end
'''.replace('EXPR',expr)
    def run(text):
        lua=LuaRuntime(unpack_returned_tuples=True)
        return lua.execute("events={};function values() events[#events+1]='call';return 7,8 end\n"+text+"\nlocal t=table.pack(Main(7,8));return t.n,t[1],table.concat(events,',')")
    assert run(readable_file(source)[0])==run(source)


def test_yielded_call_keeps_one_resumed_result():
    source='''function Main()
    local value = coroutine.yield(7,8)
    return value
end
'''
    def run(text):
        lua=LuaRuntime(unpack_returned_tuples=True)
        return lua.execute(text+'''
local co=coroutine.create(Main)
local first=table.pack(coroutine.resume(co))
local second=table.pack(coroutine.resume(co,9,10))
return first.n,first[1],first[2],first[3],second.n,second[1],second[2],coroutine.status(co)
''')
    assert run(readable_file(source)[0])==run(source)


@pytest.mark.parametrize('literal',['"x + -2"',"'(#array * 12) / 12'"])
def test_code_and_opaque_text_on_same_line(literal):
    source='function Main()\n    return 3 + -2,'+literal+' -- cost + -3\nend\n'
    direct=''.join(cosmetics(source.splitlines(True))[0])
    assert literal in direct and '-- cost + -3' in direct
    assert execute(direct)==execute(source)


def test_discarded_initializer_values_keep_their_effects():
    source='''function Main()
    local value = first(), discarded()
    return value
end
'''
    def run(text):
        lua=LuaRuntime(unpack_returned_tuples=True)
        return lua.execute("events={};function first() events[#events+1]='first';return 7,8 end;function discarded() events[#events+1]='discarded';return 9 end\n"+text+"\nlocal t=table.pack(Main());return t.n,t[1],table.concat(events,',')")
    assert run(readable_file(source)[0])==run(source)==(1,7,'first,discarded')


@pytest.mark.parametrize('trailing',[',',';',''])
@pytest.mark.parametrize('inlined',[False,True])
def test_last_table_field_preserves_one_value(trailing,inlined):
    source='function Main()\n'
    if inlined:source+='    local value = values()\n'
    source+='    local result = {'+('value' if inlined else '(values())')+trailing+'}\n    return #result\nend\n'
    assert execute(readable_file(source)[0])==execute(source)


@pytest.mark.parametrize('inlined',[False,True])
def test_generic_for_keeps_single_iterator_value(inlined):
    source='''function factory()
    return function(state,index) if index==nil then return 1 end end,{},99
end
function Main()
'''
    if inlined:source+='    local iterator = factory()\n'
    source+='    local count = 0\n    for item in '+('iterator' if inlined else '(factory())')+' do\n        count = count + 1\n    end\n    return count\nend\n'
    assert execute(readable_file(source)[0])==execute(source)


def test_single_line_diagnostic_does_not_disable_blank_cleanup():
    source='function Main()\n\n    local value = nil --[[unresolved native value]]\n\n\n    return value\nend\n'
    after=tidy_blank_lines(source)
    assert '--[[unresolved native value]]' in after and '\n\n' not in after
    assert execute(after)==execute(source)


@pytest.mark.parametrize('expr',['((values()))','(((values())))','((...))','("a" .. tostring(7))'])
def test_nested_groups_preserve_only_needed_value_adjustment(expr):
    source='function Main(...)\n    local value = '+expr+'\n    return count(value)\nend\n'
    assert execute(readable_file(source)[0])==execute(source)

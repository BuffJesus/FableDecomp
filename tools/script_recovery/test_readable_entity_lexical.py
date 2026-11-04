from pathlib import Path
import json
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import alias_entity_state,inline_entity_fields
CASES=json.loads((Path(__file__).parent/'fixtures/entity_lexical/cases.json').read_text(encoding='utf-8'))
NORMAL=next(case['source'].split('local payload',1)[0] for case in CASES if case['name']=='normal_long_literal')

def transform(source):
    source,_=alias_entity_state(source)
    return inline_entity_fields(source)[0]
def execute(source,argument=None):
    vm=LuaRuntime(unpack_returned_tuples=True);vm.execute(source)
    return vm.globals().Main(vm.eval(argument)) if argument else vm.globals().Main()

@pytest.mark.parametrize('case',CASES,ids=[case['name'] for case in CASES])
def test_values_and_opaque_fragments_survive(case):
    source=case['source'];after=transform(source)
    assert execute(after,case.get('argument'))==execute(source,case.get('argument'))
    assert case.get('opaque') is None or case['opaque'] in after

@pytest.mark.parametrize('kind,value',[('Bool','false'),('Int','0'),('Float','1.25'),('String','"value"')])
def test_real_primitive_reads_and_writes_inline(kind,value):
    source=NORMAL+'function Main()\nstate:Set'+kind+'("Actual", '+value+')\nreturn state:Get'+kind+'("Actual")\nend\n'
    after,count=inline_entity_fields(source)
    assert count==1 and 'local state = {}' not in after
    assert execute(after)==execute(source)

def test_thing_identity_stays_retained_by_field():
    source=NORMAL+'''local marker={id=41}
function Main()
state:SetThing("Actual", marker)
local captured=state:GetThing("Actual")
return captured==marker,captured.id
end
'''
    assert execute(transform(source))==execute(source)==(True,41)

@pytest.mark.parametrize('operator', ['==','~=','<','>','<=','>=','&','|','~','<<','>>','#'])
def test_multiline_expression_setter_stays_a_call(operator):
    expression = '1 ' + operator if operator != '#' else '#'
    source=NORMAL+'function Main()\nreturn '+expression+'\nstate:SetInt("Actual",4)\nend\n'
    LuaRuntime().compile(source)
    after,count=inline_entity_fields(source)
    assert after==source and count==0

@pytest.mark.parametrize('context',[
    'CALL\nreturn 4', 'do CALL end\nreturn 4',
    'if true then CALL else return 0 end\nreturn 4',
    'if false then return 0 else CALL end\nreturn 4',
    'repeat CALL until true\nreturn 4', '::again:: CALL;return 4',
    'local x=1;CALL;return x', 'while false do CALL end\nreturn 4',
])
def test_discarded_getter_call_remains_valid_statement(context):
    source=NORMAL+'function Main()\n'+context.replace('CALL','state:GetInt("Actual")')+'\nend\n'
    after,count=inline_entity_fields(source)
    assert after==source and count==0
    assert execute(after)==execute(source)

@pytest.mark.parametrize('suffix',[':touch()', '.touch()', '["touch"]()', '()', '{}', '"argument"', '[=[argument]=]'])
def test_chained_setter_keeps_side_effect_before_nil_result_error(suffix):
    source=NORMAL+'''local count=0
local marker=setmetatable({touch=function() count=count+1 end},{__call=function() count=count+1 end})
function Main()
local ok=pcall(function() state:SetThing("Actual",marker)'''+suffix+''' end)
return ok,count,state:GetThing("Actual")==marker
end
'''
    after,count=inline_entity_fields(source)
    assert after==source and count==0
    assert execute(after)==execute(source)==(False,0,True)

@pytest.mark.parametrize('suffix',[':touch()', '.touch()', '["touch"]()', '()', '{}', '"argument"', '[=[argument]=]'])
def test_getter_result_chaining_retains_identity_and_call_effects(suffix):
    source=NORMAL+'''local count=0
local marker=setmetatable({touch=function() count=count+1 end},{__call=function() count=count+1 end})
function Main()
state:SetThing("Actual",marker)
state:GetThing("Actual")'''+suffix+'''
return count,state:GetThing("Actual")==marker
end
'''
    after,count=inline_entity_fields(source)
    assert count==1
    assert execute(after)==execute(source)==(1,True)

@pytest.mark.parametrize('code',[
    'state:SetInt("Actual", 4, bump())\nreturn state:GetInt("Actual", bump()),count',
    'state:SetInt("Actual" .. suffix, 4)\nreturn state:GetInt("Actual" .. suffix)',
    "state:SetInt('Actual', 4)\nreturn state:GetInt('Actual')",
    'return state:SetInt("Actual", 4),state:GetInt("Actual")',
    'local function accept(a,b) return a,b end\nreturn accept(\nstate:SetInt("Actual",4),state:GetInt("Actual"))',
    'state:SetInt("A",3)\nstate:SetInt("Actual",state:GetInt("A")+2)\nreturn state:GetInt("Actual")',
    'state:SetInt("Actual",state:SetInt("A",4))\nreturn state:GetInt("Actual"),state:GetInt("A")',
])
def test_dynamic_extra_arguments_and_setter_expression_effects_survive(code):
    source=NORMAL+'''local count,suffix=0,"Suffix"
local function bump() count=count+1;return count end
function Main()
'''+code+'\nend\n'
    assert execute(transform(source))==execute(source)

@pytest.mark.parametrize('text',[
    'someValue = (literal)',
    'function Example()\nlocal actual = state:GetInt("Actual")\nreturn actual\nend',
    'state:GetInt(dynamic)\nstate:SetInt("Actual", 999)',
    'state:GetInt("Actual")\nstate:GetThing("Ghost")',
])
def test_opaque_snapshot_cosmetic_and_dynamic_lookalikes_are_unchanged(text):
    source=NORMAL+'local payload = [==[\n'+text+'\n]==]\nfunction Main()\nstate:SetInt("Actual",4)\nreturn state:GetInt("Actual"),payload\nend\n'
    assert execute(transform(source))==execute(source)

def test_field_spelling_collisions_in_comments_do_not_create_fields():
    source=NORMAL+'-- state:GetInt("Foo")\nfunction Main()\nstate:SetInt("foo",4)\nreturn state:GetInt("foo")\nend\n'
    after,count=inline_entity_fields(source)
    assert count==1 and '-- state:GetInt("Foo")' in after
    assert execute(after)==execute(source)==4

@pytest.mark.parametrize('mutation',[
    'state:SetInt("Actual",2)', 'mutate()',
    'local thread=coroutine.create(mutate);coroutine.resume(thread)',
])
def test_snapshot_retains_value_across_direct_helper_and_coroutine_writes(mutation):
    source=NORMAL+'''state:SetInt("Actual", 1)
local function mutate()
state:SetInt("Actual", 2)
end
function Main()
local actual = state:GetInt("Actual")
'''+mutation+'''
return actual,state:GetInt("Actual")
end
'''
    assert execute(transform(source))==execute(source)==(1,2)

def test_snapshot_name_does_not_capture_a_local_in_another_function():
    source=NORMAL+'''state:SetInt("Actual", 1)
local function other()
local actual = 77
return actual,state:GetInt("Actual")
end
function Main()
local actual = state:GetInt("Actual")
local shadow,field=other()
return actual,shadow,field
end
'''
    assert execute(transform(source))==execute(source)==(1,77,1)

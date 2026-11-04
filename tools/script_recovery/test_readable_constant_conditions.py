"""Behavioral tests to promote with the constant-fold correction."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_unit import readable_file
from tools.script_recovery.build_readable_unit import prune_empty_ifs
from tools.script_recovery.readable_style import fold_constant_conditions, prune_dead_bitmask_staging

def execute(source, *args):
    lua=LuaRuntime(unpack_returned_tuples=True)
    # Encode type, arity, value and calls without confusing nil with no return.
    return lua.execute('trace={}\n'+source+'''
local result=table.pack(pcall(Main,...))
local out={tostring(result.n)}
for i=1,result.n do out[#out+1]=type(result[i])..':'..tostring(result[i]) end
return table.concat(out,'|'),table.concat(trace,',')
''',*args)

def variants(source):
    yield ''.join(fold_constant_conditions(source.splitlines(True))[0])
    yield readable_file(source)[0]

@pytest.mark.parametrize('literal',[
    '"not false"',"'not true'",'[[true and keep]]','[=[keep or false]=]',
    '[=[\nnot false\nif false then\nend\n]=]',
    '"if 1 ~= 0 then"','"false or true and not false"',
])
def test_literal_contents_are_opaque(literal):
    source='function Main()\n    return '+literal+'\nend\n'
    for styled in variants(source):
        assert execute(styled)==execute(source)

@pytest.mark.parametrize('expression',[
    'value and true','value or false','true and value','false or value',
    'not true','not false','not not true','not 1 ~= 0','not 0 == 0',
    '1 == 0 + 1','1 ~= 0 * 0','1 == 0 or value','value and 1 ~= 0',
    '1 ~= 0','0 == 0','1 & 1 ~= 0',
])
@pytest.mark.parametrize('value',[None,False,True,0,7,''])
def test_operand_values_precedence_and_truthiness(expression,value):
    source='function Main(value)\n    return '+expression+'\nend\n'
    for styled in variants(source):
        assert execute(styled,value)==execute(source,value)

@pytest.mark.parametrize('expression',['true and values()','false or values()'])
@pytest.mark.parametrize('context',[
    'return EXPR',
    'local t={EXPR}\nreturn #t,t[1],t[2]',
    'return count(EXPR)',
])
def test_boolean_expression_keeps_single_result(expression,context):
    body=context.replace('EXPR',expression)
    source='''local function values()
    trace[#trace+1]='values'
    return 7,8
end
local function count(...) return select('#',...),... end
function Main()
'''+''.join('    '+line+'\n' for line in body.splitlines())+'end\n'
    for styled in variants(source):
        assert execute(styled)==execute(source)

@pytest.mark.parametrize('inline',[False,True])
@pytest.mark.parametrize('truth',['true','false'])
def test_constant_branch_preserves_scope_close_order_and_return_placement(inline,truth):
    if inline:
        body='    if '+truth+' then return 1 end\n    return 2\n'
    else:
        body='''    local value=1
    if TRUTH then
        local value=2
        local owned <close> = setmetatable({}, {__close=function() trace[#trace+1]='close' end})
        trace[#trace+1]=tostring(value)
    end
    trace[#trace+1]='after'
    return value
'''.replace('TRUTH',truth)
    source='function Main()\n'+body+'end\n'
    for styled in variants(source):
        assert execute(styled)==execute(source)

@pytest.mark.parametrize('text',[
    '-- not false and true or false\n',
    '--[[\nif false then\n    nativeDiagnostic()\nend\nnot false\n]]\n',
])
def test_comment_text_is_not_rewritten(text):
    source='function Main()\n'+text+'    return 1\nend\n'
    styled=''.join(fold_constant_conditions(source.splitlines(True))[0])
    assert text in styled

def test_false_branch_with_alternate_label_entry_is_preserved():
    source='''function Main()
    goto inside
    if false then
        ::inside::
        return 7
    end
    return 9
end
'''
    # Lua permits a jump only within visible label scope; keep the unresolved
    # input intact rather than hiding the invalid edge in a presentation pass.
    assert fold_constant_conditions(source.splitlines(True))==(source.splitlines(True),0)

def test_live_constant_folds_still_reduce_simple_native_predicates():
    source='function Main()\n    if 1 ~= 0 then\n        return not false\n    end\nend\n'
    styled,count=fold_constant_conditions(source.splitlines(True))
    assert count and '1 ~= 0' not in ''.join(styled) and 'not false' not in ''.join(styled)
    assert execute(''.join(styled))==execute(source)

@pytest.mark.parametrize('truth',['true','false'])
def test_inline_else_is_not_mistaken_for_straight_statement(truth):
    source='function Main()\n    if '+truth+' then return 1 else return 2 end\nend\n'
    for styled in variants(source):
        assert execute(styled)==execute(source)

@pytest.mark.parametrize('predicate',[
    'probe(value)','object.value','not object.value',
    'quest:IsActiveThreadTerminating()',
])
@pytest.mark.parametrize('value',[None,False,True])
def test_empty_branch_retains_predicate_calls_and_global_lookups(predicate,value):
    source='''function Main(value)
    local function probe(v)
        trace[#trace+1]='query'
        return v
    end
    local object=setmetatable({}, {__index=function() return probe(value) end})
    local quest={IsActiveThreadTerminating=function() return probe(value) end}
    if PREDICATE then
    end
    return 7
end
'''.replace('PREDICATE',predicate)
    for styled in (prune_empty_ifs(source)[0],readable_file(source)[0]):
        assert execute(styled,value)==execute(source,value)

def test_empty_if_text_in_long_string_is_preserved_directly():
    source='return [=[\nif false then\nend\n]=]\n'
    assert prune_empty_ifs(source)==(source,0)

@pytest.mark.parametrize('condition',[
    '(31 & 16) ~= 0','(0xffffffff & 4294967295) == 0','(0 & 0) ~= 0',
])
def test_closed_uint32_mask_predicate_can_disappear(condition):
    source='function Main()\n    if '+condition+' then\n    end\n    return 7\nend\n'
    styled,count=prune_empty_ifs(source)
    assert count==1 and 'if ' not in styled
    assert execute(styled)==execute(source)

@pytest.mark.parametrize('condition',[
    '(0x100000000 & 1) ~= 0','(31.5 & 1) ~= 0','(value & 16) ~= 0',
    '(1 / 0) ~= 0','probe()','object.value',
])
def test_ambiguous_empty_predicate_remains(condition):
    source='function Main()\n    if '+condition+' then\n    end\nend\n'
    assert prune_empty_ifs(source)==(source,0)

@pytest.mark.parametrize('body',['', '        flags = flags & 15\n'])
def test_private_uint_chain_accepts_an_emptied_final_guard(body):
    source='''function Main()
    local flags
    flags = 31
    if (flags & 16) ~= 0 then
BODY    end
    if (flags & 8) ~= 0 then
    end
    return 7
end
'''.replace('BODY',body)
    result,count=prune_dead_bitmask_staging(source.splitlines(True))
    assert count and 'flags' not in ''.join(result)
    assert execute(''.join(result))==execute(source)

@pytest.mark.parametrize('change',['global','live','unknown','diagnostic'])
def test_empty_guard_does_not_relax_private_uint_proof(change):
    source='''function Main()
    local flags
    flags = 31
    if (flags & 8) ~= 0 then
    end
    return 7
end
'''
    if change=='global':source=source.replace('    local flags\n','')
    elif change=='live':source=source.replace('return 7','return flags')
    elif change=='unknown':source=source.replace('flags = 31','flags = probe()')
    elif change=='diagnostic':source=source.replace('    end\n','        -- native unresolved\n    end\n')
    assert prune_dead_bitmask_staging(source.splitlines(True))==(source.splitlines(True),0)

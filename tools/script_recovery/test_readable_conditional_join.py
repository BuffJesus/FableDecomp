"""A two-jump literal exit must retain condition order and local close order."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import fold_literal_exit_join
from tools.script_recovery.build_readable_unit import readable_file

SOURCE='''local outer,inner,failure=...
local trace={}
local function close(name)
    return setmetatable({}, {__close=function()
        trace[#trace+1]='close:'..name
        if failure=='close:'..name then error('controlled:'..name) end
    end})
end
local function check(name,value)
    trace[#trace+1]=name
    if failure==name then error('controlled:'..name) end
    return value
end
local function Main(outer,inner)
    local function mark(name)
        trace[#trace+1]=name
        if failure==name then error('controlled:'..name) end
    end
    if outer then
        goto JOIN
    else
        local owned <close> = close('prefix')
        mark('prefix')
        if inner then
            local temporary <close> = close('body')
            mark('body')
            goto JOIN
        end
    end
    do return false end
    ::JOIN::
    mark('join')
    return true
end
local packed=table.pack(pcall(Main,outer,inner))
return packed.n,packed[1],packed[2],table.concat(trace,',')
'''


def run(source,outer,inner,failure=None):
    return LuaRuntime(unpack_returned_tuples=True).execute(source,outer,inner,failure)


@pytest.mark.parametrize('outer',[None,False,True,0,1,''])
@pytest.mark.parametrize('inner',[None,False,True,0,1,''])
def test_truthiness_and_close_order(outer,inner):
    result,count=fold_literal_exit_join(SOURCE)
    assert count==1
    assert run(result,outer,inner)==run(SOURCE,outer,inner)


@pytest.mark.parametrize('ret',['return','return nil','return true','return false'])
@pytest.mark.parametrize('outer,inner',[(False,False),(False,True),(True,False),(True,True)])
def test_return_arity_and_literals(ret,outer,inner):
    source=SOURCE.replace('do return false end','do '+ret+' end')
    result,count=fold_literal_exit_join(source)
    assert count==1 and run(result,outer,inner)==run(source,outer,inner)


@pytest.mark.parametrize('failure',['outer','inner','prefix','body','close:body','close:prefix'])
def test_calls_and_error_unwinding(failure):
    source=SOURCE.replace('if outer then',"if check('outer',outer) then").replace('if inner then',"if check('inner',inner) then")
    # Calls with string operands are outside the conservative condition grammar.
    source=source.replace("check('outer',outer)",'check(outerName,outer)').replace("check('inner',inner)",'check(innerName,inner)')
    source="local outerName,innerName='outer','inner'\n"+source
    result,count=fold_literal_exit_join(source)
    assert count==1
    before=run(source,False,True,failure)
    after=run(result,False,True,failure)
    assert before==after
    assert before[1] is False


def test_shadowed_close_locals_keep_lifo_order():
    source=SOURCE.replace('local temporary <close>','local owned <close>')
    result,count=fold_literal_exit_join(source)
    assert count==1 and run(result,False,True)==run(source,False,True)
    assert run(result,False,True)[-1]=='prefix,body,close:body,close:prefix,join'


@pytest.mark.parametrize('change',[
    'variable','call','string','number','multiple','extra_jump','duplicate_label',
    'nested_body','prefix_label','body_jump','return_comment','label_comment',
    'different_target','whole_string','whole_comment','string_condition',
])
def test_ambiguous_edges_and_text_are_preserved(change):
    source=SOURCE
    if change in ('variable','call','string','number','multiple'):
        value={'variable':'outer','call':'check(outer,inner)','string':'"text"','number':'7','multiple':'true,false'}[change]
        source=source.replace('do return false end','do return '+value+' end')
    elif change=='extra_jump':source=source.replace('    if outer then','    if bypass then goto JOIN end\n    if outer then')
    elif change=='duplicate_label':source=source.replace('local packed=', 'local function Other()\n    ::JOIN::\nend\nlocal packed=')
    elif change=='nested_body':source=source.replace("            mark('body')", "            if inner then\n                mark('body')\n            end")
    elif change=='prefix_label':source=source.replace("        mark('prefix')", "        ::side::\n        mark('prefix')")
    elif change=='body_jump':source=source.replace("            mark('body')", "            goto side\n            mark('body')")
    elif change=='return_comment':source=source.replace('do return false end','do return false end -- diagnostic')
    elif change=='label_comment':source=source.replace('    ::JOIN::','    ::JOIN:: -- diagnostic')
    elif change=='different_target':source=source.replace('    ::JOIN::','    ::OTHER::')
    elif change=='whole_string':source='return [=[\n'+source+']=]'
    elif change=='whole_comment':source='--[=[\n'+source+']=]\nreturn 7'
    elif change=='string_condition':source=source.replace('if outer then','if "text" then')
    assert fold_literal_exit_join(source)==(source,0)


def test_body_diagnostics_are_preserved():
    source=SOURCE.replace("            mark('body')", "            -- native assignment evidence\n            mark('body')")
    result,count=fold_literal_exit_join(source)
    assert count==1 and '-- native assignment evidence' in result
    assert run(result,False,True)==run(source,False,True)


@pytest.mark.parametrize('equal,terminating',[(False,False),(False,True),(True,False),(True,True)])
def test_full_callback_pipeline(equal,terminating):
    source='''function Main(quest, equal, terminating)
    return quest:WithRetailResources(function(resources)
    if not equal then
        goto JOIN
    else
        quest:Mark("query")
        if not terminating then
            quest:Mark("assign")
            goto JOIN
        end
    end
    do return false end
    ::JOIN::
    quest:Mark("join")
    return true
    end)
end
'''
    result,_=readable_file(source)
    assert 'goto JOIN' not in result
    harness='''
local trace={}
local q={Mark=function(_,v) trace[#trace+1]=v end,
WithRetailResources=function(_,callback)
    local result=callback({})
    trace[#trace+1]='cleanup'
    return result
end}
return Main(q,...),table.concat(trace,',')
'''
    assert LuaRuntime(unpack_returned_tuples=True).execute(result+harness,equal,terminating)==LuaRuntime(unpack_returned_tuples=True).execute(source+harness,equal,terminating)

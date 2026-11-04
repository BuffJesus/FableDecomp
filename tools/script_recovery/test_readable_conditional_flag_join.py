"""Flag writes cannot move across a predicate or a local close handler."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import fold_literal_flag_join
from tools.script_recovery.build_readable_unit import readable_file

SOURCE='''local outer,inner,failure=...
local flag
flag=99
local trace={}
local outerName,innerName='outer','inner'
local function check(name,value)
    trace[#trace+1]=name..':'..tostring(flag)
    if failure==name then error('controlled:'..name) end
    return value
end
local function Main(outer,inner)
    if check(outerName,outer) then
        goto FALLBACK
    else
        check('prefix',true)
        flag = 1
        if check(innerName,inner) then goto FALLBACK end
    end
    goto DONE
    ::FALLBACK::
    flag = 0
    ::DONE::
    return flag
end
local packed=table.pack(pcall(Main,outer,inner))
return packed.n,packed[1],packed[2],flag,table.concat(trace,',')
'''


def run(source,outer,inner,failure=None):
    return LuaRuntime(unpack_returned_tuples=True).execute(source,outer,inner,failure)


def constants(initial,fallback):
    return (SOURCE.replace('flag = 1','flag = __INITIAL_VALUE__')
            .replace('flag = 0','flag = __FALLBACK_VALUE__')
            .replace('__INITIAL_VALUE__',initial).replace('__FALLBACK_VALUE__',fallback))


@pytest.mark.parametrize('initial,fallback',[('1','0'),('0','1'),('true','false'),('false','true')])
@pytest.mark.parametrize('outer',[None,False,True,0,1,''])
@pytest.mark.parametrize('inner',[None,False,True,0,1,''])
def test_truthiness_and_predicate_observed_flag(initial,fallback,outer,inner):
    source=constants(initial,fallback)
    result,count=fold_literal_flag_join(source)
    assert count==1
    assert run(result,outer,inner)==run(source,outer,inner)


@pytest.mark.parametrize('initial,fallback',[('1','0'),('0','1'),('true','false'),('false','true')])
@pytest.mark.parametrize('failure',['outer','prefix','inner'])
def test_exception_observes_write_order(initial,fallback,failure):
    source=constants(initial,fallback)
    result,count=fold_literal_flag_join(source)
    assert count==1
    before=run(source,False,True,failure)
    assert before[1] is False
    assert run(result,False,True,failure)==before


def test_local_close_can_observe_flag_before_shared_store():
    source=SOURCE.replace("        check('prefix',true)",
        "        local owned <close> = setmetatable({}, {__close=function() trace[#trace+1]='close:'..tostring(flag) end})\n        check('prefix',true)")
    assert fold_literal_flag_join(source)==(source,0)
    result=run(source,False,True)
    assert result[3]==0 and result[-1].endswith('inner:1,close:1')


@pytest.mark.parametrize('change',[
    'plain_local','nested','label','global','late','closed','shadow','parameter',
    'loop_binding','extra_fallback','extra_done','duplicate_label','goto_comment',
    'label_comment','store_comment','same_value','different_value','environment',
    'whole_string','whole_comment','string_condition',
])
def test_ambiguous_binding_scope_or_edge_is_preserved(change):
    source=SOURCE
    if change=='plain_local':source=source.replace("        check('prefix',true)", "        local unused = 7\n        check('prefix',true)")
    elif change=='nested':source=source.replace("        check('prefix',true)", "        if inner then check('prefix',true) end")
    elif change=='label':source=source.replace("        check('prefix',true)", "        ::side::\n        check('prefix',true)")
    elif change=='global':source=source.replace('local flag\n','')
    elif change=='late':source=source.replace('local flag\n','').replace('local packed=', 'local flag\nlocal packed=')
    elif change=='closed':source=source.replace('local flag\n','do\nlocal flag\nend\n')
    elif change=='shadow':source=source.replace('local function Main(outer,inner)\n','local function Main(outer,inner)\n    local flag = 88\n')
    elif change=='parameter':source=source.replace('local function Main(outer,inner)','local function Main(outer,inner,flag)')
    elif change=='loop_binding':source=source.replace('local function Main(outer,inner)\n','local function Main(outer,inner)\n    for flag=1,1 do end\n')
    elif change=='extra_fallback':source=source.replace('    if check(outerName,outer)', '    if bypass then goto FALLBACK end\n    if check(outerName,outer)')
    elif change=='extra_done':source=source.replace('    if check(outerName,outer)', '    if bypass then goto DONE end\n    if check(outerName,outer)')
    elif change=='duplicate_label':source=source.replace('local packed=', 'local function Other()\n    ::DONE::\nend\nlocal packed=')
    elif change=='goto_comment':source=source.replace('    goto DONE\n','    goto DONE -- diagnostic\n')
    elif change=='label_comment':source=source.replace('    ::DONE::','    ::DONE:: -- diagnostic')
    elif change=='store_comment':source=source.replace('    flag = 0','    flag = 0 -- diagnostic')
    elif change=='same_value':source=source.replace('flag = 0','flag = 1')
    elif change=='different_value':source=source.replace('flag = 0','flag = 7')
    elif change=='environment':source=source.replace('flag','_ENV')
    elif change=='whole_string':source='return [=[\n'+source+']=]'
    elif change=='whole_comment':source='--[=[\n'+source+']=]\nreturn 7'
    elif change=='string_condition':source=source.replace('if check(outerName,outer)','if "text"')
    assert fold_literal_flag_join(source)==(source,0)


def test_prefix_diagnostic_is_kept():
    source=SOURCE.replace("        check('prefix',true)", "        -- native reply-string evidence\n        check('prefix',true)")
    result,count=fold_literal_flag_join(source)
    assert count==1 and '-- native reply-string evidence' in result
    assert run(source,False,True)==run(result,False,True)


@pytest.mark.parametrize('outer,inner',[(False,False),(False,True),(True,False),(True,True)])
def test_full_callback_pipeline(outer,inner):
    source='''function Main(quest, outer, inner)
    return quest:WithRetailResources(function(resources)
    local native_reply_available
    if outer then
        goto FALLBACK
    else
        quest:Mark("text lookup")
        native_reply_available = 1
        if inner then goto FALLBACK end
    end
    goto DONE
    ::FALLBACK::
    native_reply_available = 0
    ::DONE::
    quest:Mark("joined")
    return native_reply_available
    end)
end
'''
    result,_=readable_file(source)
    assert 'goto FALLBACK' not in result and 'goto DONE' not in result
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
    assert LuaRuntime(unpack_returned_tuples=True).execute(source+harness,outer,inner)==LuaRuntime(unpack_returned_tuples=True).execute(result+harness,outer,inner)

"""Unreachable jumps must not change return arity, cleanup or alternate entry."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import prune_unreachable_return_jumps
from tools.script_recovery.build_readable_unit import readable_file


def transform(source):
    lines,count=prune_unreachable_return_jumps(source.splitlines(True))
    return ''.join(lines),count


@pytest.mark.parametrize('ret',['',' nil',' true',' false'])
@pytest.mark.parametrize('fail',[False,True])
def test_return_arity_and_cleanup_errors(ret,fail):
    source='''local trace={}
local closer=setmetatable({}, {__close=function() trace[#trace+1]='close'; CLOSE_BODY end})
local function Main()
    local owned <close> = closer
    trace[#trace+1]='body'
    do returnRET end
    goto finish
    ::finish::
    trace[#trace+1]='unreachable'
end
local result=table.pack(pcall(Main))
return result.n,result[1],result[2],table.concat(trace,',')
'''.replace('RET',ret).replace('CLOSE_BODY',"error('controlled')" if fail else '')
    result,count=transform(source)
    assert count==1
    before=LuaRuntime(unpack_returned_tuples=True).execute(source)
    after=LuaRuntime(unpack_returned_tuples=True).execute(result)
    assert before==after
    assert after[-1]=='body,close'


@pytest.mark.parametrize('source',[
    'do return true end\n::entry::\ngoto finish\n',
    'if cond then return true end\ngoto finish\n',
    'do return true end\n-- diagnostic\ngoto finish\n',
    'do return true end -- diagnostic\ngoto finish\n',
    'do return true end\ngoto finish -- diagnostic\n',
    'do return "value" end\ngoto finish\n',
    'do return value end\ngoto finish\n',
    'do return call() end\ngoto finish\n',
    'return [[\ndo return true end\ngoto finish\n]]\n',
    'return [=[\ndo return true end\ngoto finish\n]=]\n',
    '--[[\ndo return true end\ngoto finish\n]]\n',
])
def test_ambiguous_or_diagnostic_text_is_preserved(source):
    assert transform(source)==(source,0)


@pytest.mark.parametrize('take_entry',[False,True])
def test_intervening_label_keeps_reachable_jump(take_entry):
    source='''function Main(entry)
    if entry then goto entry end
    do return false end
    ::entry::
    goto finish
    do return nil end
    ::finish::
    return true
end
'''
    result,count=transform(source)
    assert count==0
    assert LuaRuntime().execute(result+'return Main(...)',take_entry)==take_entry


@pytest.mark.parametrize('ret',['',' nil',' true',' false'])
def test_full_pipeline_preserves_callback_result_and_cleanup(ret):
    source='''function Main(quest, me)
    return quest:WithRetailResources(function(resources)
        quest:Mark("body")
        do returnRET end
        goto finish
        ::finish::
        quest:Mark("unreachable")
    end)
end
'''.replace('RET',ret)
    result,_=readable_file(source)
    assert 'goto finish' not in result
    harness='''
local trace={}
local q={Mark=function(_,v) trace[#trace+1]=v end,
WithRetailResources=function(_,callback)
    local r=table.pack(callback({}))
    trace[#trace+1]='cleanup'
    return table.unpack(r,1,r.n)
end}
local r=table.pack(Main(q))
return r.n,r[1],table.concat(trace,',')
'''
    assert LuaRuntime(unpack_returned_tuples=True).execute(result+harness)==LuaRuntime(unpack_returned_tuples=True).execute(source+harness)

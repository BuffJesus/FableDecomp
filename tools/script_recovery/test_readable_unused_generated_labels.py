"""Unused generated labels are presentation; live targets and diagnostics stay."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_style import prune_unused_generated_labels, style_source


@pytest.mark.parametrize('source,expected',[
    ('function Main()\n    ::LAB_unused::\n    return 7\nend\nreturn Main()',1),
    ('local trace={}\nlocal function Main()\n    local owned <close> = setmetatable({}, {__close=function() trace[#trace+1]=9 end})\n    ::FLOW_unused::\n    return 7\nend\nreturn Main(),table.concat(trace)',1),
    ('function Main()\n    goto LAB_live\n    ::LAB_live::\n    return 7\nend\nreturn Main()',0),
    ('function Main()\n    goto -- comment\n    LAB_live\n    ::LAB_live::\n    return 7\nend\nreturn Main()',0),
    ('function Main()\n    goto --[[ comment ]] LAB_live\n    ::LAB_live::\n    return 7\nend\nreturn Main()',0),
    ('function Main()\n    ::LAB_unused:: -- keep diagnostic\n    return 7\nend\nreturn Main()',0),
    ('function Main()\n    ::human_label::\n    return 7\nend\nreturn Main()',0),
    ('return [[\n::LAB_text::\n]]',0),
    ('return [=[\n::LAB_text::\n]=]',0),
    ('--[[\n::LAB_comment::\n]]\nreturn 7',0),
    ('goto LAB_global\nfunction Main() return 0 end\n::LAB_global::\nreturn 7',0),
])
def test_behavior_and_preserved_targets(source,expected):
    result,count=prune_unused_generated_labels(source)
    assert count==expected
    assert LuaRuntime(unpack_returned_tuples=True).execute(source)==LuaRuntime(unpack_returned_tuples=True).execute(result)
    if not count:assert source==result


def test_same_name_in_other_scope_conservatively_keeps_both():
    source='''function Main()
    goto LAB_live
    ::LAB_live::
    return 7
end
function Other()
    ::LAB_live::
    return 8
end
return Main(),Other()
'''
    assert prune_unused_generated_labels(source)==(source,0)


def test_string_and_comment_goto_lookalikes_do_not_keep_label():
    source='''function Main()
    -- goto LAB_unused
    local text = "goto LAB_unused"
    ::LAB_unused::
    return text
end
return Main()
'''
    result,count=prune_unused_generated_labels(source)
    assert count==1 and '-- goto LAB_unused' in result
    assert LuaRuntime().execute(result)==LuaRuntime().execute(source)


def test_last_file_cleanup_preserves_callback_calls():
    source='''function Main(quest)
    return quest:WithRetailResources(function(resources)
        quest:Mark("body")
        ::FLOW_unused::
        quest:Mark("after")
        return false
    end)
end
'''
    result,report=style_source(source)
    assert '::FLOW_unused::' not in result
    assert report['unusedGeneratedLabels']==1
    harness='''
local trace={}
local q={Mark=function(_,v) trace[#trace+1]=v end,
WithRetailResources=function(_,callback)
    local result=callback({})
    trace[#trace+1]='cleanup'
    return result
end}
return Main(q),table.concat(trace,',')
'''
    assert LuaRuntime(unpack_returned_tuples=True).execute(result+harness)==LuaRuntime(unpack_returned_tuples=True).execute(source+harness)==(False,'body,after,cleanup')

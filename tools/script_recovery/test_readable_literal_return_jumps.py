"""Literal return jumps keep callback boundaries, arity and cleanup order."""
import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.readable_style import fold_nested_return_jumps


def transform(source):
    lines,count=fold_nested_return_jumps(source.splitlines(True))
    return ''.join(lines),count


def run(source, early):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.globals().EARLY=early
    return lua.execute(source+'''
local events={}
local q={Mark=function(_,value) events[#events+1]=value end}
function q:WithRetailResources(callback)
 events[#events+1]='open'
 local result=table.pack(callback({}))
 events[#events+1]='close'
 return table.unpack(result,1,result.n)
end
local result=table.pack(Main(q,EARLY))
return result.n,result[1],table.concat(events,',')
''')


@pytest.mark.parametrize('value', ['', ' nil', ' true', ' false'])
@pytest.mark.parametrize('early', [False, True])
def test_callback_result_and_cleanup_are_preserved(value, early):
    source='''function Main(quest,early)
    return quest:WithRetailResources(function(resources)
    if early then goto DONE end
    quest:Mark("body")
    ::DONE::
    returnVALUE
    end)
end
'''.replace('VALUE',value)
    result,count=transform(source)
    assert count==1 and 'goto DONE' not in result
    assert run(source,early)==run(result,early)
    assert run(result,early)[2]==('open,close' if early else 'open,body,close')


def test_non_tail_jump_uses_return_block():
    source='''function Main(quest,early)
    return quest:WithRetailResources(function(resources)
    goto DONE
    quest:Mark("unreachable")
    ::DONE::
    return false
    end)
end
'''
    result,count=transform(source)
    assert count==1 and 'do return false end' in result
    assert run(result,True)==run(source,True)==(1,False,'open,close')


@pytest.mark.parametrize('expression', ['answer', 'answer()', '"text"', "'false'", '[[text]]', 'false, nil', '0'])
def test_other_return_expressions_are_not_moved(expression):
    source='function Main(quest,early)\n    if early then goto DONE end\n    ::DONE::\n    return '+expression+'\nend\n'
    result,count=transform(source)
    assert result==source and count==0


def test_duplicate_labels_in_separate_scopes_are_not_folded():
    source='''function Main(quest,early)
    if early then
        goto DONE
        ::DONE::
        return false
    else
        goto DONE
        ::DONE::
        return true
    end
end
'''
    result,count=transform(source)
    assert result==source and count==0


def test_named_closure_is_excluded():
    source='''function Main(quest,early)
    local function inner()
        if early then goto DONE end
        ::DONE::
        return false
    end
    return inner()
end
'''
    result,count=transform(source)
    assert result==source and count==0

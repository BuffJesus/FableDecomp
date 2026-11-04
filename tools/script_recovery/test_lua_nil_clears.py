"""A clear must kill the incoming storage without extending userdata lifetime."""
import itertools
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lua_local_versions import nil_clear_targets,split_hoisted_locals
from tools.script_recovery.readable_style import style_source

GC_SOURCE='''function Main()
    local pCVar1, pCVar2, pCVar3
    pCVar1 = make("first")
    pCVar3 = pCVar1
    pCVar1 = make("second")
    pCVar2 = pCVar1
    pCVar1, pCVar2 = nil, nil
    collectgarbage("collect")
    local inside = table.concat(events, ",")
    return pCVar3.tag, inside
end
'''
BRANCH_SOURCE='''function Main(a, count)
    local iVar1, iVar2
    iVar1 = 7
    iVar2 = 8
    if a then
        iVar1, iVar2 = nil, nil
    end
    local first = iVar1
    while count > 0 do
        iVar1 = count
        iVar2 = count + 1
        iVar1, iVar2 = nil, nil
        count = count - 1
    end
    return first, iVar1, iVar2
end
'''
def transform(source,stage):
    if stage=='original':return source
    source,mapping=split_hoisted_locals(source)
    if stage=='styled':source,_=style_source(source)
    return source

@pytest.mark.parametrize('stage',['original','split','styled'])
def test_cleared_temporary_finalizes_while_copied_older_value_remains_usable(stage):
    if stage!='original':assert split_hoisted_locals(GC_SOURCE)[1]
    vm=LuaRuntime(unpack_returned_tuples=True)
    vm.execute('''events={}
function make(tag)
    return setmetatable({tag=tag},{__gc=function(t) events[#events+1]=t.tag end})
end
'''+transform(GC_SOURCE,stage))
    assert vm.globals().Main()==('first','second')
    vm.execute('collectgarbage("collect")')
    assert vm.eval('table.concat(events,",")')=='second,first'

@pytest.mark.parametrize('stage',['split','styled'])
@pytest.mark.parametrize('a,count',list(itertools.product([False,True],range(4))))
def test_branch_merge_and_loop_backedge_read_the_cleared_binding(stage,a,count):
    def run(source):
        vm=LuaRuntime(unpack_returned_tuples=True);vm.execute(source)
        return vm.globals().Main(a,count)
    assert run(transform(BRANCH_SOURCE,stage))==run(BRANCH_SOURCE)

@pytest.mark.parametrize('source',['a,a = nil,nil','a,b = nil','a,b = nil,3',
    '_ENV,b = nil,nil','a,b = f(),nil','a.x,b = nil,nil'])
def test_ambiguous_or_effectful_parallel_store_is_not_a_clear(source):
    assert not nil_clear_targets(source)

def test_distinct_nil_destinations_are_recognized():
    assert nil_clear_targets(' a, b = nil, nil ')=={'a','b'}

def test_captured_clear_binding_remains_pinned():
    source='''function Main()
    local iVar1, iVar2
    iVar1 = 7
    local function read()
        return iVar1
    end
    iVar1, iVar2 = nil, nil
    return read()
end
'''
    output,mapping=split_hoisted_locals(source)
    assert 'iVar1' not in mapping
    vm=LuaRuntime();vm.execute(output);assert vm.globals().Main() is None

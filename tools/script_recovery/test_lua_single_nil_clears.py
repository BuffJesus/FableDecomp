import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lua_local_versions import nil_clear_targets,split_hoisted_locals
from tools.script_recovery.readable_style import style_source

SOURCE='''function Main()
    local pCVar1, pCVar3
    pCVar1 = make("first")
    pCVar3 = pCVar1
    pCVar1 = make("second")
    use(pCVar1)
    pCVar1 = nil
    collectgarbage("collect")
    return pCVar3.tag, table.concat(events, ",")
end
'''
@pytest.mark.parametrize('stage',['original','split','styled'])
def test_single_clear_releases_previously_observed_value_before_return(stage):
    text=SOURCE
    if stage!='original':
        text,mapping=split_hoisted_locals(text);assert mapping
    if stage=='styled':text,_=style_source(text)
    vm=LuaRuntime(unpack_returned_tuples=True)
    vm.execute('''events={}
function make(tag)
 return setmetatable({tag=tag},{__gc=function(t) events[#events+1]=t.tag end})
end
function use(value) assert(value.tag=="second") end
'''+text)
    assert vm.globals().Main()==('first','second')
    vm.execute('collectgarbage("collect")')
    assert vm.eval('table.concat(events,",")')=='second,first'

@pytest.mark.parametrize('text',['a = f()','a = nil, nil','_ENV = nil','a.x = nil','a = nil or f()'])
def test_effectful_environment_or_arity_mismatch_is_not_a_clear(text):
    assert not nil_clear_targets(text)

def test_single_bare_nil_clear_is_recognized():
    assert nil_clear_targets('  a = nil  ')=={'a'}


import pytest
from tools.script_recovery.readable_style import Flow

@pytest.mark.parametrize('body,eligible',[
    ('local value\nvalue = nil\nreturn value',True),
    ('local value\nif enabled then\nvalue = nil\nend\nreturn value',True),
    ('local value\nvalue = make()\nvalue = nil\nreturn value',False),
    ('value = nil\nreturn value',False),
    ('local value = make()\nvalue = nil\nreturn value',False),
    ('local value\nlocal value\nvalue = nil\nreturn value',False),
    ('local value\nlocal function read()\nreturn value\nend\nvalue = nil\nreturn read()',False),
    ('local value\nlocal function write()\nvalue = make()\nend\nwrite()\nvalue = nil\nreturn value',False),
    ('local value\nvalue = nil\nif enabled then value = make() end\nreturn value',False),
    ('local value, other\nvalue = nil\nvalue, other = make(), 2\nreturn value',False),
    ('local value, other\nvalue = nil\nvalue, other = nil, nil\nreturn value',False),
    ('local value, other\nvalue, other = nil, nil\nreturn value',False),
    ('local value\nif enabled then\nvalue = make()\nend\nvalue = nil\nreturn value',False),
    ('local value\nvalue = nil\nvalue = nil\nreturn value',False),
    ('local _ENV\n_ENV = nil\nreturn _ENV',False),
])
def test_only_one_uncaptured_uninitialized_single_destination_is_inert(body,eligible):
    source='function Main()\n'+''.join('    '+line+'\n' for line in body.splitlines())+'end\n'
    flow=Flow(source.splitlines(True))
    if not flow.ok:
        assert not eligible and not flow.inert_nil_initializations
        return
    assert bool(flow.inert_nil_initializations)==eligible
    if eligible:assert 'value' not in flow.pinned

def test_parameter_clear_has_a_preexisting_value_to_retire():
    source='function Main(value)\n    value = nil\n    return value\nend\n'
    flow=Flow(source.splitlines(True));assert flow.ok
    assert not flow.inert_nil_initializations and 'value' in flow.pinned


from tools.script_recovery.build_readable_unit import readable_file

@pytest.mark.parametrize('styled',[False,True])
def test_full_readable_pipeline_releases_the_observed_old_value(styled):
    text=readable_file(SOURCE,style=styled)[0]
    vm=LuaRuntime(unpack_returned_tuples=True)
    vm.execute("""events={}
function make(tag) return setmetatable({tag=tag},{__gc=function(t) events[#events+1]=t.tag end}) end
function use(value) assert(value.tag=='second') end
"""+text)
    assert vm.globals().Main()==('first','second')

LOOP_SOURCE="""function Main(clear, count)
    local pCVar1, iVar2
    iVar2 = 0
    while iVar2 < count do
        pCVar1 = make('value' .. iVar2)
        use(pCVar1)
        if clear then
            pCVar1 = nil
        end
        collectgarbage('collect')
        iVar2 = iVar2 + 1
    end
    return table.concat(events, ','), iVar2
end
"""
@pytest.mark.parametrize('stage',['split','styled','unstyled_file','styled_file'])
@pytest.mark.parametrize('clear',[False,True])
@pytest.mark.parametrize('count',range(5))
def test_single_clear_branch_and_backedge_preserve_collection_time(stage,clear,count):
    if stage in ['split','styled']:
        text,_=split_hoisted_locals(LOOP_SOURCE)
        if stage=='styled':text,_=style_source(text)
    else:text=readable_file(LOOP_SOURCE,style=stage=='styled_file')[0]
    def run(chunk):
        vm=LuaRuntime(unpack_returned_tuples=True)
        vm.execute("""collectgarbage('stop')
events={}
function make(tag) return setmetatable({tag=tag},{__gc=function(t) events[#events+1]=t.tag end}) end
function use(value) assert(type(value.tag)=='string') end
"""+chunk)
        result=vm.globals().Main(clear,count)
        vm.execute("collectgarbage('collect')")
        return result,vm.eval("table.concat(events,',')")
    assert run(text)==run(LOOP_SOURCE)

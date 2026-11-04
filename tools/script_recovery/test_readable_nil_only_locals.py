from pathlib import Path
import sys
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.readable_nil_only_locals import prune

def run(source,flag=False):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute('events={}; function record(x) events[#events+1]=x end; global=41')
    lua.execute(source)
    result=lua.globals().Main(flag)
    return result,lua.eval("table.concat(events,',')"),lua.globals()['global']

@pytest.mark.parametrize('separator',[',',', ', ' , ', ',\t'])
@pytest.mark.parametrize('stores',[1,3])
def test_unused_nil_slot_removed_from_parallel_writes(separator,stores):
    source='function Main(flag)\n    local dead'+separator+'live\n    live = 7\n'
    source+=('    if flag then\n        dead'+separator+'live = nil'+separator+'nil\n    end\n')*stores
    source+='    return live\nend\n'
    out,proof=prune(source)
    assert [r['name'] for r in proof]==['dead'] and 'dead' not in out
    assert 'live = nil' in out and all(run(source,v)==run(out,v) for v in [False,True])

def test_only_nil_stores_with_no_reads_disappear_without_changing_events():
    source='function Main(flag)\n    local dead\n    record("before")\n    dead = nil\n    if flag then\n        dead = nil\n    end\n    record("after")\n    return 3\nend\n'
    out,proof=prune(source)
    assert len(proof)==1 and 'dead' not in out
    assert all(run(source,v)==run(out,v) for v in [False,True])

@pytest.mark.parametrize('body',[
    'dead = {}\n    dead = nil',
    'if flag then\n        dead = {}\n    end\n    dead = nil',
    'dead = nil\n    record(dead)',
    'dead = nil\n    if flag then record(dead) end',
    'dead = nil\n    local function capture()\n        return dead\n    end\n    record(capture())',
    'dead = nil\n    local t = {dead=2}\n    record(t.dead)',
    'dead = nil\n    for dead=1,2 do record(dead) end',
])
def test_reads_nonnil_writes_capture_keys_or_shadowing_preserved(body):
    source='function Main(flag)\n    local dead\n    '+body+'\n    return 1\nend\n'
    out,proof=prune(source)
    assert out==source and proof==[]
    assert all(run(source,v)==run(out,v) for v in [False,True])

def test_populated_resource_clear_and_gc_order_are_retained():
    source='''function Main(flag)
    local dead, resource
    resource = setmetatable({}, {__gc=function() record('collected') end})
    dead, resource = nil, nil
    collectgarbage('collect')
    record('after')
    return 7
end
'''
    out,proof=prune(source)
    assert [r['name'] for r in proof]==['dead']
    assert 'resource = nil' in out and run(out)==run(source)==(7,'collected,after',41)

@pytest.mark.parametrize('source',[
    'function Main(flag)\n    local dead,\n        live\n    dead, live = nil, nil\n    return 1\nend\n',
    'function Main(flag)\n    local dead\n        = 7\n    dead = nil\n    return 1\nend\n',
    'function Main(flag)\n    local dead\n    dead = nil\n        or {}\n    return 1\nend\n',
    'function Main(flag)\n    local dead, live\n    dead, live = nil, nil\n        , record("extra")\n    return 1\nend\n',
    'function Main(flag)\n    local dead -- annotation\n    dead = nil\n    return 1\nend\n',
    'function Main(flag)\n    local dead\n    dead = nil -- annotation\n    return 1\nend\n',
    'function Main(flag)\n    local dead\n    local dead\n    dead = nil\n    return 1\nend\n',
    'function Main(flag)\n    local dead\n    dead, dead = nil, nil\n    return 1\nend\n',
    'function Main(dead)\n    local dead\n    dead = nil\n    return 1\nend\n',
    'function Main(flag)\n    dead = nil\n    local dead\n    dead = nil\n    return 1\nend\n',
    'function Main(flag)\n    if flag then\n        local dead\n    end\n    dead = nil\n    return 1\nend\n',
    'function Main(flag)\n    local dead <close> = nil\n    return 1\nend\n',
])
def test_unsupported_declarations_stores_or_binding_scope_stay_verbatim(source):
    out,proof=prune(source)
    assert out==source and proof==[]
    assert all(run(source,v)==run(out,v) for v in [False,True])

@pytest.mark.parametrize('name',['debug','_ENV','getfenv','setfenv','load','loadstring'])
def test_environment_and_introspection_are_outside_scope(name):
    source='function Main(flag)\n    local dead\n    dead = nil\n    return '+name+'\nend\n'
    assert prune(source)==(source,[])

@pytest.mark.parametrize('literal',['"local dead; dead = nil"',"'dead'",'[=[\nlocal dead\ndead = nil\n]=]'])
def test_strings_are_opaque(literal):
    source='function Main(flag)\n    local dead\n    dead = nil\n    return '+literal+'\nend\n'
    out,proof=prune(source)
    assert literal in out and run(out)==run(source)
    if '\n' in literal:assert out==source and proof==[]

def test_pruned_parallel_store_is_idempotent():
    source='function Main(flag)\n    local dead, live\n    dead, live = nil, nil\n    return live\nend\n'
    out,proof=prune(source)
    assert len(proof)==1 and prune(out)==(out,[])


@pytest.mark.parametrize('style',[False,True])
def test_complete_readable_pipeline_retains_populated_gc_clear(style):
    from tools.script_recovery.build_readable_unit import readable_file
    source="""function Main(flag)
    local dead, resource
    resource = setmetatable({}, {__gc=function() record('collected') end})
    dead, resource = nil, nil
    collectgarbage('collect')
    record('after')
    return 7
end
"""
    out,report=readable_file(source,style=style)
    assert run(out)==run(source)==(7,'collected,after',41)
    assert 'resource = nil' in out
    assert len(report['removedNilOnlyLocals']['Main'])==1

"""Preserve scalar stores on compound lines without losing supported rewrites."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import readable_style as style
from tools.script_recovery.lua_local_versions import compound_scalar_stores, split_hoisted_locals
from tools.script_recovery.lua_literal_values import inline_literal_locals
from tools.script_recovery.readable_lua import readable_source

def run(source,flag):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source)
 return lua.execute('local q={flag=...,events={}};function q:Record(x)self.events[#self.events+1]=tostring(x)end;local v=Main(q);return v,table.concat(q.events,",")',flag)

@pytest.mark.parametrize('statement',[
 'q:Record(r); r = 1',
 'q:Record(r); r = 1; q:Record(r)',
 'q:Record(r); r, other = 1, 2',
 'r = 2; r = 1',
 'if q.flag then r = 1; return r end',
 'if q.flag then q:Record(r); r = 1; return r end',
])
def test_store_destination_and_later_reads_preserved(statement):
 source='function Main(q)\n    local r, other\n    r = 0\n    '+statement+'\n    return r\nend\n'
 assert "r" in style.Flow(source.splitlines(True)).pinned
 literals,n=style.propagate_literals(source.splitlines(True));assert ''.join(literals)==source and n==0
 versions,info=split_hoisted_locals(source);assert versions==source and not info
 styled,stats=style.style_function(source,set())
 for flag in [False,True]:assert run(styled,flag)==run(source,flag)

@pytest.mark.parametrize('statement',[
 'record(r); r = 1',
 'record(r); r, other = 1, 2',
])
def test_cleanup_closure_keeps_inline_store(statement):
 source='function Main(q)\n    local r, other\n    local function cleanup()\n        r = 0\n        '+statement.replace('record(', 'q:Record(')+'\n        q:Record(r)\n    end\n    cleanup()\n    return r\nend\n'
 result,n=style.tidy_closures(source.splitlines(True));assert ''.join(result)==source and n==0
 assert run(''.join(result),True)==run(source,True)

@pytest.mark.parametrize('fake',[
 'q:Record("; r = 1")',
 'q:Record([[then r = 1]])',
 'q:Record(r) -- ; r = 1',
 'q:Record(r) --[[ else r = 1 ]]',
])
def test_strings_and_comments_do_not_disable_supported_flow(fake):
 source='function Main(q)\n    local r\n    r = 0\n    '+fake+'\n    return r\nend\n'
 assert style.Flow(source.splitlines(True)).ok
 result,n=style.propagate_literals(source.splitlines(True));assert n>=1
 assert run(''.join(result),True)==run(source,True)

def test_supported_one_statement_flow_still_inlines():
 source='function Main(q)\n    local r\n    r = 0\n    q:Record(r)\n    return r\nend\n'
 assert style.Flow(source.splitlines(True)).ok
 result,n=style.propagate_literals(source.splitlines(True));assert n==2
 assert run(''.join(result),True)==run(source,True)

def test_copy_propagation_and_local_versioning_preserve_reassignment():
 source='function Main(q)\n    local r, other\n    other = 7\n    r = other\n    q:Record(r); r = 1\n    return r\nend\n'
 result,n=style.propagate_copies(source.splitlines(True));assert ''.join(result)==source and n==0
 out,mapping=readable_source(source,inline_literals=True)
 for flag in [False,True]:assert run(out,flag)==run(source,flag)


def execute(source):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source)
 return lua.execute('local q={events={}};function q:Record(v)self.events[#self.events+1]=tostring(v)end;local a,b=Main(q);return a,b,table.concat(q.events,",")')

@pytest.mark.parametrize('value',['nil','false','true','0','-2','7.5'])
def test_guarded_destination_keeps_storage_other_literal_still_inlines(value):
 source=f'function Main(q)\n    local r1, r2\n    r1 = {value}\n    r2 = 7\n    q:Record(r2)\n    q:Record(r1);r1 = 3\n    return r1\nend\n'
 out,n=style.propagate_literals(source.splitlines(True));out=''.join(out)
 assert n==1 and 'q:Record(7)'in out and 'q:Record(r1);r1 = 3'in out
 assert execute(out)==execute(source)
 out,report=inline_literal_locals(source)
 assert 'r1'not in report and 'r2'in report and execute(out)==execute(source)

def test_local_version_split_preserves_inline_mutation_and_splits_other_reuse():
 source='function Main(q)\n    local r1, r2\n    r1 = 0\n    r2 = 1\n    q:Record(r2)\n    r2 = 2\n    q:Record(r2)\n    q:Record(r1); r1 = 3\n    return r1\nend\n'
 out,report=split_hoisted_locals(source)
 assert 'r1'not in report and 'r2'in report
 assert 'q:Record(r1); r1 = 3'in out and execute(out)==execute(source)

def test_unrelated_closure_literal_can_be_removed_without_rewriting_later_store():
 source='function Main(q)\n    local r1, r2\n    local function cleanup()\n        r1 = 0\n        r2 = 7\n        q:Record(r2)\n        q:Record(r1); r1 = 3\n    end\n    cleanup()\n    return r1\nend\n'
 out,n=style.tidy_closures(source.splitlines(True));out=''.join(out)
 assert n>=1 and 'q:Record(7)'in out and 'q:Record(r1); r1 = 3'in out
 assert execute(out)==execute(source)


@pytest.mark.parametrize('name',['doneIntro','doneToutIntro','double','domain','thenValue','elseCount','elsewhere'])
def test_identifier_prefix_is_not_an_inline_control_keyword(name):
 source=f'function Main(q)\n    local {name}\n    {name} = 1\n    q:Record({name})\n    return {name}\nend\n'
 assert not compound_scalar_stores(style._structure(source.splitlines(True)))
 flow=style.Flow(source.splitlines(True));assert flow.ok and name not in flow.pinned
 out,count=style.propagate_literals(source.splitlines(True));assert count==2
 assert 'q:Record(1)'in ''.join(out)

@pytest.mark.parametrize('name',['doneIntro','thenValue','elseCount'])
def test_same_names_after_a_real_semicolon_are_preserved(name):
 source=f'function Main(q)\n    local {name}\n    {name} = 1\n    q:Record({name}); {name} = 2\n    return {name}\nend\n'
 assert compound_scalar_stores(style._structure(source.splitlines(True)))=={name}
 out,count=style.propagate_literals(source.splitlines(True));assert count==0 and ''.join(out)==source

def test_read_only_cleanup_capture_keeps_existing_copy_optimization():
 source='function Main(resources)\n    local resource, owner\n    local function close()\n        resources:ReleaseResource(resource)\n    end\n    owner = resources:NewResource()\n    resource = owner\n    resources:ReleaseResource(resource)\n    close()\nend\n'
 result,count=style.propagate_copies(source.splitlines(True));assert count==1
 assert '    resources:ReleaseResource(owner)\n'in ''.join(result)
 assert '        resources:ReleaseResource(resource)\n'in ''.join(result)


def observe(source,flag):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(source)
 return lua.execute('local q={flag=...,events={}};function q:Record(v)self.events[#self.events+1]=tostring(v)end;local result=Main(q);return result,table.concat(q.events,",")',flag)

@pytest.mark.parametrize('statement',[
 'q:Record(r); r = 1',
 'q:Record(8); r = 1',
 'if q.flag then r = 1; return r end',
])
def test_single_use_never_replaces_compound_destination(statement):
 source='function Main(q)\n    local r\n    r = 0\n    '+statement+'\n    return r\nend\n'
 output,count=style.inline_single_use(source.splitlines(True));output=''.join(output)
 for flag in [False,True]:assert observe(output,flag)==observe(source,flag)

def test_earlier_condition_use_can_inline_before_later_compound_store():
 source='function Main(q)\n    local r\n    r = q.flag\n    if r then\n        q:Record(9)\n    end\n    r = 3\n    q:Record(r); r = 1\n    return r\nend\n'
 output,count=style.inline_single_use(source.splitlines(True));output=''.join(output)
 assert count==1 and 'if q.flag then' in output
 for flag in [False,True]:assert observe(output,flag)==observe(source,flag)

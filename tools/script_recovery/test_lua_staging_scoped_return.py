import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import lua_staging_liveness as P

BASE='''function Main(q)
    local value
    value = nil
    do return end
    q:Use(value)
end
'''
def prove(source):
 LuaRuntime().compile(source)
 return P.prove_unused_staging(source,source.index('value = nil'),'value')

def test_exact_bare_scoped_return_stops_taint_and_really_exits_lua():
 result=prove(BASE);assert result['assignmentLine']==3 and result['visitedNodes']==1
 vm=LuaRuntime();vm.execute(BASE);q=vm.eval('{Use=function() error("unreachable value use") end}')
 assert vm.globals().Main(q) is None

@pytest.mark.parametrize('body',[
 'q:Use(value)\n    do return end',
 'if q:Cond() then\n        do return end\n    end\n    q:Use(value)',
 'if q:Cond() then goto live end\n    do return end\n    ::live::\n    q:Use(value)',
 'while q:Cond() do\n        do return end\n    end\n    q:Use(value)',
 'local copy = value\n    q:Use(copy)\n    do return end',
])
def test_return_on_one_path_cannot_hide_a_live_read(body):
 source=BASE[:BASE.index('    do return end')]+body+'\nend\n'
 with pytest.raises(ValueError,match='live staging value read/escape'):prove(source)

@pytest.mark.parametrize('body',[
 'do return value end',
 'do\n        return\n    end',
 'do local other = value; return end',
 'do return end q:Use(value)',
])
def test_other_do_forms_are_still_unmodeled(body):
 source=BASE[:BASE.index('    do return end')]+body+'\nend\n'
 with pytest.raises(ValueError):P.prove_unused_staging(source,source.index('value = nil'),'value')

@pytest.mark.parametrize('body',[
 '-- do return end\n    q:Use(value)',
 'local text = "do return end"\n    q:Use(value)',
 '--[[ do return end ]]\n    q:Use(value)',
])
def test_comments_and_strings_cannot_create_a_false_return_edge(body):
 source=BASE[:BASE.index('    do return end')]+body+'\nend\n'
 with pytest.raises(ValueError,match='live staging value read/escape'):prove(source)

def test_trailing_comment_does_not_change_a_real_bare_return():
 result=prove(BASE.replace('do return end','do return end -- native terminal scope'))
 assert result['visitedNodes']==1

def test_loop_false_path_and_overwrite_are_retained():
 source=BASE.replace('    do return end\n','    while q:Cond() do\n        do return end\n    end\n    value = 4\n')
 assert prove(source)['visitedNodes']>=2


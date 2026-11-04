import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_unit import tidy_declarations,readable_file


def execute(source):
    lua=LuaRuntime(unpack_returned_tuples=True)
    return lua.execute('alpha=99;beta=88\n'+source+'''\nlocal a,b=Main();if type(a)=='function' then a,b=a() end;return a,b,alpha,beta''')


@pytest.mark.parametrize('declaration',[
    'local alpha,beta','local alpha, beta','local alpha , beta','local alpha,\tbeta',
    'local alpha ,beta','local alpha,beta -- lexical locals','local alpha ,\t beta',
    'local alpha,\n        beta','local alpha\n        , beta',
    'local alpha,\n        -- continued list\n        beta',
])
@pytest.mark.parametrize('stage',['direct','unstyled','styled'])
def test_declaration_whitespace_retains_bindings(declaration,stage):
    source='function Main()\n    '+declaration+'\n    alpha = 7\n    beta = 9\n    return alpha, beta\nend\n'
    after=tidy_declarations(source)[0] if stage=='direct' else readable_file(source,style=stage=='styled')[0]
    assert execute(after)==execute(source)


@pytest.mark.parametrize('declaration',[
    'local alpha,beta\n        = 7, 9',
    'local alpha, beta\n        -- initialize both\n        = 7, 9',
    'local alpha\n        , beta = 7, 9',
    'local alpha,\n        beta = 7, 9',
])
@pytest.mark.parametrize('stage',['direct','unstyled','styled'])
def test_multiline_initializer_cannot_become_declaration_only(declaration,stage):
    source='function Main()\n    '+declaration+'\n    return beta\nend\n'
    after=tidy_declarations(source)[0] if stage=='direct' else readable_file(source,style=stage=='styled')[0]
    assert execute(after)==execute(source)


@pytest.mark.parametrize('source',[
    'local text=[=[\nlocal alpha,beta\n]=]\n',
    '--[[\nlocal alpha,beta\n]]\n',
    'local alpha, beta -- annotation\n',
])
def test_opaque_or_annotated_declarations_stay_verbatim(source):
    assert tidy_declarations(source)==(source,0)


@pytest.mark.parametrize('separator',[',',', ', ' , ', ',\t', ' ,'])
@pytest.mark.parametrize('capture',[False,True])
@pytest.mark.parametrize('stage',['direct','unstyled','styled'])
def test_unused_and_captured_names_keep_lexical_scope(separator,capture,stage):
    if capture:
        body='    local function callback()\n        return alpha, beta\n    end\n    alpha = 7\n    beta = 9\n    return callback\n'
    else:
        body='    beta = 9\n    return beta\n'
    source='function Main()\n    local alpha'+separator+'beta\n'+body+'end\n'
    after=tidy_declarations(source)[0] if stage=='direct' else readable_file(source,style=stage=='styled')[0]
    assert execute(after)==execute(source)


def test_continued_declaration_reports_conservative_preservation():
    source='function Main()\n    local alpha\n        , beta\n    alpha = 7\n    return alpha\nend\n'
    after,report=readable_file(source)
    assert after==source and report['preservedMultilineDeclaration']


@pytest.mark.parametrize('separator',[',',', ', ' , ', ',\t', ' ,'])
@pytest.mark.parametrize('stage',['direct','unstyled','styled'])
@pytest.mark.parametrize('initialize',[False,True])
def test_environment_is_an_implicit_local_dependency(separator,stage,initialize):
    source='function Main()\n    local _ENV'+separator+'unused\n'
    if initialize:source+='    _ENV = {outside=9}\n'
    source+='    return outside\nend\n'
    after=tidy_declarations(source)[0] if stage=='direct' else readable_file(source,style=stage=='styled')[0]
    def run(text):
        lua=LuaRuntime(unpack_returned_tuples=True)
        return lua.execute('outside=7\n'+text+'\nlocal ok,value=pcall(Main);return ok,ok and value or tostring(value):match("attempt to index a nil value")~=nil')
    assert run(after)==run(source)


def test_environment_preservation_is_reported():
    source='function Main()\n    local _ENV = {outside=9}\n    return outside\nend\n'
    after,report=readable_file(source)
    assert after==source and report['preservedEnvironment']

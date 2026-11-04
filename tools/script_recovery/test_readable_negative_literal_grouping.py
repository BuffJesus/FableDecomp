"""Literal staging must retain unary precedence and lexical token boundaries."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import readable_style as style
from tools.script_recovery.build_readable_unit import readable_file

def execute(source,flag):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute(source)
    return lua.globals().Main(flag)

def outcome(source,flag):
    try:
        return 'return',execute(source,flag)
    except Exception as error:
        # Source locations and this exact Lua local-name diagnostic change
        # when literal staging disappears; retain every other message detail.
        message=str(error).split('\n',1)[0].split(': ',1)[-1]
        if message == "number (local 'value') has no integer representation":
            message = 'number has no integer representation'
        return 'error',type(error).__name__,message

@pytest.mark.parametrize('first,second',[('-2','-3'),('-2.5','-0.5'),('-0.0','-1.0')])
@pytest.mark.parametrize('expression',[
    'value ^ 2','value ^ -2','-value ^ 2','value^2','7-value',
    'value.."x"','value .. value','(value + 1) ^ 2','2 ^ value',
    'value * value','value // 2','value & 3',
])
@pytest.mark.parametrize('stage',['literal_pass','style','readable'])
def test_negative_literal_read_grouping(first,second,expression,stage):
    source=('function Main(flag)\n    local value\n    if flag then\n'
            '        value = '+first+'\n        return '+expression+', value\n'
            '    else\n        value = '+second+'\n        return '+expression+', value\n'
            '    end\nend\n')
    if stage=='literal_pass':after=''.join(style.propagate_literals(source.splitlines(True))[0])
    elif stage=='style':after=style.style_source(source)[0]
    else:after=readable_file(source)[0]
    for flag in (False,True):
        assert outcome(after,flag)==outcome(source,flag),after

@pytest.mark.parametrize('stage',['literal_pass','style','readable'])
def test_negative_zero_sign_preserved(stage):
    source='function Main(flag)\n    local value\n    value = -0.0\n    return 1 / (value ^ 2), 1 / value\nend\n'
    if stage=='literal_pass':after=''.join(style.propagate_literals(source.splitlines(True))[0])
    elif stage=='style':after=style.style_source(source)[0]
    else:after=readable_file(source)[0]
    assert execute(after,False)==execute(source,False)

@pytest.mark.parametrize('value',['0','1','2.5','true','false','nil'])
def test_other_literal_replacements_unchanged(value):
    source='function Main()\n    local value\n    value = '+value+'\n    return value, value\nend\n'
    after=''.join(style.propagate_literals(source.splitlines(True))[0])
    assert 'return '+value+', '+value in after

def test_literals_and_comments_are_opaque():
    source='function Main()\n    local value\n    value = -2\n    return "value^2", value ^ 2, value -- value^2\nend\n'
    after=''.join(style.propagate_literals(source.splitlines(True))[0])
    assert '"value^2"' in after and '-- value^2' in after
    assert execute(after,False)==execute(source,False)

@pytest.mark.parametrize('value',['-2147483648','-9223372036854775808','-0.0','-2.5'])
@pytest.mark.parametrize('callback',[False,True])
def test_integer_bounds_and_returned_resource_callback(value,callback):
    body=('    local value\n    if q.flag then\n        value = '+value+'\n'
          '        return value ^ 2, value\n    else\n        value = -3\n'
          '        return value ^ 2, value\n    end\n')
    source='function Main(q)\n'+body+'end\n'
    if callback:
        source='function Main(q)\n    return q:WithRetailResources(function(resources)\n'+body+'    end)\nend\n'
    after,_=readable_file(source)
    def run(text,flag):
        lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(text)
        return lua.execute('local q={flag=...};function q:WithRetailResources(fn)return fn({})end;return Main(q)',flag)
    for flag in (False,True):
        assert run(after,flag)==run(source,flag),after

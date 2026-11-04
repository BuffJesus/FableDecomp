"""Function-shaped literals/comments must never enter callback conversion."""
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import readable_callback_scopes as candidate

def wrapper(name='Fake'):
    return ('function '+name+'(q)\n    return q:WithRetailResources(function(resources)\n'
            '    local value = 1\n    return value\n    end)\nend\n')

@pytest.mark.parametrize('equals',['','=', '==','===='])
@pytest.mark.parametrize('comment',[False,True])
@pytest.mark.parametrize('real_position',['before','after','both','none'])
def test_noncode_wrappers_never_converted(equals,comment,real_position):
    literal = ('--' if comment else 'local literal = ')+ '['+equals+'[\n'+wrapper()+ 'function Tail()\nend\n]'+equals+']\n'
    source = (wrapper('Before') if real_position in ('before','both') else '')+literal+(wrapper('After') if real_position in ('after','both') else '')
    calls=[]
    def convert(text,**options):
        calls.append(text.split('(')[0].split()[-1])
        return text,{}
    after,reports=candidate.rewrite(source,convert)
    assert literal in after
    # The existing sole-wrapper contract rejects a wrapper followed by an
    # outer statement/comment in its chunk. Do not broaden that contract.
    expected=['After'] if real_position in ('after','both') else []
    if real_position in ('before','both'):
        assert wrapper('Before') in after
    assert calls==expected and list(reports)==expected
    lua=LuaRuntime();lua.execute(after)
    if not comment:
        before_value=LuaRuntime().execute(source+'return literal')
        after_value=LuaRuntime().execute(after+'return literal')
        assert before_value==after_value

@pytest.mark.parametrize('quote',['"',"'"])
def test_escaped_newline_short_string_preserved(quote):
    body = wrapper()+'function Tail()\nend\n'
    # Lua short strings permit escaped literal newlines.
    source='local literal = '+quote+body.replace('\n','\\\n')+quote+'\n'+wrapper('Real')
    after,reports=candidate.rewrite(source,lambda text,**_: (text,{}))
    assert list(reports)==['Real']
    assert LuaRuntime().execute(source+'return literal')==LuaRuntime().execute(after+'return literal')

def test_plain_callback_still_rewritten():
    source=wrapper('Main')
    after,reports=candidate.rewrite(source,lambda text,**_: (text,{}))
    assert after!=source and list(reports)==['Main']

def test_no_function_input_exact():
    source='local x = "function Fake(q)"\n-- function Main(q)\n'
    assert candidate.rewrite(source,lambda text,**_: (text,{}))==(source,{})

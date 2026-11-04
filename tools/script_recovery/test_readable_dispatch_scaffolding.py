import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.readable_dispatch_scaffolding import prune_unused_dispatch_seeds


def pair(offset='4',name='scratch',native='CVar12',local=False):
    return ('    -- TODO(native): '+native+' = *(this + '+offset+')\n'
            '    '+('local ' if local else '')+name+' = nil --[[unresolved native value]]\n')


@pytest.mark.parametrize('entity,offset',[(True,'4'),(True,'0x4'),(False,'64'),(False,'0x40')])
def test_unread_private_dispatch_seed_retires(entity,offset):
    source='function Main(quest)\n    local scratch, other\n'+pair(offset)+'    return 17\nend\n'
    output,evidence=prune_unused_dispatch_seeds(source,entity=entity)
    assert len(evidence)==1 and evidence[0]['offset']==int(offset,0)
    assert '-- TODO(native)' not in output and 'unresolved native value' not in output
    lua=LuaRuntime()
    for text in (source,output):
        lua.execute(text)
        assert lua.globals().Main(lua.table())==17


def test_a_private_local_nil_initializer_can_retire():
    source='function Main()\n'+pair(local=True)+'end\n'
    assert len(prune_unused_dispatch_seeds(source,entity=True)[1])==1


@pytest.mark.parametrize('extra',[
    '    return scratch\n', '    Consume(scratch)\n',
    '    scratch = quest:GetStateInt("Value")\n',
    '    local function callback() return scratch end\n',
    '    -- TODO(native): consume(scratch)\n',
    '    local label = "scratch"\n',
    '    local scratch\n',
    '    local scratch = 9\n',
])
def test_read_capture_other_store_shadow_or_diagnostic_keeps_marker(extra):
    source='function Main(quest)\n    local scratch\n'+pair()+extra+'end\n'
    assert prune_unused_dispatch_seeds(source,entity=True)==(source,[])


@pytest.mark.parametrize('declaration',['','    local other\n'])
def test_global_assignment_is_never_retired(declaration):
    source='function Main()\n'+declaration+pair()+'end\n'
    assert prune_unused_dispatch_seeds(source,entity=True)==(source,[])


@pytest.mark.parametrize('prefix,suffix',[
    ('','    local scratch\n'),
    ('    if true then\n        local scratch\n    end\n',''),
    ('    local scratch, scratch\n',''),
])
def test_late_out_of_scope_or_duplicate_declarations_are_not_ownership(prefix,suffix):
    source='function Main()\n'+prefix+pair()+suffix+'end\n'
    assert prune_unused_dispatch_seeds(source,entity=True)==(source,[])


def test_parameter_is_not_an_unread_private_local():
    source='function Main(scratch)\n'+pair()+'end\n'
    assert prune_unused_dispatch_seeds(source,entity=True)==(source,[])


@pytest.mark.parametrize('entity,offset',[(False,'4'),(True,'0x40'),(None,'4'),('entity','4'),(True,'8')])
def test_unknown_owner_or_other_pointer_field_is_preserved(entity,offset):
    source='function Main()\n    local scratch\n'+pair(offset)+'end\n'
    assert prune_unused_dispatch_seeds(source,entity=entity)==(source,[])


@pytest.mark.parametrize('wrapper',[lambda text:'local text=[=[\n'+text+']=]\n',
                                    lambda text:'--[=[\n'+text+']=]\n'])
def test_text_and_long_comments_never_form_dispatch_evidence(wrapper):
    source='function Main()\n    local scratch\n'+wrapper(pair())+'end\n'
    assert prune_unused_dispatch_seeds(source,entity=True)==(source,[])


def test_local_declaration_hidden_in_long_text_does_not_own_a_global():
    source='function Main()\n'+pair()+'    local data=[=[\n    local scratch\n]=]\nend\n'
    assert prune_unused_dispatch_seeds(source,entity=True)==(source,[])


def test_independent_private_bindings_and_repeated_stores_are_supported():
    source=('function Main()\n    local first, second\n'+pair(name='first')+pair(name='second')+
            pair(name='first')+'    return 9\nend\n')
    output,evidence=prune_unused_dispatch_seeds(source,entity=True)
    assert len(evidence)==3 and '-- TODO(native)' not in output


def test_a_later_private_declaration_keeps_token_offsets_correct():
    source=('function Main()\n'+pair(local=True)+'    local second\n'+pair(name='second')+'end\n')
    assert len(prune_unused_dispatch_seeds(source,entity=True)[1])==2


def test_other_unresolved_native_values_are_never_suppressed():
    source='function Main()\n    local scratch\n    -- TODO(native): scratch = Unknown()\n    scratch = nil --[[unresolved native value]]\nend\n'
    assert prune_unused_dispatch_seeds(source,entity=True)==(source,[])

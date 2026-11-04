import pytest

from tools.script_recovery.annotate_interface_slots import annotate

CALL = 'pCVar10 = (**(code **)((int)*pCVar11 + 0x118))(pCVar11);'
LOAD = 'pCVar11 = *(int **)(this + 4);'


@pytest.mark.parametrize('owner,offset', [('this','4'),('this','0x40'),('param_1','4')])
@pytest.mark.parametrize('between', ['', '\ngsivt3 = **(int **)(this + 4);'])
def test_cast_vtable_keeps_proven_interface_receiver(owner, offset, between):
    source=LOAD.replace('this + 4',owner+' + '+offset)+between+'\n'+CALL
    result=annotate(source,{0x118:'GetHero'})
    assert result==source.replace('(**(code **)((int)*pCVar11 + 0x118))','GSI->GetHero')


@pytest.mark.parametrize('load,between,call', [
    ('', '', CALL),
    ('pCVar11 = other;', '', CALL),
    ('pCVar11 = GSI->GetHero();', '', CALL),
    (LOAD.replace('+ 4','+ 8'), '', CALL),
    (LOAD.replace('this','other'), '', CALL),
    (LOAD.replace('= *(', '= **('), '', CALL),
    (LOAD, '\npCVar11 = other;', CALL),
    (LOAD, '\n}\n', CALL),
    (LOAD, '\nif (ready) {', CALL),
    (LOAD, '\nLAB_123:', CALL),
    (LOAD, '\ncallback();', CALL),
    (LOAD, '', CALL.replace('))(pCVar11)', '))(other)')),
    (LOAD, '', CALL.replace('0x118', '0x999')),
])
def test_unproven_or_redefined_receivers_remain_unresolved(load,between,call):
    source=load+between+'\n'+call
    assert annotate(source,{0x118:'GetHero'})==source


def test_each_branch_requires_its_own_load_and_preserves_following_string_use():
    source='if (first) {\n'+LOAD+'\n'+CALL+'\npCVar11 = &xStack_f0;\n} else {\n'+LOAD+'\n'+CALL+'\npCVar11 = &xStack_f8;\n}'
    result=annotate(source,{0x118:'GetHero'})
    assert result.count('GSI->GetHero(pCVar11)')==2
    assert 'pCVar11 = &xStack_f0;' in result and 'pCVar11 = &xStack_f8;' in result
